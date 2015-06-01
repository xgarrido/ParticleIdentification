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
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_measurement.h>

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

      /// Check internal probability validity
      bool has_internal_probability() const;

      /// Set internal probability
      void set_internal_probability(double);

      /// Return internal probability
      double get_internal_probability() const;

      /// Check internal probability validity
      bool has_external_probability() const;

      /// Set external probability
      void set_external_probability(double);

      /// Return external probability
      double get_external_probability() const;

      /// Check delta vertices y validity
      bool has_delta_vertices_y() const;

      /// Set delta vertices y
      void set_delta_vertices_y(double);

      /// Return delta vertices y
      double get_delta_vertices_y() const;

      /// Check delta vertices z validity
      bool has_delta_vertices_z() const;

      /// Set delta vertices z
      void set_delta_vertices_z(double);

      /// Return delta vertices z
      double get_delta_vertices_z() const;

      /// Check angle validity
      bool has_angle() const;

      /// Set angle value
      void set_angle(double);

      /// Return angle measurement
      double get_angle() const;

      /// Check minimal energy validity
      bool has_minimal_energy() const;

      /// Set minimal energy
      void set_minimal_energy(double);

      /// Return minimal energy
      double get_minimal_energy() const;

      /// Check maximal energy validity
      bool has_maximal_energy() const;

      /// Set maximal energy
      void set_maximal_energy(double);

      /// Return maximal energy
      double get_maximal_energy() const;

      /// Check total energy validity
      bool has_total_energy() const;

      /// Return total energy
      double get_total_energy() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      TOF_measurement _tof_;                              //!< Time-Of-Flight meas.
      delta_vertices_measurement _delta_vertices_source_; //!< Delta vertices on source foil
      angle_measurement _angle_;                          //!< Angle meas.
      double _electron_minimal_energy_;                   //!< Minimal energy of the electron
      double _electron_maximal_energy_;                   //!< Maximal energy of the electron

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
