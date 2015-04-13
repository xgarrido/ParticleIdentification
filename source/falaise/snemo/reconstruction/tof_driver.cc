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
      // _set_initialized(false);
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
                            std::vector<double> & proba_int_, std::vector<double> & proba_ext_)
    {
      int status = 0;

      status = _process_algo(pt1_, pt2_, proba_int_, proba_ext_);

      if (status != 0) {
        DT_LOG_ERROR(get_logging_priority(),
                     "Computing TOF measurements with '" << tof_id() << "' algorithm has failed !");
        return status;
      }

      return status;
    }

    void tof_driver::_set_defaults()
    {
      _initialized_ = false;
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      _sigma_t_gamma_interaction_ = 0.6 * CLHEP::ns;
      return;
    }

    // Initialization :
    void tof_driver::initialize(const datatools::properties & setup_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Driver '" << tof_id() << "' is already initialized !");

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

      // _set_initialized(true);
      return;
    }

    void tof_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

    int tof_driver::_process_algo(const snemo::datamodel::particle_track & pt1_,
                                  const snemo::datamodel::particle_track & pt2_,
                                  std::vector<double> & proba_int_, std::vector<double> & proba_ext_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // // Invalidate results
      // datatools::invalidate(proba_int_);
      // datatools::invalidate(proba_ext_);

      if (! pt1_.has_associated_calorimeter_hits() ||
          ! pt2_.has_associated_calorimeter_hits()) {
        DT_LOG_WARNING(get_logging_priority(), "No associated calorimeter !");
        return 1;
      }

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) &&
          snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        DT_LOG_WARNING(get_logging_priority(), "TOF calculation can not be done for 2 gammas !");
        return 1;
      }

      // Either specialize the methods or consider the case here
      if (! snemo::datamodel::pid_utils::particle_is_gamma(pt1_) &&
          ! snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        _process_charged_particles(pt1_, pt2_, proba_int_, proba_ext_);
      } else if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ||
                 snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        _process_charged_gamma_particles(pt1_, pt2_, proba_int_, proba_ext_);
      } else {
        DT_LOG_WARNING(get_logging_priority(), "Topology not supported !");
        return 1;
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return 0;
    }

    void tof_driver::_process_charged_particles(const snemo::datamodel::particle_track & pt1_,
                                                const snemo::datamodel::particle_track & pt2_,
                                                std::vector<double> & proba_int_, std::vector<double> & proba_ext_)
    {
      // Compute theoretical times given energy, mass and track length
      const double E1 = _get_energy(pt1_);
      const double E2 = _get_energy(pt2_);
      const double tl1 = _get_charged_particle_track_length(pt1_);
      const double tl2 = _get_charged_particle_track_length(pt2_);
      const double m1 = _get_mass(pt1_);
      const double m2 = _get_mass(pt2_);
      const double t1_th = _get_theoretical_time(E1, m1, tl1);
      const double t2_th = _get_theoretical_time(E2, m2, tl2);
      DT_LOG_DEBUG(get_logging_priority(), "t1 th : " << t1_th/CLHEP::ns << " ns");
      DT_LOG_DEBUG(get_logging_priority(), "t2 th : " << t2_th/CLHEP::ns << " ns");

      double t1, t2;
      double sigma_t1, sigma_t2;
      _get_time(pt1_, t1, sigma_t1);
      _get_time(pt2_, t2, sigma_t2);
      DT_LOG_DEBUG(get_logging_priority(), "t1 meas. : " << t1/CLHEP::ns << " ns");
      DT_LOG_DEBUG(get_logging_priority(), "t2 meas. : " << t2/CLHEP::ns << " ns");

      const double sigma_l = 0.6 * CLHEP::ns;
      const double sigma_exp
        = std::pow(sigma_t1, 2) + std::pow(sigma_t2, 2) + std::pow(sigma_l, 2);

      const double chi2_int = std::pow(t1 - t2 - (t1_th - t2_th), 2)/sigma_exp;
      const double chi2_ext = std::pow(std::abs(t1 - t2) - (t1_th + t2_th), 2)/sigma_exp;

      proba_int_.push_back(gsl_cdf_chisq_Q(chi2_int, 1));
      proba_ext_.push_back(gsl_cdf_chisq_Q(chi2_ext, 1));

      DT_LOG_DEBUG(get_logging_priority(), "P_int " << proba_int_.front());
      DT_LOG_DEBUG(get_logging_priority(), "P_ext " << proba_ext_.front());
      return;
    }

    void tof_driver::_process_charged_gamma_particles(const snemo::datamodel::particle_track & pt1_,
                                                      const snemo::datamodel::particle_track & pt2_,
                                                      std::vector<double> & proba_int_, std::vector<double> & proba_ext_)
    {
      const snemo::datamodel::particle_track & a_gamma
        = (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ? pt1_ : pt2_);
      const snemo::datamodel::particle_track & a_charged
        = (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ? pt2_ : pt1_);

      // Compute theoretical times given energy, mass and track length
      const double E1 = _get_energy(a_charged);
      const double E2 = 1; // dummy, non-zero value
      const double m1 = _get_mass(a_charged);
      const double m2 = _get_mass(a_gamma);

      const double tl1 = _get_charged_particle_track_length(a_charged);
      const double t1_th = _get_theoretical_time(E1, m1, tl1);
      double t1, sigma_t1;
      _get_time(a_charged, t1, sigma_t1);
      DT_LOG_DEBUG(get_logging_priority(), "t1 meas. : " << t1/CLHEP::ns << " ns");

      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_gamma_calorimeters = a_gamma.get_associated_calorimeter_hits ();

      snemo::datamodel::particle_track::vertex_collection_type the_gamma_calos_vertices;
      a_gamma.fetch_vertices(the_gamma_calos_vertices, snemo::datamodel::particle_track::VERTEX_ON_MAIN_CALORIMETER |
                             snemo::datamodel::particle_track::VERTEX_ON_X_CALORIMETER              |
                             snemo::datamodel::particle_track::VERTEX_ON_GAMMA_VETO);

      for (snemo::datamodel::particle_track::vertex_collection_type::iterator
             ivtx = the_gamma_calos_vertices.begin();
           ivtx != the_gamma_calos_vertices.end(); ++ivtx) {

        double tl2, t2, sigma_t2;
        _get_vertex_to_calo_info(a_charged, the_gamma_calorimeters, ivtx->get(),
                                 tl2, t2, sigma_t2);

        const double t2_th = _get_theoretical_time(E2, m2, tl2);
        const double sigma_l = 0.6 * CLHEP::ns;
        const double sigma_exp = std::pow(sigma_t1, 2) + std::pow(sigma_t2, 2)
                                 + std::pow(sigma_l, 2);
        const double chi2_int = std::pow(t1 - t2 - (t1_th - t2_th), 2)/sigma_exp;
        const double chi2_ext = std::pow(std::abs(t1 - t2) - (t1_th + t2_th), 2)/sigma_exp;

        proba_int_.push_back(gsl_cdf_chisq_Q(chi2_int, 1));
        proba_ext_.push_back(gsl_cdf_chisq_Q(chi2_ext, 1));

        // DT_LOG_DEBUG(get_logging_priority(), "P_int " << proba_int_.back());
        // DT_LOG_DEBUG(get_logging_priority(), "P_ext " << proba_ext_.back());
      }
      return;
    }

    double tof_driver::_get_energy(const snemo::datamodel::particle_track & particle_)
    {
      DT_THROW_IF(! particle_.has_associated_calorimeter_hits(), std::logic_error,
                  "Particle track is not associated to any calorimeter block !");
      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters = particle_.get_associated_calorimeter_hits ();
      // DT_THROW_IF(the_calorimeters.size() >= 2, std::logic_error,
      //             "Particle track is associated to more than 1 calorimeter block !");
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

    void tof_driver::_get_time(const snemo::datamodel::particle_track & particle_,
                               double & t_, double & sigma_t_)
    {
      DT_THROW_IF(! particle_.has_associated_calorimeter_hits(), std::logic_error,
                  "Particle track is not associated to any calorimeter block !");
      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters = particle_.get_associated_calorimeter_hits ();
      // DT_THROW_IF(the_calorimeters.size() >= 2, std::logic_error,
      //             "Particle track is associated to more than 1 calorimeter block !");

      t_ = the_calorimeters.front().get().get_time();
      sigma_t_ = the_calorimeters.front().get().get_sigma_time();
      return;
    }

    void tof_driver::_get_time_external_hyp(const snemo::datamodel::particle_track & particle_,
                                            double & t_, double & sigma_t_)
    {
      DT_THROW_IF(! particle_.has_associated_calorimeter_hits(), std::logic_error,
                  "Particle track is not associated to any calorimeter block !");
      const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
        the_calorimeters = particle_.get_associated_calorimeter_hits ();

      t_ = the_calorimeters.back().get().get_time();
      sigma_t_ = the_calorimeters.back().get().get_sigma_time();
      return;
    }

    double tof_driver::_get_mass(const snemo::datamodel::particle_track & particle_)
    {
      double mass = datatools::invalid_real();

      if (snemo::datamodel::pid_utils::particle_is_electron(particle_) ||
          snemo::datamodel::pid_utils::particle_is_positron(particle_)) {
        mass = CLHEP::electron_mass_c2;
      } else if (snemo::datamodel::pid_utils::particle_is_gamma(particle_)) {
        mass = 0.0 * CLHEP::eV;
      } else if (snemo::datamodel::pid_utils::particle_is_alpha(particle_)) {
        mass = 3.727417 * CLHEP::GeV;
      } else {
        DT_THROW_IF(true, std::logic_error,
                    "Particle type inappropriate for TOF calculations !");
      }
      return mass;
    }

    double tof_driver::_get_charged_particle_track_length(const snemo::datamodel::particle_track & particle_)
    {
      DT_THROW_IF(! particle_.has_trajectory(), std::logic_error,
                  "Particle has no attached trajectory !");
      const snemo::datamodel::tracker_trajectory & a_trajectory = particle_.get_trajectory();
      const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
      return a_track_pattern.get_shape().get_length();
    }

    double tof_driver::_get_gamma_track_length(const snemo::datamodel::particle_track & ptg_,
                                               const snemo::datamodel::particle_track & pte_,
                                               const bool external_hyp_)
    {
      /*the first vertex on calorimeter is the first in time but is it always true ???*/
      DT_THROW_IF(! pte_.has_vertices(), std::logic_error,
                  "Electron has no vertices associated !");

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices = pte_.get_vertices();
      geomtools::vector_3d electron_foil_vertex;
      geomtools::invalidate(electron_foil_vertex);
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivtx = the_vertices.begin(); ivtx < the_vertices.end(); ++ivtx) {
        const geomtools::blur_spot & a_vertex = ivtx->get();
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(a_vertex))
          continue;

        electron_foil_vertex = a_vertex.get_position();
        break;
      }
      DT_THROW_IF(! geomtools::is_valid(electron_foil_vertex), std::logic_error,
                  "Electron has no vertices on the calorimeter !");

      DT_THROW_IF(! ptg_.has_vertices(), std::logic_error,
                  "Gamma has no vertices associated !");
      const snemo::datamodel::particle_track::vertex_collection_type & the_gamma_vertices = ptg_.get_vertices();
      geomtools::vector_3d gamma_first_calo_vertex;
      geomtools::invalidate(gamma_first_calo_vertex);
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivtx = the_gamma_vertices.begin(); ivtx < the_gamma_vertices.end(); ++ivtx) {
        const geomtools::blur_spot & a_vertex = ivtx->get();
        if (snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(a_vertex) ||
            snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(a_vertex) ||
            snemo::datamodel::particle_track::vertex_is_on_gamma_veto(a_vertex)) {
          gamma_first_calo_vertex = a_vertex.get_position();
          if (! external_hyp_) break;
        }
      }

      DT_THROW_IF(! geomtools::is_valid(gamma_first_calo_vertex), std::logic_error,
                  "Gamma has no vertices on the calorimeter !");

      return (electron_foil_vertex - gamma_first_calo_vertex).mag();
    }

    int tof_driver::_get_vertex_to_calo_info(const snemo::datamodel::particle_track & ptc_,
                                             const snemo::datamodel::calibrated_calorimeter_hit::
                                             collection_type & the_gamma_calorimeters_,
                                             const geomtools::blur_spot & a_vertex_,
                                             double & track_length_,
                                             double & time_, double & sigma_time_)
    {
      /*the first vertex on calorimeter is the first in time but is it always true ???*/
      DT_THROW_IF(! ptc_.has_vertices(), std::logic_error,
                  "Electron has no vertices associated !");

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices = ptc_.get_vertices();
      geomtools::vector_3d electron_foil_vertex;
      geomtools::invalidate(electron_foil_vertex);
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivtx = the_vertices.begin(); ivtx < the_vertices.end(); ++ivtx) {
        const geomtools::blur_spot & a_vertex = ivtx->get();
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(a_vertex))
          continue;

        electron_foil_vertex = a_vertex.get_position();
        break;
      }

      DT_THROW_IF(! geomtools::is_valid(electron_foil_vertex), std::logic_error,
                  "Electron has no vertices on the calorimeter !");

      geomtools::base_hit::has_geom_id_predicate hit_pred(a_vertex_.get_geom_id());
      datatools::mother_to_daughter_predicate<geomtools::base_hit,
                                              snemo::datamodel::calibrated_calorimeter_hit> pred_M2D(hit_pred);
      datatools::handle_predicate<snemo::datamodel::calibrated_calorimeter_hit> pred_via_handle(pred_M2D);
      snemo::datamodel::calibrated_calorimeter_hit::collection_type::const_iterator
        found = std::find_if(the_gamma_calorimeters_.begin(), the_gamma_calorimeters_.end(), pred_via_handle);
      DT_THROW_IF(found == the_gamma_calorimeters_.end(), std::logic_error,
                  "Calibrated calorimeter hit with id " << a_vertex_.get_geom_id()
                  << " can not be found");

      track_length_ = (electron_foil_vertex - a_vertex_.get_position()).mag();

      const snemo::datamodel::calibrated_calorimeter_hit & a_calo_hit = found->get();
      time_ = a_calo_hit.get_time();
      sigma_time_ = a_calo_hit.get_sigma_time();

      return 0;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

  /* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::tof_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::tof_driver");
  ocd_.set_class_description("A driver class to measure Time-Of-Flight aka TOF");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("The driver manager for the TOF measurement\n"
                               "/todo What does the manager do ?"
                               );


  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::tof_driver,
                               "snemo::reconstruction::tof_driver")
