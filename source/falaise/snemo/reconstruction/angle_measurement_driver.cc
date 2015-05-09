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

      std::vector<double> v_angle;
      status = _process_algo(pt1_, pt2_, v_angle);
      angle_ = v_angle.front();

      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Computing topology quantities with '" << angle_measurement_id() << "' algorithm has failed !");
      }
      return status;
    }

    int angle_measurement_driver::process(const snemo::datamodel::particle_track & pt1_,
                                          const snemo::datamodel::particle_track & pt2_,
                                          std::vector<double> & angle_)
    {
      int status = 0;
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << angle_measurement_id() << "' is already initialized !");

      status = _process_algo(pt1_, pt2_, angle_);

      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Computing topology quantities with '" << angle_measurement_id() << "' algorithm has failed !");
        return status;
      }

      return status;
    }

    int angle_measurement_driver::_process_algo(const snemo::datamodel::particle_track & pt_,
                                                double & angle_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      datatools::invalidate(angle_);

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt_)) {
        DT_LOG_WARNING(get_logging_priority(),
                       "No angle can be deduced from a single gamma !");
        return 1;
      } else {
        _process_single_charged(pt_, angle_);
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

    int angle_measurement_driver::_process_algo(const snemo::datamodel::particle_track & pt1_,
                                                const snemo::datamodel::particle_track & pt2_,
                                                std::vector<double> & angle_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      if (! snemo::datamodel::pid_utils::particle_is_gamma(pt1_) &&
          ! snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        _process_charged_particles(pt1_, pt2_, angle_);
      } else if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ||
                 snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        _process_charged_gamma_particles(pt1_, pt2_, angle_);
      } else {
        DT_LOG_WARNING(get_logging_priority(), "Topology not supported !");
        return 1;
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

    int angle_measurement_driver::_process_single_charged(const snemo::datamodel::particle_track & pt_,
                                                          double & angle_)
    {
      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices = pt_.get_vertices();
      geomtools::vector_3d foil_vertex;
      geomtools::invalidate(foil_vertex);
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             i_vtx = the_vertices.begin(); i_vtx != the_vertices.end(); ++i_vtx) {
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(i_vtx->get()) )
          continue;

        foil_vertex = i_vtx->get().get_position();
        break;
      }
      DT_THROW_IF(! geomtools::is_valid(foil_vertex), std::logic_error,
                  "Charged particle has no vertices on the source foil !");

      const snemo::datamodel::tracker_trajectory & a_trajectory = pt_.get_trajectory();
      const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
      geomtools::vector_3d dir = a_track_pattern.get_shape().get_direction_on_curve(foil_vertex);

      geomtools::vector_3d Ox(1,0,0);
      // Ox.setX(1);
      // Ox /= Ox.mag();
      /*vectors already normalized*/
      angle_ = std::acos(dir * Ox) / M_PI * 180 * CLHEP::degree;
      DT_LOG_TRACE(get_logging_priority(), "Angle = " << angle_/CLHEP::degree << "°");
      return 0;
    }

    int angle_measurement_driver::_process_charged_particles(const snemo::datamodel::particle_track & pt1_,
                                                             const snemo::datamodel::particle_track & pt2_,
                                                             std::vector<double> & angle_)
    {
      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices1 = pt1_.get_vertices();
      geomtools::vector_3d foil_vertex1;
      geomtools::invalidate(foil_vertex1);
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             i_vtx1 = the_vertices1.begin(); i_vtx1 != the_vertices1.end(); ++i_vtx1) {
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(i_vtx1->get()))
          continue;

        foil_vertex1 = i_vtx1->get().get_position();
        break;
      }
      DT_THROW_IF(! geomtools::is_valid(foil_vertex1), std::logic_error,
                  "First charged particle has no vertices on the source foil !");

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices2 = pt2_.get_vertices();
      geomtools::vector_3d foil_vertex2;
      geomtools::invalidate(foil_vertex2);
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             i_vtx2 = the_vertices2.begin(); i_vtx2 != the_vertices2.end(); ++i_vtx2) {
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(i_vtx2->get()))
          continue;

        foil_vertex2 = i_vtx2->get().get_position();
        break;
      }
      DT_THROW_IF(! geomtools::is_valid(foil_vertex2), std::logic_error,
                  "Second charged particle has no vertices on the source foil !");

      const snemo::datamodel::tracker_trajectory & a_trajectory1 = pt1_.get_trajectory();
      const snemo::datamodel::base_trajectory_pattern & a_track_pattern1 = a_trajectory1.get_pattern();
      geomtools::vector_3d dir1 = a_track_pattern1.get_shape().get_direction_on_curve(foil_vertex1);
      const snemo::datamodel::tracker_trajectory & a_trajectory2 = pt2_.get_trajectory();
      const snemo::datamodel::base_trajectory_pattern & a_track_pattern2 = a_trajectory2.get_pattern();
      geomtools::vector_3d dir2 = a_track_pattern2.get_shape().get_direction_on_curve(foil_vertex2);

      DT_LOG_TRACE(get_logging_priority(), "dir1 = " << dir1/CLHEP::mm << " mm");
      DT_LOG_TRACE(get_logging_priority(), "dir2 = " << dir2/CLHEP::mm << " mm");

      angle_.push_back(std::acos(dir1 * dir2) / M_PI * 180 * CLHEP::degree);
      DT_LOG_TRACE(get_logging_priority(), "Angle = " << angle_.back()/CLHEP::degree << "°");
      return 0;
    }

    int angle_measurement_driver::_process_charged_gamma_particles(const snemo::datamodel::particle_track & pt1_,
                                                                   const snemo::datamodel::particle_track & pt2_,
                                                                   std::vector<double> & angle_)
    {
      const snemo::datamodel::particle_track & pt_gamma
        = (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ? pt1_ : pt2_);
      const snemo::datamodel::particle_track & pt_charged
        = (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ? pt2_ : pt1_);

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_charged = pt_charged.get_vertices();
      geomtools::vector_3d foil_vertex_charged;
      geomtools::invalidate(foil_vertex_charged);
      for(snemo::datamodel::particle_track::vertex_collection_type::const_iterator
            i_vtx_charged = the_vertices_charged.begin();
          i_vtx_charged != the_vertices_charged.end(); ++i_vtx_charged) {
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(i_vtx_charged->get()))
          continue;

        foil_vertex_charged = i_vtx_charged->get().get_position();
        break;
      }
      DT_THROW_IF(! geomtools::is_valid(foil_vertex_charged), std::logic_error,
                  "Charged particle has no vertices on the source foil !");
      const snemo::datamodel::tracker_trajectory & a_trajectory_charged = pt_charged.get_trajectory();
      const snemo::datamodel::base_trajectory_pattern & a_track_pattern_charged = a_trajectory_charged.get_pattern();
      geomtools::vector_3d dir1 = a_track_pattern_charged.get_shape().get_direction_on_curve(foil_vertex_charged);

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_gamma = pt_gamma.get_vertices();
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             i_vtx = the_vertices_gamma.begin();
           i_vtx != the_vertices_gamma.end(); ++i_vtx) {
        if (snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(i_vtx->get()) ||
            snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(i_vtx->get())    ||
            snemo::datamodel::particle_track::vertex_is_on_gamma_veto(i_vtx->get())) {
          const geomtools::vector_3d & calo_vertex = i_vtx->get().get_position();
          geomtools::vector_3d dir2 = calo_vertex - foil_vertex_charged;
          dir2 /= dir2.mag();
          angle_.push_back(std::acos(dir1 * dir2) / M_PI * 180 * CLHEP::degree);
        }
      }

      return 0;
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
