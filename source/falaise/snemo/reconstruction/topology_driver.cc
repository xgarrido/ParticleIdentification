/// \file falaise/snemo/reconstruction/topology_driver.cc

// Ourselves:
#include <snemo/reconstruction/topology_driver.h>

// Third party:
// - Bayeux/cuts:
#include <bayeux/cuts/cut_manager.h>

// This project:
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/pid_utils.h>

#include <falaise/snemo/datamodels/topology_1e_pattern.h>
#include <falaise/snemo/datamodels/topology_2e_pattern.h>
#include <falaise/snemo/datamodels/topology_1e1p_pattern.h>
#include <falaise/snemo/datamodels/topology_1e1g_pattern.h>
#include <falaise/snemo/datamodels/topology_2eNg_pattern.h>

#include <snemo/reconstruction/tof_driver.h>
#include <snemo/reconstruction/delta_vertices_driver.h>
#include <snemo/reconstruction/angle_measurement_driver.h>

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

        if (a_driver_name == "TOFD") {
          // Initialize TOF Driver
          _TOFD_.reset(new snemo::reconstruction::tof_driver);
          datatools::properties TOFD_config;
          setup_.export_and_rename_starting_with(TOFD_config, std::string(a_driver_name + "."), "");
          _TOFD_->initialize(TOFD_config);
        } else if (a_driver_name == "DVD") {
          // Initialize Delta Vertices Driver
          _DVD_.reset(new snemo::reconstruction::delta_vertices_driver);
          datatools::properties DVD_config;
          setup_.export_and_rename_starting_with(DVD_config, std::string(a_driver_name + "."), "");
          _DVD_->initialize(DVD_config);
        } else if (a_driver_name == "AMD") {
          // Initialize Delta Vertices Driver
          _AMD_.reset(new snemo::reconstruction::angle_measurement_driver);
          datatools::properties AMD_config;
          setup_.export_and_rename_starting_with(AMD_config, std::string(a_driver_name + "."), "");
          _AMD_->initialize(AMD_config);
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

      status = _process_algo(ptd_,td_);
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
      _TOFD_.reset(0);
      _DVD_.reset(0);
      _AMD_.reset(0);

      return;
    }

    int topology_driver::_process_algo(const snemo::datamodel::particle_track_data & ptd_,
                                       snemo::datamodel::topology_data & td_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      const std::string a_classification = topology_driver::_build_classification_(ptd_);
      DT_LOG_TRACE(get_logging_priority(), "Event classification : " << a_classification);

      if (a_classification == "1e") {
        this->_fill_1e_topology_(ptd_, td_);
      }
      else if (a_classification == "2e") {
        this->_fill_2e_topology_(ptd_, td_);
      }
      else if (a_classification == "1e1p") {
        this->_fill_2e_topology_(ptd_, td_);
      }
      else if (a_classification == "1e1g") {
        this->_fill_1e1g_topology_(ptd_, td_);
      }
      else if (a_classification == "2e1g" ||
               a_classification == "2e2g" ||
               a_classification == "2e3g" ||
               a_classification == "2e4g" ||
               a_classification == "2e5g") {
        this->_fill_2eNg_topology_(ptd_, td_);
      } else {
        DT_LOG_WARNING(get_logging_priority(),
                       "Event classification '" << a_classification << "' unsupported !");
      }

      if (get_logging_priority() >= datatools::logger::PRIO_DEBUG) {
        DT_LOG_DEBUG(get_logging_priority(), "Topology data dump :");
        td_.tree_dump();
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return 0;
    }

    std::string topology_driver::_build_classification_(const snemo::datamodel::particle_track_data & ptd_)
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
      return classification.str();

    }

    void topology_driver::_fill_1e_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                             snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_1e_pattern * t1ep = new snemo::datamodel::topology_1e_pattern;
      h_pattern.reset(t1ep);
      td_.set_pattern_handle(h_pattern);

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();
      const snemo::datamodel::particle_track & pt = the_particles.front().get();

      double angle = datatools::invalid_real();
      if (_AMD_) _AMD_->process(pt, angle);

      return;
    }

    void topology_driver::_fill_2e_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                             snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_2e_pattern * t2ep = new snemo::datamodel::topology_2e_pattern;
      h_pattern.reset(t2ep);
      td_.set_pattern_handle(h_pattern);

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();
      const snemo::datamodel::particle_track & pt1 = the_particles.front().get();
      const snemo::datamodel::particle_track & pt2 = the_particles.back().get();

      double proba_int = datatools::invalid_real();
      double proba_ext = datatools::invalid_real();

      if (_TOFD_) _TOFD_->process(pt1, pt2, proba_int, proba_ext);

      if(datatools::is_valid(proba_int))
        t2ep->set_internal_probability(proba_int);
      if(datatools::is_valid(proba_ext))
        t2ep->set_external_probability(proba_ext);

      double delta_vertices_y = datatools::invalid_real();
      double delta_vertices_z = datatools::invalid_real();
      if (_DVD_) _DVD_->process(pt1, pt2, delta_vertices_y, delta_vertices_z);

      if(datatools::is_valid(delta_vertices_y))
        t2ep->set_delta_vertices_y(delta_vertices_y);
      if(datatools::is_valid(delta_vertices_z))
        t2ep->set_delta_vertices_z(delta_vertices_z);

      if(std::abs(delta_vertices_y) < 100. && std::abs(delta_vertices_z) < 100.
         && proba_int > 0.04 && proba_ext < 0.01) {
        double angle = datatools::invalid_real();
        if (_AMD_) _AMD_->process(pt1, pt2, angle);
        if(datatools::is_valid(angle))
          t2ep->set_angle(angle);
      }
      return;
    }

    void topology_driver::_fill_1e1p_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                               snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_1e1p_pattern * t1e1pp = new snemo::datamodel::topology_1e1p_pattern;
      h_pattern.reset(t1e1pp);
      td_.set_pattern_handle(h_pattern);

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();
      const snemo::datamodel::particle_track & pt1 = the_particles.front().get();
      const snemo::datamodel::particle_track & pt2 = the_particles.back().get();

      double proba_int = datatools::invalid_real();
      double proba_ext = datatools::invalid_real();

      if (_TOFD_) _TOFD_->process(pt1, pt2, proba_int, proba_ext);

      if(datatools::is_valid(proba_int))
        t1e1pp->set_internal_probability(proba_int);
      if(datatools::is_valid(proba_ext))
        t1e1pp->set_external_probability(proba_ext);

      double delta_vertices_y = datatools::invalid_real();
      double delta_vertices_z = datatools::invalid_real();
      if (_DVD_) _DVD_->process(pt1, pt2, delta_vertices_y, delta_vertices_z);

      if(datatools::is_valid(delta_vertices_y))
        t1e1pp->set_delta_vertices_y(delta_vertices_y);
      if(datatools::is_valid(delta_vertices_z))
        t1e1pp->set_delta_vertices_z(delta_vertices_z);

      if(std::abs(delta_vertices_y) < 100. && std::abs(delta_vertices_z) < 100.
         && proba_int > 0.04 && proba_ext < 0.01) {
        double angle = datatools::invalid_real();
        if (_AMD_) _AMD_->process(pt1, pt2, angle);
        if(datatools::is_valid(angle))
          t1e1pp->set_angle(angle);
      }
      return;
    }

    void topology_driver::_fill_1e1g_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                               snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_1e1g_pattern * t1e1gp = new snemo::datamodel::topology_1e1g_pattern;
      h_pattern.reset(t1e1gp);
      td_.set_pattern_handle(h_pattern);

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();
      const snemo::datamodel::particle_track & pt1 = the_particles.front().get();
      const snemo::datamodel::particle_track & pt2 = the_particles.back().get();

      std::vector<double> proba_int = std::numeric_limits< std::vector<double> >::quiet_NaN();
      std::vector<double> proba_ext = std::numeric_limits< std::vector<double> >::quiet_NaN();

      if (_TOFD_) _TOFD_->process(pt1, pt2, proba_int, proba_ext);

      // double proba_int = datatools::invalid_real();
      // double proba_ext = datatools::invalid_real();

      // check validity for vector
      t1e1gp->set_internal_probability(proba_int);
      t1e1gp->set_external_probability(proba_ext);

      // if(proba_int > 0.04 && proba_ext < 0.01) {
      //condition or not ?
      if(1) {
        double angle = datatools::invalid_real();
        if (_AMD_) _AMD_->process(pt1, pt2, angle);
        if(datatools::is_valid(angle))
          t1e1gp->set_angle(angle);
      }

      return;
    }

    void topology_driver::_fill_2eNg_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                               snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_2eNg_pattern * t2eNgp = new snemo::datamodel::topology_2eNg_pattern;
      h_pattern.reset(t2eNgp);
      td_.set_pattern_handle(h_pattern);

      const datatools::properties & aux = ptd_.get_auxiliaries();
      DT_THROW_IF(! aux.has_key(snemo::datamodel::pid_utils::gamma_label()),
                  std::logic_error, "No gammas have been found in this event !");
      t2eNgp->set_number_of_gammas(aux.fetch_integer(snemo::datamodel::pid_utils::gamma_label()));

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();

      for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
             i_particle = the_particles.begin();
           i_particle != boost::prior(the_particles.end()); ++i_particle) {
        for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
               j_particle = boost::next(i_particle);
             j_particle != the_particles.end(); ++j_particle) {

          snemo::datamodel::topology_2eNg_pattern::TOF_collection_type & tofs
            = t2eNgp->grab_TOF_collection();
          {
            snemo::datamodel::TOF_measurement dummy;
            tofs.push_back(dummy);
          }
          snemo::datamodel::TOF_measurement & a_tof = tofs.back();;
          if (_TOFD_) _TOFD_->process(i_particle->get(), j_particle->get(),
                                      a_tof.grab_internal_probabilities(),
                                      a_tof.grab_external_probabilities());
          // Still need to add the handle...
        // if(tof_measurement.internal_probability.front()>0.04 && tof_measurement.external_probability.front()<0.01) {
        //   double angle = datatools::invalid_real();
        //   if (_AMD_)
        //     _AMD_->process(i_particle->get(), j_particle->get(), angle);
        // }
        }
      }

      //      t2eNgp->tree_dump();
      return;
    }

  }  // end of namespace reconstruction

}  // end of namespace snemo
/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::topology_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::topology_driver");
  ocd_.set_class_description("A driver class for the Topology algorithm");
  ocd_.set_class_library("Falaise_Topology");
  ocd_.set_class_documentation("The driver manager for the topoogy algorithm\n"
                               "/todo What does the manager do ?"
                               );


  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::topology_driver,
                               "snemo::reconstruction::topology_driver")
