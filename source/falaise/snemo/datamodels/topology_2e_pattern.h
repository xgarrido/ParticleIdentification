/// \file falaise/snemo/datamodels/topology_2e_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The 2e class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/topology_data.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 2e class of reconstructed topology
    class topology_2e_pattern : public base_topology_pattern
    {
    public:

      struct TOF_measurement
      {
        double internal_probability;
        double external_probability;
      };

      struct delta_vertices_measurement
      {
        double delta_vertices_y;
        double delta_vertices_z;
      };

      // /// Typedef for pairing particles
      // typedef std::pair<snemo::datamodel::particle_track::handle_type,
      //                   snemo::datamodel::particle_track::handle_type> particle_pair_type;

      // /// Typedef for TOF dictionnary
      // typedef std::map<particle_pair_type, TOF_measurement> TOF_dict_type;

      // /// Typedef for TOF dictionnary
      // typedef std::map<particle_pair_type, delta_vertices_measurement> delta_vertices_dict_type;

      /// Return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Constructor
      topology_2e_pattern();

      /// Destructor
      virtual ~topology_2e_pattern();

      /// Check internal probability validity
      bool has_internal_probability() const;

      /// Set internal probability
      void set_internal_probability(const double prob_);

      /// Return internal probability
      double get_internal_probability() const;

      /// Check internal probability validity
      bool has_external_probability() const;

      /// Set external probability
      void set_external_probability(const double prob_);

      /// Return external probability
      double get_external_probability() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      TOF_measurement _tof_;

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
