/** \file falaise/snemo/datamodels/topology_1e_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_1e_builder.h>
#include <falaise/snemo/datamodels/topology_1e_pattern.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_1e_builder,
                                                                    "snemo::reconstruction::topology_1e_builder");

    snemo::datamodel::base_topology_pattern::handle_type topology_1e_builder::create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_1e_pattern);
      return h;
    }

    void topology_1e_builder::build_measurement_dictionary(const snemo::datamodel::particle_track_data & ptd_,snemo::datamodel::base_topology_pattern & pattern_) {

      snemo::datamodel::base_topology_pattern::measurement_dict_type & meas_ = pattern_.grab_measurement_dictionary();

      snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & particle_tracks_dict = pattern_.grab_particle_tracks_dictionary();

      const snemo::datamodel::particle_track & e1 = particle_tracks_dict["e1"].get();

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas_["angle_e1"].reset(ptr_angle);
        if (get_measurement_drivers().AMD) get_measurement_drivers().AMD->process(e1, ptr_angle->grab_angle());
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas_["energy_e1"].reset(ptr_energy);
        if (get_measurement_drivers().EMD) get_measurement_drivers().EMD->process(e1, ptr_energy->grab_energy());
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
