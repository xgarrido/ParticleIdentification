/** \file falaise/snemo/datamodels/topology_2e_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2e_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2e_pattern,
                                                      "snemo::datamodel::topology_2e_pattern")

    std::string topology_2e_pattern::pattern_id() const
    {
      const std::string id("2e");
      return id;
    }

    topology_2e_pattern::topology_2e_pattern()
      : base_topology_pattern()
    {
      return;
    }

    topology_2e_pattern::~topology_2e_pattern()
    {
      return;
    }

    void topology_2e_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

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
