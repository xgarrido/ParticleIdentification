/// \file falaise/snemo/reconstruction/topology_driver.cc

// Ourselves:
#include <snemo/reconstruction/topology_driver.h>

// Standard library
#include <regex>

// Third party:
// - Bayeux/cuts:
#include <bayeux/cuts/cut_manager.h>

// This project:
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>

#include <falaise/snemo/reconstruction/tof_driver.h>
#include <falaise/snemo/reconstruction/vertex_driver.h>
#include <falaise/snemo/reconstruction/angle_driver.h>
#include <falaise/snemo/reconstruction/energy_driver.h>

#include <falaise/snemo/reconstruction/base_topology_builder.h>

namespace snemo {

  namespace reconstruction {

    const std::string & topology_driver::topology_id()
    {
      static const std::string _id("TD");
      return _id;
    }

    void topology_driver::set_initialized(const bool initialized_)
    {
      _initialized_ = initialized_;
      return;
    }

    bool topology_driver::is_initialized() const
    {
      return _initialized_;
    }

    void topology_driver::set_logging_priority(const datatools::logger::priority priority_)
    {
      _logging_priority_ = priority_;
      return;
    }

    datatools::logger::priority topology_driver::get_logging_priority() const
    {
      return _logging_priority_;
    }

    // Constructor
    topology_driver::topology_driver()
    {
      _set_defaults();
      set_initialized(false);
      return;
    }

    // Destructor
    topology_driver::~topology_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    // Initialize the gamma tracker through configuration properties
    void topology_driver::initialize(const datatools::properties & setup_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Driver '" << topology_id() << "' is already initialized !");

      // Logging priority
      datatools::logger::priority lp = datatools::logger::extract_logging_configuration(setup_);
      DT_THROW_IF(lp == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority level for geometry manager !");
      set_logging_priority(lp);

      // Drivers :
      DT_THROW_IF(! setup_.has_key("drivers"), std::logic_error, "Missing 'drivers' key !");
      std::vector<std::string> driver_names;
      setup_.fetch("drivers", driver_names);
      for (std::vector<std::string>::const_iterator
             idriver = driver_names.begin();
           idriver != driver_names.end(); ++idriver) {
        const std::string & a_driver_name = *idriver;

        if (a_driver_name == snemo::reconstruction::tof_driver::get_id()) {
          // Initialize TOF Driver
          _drivers_.TOFD.reset(new snemo::reconstruction::tof_driver);
          datatools::properties TOFD_config;
          setup_.export_and_rename_starting_with(TOFD_config, std::string(a_driver_name + "."), "");
          _drivers_.TOFD->initialize(TOFD_config);
        } else if (a_driver_name == snemo::reconstruction::vertex_driver::get_id()) {
          // Initialize Vertex Driver
          _drivers_.VD.reset(new snemo::reconstruction::vertex_driver);
          datatools::properties VD_config;
          setup_.export_and_rename_starting_with(VD_config, std::string(a_driver_name + "."), "");
          _drivers_.VD->initialize(VD_config);
        } else if (a_driver_name == snemo::reconstruction::angle_driver::get_id()) {
          // Initialize Angle Driver
          _drivers_.AMD.reset(new snemo::reconstruction::angle_driver);
          datatools::properties AMD_config;
          setup_.export_and_rename_starting_with(AMD_config, std::string(a_driver_name + "."), "");
          _drivers_.AMD->initialize(AMD_config);
        } else if (a_driver_name == snemo::reconstruction::energy_driver::get_id()) {
          // Initialize Energy Driver
          _drivers_.EMD.reset(new snemo::reconstruction::energy_driver);
          datatools::properties EMD_config;
          setup_.export_and_rename_starting_with(EMD_config, std::string(a_driver_name + "."), "");
          _drivers_.EMD->initialize(EMD_config);
        } else {
          DT_THROW_IF(true, std::logic_error, "Driver '" << a_driver_name << "' does not exist !");
        }
      }

      set_initialized(true);
      return;
    }

    // Reset the gamma tracker
    void topology_driver::reset()
    {
      _set_defaults();
      set_initialized(false);
      return;
    }

    int topology_driver::process(const snemo::datamodel::particle_track_data & ptd_,
                                 snemo::datamodel::topology_data & td_)
    {
      int status = 0;
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << topology_id() << "' is already initialized !");

      status = _process_algo(ptd_, td_);
      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Computing topology quantities with '" << topology_id() << "' algorithm has failed !");
        return status;
      }

      return status;
    }

    void topology_driver::_set_defaults()
    {
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      _drivers_.TOFD.reset(0);
      _drivers_.VD.reset(0);
      _drivers_.AMD.reset(0);
      _drivers_.EMD.reset(0);
      return;
    }

