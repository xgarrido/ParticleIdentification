/** \file falaise/snemo/reconstruction/angle_measurement_driver.h
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

#ifndef FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_MEASUREMENT_DRIVER_H
#define FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_MEASUREMENT_DRIVER_H 1

// Standard library:
#include <string>
#include <vector>

// - Bayeux/datatools:
#include <datatools/logger.h>
// - Bayeux/geomtools:
#include <geomtools/clhep.h>

namespace snemo {

  namespace datamodel {
    class particle_track;
  }

  namespace reconstruction {

    /// Driver for the angle measurement algorithms
    class angle_measurement_driver
    {
    public:

      /// Dedicated driver id
      static const std::string & get_id();

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      /// Constructor
      angle_measurement_driver();

      /// Destructor
      ~angle_measurement_driver();

      /// Main process for single prticle track measurement
      int process(const snemo::datamodel::particle_track & pt_,
                  double & angle_);

      /// Main process for angle between two particle tracks
      int process(const snemo::datamodel::particle_track & pt1_,
                  const snemo::datamodel::particle_track & pt2_,
                  double & angle_);


      /// Check if the driver is initialized
      bool is_initialized() const;

      /// Initialize the driver through configuration properties
      void initialize(const datatools::properties & setup_);

      /// Reset the driver
      void reset();

    protected:

      /// Set the initialization flag
      void _set_initialized(bool);

      /// Give default values to specific class members.
      void _set_defaults ();

      /// Special method to process single particle track
      int _process_algo(const snemo::datamodel::particle_track & pt_,
                        double & angle);

      /// Special method to process two particle tracks
      int _process_algo(const snemo::datamodel::particle_track & pt1_,
                        const snemo::datamodel::particle_track & pt2_,
                        double & angle);

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
DOCD_CLASS_DECLARATION(snemo::reconstruction::angle_measurement_driver)

#endif // FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_MEASUREMENT_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
