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
#include <falaise/snemo/datamodels/tof_measurement.h>
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
      snemo::datamodel::particle_track electron1;
      electron1.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                          snemo::datamodel::pid_utils::electron_label());
      snemo::datamodel::particle_track electron2;
      electron2.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                          snemo::datamodel::pid_utils::electron_label());
      snemo::datamodel::particle_track gamma;
      gamma.grab_auxiliaries().update(snemo::datamodel::pid_utils::pid_label_key(),
                                      snemo::datamodel::pid_utils::gamma_label());

      // Add electron1 source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = electron1.grab_vertices();
        the_vertices.push_back(new geomtools::blur_spot);
        geomtools::blur_spot & a_vertex = the_vertices.back().grab();
        a_vertex.set_position(geomtools::vector_3d(0, 0, 0));
        a_vertex.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                           snemo::datamodel::particle_track::vertex_on_source_foil_label());
      }
      // Add electron2 source foil vertex
      {
        snemo::datamodel::particle_track::vertex_collection_type & the_vertices
          = electron2.grab_vertices();
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

      // Add electron1 fake trajectory
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
        electron1.set_trajectory_handle(a_trajectory);
      }

      // Add electron2 fake trajectory
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
        electron2.set_trajectory_handle(a_trajectory);
      }

    //Push some fake electron1 calorimeter hit
    {
      snemo::datamodel::calibrated_calorimeter_hit::collection_type & the_calos
        = electron1.grab_associated_calorimeter_hits();
      the_calos.push_back(new snemo::datamodel::calibrated_calorimeter_hit);
      snemo::datamodel::calibrated_calorimeter_hit & a_calo = the_calos.back().grab();
      a_calo.set_energy(1000 * CLHEP::keV);
      a_calo.set_sigma_energy(80 * CLHEP::keV);
      a_calo.set_time(1.6 * CLHEP::ns);
      a_calo.set_sigma_time(0.05 * CLHEP::ns);
    }
    //Push some fake electron2 calorimeter hit
    {
      snemo::datamodel::calibrated_calorimeter_hit::collection_type & the_calos
        = electron2.grab_associated_calorimeter_hits();
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

    electron1.tree_dump();
    electron2.tree_dump();
    gamma.tree_dump();

    snemo::datamodel::tof_measurement tof_e1_e2;
    TOFD.process(electron1, electron2, tof_e1_e2);
    snemo::datamodel::tof_measurement tof_e1_g1;
    TOFD.process(electron1, gamma, tof_e1_g1);

    std::clog << "TOF measurement for e1/e2" << std::endl;
    tof_e1_e2.tree_dump();
    std::clog << "TOF measurement for e1/g1" << std::endl;
    tof_e1_g1.tree_dump();
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
