/// \file falaise/snemo/reconstruction/topology_module.cc

// Ourselves:
#include <snemo/reconstruction/topology_module.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/service_manager.h>
// - Bayeux/geomtools:
#include <geomtools/geometry_service.h>
#include <geomtools/manager.h>

// This project:
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/processing/services.h>

#include <snemo/reconstruction/topology_driver.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    DPP_MODULE_REGISTRATION_IMPLEMENT(topology_module,
                                      "snemo::reconstruction::topology_module")

    void topology_module::_set_defaults()
    {
      _PTD_label_ = snemo::datamodel::data_info::default_particle_track_data_label();
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      _driver_.reset(0);
      return;
    }

    // Initialization :
    void topology_module::initialize(const datatools::properties  & setup_,
                                     datatools::service_manager   & /* service_manager_ */,
                                     dpp::module_handle_dict_type & /* module_dict_ */)
    {
      DT_THROW_IF (is_initialized(),
                   std::logic_error,
                   "Module '" << get_name() << "' is already initialized ! ");

      dpp::base_module::_common_initialize(setup_);

      if (setup_.has_key("PTD_label")) {
        _PTD_label_ = setup_.fetch_string("PTD_label");
      }

      if (setup_.has_key("TD_label")) {
        _TD_label_ = setup_.fetch_string("TD_label");
      }

      // Drivers :
      _driver_.reset(new snemo::reconstruction::topology_driver);
      _driver_->initialize(setup_);

      _set_initialized(true);
      return;
    }

    void topology_module::reset()
    {
      DT_THROW_IF (! is_initialized(),
                   std::logic_error,
                   "Module '" << get_name() << "' is not initialized !");
      _set_initialized(false);
      _set_defaults();
      return;
    }

    // Constructor :
    topology_module::topology_module(datatools::logger::priority logging_priority_)
      : dpp::base_module(logging_priority_)
    {
      _set_defaults();
      return;
    }

    // Destructor :
    topology_module::~topology_module()
    {
      if (is_initialized()) topology_module::reset();
      return;
    }

    // Processing :
    dpp::base_module::process_status topology_module::process(datatools::things & data_record_)
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
      const snemo::datamodel::particle_track_data & the_particle_track_data
        = data_record_.get<snemo::datamodel::particle_track_data>(_PTD_label_);

      /*********************************
       * Check particle track data     *
       *********************************/
      const bool preserve_former_output = false;

      // check if some 'topology_data' are available in the data model:
      snemo::datamodel::topology_data * ptr_topology_data = 0;
      if (! data_record_.has(_TD_label_)) {
        ptr_topology_data
          = &(data_record_.add<snemo::datamodel::topology_data>(_TD_label_));
      } else {
        ptr_topology_data
          = &(data_record_.grab<snemo::datamodel::topology_data>(_TD_label_));
        if (! preserve_former_output) {
          ptr_topology_data->reset();
        }
      }
      snemo::datamodel::topology_data & the_topology_data = *ptr_topology_data;

      // Main processing method :
      _process(the_particle_track_data, the_topology_data);

      return dpp::base_module::PROCESS_SUCCESS;
    }

    void topology_module::_process(const snemo::datamodel::particle_track_data & ptd_,
                                   snemo::datamodel::topology_data & td_ )
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // Process the fitter driver :
      _driver_.get()->process(ptd_,td_);

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::topology_module, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::topology_module");
  ocd_.set_class_description("A module that considers the event topology");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("This module uses the ``snemo::datamodel::particle_track_data`` bank               \n"
                               "and, given the particles identified, computes relevant topology quantities before \n"
                               "storing them in ``snemo::datamodel::topology_data.``                              \n");

  dpp::base_module::common_ocd(ocd_);

  {
    // Description of the 'PTD_label' configuration property :
    datatools::configuration_property_description & cpd
      = ocd_.add_property_info();
    cpd.set_name_pattern("PTD_label")
      .set_terse_description("The label/name of the 'particle track data' bank")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("This is the name of the input bank to be used as           \n"
                            "the source of calorimeter hits and reconstructed vertices. \n")
      .set_default_value_string(snemo::datamodel::data_info::default_particle_track_data_label())
      .add_example("Use an alternative name for the 'particle track data' bank:: \n"
                   "                                                             \n"
                   "  PTD_label : string = \"PTD2\"                              \n"
                   "                                                             \n"
                   )
      ;
  }

  {
    // Description of the 'TD_label' configuration property :
    datatools::configuration_property_description & cpd
      = ocd_.add_property_info();
    cpd.set_name_pattern("TD_label")
      .set_terse_description("The label/name of the 'topology data' bank")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("This is the name of the output bank to be used \n"
                            "to select events based on their topology.      \n")
      .set_default_value_string("TD")//snemo::datamodel::data_info::default_topology_data_label())
      .add_example("Use an alternative name for the 'topology data' bank:: \n"
                   "                                                       \n"
                   "  TD_label : string = \"TD2\"                          \n"
                   "                                                       \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("drivers")
      .set_terse_description("The driver ids to be used")
      .set_traits(datatools::TYPE_STRING,
                  datatools::configuration_property_description::ARRAY)
      .set_mandatory(false)
      .add_example("Use TOF driver only::            \n"
                   "                                 \n"
                   "  drivers : string[1] = \"TOFD\" \n"
                   "                                 \n"
                   )
      ;
  }

  // Invoke specific OCD support from the driver class:
  ::snemo::reconstruction::topology_driver::init_ocd(ocd_);

  // Additionnal configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the      \n"
                               "``datatools::properties`` ASCII format::         \n"
                               "                                                 \n"
                               "  PTD_label : string = \"PTD\"                   \n"
                               "  TD_label  : string = \"TD\"                    \n"
                               "  drivers   : string[3] = \"TOFD\" \"VD\" \"AD\" \n"
                               "  TOFD.logging.priority : string = \"error\"     \n"
                               "  VD.logging.priority  : string = \"error\"      \n"
                               "  AMD.logging.priority  : string = \"error\"     \n"
                               "                                                 \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}

DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::topology_module,
                               "snemo::reconstruction::topology_module")
