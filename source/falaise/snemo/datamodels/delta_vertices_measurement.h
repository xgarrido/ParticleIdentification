/// \file falaise/snemo/datamodels/delta_vertices_measurement.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The base class of topology measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H 1

// Standard library:
#include <string>

// This project
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace geomtools {
  class blur_spot;
}

namespace snemo {

  namespace datamodel {

    /// \brief The Delta Vertices measurement
    class delta_vertices_measurement : public base_topology_measurement {

    public:

      typedef std::pair<std::string, double> pair_common_vertices_probability;

      /// Constructor
      delta_vertices_measurement();

      /// Destructor
      ~delta_vertices_measurement();

      /// Check delta vertices blurspot validity
      bool is_valid() const;

      // /// Set delta vertices blurspot value
      // void set_delta_vertices(geomtools::blur_spot & delta_vertices_);

      // /// Get a non-mutable reference to delta vertices blurspot
      // const geomtools::blur_spot & get_delta_vertices() const;

      // /// Get a mutable reference to delta vertices blurspot
      // geomtools::blur_spot & grab_delta_vertices();

      /// Get a non-mutable reference to delta vertices blurspot
      const double & get_probability() const;

      /// Get a mutable reference to delta vertices blurspot
      double & grab_probability();

      const std::string & get_vertices_location() const;

      std::string & grab_vertices_location();

    private:

      // geomtools::blur_spot & _delta_vertices_;

      // double _probability_;

      pair_common_vertices_probability _vertices_probability_;

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
