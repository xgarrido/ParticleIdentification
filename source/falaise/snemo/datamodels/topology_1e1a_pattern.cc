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
      datatools::invalidate(_alpha_delayed_time_);
      datatools::invalidate(_alpha_track_length_);
      datatools::invalidate(_electron_track_length_);
      datatools::invalidate(_electron_energy_);
      return;
    }

    topology_1e1a_pattern::~topology_1e1a_pattern()
    {
      return;
    }

    bool topology_1e1a_pattern::is_valid() const
    {
      return has_electron_particle() && has_alpha_particle();
    }

    bool topology_1e1a_pattern::has_electron_particle() const
    {
      return _electron_particle_.has_data();
    }

    void topology_1e1a_pattern::set_electron_particle(const particle_track::handle_type & handle_)
    {
      _electron_particle_ = handle_;
      return;
    }

    const particle_track & topology_1e1a_pattern::get_electron_particle() const
    {
      return _electron_particle_.get();
    }

    bool topology_1e1a_pattern::has_alpha_particle() const
    {
      return _alpha_particle_.has_data();
    }

    void topology_1e1a_pattern::set_alpha_particle(const particle_track::handle_type & handle_)
    {
      _alpha_particle_ = handle_;
      return;
    }

    const particle_track & topology_1e1a_pattern::get_alpha_particle() const
    {
      return _alpha_particle_.get();
    }

    bool topology_1e1a_pattern::has_delta_vertices_y() const
    {
      return _delta_vertices_.has_delta_vertices_y();
    }

    void topology_1e1a_pattern::set_delta_vertices_y(double deltaV_y_)
    {
      _delta_vertices_.set_delta_vertices_y(deltaV_y_);
      return;
    }

    double topology_1e1a_pattern::get_delta_vertices_y() const
    {
      return _delta_vertices_.get_delta_vertices_y();
    }

    bool topology_1e1a_pattern::has_delta_vertices_z() const
    {
      return _delta_vertices_.has_delta_vertices_z();
    }

    void topology_1e1a_pattern::set_delta_vertices_z(double deltaV_z_)
    {
      _delta_vertices_.set_delta_vertices_z(deltaV_z_);
      return;
    }

    double topology_1e1a_pattern::get_delta_vertices_z() const
    {
      return _delta_vertices_.get_delta_vertices_z();
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

    bool topology_1e1a_pattern::has_alpha_track_length() const
    {
      return datatools::is_valid(_alpha_track_length_);
    }

    void topology_1e1a_pattern::set_alpha_track_length(double length_)
    {
      _alpha_track_length_ = length_;
      return;
    }

    double topology_1e1a_pattern::get_alpha_track_length() const
    {
      return _alpha_track_length_;
    }

    bool topology_1e1a_pattern::has_electron_track_length() const
    {
      return datatools::is_valid(_electron_track_length_);
    }

    void topology_1e1a_pattern::set_electron_track_length(double length_)
    {
      _electron_track_length_ = length_;
      return;
    }

    double topology_1e1a_pattern::get_electron_track_length() const
    {
      return _electron_track_length_;
    }

    bool topology_1e1a_pattern::has_electron_energy() const
    {
      return datatools::is_valid(_electron_energy_);
    }

    void topology_1e1a_pattern::set_electron_energy(double energy_)
    {
      _electron_energy_ = energy_;
      return;
    }

    double topology_1e1a_pattern::get_electron_energy() const
    {
      return _electron_energy_;
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

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Angle : ";
      if (has_angle()) {
        out_ << get_angle()/CLHEP::degree << "°";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Alpha delayed time : ";
      if (has_alpha_delayed_time()) {
        out_ << get_alpha_delayed_time()/CLHEP::microsecond << " us";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Alpha track length : ";
      if (has_alpha_track_length()) {
        out_ << get_alpha_track_length()/CLHEP::cm << " cm";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Electron track length : ";
      if (has_electron_track_length()) {
        out_ << get_electron_track_length()/CLHEP::cm << " cm";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Electron energy : ";
      if (has_electron_energy()) {
        out_ << get_electron_energy()/CLHEP::keV << " keV";
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
