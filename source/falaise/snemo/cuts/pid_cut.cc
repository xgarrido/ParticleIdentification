// falaise/snemo/cuts/pid_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/pid_cut.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(pid_cut, "snemo::cut::pid_cut")

    void pid_cut::_set_defaults()
    {
      _PTD_label_ = snemo::datamodel::data_info::default_particle_track_data_label();
      _electron_range_.invalidate();
      _positron_range_.invalidate();
      _gamma_range_.invalidate();
      _alpha_range_.invalidate();
      _undefined_range_.invalidate();
      return;
    }

    pid_cut::pid_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    pid_cut::~pid_cut()
    {
      if (is_initialized()) this->pid_cut::reset();
      return;
    }

    void pid_cut::reset()
    {
      this->_set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void pid_cut::initialize(const datatools::properties & configuration_,
                             datatools::service_manager  & /* service_manager_ */,
                             cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("PTD_label")) {
        _PTD_label_ = configuration_.fetch_string("PTD_label");
      }

      auto parse_range = [&configuration_](const std::string & prefix,
                                           datatools::integer_range & range) {
        range.make_full_positive();
        if (configuration_.has_key(prefix + "_range.min")) {
          range.set_lower(configuration_.fetch_integer(prefix + "_range.min"));
        }
        if (configuration_.has_key(prefix + "_range.max")) {
          range.set_upper(configuration_.fetch_integer(prefix + "_range.max"));
        }
      };

      parse_range("electron", _electron_range_);
      parse_range("positron", _positron_range_);
      parse_range("gamma", _gamma_range_);
      parse_range("alpha", _alpha_range_);
      parse_range("undefined", _undefined_range_);

      this->i_cut::_set_initialized(true);
      return;
    }


    int pid_cut::_accept()
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

      const datatools::properties & aux = PTD.get_auxiliaries();

      std::string key;
      if (aux.has_key(key = snemo::datamodel::pid_utils::electron_label())) {
        if (_electron_range_.has(aux.fetch_integer(key))) {
          DT_LOG_DEBUG(get_logging_priority(), "Bad number of electrons !");
          return cuts::SELECTION_REJECTED;
        }
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::positron_label())) {
        if (_positron_range_.has(aux.fetch_integer(key))) {
          DT_LOG_DEBUG(get_logging_priority(), "Bad number of positrons !");
          return cuts::SELECTION_REJECTED;
        }
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::gamma_label())) {
        if (_gamma_range_.has(aux.fetch_integer(key))) {
          DT_LOG_DEBUG(get_logging_priority(), "Bad number of gammas !");
          return cuts::SELECTION_REJECTED;
        }
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::alpha_label())) {
        if (_alpha_range_.has(aux.fetch_integer(key))) {
          DT_LOG_DEBUG(get_logging_priority(), "Bad number of alphas !");
          return cuts::SELECTION_REJECTED;
        }
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::undefined_label())) {
        if (_undefined_range_.has(aux.fetch_integer(key))) {
          DT_LOG_DEBUG(get_logging_priority(), "Bad number of undefined particles !");
          return cuts::SELECTION_REJECTED;
        }
      }

      return cuts::SELECTION_ACCEPTED;
    }

  }  // end of namespace cut

}  // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
