/// \file falaise/snemo/datamodels/delta_vertices_measurement.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The base class of topology measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H 1

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The Delta Vertices measurement
    class delta_vertices_measurement : public base_topology_measurement {

    public:

      /// Constructor
      delta_vertices_measurement();

      /// Destructor
      ~delta_vertices_measurement();

      /// Check y-delta vertices
      bool has_delta_vertices_y() const;

      /// Set y-delta vertices
      void set_delta_vertices_y(double delta_);

      /// Get a non-mutable reference to delta vertices y
      const double & get_delta_vertices_y() const;

      /// Get a mutable reference to delta vertices y
      double & grab_delta_vertices_y();

      /// Check z-delta vertices
      bool has_delta_vertices_z() const;

      /// Set z-delta vertices
      void set_delta_vertices_z(double delta_);

      /// Get a non-mutable reference to delta vertices z
      const double & get_delta_vertices_z() const;

      /// Get a mutable reference to delta vertices z
      double & grab_delta_vertices_z();

    private:

      double _delta_vertices_y_;
      double _delta_vertices_z_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
