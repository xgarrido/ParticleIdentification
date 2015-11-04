// test_delta_vertices_driver.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// This project:
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/reconstruction/delta_vertices_driver.h>

int main()
{
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'delta_vertices_driver' class." << std::endl;

    snemo::reconstruction::delta_vertices_driver DVD;
    datatools::properties DVD_config;
    DVD_config.store("logging.priority", "debug");
    DVD.initialize(DVD_config);

    // Fake electron tracks :
    {
      snemo::datamodel::particle_track electron;
      electron.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                         snemo::datamodel::pid_utils::electron_label());
      snemo::datamodel::particle_track electron_2;
      electron_2.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                         snemo::datamodel::pid_utils::electron_label());
      // Add a source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = electron.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_blur_dimension(geomtools::blur_spot::dimension_three);
        a_vertex.set_position(geomtools::vector_3d(0, 0, 0));
        a_vertex.set_errors(0.1 * CLHEP::mm, 2 * CLHEP::mm, 7 * CLHEP::mm);
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_source_foil_label());
      }

      // Add a source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = electron_2.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_blur_dimension(geomtools::blur_spot::dimension_three);
        a_vertex.set_position(geomtools::vector_3d(0, 2*CLHEP::mm, 7*CLHEP::mm));
        a_vertex.set_errors(0.1 * CLHEP::mm, 2 * CLHEP::mm, 7 * CLHEP::mm);
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_source_foil_label());
      }

      electron.tree_dump();
      electron_2.tree_dump();
      geomtools::blur_spot vertices_barycenter;
      vertices_barycenter.invalidate();
      DVD.process(electron, electron_2, vertices_barycenter);
      double vertices_probability;
      vertices_probability = vertices_barycenter.get_auxiliaries().fetch_real("Probability");
      std::clog << "Vertices probability = " << vertices_probability << std::endl;
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
