/// \file falaise/snemo/datamodels/topology_2e_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The 2e class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/topology_data.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 2e class of reconstructed topology
    class topology_2e_pattern : DATATOOLS_SERIALIZABLE_CLASS
    {
    public:

      /// Check if a valid pattern ID exists
      bool has_pattern_id() const;

      /// Return the pattern ID
      const std::string & get_pattern_id() const;

      /// Constructor
      topology_2e_pattern(const std::string & pattern_id_ = "");

      /// Destructor
      virtual ~topology_2e_pattern();

    protected:

      /// Set the pattern ID
      void _set_pattern_id(const std::string & pattern_id_);

    private:

      std::string _pattern_id_; //!< The pattern identifier
      std::map < std::pair < snemo::datamodel::particle_track::handle_type &,
                             snemo::datamodel::particle_track::handle_type & > ,
                 struct snemo::datamodel::topology_data::TOF_info_type & > _ptp2info_; // particle_track pair to info

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
