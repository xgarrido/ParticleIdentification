/** \file falaise/snemo/datamodels/topology_1eNg_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1eNg_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1eNg_pattern,
                                                      "snemo::datamodel::topology_1eNg_pattern")

    // static
    const std::string & topology_1eNg_pattern::pattern_id()
    {
      static const std::string _id("1eNg");
      return _id;
    }

    topology_1eNg_pattern::topology_1eNg_pattern()
      : base_topology_pattern(topology_1eNg_pattern::pattern_id())
    {
      // _number_of_gammas_ = 0;
      return;
    }

    topology_1eNg_pattern::~topology_1eNg_pattern()
    {
      return;
    }

    void topology_1eNg_pattern::set_number_of_gammas(const size_t ngammas_)
    {
      _number_of_gammas_ = ngammas_;
      return;
    }

    size_t topology_1eNg_pattern::get_number_of_gammas() const
    {
      return _number_of_gammas_;
    }

    const topology_1eNg_pattern::TOF_collection_type & topology_1eNg_pattern::get_TOF_collection() const
    {
      return _tofs_;
    }

    topology_1eNg_pattern::TOF_collection_type & topology_1eNg_pattern::grab_TOF_collection()
    {
      return _tofs_;
    }

    const topology_1eNg_pattern::angle_collection_type & topology_1eNg_pattern::get_angle_collection() const
    {
      return _angles_;
    }

    topology_1eNg_pattern::angle_collection_type & topology_1eNg_pattern::grab_angle_collection()
    {
      return _angles_;
    }

    // bool topology_1eNg_pattern::has_TOF_measurement() const
    // {
    //   return has_internal_probability() && has_external_probability();
    // }

    void topology_1eNg_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Internal probability : " << get_internal_probability() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "External probability : " << get_external_probability() << std::endl;

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
