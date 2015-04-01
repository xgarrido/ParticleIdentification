/// \file falaise/snemo/reconstruction/tof_driver.cc

// Ourselves:
#include <snemo/reconstruction/tof_driver.h>

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
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/processing/services.h>
#include <falaise/snemo/geometry/locator_plugin.h>
#include <falaise/snemo/geometry/calo_locator.h>
#include <falaise/snemo/geometry/xcalo_locator.h>
#include <falaise/snemo/geometry/gveto_locator.h>
#include <falaise/snemo/datamodels/base_trajectory_pattern.h>
#include <falaise/snemo/datamodels/line_trajectory_pattern.h>
#include <falaise/snemo/datamodels/helix_trajectory_pattern.h>

namespace snemo {

  namespace reconstruction {

    const std::string & tof_driver::tof_id()
    {
      static const std::string _id("tof");
      return _id;
    }


    bool tof_driver::is_initialized() const
    {
      return _initialized_;
    }

    void tof_driver::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    // Constructor
    tof_driver::tof_driver()
    {
      _set_defaults();
      return;
    }

    // Destructor
    tof_driver::~tof_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void tof_driver::_set_defaults()
    {

      _sigma_t_gamma_interaction_uncertainty_ = 0.6 * CLHEP::ns;
      return;
    }

    // Initialization :
    void tof_driver::initialize(const datatools::properties  & setup_)
    {


      std::string key;

      if (setup_.has_key(key = "TOFD.sigma_t_gamma_interaction_uncertainty")) {
        _sigma_t_gamma_interaction_uncertainty_ = setup_.fetch_real(key);
        if (! setup_.has_explicit_unit(key)) {
          _sigma_t_gamma_interaction_uncertainty_ *= CLHEP::ns;
        }
      }

      // Extract the setup of the gamma clustering algo :
      setup_.export_and_rename_starting_with(_tof_setup_, "TOF.", "");

      _set_initialized(true);
      return;
    }

    void tof_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

