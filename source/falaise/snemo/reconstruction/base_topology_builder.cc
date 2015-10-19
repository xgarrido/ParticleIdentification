/** \file falaise/snemo/datamodels/base_topology_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/base_topology_builder.h>

namespace snemo {

  namespace reconstruction {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_topology_builder,
                                                     "snemo::reconstruction::base_topology_builder/__system__");

    const measurement_drivers & base_topology_builder::get_measurement_drivers() const {
      return *_drivers_;
    }

    void base_topology_builder::build(const snemo::datamodel::particle_track_data & source_,
                                      snemo::datamodel::base_topology_pattern & target_) {
      _build_particle_tracks_dictionary(source_, target_.grab_particle_tracks_dictionary());

      build_measurement_dictionary(source_, target_.grab_measurement_dictionary());
    }

    void base_topology_builder::_build_particle_tracks_dictionary(const snemo::datamodel::particle_track_data & ptd_,
                                                                  snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & tracks_) {

      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();

      size_t n_electrons = 0;
      size_t n_positrons = 0;
      size_t n_alphas = 0;
      size_t n_gammas = 0;

      // for (auto i_particle = the_particles) {
      for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
             i_particle = the_particles.begin();
           i_particle != the_particles.end(); ++i_particle) {
        std::ostringstream key;
        if(snemo::datamodel::pid_utils::particle_is_electron(i_particle->get())) {
          n_electrons++;
          key << "e" << n_electrons;
        }
        else if(snemo::datamodel::pid_utils::particle_is_positron(i_particle->get())) {
          n_positrons++;
          key << "p" << n_positrons;
        }
        else if(snemo::datamodel::pid_utils::particle_is_alpha(i_particle->get())) {
          n_alphas++;
          key << "a" << n_alphas;
        }
        else if(snemo::datamodel::pid_utils::particle_is_gamma(i_particle->get())) {
          n_gammas++;
          key << "g" << n_gammas;
        }
        else
          continue; // no undefined particles for now

        tracks_[key.str()] = *i_particle;
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
