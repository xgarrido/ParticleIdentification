// -*- mode: c++; -*-
/// \file falaise/snemo/datamodels/the_serializable_bis.h

#ifndef FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H
#define FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H 1

// Third party:
// - Boost:
#include <boost/serialization/export.hpp>
// - Bayeux/datatools:
#include <datatools/archives_instantiation.h>

/******************************************
 * snemo::datamodel::topology_measurement *
 ******************************************/
#include <falaise/snemo/datamodels/topology_measurement.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::TOF_measurement)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::TOF_measurement)


/**************************************
 * snemo::datamodel::topology_pattern *
 **************************************/

#include <falaise/snemo/datamodels/base_topology_pattern.ipp>

#include <falaise/snemo/datamodels/topology_1e_pattern.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_1e_pattern)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_1e_pattern)

#include <falaise/snemo/datamodels/topology_2e_pattern.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_2e_pattern)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_2e_pattern)

#include <falaise/snemo/datamodels/topology_1e1p_pattern.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_1e1p_pattern)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_1e1p_pattern)

#include <falaise/snemo/datamodels/topology_1eNg_pattern.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_1eNg_pattern)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_1eNg_pattern)

#include <falaise/snemo/datamodels/topology_2eNg_pattern.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_2eNg_pattern)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_2eNg_pattern)

#include <falaise/snemo/datamodels/topology_1e1a_pattern.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_1e1a_pattern)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_1e1a_pattern)

/***********************************
 * snemo::datamodel::topology_data *
 ***********************************/

#include <falaise/snemo/datamodels/topology_data.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_data)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_data)

#endif // FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H
