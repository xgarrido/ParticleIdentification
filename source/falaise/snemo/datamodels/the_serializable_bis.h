// -*- mode: c++; -*-
/// \file falaise/snemo/datamodels/the_serializable_bis.h

#ifndef FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H
#define FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H 1

// Third party:
// - Boost:
#include <boost/serialization/export.hpp>
// - Bayeux/datatools:
#include <datatools/archives_instantiation.h>

/*************************************************
 * snemo::datamodel::tracker_trajectory_solution *
 *************************************************/

// #include <falaise/snemo/datamodels/base_trajectory_pattern.ipp>
// #include <falaise/snemo/datamodels/line_trajectory_pattern.ipp>
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::line_trajectory_pattern)
// BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::line_trajectory_pattern)

// #include <falaise/snemo/datamodels/helix_trajectory_pattern.ipp>
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::helix_trajectory_pattern)
// BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::helix_trajectory_pattern)

// #include <falaise/snemo/datamodels/polyline_trajectory_pattern.ipp>
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::polyline_trajectory_pattern)
// BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::polyline_trajectory_pattern)

// #include <falaise/snemo/datamodels/topology.ipp>
// #include <falaise/snemo/datamodels/topology_solution.ipp>
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology)
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_solution)
#include <falaise/snemo/datamodels/topology_data.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_data)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_data)

#endif // FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H
