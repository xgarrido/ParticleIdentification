/// \file falaise/snemo/reconstruction/particle_identification_module.cc

// Ourselves:
#include <snemo/reconstruction/particle_identification_module.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/service_manager.h>
// - Bayeux/cuts:
#include <cuts/cut_service.h>
#include <cuts/cut_manager.h>

// This project:
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/processing/services.h>

// PID driver
#include <snemo/reconstruction/particle_identification_driver.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    DPP_MODULE_REGISTRATION_IMPLEMENT(particle_identification_module,
                                      "snemo::reconstruction::particle_identification_module")

    void particle_identification_module::_set_defaults()
    {
      _PTD_label_ = snemo::datamodel::data_info::default_particle_track_data_label();
      _driver_.reset(0);
      return;
    }

    // Initialization :
    void particle_identification_module::initialize(const datatools::properties  & setup_,
                                                    datatools::service_manager   & service_manager_,
                                                    dpp::module_handle_dict_type & /* module_dict_ */)
    {
      DT_THROW_IF (is_initialized(),
                   std::logic_error,
                   "Module '" << get_name() << "' is already initialized ! ");

      dpp::base_module::_common_initialize(setup_);

      if (setup_.has_key("PTD_label")) {
        _PTD_label_ = setup_.fetch_string("PTD_label");
      }

      // Cut manager :
      std::string cut_label = snemo::processing::service_info::default_cut_service_label();
      if (setup_.has_key("Cut_label")) {
        cut_label = setup_.fetch_string("Cut_label");
      }
      DT_THROW_IF(cut_label.empty(), std::logic_error,
                  "Module '" << get_name() << "' has no valid '" << "Cut_label" << "' property !");
      DT_THROW_IF(! service_manager_.has(cut_label) ||
                  ! service_manager_.is_a<cuts::cut_service>(cut_label),
                  std::logic_error,
                  "Module '" << get_name() << "' has no '" << cut_label << "' service !");
      cuts::cut_service & Cut
        = service_manager_.grab<cuts::cut_service>(cut_label);

      // PID algorithm :
      DT_THROW_IF(!setup_.has_key("drivers"), std::logic_error, "Missing 'drivers' algorithm");
      std::vector<std::string> driver_names;
      setup_.fetch("drivers", driver_names);
      for (std::vector<std::string>::const_iterator
             idriver = driver_names.begin();
           idriver != driver_names.end(); ++idriver) {
        const std::string & a_driver_name = *idriver;
        if (a_driver_name == particle_identification_driver::get_id()) {
          _driver_.reset(new snemo::reconstruction::particle_identification_driver);
        } else {
          DT_THROW_IF(true, std::logic_error,
                      "Unsupported '" << a_driver_name << "'particle identification algorithm ");
        }

        // Plug the cut manager :
        _driver_.get()->set_cut_manager(Cut.grab_cut_manager());

        // Initialize the PID driver :
        datatools::properties PID_config;
        setup_.export_and_rename_starting_with(PID_config, a_driver_name + ".", "");
        _driver_.get()->initialize(PID_config);
      }

      _set_initialized(true);
      return;
    }

    void particle_identification_module::reset()
    {
      DT_THROW_IF (! is_initialized(),
                   std::logic_error,
                   "Module '" << get_name() << "' is not initialized !");
      _set_initialized(false);
      _set_defaults();
      return;
    }

    // Constructor :
    particle_identification_module::particle_identification_module(datatools::logger::priority logging_priority_)
      : dpp::base_module(logging_priority_)
    {
      _set_defaults();
      return;
    }

    // Destructor :
    particle_identification_module::~particle_identification_module()
    {
      if (is_initialized()) particle_identification_module::reset();
      return;
    }

    // Processing :
    dpp::base_module::process_status particle_identification_module::process(datatools::things & data_record_)
    {
      DT_THROW_IF (! is_initialized(), std::logic_error,
                   "Module '" << get_name() << "' is not initialized !");

      // Check particle track data
      const bool abort_at_missing_input = true;
      if (! data_record_.has(_PTD_label_)) {
        DT_THROW_IF(abort_at_missing_input, std::logic_error, "Missing particle track data to be processed !");
        // leave the data unchanged.
        return dpp::base_module::PROCESS_ERROR;
      }
      // Grab the 'particle_track_data' entry from the data model :
      snemo::datamodel::particle_track_data & the_particle_track_data
        = data_record_.grab<snemo::datamodel::particle_track_data>(_PTD_label_);

      // Main processing method :
      _process(the_particle_track_data);

      return dpp::base_module::PROCESS_SUCCESS;
    }

    void particle_identification_module::_process(snemo::datamodel::particle_track_data & ptd_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // Process the fitter driver :
      _driver_.get()->process(ptd_);

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::particle_identification_module, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::particle_identification_module");
  ocd_.set_class_description("A module that performs particle identification");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("This module uses the ``snemo::datamodel::particle_track_data`` bank \n"
                               "and given selection criteria, assign a particle name to each ``snemo::datamodel::particle_track.`` \n");

  dpp::base_module::common_ocd(ocd_);

  {
    // Description of the 'PTD_label' configuration property :
    datatools::configuration_property_description & cpd
      = ocd_.add_property_info();
    cpd.set_name_pattern("PTD_label")
      .set_terse_description("The label/name of the 'particle track data' bank")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("This is the name of the bank to be used as    \n"
                            "the source of calorimeter hits and reconstructed vertices. \n"
                            )
      .set_default_value_string(snemo::datamodel::data_info::default_particle_track_data_label())
      .add_example("Use an alternative name for the 'particle track data' bank:: \n"
                   "                                  \n"
                   "  PTD_label : string = \"PTD2\"   \n"
                   "                                  \n"
                   )
      ;
  }

  {
    // Description of the 'Geo_label' configuration property :
    datatools::configuration_property_description & cpd
      = ocd_.add_property_info();
    cpd.set_name_pattern("Cut_label")
      .set_terse_description("The label/name of the cut service")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("This is the name of the service to be used as the \n"
                            "cut service.                                      \n"
                            )
      .set_default_value_string(snemo::processing::service_info::default_cut_service_label())
      .add_example("Use an alternative name for the cut service:: \n"
                   "                                              \n"
                   "  Cuts_label : string = \"cuts2\"             \n"
                   "                                              \n"
                   )
      ;
  }
  // Additionnal configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the      \n"
                               "``datatools::properties`` ASCII format::         \n"
                               "                                         \n"
                               "  PTD_label : string = \"PTD\"           \n"
                               "                                         \n"
                               "Additional specific parameters are used to configure         \n"
                               "the embedded ``Particle_Identification`` driver itself; see the OCD support \n"
                               "of the ``snemo::reconstruction::particle_identification_driver`` class.     \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::particle_identification_module,
                               "snemo::reconstruction::particle_identification_module")
