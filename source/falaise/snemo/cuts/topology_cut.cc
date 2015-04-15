// falaise/snemo/cuts/topology_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/topology_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_2e_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(topology_cut, "snemo::cut::topology_cut");

    topology_cut::particle_range::particle_range()
    {
      min = 0;
      max = 0;
      return;
    }

    void topology_cut::particle_range::parse(const datatools::properties & setup_,
                                             const std::string & prefix_)
    {
      if (setup_.has_key(prefix_ + "_range.min")) {
        min = setup_.fetch_integer(prefix_ + "_range.min");
      }
      if (setup_.has_key(prefix_ + "_range.max")) {
        max = setup_.fetch_integer(prefix_ + "_range.max");
      }
      return;
    }

    bool topology_cut::particle_range::check(const size_t n_)
    {
      if (n_ < min) {
        return false;
      }
      if (n_ > max) {
        return false;
      }
      return true;
    }

    void topology_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _PTD_label_ = snemo::datamodel::data_info::default_particle_track_data_label();
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      return;
    }

    uint32_t topology_cut::get_mode() const
    {
      return _mode_;
    }

    topology_cut::topology_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    topology_cut::~topology_cut()
    {
      if (is_initialized()) this->topology_cut::reset();
      return;
    }

    void topology_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void topology_cut::initialize(const datatools::properties & configuration_,
                                        datatools::service_manager  & /* service_manager_ */,
                                        cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      std::cout << "DEBUG initialize" << std::endl;

      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("PTD_label")) {
        _PTD_label_ = configuration_.fetch_string("PTD_label");
      }

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }

      _electron_range_.parse(configuration_, "electron");
      _positron_range_.parse(configuration_, "positron");
      _gamma_range_.parse(configuration_, "gamma");
      _alpha_range_.parse(configuration_, "alpha");
      _undefined_range_.parse(configuration_, "undefined");

      _unassociated_calorimeter_hits_range_.parse(configuration_, "unassociated_calos");

      if (configuration_.has_key ("internal_probability")) {
        const double Pint = configuration_.fetch_real ("internal_probability");
        DT_THROW_IF (Pint < 0 || Pint >1, std::logic_error, "Invalid internal probability (" << Pint << ") !");
        _prob_int_ = Pint;
        std::cout << _prob_int_ << std::endl;
      }

      if (configuration_.has_key ("external_probability")) {
        const double Pext = configuration_.fetch_real ("external_probability");
        DT_THROW_IF (Pext < 0 || Pext >1, std::logic_error, "Invalid external probability (" << Pext << ") !");
        _prob_ext_ = Pext;
      }

      if (configuration_.has_key ("delta_vertices_y")) {
        const double delta_y = configuration_.fetch_real ("delta_vertices_y");
        // DT_THROW_IF (delta_y < 0, std::logic_error, "Invalid internal probability (" << Pint << ") !");
        _delta_vertices_y_ = std::fabs(delta_y);
      }

      if (configuration_.has_key ("delta_vertices_z")) {
        const double delta_z = configuration_.fetch_real ("delta_vertices_z");
        // DT_THROW_IF (delta_z < 0, std::logic_error, "Invalid internal probability (" << Pint << ") !");
        _delta_vertices_z_ = std::fabs(delta_z);
      }

      std::cout << "DEBUG initialized" << std::endl;

      this->i_cut::_set_initialized(true);
      return;
    }


    int topology_cut::_accept()
    {
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_PTD_label_)) {
        DT_LOG_DEBUG(get_logging_priority(), "Event record has no '" << _PTD_label_ << "' bank !");
        return cut_returned;
      }

      const snemo::datamodel::particle_track_data & PTD
        = ER.get<snemo::datamodel::particle_track_data>(_PTD_label_);
      if (! PTD.has_particles()) {
        DT_LOG_DEBUG(get_logging_priority(), "Particle track data has no particle");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::particle_track_data::particle_collection_type & particles
        = PTD.get_particles();

      size_t n_unasso_calos = PTD.get_non_associated_calorimeters().size();

      // std::cout << " unasso calos " << n_unasso_calos << std::endl;

      size_t nelectrons = 0;
      size_t npositrons = 0;
      size_t nalphas    = 0;
      size_t ngammas    = 0;
      size_t nundefined = 0;

      // for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
      //        it = particles.begin(); it != particles.end(); ++it) {
      //   const snemo::datamodel::particle_track & a_particle = it->get();

      //   if (snemo::datamodel::particle_track::particle_is_electron(a_particle)) {
      //     nelectrons++;
      //   } else if (snemo::datamodel::particle_track::particle_is_positron(a_particle)) {
      //     npositrons++;
      //   } else if (snemo::datamodel::particle_track::particle_is_gamma(a_particle)) {
      //     ngammas++;
      //   } else if (snemo::datamodel::particle_track::particle_is_alpha(a_particle)) {
      //     nalphas++;
      //   } else {
      //     nundefined++;
      //   }
      // }

      DT_LOG_TRACE(get_logging_priority(), "nelectron  = " << nelectrons);
      DT_LOG_TRACE(get_logging_priority(), "npositron  = " << npositrons);
      DT_LOG_TRACE(get_logging_priority(), "nalphas    = " << nalphas);
      DT_LOG_TRACE(get_logging_priority(), "ngammas    = " << ngammas);
      DT_LOG_TRACE(get_logging_priority(), "nundefined = " << nundefined);

      DT_LOG_TRACE(get_logging_priority(), "n_unasso_calos = " << n_unasso_calos);

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cut_returned;
      }

    //   std::cout << "-------------- DEBUG " << std::endl;

      const snemo::datamodel::topology_data & TD
        = ER.get<snemo::datamodel::topology_data>(_TD_label_);
      const snemo::datamodel::base_topology_pattern & a_pattern = TD.get_pattern();

      DT_THROW_IF(!a_pattern.has_pattern_id(), std::logic_error,
                  "The topology pattern ID is missing ! ");
      const std::string & a_pattern_id = a_pattern.get_pattern_id();

      double internal_prob = datatools::invalid_real();
      double external_prob = datatools::invalid_real();
      double delta_vertices_y = datatools::invalid_real();
      double delta_vertices_z = datatools::invalid_real();

      if(a_pattern_id == snemo::datamodel::topology_2e_pattern::pattern_id()) {
      const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
        = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
      // std::cout << "--------------pattern id " << a_pattern.get_pattern_id() << std::endl;

      internal_prob    = ptr_2e_pattern->get_internal_probability();
      external_prob    = ptr_2e_pattern->get_external_probability();
      delta_vertices_y = std::fabs(ptr_2e_pattern->get_delta_vertices_y());
      delta_vertices_z = std::fabs(ptr_2e_pattern->get_delta_vertices_z());
    }

      bool check = true;
      if (! _electron_range_.check(nelectrons)) check = false;
      if (! _positron_range_.check(npositrons)) check = false;
      if (! _gamma_range_.check(ngammas)) check = false;
      if (! _alpha_range_.check(nalphas)) check = false;
      if (! _undefined_range_.check(nundefined)) check = false;
      if (! _unassociated_calorimeter_hits_range_.check(n_unasso_calos))
        {
          check = false;
          // std::cout << "not in range " << std::endl;
        }

      std::cout << _prob_int_ << " " << _prob_ext_ << " " << _delta_vertices_y_ << " " << _delta_vertices_z_ << std::endl;
      std::cout << internal_prob << " " << external_prob << " " << delta_vertices_y << " " << delta_vertices_z << std::endl;
      if (internal_prob < _prob_int_)
        check = false;
      if (external_prob > _prob_ext_)
        check = false;
      if (delta_vertices_y > _delta_vertices_y_)
        check = false;
      if (delta_vertices_z > _delta_vertices_z_)
        check = false;

      cut_returned = cuts::SELECTION_ACCEPTED;
      if (! check) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by topology cut!");
        cut_returned = cuts::SELECTION_REJECTED;
      }
      if(check)
        std::cout << "pass cut " << std::endl;

      return cut_returned;
    }

  }  // end of namespace cut

}  // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
