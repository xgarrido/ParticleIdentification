/** \file falaise/snemo/datamodels/topology_2eNg_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_2eNg_builder.h>
#include <falaise/snemo/datamodels/topology_2eNg_pattern.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_2eNg_builder,
                                                                    "snemo::reconstruction::topology_2eNg_builder");

    snemo::datamodel::base_topology_pattern::handle_type topology_2eNg_builder::create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_2eNg_pattern);
      return h;
    }

    void topology_2eNg_builder::build_measurement_dictionary(const snemo::datamodel::particle_track_data & ptd_,snemo::datamodel::base_topology_pattern & pattern_) {

      snemo::datamodel::base_topology_pattern::measurement_dict_type & meas_ = pattern_.grab_measurement_dictionary();

      snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & particle_tracks_dict = pattern_.grab_particle_tracks_dictionary();

      snemo::reconstruction::topology_2e_builder::build_measurement_dictionary(ptd_,pattern_);

      const snemo::datamodel::particle_track & e1 = particle_tracks_dict["e1"].get();
      const snemo::datamodel::particle_track & e2 = particle_tracks_dict["e2"].get();

      // const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
      //   = ptd_.get_particles();
      //
      // Either this way
      // for (size_t i_gamma = 1; i_gamma <= h_pattern_->get_number_of_gammas();++i_gamma) {
      // Or this way but less evolutive
      for (size_t i_gamma = 1; i_gamma <= particle_tracks_dict.size()-1;++i_gamma) {
        std::ostringstream key;
        key << "g" << i_gamma;
        const snemo::datamodel::particle_track & gamma = particle_tracks_dict_[key];

        {
          snemo::datamodel::tof_measurement * ptr_tof = new snemo::datamodel::tof_measurement;
          std::ostringstream key_meas;
          key_meas << "tof_e1_" << key;
          meas_dict[key_meas].reset(ptr_tof);
          if (get_measurement_drivers().TOFD) get_measurement_drivers().TOFD->process(e1, gamma,
                                      ptr_tof->grab_internal_probabilities(),
                                      ptr_tof->grab_external_probabilities());
        }

        {
          snemo::datamodel::tof_measurement * ptr_tof = new snemo::datamodel::tof_measurement;
          std::ostringstream key_meas;
          key_meas << "tof_e2_" << key;
          meas_dict[key_meas].reset(ptr_tof);
          if (get_measurement_drivers().TOFD) get_measurement_drivers().TOFD->process(e2, gamma,
                                      ptr_tof->grab_internal_probabilities(),
                                      ptr_tof->grab_external_probabilities());
        }

        {
          snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
          std::ostringstream key_meas;
          key_meas << "angle_e1_" << key;
          meas_dict[key_meas].reset(ptr_angle);
          if (get_measurement_drivers().AMD) get_measurement_drivers().AMD->process(e1, gamma,
                                    ptr_angle->grab_angle());
        }


        {
          snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
          std::ostringstream key_meas;
          key_meas << "angle_e2_" << key;
          meas_dict[key_meas].reset(ptr_angle);
          if (get_measurement_drivers().AMD) get_measurement_drivers().AMD->process(e2, gamma,
                                    ptr_angle->grab_angle());
        }

        {
          snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
          std::ostringstream key_meas;
          key_meas << "energy_" << key;
          meas_dict["energy_"].reset(ptr_energy);
          if (get_measurement_drivers().EMD) get_measurement_drivers().EMD->process(gamma, ptr_energy->grab_energy());
        }
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
