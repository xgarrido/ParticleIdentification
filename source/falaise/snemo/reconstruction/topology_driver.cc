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

#include <falaise/snemo/datamodels/topology_2e_pattern.h>
#include <falaise/snemo/datamodels/topology_1e1g_pattern.h>
#include <falaise/snemo/datamodels/topology_2e1g_pattern.h>

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

    bool topology_driver::has_geometry_manager() const
    {
      return _geometry_manager_ != 0;
    }

    void topology_driver::set_geometry_manager(const geomtools::manager & gmgr_)
    {
      DT_THROW_IF (is_initialized(), std::logic_error, "Already initialized/locked !");
      _geometry_manager_ = &gmgr_;
      return;
    }

    const geomtools::manager & topology_driver::get_geometry_manager() const
    {
      DT_THROW_IF (! has_geometry_manager(), std::logic_error, "No geometry manager is setup !");
      return *_geometry_manager_;
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

        if (a_driver_name == "TD") {
          continue;
        }
        if (a_driver_name == "TOFD") {
          // Initialize TOF Driver
          _TOFD_.reset(new snemo::reconstruction::tof_driver);
          datatools::properties TOFD_config;
          setup_.export_and_rename_starting_with(TOFD_config, std::string(a_driver_name + "."), "");
          _TOFD_->initialize(TOFD_config);
        } else if (a_driver_name == "DVD") {
          // Initialize Delta Vertices Driver
          _DVD_.reset(new snemo::reconstruction::delta_vertices_driver);
          // _DVD_->set_geometry_manager(get_geometry_manager());
          datatools::properties DVD_config;
          setup_.export_and_rename_starting_with(DVD_config, std::string(a_driver_name + "."), "");
          _DVD_->initialize(DVD_config);
        } else if (a_driver_name == "AMD") {
          // Initialize Delta Vertices Driver
          _AMD_.reset(new snemo::reconstruction::angle_measurement_driver);
          // _AMD_->set_geometry_manager(get_geometry_manager());
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

      if (a_classification == "2e") {
        this->_fill_2e_topology_(ptd_, td_);
      }
      else if (a_classification == "1e1g") {
        this->_fill_1e1g_topology_(ptd_, td_);
      } else {
        DT_LOG_WARNING(get_logging_priority(),
                       "Event classification '" << a_classification << "' unsupported !");
      }

      // aux_td.dump();
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

    void topology_driver::_fill_2e_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                             snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      // typedef datatools::handle< snemo::datamodel::base_topology_pattern > h_pattern;
      snemo::datamodel::topology_2e_pattern * t2ep = new snemo::datamodel::topology_2e_pattern;
      h_pattern.reset(t2ep);
      td_.set_pattern_handle(h_pattern);

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();
      const snemo::datamodel::particle_track & pt1 = the_particles.front().get();
      const snemo::datamodel::particle_track & pt2 = the_particles.back().get();

      if (pt1.has_associated_calorimeter_hits() && pt2.has_associated_calorimeter_hits()) {
        // To be replaced by dedicated fields of 'topology_2e_pattern'
        double proba_int = datatools::invalid_real();
        double proba_ext = datatools::invalid_real();
        _TOFD_->process(pt1, pt2, proba_int, proba_ext);
        t2ep->set_internal_probability(proba_int);
        t2ep->set_external_probability(proba_ext);

      } else {
        DT_LOG_DEBUG(get_logging_priority(),
                     "Electron particles do not have associated calorimeter hit !");
      }

      double delta_vertices_y = datatools::invalid_real();
      double delta_vertices_z = datatools::invalid_real();

      _DVD_->process(pt1, pt2, delta_vertices_y, delta_vertices_z);
      t2ep->set_delta_vertices_y(delta_vertices_y);
      t2ep->set_delta_vertices_z(delta_vertices_z);

      // if(std::abs(delta_vertices_y) < 50. && std::abs(delta_vertices_z) < 50.) {
      if(1) {
        double angle = datatools::invalid_real();
        _AMD_->process(pt1, pt2, angle);
      }
      // td_.tree_dump();

      if (get_logging_priority() >= datatools::logger::PRIO_DEBUG) {
        DT_LOG_DEBUG(get_logging_priority(), "Topology data dump :");
        td_.tree_dump();
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

      if (pt1.has_associated_calorimeter_hits() && pt2.has_associated_calorimeter_hits()) {
        // To be replaced by dedicated fields of 'topology_1e1g_pattern'
        double proba_int = datatools::invalid_real();
        double proba_ext = datatools::invalid_real();
        _TOFD_->process(pt1, pt2, proba_int, proba_ext);
        t1e1gp->set_internal_probability(proba_int);
        t1e1gp->set_external_probability(proba_ext);
      } else {
        DT_LOG_DEBUG(get_logging_priority(),
                     "Electron and gamma particles do not have associated calorimeter hit !");
      }

      double angle = datatools::invalid_real();
      _AMD_->process(pt1, pt2, angle);

      if (get_logging_priority() >= datatools::logger::PRIO_DEBUG) {
        DT_LOG_DEBUG(get_logging_priority(), "Topology data dump :");
        td_.tree_dump();
      }
      return;
    }

    void topology_driver::_fill_2e1g_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                               snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_2e1g_pattern * t2e1gp = new snemo::datamodel::topology_2e1g_pattern;
      h_pattern.reset(t2e1gp);
      td_.set_pattern_handle(h_pattern);

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();

      // const snemo::datamodel::particle_track & pt1 = the_particles.at(0).get();
      // const snemo::datamodel::particle_track & pt2 = the_particles.at(1).get();
      // const snemo::datamodel::particle_track & pt3 = the_particles.at(the_particles.size()-1).get();

      // for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
      //        iparticle = the_particles.begin();
      //      iparticle != the_particles.back();
      //      ++iparticle) {
      //   for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
      //          iparticle_next = iparticle;
      //        iparticle != the_particles.end();
      //        ++iparticle) {

      for (size_t i_particle = 0; i_particle != the_particles.size()-1; ++i_particle) {
        for (size_t j_particle = i_particle + 1; j_particle != the_particles.size(); ++j_particle) {

          const snemo::datamodel::particle_track & pt_i = the_particles.at(i_particle).get();
          const snemo::datamodel::particle_track & pt_j = the_particles.at(j_particle).get();

          // To be replaced by dedicated fields of 'topology_2e1g_pattern'

          double proba_int = datatools::invalid_real();
          double proba_ext = datatools::invalid_real();

          _TOFD_->process(pt_i, pt_j, proba_int, proba_ext);

          if(datatools::is_valid(proba_int) && datatools::is_valid(proba_ext)) {
            t2e1gp->set_internal_probability(proba_int);
            t2e1gp->set_external_probability(proba_ext);
          }
        }
      }

      if (get_logging_priority() >= datatools::logger::PRIO_DEBUG) {
        DT_LOG_DEBUG(get_logging_priority(), "Topology data dump :");
        td_.tree_dump();
      }
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
