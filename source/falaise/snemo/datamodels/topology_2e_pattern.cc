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
      datatools::plus_infinity(_minimal_energy_);
      datatools::minus_infinity(_maximal_energy_);
      return;
    }

    topology_2e_pattern::~topology_2e_pattern()
    {
      return;
    }

    bool topology_2e_pattern::is_valid() const
    {
      return has_electron_particles() && _electron_particles_.size() == 2;
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

    double topology_2e_pattern::get_minimal_energy()
    {
      if (datatools::is_plus_infinity(_minimal_energy_)) {
        _compute_energies_();
      }
      return _minimal_energy_;
    }

    double topology_2e_pattern::get_maximal_energy()
    {
      if (datatools::is_minus_infinity(_maximal_energy_)) {
        _compute_energies_();
      }
      return _maximal_energy_;
    }

    double topology_2e_pattern::get_total_energy()
    {
      return get_maximal_energy() + get_minimal_energy();
    }

    void topology_2e_pattern::_compute_energies_()
    {
      DT_THROW_IF(! has_electron_particles(), std::logic_error, "Missing electron particles !");
      for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
             i = get_electron_particles().begin();
           i != get_electron_particles().end(); ++i) {
        const snemo::datamodel::particle_track & a_electron = i->get();
        double energy = 0.0;
        if (! a_electron.has_associated_calorimeter_hits()) continue;
        const snemo::datamodel::calibrated_calorimeter_hit::collection_type & the_calos
          = a_electron.get_associated_calorimeter_hits();
        for (snemo::datamodel::calibrated_calorimeter_hit::collection_type::const_iterator
               icalo = the_calos.begin(); icalo != the_calos.end(); ++icalo) {
          const snemo::datamodel::calibrated_calorimeter_hit & a_calo = icalo->get();
          energy += a_calo.get_energy();
        }
        _minimal_energy_ = std::min(energy, _minimal_energy_);
        _maximal_energy_ = std::min(energy, _maximal_energy_);
      }
      return;
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
