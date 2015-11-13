/** \file falaise/snemo/reconstruction/tof_driver.h
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
 *   A driver class that wraps the Time-Of-Flight algorithm.
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
// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>

// Forward declaration
namespace geomtools {
  class blur_spot;
}

namespace snemo {

  namespace datamodel {
    class particle_track;
    class tof_measurement;
  }

  namespace reconstruction {

    /// Driver for the gamma clustering algorithms
    class tof_driver
    {
    public:

      /// Toolbox for TOF calculation
      struct tof_tool {

        /// Gives the energy of particle
        static double get_energy(const snemo::datamodel::particle_track & particle_);

        /// Gives the mass of the particle
        static double get_mass(const snemo::datamodel::particle_track & particle_);

        /// Returns the beta
        static double beta(double energy_, double mass_);

        /// Gives the theoretical time of the track
        static double get_theoretical_time(double energy_, double mass_, double track_length_);

        /// Gives the times for two charged particles (single deposit)
        static void get_time(const snemo::datamodel::particle_track & particle_,
                             double & t_, double & sigma_t);

        /// Gives the track length of an electron
        static double get_charged_particle_track_length(const snemo::datamodel::particle_track & particle_);

        /// Gives the track length of a gamma from the electron vertex
        static double get_gamma_track_length(const snemo::datamodel::particle_track & gamma_,
                                             const snemo::datamodel::particle_track & electron_,
                                             const bool external_hyp_ = false);

        static datatools::logger::priority logging; //!< Internal logging priority
      };

      /// Dedicated driver id
      static const std::string & get_id();

      /// Constructor
      tof_driver();

      /// Destructor
      ~tof_driver();

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      /// Check if the driver is initialized
      bool is_initialized() const;

      /// Initialize the driver through configuration properties
      void initialize(const datatools::properties & setup_);

      /// Main process
      void process(const snemo::datamodel::particle_track & pt1_,
                   const snemo::datamodel::particle_track & pt2_,
                   snemo::datamodel::tof_measurement & tof_);

      /// Reset the driver
      void reset();

      /// OCD support:
      static void init_ocd(datatools::object_configuration_description & ocd_);

    protected:

      /// Set the initialization flag
      void _set_initialized(bool);

      /// Give default values to specific class members.
      void _set_defaults ();

      /// Main method to process particles and to retrieve internal/external TOF probabilities
      void _process_algo(const snemo::datamodel::particle_track & pt1_,
                         const snemo::datamodel::particle_track & pt2_,
                         std::vector<double> & proba_int_, std::vector<double> & proba_ext_);

      /// Special method to process charged particles
      void _process_charged_particles(const snemo::datamodel::particle_track & pt1_,
                                      const snemo::datamodel::particle_track & pt2_,
                                      std::vector<double> & proba_int_, std::vector<double> & proba_ext_);

      /// Special method to process gamma particles
      void _process_charged_gamma_particles(const snemo::datamodel::particle_track & pt1_,
                                            const snemo::datamodel::particle_track & pt2_,
                                            std::vector<double> & proba_int_, std::vector<double> & proba_ext_);
    private:

      /// Special internal method to extract gamma information (track length,
      /// time) given a charged particle track
      void _get_vertex_to_calo_info_(const snemo::datamodel::particle_track & ptc_,
                                     const snemo::datamodel::particle_track & ptg_,
                                     const geomtools::blur_spot & vertex_,
                                     double & track_length_, double & time_, double & sigma_time_);

    private:
      bool _initialized_;                             //!< Initialization status
      datatools::logger::priority _logging_priority_; //!< Logging priority
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
