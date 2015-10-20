/** \file falaise/snemo/datamodels/tof_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/tof_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(tof_measurement,
                                                      "snemo::datamodel::tof_measurement")

    tof_measurement::tof_measurement()
    {
      return;
    }

    tof_measurement::~tof_measurement()
    {
      return;
    }

    const tof_measurement::probability_type & tof_measurement::get_internal_probabilities() const
    {
      return _internal_probabilities_;
    }

    tof_measurement::probability_type & tof_measurement::grab_internal_probabilities()
    {
      return _internal_probabilities_;
    }

    const tof_measurement::probability_type & tof_measurement::get_external_probabilities() const
    {
      return _external_probabilities_;
    }

    tof_measurement::probability_type & tof_measurement::grab_external_probabilities()
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
