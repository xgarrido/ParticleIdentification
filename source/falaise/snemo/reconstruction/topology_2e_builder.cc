/** \file falaise/snemo/datamodels/topology_2e_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_2e_builder.h>
#include <falaise/snemo/reconstruction/tof_driver.h>
#include <falaise/snemo/reconstruction/delta_vertices_driver.h>
#include <falaise/snemo/reconstruction/angle_driver.h>
#include <falaise/snemo/reconstruction/energy_driver.h>
#include <falaise/snemo/datamodels/topology_2e_pattern.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/delta_vertices_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/energy_measurement.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_2e_builder,
                                                                    "snemo::reconstruction::topology_2e_builder");

    snemo::datamodel::base_topology_pattern::handle_type topology_2e_builder::_create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_2e_pattern);
      return h;
    }

    void topology_2e_builder::_build_measurement_dictionary(snemo::datamodel::base_topology_pattern & pattern_)
    {
      const std::string e1_label = "e1";
      DT_THROW_IF(! pattern_.has_particle_track(e1_label), std::logic_error,
                  "No particle with label '" << e1_label << "' has been stored !");
      const snemo::datamodel::particle_track & e1 = pattern_.get_particle_track(e1_label);

      const std::string e2_label = "e2";
      DT_THROW_IF(! pattern_.has_particle_track(e2_label), std::logic_error,
                  "No particle with label '" << e2_label << "' has been stored !");
      const snemo::datamodel::particle_track & e2 = pattern_.get_particle_track(e2_label);

      snemo::datamodel::base_topology_pattern::measurement_dict_type & meas
        = pattern_.grab_measurement_dictionary();
      const snemo::reconstruction::measurement_drivers & drivers
        = base_topology_builder::get_measurement_drivers();
      {
        snemo::datamodel::tof_measurement * ptr_tof = new snemo::datamodel::tof_measurement;
        meas["tof_" + e1_label + "_" + e2_label].reset(ptr_tof);
        if (drivers.TOFD) drivers.TOFD->process(e1, e2,
                                                ptr_tof->grab_internal_probabilities(),
                                                ptr_tof->grab_external_probabilities());
      }

      {
        snemo::datamodel::delta_vertices_measurement * ptr_delta_vertices_source = new snemo::datamodel::delta_vertices_measurement;
        meas["delta_vertices_source_" + e1_label + "_" + e2_label].reset(ptr_delta_vertices_source);
        if (drivers.DVD) drivers.DVD->process(e1, e2,
                                              ptr_delta_vertices_source->grab_delta_vertices_y(),
                                              ptr_delta_vertices_source->grab_delta_vertices_z());
      }

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas["angle_" + e1_label + "_" + e2_label].reset(ptr_angle);
        if (drivers.AMD) drivers.AMD->process(e1, e2, ptr_angle->grab_angle());
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas["energy_" + e1_label].reset(ptr_energy);
        if (drivers.EMD) drivers.EMD->process(e1, ptr_energy->grab_energy());
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas["energy_" + e2_label].reset(ptr_energy);
        if (drivers.EMD) drivers.EMD->process(e2, ptr_energy->grab_energy());
      }

      return;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
