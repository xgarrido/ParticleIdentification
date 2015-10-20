/** \file falaise/snemo/datamodels/topology_2e_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2e_pattern.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/energy_measurement.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2e_pattern,
                                                      "snemo::datamodel::topology_2e_pattern")

    std::string topology_2e_pattern::pattern_id() const
    {
      const std::string id("2e");
      return id;
    }

    topology_2e_pattern::topology_2e_pattern()
      : base_topology_pattern()
    {
      return;
    }

    topology_2e_pattern::~topology_2e_pattern()
    {
      return;
    }

    bool topology_2e_pattern::has_electron_minimal_energy() const
    {
      return has_measurement("energy_e1") && has_measurement("energy_e2");
    }

    double topology_2e_pattern::get_electron_minimal_energy() const
    {
      DT_THROW_IF(! has_electron_minimal_energy(), std::logic_error, "No electron minimal energy measurement stored !");
      //bien dégueulasse
      return std::min(dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e1")).get_energy(),
                      dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e2")).get_energy());
    }

    bool topology_2e_pattern::has_electron_maximal_energy() const
    {
      return has_measurement("energy_e1") && has_measurement("energy_e2");
    }

    double topology_2e_pattern::get_electron_maximal_energy() const
    {
      DT_THROW_IF(! has_electron_maximal_energy(), std::logic_error, "No electron maximal energy measurement stored !");
      return std::max(dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e1")).get_energy(),
                      dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e2")).get_energy());
    }

    double topology_2e_pattern::get_electrons_energy_sum() const
    {
      //Only one energy check
      DT_THROW_IF(! has_electron_maximal_energy(), std::logic_error, "No electron energy measurement stored !");
      return get_electron_minimal_energy() + get_electron_maximal_energy();
    }

    double topology_2e_pattern::get_electrons_energy_difference() const
    {
      //Only one energy check
      DT_THROW_IF(! has_electron_maximal_energy(), std::logic_error, "No electron energy measurement stored !");
      return std::abs(get_electron_minimal_energy() - get_electron_maximal_energy());
    }

    bool topology_2e_pattern::has_electrons_internal_probability() const
    {
      return has_measurement("tof_e1_e2");
    }

    double topology_2e_pattern::get_electrons_internal_probability() const
    {
      DT_THROW_IF(! has_electrons_internal_probability(), std::logic_error, "No electrons TOF measurement stored !");
      dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement("tof_e1_e2")).get_internal_probabilities().front();
    }

    bool topology_2e_pattern::has_electrons_external_probability() const
    {
      return has_measurement("tof_e1_e2");
    }

    double topology_2e_pattern::get_electrons_external_probability() const
    {
      DT_THROW_IF(! has_electrons_external_probability(), std::logic_error, "No electrons TOF measurement stored !");
      dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement("tof_e1_e2")).get_external_probabilities().front();
    }

    bool topology_2e_pattern::has_electrons_angle() const
    {
      return has_measurement("angle_e1_e2");
    }

    double topology_2e_pattern::get_electrons_angle() const
    {
      DT_THROW_IF(! has_electrons_external_probability(), std::logic_error, "No electrons angle measurement stored !");
      return dynamic_cast<const snemo::datamodel::angle_measurement&> (get_measurement("angle_e1_e2")).get_angle();
    }

    void topology_2e_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      return;
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
