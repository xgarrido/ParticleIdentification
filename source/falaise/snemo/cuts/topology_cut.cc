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

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(topology_cut, "snemo::cut::topology_cut");

    topology_cut::particle_range::particle_range()
    {
      min = -1;
      max = -1;
      return;
    }

    bool topology_cut::particle_range::check(const snemo::datamodel::particle_track & particle_)
    {
      return true;
    }

    void topology_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _PTD_label_ = snemo::datamodel::data_info::default_particle_track_data_label();
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
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("PTD_label")) {
        _PTD_label_ = configuration_.fetch_string("PTD_label");
      }

      if (configuration_.has_key("electron_range.min")) {
        _electron_range_.min = configuration_.fetch_integer("electron_range.min");
      }
      if (configuration_.has_key("electron_range_max")) {
        _electron_range_.max = configuration_.fetch_integer("electron_range.max");
      }
      if (configuration_.has_key("positron_range.min")) {
        _positron_range_.min = configuration_.fetch_integer("positron_range.min");
      }
      if (configuration_.has_key("positron_range.max")) {
        _positron_range_.max = configuration_.fetch_integer("positron_range.max");
      }
      if (configuration_.has_key("gamma_range.min")) {
        _gamma_range_.min = configuration_.fetch_integer("gamma_range.min");
      }
      if (configuration_.has_key("gamma_range.max")) {
        _gamma_range_.max = configuration_.fetch_integer("gamma_range.max");
      }
      if (configuration_.has_key("alpha_range.min")) {
        _alpha_range_.min = configuration_.fetch_integer("alpha_range.min");
      }
      if (configuration_.has_key("alpha_range.max")) {
        _alpha_range_.max = configuration_.fetch_integer("alpha_range.max");
      }

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

      size_t nelectrons = 0;
      size_t npositrons = 0;
      size_t nalphas    = 0;
      size_t ngammas    = 0;
      size_t nundefined = 0;
      bool check_topolgy = true;
      for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
             it = particles.begin(); it != particles.end(); ++it) {
        const snemo::datamodel::particle_track & a_particle = it->get();
        // const datatools::properties & aux = a_particle.get_auxiliaries();
        // if (! aux.has_key(snemo::datamodel::pid_utils::pid_label_key())) {
        //   continue;
        // }

        if (snemo::datamodel::particle_track::particle_is_electron(a_particle)) {
          nelectrons++;
        } else if (snemo::datamodel::particle_track::particle_is_positron(a_particle)) {
          npositrons++;
        } else if (snemo::datamodel::particle_track::particle_is_gamma(a_particle)) {
          ngammas++;
        } else if (snemo::datamodel::particle_track::particle_is_alpha(a_particle)) {
          nalphas++;
        } else {
          nundefined++;
        }

        bool check = true;
        if (! _electron_range_.check(a_particle)) check = false;

        if (! check) {
          DT_LOG_DEBUG(get_logging_priority(), "Event rejected by topology cut!");
          check_topolgy = false;
        }
      }

      cut_returned = cuts::SELECTION_REJECTED;
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
