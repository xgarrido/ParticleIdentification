/// \file falaise/snemo/datamodels/topology_1e1p_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The 1e1p class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/topology_1e_pattern.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 1e1p class of reconstructed topology
    class topology_1e1p_pattern : public topology_1e_pattern
    {
    public:

      /// Static function to return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Return pattern identifier of the pattern
      virtual std::string get_pattern_id() const;

      /// Constructor
      topology_1e1p_pattern();

      /// Destructor
      virtual ~topology_1e1p_pattern();

      /// Check positron energy validity
      bool has_positron_energy() const;

      /// Get positron energy
      double get_positron_energy() const;

      /// Check angle measurement availability
      bool has_positron_angle() const;

      /// Return positron angle
      double get_positron_angle() const;

      /// Check angle measurement availability
      bool has_electron_positron_angle() const;

      /// Return electron-positron angle
      double get_electron_positron_angle() const;

      /// Check electron-positron TOF internal probability validity
      bool has_electron_positron_internal_probability() const;

      /// Get electron-positron TOF internal probability
      double get_electron_positron_internal_probability() const;

      /// Check electron-positron TOF external probability validity
      bool has_electron_positron_external_probability() const;

      /// Get electron-positron TOF external probability
      double get_electron_positron_external_probability() const;

      /// Check common vertices probability between electron and positron validity
      bool has_electron_positron_vertices_probability() const;

      /// Get common vertices probability between electron and positron
      double get_electron_positron_vertices_probability() const;

      /// Check electron minimal energy validity
      bool has_electron_positron_minimal_energy() const;

      /// Get electron minimal energy
      double get_electron_positron_minimal_energy() const;

      /// Check electron maximal energy validity
      bool has_electron_positron_maximal_energy() const;

      /// Get electron maximal energy
      double get_electron_positron_maximal_energy() const;

    private:

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_1e1p_pattern,
                        "snemo::datamodel::topology_1e1p_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1P_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
