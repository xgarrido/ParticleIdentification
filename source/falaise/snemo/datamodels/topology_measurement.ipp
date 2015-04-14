// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/topology_measurement.ipp

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_MEASUREMENT_IPP
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_MEASUREMENT_IPP 1

namespace snemo {

  namespace datamodel {

    /// Serialization method
    template<class Archive>
    void TOF_measurement::serialize(Archive & ar_, const unsigned int /* version */)
    {
      ar_ & boost::serialization::make_nvp("internal_probabilities", _internal_probabilities_);
      ar_ & boost::serialization::make_nvp("external_probabilities", _external_probabilities_);
      return;
    }

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_MEASUREMENT_IPP
