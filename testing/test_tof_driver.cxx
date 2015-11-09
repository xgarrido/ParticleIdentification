// test_tof_driver.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// This project:
#include <falaise/snemo/datamodels/line_trajectory_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/reconstruction/tof_driver.h>

int main()
{
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'tof_driver' class." << std::endl;

    snemo::reconstruction::tof_driver TOFD;
    datatools::properties TOFD_config;
    TOFD_config.store("logging.priority", "debug");
    TOFD.initialize(TOFD_config);

    // Fake tracks track :
    {
      snemo::datamodel::particle_track electron;
      electron.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                         snemo::datamodel::pid_utils::electron_label());
      snemo::datamodel::particle_track electron_2;
      electron_2.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                         snemo::datamodel::pid_utils::electron_label());
      snemo::datamodel::particle_track gamma;
      gamma.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                      snemo::datamodel::pid_utils::gamma_label());

      // Add electron source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = electron.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_position(geomtools::vector_3d(0, 0, 0));
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_source_foil_label());
      }
      // Add electron_2 source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = electron_2.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_position(geomtools::vector_3d(0, 0, 0));
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_source_foil_label());
      }
      // Add gamma source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = gamma.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_position(geomtools::vector_3d(0, 0, 0));
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_source_foil_label());
      }
      // Add gamma calo vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = gamma.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_position(geomtools::vector_3d(45*CLHEP::cm, 45*CLHEP::cm, 0));
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_main_calorimeter_label());
        std::istringstream iss("[1302:0.1.4.6.*]");
        geomtools::geom_id a_gid;
        iss >> a_gid;
        a_vertex.set_geom_id(a_gid);
         }

      // Add electron fake trajectory
      {
        snemo::datamodel::line_trajectory_pattern * ltp
          = new snemo::datamodel::line_trajectory_pattern;
        geomtools::line_3d & l3d = ltp->grab_segment();
        l3d.set_first(geomtools::vector_3d(0, 0, 0));
        l3d.set_last(geomtools::vector_3d(0, 45*CLHEP::cm, 0));
        snemo::datamodel::tracker_trajectory::handle_pattern a_pattern;
        a_pattern.reset(ltp);
        snemo::datamodel::tracker_trajectory::handle_type a_trajectory;
        a_trajectory.reset(new snemo::datamodel::tracker_trajectory);
        a_trajectory.grab().set_pattern_handle(a_pattern);
        electron.set_trajectory_handle(a_trajectory);
      }

      // Add electron_2 fake trajectory
      {
        snemo::datamodel::line_trajectory_pattern * ltp
          = new snemo::datamodel::line_trajectory_pattern;
        geomtools::line_3d & l3d = ltp->grab_segment();
        l3d.set_first(geomtools::vector_3d(0, 0, 0));
        l3d.set_last(geomtools::vector_3d(0, -45*CLHEP::cm, 0));
        snemo::datamodel::tracker_trajectory::handle_pattern a_pattern;
        a_pattern.reset(ltp);
        snemo::datamodel::tracker_trajectory::handle_type a_trajectory;
        a_trajectory.reset(new snemo::datamodel::tracker_trajectory);
        a_trajectory.grab().set_pattern_handle(a_pattern);
        electron_2.set_trajectory_handle(a_trajectory);
      }

    //Push some fake electron calorimeter hit
    {
      snemo::datamodel::calibrated_calorimeter_hit::collection_type & the_calos
        = electron.grab_associated_calorimeter_hits();
      the_calos.push_back(new snemo::datamodel::calibrated_calorimeter_hit);
      snemo::datamodel::calibrated_calorimeter_hit & a_calo = the_calos.back().grab();
      a_calo.set_energy(1000 * CLHEP::keV);
      a_calo.set_sigma_energy(80 * CLHEP::keV);
      a_calo.set_time(1.6 * CLHEP::ns);
      a_calo.set_sigma_time(0.05 * CLHEP::ns);
    }
    //Push some fake electron_2 calorimeter hit
    {
      snemo::datamodel::calibrated_calorimeter_hit::collection_type & the_calos
        = electron_2.grab_associated_calorimeter_hits();
      the_calos.push_back(new snemo::datamodel::calibrated_calorimeter_hit);
      snemo::datamodel::calibrated_calorimeter_hit & a_calo = the_calos.back().grab();
      a_calo.set_energy(1000 * CLHEP::keV);
      a_calo.set_sigma_energy(80 * CLHEP::keV);
      a_calo.set_time(1.4 * CLHEP::ns);
      a_calo.set_sigma_time(0.05 * CLHEP::ns);
    }
    //Push some fake gamma calorimeter hit
    {
      snemo::datamodel::calibrated_calorimeter_hit::collection_type & the_calos
        = gamma.grab_associated_calorimeter_hits();
      the_calos.push_back(new snemo::datamodel::calibrated_calorimeter_hit);
      snemo::datamodel::calibrated_calorimeter_hit & a_calo = the_calos.back().grab();
      a_calo.set_energy(1000 * CLHEP::keV);
      a_calo.set_sigma_energy(80 * CLHEP::keV);
      a_calo.set_time(2 * CLHEP::ns);
      a_calo.set_sigma_time(0.05 * CLHEP::ns);
      std::istringstream iss("[1302:0.1.4.6.*]");
      geomtools::geom_id a_gid;
      iss >> a_gid;
      a_calo.set_geom_id(a_gid);
    }

    electron.tree_dump();
    electron_2.tree_dump();
    gamma.tree_dump();

    std::vector<double> proba_int_e1_e2 = {}; // ensures the invalidation, C++11 ?
    std::vector<double> proba_ext_e1_e2 = {};
    std::vector<double> proba_int_e1_g1 = {};
    std::vector<double> proba_ext_e1_g1 = {};
    TOFD.process(electron, electron_2, proba_int_e1_e2, proba_ext_e1_e2);
    TOFD.process(electron, gamma, proba_int_e1_g1, proba_ext_e1_g1);
    std::clog << "Internal probability e1 e2 = " << proba_int_e1_e2.front() << std::endl;
    std::clog << "External probability e1 e2 = " << proba_ext_e1_e2.front() << std::endl;
    std::clog << "Internal probability e1 g1 = " << proba_int_e1_g1.front() << std::endl;
    std::clog << "External probability e1 g1 = " << proba_ext_e1_g1.front() << std::endl;
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
