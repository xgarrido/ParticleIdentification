/// \file falaise/snemo/datamodels/energy_measurement.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The base class of topology measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_ENERGY_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_ENERGY_MEASUREMENT_H 1

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>

#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The Energy measurement
    class energy_measurement : public base_topology_measurement {
    public:

      /// Constructor
      energy_measurement();

      /// Destructor
      ~energy_measurement();

      /// Check energy validity
      bool has_energy() const;

      /// Set energy value
      void set_energy(double energy_);

      /// Get a non-mutable reference to energy
      const double & get_energy() const;

      /// Get a mutable reference to energy
      double & grab_energy();

    private:

      double _energy_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_ENERGY_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
