// falaise/snemo/cuts/angle_measurement_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/angle_measurement_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>
#include <datatools/clhep_units.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/angle_measurement.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(angle_measurement_cut, "snemo::cut::angle_measurement_cut")

    void angle_measurement_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      datatools::invalidate(_angle_range_min_);
      datatools::invalidate(_angle_range_max_);
      return;
    }

    uint32_t angle_measurement_cut::get_mode() const
    {
      return _mode_;
    }

    bool angle_measurement_cut::is_mode_has_angle() const
    {
      return _mode_ & MODE_HAS_ANGLE;
    }

    bool angle_measurement_cut::is_mode_range_angle() const
    {
      return _mode_ & MODE_RANGE_ANGLE;
    }

    angle_measurement_cut::angle_measurement_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      this->register_supported_user_data_type<snemo::datamodel::base_topology_measurement>();
      this->register_supported_user_data_type<snemo::datamodel::angle_measurement>();
      return;
    }

    angle_measurement_cut::~angle_measurement_cut()
    {
      if (is_initialized()) this->angle_measurement_cut::reset();
      return;
    }

    void angle_measurement_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void angle_measurement_cut::initialize(const datatools::properties & configuration_,
                                           datatools::service_manager  & /* service_manager_ */,
                                           cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (_mode_ == MODE_UNDEFINED) {
        if (configuration_.has_flag("mode.has_angle")) {
          _mode_ |= MODE_HAS_ANGLE;
        }
        if (configuration_.has_flag("mode.range_angle")) {
          _mode_ |= MODE_RANGE_ANGLE;
        }
        DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                    "Missing at least a 'mode.XXX' property !");

        // mode HAS_ANGLE:
        if (is_mode_has_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using HAS_ANGLE mode...");
        } // end if is_mode_has_angle

        // mode PARTICLE_RANGE_ANGLE:
        if (is_mode_range_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_ANGLE mode...");
          size_t count = 0;
          if (configuration_.has_key("range_angle.min")) {
            double amin = configuration_.fetch_real("range_angle.min");
            if (! configuration_.has_explicit_unit("range_angle.min")) {
              amin *= CLHEP::degree;
            }
            DT_THROW_IF(amin < 0.0*CLHEP::degree || amin > 360.0*CLHEP::degree,
                        std::range_error,
                        "Invalid minimal angle value (" << amin << ") !");
            _angle_range_min_ = amin;
            count++;
          }
          if (configuration_.has_key("range_angle.max")) {
            double amax = configuration_.fetch_real("range_angle.max");
            if (! configuration_.has_explicit_unit("range_angle.max")) {
              amax *= CLHEP::degree;
            }
            DT_THROW_IF(amax < 0.0*CLHEP::degree || amax > 360.0*CLHEP::degree,
                        std::range_error,
                        "Invalid maximal angle (" << amax << ") !");
            _angle_range_max_ = amax;
            count++;
          }
          DT_THROW_IF(count == 0, std::logic_error,
                      "Missing 'range_angle.min' or 'range_angle.max' property !");
          if (count == 2 && _angle_range_min_ >= 0 && _angle_range_max_ >= 0) {
            DT_THROW_IF(_angle_range_min_ > _angle_range_max_, std::logic_error,
                        "Invalid 'range_angle.min' > 'range_angle.max' values !");
          }
        } // end if is_mode_range_angle
      }
      this->i_cut::_set_initialized(true);
      return;
    }


    int angle_measurement_cut::_accept()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get angle measurement
      const snemo::datamodel::angle_measurement * ptr_meas = 0;
      if (is_user_data<snemo::datamodel::angle_measurement>()) {
        ptr_meas = &(get_user_data<snemo::datamodel::angle_measurement>());
      } else if (is_user_data<snemo::datamodel::base_topology_measurement>()) {
        const snemo::datamodel::base_topology_measurement & btm
          = get_user_data<snemo::datamodel::base_topology_measurement>();
        ptr_meas = dynamic_cast<const snemo::datamodel::angle_measurement *>(&btm);
      } else {
        DT_THROW_IF(true, std::logic_error, "Invalid data type !");
      }
      const snemo::datamodel::angle_measurement & a_angle_meas = *ptr_meas;

      // Check if measurement has angle
      bool check_has_angle = true;
      if (is_mode_has_angle()) {
        if (! a_angle_meas.has_angle()) {
          check_has_angle = false;
        }
      }

      // Check if measurement has correct angle
      bool check_range_angle = true;
      if (is_mode_range_angle()) {
        if (! a_angle_meas.has_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing angle !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double angle = a_angle_meas.get_angle();
        bool check = true;
        if (datatools::is_valid(_angle_range_min_)) {
          if (angle < _angle_range_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle/CLHEP::degree << "°) lower than "
                         << _angle_range_min_/CLHEP::degree << "°");
            check = false;
          }
        }
        if (datatools::is_valid(_angle_range_max_)) {
          if (angle > _angle_range_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle/CLHEP::degree << "°) greater than "
                         << _angle_range_max_/CLHEP::degree << "°");
            check = false;
          }
        }
        if (! check) check_range_angle = false;
      } // end of is_mode_range_angle

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_angle &&
          check_range_angle) {
        DT_LOG_DEBUG(get_logging_priority(), "Event accepted by angle measurement cut!");
        cut_returned = cuts::SELECTION_ACCEPTED;
      }
      return cut_returned;
    }

  }  // end of namespace cut

}  // end of namespace snemo

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::cut::angle_measurement_cut, ocd_)
{
  ocd_.set_class_name("snemo::cut::angle_measurement_cut");
  ocd_.set_class_description("Cut based on criteria applied to a angle measurement");
  ocd_.set_class_library("falaise");
  // ocd_.set_class_documentation("");

  cuts::i_cut::common_ocd(ocd_);

  {
    // Description of the 'mode.has_angle' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.has_angle")
      .set_terse_description("Mode requiring angle availability")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the requested angle mode:: \n"
                   "                                    \n"
                   "  mode.has_angle : boolean = true   \n"
                   "                                    \n"
                   )
      ;
  }

  {
    // Description of the 'mode.range_nagle' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.range_angle")
      .set_terse_description("Mode with a special requested ranged of angle value")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the mode::                 \n"
                   "                                    \n"
                   "  mode.range_angle : boolean = true \n"
                   "                                    \n"
                   )
      ;
  }

  {
    // Description of the 'range_angle.min' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_angle.min")
      .set_terse_description("Minimal value of the requested ranged angle")
      .set_triggered_by_flag("mode.range_angle")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("angle")
      .set_unit_symbol("degree")
      .add_example("Set a specific minimal value of the angle::   \n"
                   "                                              \n"
                   "  range_angle.min : real as angle = 30 degree \n"
                   "                                              \n"
                   )
      ;
  }

  {
    // Description of the 'range_angle.max' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_angle.max")
      .set_terse_description("Maximal value of the requested ranged angle")
      .set_triggered_by_flag("mode.range_angle")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("angle")
      .set_unit_symbol("degree")
      .add_example("Set a specific maximal value of the angle::   \n"
                   "                                              \n"
                   "  range_angle.max : real as angle = 90 degree \n"
                   "                                              \n"
                   )
      ;
  }

  // Additional configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the     \n"
                               "``datatools::properties`` ASCII format::        \n"
                               "                                                \n"
                               "   mode.has_angle : boolean = true              \n"
                               "   mode.range_angle : boolean = true            \n"
                               "   range_angle.min : real as angle = 5 degree   \n"
                               "   range_angle.max : real as angle = 100 degree \n"
                               "                                                \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation

// Registration macro for class 'snemo::cut::simulated_data_cut' :
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::cut::angle_measurement_cut, "snemo::cut::angle_measurement_cut")

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
