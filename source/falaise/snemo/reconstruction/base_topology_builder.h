/// \file falaise/snemo/datamodels/base_topology_builder.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-16
 * Last modified: 2015-10-16
 *
 * Description: The base class to build topology pattern
 */

#ifndef FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_BUILDER_H
#define FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_BUILDER_H 1

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/factory_macros.h>
#include <bayeux/datatools/handle.h>

// This project:
#include <falaise/snemo/reconstruction/topology_driver.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/TOF_measurement.h>
#include <falaise/snemo/datamodels/delta_vertices_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/energy_measurement.h>

namespace snemo {

  // Forward declaration
  namespace datamodel {
    class particle_track_data;
  }

  namespace reconstruction {

    /// \brief The base class to build topology pattern
    class base_topology_builder
    {
    public:

      ///
      void set_measurement_drivers(measurement_drivers &);

      ///
      const measurement_drivers & get_measurement_drivers() const;

      ///
      virtual snemo::datamodel::base_topology_pattern::handle_type create_pattern() = 0;

      ///
      virtual void build(const snemo::datamodel::particle_track_data & source_,
                         snemo::datamodel::base_topology_pattern & pattern_);

      virtual void build_measurement_dictionary(const snemo::datamodel::particle_track_data & source_,
                                                snemo::datamodel::base_topology_pattern & pattern_) = 0;

    protected:

      void _build_particle_tracks_dictionary(const snemo::datamodel::particle_track_data & source_,
                                             snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & tracks_);

    private:

      measurement_drivers * _drivers_;

      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_topology_builder);

    };
  }
}

// Interface macro for automated registration of a topology builder class in the global register
#define FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_INTERFACE(BuilderType) \
  public:                                                                            \
  virtual std::string get_type_id() const;                                           \
  private:                                                                           \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::snemo::reconstruction::base_topology_builder, BuilderType);

// Implementation macro for automated registration of a topology builder in the global register
#define FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(BuilderType,BuilderID) \
  std::string BuilderType::get_type_id() const { return BuilderID; }                           \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::snemo::reconstruction::base_topology_builder, BuilderType, BuilderID);

#endif // FALAISE_SNEMO_DATAMODEL_BASE_TOPOLOGY_BUILDER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
