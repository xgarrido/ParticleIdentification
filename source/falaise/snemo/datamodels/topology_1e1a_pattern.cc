/** \file falaise/snemo/datamodels/topology_1e1a_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1a_pattern.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/delta_vertices_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e1a_pattern,
                                                      "snemo::datamodel::topology_1e1a_pattern")

    // static
    const std::string & topology_1e1a_pattern::pattern_id()
    {
      static const std::string _id("1e1a");
      return _id;
    }

    std::string topology_1e1a_pattern::get_pattern_id() const
    {
      return topology_1e1a_pattern::pattern_id();
    }

    topology_1e1a_pattern::topology_1e1a_pattern()
      : topology_1e_pattern()
    {
      return;
    }

    topology_1e1a_pattern::~topology_1e1a_pattern()
    {
      return;
    }

    bool topology_1e1a_pattern::has_alpha_angle() const
    {
      return has_measurement("angle_a1");
    }

    double topology_1e1a_pattern::get_alpha_angle() const
    {
      DT_THROW_IF(! has_alpha_angle(), std::logic_error, "No alpha angle measurement stored !");
      return dynamic_cast<const snemo::datamodel::angle_measurement&> (get_measurement("angle_a1")).get_angle();
    }

    bool topology_1e1a_pattern::has_electron_alpha_angle() const
    {
      return has_measurement("angle_e1_a1");
    }

    double topology_1e1a_pattern::get_electron_alpha_angle() const
    {
      DT_THROW_IF(! has_electron_alpha_angle(), std::logic_error, "No electron-alpha angle measurement stored !");
      return dynamic_cast<const snemo::datamodel::angle_measurement&> (get_measurement("angle_e1_a1")).get_angle();
    }

    bool topology_1e1a_pattern::has_electron_alpha_vertices_probability() const
    {
      return has_measurement("vertices_probability_e1_a1");
    }

    double topology_1e1a_pattern::get_electron_alpha_vertices_probability() const
    {
      DT_THROW_IF(! has_electron_alpha_vertices_probability(), std::logic_error, "No common electron-alpha vertices measurement stored !");
      return dynamic_cast<const snemo::datamodel::delta_vertices_measurement&> (get_measurement("vertices_probability_e1_a1")).get_probability();
    }

    // delta_vertices

    // double topology_1e1a_pattern::get_alpha_delayed_time() const
    // {
    //   double time = datatools::invalid_real();
    //   if (has_alpha_particle()) {
    //     const snemo::datamodel::particle_track & the_alpha = get_alpha_particle();
    //     if (the_alpha.has_trajectory()) {
    //       const snemo::datamodel::tracker_trajectory & a_trajectory = the_alpha.get_trajectory();
    //       const datatools::properties & aux = a_trajectory.get_auxiliaries();
    //       if (aux.has_key("t0")) {
    //         time = aux.fetch_real("t0");
    //       }
    //     }
    //   }
    //   return time;
    // }

    // double topology_1e1a_pattern::get_alpha_track_length() const
    // {
    //   double length = datatools::invalid_real();
    //   if (has_alpha_particle()) {
    //     const snemo::datamodel::particle_track & the_alpha = get_alpha_particle();
    //     if (the_alpha.has_trajectory()) {
    //       const snemo::datamodel::tracker_trajectory & a_trajectory = the_alpha.get_trajectory();
    //       const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
    //       length = a_track_pattern.get_shape().get_length();
    //     }
    //   }
    //   return length;
    // }

    // double topology_1e1a_pattern::get_electron_track_length() const
    // {
    //   double length = datatools::invalid_real();
    //   if (has_electron_particle()) {
    //     const snemo::datamodel::particle_track & the_electron = get_electron_particle();
    //     if (the_electron.has_trajectory()) {
    //       const snemo::datamodel::tracker_trajectory & a_trajectory = the_electron.get_trajectory();
    //       const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
    //       length = a_track_pattern.get_shape().get_length();
    //     }
    //   }
    //   return length;
    // }

  } // end of namespace datamodel

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
