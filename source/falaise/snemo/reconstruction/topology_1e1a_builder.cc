/** \file falaise/snemo/datamodels/topology_1e1a_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_1e1a_builder.h>
#include <falaise/snemo/reconstruction/angle_driver.h>
#include <falaise/snemo/reconstruction/vertex_driver.h>
#include <falaise/snemo/datamodels/topology_1e1a_pattern.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/vertex_measurement.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_1e1a_builder,
                                                                    "snemo::reconstruction::topology_1e1a_builder")

    snemo::datamodel::base_topology_pattern::handle_type topology_1e1a_builder::_create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_1e1a_pattern);
      return h;
    }

    void topology_1e1a_builder::_build_measurement_dictionary(snemo::datamodel::base_topology_pattern & pattern_)
    {
      snemo::reconstruction::topology_1e_builder::_build_measurement_dictionary(pattern_);

      const std::string e1_label = "e1";
      DT_THROW_IF(! pattern_.has_particle_track(e1_label), std::logic_error,
                  "No particle with label '" << e1_label << "' has been stored !");
      const snemo::datamodel::particle_track & e1 = pattern_.get_particle_track(e1_label);

      const std::string a1_label = "a1";
      DT_THROW_IF(! pattern_.has_particle_track(a1_label), std::logic_error,
                  "No particle with label '" << a1_label << "' has been stored !");
      const snemo::datamodel::particle_track & a1 = pattern_.get_particle_track(a1_label);

      snemo::datamodel::base_topology_pattern::measurement_dict_type & meas
        = pattern_.grab_measurement_dictionary();
      const snemo::reconstruction::measurement_drivers & drivers
        = base_topology_builder::get_measurement_drivers();
      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas["angle_" + a1_label].reset(ptr_angle);
        if (drivers.AMD) drivers.AMD->process(a1, *ptr_angle);
      }

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas["angle_" + e1_label + "_" + a1_label].reset(ptr_angle);
        if (drivers.AMD) drivers.AMD->process(e1, a1, *ptr_angle);
      }

      {
        snemo::datamodel::vertex_measurement * ptr_vertex_measurement = new snemo::datamodel::vertex_measurement;
        meas["vertex_" + e1_label + "_" + a1_label].reset(ptr_vertex_measurement);
        if (drivers.VD) drivers.VD->process(e1, a1, *ptr_vertex_measurement);
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
