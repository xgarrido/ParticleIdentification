/// \file falaise/snemo/datamodels/topology_measurement.h
/* Author(s) :    Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-05-14
 * Last modified: 2015-05-14
 *
 * Description: The 2
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_MEASUREMENT_H 1

// Standard library:
#include <vector>

// Third party:
// - Bayeux/datatools:
#include <datatools/i_serializable.h>
// - Falaise:
#include <falaise/snemo/datamodels/particle_track.h>

namespace snemo {

  namespace datamodel {

    /// \brief The Time Of Flight measurement
    class TOF_measurement : DATATOOLS_SERIALIZABLE_CLASS
    {
    public:

      /// Typedef for probability type
      typedef std::vector<double> probability_type;

      /// Typedef for pair of particle tracks
      typedef std::pair<snemo::datamodel::particle_track::handle_type,
                        snemo::datamodel::particle_track::handle_type> particle_pair_type;

      /// Constructor
      TOF_measurement();

      /// Destructor
      ~TOF_measurement();

      /// Get a non-mutable reference to internal probabilities
      const probability_type & get_internal_probabilities() const;

      /// Get a mutable reference to internal probabilities
      probability_type & grab_internal_probabilities();

      /// Get a non-mutable reference to external probabilities
      const probability_type & get_external_probabilities() const;

      /// Get a mutable reference to external probabilities
      probability_type & grab_external_probabilities();

      /// Check if particle tracks are associated
      bool has_particle_tracks() const;

      /// Get a non-mutable reference to particles tracks
      const particle_pair_type & get_particle_tracks() const;

      /// Get a mutable reference to particles tracks
      particle_pair_type & grab_particle_tracks();

      /// Attach a cluster by handle
      void set_particle_tracks(const snemo::datamodel::particle_track::handle_type & hpt1_,
                               const snemo::datamodel::particle_track::handle_type & hpt2_);

    private:

      probability_type _internal_probabilities_;
      probability_type _external_probabilities_;
      particle_pair_type _particle_track_pair_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

    /// \brief The Delta Vertices measurement
    class delta_vertices_measurement : DATATOOLS_SERIALIZABLE_CLASS
    {
    public:

      /// Typedef for pair of particle tracks
      typedef std::pair<snemo::datamodel::particle_track::handle_type,
                        snemo::datamodel::particle_track::handle_type> particle_pair_type;

      /// Constructor
      delta_vertices_measurement();

      /// Destructor
      ~delta_vertices_measurement();

      /// Get a non-mutable reference to delta vertices y
      const double & get_delta_vertices_y() const;

      /// Get a mutable reference to delta vertices y
      double & grab_delta_vertices_y();

      /// Get a non-mutable reference to delta vertices z
      const double & get_delta_vertices_z() const;

      /// Get a mutable reference to delta vertices z
      double & grab_delta_vertices_z();

      /// Check if particle tracks are associated
      bool has_particle_tracks() const;

      /// Get a non-mutable reference to particles tracks
      const particle_pair_type & get_particle_tracks() const;

      /// Get a mutable reference to particles tracks
      particle_pair_type & grab_particle_tracks();

      /// Attach a cluster by handle
      void set_particle_tracks(const snemo::datamodel::particle_track::handle_type & hpt1_,
                               const snemo::datamodel::particle_track::handle_type & hpt2_);

    private:

      double _delta_vertices_y_;
      double _delta_vertices_z_;
      particle_pair_type _particle_track_pair_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };


    /// \brief The Angle measurement
    class angle_measurement : DATATOOLS_SERIALIZABLE_CLASS
    {
    public:

      /// Typedef for angle type
      typedef std::vector<double> angle_type;

      /// Typedef for pair of particle tracks
      typedef std::pair<snemo::datamodel::particle_track::handle_type,
                        snemo::datamodel::particle_track::handle_type> particle_pair_type;

      /// Constructor
      angle_measurement();

      /// Destructor
      ~angle_measurement();

      /// Get a non-mutable reference to delta vertices z
      const angle_type & get_angle() const;

      /// Get a mutable reference to delta vertices z
      angle_type & grab_angle();

      /// Check if particle tracks are associated
      bool has_particle_tracks() const;

      /// Get a non-mutable reference to particles tracks
      const particle_pair_type & get_particle_tracks() const;

      /// Get a mutable reference to particles tracks
      particle_pair_type & grab_particle_tracks();

      /// Attach a cluster by handle
      void set_particle_tracks(const snemo::datamodel::particle_track::handle_type & hpt1_,
                               const snemo::datamodel::particle_track::handle_type & hpt2_);

    private:

      angle_type _angle_;
      particle_pair_type _particle_track_pair_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };
  }
}

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_MEASUREMENT_H
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
