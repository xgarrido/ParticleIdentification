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

    datatools::handle<snemo::datamodel::base_topology_pattern> topology_2e_builder::create_pattern()
    {
      datatools::handle<snemo::datamodel::base_topology_pattern> h(new snemo::datamodel::topology_2e_pattern);
      return h;
    }


    ///
    void topology_2e_builder::build(const snemo::datamodel::particle_track_data & source_,
                                    snemo::datamodel::base_topology_pattern & target_)
    {
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
