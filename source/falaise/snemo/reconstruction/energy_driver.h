/** \file falaise/snemo/reconstruction/energy_driver.h
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

#ifndef FALAISE_ENERGY_PLUGIN_SNEMO_RECONSTRUCTION_ENERGY_DRIVER_H
#define FALAISE_ENERGY_PLUGIN_SNEMO_RECONSTRUCTION_ENERGY_DRIVER_H 1

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/logger.h>

namespace snemo {

  namespace datamodel {
    class particle_track;
  }

  namespace reconstruction {

    /// Driver for the gamma clustering algorithms
    class energy_driver
    {
    public:

      /// Dedicated driver id
      static const std::string & get_id();

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      /// Constructor
      energy_driver();

      /// Destructor
      ~energy_driver();

      /// Main process
      void process(const snemo::datamodel::particle_track & pt_, double & energy_);

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

      /// Special method to process and generate particle track data
      void _process_algo(const snemo::datamodel::particle_track & pt_, double & energy_);

      /// Give default values to specific class members.
      void _set_defaults();

    private:
      bool                        _initialized_;      //!< Initialization status
      datatools::logger::priority _logging_priority_; //!< Logging priority
    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snemo::reconstruction::energy_driver)

#endif // FALAISE_ENERGY_PLUGIN_SNEMO_RECONSTRUCTION_ENERGY_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
