/** \file falaise/snemo/datamodels/delta_vertices_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/delta_vertices_measurement.h>

// - Bayeux/geomtools:
#include <geomtools/blur_spot.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(delta_vertices_measurement,
                                                      "snemo::datamodel::delta_vertices_measurement")

    delta_vertices_measurement::delta_vertices_measurement()
    {
      // _probability_.invalidate()
        return;
    }

    delta_vertices_measurement::~delta_vertices_measurement()
    {
      return;
    }

    bool delta_vertices_measurement::is_valid() const
    {
      return datatools::is_valid(_probability_);
    }

    // void delta_vertices_measurement::set_delta_vertices(geomtools::blur_spot & delta_vertices_)
    // {
    //   _delta_vertices_ = delta_vertices_;
    //   return;
    // }

    // const geomtools::blur_spot & delta_vertices_measurement::get_delta_vertices() const
    // {
    //   return _delta_vertices_;
    // }

    // geomtools::blur_spot & delta_vertices_measurement::grab_delta_vertices()
    // {
    //   return _delta_vertices_;
    // }

    const double & delta_vertices_measurement::get_probability() const
    {
      return _probability_;
    }

    double & delta_vertices_measurement::grab_probability()
    {
      return _probability_;
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
