/** \file falaise/snemo/datamodels/base_topology_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/base_topology_builder.h>

// - Falaise:
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/pid_utils.h>

namespace snemo {

  namespace reconstruction {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_topology_builder,
                                                     "snemo::reconstruction::base_topology_builder/__system__")

    bool base_topology_builder::has_measurement_drivers() const
    {
      return _drivers != 0;
    }

    void base_topology_builder::set_measurement_drivers(const measurement_drivers & drivers_)
    {
      _drivers = &drivers_;
      return;
    }

    const measurement_drivers & base_topology_builder::get_measurement_drivers() const
    {
      return *_drivers;
    }

    base_topology_builder::base_topology_builder()
    {
      _drivers = 0;
      return;
    }

    base_topology_builder::~base_topology_builder()
    {
      return;
    }

    snemo::datamodel::base_topology_pattern::handle_type base_topology_builder::create_pattern()
    {
      return _create_pattern();
    }

    void base_topology_builder::build(const snemo::datamodel::particle_track_data & source_,
                                      snemo::datamodel::base_topology_pattern & pattern_)
    {
      DT_THROW_IF(! has_measurement_drivers(), std::logic_error, "Missing measurement drivers !");
      this->_build_particle_tracks_dictionary(source_, pattern_.grab_particle_track_dictionary());
      _build_measurement_dictionary(pattern_);
    }

    void base_topology_builder::_build_particle_tracks_dictionary(const snemo::datamodel::particle_track_data & ptd_,
                                                                  snemo::datamodel::base_topology_pattern::particle_track_dict_type & tracks_)
    {
      size_t n_electrons = 0;
      size_t n_positrons = 0;
      size_t n_alphas    = 0;
      size_t n_gammas    = 0;
      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();
      for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
             i_particle = the_particles.begin();
           i_particle != the_particles.end(); ++i_particle) {
        const snemo::datamodel::particle_track & a_particle = i_particle->get();
        std::ostringstream key;
        if (snemo::datamodel::pid_utils::particle_is_electron(a_particle)) {
          key << "e" << ++n_electrons;
        }
        else if (snemo::datamodel::pid_utils::particle_is_positron(a_particle)) {
          key << "p" << ++n_positrons;
        }
        else if (snemo::datamodel::pid_utils::particle_is_alpha(a_particle)) {
          key << "a" << ++n_alphas;
        }
        else if (snemo::datamodel::pid_utils::particle_is_gamma(a_particle)) {
          key << "g" << ++n_gammas;
        }
        else {
          continue; // no undefined particles for now
        }
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
