// test_angle_driver.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// This project:
#include <falaise/snemo/datamodels/line_trajectory_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/reconstruction/angle_driver.h>

int main()
{
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'angle_driver' class." << std::endl;

    snemo::reconstruction::angle_driver AD;
    datatools::properties AD_config;
    AD_config.store("logging.priority", "debug");
    AD.initialize(AD_config);

    // Fake electron track :
    {
      snemo::datamodel::particle_track electron;
      electron.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                         snemo::datamodel::pid_utils::electron_label());
      // Add a source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = electron.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_position(geomtools::vector_3d(0, 0, 0));
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_source_foil_label());

      }
      // Add also a fake trajectory
      {
        snemo::datamodel::line_trajectory_pattern * ltp
          = new snemo::datamodel::line_trajectory_pattern;
        geomtools::line_3d & l3d = ltp->grab_segment();
        l3d.set_first(geomtools::vector_3d(0, 0, 0));
        l3d.set_last(geomtools::vector_3d(0, 1, 0));
        snemo::datamodel::tracker_trajectory::handle_pattern a_pattern;
        a_pattern.reset(ltp);
        snemo::datamodel::tracker_trajectory::handle_type a_trajectory;
        a_trajectory.reset(new snemo::datamodel::tracker_trajectory);
        a_trajectory.grab().set_pattern_handle(a_pattern);
        electron.set_trajectory_handle(a_trajectory);
      }
      electron.tree_dump();
      snemo::datamodel::angle_measurement angle;
      AD.process(electron, angle);
      std::clog << "Electron angle : " << std::endl;
      angle.tree_dump();
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
