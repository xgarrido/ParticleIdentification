// falaise/snemo/cuts/pid_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/pid_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(pid_cut, "snemo::cut::pid_cut")

    pid_cut::particle_range::particle_range()
    {
      min = 0;
      max = 0;
      return;
    }

    void pid_cut::particle_range::parse(const datatools::properties & setup_,
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

    bool pid_cut::particle_range::check(const size_t n_)
    {
      if (n_ < min) {
        return false;
      }
      if (n_ > max) {
        return false;
      }
      return true;
    }

    void pid_cut::_set_defaults()
    {
      _PTD_label_ = snemo::datamodel::data_info::default_particle_track_data_label();
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
      _set_defaults();
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

      _electron_range_.parse(configuration_, "electron");
      _positron_range_.parse(configuration_, "positron");
      _gamma_range_.parse(configuration_, "gamma");
      _alpha_range_.parse(configuration_, "alpha");
      _undefined_range_.parse(configuration_, "undefined");

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

      size_t nelectrons = 0;
      size_t npositrons = 0;
      size_t nalphas    = 0;
      size_t ngammas    = 0;
      size_t nundefined = 0;

      std::string key;
      if (aux.has_key(key = snemo::datamodel::pid_utils::electron_label())) {
        nelectrons = aux.fetch_integer(key);
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::positron_label())) {
        npositrons = aux.fetch_integer(key);
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::gamma_label())) {
        ngammas = aux.fetch_integer(key);
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::alpha_label())) {
        nalphas = aux.fetch_integer(key);
      }
      if (aux.has_key(key = snemo::datamodel::pid_utils::undefined_label())) {
        nundefined = aux.fetch_integer(key);
      }

      DT_LOG_TRACE(get_logging_priority(), "nelectron  = " << nelectrons);
      DT_LOG_TRACE(get_logging_priority(), "npositron  = " << npositrons);
      DT_LOG_TRACE(get_logging_priority(), "nalphas    = " << nalphas);
      DT_LOG_TRACE(get_logging_priority(), "ngammas    = " << ngammas);
      DT_LOG_TRACE(get_logging_priority(), "nundefined = " << nundefined);

      bool check = true;
      if (! _electron_range_.check(nelectrons)) check = false;
      if (! _positron_range_.check(npositrons)) check = false;
      if (! _gamma_range_.check(ngammas)) check = false;
      if (! _alpha_range_.check(nalphas)) check = false;
      if (! _undefined_range_.check(nundefined)) check = false;

      cut_returned = cuts::SELECTION_ACCEPTED;
      if (! check) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by pid cut!");
        cut_returned = cuts::SELECTION_REJECTED;
      }

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