    int topology_driver::_process_algo(const snemo::datamodel::particle_track_data & ptd_,
                                       snemo::datamodel::topology_data & td_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      const std::string a_classification = topology_driver::_get_classification_(ptd_);
      td_.grab_auxiliaries().store(snemo::datamodel::pid_utils::classification_label_key(),
                                   a_classification);
      const std::string a_builder_class_id = topology_driver::_get_builder_class_id_(a_classification);
      if (a_builder_class_id.empty()) {
        DT_LOG_WARNING(get_logging_priority(), "Topology not supported for the measurements ");
        return 0;
      }

      const base_topology_builder::factory_register_type & FB
        = DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(base_topology_builder);
      DT_THROW_IF(! FB.has(a_builder_class_id), std::logic_error,
                  "Topology builder class id '" << a_builder_class_id << "' "
                  << "is not available from the system builder factory register !");
      const base_topology_builder::factory_register_type::factory_type & the_factory
        = FB.get(a_builder_class_id);
      snemo::reconstruction::base_topology_builder * new_builder = the_factory();
      td_.set_pattern_handle(new_builder->create_pattern());

      // Build new topology pattern
      new_builder->set_measurement_drivers(_drivers_);
      new_builder->build(ptd_, td_.grab_pattern());

      if (get_logging_priority() >= datatools::logger::PRIO_TRACE) {
        DT_LOG_TRACE(get_logging_priority(), "New pattern: ");
        td_.get_pattern().tree_dump(std::clog, "", "[trace]: ");
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return 0;
    }

    std::string topology_driver::_get_classification_(const snemo::datamodel::particle_track_data & ptd_) const
    {
      const datatools::properties & aux = ptd_.get_auxiliaries();
      std::ostringstream classification;

      std::string key;
      if (aux.has_key(key = snemo::datamodel::pid_utils::electron_label())) {
        classification << aux.fetch_integer(key) << "e";
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::positron_label())) {
        classification << aux.fetch_integer(key) << "p";
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::gamma_label())) {
        classification << aux.fetch_integer(key) << "g";
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::alpha_label())) {
        classification << aux.fetch_integer(key) << "a";
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::undefined_label())) {
        classification << aux.fetch_integer(key) << "X";
      }
      const std::string a_classification = classification.str();
      DT_LOG_TRACE(get_logging_priority(), "Event classification : " << a_classification);
      return a_classification;
    }

    std::string topology_driver::_get_builder_class_id_(const std::string & classification_) const
    {
      // Regex machinery...
      std::string a_class_id;
      if (classification_ == "1e") {
        a_class_id = "snemo::reconstruction::topology_1e_builder";
      } else if (classification_ == "1e1a") {
        a_class_id = "snemo::reconstruction::topology_1e1a_builder";
      } else if (classification_ == "1e1p") {
        a_class_id = "snemo::reconstruction::topology_1e1p_builder";
      } else if (std::regex_match(classification_, std::regex("1e[0-9]+g"))) {
        a_class_id = "snemo::reconstruction::topology_1eNg_builder";
      } else if (classification_ == "2e") {
        a_class_id = "snemo::reconstruction::topology_2e_builder";
      } else if (std::regex_match(classification_, std::regex("2e[0-9]+g"))) {
        a_class_id = "snemo::reconstruction::topology_2eNg_builder";
      } else {
        DT_LOG_WARNING(get_logging_priority(), "Non supported classification '" << classification_ << "' !");
      }
      DT_LOG_TRACE(get_logging_priority(), "Builder class id : " << a_class_id);
      return a_class_id;
    }

    // static
    void topology_driver::init_ocd(datatools::object_configuration_description & ocd_)
    {
      // Prefix "TD" stands for "Topology Driver" :
      datatools::logger::declare_ocd_logging_configuration(ocd_, "fatal", "TD.");

      // Invoke specific OCD support from the driver class:
      ::snemo::reconstruction::tof_driver::init_ocd(ocd_);
      ::snemo::reconstruction::vertex_driver::init_ocd(ocd_);
      ::snemo::reconstruction::angle_driver::init_ocd(ocd_);
      ::snemo::reconstruction::energy_driver::init_ocd(ocd_);

      return;
    }

  }  // end of namespace reconstruction

}  // end of namespace snemo
/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::topology_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::topology_driver");
  ocd_.set_class_description("A driver class for the topology algorithms");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("The driver manages the different topology patterns\n"
                               "and addresses the related measurements.");

  // Invoke specific OCD support :
  ::snemo::reconstruction::topology_driver::init_ocd(ocd_);

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::topology_driver,
                               "snemo::reconstruction::topology_driver")
