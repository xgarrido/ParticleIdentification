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
#include <datatools/i_serializable.h>
// - Bayeux/geomtools:
#include <geomtools/i_shape_1d.h>

namespace snemo {

  namespace datamodel {

    /// \brief The base class of reconstructed topology
    class base_topology_pattern : DATATOOLS_SERIALIZABLE_CLASS
    {
    public:

      /// Check if a valid pattern ID exists
      bool has_pattern_id() const;

      /// Return the pattern ID
      const std::string & get_pattern_id() const;

      /// Constructor
      base_topology_pattern(const std::string & pattern_id_ = "");

      /// Destructor
      virtual ~base_topology_pattern();

    protected:

      /// Set the pattern ID
      void _set_pattern_id(const std::string & pattern_id_);

    private:

      std::string _pattern_id_; //!< The pattern identifier

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
