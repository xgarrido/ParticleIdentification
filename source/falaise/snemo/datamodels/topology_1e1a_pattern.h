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
#include <falaise/snemo/datamodels/topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 1e1a class of reconstructed topology
    class topology_1e1a_pattern : public base_topology_pattern
    {
    public:

      /// Return pattern identifier of the pattern
      static const std::string & pattern_id();

      /// Constructor
      topology_1e1a_pattern();

      /// Destructor
      virtual ~topology_1e1a_pattern();

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

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      delta_vertices_measurement _DeltaV_;
      angle_measurement _angle_;

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
