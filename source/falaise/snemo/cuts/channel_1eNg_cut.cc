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
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_1eNg_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(channel_1eNg_cut, "snemo::cut::channel_1eNg_cut");

    void channel_1eNg_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_channel_1eNg_data_label();
      // datatools::invalidate(_number_of_gammas_min_); does not work for integers
      // datatools::invalidate(_number_of_gammas_max_);
      datatools::invalidate(_prob_int_min_);
      datatools::invalidate(_prob_int_max_);
      datatools::invalidate(_prob_ext_min_);
      datatools::invalidate(_prob_ext_max_);
      return;
    }

    uint32_t channel_1eNg_cut::get_mode() const
    {
      return _mode_;
    }

    bool channel_1eNg_cut::is_mode_has_gamma() const
    {
      return _mode_ & MODE_HAS_GAMMA;
    }

    bool channel_1eNg_cut::is_mode_range_gamma() const
    {
      return _mode_ & MODE_RANGE_GAMMA;
    }

    bool channel_1eNg_cut::is_mode_has_internal_probability() const
    {
      return _mode_ & MODE_HAS_INTERNAL_PROBABILITY;
    }

    bool channel_1eNg_cut::is_mode_has_external_probability() const
    {
      return _mode_ & MODE_HAS_EXTERNAL_PROBABILITY;
    }

    bool channel_1eNg_cut::is_mode_range_internal_probability() const
    {
      return _mode_ & MODE_RANGE_INTERNAL_PROBABILITY;
    }

    bool channel_1eNg_cut::is_mode_range_external_probability() const
    {
      return _mode_ & MODE_RANGE_EXTERNAL_PROBABILITY;
    }

    channel_1eNg_cut::channel_1eNg_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
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
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void channel_1eNg_cut::initialize(const datatools::properties & configuration_,
                                    datatools::service_manager  & /* service_manager_ */,
                                    cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }
      if (configuration_.has_flag("mode.has_gamma")) {
        _mode_ |= MODE_HAS_GAMMA;
      }
      if (configuration_.has_flag("mode.range_gamma")) {
        _mode_ |= MODE_RANGE_GAMMA;
      }
      if (configuration_.has_flag("mode.has_internal_probability")) {
        _mode_ |= MODE_HAS_INTERNAL_PROBABILITY;
      }
      if (configuration_.has_flag("mode.has_external_probability")) {
        _mode_ |= MODE_HAS_EXTERNAL_PROBABILITY;
      }
      if (configuration_.has_flag("mode.range_internal_probability")) {
        _mode_ |= MODE_RANGE_INTERNAL_PROBABILITY;
      }
      if (configuration_.has_flag("mode.range_external_probability")) {
        _mode_ |= MODE_RANGE_EXTERNAL_PROBABILITY;
      }
      DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                  "Missing at least a 'mode.XXX' property !");

      if (is_mode_range_gamma()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_GAMMA mode...");
        size_t count = 0;
        if (configuration_.has_key("range_gamma.min")) {
          int ngamma_min = configuration_.fetch_integer("range_gamma.min");
          DT_THROW_IF(ngamma_min < 0,
                      std::range_error,
                      "Invalid minimal number of gammas (" << ngamma_min << ") !");
          _number_of_gammas_min_ = ngamma_min;
          count++;
        }
        if (configuration_.has_key("range_gamma.max")) {
          int ngamma_max = configuration_.fetch_integer("range_gamma.max");
          DT_THROW_IF(ngamma_max < 0,
                      std::range_error,
                      "Invalid maximal number of gammas (" << ngamma_max << ") !");
          _number_of_gammas_max_ = ngamma_max;
          count++;
        }

        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_gamma.min' or 'range_gamma.max' property !");
        if (count == 2 && _number_of_gammas_min_ >= 0 && _number_of_gammas_max_ >= 0) {
          DT_THROW_IF(_number_of_gammas_min_ > _number_of_gammas_max_, std::logic_error,
                      "Invalid 'range_gamma.min' > 'range_gamma.max' values !");
        }
      }

      if (is_mode_range_internal_probability()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_INTERNAL_PROBABILITY mode...");
        size_t count = 0;
        if (configuration_.has_key("range_internal_probability.min")) {
          double pmin = configuration_.fetch_real("range_internal_probability.min");
          if (! configuration_.has_explicit_unit("range_internal_probability.min")) {
            pmin *= CLHEP::perCent;
          }
          DT_THROW_IF(pmin < 0.0*CLHEP::perCent || pmin > 100.0*CLHEP::perCent,
                      std::range_error,
                      "Invalid minimal internal probability (" << pmin << ") !");
          _prob_int_min_ = pmin;
          count++;
        }
        if (configuration_.has_key("range_internal_probability.max")) {
          double pmax = configuration_.fetch_real("range_internal_probability.max");
          if (! configuration_.has_explicit_unit("range_internal_probability.max")) {
            pmax *= CLHEP::perCent;
          }
          DT_THROW_IF(pmax < 0.0*CLHEP::perCent || pmax > 100.0*CLHEP::perCent,
                      std::range_error,
                      "Invalid maximal internal probability (" << pmax << ") !");
          _prob_int_max_ = pmax;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_internal_probability.min' or 'range_internal_probability.max' property !");
        if (count == 2 && _prob_int_min_ >= 0 && _prob_int_max_ >= 0) {
          DT_THROW_IF(_prob_int_min_ > _prob_int_max_, std::logic_error,
                      "Invalid 'range_internal_probability.min' > 'range_internal_probability.max' values !");
        }
      }

      if (is_mode_range_external_probability()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_EXTERNAL_PROBABILITY mode...");
        size_t count = 0;
        if (configuration_.has_key("range_external_probability.min")) {
          double pmin = configuration_.fetch_real("range_external_probability.min");
          if (! configuration_.has_explicit_unit("range_external_probability.min")) {
            pmin *= CLHEP::perCent;
          }
          DT_THROW_IF(pmin < 0.0*CLHEP::perCent || pmin > 100.0*CLHEP::perCent,
                      std::range_error,
                      "Invalid minimal external probability (" << pmin << ") !");
          _prob_ext_min_ = pmin;
          // std::cout << "--------  ext pmin : " << pmin << std::endl;
          count++;
        }
        if (configuration_.has_key("range_external_probability.max")) {
          double pmax = configuration_.fetch_real("range_external_probability.max");
          if (! configuration_.has_explicit_unit("range_external_probability.max")) {
            pmax *= CLHEP::perCent;
          }
          DT_THROW_IF(pmax < 0.0*CLHEP::perCent || pmax > 100.0*CLHEP::perCent,
                      std::range_error,
                      "Invalid maximal external probability (" << pmax << ") !");
          _prob_ext_max_ = pmax;
          // std::cout << "--------  ext pmax : " << pmax << std::endl;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_external_probability.min' or 'range_external_probability.max' property !");
        if (count == 2 && _prob_ext_min_ >= 0 && _prob_ext_max_ >= 0) {
          DT_THROW_IF(_prob_ext_min_ > _prob_ext_max_, std::logic_error,
                      "Invalid 'range_external_probability.min' > 'range_external_probability.max' values !");
        }
      }

      this->i_cut::_set_initialized(true);

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
      const std::string & a_pattern_id = a_pattern.get_pattern_id();
      if (a_pattern_id != snemo::datamodel::topology_1eNg_pattern::pattern_id()) {
        DT_LOG_DEBUG(get_logging_priority(), "This cut is only applicable to '"
                     << snemo::datamodel::topology_1eNg_pattern::pattern_id() << "' topology !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_1eNg_pattern & a_1eNg_pattern
        = dynamic_cast<const snemo::datamodel::topology_1eNg_pattern &>(a_pattern);

      // Check if event has gammas
      bool check_has_gamma = true;
      if (is_mode_has_gamma()) {
        if (a_1eNg_pattern.get_number_of_gammas() == 0)
          check_has_gamma = false;
      }

      // Check if event has correct number of gammas
      bool check_range_gamma = true;
      if (is_mode_range_gamma()) {
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
        if (! a_1eNg_pattern.has_internal_probabilities()) {
          check_has_internal_probability = false;
        }
      }

      // Check if event has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (! a_1eNg_pattern.has_internal_probabilities()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        snemo::datamodel::topology_1eNg_pattern::TOF_collection_type tof_collection = a_1eNg_pattern.get_TOF_collection();

        for(unsigned int i=0; i<tof_collection.size(); i++) {
          double pint = tof_collection.at(i).get_internal_probabilities().front();
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
          // std::cout << "------------------"  << std::endl;
          // std::cout << "        "<< i+1 <<"/" << tof_collection.size() << " gamma" << std::endl;
          // std::cout << "    int "<< _prob_int_min_ <<" < " << pint <<" < " << _prob_int_max_ << std::endl;
        }
      }

      // Check if event has external probability
      bool check_has_external_probability = true;
      if (is_mode_has_external_probability()) {
        if (! a_1eNg_pattern.has_external_probabilities()) {
          check_has_external_probability = false;
        }
      }

      // Check if event has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (! a_1eNg_pattern.has_external_probabilities()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        snemo::datamodel::topology_1eNg_pattern::TOF_collection_type tof_collection = a_1eNg_pattern.get_TOF_collection();

        for(unsigned int i=0; i<tof_collection.size(); i++) {
          for(unsigned int j=0; j<tof_collection.at(i).get_external_probabilities().size(); j++) {
            double pext = tof_collection.at(i).get_external_probabilities().at(j);
            if (datatools::is_valid(_prob_ext_min_)) {
              if (pext < _prob_ext_min_) {
                DT_LOG_DEBUG(get_logging_priority(),
                             "External probability (" << pext << ") lower than " << _prob_ext_min_);
                check_range_external_probability = false;
                // std::cout << "  !!!!!!  " << pext << " lower than " << _prob_ext_min_ <<std::endl;
              }
            }
            if (datatools::is_valid(_prob_ext_max_)) {
              if (pext > _prob_ext_max_) {
                DT_LOG_DEBUG(get_logging_priority(),
                             "External probability (" << pext << ") greater than " << _prob_ext_max_);
                check_range_external_probability = false;
                // std::cout << "  !!!!!!  " << pext << " greater than " << _prob_ext_max_ <<std::endl;
              }
              // std::cout << "        "<< i+1 <<"/" << tof_collection.size() << " gamma" << std::endl;
              // std::cout << "        "<< j+1 <<"/" << tof_collection.at(i).get_external_probabilities().size() << " calo" << std::endl;
              // std::cout << "    ext "<< _prob_ext_min_ <<" < " << pext <<" < " << _prob_ext_max_ << std::endl;
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
