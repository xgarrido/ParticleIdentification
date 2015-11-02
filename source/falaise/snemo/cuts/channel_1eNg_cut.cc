// falaise/snemo/cuts/channel_1eNg_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/channel_1eNg_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_1eNg_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(channel_1eNg_cut, "snemo::cut::channel_1eNg_cut");

    void channel_1eNg_cut::_set_defaults()
    {
      _number_of_gammas_min_ = -1;
      _number_of_gammas_max_ = -1;
      base_channel_cut::_set_defaults();
      return;
    }

    bool channel_1eNg_cut::is_mode_has_number_of_gammas() const
    {
      return _mode_ & datatools::bit_mask::bit09;
    }

    bool channel_1eNg_cut::is_mode_range_number_of_gammas() const
    {
      return _mode_ & datatools::bit_mask::bit10;
    }

    channel_1eNg_cut::channel_1eNg_cut(datatools::logger::priority logger_priority_)
      : base_channel_cut::base_channel_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    channel_1eNg_cut::~channel_1eNg_cut()
    {
      if (is_initialized()) this->channel_1eNg_cut::reset();
      return;
    }

    void channel_1eNg_cut::reset()
    {
      _set_defaults();
      this->base_channel_cut::_reset();
      this->base_channel_cut::_set_initialized(false);
      return;
    }

    void channel_1eNg_cut::initialize(const datatools::properties & configuration_,
                                      datatools::service_manager  & service_manager_,
                                      cuts::cut_handle_dict_type  & cut_dict_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      if (configuration_.has_flag("mode.has_number_of_gammas")) {
        _mode_ |= datatools::bit_mask::bit09;
      }
      if (configuration_.has_flag("mode.range_number_of_gammas")) {
        _mode_ |= datatools::bit_mask::bit10;
      }

      if (is_mode_range_number_of_gammas()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_NUMBER_OF_GAMMAS mode...");
        size_t count = 0;
        if (configuration_.has_key("range_number_of_gammas.min")) {
          double ngammas_min = configuration_.fetch_real("range_number_of_gammas.min");
          DT_THROW_IF(ngammas_min < 0, std::range_error,
                      "Invalid minimal number of gammas (" << ngammas_min << ") !");
          _number_of_gammas_min_ = ngammas_min;
          count++;
        }
        if (configuration_.has_key("range_number_of_gammas.max")) {
          double ngammas_max = configuration_.fetch_real("range_number_of_gammas.max");
          DT_THROW_IF(ngammas_max < 0, std::range_error,
                      "Invalid maximal number of gammas (" << ngammas_max << ") !");
          _number_of_gammas_max_ = ngammas_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_number_of_gammas.min' or 'range_number_of_gammas.max' property !");
        if (count == 2 && _number_of_gammas_min_ >= 0 && _number_of_gammas_max_ >= 0) {
          DT_THROW_IF(_number_of_gammas_min_ > _number_of_gammas_max_, std::logic_error,
                      "Invalid 'range_number_of_gammas.min' > 'range_number_of_gammas.max' values !");
        }
      }

      base_channel_cut::initialize(configuration_, service_manager_, cut_dict_);
      // this->base_channel_cut::initialize(configuration_);
      return;
    }

    int channel_1eNg_cut::_accept()
    {
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cut_returned;
      }

      const snemo::datamodel::topology_data & TD = ER.get<snemo::datamodel::topology_data>(_TD_label_);
      if (! TD.has_pattern()) {
        DT_LOG_DEBUG(get_logging_priority(), "Missing topology pattern !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::base_topology_pattern & a_pattern = TD.get_pattern();
      const std::string & a_pattern_id = a_pattern.pattern_id();

      std::string pattern_id_1eNg;
      {
        snemo::datamodel::topology_1eNg_pattern * a_1eNg_pattern = new snemo::datamodel::topology_1eNg_pattern;
        pattern_id_1eNg = a_1eNg_pattern->pattern_id();
      }

      if (a_pattern_id != pattern_id_1eNg) {
        DT_LOG_DEBUG(get_logging_priority(), "This cut is only applicable to '"
                     << pattern_id_1eNg << "' topology !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_1eNg_pattern & a_1eNg_pattern
        = dynamic_cast<const snemo::datamodel::topology_1eNg_pattern &>(a_pattern);

      // Check if event has gammas
      bool check_has_gamma = true;
      if (is_mode_has_number_of_gammas()) {
        if (! a_1eNg_pattern.has_number_of_gammas())
          check_has_gamma = false;
      }

      // Check if event has correct number of gammas
      bool check_range_gamma = true;
      if (is_mode_range_number_of_gammas()) {
        const int ngamma = a_1eNg_pattern.get_number_of_gammas();
        if (ngamma < _number_of_gammas_min_) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Number of gammas (" << ngamma << ") lower than " << _number_of_gammas_min_);
          check_range_gamma = false;
        }
        if (ngamma > _number_of_gammas_max_) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Number of gammas (" << ngamma << ") greater than " << _number_of_gammas_max_);
          check_range_gamma = false;
        }
      }

      // Check if event has internal probability
      bool check_has_internal_probability = true;
      if (is_mode_has_internal_probability()) {
        if (! a_1eNg_pattern.has_electron_gammas_internal_probabilities()) {
          check_has_internal_probability = false;
        }
      }

      // Check if event has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (! a_1eNg_pattern.has_electron_gammas_internal_probabilities()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        snemo::datamodel::topology_1eNg_pattern::tof_collection_type internal_probabilities = a_1eNg_pattern.get_electron_gammas_internal_probabilities();

        for(unsigned int i=0; i<internal_probabilities.size(); i++) {
          //If gammas are indeed intern with the electrons, only the proba
          //with the first calorimeter in the list is checked
          double pint = internal_probabilities.at(i).front();
          if (datatools::is_valid(_prob_int_min_)) {
            if (pint < _prob_int_min_) {
              DT_LOG_DEBUG(get_logging_priority(),
                           "Internal probability (" << pint << ") lower than " << _prob_int_min_);
              check_range_internal_probability = false;
            }
          }

          if (datatools::is_valid(_prob_int_max_)) {
            if (pint > _prob_int_max_) {
              DT_LOG_DEBUG(get_logging_priority(),
                           "Internal probability (" << pint << ") greater than " << _prob_int_max_);
              check_range_internal_probability = false;
            }
          }
        }
      }

      // Check if event has external probability
      bool check_has_external_probability = true;
      if (is_mode_has_external_probability()) {
        if (! a_1eNg_pattern.has_electron_gammas_external_probabilities()) {
          check_has_external_probability = false;
        }
      }

      // Check if event has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (! a_1eNg_pattern.has_electron_gammas_external_probabilities()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        snemo::datamodel::topology_1eNg_pattern::tof_collection_type external_probabilities = a_1eNg_pattern.get_electron_gammas_external_probabilities();
        for(unsigned int i=0; i<external_probabilities.size(); ++i) {
          for(unsigned int j=0; j<external_probabilities.at(i).size(); ++j) {
            // Requires that no calorimeter of any gamma has a good external proba
            double pext = external_probabilities.at(i).at(j);
            if (datatools::is_valid(_prob_ext_min_)) {
              if (pext < _prob_ext_min_) {
                DT_LOG_DEBUG(get_logging_priority(),
                             "External probability (" << pext << ") lower than " << _prob_ext_min_);
                check_range_external_probability = false;
              }
            }
            if (datatools::is_valid(_prob_ext_max_)) {
              if (pext > _prob_ext_max_) {
                DT_LOG_DEBUG(get_logging_priority(),
                             "External probability (" << pext << ") greater than " << _prob_ext_max_);
                check_range_external_probability = false;
              }
            }
          }
        }
      }

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_gamma &&
          check_range_gamma &&
          check_has_internal_probability &&
          check_range_internal_probability &&
          check_has_external_probability &&
          check_range_external_probability) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by channel 1eNg cut!");
        cut_returned = cuts::SELECTION_ACCEPTED;
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
