/// \file falaise/snemo/datamodels/topology_2e1g_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The 2e1g class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E1G_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E1G_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/topology_data.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 2e1g class of reconstructed topology
    class topology_2e1g_pattern : public base_topology_pattern
    {
    public:

      struct TOF_measurement
      {
        std::vector <double> internal_probability;
        std::vector <double> external_probability;
      };

      struct delta_vertices_measurement
      {
        double delta_vertices_y;
        double delta_vertices_z;
      };

      struct angle_measurement
      {
        double angle;
      };

      /// Typedef for pairing particles
      typedef std::pair<snemo::datamodel::particle_track::handle_type,
                        snemo::datamodel::particle_track::handle_type> particle_pair_type;

      /// Typedef for TOF dictionnary
      typedef std::map<particle_pair_type, TOF_measurement> TOF_dict_type;

      // /// Typedef for TOF dictionnary
      // typedef std::map<particle_pair_type, delta_vertices_measurement> delta_vertices_dict_type;

      /// Return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Constructor
      topology_2e1g_pattern();

      /// Destructor
      virtual ~topology_2e1g_pattern();

      // /// Check internal probability validity
      // bool has_internal_probability() const;

      // /// Set internal probability
      // void set_internal_probability(const std::vector<double> prob_);

      // /// Return internal probability
      // std::vector<double> get_internal_probability() const;

      // /// Check internal probability validity
      // bool has_external_probability() const;

      // /// Set external probability
      // void set_external_probability(const std::vector<double> prob_);

      // /// Return external probability
      // std::vector <double> get_external_probability() const;

      /// Check delta vertices y validity
      bool has_delta_vertices_y() const;

      /// Set delta vertices y
      void set_delta_vertices_y(const double & delta_vertices_y_);

      /// Return delta vertices y
      double get_delta_vertices_y() const;

      /// Check delta vertices z validity
      bool has_delta_vertices_z() const;

      /// Set delta vertices z
      void set_delta_vertices_z(const double & delta_vertices_z_);

      /// Return delta vertices z
      double get_delta_vertices_z() const;

      /// Check angle validity
      bool has_angle() const;

      /// Set angle
      void set_angle(const double & angle_);

      /// Return internal probability
      double get_angle() const;

      /// Return a non mutable reference to the TOF dictionary
      const TOF_dict_type & get_TOF_dict() const;

      /// Return a mutable reference to the TOF dictionary
      TOF_dict_type & grab_TOF_dict();

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      // TOF_measurement _tof_;
      delta_vertices_measurement _DeltaV_;
      angle_measurement _angle_;
      TOF_dict_type _tof_dict_;

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_2e1g_pattern,
                        "snemo::datamodel::topology_2e1g_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E1G_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
