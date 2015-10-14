/// \file falaise/snemo/datamodels/topology_2e_pattern.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-05-19
 * Last modified: 2015-05-27
 *
 * Description: The 2 electrons topology pattern class
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_measurement.h>
#include <falaise/snemo/datamodels/particle_track_data.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 2 electrons class of reconstructed topology
    class topology_2e_pattern : public base_topology_pattern
    {
    public:

      /// Return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Constructor
      topology_2e_pattern();

      /// Destructor
      virtual ~topology_2e_pattern();

      /// Check if topology is valid
      bool is_valid() const;

      virtual void build_particle_tracks_dictionary(const snemo::datamodel::particle_track_data::particle_collection_type & the_particles_,
                                      snemo::datamodel::base_topology_pattern::measurement_particle_tracks_type & particle_tracks_dict_);

      virtual void build_measurement_dictionary(const snemo::datamodel::particle_track_data::particle_collection_type & the_particles_,
                                      snemo::datamodel::base_topology_pattern::measurement_particle_tracks_type & particle_tracks_dict_);

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_2e_pattern,
                        "snemo::datamodel::topology_2e_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
