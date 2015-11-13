/** \file falaise/snemo/reconstruction/angle_driver.h
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
 *   A driver class that wraps the angle measurement algorithm.
 *
 * History:
 *
 */

#ifndef FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_DRIVER_H
#define FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_DRIVER_H 1

// Standard library:
#include <string>
#include <vector>

// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>
// - Bayeux/geomtools:
#include <bayeux/geomtools/clhep.h>

namespace snemo {

  namespace datamodel {
    class particle_track;
    class angle_measurement;
  }

  namespace reconstruction {

    /// Driver for the angle measurement algorithms
    class angle_driver
    {
    public:

      /// Dedicated driver id
      static const std::string & get_id();

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      /// Constructor
      angle_driver();

      /// Destructor
      ~angle_driver();

      /// Check if the driver is initialized
      bool is_initialized() const;

      /// Initialize the driver through configuration properties
      void initialize(const datatools::properties & setup_);

      /// Main process for single particle angle measurement
      void process(const snemo::datamodel::particle_track & pt_,
                   snemo::datamodel::angle_measurement & angle_);

      /// Main process for angle between two particle tracks
      void process(const snemo::datamodel::particle_track & pt1_,
                   const snemo::datamodel::particle_track & pt2_,
                   snemo::datamodel::angle_measurement & angle_);

      /// Reset the driver
      void reset();

      /// OCD support:
      static void init_ocd(datatools::object_configuration_description & ocd_);

    protected:

      /// Set the initialization flag
      void _set_initialized(bool);

      /// Give default values to specific class members.
      void _set_defaults();

      /// Special method to process single particle track
      void _process_algo(const snemo::datamodel::particle_track & pt_,
                         double & angle_);

      /// Special method to process two particle tracks
      void _process_algo(const snemo::datamodel::particle_track & pt1_,
                         const snemo::datamodel::particle_track & pt2_,
                         double & angle_);

      /// Get direction of a particle track
      void _get_direction(const snemo::datamodel::particle_track & pt_,
                          geomtools::vector_3d & direction_);

    private:
      bool                        _initialized_;      //!< Initialization status
      datatools::logger::priority _logging_priority_; //!< Logging priority
    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snemo::reconstruction::angle_driver)

#endif // FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
