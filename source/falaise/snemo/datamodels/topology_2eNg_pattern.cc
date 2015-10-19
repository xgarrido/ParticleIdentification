/** \file falaise/snemo/datamodels/topology_2eNg_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2eNg_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2eNg_pattern,
                                                      "snemo::datamodel::topology_2eNg_pattern")

    std::string topology_2eNg_pattern::pattern_id() const
    {
      const std::string id("2eNg");
      return id;
    }

    topology_2eNg_pattern::topology_2eNg_pattern()
      : topology_2e_pattern()
    {
      // _number_of_gammas_ = 0;
      return;
    }

    topology_2eNg_pattern::~topology_2eNg_pattern()
    {
      return;
    }

    // void topology_2eNg_pattern::set_number_of_gammas(const size_t ngammas_)
    // {
    //   _number_of_gammas_ = ngammas_;
    //   return;
    // }

    // size_t topology_2eNg_pattern::get_number_of_gammas() const
    // {
    //   return _number_of_gammas_;
    // }

    void topology_2eNg_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      // for(size_t i=0; i<get_internal_probability().size();++i) {
      //   out_ << indent << datatools::i_tree_dumpable::tag
      //        << "Internal probability : " << get_internal_probability().at(i) << std::endl;
      //   out_ << indent << datatools::i_tree_dumpable::tag
      //        << "External probability : " << get_external_probability().at(i) << std::endl;
      // }

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta Vertices Y : " << get_delta_vertices_y() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta Vertices Z : " << get_delta_vertices_z() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Angle Z : " << get_angle() << std::endl;

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
