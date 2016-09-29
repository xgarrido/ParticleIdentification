/** \file falaise/snemo/reconstruction/vertex_driver.h
 * Author(s)     : Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date : 2015-04-01
 * Last modified : 2015-04-01
 *
 * Copyright (C) 2015 Steven Calvez <calvez@lal.in2p3.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Description:
 *
 *   A driver class that wraps the measurement of delta vertices algorithm.
 *
 * History:
 *
 */

#ifndef FALAISE_VERTEX_PLUGIN_SNEMO_RECONSTRUCTION_VERTEX_DRIVER_H
#define FALAISE_VERTEX_PLUGIN_SNEMO_RECONSTRUCTION_VERTEX_DRIVER_H 1

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>

// Forward declaration
namespace geomtools {
  class blur_spot;
}

namespace snemo {

  namespace datamodel {
    class particle_track;
    class vertex_measurement;
  }

  namespace reconstruction {

    /// Driver for the gamma clustering algorithms
    class vertex_driver
    {
    public:

      /// Dedicated driver id
      static const std::string & get_id();

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      /// Constructor
      vertex_driver();

      /// Destructor
      ~vertex_driver();

      /// Main process for single particle
      void process(const snemo::datamodel::particle_track & pt_,
                   snemo::datamodel::vertex_measurement & vertex_);

      /// Main process for two particles
      void process(const snemo::datamodel::particle_track & pt1_,
                   const snemo::datamodel::particle_track & pt2_,
                   snemo::datamodel::vertex_measurement & vertex_);

      /// Check if theclusterizer is initialized
      bool is_initialized() const;

      /// Initialize the gamma tracker through configuration properties
      void initialize(const datatools::properties & setup_);

      /// Reset the clusterizer
      void reset();

      /// OCD support:
      static void init_ocd(datatools::object_configuration_description & ocd_);

    protected:

      /// Set the initialization flag
      void _set_initialized(bool);

      /// Give default values to specific class members.
      void _set_defaults();

      /// Special method to process and determine single particle vertex
      void _process_algo(const snemo::datamodel::particle_track & pt_,
                         snemo::datamodel::vertex_measurement & vertex_);

      /// Special method to process and determine common vertex between particle tracks
      void _process_algo(const snemo::datamodel::particle_track & pt1_,
                         const snemo::datamodel::particle_track & pt2_,
                         snemo::datamodel::vertex_measurement & vertex_);

      /// Find the common vertex between two vertices
      void _find_common_vertex(const geomtools::blur_spot & vtx1_,
                               const geomtools::blur_spot & vtx2_,
                               snemo::datamodel::vertex_measurement & vertex_);

    private:
      bool                        _initialized_;      //!< Initialization status
      datatools::logger::priority _logging_priority_; //!< Logging priority
    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snemo::reconstruction::vertex_driver)

#endif // FALAISE_VERTEX_PLUGIN_SNEMO_RECONSTRUCTION_VERTEX_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
