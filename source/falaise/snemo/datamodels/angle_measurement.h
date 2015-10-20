/// \file falaise/snemo/datamodels/angle_measurement.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The base class of topology measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_H 1

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>

#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The Angle measurement
    class angle_measurement : public base_topology_measurement {

    public:

      /// Constructor
      angle_measurement();

      /// Destructor
      ~angle_measurement();

      /// Check angle validity
      bool is_valid() const;

      /// Set angle value
      void set_angle(double angle_);

      /// Get a non-mutable reference to angle
      const double & get_angle() const;

      /// Get a mutable reference to angle
      double & grab_angle();

    private:

      double _angle_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
