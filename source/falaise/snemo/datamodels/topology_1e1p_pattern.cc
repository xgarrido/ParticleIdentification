/** \file falaise/snemo/datamodels/topology_1e1p_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1p_pattern.h>
#include <falaise/snemo/datamodels/energy_measurement.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/tof_measurement.h>
#include <falaise/snemo/datamodels/delta_vertices_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e1p_pattern,
                                                      "snemo::datamodel::topology_1e1p_pattern")


    std::string topology_1e1p_pattern::pattern_id() const
    {
      const std::string id("1e1p");
      return id;
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

    bool topology_1e1p_pattern::has_positron_energy() const
    {
      return has_measurement("energy_p1");
    }

    double topology_1e1p_pattern::get_positron_energy() const
    {
      DT_THROW_IF(! has_positron_energy(), std::logic_error, "No positron energy measurement stored !");
      return dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_p1")).get_energy();
    }

    bool topology_1e1p_pattern::has_positron_angle() const
    {
      return has_measurement("angle_p1");
    }

    double topology_1e1p_pattern::get_positron_angle() const
    {
      DT_THROW_IF(! has_positron_angle(), std::logic_error, "No positron angle measurement stored !");
      return dynamic_cast<const snemo::datamodel::angle_measurement&> (get_measurement("angle_p1")).get_angle();
    }

    bool topology_1e1p_pattern::has_electron_positron_angle() const
    {
      return has_measurement("angle_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_angle() const
    {
      DT_THROW_IF(! has_electron_positron_angle(), std::logic_error, "No electron-positron angle measurement stored !");
      return dynamic_cast<const snemo::datamodel::angle_measurement&> (get_measurement("angle_e1_p1")).get_angle();
    }

    bool topology_1e1p_pattern::has_electron_positron_internal_probability() const
    {
      return has_measurement("tof_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_internal_probability() const
    {
      DT_THROW_IF(! has_electron_positron_internal_probability(), std::logic_error, "No electron-positron TOF measurement stored !");
      return dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement("tof_e1_p1")).get_internal_probabilities().front();
    }

    bool topology_1e1p_pattern::has_electron_positron_external_probability() const
    {
      return has_measurement("tof_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_external_probability() const
    {
      DT_THROW_IF(! has_electron_positron_external_probability(), std::logic_error, "No electron-positron TOF measurement stored !");
      return dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement("tof_e1_p1")).get_external_probabilities().front();
    }

    bool topology_1e1p_pattern::has_electron_positron_vertices_probability() const
    {
      return has_measurement("vertices_probability_e1_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_vertices_probability() const
    {
      DT_THROW_IF(! has_electron_positron_vertices_probability(), std::logic_error, "No common electrons vertices measurement stored !");
      return dynamic_cast<const snemo::datamodel::delta_vertices_measurement&> (get_measurement("vertices_probability_e1_e2")).get_probability();
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
      return has_measurement("energy_e1") && has_measurement("energy_p1");
    }

    double topology_1e1p_pattern::get_electron_positron_maximal_energy() const
    {
      DT_THROW_IF(! has_electron_positron_maximal_energy(), std::logic_error, "No electron/positron maximal energy measurement stored !");
      return std::max(dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e1")).get_energy(),
                      dynamic_cast<const snemo::datamodel::energy_measurement&> (get_measurement("energy_e2")).get_energy());
    }

    void topology_1e1p_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Internal probability : " << get_internal_probability() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "External probability : " << get_external_probability() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta vertices Y : " << get_delta_vertices_y()/CLHEP::mm << " mm" << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta vertices Z : " << get_delta_vertices_z()/CLHEP::mm << " mm" << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Angle : " << get_angle() << std::endl;

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
