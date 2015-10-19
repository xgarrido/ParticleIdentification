/** \file falaise/snemo/datamodels/topology_2e_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_2e_builder.h>
#include <falaise/snemo/datamodels/topology_2e_pattern.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_2e_builder,
                                                                    "snemo::reconstruction::topology_2e_builder");

    snemo::datamodel::base_topology_pattern::handle_type topology_2e_builder::create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_2e_pattern);
      return h;
    }

    void topology_2e_builder::build_measurement_dictionary(const snemo::datamodel::particle_track_data & ptd_, snemo::datamodel::base_topology_pattern & pattern_) {

      snemo::datamodel::base_topology_pattern::measurement_dict_type & meas_ = pattern_.grab_measurement_dictionary();

      snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & particle_tracks_dict = pattern_.grab_particle_tracks_dictionary();

      const snemo::datamodel::particle_track & e1 = particle_tracks_dict["e1"].get();
      const snemo::datamodel::particle_track & e2 = particle_tracks_dict["e2"].get();

      {
        snemo::datamodel::TOF_measurement * ptr_tof = new snemo::datamodel::TOF_measurement;

        meas_["tof_e1_e2"].reset(ptr_tof);
        if (get_measurement_drivers().TOFD) get_measurement_drivers().TOFD->process(e1, e2,
                                                                                    ptr_tof->grab_internal_probabilities(),
                                                                                    ptr_tof->grab_external_probabilities());
      }

      {
        snemo::datamodel::delta_vertices_measurement * ptr_delta_vertices_source = new snemo::datamodel::delta_vertices_measurement;
        meas_["delta_vertices_source_e1_e2"].reset(ptr_delta_vertices_source);
        if (get_measurement_drivers().DVD) get_measurement_drivers().DVD->process(e1, e2,
                                                                                  ptr_delta_vertices_source->grab_delta_vertices_y(),
                                                                                  ptr_delta_vertices_source->grab_delta_vertices_z());
      }

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas_["angle_e1_e2"].reset(ptr_angle);
        if (get_measurement_drivers().AMD) get_measurement_drivers().AMD->process(e1, e2, ptr_angle->grab_angle());
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas_["energy_e1"].reset(ptr_energy);
        if (get_measurement_drivers().EMD) get_measurement_drivers().EMD->process(e1, ptr_energy->grab_energy());
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas_["energy_e2"].reset(ptr_energy);
        if (get_measurement_drivers().EMD) get_measurement_drivers().EMD->process(e2, ptr_energy->grab_energy());
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
