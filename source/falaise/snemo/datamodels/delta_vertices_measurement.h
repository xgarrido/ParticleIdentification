/// \file falaise/snemo/datamodels/delta_vertices_measurement.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-24
 * Last modified: 2015-10-24
 *
 * Description: The class for delta vertices measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H 1

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

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::delta_vertices_measurement,
                        "snemo::datamodel::delta_vertices_measurement")

#endif // FALAISE_SNEMO_DATAMODEL_DELTA_VERTICES_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
