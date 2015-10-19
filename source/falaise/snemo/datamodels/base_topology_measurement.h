/// \file falaise/snemo/datamodels/base_topology_measurement.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The base class of topology measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_MEASUREMENT_H 1

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>

namespace snemo {

  namespace datamodel {

    /// \brief The base class of reconstructed topology
    class base_topology_measurement : DATATOOLS_SERIALIZABLE_CLASS,
                                      public datatools::i_tree_dumpable
    {
    public:

      /// Constructor
      base_topology_measurement();

      /// Destructor
      virtual ~base_topology_measurement();

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

    /// \brief The Time Of Flight measurement
    class TOF_measurement : DATATOOLS_SERIALIZABLE_CLASS,
                            public base_topology_measurement {

    public:

      /// Typedef for probability type
      typedef std::vector<double> probability_type;

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

    private:

      probability_type _internal_probabilities_;
      probability_type _external_probabilities_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

    /// \brief The Delta Vertices measurement
    class delta_vertices_measurement : DATATOOLS_SERIALIZABLE_CLASS,
                            public base_topology_measurement {

    public:

      /// Constructor
      delta_vertices_measurement();

      /// Destructor
      ~delta_vertices_measurement();

      /// Check y-delta vertices
      bool has_delta_vertices_y() const;

      /// Set y-delta vertices
      void set_delta_vertices_y(double delta_);

      /// Get a non-mutable reference to delta vertices y
      const double & get_delta_vertices_y() const;

      /// Get a mutable reference to delta vertices y
      double & grab_delta_vertices_y();

      /// Check z-delta vertices
      bool has_delta_vertices_z() const;

      /// Set z-delta vertices
      void set_delta_vertices_z(double delta_);

      /// Get a non-mutable reference to delta vertices z
      const double & get_delta_vertices_z() const;

      /// Get a mutable reference to delta vertices z
      double & grab_delta_vertices_z();

    private:

      double _delta_vertices_y_;
      double _delta_vertices_z_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };


    /// \brief The Angle measurement
    class angle_measurement : DATATOOLS_SERIALIZABLE_CLASS,
                              public base_topology_measurement {
    public:

      /// Constructor
      angle_measurement();

      /// Destructor
      ~angle_measurement();

      /// Check angle validity
      bool has_angle() const;

      /// Set angle value
      void set_angle(double angle_);

      /// Get a non-mutable reference to angle
      const double & get_angle() const;

      /// Get a mutable reference to angle
      double & grab_angle();

    private:

      double _angle_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

    /// \brief The Energy measurement
    class energy_measurement : DATATOOLS_SERIALIZABLE_CLASS,
                              public base_topology_measurement {
    public:

      /// Constructor
      energy_measurement();

      /// Destructor
      ~energy_measurement();

      /// Check energy validity
      bool has_energy() const;

      /// Set energy value
      void set_energy(double energy_);

      /// Get a non-mutable reference to energy
      const double & get_energy() const;

      /// Get a mutable reference to energy
      double & grab_energy();

    private:

      double _energy_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
