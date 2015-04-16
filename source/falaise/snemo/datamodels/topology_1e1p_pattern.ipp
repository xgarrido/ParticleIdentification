// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/topology_1e1p_pattern.ipp

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_PATTERN_IPP
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_PATTERN_IPP 1

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1p_pattern.h>

// Third party:
// - Boost:
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.ipp>

namespace snemo {

  namespace datamodel {

    /// Serialization method
    template<class Archive>
    void topology_1e1p_pattern::serialize(Archive & ar_, const unsigned int /* version */)
    {
      ar_ & BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_topology_pattern);
      ar_ & boost::serialization::make_nvp("tof", _tof_);
      ar_ & boost::serialization::make_nvp("delta_vertices", _DeltaV_);
      ar_ & boost::serialization::make_nvp("angle", _angle_);
      return;
    }

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_PATTERN_IPP
