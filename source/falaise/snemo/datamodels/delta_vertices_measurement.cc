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
      _vertices_barycenter_.invalidate();
      return;
    }

    delta_vertices_measurement::~delta_vertices_measurement()
    {
      return;
    }

    bool delta_vertices_measurement::is_valid() const
    {
      //not sure, does it assume the existence of a blur_spot
      // return  _vertices_barycenter_.get_auxiliaries().has_key("Probability");
      return (geomtools::is_valid(_vertices_barycenter_.get_position()) &&
              _vertices_barycenter_.get_auxiliaries().has_key("Probability"));
    }

    double delta_vertices_measurement::get_probability() const
    {
      return _vertices_barycenter_.get_auxiliaries().fetch_real("Probability");
    }

    const geomtools::blur_spot & delta_vertices_measurement::get_vertices_barycenter() const
    {
      return _vertices_barycenter_;
    }

    geomtools::blur_spot & delta_vertices_measurement::grab_vertices_barycenter()
    {
      return _vertices_barycenter_;
    }

    // double & delta_vertices_measurement::grab_probability()
    // {
    //   return _vertices_barycenter_.grab_auxiliaries().fetch_real("Probability");
    // }

    // const std::string & delta_vertices_measurement::get_vertices_location() const
    // {
    //   return _vertices_probability_.first;
    // }

    // std::string & delta_vertices_measurement::grab_vertices_location()
    // {
    //   return _vertices_probability_.first;
    // }

  } // end of namespace datamodel

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
