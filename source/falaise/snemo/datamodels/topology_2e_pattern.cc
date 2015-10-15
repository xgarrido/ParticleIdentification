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

    virtual void topology_2e_pattern::build_particle_tracks_dictionary(const snemo::datamodel::particle_track_data::particle_collection_type & the_particles_, snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & particle_tracks_dict_)
    {
      size_t n_electrons = 0;
      for (auto i_particle = the_particles_.begin(); i_particle != the_particles_.end(); ++i_particle) {
        if(snemo::datamodel::pid_utils::particle_is_electron(i_particle->get())) {
          n_electrons++;
          std::ostringstream key;
          key << "e" << n_electrons;
          particle_tracks_dict_[key] = the_particles_->get();
        }
      }

      return;
    }

    /*
    virtual void topology_2e_pattern::build_measurement_dictionary(snemo::datamodel::base_topology_pattern::particle_tracks_dict_type & particle_tracks_dict_, snemo::datamodel::base_topology_pattern::measurement_dict_type & meas_dict_)
    {
      //temporary ?
      const snemo::datamodel::particle_track & e1 = particle_tracks_dict_["e1"];
      const snemo::datamodel::particle_track & e2 = particle_tracks_dict_["e2"];

      {
        snemo::datamodel::TOF_measurement * ptr_tof = new snemo::datamodel::TOF_measurement;
        meas_dict_["tof_e1_e2"].reset(ptr_tof);
        if (_TOFD_) _TOFD_->process(e1, e2,
                                    ptr_tof->grab_internal_probabilities(),
                                    ptr_tof->grab_external_probabilities());
      }

      {
        snemo::datamodel::delta_vertices_measurement * ptr_delta_vertices_source = new snemo::datamodel::delta_vertices_measurement;
        meas_dict_["delta_vertices_source_e1_e2"].reset(ptr_delta_vertices_source);
        if (_DVD_) _DVD_->process(e1, e2,
                                  ptr_delta_vertices_source->grab_internal_probabilities(),
                                  ptr_delta_vertices_source->grab_external_probabilities());
      }

      {
        snemo::datamodel::angle_measurement * ptr_angle = new snemo::datamodel::angle_measurement;
        meas_dict_["angle_e1_e2"].reset(ptr_angle);
        if (_AMD_) _AMD_->process(e1, e2, ptr_angle->grab_angle());
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas_dict_["energy_e1"].reset(ptr_energy);
        if (_EMD_) _EMD_->process(e1, ptr_energy->grab_energy());
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas_dict_["energy_e2"].reset(ptr_energy);
        if (_EMD_) _EMD_->process(e2, ptr_energy->grab_energy());
      }

      return;
    }
    */
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
