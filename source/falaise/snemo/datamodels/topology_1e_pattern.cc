/** \file falaise/snemo/datamodels/topology_1e_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e_pattern,
                                                      "snemo::datamodel::topology_1e_pattern")

    std::string topology_1e_pattern::pattern_id() const
    {
      const std::string id("1e");
      return id;
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

    // bool topology_1e_pattern::has_angle() const
    // {
    //   return _angle_.has_angle();
    // }

    // void topology_1e_pattern::set_angle(double angle_)
    // {
    //   _angle_.set_angle(angle_);
    //   return;
    // }

    // double topology_1e_pattern::get_angle() const
    // {
    //   DT_THROW_IF(! has_angle(), std::logic_error, "No angle stored !");
    //   return _angle_.get_angle();
    // }

    // bool topology_1e_pattern::has_electron_energy() const
    // {
    //   return datatools::is_valid(_electron_energy_);
    // }

    // void topology_1e_pattern::set_electron_energy(double energy_)
    // {
    //   _electron_energy_ = energy_;
    //   return;
    // }

    // double topology_1e_pattern::get_electron_energy() const
    // {
    //   return _electron_energy_;
    // }

    void topology_1e_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      // out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
      //      << "Angle : ";
      // if (has_angle()) {
      //   out_ << get_angle()/CLHEP::degree << "°";
      // } else {
      //   out_ << "No value";
      // }
      // out_ << std::endl;

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
