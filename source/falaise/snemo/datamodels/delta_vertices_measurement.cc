/** \file falaise/snemo/datamodels/delta_vertices_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/delta_vertices_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(delta_vertices_measurement,
                                                      "snemo::datamodel::delta_vertices_measurement")

    delta_vertices_measurement::delta_vertices_measurement()
    {
      datatools::invalidate(_delta_vertices_y_);
      datatools::invalidate(_delta_vertices_z_);
      return;
    }

    delta_vertices_measurement::~delta_vertices_measurement()
    {
      return;
    }

    bool delta_vertices_measurement::has_delta_vertices_y() const
    {
      return datatools::is_valid(_delta_vertices_y_);
    }

    void delta_vertices_measurement::set_delta_vertices_y(double delta_)
    {
      _delta_vertices_y_ = delta_;
      return;
    }

    const double & delta_vertices_measurement::get_delta_vertices_y() const
    {
      return _delta_vertices_y_;
    }

    double & delta_vertices_measurement::grab_delta_vertices_y()
    {
      return _delta_vertices_y_;
    }

    bool delta_vertices_measurement::has_delta_vertices_z() const
    {
      return datatools::is_valid(_delta_vertices_z_);
    }

    void delta_vertices_measurement::set_delta_vertices_z(double delta_)
    {
      _delta_vertices_z_ = delta_;
      return;
    }

    const double & delta_vertices_measurement::get_delta_vertices_z() const
    {
      return _delta_vertices_z_;
    }

    double & delta_vertices_measurement::grab_delta_vertices_z()
    {
      return _delta_vertices_z_;
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
