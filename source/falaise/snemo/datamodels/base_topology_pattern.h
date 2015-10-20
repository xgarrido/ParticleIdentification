/// \file falaise/snemo/datamodels/base_topology_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The base class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_PATTERN_H 1

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The base class of reconstructed topology
    class base_topology_pattern : DATATOOLS_SERIALIZABLE_CLASS,
                                  public datatools::i_tree_dumpable
    {
    public:

      /// Typedef to particle track dictionary
      typedef std::map<std::string, snemo::datamodel::particle_track::handle_type> particle_track_dict_type;

      /// Typedef to base topology pattern handle
      typedef datatools::handle<snemo::datamodel::base_topology_pattern> handle_type;

      /// Typedef to measurement handle
      typedef datatools::handle<base_topology_measurement> handle_measurement;

      /// Typedef to measurement dictionary
      typedef std::map<std::string, handle_measurement> measurement_dict_type;

      /// Return topology pattern id
      virtual std::string pattern_id() const = 0;

      /// Get a mutable reference to particle track dictionary
      particle_track_dict_type & grab_particle_track_dictionary();

      /// Get a non-mutable reference to particle track dictionary
      const particle_track_dict_type & get_particle_track_dictionary() const;

      /// Check if a particle track exists
      bool has_particle_track(const std::string &) const;

      /// Get a given particle track
      const snemo::datamodel::particle_track & get_particle_track(const std::string &) const;

      /// Check if a measurement is available
      bool has_measurement(const std::string &) const;

      /// Get a given measurement
      const snemo::datamodel::base_topology_measurement & get_measurement(const std::string &) const;

      /// Get a mutable reference to measurement dictionary
      measurement_dict_type & grab_measurement_dictionary();

      /// Get a non-mutable reference to measurement dictionary
      const measurement_dict_type & get_measurement_dictionary() const;

      /// Constructor
      base_topology_pattern();

      /// Destructor
      virtual ~base_topology_pattern();

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      particle_track_dict_type _tracks_;
      measurement_dict_type _meas_;

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
