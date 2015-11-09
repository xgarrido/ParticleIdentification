// test_tof_measurement_cut.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/cuts/tof_measurement_cut.h>

int main()
{
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'tof_measurement_cut' class." << std::endl;

    // Fake TOF measurement
    snemo::datamodel::tof_measurement TM;
    snemo::datamodel::tof_measurement::probability_type & int_probs
      = TM.grab_internal_probabilities();
    int_probs.push_back(10 * CLHEP::perCent);
    int_probs.push_back(30 * CLHEP::perCent);
    int_probs.push_back(40 * CLHEP::perCent);
    snemo::datamodel::tof_measurement::probability_type & ext_probs
      = TM.grab_external_probabilities();
    ext_probs.push_back(1e-1 * CLHEP::perCent);
    ext_probs.push_back(1e-4 * CLHEP::perCent);
    ext_probs.push_back(1e-5 * CLHEP::perCent);
    TM.tree_dump(std::cout, "TOF measurement dump:", "[notice]: ");

    {
      snemo::cut::tof_measurement_cut TMC;
      datatools::properties TMC_config;
      TMC_config.store("logging.priority", "debug");
      TMC_config.store("mode.has_internal_probability", true);
      TMC_config.store("mode.has_external_probability", true);
      TMC.initialize_standalone(TMC_config);
      TMC.set_user_data(TM);
      const int status = TMC.process();
      std::cout << "Current TOF measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "no ";
      }
      std::cout << "internal/external probabilities" << std::endl;
    }
    {
      snemo::cut::tof_measurement_cut TMC;
      datatools::properties TMC_config;
      TMC_config.store("logging.priority", "debug");
      TMC_config.store("mode.has_internal_probability", true);
      TMC_config.store("mode.range_internal_probability", true);
      TMC_config.store("range_internal_probability.mode", "strict");
      TMC_config.store_real_with_explicit_unit("range_internal_probability.min", 10 * CLHEP::perCent);
      TMC.initialize_standalone(TMC_config);
      TMC.set_user_data(TM);
      const int status = TMC.process();
      std::cout << "Current TOF measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "no ";
      } else {
        std::cout << "at least one ";
      }
      std::cout << "internal probability greater than 10%" << std::endl;
    }
    {
      snemo::cut::tof_measurement_cut TMC;
      datatools::properties TMC_config;
      TMC_config.store("logging.priority", "debug");
      TMC_config.store("mode.has_internal_probability", true);
 TMC_config.store("mode.range_internal_probability", true);
      TMC_config.store("range_internal_probability.mode", "all");
      TMC_config.store_real_with_explicit_unit("range_internal_probability.min", 5 * CLHEP::perCent);
      TMC_config.store_real_with_explicit_unit("range_internal_probability.max", 50 * CLHEP::perCent);
      TMC.initialize_standalone(TMC_config);
      TMC.set_user_data(TM);
      const int status = TMC.process();
      std::cout << "Current TOF measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "not ";
      }
      std::cout << "all internal probability between 5% and 50%" << std::endl;
    }
    {
      snemo::cut::tof_measurement_cut TMC;
      datatools::properties TMC_config;
      TMC_config.store("logging.priority", "debug");
      TMC_config.store("mode.has_internal_probability", true);
      TMC_config.store("mode.range_internal_probability", true);
      TMC_config.store("range_internal_probability.mode", "all");
      TMC_config.store_real_with_explicit_unit("range_internal_probability.min", 5 * CLHEP::perCent);
      TMC_config.store_real_with_explicit_unit("range_internal_probability.max", 20 * CLHEP::perCent);
      TMC.initialize_standalone(TMC_config);
      TMC.set_user_data(TM);
      const int status = TMC.process();
      std::cout << "Current TOF measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "not ";
      }
      std::cout << "all internal probability between 5% and 20%" << std::endl;
    }
    {
      snemo::cut::tof_measurement_cut TMC;
      datatools::properties TMC_config;
      TMC_config.store("logging.priority", "debug");
      TMC_config.store("mode.has_external_probability", true);
      TMC_config.store("mode.range_external_probability", true);
      TMC_config.store("range_external_probability.mode", "all");
      TMC_config.store_real_with_explicit_unit("range_external_probability.max", 5 * CLHEP::perCent);
      TMC.initialize_standalone(TMC_config);
      TMC.set_user_data(TM);
      const int status = TMC.process();
      std::cout << "Current TOF measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "not ";
      }
      std::cout << "all external probability below 5%" << std::endl;
    }

  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
