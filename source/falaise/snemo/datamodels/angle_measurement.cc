/** \file falaise/snemo/datamodels/angle_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/angle_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(angle_measurement,
                                                      "snemo::datamodel::angle_measurement")

    angle_measurement::angle_measurement()
    {
      datatools::invalidate(_angle_);
      return;
    }

    angle_measurement::~angle_measurement()
    {
      return;
    }

    bool angle_measurement::is_valid() const
    {
      return datatools::is_valid(_angle_);
    }

    void angle_measurement::set_angle(double angle_)
    {
      _angle_ = angle_;
      return;
    }

    const double & angle_measurement::get_angle() const
    {
      return _angle_;
    }

    double & angle_measurement::grab_angle()
    {
      return _angle_;
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
