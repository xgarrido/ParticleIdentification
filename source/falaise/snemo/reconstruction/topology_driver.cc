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
#include <falaise/snemo/datamodels/topology_1eNg_pattern.h>
#include <falaise/snemo/datamodels/topology_2eNg_pattern.h>
#include <falaise/snemo/datamodels/topology_1e1a_pattern.h>

#include <snemo/datamodels/pid_utils.h>
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

        if (a_driver_name == snemo::reconstruction::tof_driver::get_id()) {
          // Initialize TOF Driver
          _TOFD_.reset(new snemo::reconstruction::tof_driver);
          datatools::properties TOFD_config;
          setup_.export_and_rename_starting_with(TOFD_config, std::string(a_driver_name + "."), "");
          _TOFD_->initialize(TOFD_config);
        } else if (a_driver_name == snemo::reconstruction::delta_vertices_driver::get_id()) {
          // Initialize Delta Vertices Driver
          _DVD_.reset(new snemo::reconstruction::delta_vertices_driver);
          datatools::properties DVD_config;
          setup_.export_and_rename_starting_with(DVD_config, std::string(a_driver_name + "."), "");
          _DVD_->initialize(DVD_config);
        } else if (a_driver_name == snemo::reconstruction::angle_measurement_driver::get_id()) {
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

      status = _process_algo(ptd_, td_);
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
        DT_LOG_NOTICE(get_logging_priority(), "Fill '1e' topology");
        this->_fill_1e_topology_(ptd_, td_);
      }
      else if (a_classification == "2e") {
        DT_LOG_NOTICE(get_logging_priority(), "Fill '2e' topology");
        this->_fill_2e_topology_(ptd_, td_);
      }
      else if (a_classification == "1e1p") {
        DT_LOG_NOTICE(get_logging_priority(), "Fill '1e1p' topology");
        this->_fill_1e1p_topology_(ptd_, td_);
      }
      else if (a_classification == "1e1g" ||
               a_classification == "1e2g" ||
               a_classification == "1e3g" ||
               a_classification == "1e4g" ||
               a_classification == "1e5g") {
        DT_LOG_NOTICE(get_logging_priority(), "Fill '1eNg' topology");
        this->_fill_1eNg_topology_(ptd_, td_);
      }
      else if (a_classification == "2e1g" ||
               a_classification == "2e2g" ||
               a_classification == "2e3g" ||
               a_classification == "2e4g" ||
               a_classification == "2e5g") {
        DT_LOG_NOTICE(get_logging_priority(), "Fill '2eNg' topology");
        this->_fill_2eNg_topology_(ptd_, td_);
      }
      else if (a_classification == "1e1a") {
        DT_LOG_NOTICE(get_logging_priority(), "Fill '1e1a' topology");
        this->_fill_1e1a_topology_(ptd_, td_);
      }
      else {
        // DT_LOG_WARNING(get_logging_priority(),
        //                "Event classification '" << a_classification << "' unsupported !");
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
      if (datatools::is_valid(angle)) t1ep->set_angle(angle);

      {
        // Extract electron track physical quantities i.e. track length & energy
        snemo::datamodel::particle_track_data::particle_collection_type electron_tracks;
        const size_t nelectrons
          = snemo::datamodel::pid_utils::fetch_particles(ptd_,
                                                         electron_tracks,
                                                         snemo::datamodel::pid_utils::electron_label());
        DT_THROW_IF(nelectrons != 1, std::logic_error, "Number of electrons different from 1 !");

        const snemo::datamodel::particle_track & electron_track = electron_tracks.front().get();

        double electron_energy = datatools::invalid_real();
        if (electron_track.has_associated_calorimeter_hits()) {
          const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
            calos = electron_track.get_associated_calorimeter_hits();
          if (calos.size() == 1) {
            electron_energy = calos.front().get().get_energy();
          }
        }

        if (datatools::is_valid(electron_energy))
          t1ep->set_electron_energy(electron_energy);
      }

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

      // TOF measurements :
      double proba_int = datatools::invalid_real();
      double proba_ext = datatools::invalid_real();
      if (_TOFD_) _TOFD_->process(pt1, pt2, proba_int, proba_ext);
      if (datatools::is_valid(proba_int)) t2ep->set_internal_probability(proba_int);
      if (datatools::is_valid(proba_ext)) t2ep->set_external_probability(proba_ext);

      // Delta vertices measurements :
      double delta_vertices_y = datatools::invalid_real();
      double delta_vertices_z = datatools::invalid_real();
      if (_DVD_) _DVD_->process(pt1, pt2, delta_vertices_y, delta_vertices_z);
      if (datatools::is_valid(delta_vertices_y)) t2ep->set_delta_vertices_y(delta_vertices_y);
      if (datatools::is_valid(delta_vertices_z)) t2ep->set_delta_vertices_z(delta_vertices_z);

      // Angular measurement :
      double angle = datatools::invalid_real();
      if (_AMD_) _AMD_->process(pt1, pt2, angle);
      if (datatools::is_valid(angle)) t2ep->set_angle(angle);

      {
        // Extract electron energies
        snemo::datamodel::particle_track_data::particle_collection_type electron_tracks;
        const size_t nelectrons
          = snemo::datamodel::pid_utils::fetch_particles(ptd_, electron_tracks,
                                                         snemo::datamodel::pid_utils::electron_label());
        DT_THROW_IF(nelectrons != 2, std::logic_error, "Number of electrons different from 2 !");

        // Store geom_id to avoid double inclusion of energy deposited
        std::set<geomtools::geom_id> gids;
        double min_energy;
        double max_energy;
        datatools::plus_infinity(min_energy);
        datatools::minus_infinity(max_energy);
        for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
               iparticle = the_particles.begin(); iparticle != the_particles.end();
             ++iparticle) {
          const snemo::datamodel::particle_track & a_particle = iparticle->get();
          if (! a_particle.has_associated_calorimeter_hits()) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Particle track is not associated to any calorimeter block !");
            continue;
          }
          const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
            the_calorimeters = a_particle.get_associated_calorimeter_hits();
          if (the_calorimeters.size() > 2) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "The particle is associated to more than 2 calorimeters !");
            continue;
          }

          for (snemo::datamodel::calibrated_calorimeter_hit::collection_type::const_iterator
                 icalo = the_calorimeters.begin(); icalo != the_calorimeters.end(); ++icalo) {
            const snemo::datamodel::calibrated_calorimeter_hit & a_calo = icalo->get();
            const geomtools::geom_id & gid = a_calo.get_geom_id();
            if (gids.find(gid) != gids.end()) continue;
            gids.insert(gid);
            min_energy = std::min(a_calo.get_energy(), min_energy);
            max_energy = std::max(a_calo.get_energy(), max_energy);
          }
        }
        if (! datatools::is_plus_infinity(min_energy)) {
          t2ep->set_minimal_energy(min_energy);
        }
        if (! datatools::is_minus_infinity(max_energy)) {
          t2ep->set_maximal_energy(max_energy);
        }
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
      if (datatools::is_valid(proba_int)) t1e1pp->set_internal_probability(proba_int);
      if (datatools::is_valid(proba_ext)) t1e1pp->set_external_probability(proba_ext);

      double delta_vertices_y = datatools::invalid_real();
      double delta_vertices_z = datatools::invalid_real();
      if (_DVD_) _DVD_->process(pt1, pt2, delta_vertices_y, delta_vertices_z);
      if (datatools::is_valid(delta_vertices_y)) t1e1pp->set_delta_vertices_y(delta_vertices_y);
      if (datatools::is_valid(delta_vertices_z)) t1e1pp->set_delta_vertices_z(delta_vertices_z);

      double angle = datatools::invalid_real();
      if (_AMD_) _AMD_->process(pt1, pt2, angle);
      if (datatools::is_valid(angle)) t1e1pp->set_angle(angle);

      return;
    }

    void topology_driver::_fill_1eNg_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                               snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_1eNg_pattern * t1eNgp = new snemo::datamodel::topology_1eNg_pattern;
      h_pattern.reset(t1eNgp);
      td_.set_pattern_handle(h_pattern);

      const datatools::properties & aux = ptd_.get_auxiliaries();
      DT_THROW_IF(! aux.has_key(snemo::datamodel::pid_utils::gamma_label()),
                  std::logic_error, "No gammas have been found in this event !");
      t1eNgp->set_number_of_gammas(aux.fetch_integer(snemo::datamodel::pid_utils::gamma_label()));

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();

      for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
             i_particle = the_particles.begin();
           i_particle != boost::prior(the_particles.end()); ++i_particle) {
        for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
               j_particle = boost::next(i_particle);
             j_particle != the_particles.end(); ++j_particle) {

          if(snemo::datamodel::pid_utils::particle_is_gamma(i_particle->get()) &&
             snemo::datamodel::pid_utils::particle_is_gamma(j_particle->get()))
            continue;

          snemo::datamodel::topology_1eNg_pattern::TOF_collection_type & tofs
            = t1eNgp->grab_TOF_collection();
          {
            snemo::datamodel::TOF_measurement dummy;
            tofs.push_back(dummy);
          }
          snemo::datamodel::TOF_measurement & a_tof = tofs.back();
          a_tof.set_particle_tracks(*i_particle, *j_particle);
          if (_TOFD_) _TOFD_->process(i_particle->get(), j_particle->get(),
                                      a_tof.grab_internal_probabilities(),
                                      a_tof.grab_external_probabilities());

          snemo::datamodel::topology_1eNg_pattern::angle_collection_type & angles
            = t1eNgp->grab_angle_collection();
          {
            snemo::datamodel::angle_measurement dummy;
            angles.push_back(dummy);
          }
          snemo::datamodel::angle_measurement & an_angle = angles.back();
          an_angle.set_particle_tracks(*i_particle, *j_particle);
          if (_AMD_) _AMD_->process(i_particle->get(), j_particle->get(),
                                    an_angle.grab_angle());
        }
      }

      {
        // Extract electron energies
        snemo::datamodel::particle_track_data::particle_collection_type electron_tracks;
        const size_t nelectrons
          = snemo::datamodel::pid_utils::fetch_particles(ptd_, electron_tracks,
                                                         snemo::datamodel::pid_utils::electron_label());
        DT_THROW_IF(nelectrons != 1, std::logic_error, "Number of electrons different from 1 !");

        // Store geom_id to avoid double inclusion of energy deposited
        double electron_energy;
        datatools::minus_infinity(electron_energy);

        const snemo::datamodel::particle_track & the_electron = electron_tracks.front().get();
        if (! the_electron.has_associated_calorimeter_hits())
          DT_LOG_DEBUG(get_logging_priority(),
                       "Particle track is not associated to any calorimeter block !");

        const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
          the_calorimeters = the_electron.get_associated_calorimeter_hits();
        if (the_calorimeters.size() > 2)
          DT_LOG_DEBUG(get_logging_priority(),
                       "The particle is associated to more than 2 calorimeters !");

        // const snemo::datamodel::calibrated_calorimeter_hit & a_calo = the_calorimeters.front();
        electron_energy = the_calorimeters.front().get().get_energy();
        if (the_calorimeters.size() == 1 && ! datatools::is_minus_infinity(electron_energy))
          t1eNgp->set_electron_energy(electron_energy);

        // Extract gammas energies
        snemo::datamodel::particle_track_data::particle_collection_type gamma_tracks;
        const int ngammas = snemo::datamodel::pid_utils::fetch_particles(ptd_, gamma_tracks,
                                                                         snemo::datamodel::pid_utils::gamma_label());
        DT_THROW_IF(ngammas == 0 , std::logic_error, "No gammas in 1eNg topology !");

        std::vector<double> gamma_energies;
        for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
               iparticle = gamma_tracks.begin(); iparticle != gamma_tracks.end();
             ++iparticle) {
          const snemo::datamodel::particle_track & a_gamma = iparticle->get();
          if (! a_gamma.has_associated_calorimeter_hits()) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "A gamma has no associated calorimeter block !");
            continue;
          }
          const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
            the_calorimeters = a_gamma.get_associated_calorimeter_hits();

          double a_gamma_energy = 0;
          for (snemo::datamodel::calibrated_calorimeter_hit::collection_type::const_iterator
                 icalo = the_calorimeters.begin(); icalo != the_calorimeters.end(); ++icalo) {
            const snemo::datamodel::calibrated_calorimeter_hit & a_calo = icalo->get();
            a_gamma_energy += a_calo.get_energy();
          }
          gamma_energies.push_back(a_gamma_energy);
        }

        // For now only up to three gammas is supported
        if(ngammas == 1)
          t1eNgp->set_gamma_max_energy(gamma_energies.back());
        if(ngammas == 2) {
          t1eNgp->set_gamma_max_energy(std::max(gamma_energies[0],gamma_energies[1]));
          t1eNgp->set_gamma_min_energy(std::min(gamma_energies[0],gamma_energies[1]));
        }
        if(ngammas == 3) {
          t1eNgp->set_gamma_max_energy(std::max(std::max(gamma_energies[0],gamma_energies[1]),gamma_energies[2]));
          t1eNgp->set_gamma_mid_energy(std::max(std::min(gamma_energies[0],gamma_energies[1]),std::min(gamma_energies[1],gamma_energies[2])));
          t1eNgp->set_gamma_min_energy(std::min(std::min(gamma_energies[0],gamma_energies[1]),gamma_energies[2]));
        }
      }

      // std::cout << "Number of gammas : " << t1eNgp->get_number_of_gammas() << std::endl;
      // if(t1eNgp->get_number_of_gammas() == 1) {
      //   std::cout << "Emax : " << t1eNgp->get_gamma_max_energy() << std::endl;
      // }
      // if(t1eNgp->get_number_of_gammas() == 2) {
      //   std::cout << "Emax : " << t1eNgp->get_gamma_max_energy() << std::endl;
      //   std::cout << "Emin : " << t1eNgp->get_gamma_min_energy() << std::endl;
      // }
      // if(t1eNgp->get_number_of_gammas() == 3) {
      //   std::cout << "Emax : " << t1eNgp->get_gamma_max_energy() << std::endl;
      //   std::cout << "Emid : " << t1eNgp->get_gamma_mid_energy() << std::endl;
      //   std::cout << "Emin : " << t1eNgp->get_gamma_min_energy() << std::endl;
      // }

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
          snemo::datamodel::TOF_measurement & a_tof = tofs.back();
          a_tof.set_particle_tracks(*i_particle, *j_particle);
          if (_TOFD_) _TOFD_->process(i_particle->get(), j_particle->get(),
                                      a_tof.grab_internal_probabilities(),
                                      a_tof.grab_external_probabilities());


          // a_delta_vertices.set_particle_tracks(*i_particle, *j_particle);
          double delta_vertices_y = datatools::invalid_real();
          double delta_vertices_z = datatools::invalid_real();
          if (_DVD_) _DVD_->process(i_particle->get(), j_particle->get(),
                                    delta_vertices_y, delta_vertices_z);
          if(datatools::is_valid(delta_vertices_y) && datatools::is_valid(delta_vertices_z)){
            t2eNgp->set_delta_vertices_y(delta_vertices_y);
            t2eNgp->set_delta_vertices_z(delta_vertices_z);
          }

          snemo::datamodel::topology_2eNg_pattern::angle_collection_type & angles
            = t2eNgp->grab_angle_collection();
          {
            snemo::datamodel::angle_measurement dummy;
            angles.push_back(dummy);
          }
          snemo::datamodel::angle_measurement & an_angle = angles.back();
          an_angle.set_particle_tracks(*i_particle, *j_particle);
          if (_AMD_) _AMD_->process(i_particle->get(), j_particle->get(),
                                    an_angle.grab_angle());
        }
      }

      return;
    }

    void topology_driver::_fill_1e1a_topology_(const snemo::datamodel::particle_track_data & ptd_,
                                               snemo::datamodel::topology_data & td_)
    {
      snemo::datamodel::topology_data::handle_pattern h_pattern;
      snemo::datamodel::topology_1e1a_pattern * t1e1ap = new snemo::datamodel::topology_1e1a_pattern;
      h_pattern.reset(t1e1ap);
      td_.set_pattern_handle(h_pattern);

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();
      const snemo::datamodel::particle_track & pt1 = the_particles.front().get();
      const snemo::datamodel::particle_track & pt2 = the_particles.back().get();

      double delta_vertices_y = datatools::invalid_real();
      double delta_vertices_z = datatools::invalid_real();
      if (_DVD_) _DVD_->process(pt1, pt2, delta_vertices_y, delta_vertices_z);
      if (datatools::is_valid(delta_vertices_y)) t1e1ap->set_delta_vertices_y(delta_vertices_y);
      if (datatools::is_valid(delta_vertices_z)) t1e1ap->set_delta_vertices_z(delta_vertices_z);

      double angle = datatools::invalid_real();
      if (_AMD_) _AMD_->process(pt1, pt2, angle);
      if (datatools::is_valid(angle)) t1e1ap->set_angle(angle);

      {
        // Extract alpha track physical quantities i.e. time delay and length
        snemo::datamodel::particle_track_data::particle_collection_type alpha_tracks;
        const size_t nalphas
          = snemo::datamodel::pid_utils::fetch_particles(ptd_,
                                                         alpha_tracks,
                                                         snemo::datamodel::pid_utils::alpha_label());
        DT_THROW_IF(nalphas != 1, std::logic_error, "More than 1 alpha particle !");
        const snemo::datamodel::particle_track & alpha_track = alpha_tracks.front().get();

        double alpha_delayed_time = datatools::invalid_real();
        double alpha_track_length = datatools::invalid_real();
        if (alpha_track.has_trajectory()) {
          const snemo::datamodel::tracker_trajectory & a_trajectory = alpha_track.get_trajectory();
          const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
          alpha_track_length = a_track_pattern.get_shape().get_length();
          // Get alpha time delay from trajectory auxiliairies
          const datatools::properties & aux = a_trajectory.get_auxiliaries();
          if (aux.has_key("t0")) {
            alpha_delayed_time = aux.fetch_real("t0");
          }
        }
        if (datatools::is_valid(alpha_delayed_time)) t1e1ap->set_alpha_delayed_time(alpha_delayed_time);
        if (datatools::is_valid(alpha_track_length)) t1e1ap->set_alpha_track_length(alpha_track_length);
      }

      {
        // Extract electron track physical quantities i.e. track length & energy
        snemo::datamodel::particle_track_data::particle_collection_type electron_tracks;
        const size_t nelectrons
          = snemo::datamodel::pid_utils::fetch_particles(ptd_,
                                                         electron_tracks,
                                                         snemo::datamodel::pid_utils::electron_label());
        DT_THROW_IF(nelectrons != 1, std::logic_error, "More than 1 electron particle !");
        const snemo::datamodel::particle_track & electron_track = electron_tracks.front().get();

        double electron_track_length = datatools::invalid_real();
        if (electron_track.has_trajectory()) {
          const snemo::datamodel::tracker_trajectory & a_trajectory = electron_track.get_trajectory();
          const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
          electron_track_length = a_track_pattern.get_shape().get_length();
        }
        if (datatools::is_valid(electron_track_length)) t1e1ap->set_electron_track_length(electron_track_length);

        double electron_energy = datatools::invalid_real();
        if (electron_track.has_associated_calorimeter_hits()) {
          const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
            calos = electron_track.get_associated_calorimeter_hits();
          if (calos.size() == 1) {
            electron_energy = calos.front().get().get_energy();
          }
        }
        if (datatools::is_valid(electron_energy)) t1e1ap->set_electron_energy(electron_energy);
      }

      return;
    }

    // static
    void topology_driver::init_ocd(datatools::object_configuration_description & ocd_)
    {
      // Prefix "TD" stands for "Topology Driver" :
      datatools::logger::declare_ocd_logging_configuration(ocd_, "fatal", "TD.");

      // Invoke specific OCD support from the driver class:
      ::snemo::reconstruction::tof_driver::init_ocd(ocd_);
      ::snemo::reconstruction::delta_vertices_driver::init_ocd(ocd_);
      ::snemo::reconstruction::angle_measurement_driver::init_ocd(ocd_);

      return;
    }

  }  // end of namespace reconstruction

}  // end of namespace snemo
/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::topology_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::topology_driver");
  ocd_.set_class_description("A driver class for the topology algorithms");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("The driver manages the different topology patterns\n"
                               "and addresses the related measurements.");

  // Invoke specific OCD support :
  ::snemo::reconstruction::topology_driver::init_ocd(ocd_);

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::topology_driver,
                               "snemo::reconstruction::topology_driver")
