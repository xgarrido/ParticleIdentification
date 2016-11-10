/** \file falaise/snemo/datamodels/topology_1e1p_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1p_pattern.h>
#include <falaise/snemo/datamodels/energy_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/vertex_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e1p_pattern,
                                                      "snemo::datamodel::topology_1e1p_pattern")


    // static
    const std::string & topology_1e1p_pattern::pattern_id()
    {
      static const std::string _id("1e1p");
      return _id;
    }

    std::string topology_1e1p_pattern::get_pattern_id() const
    {
      return topology_1e1p_pattern::pattern_id();
    }

    topology_1e1p_pattern::topology_1e1p_pattern()
      : topology_1e_pattern()
    {
      return;
    }

    topology_1e1p_pattern::~topology_1e1p_pattern()
    {
      return;
    }

    bool topology_1e1p_pattern::has_positron_track() const
    {
      return has_particle_track("p1");
    }

    const snemo::datamodel::particle_track & topology_1e1p_pattern::get_positron_track() const
    {
      return get_particle_track("p1");
    }

    bool topology_1e1p_pattern::has_positron_energy() const
    {
      return has_measurement_as<snemo::datamodel::energy_measurement>("energy_p1");
    }

    double topology_1e1p_pattern::get_positron_energy() const
    {
      DT_THROW_IF(! has_positron_energy(), std::logic_error, "No positron energy measurement stored !");
      return get_measurement_as<snemo::datamodel::energy_measurement>("energy_p1").get_energy();
    }

    bool topology_1e1p_pattern::has_positron_angle() const
    {
      return has_measurement_as<snemo::datamodel::angle_measurement>("angle_p1");
    }

    double topology_1e1p_pattern::get_positron_angle() const
    {
      DT_THROW_IF(! has_positron_angle(), std::logic_error, "No positron angle measurement stored !");
      return get_measurement_as<snemo::datamodel::angle_measurement>("angle_p1").get_angle();
    }

    bool topology_1e1p_pattern::has_electron_positron_angle() const
    {
      return has_measurement_as<snemo::datamodel::angle_measurement>("angle_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_angle() const
    {
      DT_THROW_IF(! has_electron_positron_angle(), std::logic_error, "No electron-positron angle measurement stored !");
      return get_measurement_as<snemo::datamodel::angle_measurement>("angle_e1_p1").get_angle();
    }

    bool topology_1e1p_pattern::has_electron_positron_internal_probability() const
    {
      return has_measurement_as<snemo::datamodel::tof_measurement>("tof_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_internal_probability() const
    {
      DT_THROW_IF(! has_electron_positron_internal_probability(), std::logic_error, "No electron-positron TOF measurement stored !");
      return get_measurement_as<snemo::datamodel::tof_measurement>("tof_e1_p1").get_internal_probabilities().front();
    }

    bool topology_1e1p_pattern::has_electron_positron_external_probability() const
    {
      return has_measurement_as<snemo::datamodel::tof_measurement>("tof_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_external_probability() const
    {
      DT_THROW_IF(! has_electron_positron_external_probability(), std::logic_error, "No electron-positron TOF measurement stored !");
      return get_measurement_as<snemo::datamodel::tof_measurement>("tof_e1_p1").get_external_probabilities().front();
    }

    bool topology_1e1p_pattern::has_electron_positron_vertices_probability() const
    {
      return has_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_vertices_probability() const
    {
      DT_THROW_IF(! has_electron_positron_vertices_probability(), std::logic_error, "No common electron-positron vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1").get_probability();
    }

    bool topology_1e1p_pattern::has_electron_positron_vertices_distance() const
    {
      return has_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_vertices_distance_x() const
    {
      DT_THROW_IF(! has_electron_positron_vertices_distance(), std::logic_error, "No common electrons vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1").get_vertices_distance_x();
    }

    double topology_1e1p_pattern::get_electron_positron_vertices_distance_y() const
    {
      DT_THROW_IF(! has_electron_positron_vertices_distance(), std::logic_error, "No common electrons vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1").get_vertices_distance_y();
    }

    double topology_1e1p_pattern::get_electron_positron_vertices_distance_z() const
    {
      DT_THROW_IF(! has_electron_positron_vertices_distance(), std::logic_error, "No common electrons vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1").get_vertices_distance_z();
    }

    bool topology_1e1p_pattern::has_electron_positron_vertex_location() const
    {
      return has_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1");
    }

    std::string topology_1e1p_pattern::get_electron_positron_vertex_location() const
    {
      DT_THROW_IF(! has_electron_positron_vertex_location(), std::logic_error, "No common electron_positron vertices measurement stored !");
      return dynamic_cast<const snemo::datamodel::vertex_measurement&> (get_measurement("vertex_e1_p1")).get_location();
    }

    bool topology_1e1p_pattern::has_electron_positron_vertex_position() const
    {
      return has_measurement_as<snemo::datamodel::vertex_measurement>("vertex_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_vertex_position_x() const
    {
      DT_THROW_IF(! has_electron_positron_vertex_position(), std::logic_error, "No common electron_positron vertices measurement stored !");
      return dynamic_cast<const snemo::datamodel::vertex_measurement&> (get_measurement("vertex_e1_p1")).get_vertex_position_x();
    }

    double topology_1e1p_pattern::get_electron_positron_vertex_position_y() const
    {
      DT_THROW_IF(! has_electron_positron_vertex_position(), std::logic_error, "No common electron_positron vertices measurement stored !");
      return dynamic_cast<const snemo::datamodel::vertex_measurement&> (get_measurement("vertex_e1_p1")).get_vertex_position_y();
    }

    double topology_1e1p_pattern::get_electron_positron_vertex_position_z() const
    {
      DT_THROW_IF(! has_electron_positron_vertex_position(), std::logic_error, "No common electron_positron vertices measurement stored !");
      return dynamic_cast<const snemo::datamodel::vertex_measurement&> (get_measurement("vertex_e1_p1")).get_vertex_position_z();
    }

    bool topology_1e1p_pattern::has_electron_positron_minimal_energy() const
    {
      return has_measurement("energy_e1") && has_measurement("energy_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_minimal_energy() const
    {
      DT_THROW_IF(! has_electron_positron_minimal_energy(), std::logic_error, "No electron/positron minimal energy measurement stored !");
      //bien dégueulasse
      return std::min(dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e1")).get_energy(),
                      dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_p1")).get_energy());
    }

    bool topology_1e1p_pattern::has_electron_positron_maximal_energy() const
    {
      return
        has_measurement_as<snemo::datamodel::energy_measurement>("energy_e1") &&
        has_measurement_as<snemo::datamodel::energy_measurement>("energy_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_maximal_energy() const
    {
      DT_THROW_IF(! has_electron_positron_maximal_energy(), std::logic_error, "No electron/positron maximal energy measurement stored !");
      return std::max(get_measurement_as<snemo::datamodel::energy_measurement>("energy_e1").get_energy(),
                      get_measurement_as<snemo::datamodel::energy_measurement>("energy_p1").get_energy());
    }

    double topology_1e1p_pattern::get_positron_track_length() const
    {
      double length = datatools::invalid_real();
      if (has_positron_track()) {
        const snemo::datamodel::particle_track & the_positron = get_positron_track();
        if (the_positron.has_trajectory()) {
          const snemo::datamodel::tracker_trajectory & a_trajectory = the_positron.get_trajectory();
          const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
          length = a_track_pattern.get_shape().get_length();
        }
      }
      return length;
    }

  } // end of namespace datamodel

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
