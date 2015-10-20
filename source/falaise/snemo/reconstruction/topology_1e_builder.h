/// \file falaise/snemo/datamodels/topology_1e_builder.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-16
 * Last modified: 2015-10-16
 *
 * Description: The base class to build '1e' topology pattern
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_BUILDER_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_BUILDER_H 1

// This project:
#include <falaise/snemo/reconstruction/base_topology_builder.h>

namespace snemo {

  namespace reconstruction {

    /// \brief The base class to build '1e' topology pattern
    class topology_1e_builder : public base_topology_builder
    {
    protected:

      ///
      virtual snemo::datamodel::base_topology_pattern::handle_type _create_pattern();

      virtual void _build_measurement_dictionary(snemo::datamodel::base_topology_pattern & pattern_);

    private:

      /// Macro to automate the registration of the cut
      FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_INTERFACE(topology_1e_builder);
    };
  }
}

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_BUILDER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
