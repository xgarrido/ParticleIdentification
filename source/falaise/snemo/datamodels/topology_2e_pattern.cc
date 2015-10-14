/** \file falaise/snemo/datamodels/topology_2e_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2e_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2e_pattern,
                                                      "snemo::datamodel::topology_2e_pattern")

    // static
    const std::string & topology_2e_pattern::pattern_id()
    {
      static const std::string _id("2e");
      return _id;
    }

    topology_2e_pattern::topology_2e_pattern()
      : base_topology_pattern(topology_2e_pattern::pattern_id())
    {
      return;
    }

    topology_2e_pattern::~topology_2e_pattern()
    {
      return;
    }

    bool topology_2e_pattern::is_valid() const
    {
      return _e1_.has_track_id() && _e2_.has_track_id();
    }

    virtual void topology_2e_pattern::build_particle_tracks_dictionary() const
    {
      tracks_["e1"] = _e1_;
      tracks_["e2"] = _e2_;
      return;
    }

    virtual void topology_2e_pattern::build_measurement_dictionary() const
    {
      if (_tof_2e_.is_valid()) {
        meas_dict_["tof_2e"] = &_tof_2e_;
      }
      if (_delta_vertices_source_2e_.is_valid()) {
        meas_dict_["delta_vertices_source_2e"] = &_delta_vertices_source_2e_;
      }
      if (_angle_2e_.is_valid()) {
        meas_dict_["angle_2e"] = &_angle_2e_;
      }
      if (_energy_2e_.is_valid()) {
        meas_dict_["energy_2e"] = &_energy_2e_;
      }
      return;
    }

    bool topology_2e_pattern::has_electron_particles() const
    {
      return !_electron_particles_.empty();
    }

    void topology_2e_pattern::add_electron_particle(const particle_track::handle_type & handle_)
    {
      _electron_particles_.push_back(handle_);
    }

    const particle_track_data::particle_collection_type & topology_2e_pattern::get_electron_particles() const
    {
      return _electron_particles_;
    }

    bool topology_2e_pattern::has_internal_probability() const
    {
      return ! _tof_.get_internal_probabilities().empty();
    }

    void topology_2e_pattern::set_internal_probability(double prob_)
    {
      _tof_.grab_internal_probabilities().push_back(prob_);
      return;
    }

    double topology_2e_pattern::get_internal_probability() const
    {
      DT_THROW_IF(! has_internal_probability(), std::logic_error, "No internal probability !");
      return _tof_.get_internal_probabilities().front();
    }

    bool topology_2e_pattern::has_external_probability() const
    {
      return ! _tof_.get_external_probabilities().empty();
    }

    void topology_2e_pattern::set_external_probability(double prob_)
    {
      _tof_.grab_external_probabilities().push_back(prob_);
      return;
    }

    double topology_2e_pattern::get_external_probability() const
    {
      DT_THROW_IF(! has_external_probability(), std::logic_error, "No external probability !");
      return _tof_.get_external_probabilities().front();
    }

    bool topology_2e_pattern::has_delta_vertices_y() const
    {
      return _delta_vertices_source_.has_delta_vertices_y();
    }

    void topology_2e_pattern::set_delta_vertices_y(double delta_)
    {
      _delta_vertices_source_.set_delta_vertices_y(delta_);
      return;
    }

    double topology_2e_pattern::get_delta_vertices_y() const
    {
      return _delta_vertices_source_.get_delta_vertices_y();
    }

    bool topology_2e_pattern::has_delta_vertices_z() const
    {
      return _delta_vertices_source_.has_delta_vertices_z();
    }

    void topology_2e_pattern::set_delta_vertices_z(double delta_)
    {
      _delta_vertices_source_.set_delta_vertices_z(delta_);
      return;
    }

    double topology_2e_pattern::get_delta_vertices_z() const
    {
      return _delta_vertices_source_.get_delta_vertices_z();
    }

    bool topology_2e_pattern::has_angle() const
    {
      return _angle_.has_angle();
    }

    void topology_2e_pattern::set_angle(double angle_)
    {
      _angle_.set_angle(angle_);
      return;
    }

    double topology_2e_pattern::get_angle() const
    {
      return _angle_.get_angle();
    }

    void topology_2e_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Internal probability : ";
      if (has_external_probability()) {
        out_ << get_internal_probability()/CLHEP::perCent << " %";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "External probability : ";
      if (has_external_probability()) {
        out_ << get_external_probability()/CLHEP::perCent << " %";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

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
           << "Electron particles : ";
      if (has_electron_particles()) {
        out_ << std::endl;
        for (size_t i = 0; i < get_electron_particles().size(); ++i)
          get_electron_particles()[i].get().tree_dump(out_, title_, indent_, inherit_);
      } else {
        out_ << "<none>" << std::endl;
      }

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
