/// \file falaise/snemo/datamodels/topology_1e_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The 1e class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 1e class of reconstructed topology
    class topology_1e_pattern : public base_topology_pattern
    {
    public:

      /// Return pattern identifier of the pattern
      virtual std::string pattern_id() const;

      /// Constructor
      topology_1e_pattern();

      /// Destructor
      virtual ~topology_1e_pattern();

      /// Check angle validity
      bool has_angle() const;

      // /// Set angle
      // void set_angle(double);

      /// Return internal probability
      double get_angle() const;

      // /// Check electron energy validity
      // bool has_electron_energy() const;

      // /// Set electron energy
      // void set_electron_energy(double);

      // /// Get electron energy
      // double get_electron_energy() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      // angle_measurement _angle_;
      // double _electron_energy_;                    //!< Energy of electron particle

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
