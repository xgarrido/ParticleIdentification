/** \file falaise/snemo/datamodels/topology_2e_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2e_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2e_pattern,
                                                      "snemo::datamodel::topology_2e_pattern")


    bool topology_2e_pattern::has_pattern_id() const
    {
      return ! _pattern_id_.empty();
    }

    const std::string & topology_2e_pattern::get_pattern_id() const
    {
      return _pattern_id_;
    }

    void topology_2e_pattern::_set_pattern_id(const std::string & pattern_id_)
    {
      _pattern_id_ = pattern_id_;
      return;
    }

    topology_2e_pattern::topology_2e_pattern(const std::string & pattern_id_)
    {
      _set_pattern_id(pattern_id_);
      return;
    }

    topology_2e_pattern::~topology_2e_pattern()
    {
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
