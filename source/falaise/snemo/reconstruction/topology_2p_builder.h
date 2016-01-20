/// \file falaise/snemo/datamodels/topology_2p_builder.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-16
 * Last modified: 2015-10-16
 *
 * Description: The base class to build '2p' topology pattern
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2P_BUILDER_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2P_BUILDER_H 1

// This project:
#include <falaise/snemo/reconstruction/base_topology_builder.h>

namespace snemo {

  namespace reconstruction {

    /// \brief The base class to build '2p' topology pattern
    class topology_2p_builder : public base_topology_builder
    {
    public:

      ///
      virtual snemo::datamodel::base_topology_pattern::handle_type _create_pattern();

      virtual void _build_measurement_dictionary(snemo::datamodel::base_topology_pattern & pattern_);

    private:

      /// Macro to automate the registration of the cut
      FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_INTERFACE(topology_2p_builder);
    };
  }
}

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2P_BUILDER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
