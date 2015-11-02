/** \file falaise/snemo/datamodels/topology_1eNg_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_1eNg_builder.h>
#include <falaise/snemo/reconstruction/tof_driver.h>
#include <falaise/snemo/reconstruction/angle_driver.h>
#include <falaise/snemo/reconstruction/energy_driver.h>
#include <falaise/snemo/datamodels/topology_1eNg_pattern.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/energy_measurement.h>
#include <falaise/snemo/datamodels/pid_utils.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_1eNg_builder,
                                                                    "snemo::reconstruction::topology_1eNg_builder");

    snemo::datamodel::base_topology_pattern::handle_type topology_1eNg_builder::_create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_1eNg_pattern);
      return h;
    }

    void topology_1eNg_builder::_build_measurement_dictionary(snemo::datamodel::base_topology_pattern & pattern_)
    {
      snemo::reconstruction::topology_1e_builder::_build_measurement_dictionary(pattern_);

      const std::string e1_label = "e1";
      DT_THROW_IF(! pattern_.has_particle_track(e1_label), std::logic_error,
                  "No particle with label '" << e1_label << "' has been stored !");
      const snemo::datamodel::particle_track & e1 = pattern_.get_particle_track(e1_label);

      // const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
      //   = ptd_.get_particles();

      const int ngammas = pattern_.get_particle_track_dictionary().size()-1;

      dynamic_cast<snemo::datamodel::topology_1eNg_pattern &>(pattern_).set_number_of_gammas(ngammas);

      // Either this way
      // for (size_t i_gamma = 1; i_gamma <= h_pattern_->get_number_of_gammas();++i_gamma) {
      // Or this way but less evolutive
      for (size_t i_gamma = 1; i_gamma <= ngammas;++i_gamma) {
        std::ostringstream oss;
        oss << "g" << i_gamma;

        const std::string g_label = oss.str();
        DT_THROW_IF(! pattern_.has_particle_track(g_label), std::logic_error,
                    "No particle with label '" << g_label << "' has been stored !");
        const snemo::datamodel::particle_track & gamma = pattern_.get_particle_track(g_label);
        snemo::datamodel::base_topology_pattern::measurement_dict_type & meas
          = pattern_.grab_measurement_dictionary();
        const snemo::reconstruction::measurement_drivers & drivers
          = base_topology_builder::get_measurement_drivers();
        {
          snemo::datamodel::tof_measurement * ptr_tof = new snemo::datamodel::tof_measurement;
          meas["tof_e1_" + g_label].reset(ptr_tof);
          if (drivers.TOFD) drivers.TOFD->process(e1, gamma,
                                                  ptr_tof->grab_internal_probabilities(),
                                                  ptr_tof->grab_external_probabilities());
        }

        {
          snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
          meas["angle_e1_" + g_label].reset(ptr_angle);
          if (drivers.AMD) drivers.AMD->process(e1, gamma, ptr_angle->grab_angle());
        }

        {
          snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
          meas["energy_" + g_label].reset(ptr_energy);
          if (drivers.EMD) drivers.EMD->process(gamma, ptr_energy->grab_energy());
        }
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
