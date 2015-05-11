/// \file falaise/snemo/reconstruction/angle_measurement_driver.cc

// Ourselves:
#include <snemo/reconstruction/angle_measurement_driver.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/service_manager.h>
// - Bayeux/geomtools:
#include <geomtools/geometry_service.h>
#include <bayeux/geomtools/manager.h>

// This project:
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track_data.h>

namespace snemo {

  namespace reconstruction {

    const std::string & angle_measurement_driver::angle_measurement_id()
    {
      static const std::string _id("angle_measurement");
      return _id;
    }

    bool angle_measurement_driver::is_initialized() const
    {
      return _initialized_;
    }

    void angle_measurement_driver::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    void angle_measurement_driver::set_logging_priority(const datatools::logger::priority priority_)
    {
      _logging_priority_ = priority_;
      return;
    }

    datatools::logger::priority angle_measurement_driver::get_logging_priority() const
    {
      return _logging_priority_;
    }

    // Constructor
    angle_measurement_driver::angle_measurement_driver()
    {
      _set_defaults();
      return;
    }

    // Destructor
    angle_measurement_driver::~angle_measurement_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void angle_measurement_driver::_set_defaults()
    {

      _initialized_ = false;
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      return;
    }

    // Initialization :
    void angle_measurement_driver::initialize(const datatools::properties  & setup_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Driver '" << angle_measurement_id() << "' is already initialized !");

      // Logging priority
      datatools::logger::priority lp = datatools::logger::extract_logging_configuration(setup_);
      DT_THROW_IF(lp == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority level !");
      set_logging_priority(lp);

      _set_initialized(true);
      return;
    }

    void angle_measurement_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

    int angle_measurement_driver::process(const snemo::datamodel::particle_track & pt_,
                                          double & angle_)
    {
      int status = 0;
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << angle_measurement_id() << "' is already initialized !");

      status = _process_algo(pt_, angle_);

      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Computing topology quantities with '" << angle_measurement_id() << "' algorithm has failed !");
      }
      return status;
    }

    int angle_measurement_driver::process(const snemo::datamodel::particle_track & pt1_,
                                          const snemo::datamodel::particle_track & pt2_,
                                          double & angle_)
    {
      int status = 0;
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << angle_measurement_id() << "' is already initialized !");

      status = _process_algo(pt1_, pt2_, angle_);

      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Computing topology quantities with '" << angle_measurement_id() << "' algorithm has failed !");
      }
      return status;
    }

    int angle_measurement_driver::_process_algo(const snemo::datamodel::particle_track & pt_,
                                                double & angle_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // Invalidate angle meas.
      datatools::invalidate(angle_);

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt_)) {
        DT_LOG_WARNING(get_logging_priority(),
                       "No angle can be deduced from a single gamma !");
        return 1;
      }
      geomtools::vector_3d particle_dir;
      geomtools::invalidate(particle_dir);
      _get_direction(pt_, particle_dir);

      if (geomtools::is_valid(particle_dir)) {
        geomtools::vector_3d Ox(1,0,0);
        angle_ = std::acos(particle_dir * Ox) / M_PI * 180 * CLHEP::degree;
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return 0;
    }

    int angle_measurement_driver::_process_algo(const snemo::datamodel::particle_track & pt1_,
                                                const snemo::datamodel::particle_track & pt2_,
                                                double & angle_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // Invalidate angle meas.
      datatools::invalidate(angle_);

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) &&
          snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        DT_LOG_WARNING(get_logging_priority(), "The two particles are gammas ! No angle can be measured !");
        return 1;
      }

      geomtools::vector_3d particle_dir1;
      geomtools::invalidate(particle_dir1);
      _get_direction(pt1_, particle_dir1);
      geomtools::vector_3d particle_dir2;
      geomtools::invalidate(particle_dir2);
      _get_direction(pt2_, particle_dir2);

      if (geomtools::is_valid(particle_dir1) && geomtools::is_valid(particle_dir2)) {
        angle_ = std::acos(particle_dir1 * particle_dir2) / M_PI * 180 * CLHEP::degree;
      }
      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return 0;
    }

    void angle_measurement_driver::_get_direction(const snemo::datamodel::particle_track & pt_,
                                                  geomtools::vector_3d & direction_)
    {
      // Invalidate direction
      geomtools::invalidate(direction_);

      // Get foil vertices from particle tracks
      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices = pt_.get_vertices();
      geomtools::vector_3d foil_vertex;
      geomtools::invalidate(foil_vertex);
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             i_vtx = the_vertices.begin(); i_vtx != the_vertices.end(); ++i_vtx) {
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(i_vtx->get()))
          continue;

        foil_vertex = i_vtx->get().get_position();
        break;
      }
      if (! geomtools::is_valid(foil_vertex)) {
        DT_LOG_WARNING(get_logging_priority(), "Particle has no vertices on the source foil !");
        return;
      }

      // Check particle type
      if (snemo::datamodel::pid_utils::particle_is_gamma(pt_)) {
        DT_LOG_TRACE(get_logging_priority(), "Particle track is a gamma !");
        // Get the first vertex on calorimeter (should be the first associated calorimeter)
        for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
               i_vtx = the_vertices.begin(); i_vtx != the_vertices.end(); ++i_vtx) {
          if (snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(i_vtx->get()) ||
              snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(i_vtx->get())    ||
              snemo::datamodel::particle_track::vertex_is_on_gamma_veto(i_vtx->get())) {
            const geomtools::vector_3d & calo_vertex = i_vtx->get().get_position();
            direction_ = calo_vertex - foil_vertex;
            break;
          }
        }
      } else if (pt_.has_trajectory()) {
        const snemo::datamodel::tracker_trajectory & a_trajectory = pt_.get_trajectory();
        const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
        direction_ = a_track_pattern.get_shape().get_direction_on_curve(foil_vertex);
      } else {
        DT_LOG_WARNING(get_logging_priority(), "Particle track has tracker trajectory associated !");
        return;
      }

      // Return a normalized direction
      direction_ /= direction_.mag();
      return;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

  /* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::angle_measurement_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::angle_measurement_driver");
  ocd_.set_class_description("A driver class for the Angle measurement algorithm");
  ocd_.set_class_library("Falaise_AngleMeasurement");
  ocd_.set_class_documentation("The driver manager for the Angle Measurement algorithms\n"
                               "/todo What does the manager do ?"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::angle_measurement_driver,
                               "snemo::reconstruction::angle_measurement_driver")
