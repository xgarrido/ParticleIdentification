/** \file falaise/snemo/datamodels/topology_2p_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_2p_builder.h>
#include <falaise/snemo/reconstruction/tof_driver.h>
#include <falaise/snemo/reconstruction/vertex_driver.h>
#include <falaise/snemo/reconstruction/angle_driver.h>
#include <falaise/snemo/reconstruction/energy_driver.h>
#include <falaise/snemo/datamodels/topology_2p_pattern.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/vertex_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/energy_measurement.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_2p_builder,
                                                                    "snemo::reconstruction::topology_2p_builder");

    snemo::datamodel::base_topology_pattern::handle_type topology_2p_builder::_create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_2p_pattern);
      return h;
    }

    void topology_2p_builder::_build_measurement_dictionary(snemo::datamodel::base_topology_pattern & pattern_)
    {
      const std::string p1_label = "p1";
      DT_THROW_IF(! pattern_.has_particle_track(p1_label), std::logic_error,
                  "No particle with label '" << p1_label << "' has been stored !");
      const snemo::datamodel::particle_track & p1 = pattern_.get_particle_track(p1_label);

      const std::string p2_label = "p2";
      DT_THROW_IF(! pattern_.has_particle_track(p2_label), std::logic_error,
                  "No particle with label '" << p2_label << "' has been stored !");
      const snemo::datamodel::particle_track & p2 = pattern_.get_particle_track(p2_label);

      snemo::datamodel::base_topology_pattern::measurement_dict_type & meas
        = pattern_.grab_measurement_dictionary();
      const snemo::reconstruction::measurement_drivers & drivers
        = base_topology_builder::get_measurement_drivers();
      {
        snemo::datamodel::tof_measurement * ptr_tof = new snemo::datamodel::tof_measurement;
        meas["tof_" + p1_label + "_" + p2_label].reset(ptr_tof);
        if (drivers.TOFD) drivers.TOFD->process(p1, p2, *ptr_tof);
      }

      {
        snemo::datamodel::vertex_measurement * ptr_vertex_measurement = new snemo::datamodel::vertex_measurement;
        meas["vertex_" + p1_label + "_" + p2_label].reset(ptr_vertex_measurement);
        if (drivers.VD) drivers.VD->process(p1, p2, *ptr_vertex_measurement);
      }

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas["angle_" + p1_label + "_" + p2_label].reset(ptr_angle);
        if (drivers.AMD) drivers.AMD->process(p1, p2, *ptr_angle);
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas["energy_" + p1_label].reset(ptr_energy);
        if (drivers.EMD) drivers.EMD->process(p1, *ptr_energy);
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas["energy_" + p2_label].reset(ptr_energy);
        if (drivers.EMD) drivers.EMD->process(p2, *ptr_energy);
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
