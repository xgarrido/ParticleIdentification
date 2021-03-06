// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/angle_measurement.ipp

#ifndef FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_IPP
#define FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_IPP 1

// Ourselves:
#include <falaise/snemo/datamodels/angle_measurement.h>

// Third party:
// - Boost:
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
// - Bayeux/datatools:
#include <datatools/i_serializable.ipp>

// This project:
#include <falaise/snemo/datamodels/base_topology_measurement.ipp>

namespace snemo {

  namespace datamodel {

    /// Serialization method
    template<class Archive>
    void angle_measurement::serialize(Archive & ar_, const unsigned int /* version_ */)
    {
      ar_ & BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_topology_measurement);
      ar_ & boost::serialization::make_nvp("angle", _angle_);
      return;
    }

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_IPP
