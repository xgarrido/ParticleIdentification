/** \file falaise/snemo/datamodels/topology_1e1a_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_1e1a_builder.h>
#include <falaise/snemo/reconstruction/topology_1e_builder.h>
#include <falaise/snemo/datamodels/topology_1e1a_pattern.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_1e1a_builder,
                                                                    "snemo::reconstruction::topology_1e1a_builder");

    snemo::datamodel::base_topology_pattern::handle_type topology_1e1a_builder::create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_1e1a_pattern);
      return h;
    }

    void topology_1e1a_builder::build_measurement_dictionary(const snemo::datamodel::particle_track_data & ptd_,snemo::datamodel::base_topology_pattern & pattern_) {

      snemo::datamodel::base_topology_pattern::measurement_dict_type & meas_ = pattern_.grab_measurement_dictionary();

      snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & particle_tracks_dict = pattern_.grab_particle_tracks_dictionary();
      // const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
      //   = ptd_.get_particles();

      // const snemo::datamodel::particle_track & e1 = the_particles.front().get();

      snemo::reconstruction::topology_1e_builder::build_measurement_dictionary(ptd_,pattern_);

      const snemo::datamodel::particle_track & e1 = particle_tracks_dict["e1"].get();
      const snemo::datamodel::particle_track & a1 = particle_tracks_dict["a1"].get();

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas_["angle_a1"].reset(ptr_angle);
        if (get_measurement_drivers().AMD) get_measurement_drivers().AMD->process(a1, ptr_angle->grab_angle());
      }

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas_["angle_e1_a1"].reset(ptr_angle);
        if (get_measurement_drivers().AMD) get_measurement_drivers().AMD->process(e1, a1, ptr_angle->grab_angle());
      }

      {
        snemo::datamodel::delta_vertices_measurement * ptr_delta_vertices_source = new snemo::datamodel::delta_vertices_measurement;
        meas_["delta_vertices_source_e1_a1"].reset(ptr_delta_vertices_source);
        if (get_measurement_drivers().DVD) get_measurement_drivers().DVD->process(e1, a1,
                                                                                  ptr_delta_vertices_source->grab_delta_vertices_y(),
                                                                                  ptr_delta_vertices_source->grab_delta_vertices_z());
      }

      return;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
