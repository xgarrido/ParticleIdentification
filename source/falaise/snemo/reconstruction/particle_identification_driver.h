/** \file falaise/snemo/reconstruction/particle_identification_driver.h
 * Author(s)     : Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date : 2012-10-07
 * Last modified : 2014-02-09
 *
 * Copyright (C) 2012-2014 Xavier Garrido <garrido@lal.in2p3.fr>
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
 *   A driver class that wraps the GammaTracking algorithm.
 *
 * History:
 *
 */

#ifndef FALAISE_PARTICLE_IDENTIFICATION_PLUGIN_SNEMO_RECONSTRUCTION_PARTICLE_IDENTIFICATION_DRIVER_H
#define FALAISE_PARTICLE_IDENTIFICATION_PLUGIN_SNEMO_RECONSTRUCTION_PARTICLE_IDENTIFICATION_DRIVER_H 1

// Standard library:

// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/properties.h>

namespace snemo {

  namespace datamodel {
    class particle_track_data;
  }

  namespace reconstruction {

    /// Driver for the gamma tracking algorithms
    class particle_identification_driver
    {
    public:

      static const std::string & particle_identification_id();

      /// Initialization flag
      void set_initialized(const bool initialized_);

      /// Getting initialization flag
      bool is_initialized() const;

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      /// Constructor
      particle_identification_driver();

      /// Destructor
      virtual ~particle_identification_driver();

      /// Initialize the gamma tracker through configuration properties
      virtual void initialize(const datatools::properties & setup_);

      /// Reset the clusterizer
      virtual void reset();

      /// Main tracker trajectory driver
      int process(snemo::datamodel::particle_track_data & ptd_);

    protected:

      /// Set default values to class members
      void _set_defaults();

      /// Prepare data for processing
      virtual int _prepare_process(snemo::datamodel::particle_track_data & ptd_);

      /// Main identification method
      virtual int _process_algo(snemo::datamodel::particle_track_data & ptd_);

      /// Post-processing operation
      virtual int _post_process(snemo::datamodel::particle_track_data & ptd_);

    private:

      bool _initialized_;                                       //!< Initialize flag
      datatools::logger::priority _logging_priority_;           //!< Logging priority
    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snemo::reconstruction::particle_identification_driver)

#endif // FALAISE_PARTICLE_IDENTIFICATION_PLUGIN_SNEMO_RECONSTRUCTION_PARTICLE_IDENTIFICATION_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
