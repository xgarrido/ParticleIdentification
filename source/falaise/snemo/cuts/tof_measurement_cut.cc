// falaise/snemo/cuts/tof_measurement_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/tof_measurement_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>
#include <datatools/clhep_units.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/tof_measurement.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(tof_measurement_cut, "snemo::cut::tof_measurement_cut")

    void tof_measurement_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _int_prob_range_mode_ = MODE_RANGE_UNDEFINED;
      _ext_prob_range_mode_ = MODE_RANGE_UNDEFINED;
      datatools::invalidate(_int_prob_range_min_);
      datatools::invalidate(_int_prob_range_max_);
      datatools::invalidate(_ext_prob_range_min_);
      datatools::invalidate(_ext_prob_range_max_);
      return;
    }

    uint32_t tof_measurement_cut::get_mode() const
    {
      return _mode_;
    }

    bool tof_measurement_cut::is_mode_has_internal_probability() const
    {
      return _mode_ & MODE_HAS_INTERNAL_PROBABILITY;
    }

    bool tof_measurement_cut::is_mode_range_internal_probability() const
    {
      return _mode_ & MODE_RANGE_INTERNAL_PROBABILITY;
    }

    bool tof_measurement_cut::is_mode_has_external_probability() const
    {
      return _mode_ & MODE_HAS_EXTERNAL_PROBABILITY;
    }

    bool tof_measurement_cut::is_mode_range_external_probability() const
    {
      return _mode_ & MODE_RANGE_EXTERNAL_PROBABILITY;
    }

    tof_measurement_cut::tof_measurement_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      this->register_supported_user_data_type<snemo::datamodel::base_topology_measurement>();
      this->register_supported_user_data_type<snemo::datamodel::tof_measurement>();
      return;
    }

    tof_measurement_cut::~tof_measurement_cut()
    {
      if (is_initialized()) this->tof_measurement_cut::reset();
      return;
    }

    void tof_measurement_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void tof_measurement_cut::initialize(const datatools::properties & configuration_,
                                         datatools::service_manager  & /* service_manager_ */,
                                         cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (_mode_ == MODE_UNDEFINED) {
        if (configuration_.has_flag("mode.has_internal_probability")) {
          _mode_ |= MODE_HAS_INTERNAL_PROBABILITY;
        }
        if (configuration_.has_flag("mode.range_internal_probability")) {
          _mode_ |= MODE_RANGE_INTERNAL_PROBABILITY;
        }
        if (configuration_.has_flag("mode.has_external_probability")) {
          _mode_ |= MODE_HAS_EXTERNAL_PROBABILITY;
        }
        if (configuration_.has_flag("mode.range_external_probability")) {
          _mode_ |= MODE_RANGE_EXTERNAL_PROBABILITY;
        }
        DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                    "Missing at least a 'mode.XXX' property !");

        // mode HAS_INTERNAL_PROBABILITY:
        if (is_mode_has_internal_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using HAS_INTERNAL_PROBABILITY mode...");
        } // end if is_mode_has_internal_probability

        // mode PARTICLE_RANGE_INTERNAL_PROBABILITY:
        if (is_mode_range_internal_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_INTERNAL_PROBABILITY mode...");
          if (configuration_.has_key("range_internal_probability.mode")) {
            const std::string mode = configuration_.fetch_string("range_internal_probability.mode");
            if (mode == "strict") {
              _int_prob_range_mode_ = MODE_RANGE_STRICT;
            } else if (mode == "all") {
              _int_prob_range_mode_ = MODE_RANGE_ALL;
            } else {
              DT_THROW_IF(true, std::logic_error, "Unkown '" << mode << "' mode for internal probability range !");
            }
          }
          DT_THROW_IF(_int_prob_range_mode_ == MODE_RANGE_UNDEFINED, std::logic_error,
                      "Mode for internal probability range has not been set !");

          size_t count = 0;
          if (configuration_.has_key("range_internal_probability.min")) {
            double pmin = configuration_.fetch_real("range_internal_probability.min");
            if (! configuration_.has_explicit_unit("range_internal_probability.min")) {
              pmin *= CLHEP::perCent;
            }
            DT_THROW_IF(pmin < 0.0*CLHEP::perCent || pmin > 100.0*CLHEP::perCent,
                        std::range_error,
                        "Invalid minimal internal probability (" << pmin << ") !");
            _int_prob_range_min_ = pmin;
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
            _int_prob_range_max_ = pmax;
            count++;
          }
          DT_THROW_IF(count == 0, std::logic_error,
                      "Missing 'range_internal_probability.min' or 'range_internal_probability.max' property !");
          if (count == 2 && _int_prob_range_min_ >= 0 && _int_prob_range_max_ >= 0) {
            DT_THROW_IF(_int_prob_range_min_ > _int_prob_range_max_, std::logic_error,
                        "Invalid 'range_internal_probability.min' > 'range_internal_probability.max' values !");
          }
        } // end if is_mode_range_internal_probability

        // mode HAS_EXTERNAL_PROBABILITY:
        if (is_mode_has_external_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using HAS_EXTERNAL_PROBABILITY mode...");
        } // end if is_mode_has_external_probability

        // mode PARTICLE_RANGE_EXTERNAL_PROBABILITY:
        if (is_mode_range_external_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_EXTERNAL_PROBABILITY mode...");
          if (configuration_.has_key("range_external_probability.mode")) {
            const std::string mode = configuration_.fetch_string("range_external_probability.mode");
            if (mode == "strict") {
              _ext_prob_range_mode_ = MODE_RANGE_STRICT;
            } else if (mode == "all") {
              _ext_prob_range_mode_ = MODE_RANGE_ALL;
            } else {
              DT_THROW_IF(true, std::logic_error, "Unkown '" << mode << "' mode for external probability range !");
            }
          }
          DT_THROW_IF(_ext_prob_range_mode_ == MODE_RANGE_UNDEFINED, std::logic_error,
                      "Mode for external probability range has not been set !");
         size_t count = 0;
          if (configuration_.has_key("range_external_probability.min")) {
            double pmin = configuration_.fetch_real("range_external_probability.min");
            if (! configuration_.has_explicit_unit("range_external_probability.min")) {
              pmin *= CLHEP::perCent;
            }
            DT_THROW_IF(pmin < 0.0*CLHEP::perCent || pmin > 100.0*CLHEP::perCent,
                        std::range_error,
                        "Invalid minimal external probability (" << pmin << ") !");
            _ext_prob_range_min_ = pmin;
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
            _ext_prob_range_max_ = pmax;
            count++;
          }
          DT_THROW_IF(count == 0, std::logic_error,
                      "Missing 'range_external_probability.min' or 'range_external_probability.max' property !");
          if (count == 2 && _ext_prob_range_min_ >= 0 && _ext_prob_range_max_ >= 0) {
            DT_THROW_IF(_ext_prob_range_min_ > _ext_prob_range_max_, std::logic_error,
                        "Invalid 'range_external_probability.min' > 'range_external_probability.max' values !");
          }
        } // end if is_mode_range_external_probability
     }

      this->i_cut::_set_initialized(true);
      return;
    }


    int tof_measurement_cut::_accept()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get tof measurement
      const snemo::datamodel::tof_measurement * ptr_meas = 0;
      if (is_user_data<snemo::datamodel::tof_measurement>()) {
        ptr_meas = &(get_user_data<snemo::datamodel::tof_measurement>());
      } else if (is_user_data<snemo::datamodel::base_topology_measurement>()) {
        const snemo::datamodel::base_topology_measurement & btm
          = get_user_data<snemo::datamodel::base_topology_measurement>();
        ptr_meas = dynamic_cast<const snemo::datamodel::tof_measurement *>(&btm);
      } else {
        DT_THROW_IF(true, std::logic_error, "Invalid data type !");
      }
      const snemo::datamodel::tof_measurement & a_tof_meas = *ptr_meas;

      // Check if measurement has internal probability
      bool check_has_internal_probability = true;
      if (is_mode_has_internal_probability()) {
        if (! a_tof_meas.has_internal_probabilities()) {
          check_has_internal_probability = false;
        }
      }

      // Check if measurement has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (! a_tof_meas.has_internal_probabilities()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::tof_measurement::probability_type & pints
          = a_tof_meas.get_internal_probabilities();
        for (snemo::datamodel::tof_measurement::probability_type::const_iterator
               ip = pints.begin(); ip != pints.end(); ++ip) {
          const double pint = *ip;
          if (datatools::is_valid(_int_prob_range_min_)) {
            if (pint < _int_prob_range_min_) {
              DT_LOG_DEBUG(get_logging_priority(),
                           "Internal probability (" << pint/CLHEP::perCent << "%) lower than "
                           << _int_prob_range_min_/CLHEP::perCent << "%");
              check_range_internal_probability = false;
              if (_int_prob_range_mode_ == MODE_RANGE_STRICT) {
                break;
              }
            }
          }
        }
        for (snemo::datamodel::tof_measurement::probability_type::const_iterator
               ip = pints.begin(); ip != pints.end(); ++ip) {
          const double pint = *ip;
          if (datatools::is_valid(_int_prob_range_max_)) {
            if (pint > _int_prob_range_max_) {
              DT_LOG_DEBUG(get_logging_priority(),
                           "Internal probability (" << pint/CLHEP::perCent << "%) greater than "
                           << _int_prob_range_max_/CLHEP::perCent << "%");
              check_range_internal_probability = false;
              if (_int_prob_range_mode_ == MODE_RANGE_STRICT) {
                break;
              }
            }
          }
        }
      } // end of is_mode_range_internal_probability

      // Check if event has external probability
      bool check_has_external_probability = true;
      if (is_mode_has_external_probability()) {
        if (! a_tof_meas.has_external_probabilities()) {
          check_has_external_probability = false;
        }
      }

      // Check if measurement has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (! a_tof_meas.has_external_probabilities()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::tof_measurement::probability_type & pexts
          = a_tof_meas.get_external_probabilities();
        for (snemo::datamodel::tof_measurement::probability_type::const_iterator
               ip = pexts.begin(); ip != pexts.end(); ++ip) {
          const double pext = *ip;
          if (datatools::is_valid(_ext_prob_range_min_)) {
            if (pext < _ext_prob_range_min_) {
              DT_LOG_DEBUG(get_logging_priority(),
                           "External probability (" << pext/CLHEP::perCent << "%) lower than "
                           << _ext_prob_range_min_/CLHEP::perCent << "%");
              check_range_external_probability = false;
              if (_ext_prob_range_mode_ == MODE_RANGE_STRICT) {
                break;
              }
            }
          }
        }
        for (snemo::datamodel::tof_measurement::probability_type::const_iterator
               ip = pexts.begin(); ip != pexts.end(); ++ip) {
          const double pext = *ip;
          if (datatools::is_valid(_ext_prob_range_max_)) {
            if (pext > _ext_prob_range_max_) {
              DT_LOG_DEBUG(get_logging_priority(),
                           "External probability (" << pext/CLHEP::perCent << "%) greater than "
                           << _ext_prob_range_max_/CLHEP::perCent << "%");
              check_range_external_probability = false;
              if (_ext_prob_range_mode_ == MODE_RANGE_STRICT) {
                break;
              }
            }
          }
        }
      } // end of is_mode_range_external_probability

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_internal_probability &&
          check_has_external_probability &&
          check_range_internal_probability &&
          check_range_external_probability
          ) {
        DT_LOG_DEBUG(get_logging_priority(), "Event accepted by TOF measurement cut!");
        cut_returned = cuts::SELECTION_ACCEPTED;
      }
      return cut_returned;
    }

  }  // end of namespace cut

}  // end of namespace snemo

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::cut::tof_measurement_cut, ocd_)
{
  ocd_.set_class_name("snemo::cut::tof_measurement_cut");
  ocd_.set_class_description("Cut based on criteria applied to a Time-Of-Flight measurement");
  ocd_.set_class_library("falaise");
  // ocd_.set_class_documentation("");

  cuts::i_cut::common_ocd(ocd_);

  {
    // Description of the 'mode.has_internal_probability' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.has_internal_probability")
      .set_terse_description("Mode requiring internal probability availability")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the requested internal probability mode:: \n"
                   "                                                   \n"
                   "  mode.has_internal_probability : boolean = true   \n"
                   "                                                   \n"
                   )
      ;
  }

  {
    // Description of the 'mode.has_external_probability' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.has_external_probability")
      .set_terse_description("Mode requiring external probability availability")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the requested external probability mode:: \n"
                   "                                                   \n"
                   "  mode.has_external_probability : boolean = true   \n"
                   "                                                   \n"
                   )
      ;
  }

  {
    // Description of the 'mode.range_internal_probability' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.range_internal_probability")
      .set_terse_description("Mode with a special requested ranged of internal probability value")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the mode::                                \n"
                   "                                                   \n"
                   "  mode.range_internal_probability : boolean = true \n"
                   "                                                   \n"
                   )
      ;
  }

  {
    // Description of the 'mode.range_external_probability' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.range_external_probability")
      .set_terse_description("Mode with a special requested ranged of external probability value")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the mode::                                \n"
                   "                                                   \n"
                   "  mode.range_external_probability : boolean = true \n"
                   "                                                   \n"
                   )
      ;
  }

  {
    // Description of the 'range_internal_probability.mode' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_internal_probability.mode")
      .set_terse_description("Mode defining how to select internal probability value")
      .set_traits(datatools::TYPE_STRING)
      .set_long_description("Supported values are:                                   \n"
                            "                                                        \n"
                            " * ``strict`` require at least one internal probability \n"
                            " * ``all``    require all internal probabilities        \n"
                            "                                                        \n"
                            )
      .add_example("Activate the ``strict`` mode::                          \n"
                   "                                                        \n"
                   "  range_internal_probability.mode : string = \"strict\" \n"
                   "                                                        \n"
                   )
      ;
  }

  {
    // Description of the 'range_external_probability.mode' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_external_probability.mode")
      .set_terse_description("Mode defining how to select external probability value")
      .set_traits(datatools::TYPE_STRING)
      .set_long_description("Supported values are:                                   \n"
                            "                                                        \n"
                            " * ``strict`` require at least one external probability \n"
                            " * ``all``    require all external probabilities        \n"
                            "                                                        \n"
                            )
      .add_example("Activate the ``strict`` mode::                          \n"
                   "                                                        \n"
                   "  range_external_probability.mode : string = \"strict\" \n"
                   "                                                        \n"
                   )
      ;
  }

  {
    // Description of the 'range_internal_probability.min' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_internal_probability.min")
      .set_terse_description("Minimal value of the requested ranged internal probability")
      .set_triggered_by_flag("mode.range_internal_probability")
      .set_traits(datatools::TYPE_REAL)
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("fraction")
      .set_unit_symbol("%")
      .add_example("Set a specific minimal value of the internal probability:: \n"
                   "                                                           \n"
                   "  range_internal_probability.min : real as fraction = 4 %  \n"
                   "                                                           \n"
                   )
      ;
  }

  {
    // Description of the 'range_internal_probability.max' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_internal_probability.max")
      .set_terse_description("Maximal value of the requested ranged internal probability")
      .set_triggered_by_flag("mode.range_internal_probability")
      .set_traits(datatools::TYPE_REAL)
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("fraction")
      .set_unit_symbol("%")
      .add_example("Set a specific maximal value of the internal probability:: \n"
                   "                                                           \n"
                   "  range_internal_probability.max : real as fraction = 4 %  \n"
                   "                                                           \n"
                   )
      ;
  }

  {
    // Description of the 'range_external_probability.min' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_external_probability.min")
      .set_terse_description("Minimal value of the requested ranged external probability")
      .set_triggered_by_flag("mode.range_external_probability")
      .set_traits(datatools::TYPE_REAL)
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("fraction")
      .set_unit_symbol("%")
      .add_example("Set a specific minimal value of the external probability:: \n"
                   "                                                           \n"
                   "  range_external_probability.min : real as fraction = 4 %  \n"
                   "                                                           \n"
                   )
      ;
  }


  {
    // Description of the 'range_external_probability.max' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_external_probability.max")
      .set_terse_description("Maximal value of the requested ranged external probability")
      .set_triggered_by_flag("mode.range_external_probability")
      .set_traits(datatools::TYPE_REAL)
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("fraction")
      .set_unit_symbol("%")
      .add_example("Set a specific maximal value of the external probability:: \n"
                   "                                                           \n"
                   "  range_external_probability.max : real as fraction = 4 %  \n"
                   "                                                           \n"
                   )
      ;
  }

  // Additional configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the                  \n"
                               "``datatools::properties`` ASCII format::                     \n"
                               "                                                             \n"
                               "   mode.has_internal_probability : boolean = true            \n"
                               "   mode.range_internal_probability : boolean = true          \n"
                               "   range_internal_probability.mode : string = \"all\"        \n"
                               "   range_internal_probability.min : real as fraction = 5 %   \n"
                               "   range_internal_probability.max : real as fraction = 100 % \n"
                               "                                                             \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation

// Registration macro for class 'snemo::cut::tof_measurement_cut' :
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::cut::tof_measurement_cut, "snemo::cut::tof_measurement_cut")

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
