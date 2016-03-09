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

    uint32_t particle_identification_driver::get_mode() const
    {
      return _mode_;
    }

    bool particle_identification_driver::is_mode_pid_label() const
    {
      return _mode_ & MODE_PID_LABEL;
    }

    bool particle_identification_driver::is_mode_pid_user() const
    {
      return _mode_ & MODE_PID_USER;
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

      // Fetch PID mode
      if (setup_.has_flag("mode.label")) {
        _mode_ |= MODE_PID_LABEL;
      }
      if (setup_.has_flag("mode.user")) {
        _mode_ |= MODE_PID_USER;
      }
      DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error, "Missing at least a 'mode.XXX' property !");

      // Fetch PID definition
      DT_THROW_IF(! setup_.has_key("definitions"), std::logic_error,
                  "Missing definitions of particles !");
      std::vector<std::string> pid_definitions;
      setup_.fetch("definitions", pid_definitions);
      for (size_t i = 0; i < pid_definitions.size(); ++i) {
        const std::string & key = pid_definitions.at(i);
        if (is_mode_pid_label()) {
          const std::string str = key + ".label";
          if (setup_.has_key(str)) {
            const std::string a_key = snemo::datamodel::pid_utils::pid_label_key();
            const std::string a_value = setup_.fetch_string(str);
            pair_property_type ppt = std::make_pair(a_key, a_value);
            _pid_properties_.insert(std::make_pair(key, ppt));
          }
        }
        if (is_mode_pid_user()) {
          const std::string str1 = key + ".user.key";
          const std::string str2 = key + ".user.value";
          DT_THROW_IF(! setup_.has_key(str1) && ! setup_.has_key(str2), std::logic_error,
                      "Missing pair of key/value for '" << key << "' field !");
          const std::string a_key = setup_.fetch_string(str1);
          const std::string a_value = setup_.fetch_string(str2);
          pair_property_type ppt = std::make_pair(a_key, a_value);
          _pid_properties_.insert(std::make_pair(key, ppt));
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
      _mode_ = MODE_UNDEFINED;
      _cut_manager_ = 0;
      return;
    }

    int particle_identification_driver::_process_algo(snemo::datamodel::particle_track_data & ptd_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // Count number of particles given their label
      typedef std::map<std::string, size_t> particle_counter_type;
      particle_counter_type particle_counter;

      snemo::datamodel::particle_track_data::particle_collection_type & particles
        = ptd_.grab_particles();
      for (snemo::datamodel::particle_track_data::particle_collection_type::iterator
             it = particles.begin(); it != particles.end(); ++it) {
        snemo::datamodel::particle_track & a_particle = it->grab();

        bool particle_is_undefined = true;
        for (property_dict_type::const_iterator ip = _pid_properties_.begin();
             ip != _pid_properties_.end(); ++ip) {
          const std::string & cut_name = ip->first;
          DT_LOG_DEBUG(get_logging_priority(), "Applying '" << cut_name << "' selection...");

          cuts::cut_manager & cut_mgr = grab_cut_manager();
          DT_THROW_IF(! cut_mgr.has(cut_name), std::logic_error, "Cut '" << cut_name << "' is missing !");
          cuts::i_cut & a_cut = cut_mgr.grab(cut_name);
          a_cut.set_user_data(a_particle);
          const int cut_status = a_cut.process();
          a_cut.reset_user_data();

          if (cut_status != cuts::SELECTION_ACCEPTED) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Current particle does not fulfill '" << cut_name << "' criteria !");
            continue;
          }

          datatools::properties & aux = a_particle.grab_auxiliaries();
          const pair_property_type & ppt = ip->second;
          const std::string & key = ppt.first;
          std::string value = ppt.second;
          if (is_mode_pid_label()) {
            // Store particle label within 'particle_track' auxiliairies
            if (aux.has_key(key)) {
              const std::string a_label = aux.fetch_string(key);
              if (a_label != value) {
                value = a_label + "|" + value;
              }
            }
          }
          aux.update(key, value);

          particle_is_undefined = false;
          particle_counter[ppt.second]++;
        }

        if (is_mode_pid_label() && particle_is_undefined) {
          datatools::properties & aux = a_particle.grab_auxiliaries();
          aux.update(snemo::datamodel::pid_utils::pid_label_key(),
                     snemo::datamodel::pid_utils::undefined_label());
          particle_counter[snemo::datamodel::pid_utils::undefined_label()]++;
        }
      }

      for (particle_counter_type::const_iterator i = particle_counter.begin();
           i != particle_counter.end(); ++i) {
        DT_LOG_DEBUG(get_logging_priority(), "Number of '" << i->first << "' particles : "
                     << i->second);
        ptd_.grab_auxiliaries().update_integer(i->first, i->second);
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
