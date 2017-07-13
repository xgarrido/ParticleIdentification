// test_angle_measurement_cut.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/cuts/angle_measurement_cut.h>

int main()
{
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'angle_measurement_cut' class." << std::endl;

    // Fake angle measurement
    snemo::datamodel::angle_measurement AM;
    AM.set_angle(66*CLHEP::degree);
    AM.tree_dump(std::cout, "Angle measurement dump:", "[notice]: ");

    {
      snemo::cut::angle_measurement_cut AMC;
      datatools::properties AMC_config;
      AMC_config.store("logging.priority", "debug");
      AMC_config.store("mode.has_angle", true);
      AMC.initialize_standalone(AMC_config);
      AMC.set_user_data(AM);
      const int status = AMC.process();
      std::cout << "Current angle measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "no ";
      }
      std::cout << "angle" << std::endl;
    }
    {
      snemo::cut::angle_measurement_cut AMC;
      datatools::properties AMC_config;
      AMC_config.store("logging.priority", "debug");
      AMC_config.store("mode.range_angle", true);
      AMC_config.store_real_with_explicit_unit("range_angle.min", 0 * CLHEP::degree);
      AMC.initialize_standalone(AMC_config);
      AMC.set_user_data(AM);
      const int status = AMC.process();
      std::cout << "Current angle measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "no ";
      }
      std::cout << "angle greater than 0°" << std::endl;
    }
    {
      snemo::cut::angle_measurement_cut AMC;
      datatools::properties AMC_config;
      AMC_config.store("logging.priority", "debug");
      AMC_config.store("mode.range_angle", true);
      AMC_config.store_real_with_explicit_unit("range_angle.max", 60 * CLHEP::degree);
      AMC.initialize_standalone(AMC_config);
      AMC.set_user_data(AM);
      const int status = AMC.process();
      std::cout << "Current angle measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "no ";
      }
      std::cout << "angle smaller than 60°" << std::endl;
    }
    {
      snemo::cut::angle_measurement_cut AMC;
      datatools::properties AMC_config;
      AMC_config.store("logging.priority", "debug");
      AMC_config.store("mode.range_angle", true);
      AMC_config.store_real_with_explicit_unit("range_angle.min", 0 * CLHEP::degree);
      AMC_config.store_real_with_explicit_unit("range_angle.max", 50 * CLHEP::degree);
      AMC.initialize_standalone(AMC_config);
      AMC.set_user_data(AM);
      const int status = AMC.process();
      std::cout << "Current angle measurement has ";
      if (status != cuts::SELECTION_ACCEPTED) {
        std::cout << "no ";
      }
      std::cout << "angle between 0 and 50°" << std::endl;
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
// test_angle_measurement_cut.cxx
