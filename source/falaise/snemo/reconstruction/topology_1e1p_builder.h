/// \file falaise/snemo/datamodels/topology_1e1p_builder.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-16
 * Last modified: 2015-10-16
 *
 * Description: The base class to build '1e1p' topology pattern
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_BUILDER_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_BUILDER_H 1

// This project:
#include <falaise/snemo/reconstruction/base_topology_builder.h>
#include <falaise/snemo/reconstruction/topology_1e_builder.h>
#include <falaise/snemo/reconstruction/tof_driver.h>
#include <falaise/snemo/reconstruction/delta_vertices_driver.h>
#include <falaise/snemo/reconstruction/angle_measurement_driver.h>
#include <falaise/snemo/reconstruction/energy_driver.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace reconstruction {

    /// \brief The base class to build '1e1p' topology pattern
    class topology_1e1p_builder : public topology_1e_builder
    {
    public:

      ///
      virtual snemo::datamodel::base_topology_pattern::handle_type create_pattern();

      virtual void build_measurement_dictionary(const snemo::datamodel::particle_track_data & source_,
                                                snemo::datamodel::base_topology_pattern & pattern_);

    private:

      /// Macro to automate the registration of the cut
      FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_INTERFACE(topology_1e1p_builder);
    };
  }
}

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_BUILDER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
