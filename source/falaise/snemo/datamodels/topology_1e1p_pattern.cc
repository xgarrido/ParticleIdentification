/** \file falaise/snemo/datamodels/topology_1e1p_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1p_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e1p_pattern,
                                                      "snemo::datamodel::topology_1e1p_pattern")

    // static
    const std::string & topology_1e1p_pattern::pattern_id()
    {
      static const std::string _id("1e1p");
      return _id;
    }

    topology_1e1p_pattern::topology_1e1p_pattern()
      : base_topology_pattern(topology_1e1p_pattern::pattern_id())
    {
      return;
    }

    topology_1e1p_pattern::~topology_1e1p_pattern()
    {
      return;
    }

    // bool topology_1e1p_pattern::has_TOF_measurement() const
    // {
    //   return has_internal_probability() && has_external_probability();
    // }

    bool topology_1e1p_pattern::has_internal_probability() const
    {
      return datatools::is_valid(_tof_.get_internal_probabilities().front());
    }

    void topology_1e1p_pattern::set_internal_probability(double prob_)
    {
      _tof_.grab_internal_probabilities().push_back(prob_);
      return;
    }

    double topology_1e1p_pattern::get_internal_probability() const
    {
      return _tof_.get_internal_probabilities().front();
    }

    bool topology_1e1p_pattern::has_external_probability() const
    {
      return datatools::is_valid(_tof_.get_external_probabilities().front());
    }

    void topology_1e1p_pattern::set_external_probability(double prob_)
    {
      _tof_.grab_external_probabilities().push_back(prob_);
      return;
    }

    double topology_1e1p_pattern::get_external_probability() const
    {
      return _tof_.get_external_probabilities().front();
    }

    bool topology_1e1p_pattern::has_delta_vertices_y() const
    {
      return datatools::is_valid(_DeltaV_.get_delta_vertices_y());
    }

    void topology_1e1p_pattern::set_delta_vertices_y(double deltaV_y_)
    {
      _DeltaV_.grab_delta_vertices_y() = deltaV_y_;
      return;
    }

    double topology_1e1p_pattern::get_delta_vertices_y() const
    {
      return _DeltaV_.get_delta_vertices_y();
    }

    bool topology_1e1p_pattern::has_delta_vertices_z() const
    {
      return datatools::is_valid(_DeltaV_.get_delta_vertices_z());
    }

    void topology_1e1p_pattern::set_delta_vertices_z(double deltaV_z_)
    {
      _DeltaV_.grab_delta_vertices_z() = deltaV_z_;
      return;
    }

    double topology_1e1p_pattern::get_delta_vertices_z() const
    {
      return _DeltaV_.get_delta_vertices_z();
    }

    bool topology_1e1p_pattern::has_angle() const
    {
      return datatools::is_valid(_angle_.get_angle());
    }

    void topology_1e1p_pattern::set_angle(double angle_)
    {
      _angle_.grab_angle() = angle_;
      return;
    }

    double topology_1e1p_pattern::get_angle() const
    {
      return _angle_.get_angle();
    }

    void topology_1e1p_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Internal probability : " << get_internal_probability() << std::endl;
      out_ << indent << datatools::i_tree_dumpable::tag
           << "External probability : " << get_external_probability() << std::endl;
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Delta vertices Y : " << get_delta_vertices_y()/CLHEP::mm << " mm" << std::endl;
      out_ << indent << datatools::i_tree_dumpable::tag
           << "Delta vertices Z : " << get_delta_vertices_z()/CLHEP::mm << " mm" << std::endl;
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
