/// \file falaise/snemo/reconstruction/energy_driver.cc

// Ourselves:
#include <snemo/reconstruction/energy_driver.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// This project:
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track.h>

namespace snemo {

  namespace reconstruction {

    const std::string & energy_driver::get_id()
    {
      static const std::string _id("ED");
      return _id;
    }

    bool energy_driver::is_initialized() const
    {
      return _initialized_;
    }

    void energy_driver::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    void energy_driver::set_logging_priority(const datatools::logger::priority priority_)
    {
      _logging_priority_ = priority_;
      return;
    }

    datatools::logger::priority energy_driver::get_logging_priority() const
    {
      return _logging_priority_;
    }

    // Constructor
    energy_driver::energy_driver()
    {
      _set_defaults();
      return;
    }

    // Destructor
    energy_driver::~energy_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void energy_driver::_set_defaults()
    {

      _initialized_ = false;
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      return;
    }

    // Initialization :
    void energy_driver::initialize(const datatools::properties  & setup_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Driver '" << get_id() << "' is already initialized !");

      // Logging priority
      datatools::logger::priority lp = datatools::logger::extract_logging_configuration(setup_);
      DT_THROW_IF(lp == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority level !");
      set_logging_priority(lp);

      _set_initialized(true);
      return;
    }

    void energy_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

    void energy_driver::process(const snemo::datamodel::particle_track & pt_,
                                double & energy_)
    {
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << get_id() << "' is already initialized !");
      this->_process_algo(pt_,energy_);
      return;
    }

    void energy_driver::_process_algo(const snemo::datamodel::particle_track & pt_,
                                      double & energy_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      // Invalidate result
      datatools::invalidate(energy_);

      if (pt_.has_associated_calorimeter_hits()) {
        const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
          the_calos = pt_.get_associated_calorimeter_hits();
        for (snemo::datamodel::calibrated_calorimeter_hit::collection_type::const_iterator
               icalo = the_calos.begin(); icalo != the_calos.end(); ++icalo) {
          const snemo::datamodel::calibrated_calorimeter_hit & a_calo = icalo->get();
          icalo == the_calos.begin() ? energy_ = a_calo.get_energy() : energy_ += a_calo.get_energy();
        }
      } else {
        DT_LOG_DEBUG(get_logging_priority(), "Particle track is not associated to any calorimeter block !");
      }

      DT_LOG_DEBUG(get_logging_priority(), "Particle energy = " << energy_/CLHEP::MeV << " MeV");
      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return;
    }

    // static
    void energy_driver::init_ocd(datatools::object_configuration_description & ocd_)
    {
      // Prefix "ED" stands for "Energy Driver" :
      datatools::logger::declare_ocd_logging_configuration(ocd_, "fatal", "ED.");
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::energy_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::energy_driver");
  ocd_.set_class_description("A driver class for the Energy Measurement algorithm");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("The driver determines the energy deposited inside calorimeter.");
  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::energy_driver,
                               "snemo::reconstruction::energy_driver")
