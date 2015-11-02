// falaise/snemo/cuts/base_channel_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/base_channel_cut.h>

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

    // // Registration instantiation macro :
    // CUT_REGISTRATION_IMPLEMENT(base_channel_cut, "snemo::cut::base_channel_cut");

    void base_channel_cut::_set_defaults()
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
      return;
    }

    uint32_t base_channel_cut::get_mode() const
    {
      return _mode_;
    }

    bool base_channel_cut::is_mode_has_internal_probability() const
    {
      return _mode_ & MODE_HAS_INTERNAL_PROBABILITY;
    }

    bool base_channel_cut::is_mode_has_external_probability() const
    {
      return _mode_ & MODE_HAS_EXTERNAL_PROBABILITY;
    }

    bool base_channel_cut::is_mode_range_internal_probability() const
    {
      return _mode_ & MODE_RANGE_INTERNAL_PROBABILITY;
    }

    bool base_channel_cut::is_mode_range_external_probability() const
    {
      return _mode_ & MODE_RANGE_EXTERNAL_PROBABILITY;
    }

    bool base_channel_cut::is_mode_has_vertices_probability() const
    {
      return _mode_ & MODE_HAS_VERTICES_PROBABILITY;
    }

    bool base_channel_cut::is_mode_range_vertices_probability() const
    {
      return _mode_ & MODE_RANGE_VERTICES_PROBABILITY;
    }

    bool base_channel_cut::is_mode_has_angle() const
    {
      return _mode_ & MODE_HAS_ANGLE;
    }

    bool base_channel_cut::is_mode_range_angle() const
    {
      return _mode_ & MODE_RANGE_ANGLE;
    }

    base_channel_cut::base_channel_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    base_channel_cut::~base_channel_cut()
    {
      if (is_initialized()) this->base_channel_cut::reset();
      return;
    }

    void base_channel_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void base_channel_cut::initialize(const datatools::properties & configuration_,
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

      this->i_cut::_set_initialized(true);
      return;
    }


  }  // end of namespace cut

}  // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
