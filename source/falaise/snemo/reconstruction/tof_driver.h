/** \file falaise/snemo/reconstruction/tof_driver.h
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
 *   A driver class that wraps the GammaClustering algorithm.
 *
 * History:
 *
 */

#ifndef FALAISE_TOF_PLUGIN_SNEMO_RECONSTRUCTION_TOF_DRIVER_H
#define FALAISE_TOF_PLUGIN_SNEMO_RECONSTRUCTION_TOF_DRIVER_H 1

// Standard library:
#include <string>
#include <list>
#include <map>
#include <vector>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>

// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/properties.h>

// This project:
#include <falaise/snemo/datamodels/particle_track.h>

namespace geomtools {
  class manager;
}

namespace snemo {

  namespace datamodel {
    class particle_track_data;
  }

  namespace datamodel {
    class topology_data;
  }

  namespace geometry {
    class locator_plugin;
  }

  namespace reconstruction {

    /// Driver for the gamma clustering algorithms
    class tof_driver
    {
    public:

      static const std::string & tof_id();

      /// Constructor
      tof_driver();

      /// Destructor
      ~tof_driver();

      /// Check if theclusterizer is initialized
      bool is_initialized() const;

      /// Initialize the gamma tracker through configuration properties
      void initialize(const datatools::properties & setup_);

      /// Reset the clusterizer
      void reset();

    protected:

      /// Set the initialization flag
      void _set_initialized(bool);

      /// Special method to process and generate particle track data
      int _process_algo(double & proba_,
                        snemo::datamodel::particle_track & particle_1_,
                        snemo::datamodel::particle_track & particle_2_);

      /// Gives the mass of the particle_
      double _get_mass(snemo::datamodel::particle_track & particle_);

      /// Gives the track length of the particle_
      double _get_track_length(snemo::datamodel::particle_track & particle_);

      /// Give default values to specific class members.
      void _set_defaults ();

    private:
      bool _initialized_;            //!< Initialization status
      double _sigma_t_gamma_interaction_uncertainty_;     //!< The uncertainty on the track length
      datatools::properties _tof_setup_;                         //!< The Gamma Clustering parameters
      // for members
    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snemo::reconstruction::tof_driver)

#endif // FALAISE_TOF_PLUGIN_SNEMO_RECONSTRUCTION_TOF_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
