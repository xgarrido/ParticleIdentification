/** \file falaise/snemo/datamodels/topology_2p_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2p_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/energy_measurement.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/vertex_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2p_pattern,
                                                      "snemo::datamodel::topology_2p_pattern")

    // static
    const std::string & topology_2p_pattern::pattern_id()
    {
      static const std::string _id("2p");
      return _id;
    }

    std::string topology_2p_pattern::get_pattern_id() const
    {
      return topology_2p_pattern::pattern_id();
    }

    topology_2p_pattern::topology_2p_pattern()
      : base_topology_pattern()
    {
      return;
    }

    topology_2p_pattern::~topology_2p_pattern()
    {
      return;
    }

    bool topology_2p_pattern::has_positrons_energy() const
    {
      return
        has_measurement_as<snemo::datamodel::energy_measurement>("energy_p1") &&
        has_measurement_as<snemo::datamodel::energy_measurement>("energy_p2");
    }

    bool topology_2p_pattern::has_positron_minimal_energy() const
    {
      return has_positrons_energy();
    }

    double topology_2p_pattern::get_positron_minimal_energy() const
    {
      DT_THROW_IF(! has_positron_minimal_energy(), std::logic_error, "No positron minimal energy measurement stored !");
      return std::min(get_measurement_as<snemo::datamodel::energy_measurement>("energy_p1").get_energy(),
                      get_measurement_as<snemo::datamodel::energy_measurement>("energy_p2").get_energy());
    }

    bool topology_2p_pattern::has_positron_maximal_energy() const
    {
      return has_positrons_energy();
    }

    double topology_2p_pattern::get_positron_maximal_energy() const
    {
      DT_THROW_IF(! has_positron_maximal_energy(), std::logic_error, "No positron maximal energy measurement stored !");
      return std::max(get_measurement_as<snemo::datamodel::energy_measurement>("energy_p1").get_energy(),
                      get_measurement_as<snemo::datamodel::energy_measurement>("energy_p2").get_energy());
    }

    double topology_2p_pattern::get_positrons_energy_sum() const
    {
      //Only one energy check
      DT_THROW_IF(! has_positron_maximal_energy(), std::logic_error, "No positron energy measurement stored !");
      return get_positron_minimal_energy() + get_positron_maximal_energy();
    }

    double topology_2p_pattern::get_positrons_energy_difference() const
    {
      //Only one energy check
      DT_THROW_IF(! has_positron_maximal_energy(), std::logic_error, "No positron energy measurement stored !");
      return get_positron_maximal_energy() - get_positron_minimal_energy();
    }

    std::string topology_2p_pattern::get_minimal_energy_positron_name() const
    {
      if (get_measurement_as<snemo::datamodel::energy_measurement>("energy_p1").get_energy() <
          get_measurement_as<snemo::datamodel::energy_measurement>("energy_p2").get_energy())
        return "p1";
      else
        return "p2";
    }

    std::string topology_2p_pattern::get_maximal_energy_positron_name() const
    {
      if (get_measurement_as<snemo::datamodel::energy_measurement>("energy_p1").get_energy() <
          get_measurement_as<snemo::datamodel::energy_measurement>("energy_p2").get_energy())
        return "p2";
      else
        return "p1";
    }

    bool topology_2p_pattern::has_positrons_internal_probability() const
    {
      return has_measurement_as<snemo::datamodel::tof_measurement>("tof_p1_p2");
    }

    double topology_2p_pattern::get_positrons_internal_probability() const
    {
      DT_THROW_IF(! has_positrons_internal_probability(), std::logic_error, "No positrons TOF measurement stored !");
      return get_measurement_as<snemo::datamodel::tof_measurement>("tof_p1_p2").get_internal_probabilities().front();
    }

    bool topology_2p_pattern::has_positrons_external_probability() const
    {
      return has_measurement_as<snemo::datamodel::tof_measurement>("tof_p1_p2");
    }

    double topology_2p_pattern::get_positrons_external_probability() const
    {
      DT_THROW_IF(! has_positrons_external_probability(), std::logic_error, "No positrons TOF measurement stored !");
      return get_measurement_as<snemo::datamodel::tof_measurement>("tof_p1_p2").get_external_probabilities().front();
    }

    bool topology_2p_pattern::has_positrons_angle() const
    {
      return has_measurement_as<snemo::datamodel::angle_measurement>("angle_p1_p2");
    }

    double topology_2p_pattern::get_positrons_angle() const
    {
      DT_THROW_IF(! has_positrons_angle(), std::logic_error, "No positrons angle measurement stored !");
      return get_measurement_as<snemo::datamodel::angle_measurement>("angle_p1_p2").get_angle();
    }

    bool topology_2p_pattern::has_positrons_vertices_probability() const
    {
      return has_measurement_as<snemo::datamodel::vertex_measurement>("vertex_p1_p2");
    }

    double topology_2p_pattern::get_positrons_vertices_probability() const
    {
      DT_THROW_IF(! has_positrons_vertices_probability(), std::logic_error, "No common positrons vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_p1_p2").get_probability();
    }

    bool topology_2p_pattern::has_positrons_vertices_distance() const
    {
      return has_measurement_as<snemo::datamodel::vertex_measurement>("vertex_p1_p2");
    }

    double topology_2p_pattern::get_positrons_vertices_distance_x() const
    {
      DT_THROW_IF(! has_positrons_vertices_distance(), std::logic_error, "No common positrons vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_p1_p2").get_vertices_distance_x();
    }

    double topology_2p_pattern::get_positrons_vertices_distance_y() const
    {
      DT_THROW_IF(! has_positrons_vertices_distance(), std::logic_error, "No common positrons vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_p1_p2").get_vertices_distance_y();
    }

    double topology_2p_pattern::get_positrons_vertices_distance_z() const
    {
      DT_THROW_IF(! has_positrons_vertices_distance(), std::logic_error, "No common positrons vertices measurement stored !");
      return get_measurement_as<snemo::datamodel::vertex_measurement>("vertex_p1_p2").get_vertices_distance_z();
    }

    bool topology_2p_pattern::has_positrons_vertex_location() const
    {
      return has_measurement_as<snemo::datamodel::vertex_measurement>("vertex_p1_p2");
    }

    std::string topology_2p_pattern::get_positrons_vertex_location() const
    {
      DT_THROW_IF(! has_positrons_vertex_location(), std::logic_error, "No common positrons vertices measurement stored !");
      return dynamic_cast<const snemo::datamodel::vertex_measurement&> (get_measurement("vertex_p1_p2")).get_location();
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
