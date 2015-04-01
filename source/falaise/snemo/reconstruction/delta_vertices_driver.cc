/// \file falaise/snemo/reconstruction/delta_vertices_driver.cc

// Ourselves:
#include <snemo/reconstruction/delta_vertices_driver.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/service_manager.h>
// - Bayeux/geomtools:
#include <geomtools/geometry_service.h>
#include <bayeux/geomtools/manager.h>

//- GSL:
#include <gsl/gsl_cdf.h>

// This project:
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/processing/services.h>
#include <falaise/snemo/geometry/locator_plugin.h>
#include <falaise/snemo/geometry/calo_locator.h>
#include <falaise/snemo/geometry/xcalo_locator.h>
#include <falaise/snemo/geometry/gveto_locator.h>

namespace snemo {

  namespace reconstruction {

    const std::string & delta_vertices_driver::delta_vertices_id()
    {
      static const std::string _id("delta_vertices");
      return _id;
    }

    bool delta_vertices_driver::is_initialized() const
    {
      return _initialized_;
    }

    void delta_vertices_driver::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    bool delta_vertices_driver::has_geometry_manager() const
    {
      return _geometry_manager_ != 0;
    }

    void delta_vertices_driver::set_geometry_manager(const geomtools::manager & gmgr_)
    {
      DT_THROW_IF (is_initialized(), std::logic_error, "Already initialized/locked !");
      _geometry_manager_ = &gmgr_;
      return;
    }

    const geomtools::manager & delta_vertices_driver::get_geometry_manager() const
    {
      DT_THROW_IF (! has_geometry_manager(), std::logic_error, "No geometry manager is setup !");
      return *_geometry_manager_;
    }

    // Constructor
    delta_vertices_driver::delta_vertices_driver()
    {
      _set_defaults();
      return;
    }

    // Destructor
    delta_vertices_driver::~delta_vertices_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void delta_vertices_driver::_set_defaults()
    {

      _initialized_ = 0;
      _geometry_manager_ = 0;
      _sigma_t_gamma_interaction_uncertainty_ = 0.6 * CLHEP::ns;
      return;
    }

    // Initialization :
    void delta_vertices_driver::initialize(const datatools::properties  & setup_)
    {

      std::string key;

      if (setup_.has_key(key = "DVD.sigma_t_gamma_interaction_uncertainty")) {
        _sigma_t_gamma_interaction_uncertainty_ = setup_.fetch_real(key);
        if (! setup_.has_explicit_unit(key)) {
          _sigma_t_gamma_interaction_uncertainty_ *= CLHEP::ns;
        }
      }

      // Extract the setup of the gamma clustering algo :
      setup_.export_and_rename_starting_with(_delta_vertices_setup_, "DVD.", "");

      _set_initialized(true);
      return;
    }

    void delta_vertices_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

    int delta_vertices_driver::_process_algo(snemo::datamodel::particle_track_data & ptd_)
    {
      // DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::delta_vertices_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::delta_vertices_driver");
  ocd_.set_class_description("A driver class for the Gamma Clustering algorithm");
  ocd_.set_class_library("Falaise_GammaClustering");
  ocd_.set_class_documentation("The driver manager for the Gamma Clustering algorithms\n"
                               "/todo What does the manager do ?"
                               );


  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::delta_vertices_driver,
                               "snemo::reconstruction::delta_vertices_driver")
