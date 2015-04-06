/** \file falaise/snemo/datamodels/base_topology_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(base_topology_pattern,
                                                      "snemo::datamodel::base_topology_pattern")


    bool base_topology_pattern::has_pattern_id() const
    {
      return ! _pattern_id_.empty();
    }

    const std::string & base_topology_pattern::get_pattern_id() const
    {
      return _pattern_id_;
    }

    void base_topology_pattern::_set_pattern_id(const std::string & pattern_id_)
    {
      _pattern_id_ = pattern_id_;
      return;
    }

    base_topology_pattern::base_topology_pattern(const std::string & pattern_id_)
    {
      _set_pattern_id(pattern_id_);
      return;
    }

    base_topology_pattern::~base_topology_pattern()
    {
      return;
    }

    void base_topology_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) {
        indent = indent_;
      }
      if (! title_.empty()) {
        out_ << indent << title_ << std::endl;
      }

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Pattern ID : ";
      if(! has_pattern_id()) {
        out_ << "<none>";
      } else {
        out_ << "'" << get_pattern_id() << "'";
      }
      out_ << std::endl;
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
