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

    bool base_topology_pattern::has_particle_track(const std::string & key_) const
    {
      return _tracks_.find(key_) != _tracks_.end();
    }

    const snemo::datamodel::particle_track & base_topology_pattern::get_particle_track(const std::string & key_) const
    {
      return _tracks_.at(key_).get();
    }

    snemo::datamodel::base_topology_pattern::measurement_dict_type & base_topology_pattern::grab_measurement_dictionary()
    {
      return _meas_;
    }

    const snemo::datamodel::base_topology_pattern::measurement_dict_type & base_topology_pattern::get_measurement_dictionary() const
    {
      return _meas_;
    }

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