    int tof_driver::_process_algo(double & proba_,
                                  snemo::datamodel::particle_track & particle_1_,
                                  snemo::datamodel::particle_track & particle_2_)
    {
      // DT_LOG_TRACE(get_logging_priority(), "Entering...");

      if (! particle_1_.has_associated_calorimeter_hits() || ! particle_2_.has_associated_calorimeter_hits())
        {
          DT_THROW_IF(true, std::logic_error,
                      "Particle track is not associated to any calorimeter block !");
          return 0;
        }

      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters_1 = particle_1_.get_associated_calorimeter_hits ();

      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters_2 = particle_2_.get_associated_calorimeter_hits ();

      if (the_calorimeters_1.size() >= 2 || the_calorimeters_2.size() >= 2)
        {
          DT_THROW_IF(true, std::logic_error,
                      "Particle track is associated to more than 1 calorimeter block !");
          return 0;
        }

      double m1 = _get_mass(particle_1_) * CLHEP::MeV;
      double m2 = _get_mass(particle_2_) * CLHEP::MeV;

      const double t1 = the_calorimeters_1.at(0).get().get_time() * CLHEP::ns;
      const double t2 = the_calorimeters_2.at(0).get().get_time() * CLHEP::ns;

      double track_length_1 =_get_track_length(particle_1_);
      double track_length_2 =_get_track_length(particle_2_);




//       const double t_th = track_length / CLHEP::c_light;
// #warning sigma_l unit must be fixed !
//       const double sigma_l = 0.6 * CLHEP::ns;
//       const double sigma_exp = pow(head_end_calo_hit_.get_sigma_time(),2)
//         + pow(tail_begin_calo_hit_.get_sigma_time(),2) + pow(sigma_l,2);
//       const double chi2 = pow(std::abs(t1 - t2) - t_th,2)/sigma_exp;
//       proba_ = gsl_cdf_chisq_Q(chi2, 1);


      // DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

    double tof_driver::_get_mass(snemo::datamodel::particle_track & particle_)
    {
      // DT_LOG_TRACE(get_logging_priority(), "Entering...");

      datatools::properties & aux = particle_.grab_auxiliaries();

      /* probably a cleaner way to do it*/
      if(!aux.has_key(snemo::datamodel::pid_utils::pid_label_key()))
        DT_THROW_IF(true, std::logic_error,
                    "Missing information on the particle type !");

      const std::string label = aux.fetch_string(snemo::datamodel::pid_utils::pid_label_key());

      if(label == snemo::datamodel::pid_utils::electron_label())
        return 0.511;
      else if(label == snemo::datamodel::pid_utils::gamma_label())
        return 0;
      else
        DT_THROW_IF(true, std::logic_error,
                    "Particle type inappropriate for TOF calculations !");

      // DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

    double tof_driver::_get_track_length(snemo::datamodel::particle_track & particle_)
    {
      // DT_LOG_TRACE(get_logging_priority(), "Entering...");

      datatools::properties & aux = particle_.grab_auxiliaries();

      /* probably a cleaner way to do it*/
      if(! aux.has_key(snemo::datamodel::pid_utils::pid_label_key()))
        DT_THROW_IF(true, std::logic_error,
                    "Missing information on the particle type !");

      const std::string label = aux.fetch_string(snemo::datamodel::pid_utils::pid_label_key());

      if(label == snemo::datamodel::pid_utils::electron_label())
        {
          const snemo::datamodel::tracker_trajectory & a_trajectory = particle_.get_trajectory();
          const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
          const std::string & a_pattern_id = a_track_pattern.get_pattern_id();

          if (a_pattern_id == snemo::datamodel::line_trajectory_pattern::pattern_id())
            {
              const snemo::datamodel::line_trajectory_pattern * ptr_line
                = dynamic_cast<const snemo::datamodel::line_trajectory_pattern *>(&a_track_pattern);
              const geomtools::line_3d & a_line = ptr_line->get_segment();
              return a_line.get_length();
            }
          else if (a_pattern_id == snemo::datamodel::helix_trajectory_pattern::pattern_id())
            {
              const snemo::datamodel::helix_trajectory_pattern * ptr_helix
                = dynamic_cast<const snemo::datamodel::helix_trajectory_pattern *>(&a_track_pattern);
              const geomtools::helix_3d & a_helix = ptr_helix->get_helix();
              return a_helix.get_length();
            }
        }
      else if(label == snemo::datamodel::pid_utils::gamma_label())
        {
          /*the first vertex on calorimeter is the first in time but is it always true ???*/
          if(!particle_.has_vertices())
            DT_THROW_IF(true, std::logic_error,
                        "Gamma particle has no vertices associated !");

          const snemo::datamodel::particle_track::vertex_collection_type & the_vertices = particle_.get_vertices();

          bool found_calorimeter_vertex = false;
          for(snemo::datamodel::particle_track::vertex_collection_type::const_iterator i_vtx = the_vertices.begin(); i_vtx<the_vertices.end(); ++i_vtx)
            {
              if(!snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(i_vtx->get()) || !snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(i_vtx->get())
                 || !snemo::datamodel::particle_track::vertex_is_on_gamma_veto(i_vtx->get()))
                continue;

              found_calorimeter_vertex = true;
              break;
            }
          if(!found_calorimeter_vertex)
            DT_THROW_IF(true, std::logic_error,
                        "Gamma particle has no vertices on the calorimeter !");

        }
      else
        DT_THROW_IF(true, std::logic_error,
                    "Particle type inappropriate for TOF calculations !");

      // DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

//     double tof_driver::_get_tof_probability(const snemo::datamodel::calibrated_calorimeter_hit & head_end_calo_hit_,
//                                                          const snemo::datamodel::calibrated_calorimeter_hit & tail_begin_calo_hit_) const
//     {
//       const snemo::geometry::calo_locator & calo_locator   = base_gamma_builder::get_calo_locator();
//       const snemo::geometry::xcalo_locator & xcalo_locator = base_gamma_builder::get_xcalo_locator();
//       const snemo::geometry::gveto_locator & gveto_locator = base_gamma_builder::get_gveto_locator();

//       geomtools::vector_3d head_position;
//       const geomtools::geom_id & head_gid = head_end_calo_hit_.get_geom_id();
//       if (calo_locator.is_calo_block_in_current_module(head_gid))
//         calo_locator.get_block_position(head_gid, head_position);
//       else if (xcalo_locator.is_calo_block_in_current_module(head_gid))
//         xcalo_locator.get_block_position(head_gid, head_position);
//       else if (gveto_locator.is_calo_block_in_current_module(head_gid))
//         gveto_locator.get_block_position(head_gid, head_position);
//       else
//         DT_THROW_IF(true, std::logic_error,
//                     "Current geom id '" << head_gid << "' does not match any scintillator block !");

//       geomtools::vector_3d tail_position;
//       const geomtools::geom_id & tail_gid = tail_begin_calo_hit_.get_geom_id();
//       if (calo_locator.is_calo_block_in_current_module(tail_gid))
//         calo_locator.get_block_position(tail_gid, tail_position);
//       else if (xcalo_locator.is_calo_block_in_current_module(tail_gid))
//         xcalo_locator.get_block_position(tail_gid, tail_position);
//       else if (gveto_locator.is_calo_block_in_current_module(tail_gid))
//         gveto_locator.get_block_position(tail_gid, tail_position);
//       else
//         DT_THROW_IF(true, std::logic_error,
//                     "Current geom id '" << tail_gid << "' does not match any scintillator block !");

//       const double t1 = head_end_calo_hit_.get_time();
//       const double t2 = tail_begin_calo_hit_.get_time();
//       const double track_length = (head_position-tail_position).mag();
//       const double t_th = track_length / CLHEP::c_light;
// #warning sigma_l unit must be fixed !
//       const double sigma_l = 0.6 * CLHEP::ns;
//       const double sigma_exp = pow(head_end_calo_hit_.get_sigma_time(),2)
//         + pow(tail_begin_calo_hit_.get_sigma_time(),2) + pow(sigma_l,2);
//       const double chi2 = pow(std::abs(t1 - t2) - t_th,2)/sigma_exp;
//       return gsl_cdf_chisq_Q(chi2, 1);
//     }

  } // end of namespace reconstruction

} // end of namespace snemo

/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::tof_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::tof_driver");
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
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::tof_driver,
                               "snemo::reconstruction::tof_driver")
