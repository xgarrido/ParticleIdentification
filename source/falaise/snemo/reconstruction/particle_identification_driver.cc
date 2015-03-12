/// \file falaise/snemo/reconstruction/particle_identification_driver.cc

// Ourselves:
#include <snemo/reconstruction/particle_identification_driver.h>

// Third party:
// - Bayeux/cuts:
#include <bayeux/cuts/cut_manager.h>

// This project:
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/pid_utils.h>

namespace snemo {

  namespace reconstruction {

    const std::string & particle_identification_driver::particle_identification_id()
    {
      static const std::string _id("PID");
      return _id;
    }

    void particle_identification_driver::set_initialized(const bool initialized_)
    {
      _initialized_ = initialized_;
      return;
    }

    bool particle_identification_driver::is_initialized() const
    {
      return _initialized_;
    }

    void particle_identification_driver::set_logging_priority(const datatools::logger::priority priority_)
    {
      _logging_priority_ = priority_;
      return;
    }

    datatools::logger::priority particle_identification_driver::get_logging_priority() const
    {
      return _logging_priority_;
    }

    bool particle_identification_driver::has_cut_manager() const
    {
      return _cut_manager_ != 0;
    }

    void particle_identification_driver::set_cut_manager(cuts::cut_manager & cmgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Driver is already initialized !");
      _cut_manager_ = &cmgr_;
      return;
    }

    const cuts::cut_manager & particle_identification_driver::get_cut_manager() const
    {
      DT_THROW_IF(! has_cut_manager(), std::logic_error,
                  "No cuts manager is setup !");
      return *_cut_manager_;
    }

    cuts::cut_manager & particle_identification_driver::grab_cut_manager()
    {
      DT_THROW_IF(! has_cut_manager(), std::logic_error,
                  "No cuts manager is setup !");
      return *_cut_manager_;
    }

    // Constructor
    particle_identification_driver::particle_identification_driver()
    {
      _set_defaults();
      set_initialized(false);
      return;
    }

    // Destructor
    particle_identification_driver::~particle_identification_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    // Initialize the gamma tracker through configuration properties
    void particle_identification_driver::initialize(const datatools::properties & setup_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Driver '" << particle_identification_id() << "' is already initialized !");

      DT_THROW_IF(! has_cut_manager(), std::logic_error, "Missing cut manager !");
      DT_THROW_IF(! get_cut_manager().is_initialized(), std::logic_error,
                  "Cut manager is not initialized !");

      // Logging priority
      datatools::logger::priority lp = datatools::logger::extract_logging_configuration(setup_);
      DT_THROW_IF(lp == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority level for geometry manager !");
      set_logging_priority(lp);

      // Fetch PID definition
      DT_THROW_IF(!setup_.has_key("pid.definitions"), std::logic_error,
                  "Missing definitions of particles !");
      std::vector<std::string> pid_definitions;
      setup_.fetch("pid.definitions", pid_definitions);
      for (size_t i = 0; i < pid_definitions.size(); ++i) {
        const std::string & key = pid_definitions.at(i);
        std::string str;
        if (setup_.has_key(str = "pid." + key + ".label")) {
          const std::string value = setup_.fetch_string(str);
          _pid_properties_.insert(std::make_pair(key, value));
        }
      }

      set_initialized(true);
      return;
    }

    // Reset the gamma tracker
    void particle_identification_driver::reset()
    {
      _set_defaults();
      set_initialized(false);
      return;
    }

    int particle_identification_driver::process(snemo::datamodel::particle_track_data & ptd_)
    {
      int status = 0;
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << particle_identification_id() << "' is already initialized !");

      status = _process_algo(ptd_);
      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Processing of particle tracks by '" << particle_identification_id() << "' algorithm has failed !");
        return status;
      }

      return status;
    }

    void particle_identification_driver::_set_defaults()
    {
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      _cut_manager_ = 0;
      return;
    }

    int particle_identification_driver::_process_algo(snemo::datamodel::particle_track_data & ptd_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      snemo::datamodel::particle_track_data::particle_collection_type & particles
        = ptd_.grab_particles();
      for (snemo::datamodel::particle_track_data::particle_collection_type::iterator
             it = particles.begin(); it != particles.end(); ++it) {
        snemo::datamodel::particle_track & a_particle = it->grab();

        for (property_dict_type::const_iterator ip = _pid_properties_.begin();
             ip != _pid_properties_.end(); ++ip) {
          const std::string & cut_name = ip->first;
          DT_LOG_DEBUG(get_logging_priority(), "Applying '" << cut_name << "' selection...");

          cuts::cut_manager & cut_mgr = grab_cut_manager();
          DT_THROW_IF(!cut_mgr.has(cut_name), std::logic_error, "Cut '" << cut_name << "' is missing !");
          cuts::i_cut & a_cut = cut_mgr.grab(cut_name);
          a_cut.set_user_data(a_particle);
          const int cut_status = a_cut.process();
          a_cut.reset_user_data();

          if (cut_status != cuts::SELECTION_ACCEPTED) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Current particle does not fulfill '" << cut_name << "' criteria !");
            continue;
          }

          // Store particle label within 'particle_track' auxiliairies
          std::string value = ip->second;
          datatools::properties & aux = a_particle.grab_auxiliaries();
          if (aux.has_key(snemo::datamodel::pid_utils::pid_label_key())) {
            value = aux.fetch_string(snemo::datamodel::pid_utils::pid_label_key()) + "|" + value;
          }
          aux.update(snemo::datamodel::pid_utils::pid_label_key(), value);

          if (get_logging_priority() >= datatools::logger::PRIO_DEBUG) {
            DT_LOG_DEBUG(get_logging_priority(), "Particle dump:");
            a_particle.tree_dump();
          }
        }
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return 0;
    }

  }  // end of namespace reconstruction

}  // end of namespace snemo
/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::particle_identification_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::particle_identification_driver");
  ocd_.set_class_description("A driver class for the Particle Identification algorithm");
  ocd_.set_class_library("Falaise_Particle Identification");
  ocd_.set_class_documentation("The driver manager for the PID algorithms\n"
                               "/todo What does the manager do ?"
                               );


  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::particle_identification_driver,
                               "snemo::reconstruction::particle_identification_driver")
