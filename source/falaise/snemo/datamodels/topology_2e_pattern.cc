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
      datatools::invalidate(_electron_minimal_energy_);
      datatools::invalidate(_electron_maximal_energy_);
      return;
    }

    topology_2e_pattern::~topology_2e_pattern()
    {
      return;
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

    bool topology_2e_pattern::has_minimal_energy() const
    {
      return datatools::is_valid(_electron_minimal_energy_);
    }

    void topology_2e_pattern::set_minimal_energy(double energy_)
    {
      _electron_minimal_energy_ = energy_;
      return;
    }

    double topology_2e_pattern::get_minimal_energy() const
    {
      return _electron_minimal_energy_;
    }

    bool topology_2e_pattern::has_maximal_energy() const
    {
      return datatools::is_valid(_electron_maximal_energy_);
    }

    void topology_2e_pattern::set_maximal_energy(double energy_)
    {
      _electron_maximal_energy_ = energy_;
      return;
    }

    double topology_2e_pattern::get_maximal_energy() const
    {
      return _electron_maximal_energy_;
    }

    double topology_2e_pattern::get_total_energy() const
    {
      return get_maximal_energy() + get_minimal_energy();
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
           << "Minimal energy : ";
      if (has_minimal_energy()) {
        out_ << get_minimal_energy()/CLHEP::keV << " keV";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Maximal energy : ";
      if (has_maximal_energy()) {
        out_ << get_maximal_energy()/CLHEP::keV << " keV";
      } else {
        out_ << "No value";
      }
      out_ << std::endl;

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Total energy : ";
      if (has_minimal_energy() && has_maximal_energy()) {
        out_ << get_total_energy()/CLHEP::keV << " keV";
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
