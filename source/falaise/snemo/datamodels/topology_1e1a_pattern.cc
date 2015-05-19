/** \file falaise/snemo/datamodels/topology_1e1a_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1a_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e1a_pattern,
                                                      "snemo::datamodel::topology_1e1a_pattern")

    // static
    const std::string & topology_1e1a_pattern::pattern_id()
    {
      static const std::string _id("1e1a");
      return _id;
    }

    topology_1e1a_pattern::topology_1e1a_pattern()
      : base_topology_pattern(topology_1e1a_pattern::pattern_id())
    {
      _alpha_delayed_time_ = datatools::invalid_real();
      return;
    }

    topology_1e1a_pattern::~topology_1e1a_pattern()
    {
      return;
    }

    bool topology_1e1a_pattern::has_delta_vertices_y() const
    {
      return _DeltaV_.has_delta_vertices_y();
    }

    void topology_1e1a_pattern::set_delta_vertices_y(double deltaV_y_)
    {
      _DeltaV_.set_delta_vertices_y(deltaV_y_);
      return;
    }

    double topology_1e1a_pattern::get_delta_vertices_y() const
    {
      return _DeltaV_.get_delta_vertices_y();
    }

    bool topology_1e1a_pattern::has_delta_vertices_z() const
    {
      return _DeltaV_.has_delta_vertices_z();
    }

    void topology_1e1a_pattern::set_delta_vertices_z(double deltaV_z_)
    {
      _DeltaV_.set_delta_vertices_z(deltaV_z_);
      return;
    }

    double topology_1e1a_pattern::get_delta_vertices_z() const
    {
      return _DeltaV_.get_delta_vertices_z();
    }

    bool topology_1e1a_pattern::has_angle() const
    {
      return _angle_.has_angle();
    }

    void topology_1e1a_pattern::set_angle(double angle_)
    {
      _angle_.set_angle(angle_);
      return;
    }

    double topology_1e1a_pattern::get_angle() const
    {
      return _angle_.get_angle();
    }

    bool topology_1e1a_pattern::has_alpha_delayed_time() const
    {
      return datatools::is_valid(_alpha_delayed_time_);
    }

    void topology_1e1a_pattern::set_alpha_delayed_time(double time_)
    {
      _alpha_delayed_time_ = time_;
      return;
    }

    double topology_1e1a_pattern::get_alpha_delayed_time() const
    {
      return _alpha_delayed_time_;
    }

    void topology_1e1a_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

            out_ << indent << datatools::i_tree_dumpable::tag
           << "Delta vertices Y : ";
      if (has_delta_vertices_y()) {
        out_ << get_delta_vertices_y()/CLHEP::mm << " mm";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Delta vertices Z : ";
      if (has_delta_vertices_z()) {
        out_ << get_delta_vertices_z()/CLHEP::mm << " mm";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Angle : ";
      if (has_angle()) {
        out_ << get_angle()/CLHEP::degree << "°";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Alpha delayed time : ";
      if (has_alpha_delayed_time()) {
        out_ << get_alpha_delayed_time()/CLHEP::ns << " ns";
      } else {
        out_ << "No value";
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
