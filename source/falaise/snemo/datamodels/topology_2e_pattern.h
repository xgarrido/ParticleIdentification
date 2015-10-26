/// \file falaise/snemo/datamodels/topology_2e_pattern.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-05-19
 * Last modified: 2015-10-16
 *
 * Description: The 2 electrons topology pattern class
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H 1

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 2 electrons class of reconstructed topology
    class topology_2e_pattern : public base_topology_pattern
    {
    public:

      /// Return pattern identifier of the pattern
      virtual std::string pattern_id() const;

      /// Constructor
      topology_2e_pattern();

      /// Destructor
      virtual ~topology_2e_pattern();

      /// Check electron minimal energy validity
      bool has_electron_minimal_energy() const;

      /// Get electron minimal energy
      double get_electron_minimal_energy() const;

      /// Check electron maximal energy validity
      bool has_electron_maximal_energy() const;

      /// Get electron maximal energy
      double get_electron_maximal_energy() const;

      /// Get electrons energy sum
      double get_electrons_energy_sum() const;

      /// Get electrons energy difference
      double get_electrons_energy_difference() const;

      /// Check electrons TOF internal probability validity
      bool has_electrons_internal_probability() const;

      /// Get electrons TOF internal probability
      double get_electrons_internal_probability() const;

      /// Check electrons TOF external probability validity
      bool has_electrons_external_probability() const;

      /// Get electrons TOF external probability
      double get_electrons_external_probability() const;

      /// Check angle between electrons validity
      bool has_electrons_angle() const;

      /// Get angle between electrons
      double get_electrons_angle() const;

      /// Check common vertices probability between electrons validity
      bool has_electrons_vertices_probability() const;

      /// Get common vertices between electrons
      double get_electrons_vertices_probability() const;

      /// Get common vertices location between electrons
      std::string get_electrons_vertices_location() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

// #include <boost/serialization/export.hpp>
// BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_2e_pattern,
//                         "snemo::datamodel::topology_2e_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
