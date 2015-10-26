// falaise/snemo/cuts/topology_data_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/topology_data_cut.h>

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

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(topology_data_cut, "snemo::cut::topology_data_cut");

    void topology_data_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      return;
    }

    uint32_t topology_data_cut::get_mode() const
    {
      return _mode_;
    }

    bool topology_data_cut::is_mode_has_pattern_id() const
    {
      return _mode_ & MODE_HAS_PATTERN_ID;
    }

    bool topology_data_cut::is_mode_pattern_id() const
    {
      return _mode_ & MODE_PATTERN_ID;
    }

    topology_data_cut::topology_data_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    topology_data_cut::~topology_data_cut()
    {
      if (is_initialized()) this->topology_data_cut::reset();
      return;
    }

    void topology_data_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void topology_data_cut::initialize(const datatools::properties & configuration_,
                                        datatools::service_manager  & /* service_manager_ */,
                                        cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }
      if (configuration_.has_flag("mode.has_pattern_id")) {
        _mode_ |= MODE_HAS_PATTERN_ID;
      }
      if (configuration_.has_flag("mode.pattern_id")) {
        _mode_ |= MODE_PATTERN_ID;
      }
      DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                  "Missing at least a 'mode.XXX' property !");

      if (is_mode_pattern_id()) {
        std::cout << "test mode pattern_id" << std::endl;
        DT_LOG_DEBUG(get_logging_priority(), "Using PATTERN_ID mode...");
        DT_THROW_IF(! configuration_.has_key("pattern_id.label"), std::logic_error,
                    "Missing 'pattern_id.label' !");
        _pattern_id_label_ = configuration_.fetch_string("pattern_id.label");
      }

      this->i_cut::_set_initialized(true);
      return;
    }

    int topology_data_cut::_accept()
    {
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cut_returned;
      }

      const snemo::datamodel::topology_data & TD = ER.get<snemo::datamodel::topology_data>(_TD_label_);

      // Check if event has the pattern id
      bool check_has_pattern_id = true;
      if (is_mode_has_pattern_id()) {
        DT_LOG_DEBUG(get_logging_priority(), "Running HAS_PATTERN_ID mode...");
        if (! TD.has_pattern()) {
          check_has_pattern_id = false;
          std::cout << "no pattern" << std::endl;
        }
      }

      // Check if event has the correct pattern id
      bool check_pattern_id = true;
      if (is_mode_pattern_id()) {
        std::cout << "is mode" << std::endl;
        DT_LOG_DEBUG(get_logging_priority(), "Running PATTERN_ID mode...");
        if (! TD.has_pattern()) {
          DT_LOG_DEBUG(get_logging_priority(), "The event does not have associated topology pattern !");
          std::cout << "non applicable" << std::endl;
          return cuts::SELECTION_INAPPLICABLE;
          // return cuts::SELECTION_REJECTED;
        }
        const snemo::datamodel::base_topology_pattern & a_pattern = TD.get_pattern();
        const std::string & a_pattern_id = a_pattern.pattern_id();
        if (a_pattern_id != _pattern_id_label_) check_pattern_id = false;
        std::cout << "a pattern id " << a_pattern_id << std::endl;

      }

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_pattern_id &&
          check_pattern_id) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by topology cut!");
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
