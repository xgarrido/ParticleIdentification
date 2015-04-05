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

    void tof_driver::set_logging_priority(const datatools::logger::priority priority_)
    {
      _logging_priority_ = priority_;
      return;
    }

    datatools::logger::priority tof_driver::get_logging_priority() const
    {
      return _logging_priority_;
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

    int tof_driver::process(const snemo::datamodel::particle_track & pt1_,
                            const snemo::datamodel::particle_track & pt2_,
                            double & proba_int_, double & proba_ext_)
    {
      int status = 0;
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << tof_id() << "' is already initialized !");

      status = _process_algo(pt1_, pt2_, proba_int_, proba_ext_);

      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Computing topology quantities with '" << tof_id() << "' algorithm has failed !");
        return status;
      }

      return status;
    }

    void tof_driver::_set_defaults()
    {
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      _sigma_t_gamma_interaction_ = 0.6 * CLHEP::ns;
      return;
    }

    // Initialization :
    void tof_driver::initialize(const datatools::properties & setup_)
    {
      // Logging priority
      datatools::logger::priority lp = datatools::logger::extract_logging_configuration(setup_);
      DT_THROW_IF(lp == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority level for geometry manager !");
      set_logging_priority(lp);

      std::string key;
      if (setup_.has_key(key = "sigma_t_gamma_interaction")) {
        _sigma_t_gamma_interaction_ = setup_.fetch_real(key);
        if (! setup_.has_explicit_unit(key)) {
          _sigma_t_gamma_interaction_ *= CLHEP::ns;
        }
      }

      _set_initialized(true);
      return;
    }

    void tof_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

  int tof_driver::_process_algo(const snemo::datamodel::particle_track & particle_1_,
                                const snemo::datamodel::particle_track & particle_2_,
                                double & proba_int_, double & proba_ext_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      const datatools::properties & aux_1 = particle_1_.get_auxiliaries();
      const datatools::properties & aux_2 = particle_2_.get_auxiliaries();

      if(!aux_1.has_key(snemo::datamodel::pid_utils::pid_label_key()) ||
         !aux_2.has_key(snemo::datamodel::pid_utils::pid_label_key()))
        DT_THROW_IF(true, std::logic_error,
                    "Missing information on the particle type !");

      const std::string label_1 = aux_1.fetch_string(snemo::datamodel::pid_utils::pid_label_key());
      const std::string label_2 = aux_2.fetch_string(snemo::datamodel::pid_utils::pid_label_key());

      // C'est pas du C, c'est juste plus propre
      double E1, E2;
      double track_length_1, track_length_2;
      double t1, t2, sigma_t1, sigma_t2;

      double m1 = _get_mass(particle_1_) * CLHEP::MeV;
      double m2 = _get_mass(particle_2_) * CLHEP::MeV;

      //either specialize the methods or consider the case here

      if(label_1 == snemo::datamodel::pid_utils::electron_label() &&
         label_2 == snemo::datamodel::pid_utils::electron_label())
        {
          E1 = _get_energy(particle_1_);
          E2 = _get_energy(particle_2_);

          _get_times(particle_1_, t1, sigma_t1);
          _get_times(particle_2_, t2, sigma_t2);

          _get_track_length(particle_1_, particle_2_, track_length_1, track_length_2);
        }

      //should take care in the topology driver not to feed two gamma
      if(label_1 == snemo::datamodel::pid_utils::gamma_label() ||
         label_2 == snemo::datamodel::pid_utils::gamma_label())
        {
          /* /!\ For now, suppose the gamma */

          E1 = 1.; // not relevant for gamma since beta=1
          E2 = 1.; // but still, not 0 because involved in fraction

          double t1, t2, sigma_t1, sigma_t2;
          _get_times(particle_1_, t1, sigma_t1);
          _get_times(particle_2_, t2, sigma_t2);

          _get_track_length(particle_1_, particle_2_, track_length_1, track_length_2);

        }

      double t1_first, t1_last, t2_first, t2_last;
      double sigma_t1_first, sigma_t1_last, sigma_t2_first, sigma_t2_last;

      _get_times(particle_1_, t1_first, sigma_t1_first, t1_last, sigma_t1_last);
      _get_times(particle_2_, t2_first, sigma_t2_first, t2_last, sigma_t2_last);

      std::cout << "E1 : " <<  E1 << std::endl
                << "E2 : " <<  E2 << std::endl;


      _get_track_length(particle_1_, particle_2_, track_length_1, track_length_2);

      std::cout << "track length 1 : " <<  track_length_1 << std::endl
                << "track length 2 : " <<  track_length_2 << std::endl
                << "t1_first " << t1_first << " +/- " << sigma_t1_first << std::endl
                << "t1_last " << t1_last << " +/- " << sigma_t1_last << std::endl
                << "t2_first " << t2_first << " +/- " << sigma_t2_first << std::endl
                << "t2_last " << t2_last << " +/- " << sigma_t2_last << std::endl;



      const double t1_th = _get_theoretical_time(E1,m1,track_length_1);
      const double t2_th = _get_theoretical_time(E2,m2,track_length_2);

      std::cout << "t1 th : " <<  t1_th << std::endl
                << "t2_th : " <<  t2_th << std::endl;

      const double t1_first_th = track_length_1 / CLHEP::c_light;
      const double t2_first_th = track_length_2 / CLHEP::c_light;

      std::cout << "t1 first_th : " <<  t1_first_th << std::endl
                << "t2_first_th : " <<  t2_first_th << std::endl;

      const double sigma_l = 0.6 * CLHEP::ns;

      const double sigma_exp_int = pow(sigma_t1_first,2)
        + pow(sigma_t2_first,2) + pow(sigma_l,2);
      const double sigma_exp_ext = pow(sigma_t1_first,2)
        + pow(sigma_t2_first,2) + pow(sigma_l,2);

      const double chi2_int = pow(t1_first - t2_first - (t1_first_th - t2_first_th) ,2)/sigma_exp_int;
      const double chi2_ext = pow(std::abs(t1_first - t2_first) - (t1_first_th + t2_first_th) ,2)/sigma_exp_ext;
      //in this case, the external particle is the one with the "smaller" time

      std::cout << "P_int " << gsl_cdf_chisq_Q(chi2_int, 1)<< std::endl;
      std::cout << "P_ext " << gsl_cdf_chisq_Q(chi2_ext, 1)<< std::endl;
      //      proba_ = gsl_cdf_chisq_Q(chi2_int, 1);

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

    double tof_driver::_get_energy(const snemo::datamodel::particle_track & particle_)
    {
      if (! particle_.has_associated_calorimeter_hits()) {
        DT_THROW_IF(true, std::logic_error,
                    "Particle track is not associated to any calorimeter block !");
      }

      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters = particle_.get_associated_calorimeter_hits ();

      if (the_calorimeters.size() >= 2) {
        DT_THROW_IF(true, std::logic_error,
                    "Particle track is associated to more than 1 calorimeter block !");
      }

      return the_calorimeters.front().get().get_energy();
    }

    double tof_driver::_get_theoretical_time(double energy_, double mass_, double track_length_)
    {
      return track_length_ / (tof_driver::_beta(energy_, mass_) * CLHEP::c_light);
    }

    double tof_driver::_beta(double energy_, double mass_)
    {
      return std::sqrt(energy_ * (energy_ + 2.*mass_)) / (energy_ + mass_);
    }

    int tof_driver::_get_times(const snemo::datamodel::particle_track & particle_,
                               double & t_, double & sigma_t_)
    {
      if (!particle_.has_associated_calorimeter_hits()) {
        DT_THROW_IF(true, std::logic_error,
                    "Particle track is not associated to any calorimeter block !");
      }

      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters = particle_.get_associated_calorimeter_hits ();

      if (the_calorimeters.size() >= 2) {
        DT_THROW_IF(true, std::logic_error,
                    "Particle track is associated to more than 1 calorimeter block !");
      }

      t_ = the_calorimeters.front().get().get_time();
      sigma_t_ = the_calorimeters.front().get().get_sigma_time();

      return 0;
    }

    int tof_driver::_get_times(const snemo::datamodel::particle_track & particle_,
                               double & t_first_, double & sigma_t_first_,
                               double & t_last_, double & sigma_t_last_)
    {

      if (! particle_.has_associated_calorimeter_hits()) {
        DT_THROW_IF(true, std::logic_error,
                    "Particle track is not associated to any calorimeter block !");
      }

      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters = particle_.get_associated_calorimeter_hits ();

      // if (the_calorimeters.size() >= 2)
      //   {
      //     DT_THROW_IF(true, std::logic_error,
      //                 "Particle track is associated to more than 1 calorimeter block !");
      //     return 0;
      //   }

      t_first_ = the_calorimeters.front().get().get_time();
      sigma_t_first_ = the_calorimeters.front().get().get_sigma_time();
      t_last_ = the_calorimeters.back().get().get_time();
      sigma_t_last_ = the_calorimeters.back().get().get_sigma_time();

      return 0;
    }

    double tof_driver::_get_mass(const snemo::datamodel::particle_track & particle_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      const datatools::properties & aux = particle_.get_auxiliaries();

      /* probably a cleaner way to do it*/
      if(!aux.has_key(snemo::datamodel::pid_utils::pid_label_key()))
        DT_THROW_IF(true, std::logic_error,
                    "Missing information on the particle type !");

      const std::string label = aux.fetch_string(snemo::datamodel::pid_utils::pid_label_key());

#warning get mass from CLHEP
      if(label == snemo::datamodel::pid_utils::electron_label())
        return 0.511;
      else if(label == snemo::datamodel::pid_utils::gamma_label())
        return 0;
      else
        DT_THROW_IF(true, std::logic_error,
                    "Particle type inappropriate for TOF calculations !");

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

    int tof_driver::_get_track_length(const snemo::datamodel::particle_track & pt1_,
                                      const snemo::datamodel::particle_track & pt2_,
                                      double & tl1_, double & tl2_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      const datatools::properties & aux_1 = pt1_.get_auxiliaries();
      const datatools::properties & aux_2 = pt2_.get_auxiliaries();

      /*probably a cleaner way to do it*/
      if(! aux_1.has_key(snemo::datamodel::pid_utils::pid_label_key()) ||
         ! aux_2.has_key(snemo::datamodel::pid_utils::pid_label_key()))
        DT_THROW_IF(true, std::logic_error,
                    "Missing pid_label information on a particle type !");

      const std::string label_1 = aux_1.fetch_string(snemo::datamodel::pid_utils::pid_label_key());
      const std::string label_2 = aux_2.fetch_string(snemo::datamodel::pid_utils::pid_label_key());

      if(label_1 == snemo::datamodel::pid_utils::electron_label())
        {
          tl1_ = _get_electron_track_length(pt1_);// * CLHEP::mm;
        }
      else if(label_1 == snemo::datamodel::pid_utils::gamma_label())
        {
          tl1_ = _get_gamma_track_length(pt1_,pt2_);  // supposing there is not more than one gamma
        }
      else
        DT_THROW_IF(true, std::logic_error,
                    "Particle type inappropriate for TOF calculations !");

      if(label_2 == snemo::datamodel::pid_utils::electron_label())
        {
          tl2_ = _get_electron_track_length(pt2_);
        }
      else if(label_2 == snemo::datamodel::pid_utils::gamma_label())
        {
          tl2_ = _get_gamma_track_length(pt2_,pt1_); // supposing there is not more than one gamma
        }
      else
        DT_THROW_IF(true, std::logic_error,
                    "Particle type inappropriate for TOF calculations !");

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");

      return 0;
    }

    double tof_driver::_get_electron_track_length(const snemo::datamodel::particle_track & particle_)
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

      DT_THROW_IF(true, std::logic_error,
                  "Track is neither a line nor an helix !");

      return 1;
    }

    double tof_driver::_get_gamma_track_length(const snemo::datamodel::particle_track & ptg_,
                                               const snemo::datamodel::particle_track & pte_)
    {
      /*the first vertex on calorimeter is the first in time but is it always true ???*/
      if(!pte_.has_vertices())
        DT_THROW_IF(true, std::logic_error,
                    "Electron has no vertices associated !");

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices = pte_.get_vertices();
      geomtools::vector_3d electron_foil_vertex;
      bool found_calorimeter_vertex = false;
      for(snemo::datamodel::particle_track::vertex_collection_type::const_iterator i_vtx = the_vertices.begin(); i_vtx<the_vertices.end(); ++i_vtx)
        {
          if(!snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(i_vtx->get()) || !snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(i_vtx->get())
             || !snemo::datamodel::particle_track::vertex_is_on_gamma_veto(i_vtx->get()))
            continue;

          found_calorimeter_vertex = true;
          electron_foil_vertex = i_vtx->get().get_position();
          break;
        }
      if(!found_calorimeter_vertex)
        DT_THROW_IF(true, std::logic_error,
                    "Gamma particle has no vertices on the calorimeter !");

      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters = ptg_.get_associated_calorimeter_hits ();

      // different track length for internal and external hypothesis

      return 0; // for now
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
