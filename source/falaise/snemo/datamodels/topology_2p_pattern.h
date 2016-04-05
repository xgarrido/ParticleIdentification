/// \file falaise/snemo/datamodels/topology_2p_pattern.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-05-19
 * Last modified: 2015-10-16
 *
 * Description: The 2 positrons topology pattern class
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2P_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2P_PATTERN_H 1

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 2 positrons class of reconstructed topology
    class topology_2p_pattern : public base_topology_pattern
    {
    public:

      /// Static function to return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Return pattern identifier of the pattern
      virtual std::string get_pattern_id() const;

      /// Constructor
      topology_2p_pattern();

      /// Destructor
      virtual ~topology_2p_pattern();

      /// Check positron minimal energy validity
      bool has_positron_minimal_energy() const;

      /// Get positron minimal energy
      double get_positron_minimal_energy() const;

      /// Check positron maximal energy validity
      bool has_positron_maximal_energy() const;

      /// Get positron maximal energy
      double get_positron_maximal_energy() const;

      /// Check positrons energy
      bool has_positrons_energy() const;

      /// Get positrons energy sum
      double get_positrons_energy_sum() const;

      /// Get positrons energy difference
      double get_positrons_energy_difference() const;

       /// Get the name of the minimal energy positron
      std::string get_minimal_energy_positron_name() const;

      /// Get the name of the maximal energy positron
      std::string get_maximal_energy_positron_name() const;

      /// Check positrons TOF internal probability validity
      bool has_positrons_internal_probability() const;

      /// Get positrons TOF internal probability
      double get_positrons_internal_probability() const;

      /// Check positrons TOF external probability validity
      bool has_positrons_external_probability() const;

      /// Get positrons TOF external probability
      double get_positrons_external_probability() const;

      /// Check angle between positrons validity
      bool has_positrons_angle() const;

      /// Get angle between positrons
      double get_positrons_angle() const;

      /// Check common vertices probability between positrons validity
      bool has_positrons_vertices_probability() const;

      /// Get common vertices probability between positrons
      double get_positrons_vertices_probability() const;

      // /// Get common vertices location between positrons
      // std::string get_positrons_vertices_location() const;

    private:

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_2p_pattern,
                        "snemo::datamodel::topology_2p_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2P_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
