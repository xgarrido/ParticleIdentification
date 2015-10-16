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

#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The base class of reconstructed topology
    class base_topology_pattern : DATATOOLS_SERIALIZABLE_CLASS,
                                  public datatools::i_tree_dumpable
    {
    public:

      typedef std::map<std::string,  particle_track::handle_type> particle_tracks_dict_type;

      typedef datatools::handle<base_topology_measurement> handle_measurement;

      typedef std::map<std::string, handle_measurement> measurement_dict_type;

      virtual std::string pattern_id() const = 0;

      virtual void build_particle_tracks_dictionary() const = 0;

      virtual void build_measurement_dictionary() const = 0;

      particle_tracks_dict_type & grab_particle_tracks_dictionary() {
        return _tracks_;
      }

      measurement_dict_type & grab_measurement_dictionary() {
        return _meas_;
      }
      /// Check if a valid pattern ID exists
      bool has_pattern_id() const;

      /// Return the pattern ID
      const std::string & get_pattern_id() const;

      virtual void build_particle_tracks_dictionary(std::map<std::string, particle_track::handle_type> & tracks_) const = 0;

      virtual void build_measurement_dictionary(std::map<std::string, const base_topology_measurement *> & meas_) const = 0;

      /// Constructor
      base_topology_pattern(const std::string & pattern_id_ = "");

      /// Destructor
      virtual ~base_topology_pattern();

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    protected:

      /// Set the pattern ID
      void _set_pattern_id(const std::string & pattern_id_);

    private:

      std::string _pattern_id_; //!< The pattern identifier
      particle_tracks_dict_type _tracks_;
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
