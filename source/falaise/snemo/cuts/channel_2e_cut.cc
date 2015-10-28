// falaise/snemo/cuts/channel_2e_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/channel_2e_cut.h>

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
    CUT_REGISTRATION_IMPLEMENT(channel_2e_cut, "snemo::cut::channel_2e_cut");

    void channel_2e_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_channel_2e_data_label();
      datatools::invalidate(_prob_int_min_);
      datatools::invalidate(_prob_int_max_);
      datatools::invalidate(_prob_ext_min_);
      datatools::invalidate(_prob_ext_max_);
      datatools::invalidate(_vertices_probability_min_);
      datatools::invalidate(_vertices_probability_max_);
      datatools::invalidate(_angle_min_);
      datatools::invalidate(_angle_max_);
      datatools::invalidate(_minimal_energy_min_);
      datatools::invalidate(_minimal_energy_max_);
      datatools::invalidate(_maximal_energy_min_);
      datatools::invalidate(_maximal_energy_max_);
      return;
    }

    uint32_t channel_2e_cut::get_mode() const
    {
      return _mode_;
    }

    bool channel_2e_cut::is_mode_has_internal_probability() const
    {
      return _mode_ & MODE_HAS_INTERNAL_PROBABILITY;
    }

    bool channel_2e_cut::is_mode_has_external_probability() const
    {
      return _mode_ & MODE_HAS_EXTERNAL_PROBABILITY;
    }

    bool channel_2e_cut::is_mode_range_internal_probability() const
    {
      return _mode_ & MODE_RANGE_INTERNAL_PROBABILITY;
    }

    bool channel_2e_cut::is_mode_range_external_probability() const
    {
      return _mode_ & MODE_RANGE_EXTERNAL_PROBABILITY;
    }

    bool channel_2e_cut::is_mode_has_vertices_probability() const
    {
      return _mode_ & MODE_HAS_VERTICES_PROBABILITY;
    }

    bool channel_2e_cut::is_mode_range_vertices_probability() const
    {
      return _mode_ & MODE_RANGE_VERTICES_PROBABILITY;
    }

    bool channel_2e_cut::is_mode_has_angle() const
    {
      return _mode_ & MODE_HAS_ANGLE;
    }

    bool channel_2e_cut::is_mode_range_angle() const
    {
      return _mode_ & MODE_RANGE_ANGLE;
    }

    bool channel_2e_cut::is_mode_has_minimal_energy() const
    {
      return _mode_ & MODE_HAS_MINIMAL_ENERGY;
    }

    bool channel_2e_cut::is_mode_range_minimal_energy() const
    {
      return _mode_ & MODE_RANGE_MINIMAL_ENERGY;
    }

    bool channel_2e_cut::is_mode_has_maximal_energy() const
    {
      return _mode_ & MODE_HAS_MAXIMAL_ENERGY;
    }

    bool channel_2e_cut::is_mode_range_maximal_energy() const
    {
      return _mode_ & MODE_RANGE_MAXIMAL_ENERGY;
    }

    channel_2e_cut::channel_2e_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    channel_2e_cut::~channel_2e_cut()
    {
      if (is_initialized()) this->channel_2e_cut::reset();
      return;
    }

    void channel_2e_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void channel_2e_cut::initialize(const datatools::properties & configuration_,
                                    datatools::service_manager  & /* service_manager_ */,
                                    cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");
      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
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
      if (configuration_.has_flag("mode.has_vertices_probability")) {
        _mode_ |= MODE_HAS_VERTICES_PROBABILITY;
      }
      if (configuration_.has_flag("mode.range_vertices_probability")) {
        _mode_ |= MODE_RANGE_VERTICES_PROBABILITY;
      }
      if (configuration_.has_flag("mode.has_angle")) {
        _mode_ |= MODE_HAS_ANGLE;
      }
      if (configuration_.has_flag("mode.range_angle")) {
        _mode_ |= MODE_RANGE_ANGLE;
      }
      if (configuration_.has_flag("mode.has_minimal_energy")) {
        _mode_ |= MODE_HAS_MINIMAL_ENERGY;
      }
      if (configuration_.has_flag("mode.range_minimal_energy")) {
        _mode_ |= MODE_RANGE_MINIMAL_ENERGY;
      }
      if (configuration_.has_flag("mode.has_maximal_energy")) {
        _mode_ |= MODE_HAS_MAXIMAL_ENERGY;
      }
      if (configuration_.has_flag("mode.range_maximal_energy")) {
        _mode_ |= MODE_RANGE_MAXIMAL_ENERGY;
      }
      DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                  "Missing at least a 'mode.XXX' property !");

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

      if (is_mode_range_vertices_probability()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_VERTICES_PROBABILITY mode...");
        size_t count = 0;
        // if (configuration_.has_key("range_vertices_probability.location")) {
        //   std::string vertices_probability_location = configuration_.fetch_string("range_vertices_probability.location");
        //   DT_THROW_IF(!(vertices_probability_location == "source" ||
        //               vertices_probability_location == "calorimeter" ||
        //                 vertices_probability_location == "tracker"), std::logic_error,
        //               "Invalid common vertices location (" << vertices_probability_location << ") !");
        //   _vertices_probability_location_ = vertices_probability_location;
        // }
        if (configuration_.has_key("range_vertices_probability.min")) {
          double vertices_probability_min = configuration_.fetch_real("range_vertices_probability.min");
          DT_THROW_IF(vertices_probability_min < 0.0 || vertices_probability_min > 1, std::range_error,
                      "Invalid minimal vertices probability (" << vertices_probability_min << ") !");
          _vertices_probability_min_ = vertices_probability_min;
          count++;
        }
        if (configuration_.has_key("range_vertices_probability.max")) {
          double vertices_probability_max = configuration_.fetch_real("range_vertices_probability.max");
          DT_THROW_IF(vertices_probability_max < 0.0 || vertices_probability_max > 1, std::range_error,
                      "Invalid maximal vertices probability (" << vertices_probability_max << ") !");
          _vertices_probability_max_ = vertices_probability_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_vertices_probability.min' or 'range_vertices_probability.max' property !");
        if (count == 2) {// && _delta_vertices_y_min_ >= 0 && _delta_vertices_y_max_ >= 0) {
          DT_THROW_IF(_vertices_probability_min_ > _vertices_probability_max_, std::logic_error,
                      "Invalid 'range_vertices_probability.min' > 'range_vertices_probability.max' values !");
        }
      }

      if (is_mode_range_angle()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_ANGLE mode...");
        size_t count = 0;
        if (configuration_.has_key("range_angle.min")) {
          double angle_min = configuration_.fetch_real("range_angle.min");
          if (! configuration_.has_explicit_unit("range_angle.min")) {
            angle_min *= CLHEP::degree;
          }
          DT_THROW_IF(angle_min < 0.0*CLHEP::degree || angle_min > 180.*CLHEP::degree, std::range_error,
                      "Invalid minimal angle (" << angle_min << ") !");
          _angle_min_ = angle_min;
          count++;
        }
        if (configuration_.has_key("range_angle.max")) {
          double angle_max = configuration_.fetch_real("range_angle.max");
          if (! configuration_.has_explicit_unit("range_angle.max")) {
            angle_max *= CLHEP::degree;
          }
          DT_THROW_IF(angle_max < 0.0*CLHEP::degree || angle_max > 180.*CLHEP::degree, std::range_error,
                      "Invalid maximal angle (" << angle_max << ") !");
          _angle_max_ = angle_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_angle.min' or 'range_angle.max' property !");
        if (count == 2 && _angle_min_ >= 0 && _angle_max_ >= 0) {
          DT_THROW_IF(_angle_min_ > _angle_max_, std::logic_error,
                      "Invalid 'range_angle.min' > 'range_angle.max' values !");
        }
      }

      if (is_mode_range_minimal_energy()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_MINIMAL_ENERGY mode...");
        size_t count = 0;
        if (configuration_.has_key("range_minimal_energy.min")) {
          double minimal_energy_min = configuration_.fetch_real("range_minimal_energy.min");
          if (! configuration_.has_explicit_unit("range_minimal_energy.min")) {
            minimal_energy_min *= CLHEP::MeV;
          }
          DT_THROW_IF(minimal_energy_min < 0.0*CLHEP::MeV, std::range_error,
                      "Invalid minimal minimal_energy (" << minimal_energy_min << ") !");
          _minimal_energy_min_ = minimal_energy_min;
          count++;
        }
        if (configuration_.has_key("range_minimal_energy.max")) {
          double minimal_energy_max = configuration_.fetch_real("range_minimal_energy.max");
          if (! configuration_.has_explicit_unit("range_minimal_energy.max")) {
            minimal_energy_max *= CLHEP::degree;
          }
          DT_THROW_IF(minimal_energy_max < 0.0*CLHEP::degree, std::range_error,
                      "Invalid maximal minimal_energy (" << minimal_energy_max << ") !");
          _minimal_energy_max_ = minimal_energy_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_minimal_energy.min' or 'range_minimal_energy.max' property !");
        if (count == 2 && _minimal_energy_min_ >= 0 && _minimal_energy_max_ >= 0) {
          DT_THROW_IF(_minimal_energy_min_ > _minimal_energy_max_, std::logic_error,
                      "Invalid 'range_minimal_energy.min' > 'range_minimal_energy.max' values !");
        }
      }

      if (is_mode_range_maximal_energy()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_MAXIMAL_ENERGY mode...");
        size_t count = 0;
        if (configuration_.has_key("range_maximal_energy.min")) {
          double maximal_energy_min = configuration_.fetch_real("range_maximal_energy.min");
          if (! configuration_.has_explicit_unit("range_maximal_energy.min")) {
            maximal_energy_min *= CLHEP::MeV;
          }
          DT_THROW_IF(maximal_energy_min < 0.0*CLHEP::MeV, std::range_error,
                      "Invalid minimal maximal_energy (" << maximal_energy_min << ") !");
          _maximal_energy_min_ = maximal_energy_min;
          count++;
        }
        if (configuration_.has_key("range_maximal_energy.max")) {
          double maximal_energy_max = configuration_.fetch_real("range_maximal_energy.max");
          if (! configuration_.has_explicit_unit("range_maximal_energy.max")) {
            maximal_energy_max *= CLHEP::degree;
          }
          DT_THROW_IF(maximal_energy_max < 0.0*CLHEP::degree, std::range_error,
                      "Invalid maximal maximal_energy (" << maximal_energy_max << ") !");
          _maximal_energy_max_ = maximal_energy_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_maximal_energy.min' or 'range_maximal_energy.max' property !");
        if (count == 2 && _maximal_energy_min_ >= 0 && _maximal_energy_max_ >= 0) {
          DT_THROW_IF(_maximal_energy_min_ > _maximal_energy_max_, std::logic_error,
                      "Invalid 'range_maximal_energy.min' > 'range_maximal_energy.max' values !");
        }
      }

      this->i_cut::_set_initialized(true);
      return;
    }


    int channel_2e_cut::_accept()
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
      // bizarre
      if (a_pattern_id != "2e") {
        DT_LOG_DEBUG(get_logging_priority(), "This cut is only applicable to '"
                     << "2e" << "' topology !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_2e_pattern & a_2e_pattern
        = dynamic_cast<const snemo::datamodel::topology_2e_pattern &>(a_pattern);

      // Check if event has internal probability
      bool check_has_internal_probability = true;
      if (is_mode_has_internal_probability()) {
        if (! a_2e_pattern.has_electrons_internal_probability()) {
          check_has_internal_probability = false;
        }
      }

      // Check if event has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (! a_2e_pattern.has_electrons_internal_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double pint = a_2e_pattern.get_electrons_internal_probability();
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
        if (! a_2e_pattern.has_electrons_external_probability()) {
          check_has_external_probability = false;
        }
      }

      // Check if event has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (! a_2e_pattern.has_electrons_external_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double pext = a_2e_pattern.get_electrons_external_probability();
        if (datatools::is_valid(_prob_ext_min_)) {
          if (pext < _prob_ext_min_) check_range_external_probability = false;
        }
        if (datatools::is_valid(_prob_ext_max_)) {
          if (pext > _prob_ext_max_) check_range_external_probability = false;
        }
      }

      // Check if event has common vertices probability
      bool check_has_vertices_probability = true;
      if (is_mode_has_vertices_probability()) {
        if (! a_2e_pattern.has_electrons_vertices_probability()) {
          check_has_vertices_probability = false;
        }
      }

      // Check if event has required common vertices probability
      bool check_range_vertices_probability = true;
      if (is_mode_range_vertices_probability()) {
        if (! a_2e_pattern.has_electrons_vertices_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing common vertices probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        // const std::string vertices_probability_location = a_2e_pattern.get_electrons_vertices_location();
        // if (vertices_probability_location != _vertices_probability_location_) {
        //   DT_LOG_DEBUG(get_logging_priority(),
        //                "Common vertices location (" << vertices_probability_location << ") different than " << _vertices_probability_location_);
        //   check_range_vertices_probability = false;
        // }
        const double vertices_probability = a_2e_pattern.get_electrons_vertices_probability();
        if (datatools::is_valid(_vertices_probability_min_)) {
          if (vertices_probability < _vertices_probability_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Common vertices probability (" << vertices_probability << ") lower than " << _vertices_probability_min_);
            check_range_vertices_probability = false;
          }
        }
        if (datatools::is_valid(_vertices_probability_max_)) {
          if (vertices_probability > _vertices_probability_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices probability (" << vertices_probability << ") greater than " << _vertices_probability_max_);
            check_range_vertices_probability = false;
          }
        }
      }

      // Check if event has angle
      bool check_has_angle = true;
      if (is_mode_has_angle()) {
        if (! a_2e_pattern.has_electrons_angle()) {
            check_has_angle = false;
        }
      }

      // Check if event has required angle
      bool check_range_angle = true;
      if (is_mode_range_angle()) {
        if (! a_2e_pattern.has_electrons_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing angle !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double angle = a_2e_pattern.get_electrons_angle();
        if (datatools::is_valid(_angle_min_)) {
          if (angle < _angle_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle << ") lower than " << _angle_min_);
            check_range_angle = false;
          }
        }
        if (datatools::is_valid(_angle_max_)) {
          if (angle > _angle_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle << ") greater than " << _angle_max_);
            check_range_angle = false;
          }
        }
      }

      // Check if event has minimal energy
      bool check_has_minimal_energy = true;
      if (is_mode_has_minimal_energy()) {
        if (! a_2e_pattern.has_electron_minimal_energy()) {
            check_has_minimal_energy = false;
        }
      }

      // Check if event has required electron minimal energy
      bool check_range_minimal_energy = true;
      if (is_mode_range_minimal_energy()) {
        if (! a_2e_pattern.has_electron_minimal_energy()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing minimal energy !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double minimal_energy = a_2e_pattern.get_electron_minimal_energy();
        if (datatools::is_valid(_minimal_energy_min_)) {
          if ( minimal_energy < _minimal_energy_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Minimal energy (" << minimal_energy << ") lower than " << _minimal_energy_min_);
            check_range_minimal_energy = false;
          }
        }
        if (datatools::is_valid(_minimal_energy_max_)) {
          if (minimal_energy > _minimal_energy_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Minimal energy (" << minimal_energy << ") greater than " << _minimal_energy_max_);
            check_range_minimal_energy = false;
          }
        }
      }

      // Check if event has maximal energy
      bool check_has_maximal_energy = true;
      if (is_mode_has_maximal_energy()) {
        if (! a_2e_pattern.has_electron_maximal_energy()) {
            check_has_maximal_energy = false;
        }
      }

      // Check if event has required electron maximal energy
      bool check_range_maximal_energy = true;
      if (is_mode_range_maximal_energy()) {
        if (! a_2e_pattern.has_electron_maximal_energy()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing maximal energy !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double maximal_energy = a_2e_pattern.get_electron_maximal_energy();
        if (datatools::is_valid(_maximal_energy_min_)) {
          if ( maximal_energy < _maximal_energy_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Maximal energy (" << maximal_energy << ") lower than " << _maximal_energy_min_);
            check_range_maximal_energy = false;
          }
        }
        if (datatools::is_valid(_maximal_energy_max_)) {
          if (maximal_energy > _maximal_energy_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Maximal energy (" << maximal_energy << ") greater than " << _maximal_energy_max_);
            check_range_maximal_energy = false;
          }
        }
      }

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_internal_probability &&
          check_has_external_probability &&
          check_range_internal_probability &&
          check_range_external_probability &&
          check_has_vertices_probability &&
          check_range_vertices_probability &&
          check_has_angle &&
          check_range_angle &&
          check_has_minimal_energy &&
          check_range_minimal_energy &&
          check_has_maximal_energy &&
          check_range_maximal_energy) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by channel 2e cut!");
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
