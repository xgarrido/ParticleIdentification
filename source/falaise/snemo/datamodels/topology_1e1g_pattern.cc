/** \file falaise/snemo/datamodels/topology_1e1g_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1g_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e1g_pattern,
                                                      "snemo::datamodel::topology_1e1g_pattern")

    // static
    const std::string & topology_1e1g_pattern::pattern_id()
    {
      static const std::string _id("1e1g");
      return _id;
    }

    topology_1e1g_pattern::topology_1e1g_pattern()
      : base_topology_pattern(topology_1e1g_pattern::pattern_id())
    {
      _tof_.internal_probability = std::numeric_limits< std::vector<double> >::quiet_NaN();
      _tof_.external_probability = std::numeric_limits< std::vector<double> >::quiet_NaN();
      return;
    }

    topology_1e1g_pattern::~topology_1e1g_pattern()
    {
      return;
    }

    // bool topology_1e1g_pattern::has_TOF_measurement() const
    // {
    //   return has_internal_probability() && has_external_probability();
    // }

    bool topology_1e1g_pattern::has_angle() const
    {
      return datatools::is_valid(_angle_.angle);
    }

    void topology_1e1g_pattern::set_angle(const double angle_)
    {
      _angle_.angle = angle_;
      return;
    }

    double topology_1e1g_pattern::get_angle() const
    {
      return _angle_.angle;
    }

    bool topology_1e1g_pattern::has_internal_probability() const
    {
      for(std::vector<double>::const_iterator i_proba = _tof_.internal_probability.begin();
          i_proba != _tof_.internal_probability.end(); ++i_proba)
        if(!datatools::is_valid(*i_proba))
          return false;

      return true;
    }

    void topology_1e1g_pattern::set_internal_probability(const std::vector<double> prob_)
    {
      _tof_.internal_probability = prob_;
      return;
    }

    std::vector<double> topology_1e1g_pattern::get_internal_probability() const
    {
      return _tof_.internal_probability;
    }

    bool topology_1e1g_pattern::has_external_probability() const
    {
      for(std::vector<double>::const_iterator i_proba = _tof_.external_probability.begin();
          i_proba != _tof_.external_probability.end(); ++i_proba)
        if(!datatools::is_valid(*i_proba))
          return false;

      return true;
    }

    void topology_1e1g_pattern::set_external_probability(const std::vector<double> prob_)
    {
      _tof_.external_probability = prob_;
      return;
    }

    std::vector<double> topology_1e1g_pattern::get_external_probability() const
    {
      return _tof_.external_probability;
    }

    void topology_1e1g_pattern::tree_dump(std::ostream      & out_,
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
