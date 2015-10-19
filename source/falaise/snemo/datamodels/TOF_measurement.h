/// \file falaise/snemo/datamodels/TOF_measurement.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The base class of topology measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOF_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_TOF_MEASUREMENT_H 1

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>

#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The Time Of Flight measurement
    class TOF_measurement : public base_topology_measurement {

    public:

      /// Typedef for probability type
      typedef std::vector<double> probability_type;

      /// Constructor
      TOF_measurement();

      /// Destructor
      ~TOF_measurement();

      /// Get a non-mutable reference to internal probabilities
      const probability_type & get_internal_probabilities() const;

      /// Get a mutable reference to internal probabilities
      probability_type & grab_internal_probabilities();

      /// Get a non-mutable reference to external probabilities
      const probability_type & get_external_probabilities() const;

      /// Get a mutable reference to external probabilities
      probability_type & grab_external_probabilities();

    private:

      probability_type _internal_probabilities_;
      probability_type _external_probabilities_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_TOF_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
