// falaise/snemo/cuts/energy_measurement_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/energy_measurement_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>
#include <datatools/clhep_units.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/energy_measurement.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(energy_measurement_cut, "snemo::cut::energy_measurement_cut")

    void energy_measurement_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _energy_range_.invalidate();
      return;
    }

    uint32_t energy_measurement_cut::get_mode() const
    {
      return _mode_;
    }

    bool energy_measurement_cut::is_mode_has_energy() const
    {
      return _mode_ & MODE_HAS_ENERGY;
    }

    bool energy_measurement_cut::is_mode_range_energy() const
    {
      return _mode_ & MODE_RANGE_ENERGY;
    }

    energy_measurement_cut::energy_measurement_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      this->register_supported_user_data_type<snemo::datamodel::base_topology_measurement>();
      this->register_supported_user_data_type<snemo::datamodel::energy_measurement>();
      return;
    }

    energy_measurement_cut::~energy_measurement_cut()
    {
      if (is_initialized()) this->energy_measurement_cut::reset();
      return;
    }

    void energy_measurement_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void energy_measurement_cut::initialize(const datatools::properties & configuration_,
                                           datatools::service_manager  & /* service_manager_ */,
                                           cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_flag("mode.has_energy")) {
        _mode_ |= MODE_HAS_ENERGY;
      }
      if (configuration_.has_flag("mode.range_energy")) {
        _mode_ |= MODE_RANGE_ENERGY;
      }
      DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                  "Missing at least a 'mode.XXX' property !");

      // mode RANGE_ENERGY:
      if (is_mode_range_energy()) {
        datatools::real_range energy_limits;
        energy_limits.make_positive_unbounded();
        // Extract the energy bound
        auto get_range_energy = [&configuration_, &energy_limits](const std::string& key) {
          double value {datatools::invalid_real()};
          if (configuration_.has_key(key)) {
            value = configuration_.fetch_real(key);
            if (!configuration_.has_explicit_unit(key)) {
              value *= CLHEP::keV;
            }
            DT_THROW_IF(! energy_limits.has(value),
                        std::range_error,
                        "Invalid energy value (" << value << ") !");
          }
          return value;
        };

        const double emin {get_range_energy("range_energy.min")};
        const double emax {get_range_energy("range_energy.max")};
        DT_THROW_IF(!datatools::is_valid(emin) && !datatools::is_valid(emax),
                    std::logic_error,
                    "Missing 'range_energy.min' or 'range_energy.max' property !");
        if (datatools::is_valid(emin) && datatools::is_valid(emax)) {
          _energy_range_.make_bounded(emin, emax);
        } else if (datatools::is_valid(emin)) {
          _energy_range_.make_bounded(emin, energy_limits.get_upper());
        } else if (datatools::is_valid(emax)) {
          _energy_range_.make_bounded(energy_limits.get_lower(), emax);
        }
      } // end if is_mode_range_energy
      this->i_cut::_set_initialized(true);
      return;
    }


    int energy_measurement_cut::_accept()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get energy measurement
      const snemo::datamodel::energy_measurement * ptr_meas = 0;
      if (is_user_data<snemo::datamodel::energy_measurement>()) {
        ptr_meas = &(get_user_data<snemo::datamodel::energy_measurement>());
      } else if (is_user_data<snemo::datamodel::base_topology_measurement>()) {
        const snemo::datamodel::base_topology_measurement & btm
          = get_user_data<snemo::datamodel::base_topology_measurement>();
        ptr_meas = dynamic_cast<const snemo::datamodel::energy_measurement *>(&btm);
      } else {
        DT_THROW_IF(true, std::logic_error, "Invalid data type !");
      }
      const snemo::datamodel::energy_measurement & a_energy_meas = *ptr_meas;

      // Check if measurement has energy
      bool check_has_energy = true;
      if (is_mode_has_energy()) {
        if (! a_energy_meas.has_energy()) {
          check_has_energy = false;
        }
      }

      // Check if measurement has correct energy
      bool check_range_energy = true;
      if (is_mode_range_energy()) {
        if (! a_energy_meas.has_energy()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing energy !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double energy = a_energy_meas.get_energy();
        check_range_energy = _energy_range_.has(energy);
      } // end of is_mode_range_energy

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_energy &&
          check_range_energy) {
        DT_LOG_DEBUG(get_logging_priority(), "Event accepted by energy measurement cut!");
        cut_returned = cuts::SELECTION_ACCEPTED;
      }
      return cut_returned;
    }

  }  // end of namespace cut

}  // end of namespace snemo

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::cut::energy_measurement_cut, ocd_)
{
  ocd_.set_class_name("snemo::cut::energy_measurement_cut");
  ocd_.set_class_description("Cut based on criteria applied to a energy measurement");
  ocd_.set_class_library("falaise");
  // ocd_.set_class_documentation("");

  cuts::i_cut::common_ocd(ocd_);

  {
    // Description of the 'mode.has_energy' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.has_energy")
      .set_terse_description("Mode requiring energy availability")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the requested energy mode:: \n"
                   "                                     \n"
                   "  mode.has_energy : boolean = true   \n"
                   "                                     \n"
                   )
      ;
  }

  {
    // Description of the 'mode.range_nagle' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.range_energy")
      .set_terse_description("Mode with a special requested ranged of energy value")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the mode::                  \n"
                   "                                     \n"
                   "  mode.range_energy : boolean = true \n"
                   "                                     \n"
                   )
      ;
  }

  {
    // Description of the 'range_energy.min' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_energy.min")
      .set_terse_description("Minimal value of the requested ranged energy")
      .set_triggered_by_flag("mode.range_energy")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("energy")
      .set_unit_symbol("keV")
      .add_example("Set a specific minimal value of the energy:: \n"
                   "                                             \n"
                   "  range_energy.min : real as energy = 50 keV \n"
                   "                                             \n"
                   )
      ;
  }

  {
    // Description of the 'range_energy.max' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_energy.max")
      .set_terse_description("Maximal value of the requested ranged energy")
      .set_triggered_by_flag("mode.range_energy")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("energy")
      .set_unit_symbol("keV")
      .add_example("Set a specific maximal value of the energy::   \n"
                   "                                               \n"
                   "  range_energy.max : real as energy = 1000 keV \n"
                   "                                               \n"
                   )
      ;
  }

  // Additional configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the     \n"
                               "``datatools::properties`` ASCII format::        \n"
                               "                                                \n"
                               "   mode.has_energy : boolean = true             \n"
                               "   mode.range_energy : boolean = true           \n"
                               "   range_energy.min : real as energy = 50 keV   \n"
                               "   range_energy.max : real as energy = 1000 keV \n"
                               "                                                \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation

// Registration macro for class 'snemo::cut::simulated_data_cut' :
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::cut::energy_measurement_cut, "snemo::cut::energy_measurement_cut")

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
