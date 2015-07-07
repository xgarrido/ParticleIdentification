/// \file falaise/snemo/datamodels/topology_1e1a_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The 1e1a class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1A_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1A_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 1 electron - 1 alpha class of reconstructed topology
    class topology_1e1a_pattern : public base_topology_pattern
    {
    public:

      /// Return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Constructor
      topology_1e1a_pattern();

      /// Destructor
      virtual ~topology_1e1a_pattern();

      /// Check if topology is valid
      bool is_valid() const;

      /// Check if the electron particle is present
      bool has_electron_particle() const;

      /// Detach the electron particle
      void detach_electron_particle();

      /// Attach a electron particle by handle
      void set_electron_particle(const particle_track::handle_type & handle_);

      /// Return a non mutable reference on the electron particle
      const particle_track & get_electron_particle() const;

      /// Check if the alpha particle is present
      bool has_alpha_particle() const;

      /// Detach the alpha particle
      void detach_alpha_particle();

      /// Attach a alpha particle by handle
      void set_alpha_particle(const particle_track::handle_type & handle_);

      /// Return a non mutable reference on the alpha particle
      const particle_track & get_alpha_particle() const;

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

      /// Set angle
      void set_angle(double);

      /// Return internal probability
      double get_angle() const;

      /// Check alpha delayed time validity
      bool has_alpha_delayed_time() const;

      /// Set alpha delayed time
      void set_alpha_delayed_time(double);

      /// Get alpha delayed time
      double get_alpha_delayed_time() const;

      /// Check alpha track length validity
      bool has_alpha_track_length() const;

      /// Set alpha track_length
      void set_alpha_track_length(double);

      /// Get alpha track length
      double get_alpha_track_length() const;

      /// Check electron track length validity
      bool has_electron_track_length() const;

      /// Set electron track_length
      void set_electron_track_length(double);

      /// Get electron track length
      double get_electron_track_length() const;

      /// Check electron energy validity
      bool has_electron_energy() const;

      /// Set electron energy
      void set_electron_energy(double);

      /// Get electron energy
      double get_electron_energy() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      delta_vertices_measurement _delta_vertices_; //!< Delta vertices meas.
      angle_measurement _angle_;                   //!< Angle meas.
      particle_track::handle_type _electron_particle_;
      particle_track::handle_type _alpha_particle_;
      double _alpha_delayed_time_;                 //!< Time delay of alpha particle
      double _alpha_track_length_;                 //!< Track length of alpha particle
      double _electron_track_length_;              //!< Track length of electron particle
      double _electron_energy_;                    //!< Energy of electron particle

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_1e1a_pattern,
                        "snemo::datamodel::topology_1e1a_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1E1A_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
