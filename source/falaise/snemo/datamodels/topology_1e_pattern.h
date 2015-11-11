/// \file falaise/snemo/datamodels/topology_1e_pattern.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-11-09
 * Last modified: 2015-11-09
 *
 * Description: The 1 electron topology pattern class
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_PATTERN_H 1

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 1e class of reconstructed topology
    class topology_1e_pattern : public base_topology_pattern
    {
    public:

      /// Static function to return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Return pattern identifier of the pattern
      virtual std::string get_pattern_id() const;

      /// Constructor
      topology_1e_pattern();

      /// Destructor
      virtual ~topology_1e_pattern();

      /// Check angle measurement availability
      bool has_electron_angle() const;

      /// Return electron angle
      double get_electron_angle() const;

      /// Check electron energy validity
      bool has_electron_energy() const;

      /// Return electron energy
      double get_electron_energy() const;

    private:

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_1e_pattern,
                        "snemo::datamodel::topology_1e_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
