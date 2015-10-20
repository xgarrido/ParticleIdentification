/// \file falaise/snemo/datamodels/tof_measurement.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2014-01-27
 * Last modified: 2015-10-20
 *
 * Description: The Time-Of-Flight measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOF_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_TOF_MEASUREMENT_H 1

// This project:
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The Time Of Flight measurement
    class tof_measurement : public base_topology_measurement {

    public:

      /// Typedef for probability type
      typedef std::vector<double> probability_type;

      /// Constructor
      tof_measurement();

      /// Destructor
      ~tof_measurement();

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
