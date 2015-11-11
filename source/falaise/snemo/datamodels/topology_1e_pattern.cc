/** \file falaise/snemo/datamodels/topology_1e_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e_pattern.h>
#include <falaise/snemo/datamodels/energy_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e_pattern,
                                                      "snemo::datamodel::topology_1e_pattern")

    // static
    const std::string & topology_1e_pattern::pattern_id()
    {
      static const std::string _id("1e");
      return _id;
    }

    std::string topology_1e_pattern::get_pattern_id() const
    {
      return topology_1e_pattern::pattern_id();
    }

    topology_1e_pattern::topology_1e_pattern()
      : base_topology_pattern()
    {
      return;
    }

    topology_1e_pattern::~topology_1e_pattern()
    {
      return;
    }

    bool topology_1e_pattern::has_electron_angle() const
    {
      return has_measurement("angle_e1");
    }

    double topology_1e_pattern::get_electron_angle() const
    {
      DT_THROW_IF(! has_electron_angle(), std::logic_error, "No electron angle measurement stored !");
      return dynamic_cast<const snemo::datamodel::angle_measurement&> (get_measurement("angle_e1")).get_angle();
    }

    bool topology_1e_pattern::has_electron_energy() const
    {
      return has_measurement("energy_e1");
    }

    double topology_1e_pattern::get_electron_energy() const
    {
      DT_THROW_IF(! has_electron_energy(), std::logic_error, "No electron energy measurement stored !");
      return dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e1")).get_energy();
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
