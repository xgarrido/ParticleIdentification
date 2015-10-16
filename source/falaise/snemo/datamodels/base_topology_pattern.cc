/** \file falaise/snemo/datamodels/base_topology_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(base_topology_pattern,
                                                      "snemo::datamodel::base_topology_pattern")

    base_topology_pattern::base_topology_pattern()
    {
      return;
    }

    base_topology_pattern::~base_topology_pattern()
    {
      return;
    }

    snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & base_topology_pattern::grab_particle_tracks_dictionary()
    {
      return _tracks_;
    }

    const snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & base_topology_pattern::get_particle_tracks_dictionary() const
    {
      return _tracks_;
    }

    snemo::datamodel::base_topology_pattern::measurement_dict_type & base_topology_pattern::grab_measurement_dictionary()
    {
      return _meas_;
    }

    const snemo::datamodel::base_topology_pattern::measurement_dict_type & base_topology_pattern::get_measurement_dictionary() const
    {
      return _meas_;
    }

    // virtual void base_topology_pattern::build_particle_tracks_dictionary(const snemo::datamodel::particle_track_data::particle_collection_type & the_particles_,
    //                                                                    snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & particle_tracks_dict_)
    // {
    //   size_t n_electrons = 0;
    //   size_t n_positrons = 0;
    //   size_t n_alphas = 0;
    //   size_t n_gammas = 0;

    //   for (auto i_particle = the_particles_.begin(); i_particle != the_particles_.end(); ++i_particle) {
    //     std::ostringstream key;
    //     if(snemo::datamodel::pid_utils::particle_is_electron(i_particle->get())) {
    //       n_electrons++;
    //       key << "e" << n_electrons;
    //     }
    //     else if(snemo::datamodel::pid_utils::particle_is_positron(i_particle->get())) {
    //       n_positrons++;
    //       key << "p" << n_positrons;
    //     }
    //     else if(snemo::datamodel::pid_utils::particle_is_alpha(i_particle->get())) {
    //       n_alphas++;
    //       key << "a" << n_alphas;
    //     }
    //     else if(snemo::datamodel::pid_utils::particle_is_gammas(i_particle->get())) {
    //       n_gammas++;
    //       key << "g" << n_gammas;
    //     }
    //     else
    //       continue; // no undefined particles for now

    //     particle_tracks_dict_[key] = the_particles_->get();
    //   }

    //   return;
    // }


    void base_topology_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) {
        indent = indent_;
      }
      if (! title_.empty()) {
        out_ << indent << title_ << std::endl;
      }

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Pattern ID : " << "'" << pattern_id() << "'" << std::endl;
      return;
    }

  } // end of namespace datamodel

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
