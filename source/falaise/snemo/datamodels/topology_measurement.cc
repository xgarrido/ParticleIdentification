/** \file falaise/snemo/datamodels/topology_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(TOF_measurement,
                                                      "snemo::datamodel::TOF_measurement")

    TOF_measurement::TOF_measurement()
    {
      return;
    }

    TOF_measurement::~TOF_measurement()
    {
      return;
    }

    const TOF_measurement::probability_type & TOF_measurement::get_internal_probabilities() const
    {
      return _internal_probabilities_;
    }

    TOF_measurement::probability_type & TOF_measurement::grab_internal_probabilities()
    {
      return _internal_probabilities_;
    }

    const TOF_measurement::probability_type & TOF_measurement::get_external_probabilities() const
    {
      return _external_probabilities_;
    }

    TOF_measurement::probability_type & TOF_measurement::grab_external_probabilities()
    {
      return _external_probabilities_;
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
