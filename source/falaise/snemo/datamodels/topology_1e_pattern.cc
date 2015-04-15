/** \file falaise/snemo/datamodels/topology_1e_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e_pattern.h>

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

    topology_1e_pattern::topology_1e_pattern()
      : base_topology_pattern(topology_1e_pattern::pattern_id())
    {
      return;
    }

    topology_1e_pattern::~topology_1e_pattern()
    {
      return;
    }

    bool topology_1e_pattern::has_angle() const
    {
      return datatools::is_valid(_angle_.get_angle());
    }

    void topology_1e_pattern::set_angle(const double angle_)
    {
      _angle_.grab_angle() = angle_;
      return;
    }

    double topology_1e_pattern::get_angle() const
    {
      return _angle_.get_angle();
    }

    void topology_1e_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Angle : " << get_angle() << std::endl;

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
