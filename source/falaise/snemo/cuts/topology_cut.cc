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
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_2e_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(topology_cut, "snemo::cut::topology_cut");

    void topology_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      datatools::invalidate(_prob_int_min_);
      datatools::invalidate(_prob_int_max_);
      datatools::invalidate(_prob_ext_min_);
      datatools::invalidate(_prob_ext_max_);
      return;
    }

    uint32_t topology_cut::get_mode() const
    {
      return _mode_;
    }

    bool topology_cut::is_mode_pattern_id() const
    {
      return _mode_ & MODE_PATTERN_ID;
    }

    bool topology_cut::is_mode_has_internal_probability() const
    {
      return _mode_ & MODE_HAS_INTERNAL_PROBABILITY;
    }

    bool topology_cut::is_mode_has_external_probability() const
    {
      return _mode_ & MODE_HAS_EXTERNAL_PROBABILITY;
    }

    bool topology_cut::is_mode_range_internal_probability() const
    {
      return _mode_ & MODE_RANGE_INTERNAL_PROBABILITY;
    }

    bool topology_cut::is_mode_range_external_probability() const
    {
      return _mode_ & MODE_RANGE_EXTERNAL_PROBABILITY;
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

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }

      if (configuration_.has_flag("mode.pattern_id")) {
        _mode_ |= MODE_PATTERN_ID;
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

      if (is_mode_pattern_id()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using PATTERN_ID mode...");
        DT_THROW_IF(! configuration_.has_key("pattern_id.label"), std::logic_error,
                    "Missing 'pattern_id.label' !");
        _pattern_id_label_ = configuration_.fetch_string("pattern_id.label");
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
        if (configuration_.has_key("range_internal_probablity.max")) {
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
          count++;
        }
        if (configuration_.has_key("range_external_probablity.max")) {
          double pmax = configuration_.fetch_real("range_external_probability.max");
          if (! configuration_.has_explicit_unit("range_external_probability.max")) {
            pmax *= CLHEP::perCent;
          }
          DT_THROW_IF(pmax < 0.0*CLHEP::perCent || pmax > 100.0*CLHEP::perCent,
                      std::range_error,
                      "Invalid maximal external probability (" << pmax << ") !");
          _prob_ext_max_ = pmax;
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


    int topology_cut::_accept()
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

      // Check if event has the correct pattern id
      bool check_pattern_id = true;
      if (is_mode_pattern_id()) {
        DT_LOG_DEBUG(get_logging_priority(), "Running PATTERN_ID mode...");
        if (a_pattern_id != _pattern_id_label_) check_pattern_id = false;
      }

      // Check if event has internal probability
      bool check_has_internal_probability = true;
      if (is_mode_has_internal_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "Internal probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_internal_probability()) {
          check_has_internal_probability = false;
        }
      }

      // Check if event has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "Internal probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_internal_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        const double pint = ptr_2e_pattern->get_internal_probability();
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

      // Check if event has external probability
      bool check_has_external_probability = true;
      if (is_mode_has_external_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "External probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_external_probability()) {
          check_has_external_probability = false;
        }
      }

      // Check if event has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "External probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_external_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        const double pext = ptr_2e_pattern->get_external_probability();
        if (datatools::is_valid(_prob_ext_min_)) {
          if (pext < _prob_ext_min_) check_range_external_probability = false;
        }
        if (datatools::is_valid(_prob_ext_max_)) {
          if (pext > _prob_ext_max_) check_range_external_probability = false;
        }
      }

      // double internal_prob = datatools::invalid_real();
      // double external_prob = datatools::invalid_real();
      // double delta_vertices_y = datatools::invalid_real();
      // double delta_vertices_z = datatools::invalid_real();

      // if(a_pattern_id == snemo::datamodel::topology_2e_pattern::pattern_id()) {
      //   const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
      //     = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
      //   // std::cout << "--------------pattern id " << a_pattern.get_pattern_id() << std::endl;

      //   internal_prob    = ptr_2e_pattern->get_internal_probability();
      //   external_prob    = ptr_2e_pattern->get_external_probability();
      //   delta_vertices_y = std::fabs(ptr_2e_pattern->get_delta_vertices_y());
      //   delta_vertices_z = std::fabs(ptr_2e_pattern->get_delta_vertices_z());
      // }

      // std::cout << _prob_int_ << " " << _prob_ext_ << " " << _delta_vertices_y_ << " " << _delta_vertices_z_ << std::endl;
      // std::cout << internal_prob << " " << external_prob << " " << delta_vertices_y << " " << delta_vertices_z << std::endl;
      // if (internal_prob < _prob_int_)
      //   check = false;
      // if (external_prob > _prob_ext_)
      //   check = false;
      // if (delta_vertices_y > _delta_vertices_y_)
      //   check = false;
      // if (delta_vertices_z > _delta_vertices_z_)
      //   check = false;

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_pattern_id &&
          check_has_internal_probability &&
          check_has_external_probability &&
          check_range_internal_probability &&
          check_range_external_probability) {
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
