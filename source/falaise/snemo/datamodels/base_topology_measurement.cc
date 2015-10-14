/** \file falaise/snemo/datamodels/base_topology_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(base_topology_measurement,
                                                      "snemo::datamodel::base_topology_measurement")

    virtual void base_topology_measurement::register_particle_track_id(const std::string & id_)
    {
      _particle_track_ids_.push_back(id_);
      return;
    }

    virtual const std::vector<std::string> & base_topology_measurement::particle_track_ids() const
    {
      return _particle_track_ids_;
    }

    base_topology_measurement::base_topology_measurement(const std::string & id_)
    {
      register_particle_track_id(id_);
      return;
    }

    base_topology_measurement::~base_topology_measurement()
    {
      return;
    }

    void base_topology_measurement::tree_dump(std::ostream      & out_,
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
           << "Pattern ID : ";
      if(! has_pattern_id()) {
        out_ << "<none>";
      } else {
        out_ << "'" << get_pattern_id() << "'";
      }
      out_ << std::endl;
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
