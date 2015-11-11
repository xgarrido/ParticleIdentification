/** \file falaise/snemo/reconstruction/topology_driver.h
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

#ifndef FALAISE_TOPOLOGY_PLUGIN_SNEMO_RECONSTRUCTION_TOPOLOGY_DRIVER_H
#define FALAISE_TOPOLOGY_PLUGIN_SNEMO_RECONSTRUCTION_TOPOLOGY_DRIVER_H 1

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>

// - Bayeux/datatools:
#include <datatools/logger.h>

namespace snemo {

  namespace datamodel {
    class particle_track_data;
    class topology_data;
  }

  namespace reconstruction {

    // Forward declaration
    class tof_driver;
    class vertex_driver;
    class angle_driver;
    class energy_driver;

    struct measurement_drivers {
      boost::scoped_ptr<snemo::reconstruction::tof_driver> TOFD;
      boost::scoped_ptr<snemo::reconstruction::vertex_driver> VD;
      boost::scoped_ptr<snemo::reconstruction::angle_driver> AMD;
      boost::scoped_ptr<snemo::reconstruction::energy_driver> EMD;
    };

    /// \brief Driver for the topology algorithm
    class topology_driver
    {
    public:

      /// Algorithm id
      static const std::string & topology_id();

      /// Initialization flag
      void set_initialized(const bool initialized_);

      /// Getting initialization flag
      bool is_initialized() const;

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      /// Constructor
      topology_driver();

      /// Destructor
      virtual ~topology_driver();

      /// Initialize the gamma tracker through configuration properties
      virtual void initialize(const datatools::properties & setup_);

      /// Reset the clusterizer
      virtual void reset();

      /// Main tracker trajectory driver
      int process(const snemo::datamodel::particle_track_data & ptd_,
                  snemo::datamodel::topology_data & td_);

      /// OCD support:
      static void init_ocd(datatools::object_configuration_description & ocd_);


    protected:

      /// Set default values to class members
      void _set_defaults();

      /// Main identification method
      virtual int _process_algo(const snemo::datamodel::particle_track_data & ptd_,
                                snemo::datamodel::topology_data & td_);

    private:

      /// Build the topology builder class id
      std::string _get_builder_class_id_(const snemo::datamodel::particle_track_data & ptd_) const;

    private:

      bool _initialized_;                             //!< Initialize flag
      datatools::logger::priority _logging_priority_; //!< Logging priority
      measurement_drivers _drivers_;                  //!< Measurement drivers such as TOF...
    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snemo::reconstruction::topology_driver)

#endif // FALAISE_TOPOLOGY_PLUGIN_SNEMO_RECONSTRUCTION_TOPOLOGY_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
