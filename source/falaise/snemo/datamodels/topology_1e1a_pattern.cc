/** \file falaise/snemo/datamodels/topology_1e1a_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1e1a_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1e1a_pattern,
                                                      "snemo::datamodel::topology_1e1a_pattern")

    std::string topology_1e1a_pattern::pattern_id() const
    {
      const std::string id("1e1a");
      return id;
    }

    topology_1e1a_pattern::topology_1e1a_pattern()
      : base_topology_pattern()
    {
      return;
    }

    topology_1e1a_pattern::~topology_1e1a_pattern()
    {
      return;
    }

    // bool topology_1e1a_pattern::is_valid() const
    // {
    //   return has_electron_particle() && has_alpha_particle();
    // }

    // bool topology_1e1a_pattern::has_electron_particle() const
    // {
    //   return _electron_particle_.has_data();
    // }

    // void topology_1e1a_pattern::set_electron_particle(const particle_track::handle_type & handle_)
    // {
    //   _electron_particle_ = handle_;
    //   return;
    // }

    // const particle_track & topology_1e1a_pattern::get_electron_particle() const
    // {
    //   return _electron_particle_.get();
    // }

    // bool topology_1e1a_pattern::has_alpha_particle() const
    // {
    //   return _alpha_particle_.has_data();
    // }

    // void topology_1e1a_pattern::set_alpha_particle(const particle_track::handle_type & handle_)
    // {
    //   _alpha_particle_ = handle_;
    //   return;
    // }

    // const particle_track & topology_1e1a_pattern::get_alpha_particle() const
    // {
    //   return _alpha_particle_.get();
    // }

    // bool topology_1e1a_pattern::has_delta_vertices_y() const
    // {
    //   return _delta_vertices_.has_delta_vertices_y();
    // }

    // void topology_1e1a_pattern::set_delta_vertices_y(double deltaV_y_)
    // {
    //   _delta_vertices_.set_delta_vertices_y(deltaV_y_);
    //   return;
    // }

    // double topology_1e1a_pattern::get_delta_vertices_y() const
    // {
    //   return _delta_vertices_.get_delta_vertices_y();
    // }

    // bool topology_1e1a_pattern::has_delta_vertices_z() const
    // {
    //   return _delta_vertices_.has_delta_vertices_z();
    // }

    // void topology_1e1a_pattern::set_delta_vertices_z(double deltaV_z_)
    // {
    //   _delta_vertices_.set_delta_vertices_z(deltaV_z_);
    //   return;
    // }

    // double topology_1e1a_pattern::get_delta_vertices_z() const
    // {
    //   return _delta_vertices_.get_delta_vertices_z();
    // }

    // bool topology_1e1a_pattern::has_angle() const
    // {
    //   return _angle_.has_angle();
    // }

    // void topology_1e1a_pattern::set_angle(double angle_)
    // {
    //   _angle_.set_angle(angle_);
    //   return;
    // }

    // double topology_1e1a_pattern::get_angle() const
    // {
    //   return _angle_.get_angle();
    // }

    // double topology_1e1a_pattern::get_alpha_delayed_time() const
    // {
    //   double time = datatools::invalid_real();
    //   if (has_alpha_particle()) {
    //     const snemo::datamodel::particle_track & the_alpha = get_alpha_particle();
    //     if (the_alpha.has_trajectory()) {
    //       const snemo::datamodel::tracker_trajectory & a_trajectory = the_alpha.get_trajectory();
    //       const datatools::properties & aux = a_trajectory.get_auxiliaries();
    //       if (aux.has_key("t0")) {
    //         time = aux.fetch_real("t0");
    //       }
    //     }
    //   }
    //   return time;
    // }

    // double topology_1e1a_pattern::get_alpha_track_length() const
    // {
    //   double length = datatools::invalid_real();
    //   if (has_alpha_particle()) {
    //     const snemo::datamodel::particle_track & the_alpha = get_alpha_particle();
    //     if (the_alpha.has_trajectory()) {
    //       const snemo::datamodel::tracker_trajectory & a_trajectory = the_alpha.get_trajectory();
    //       const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
    //       length = a_track_pattern.get_shape().get_length();
    //     }
    //   }
    //   return length;
    // }

    // double topology_1e1a_pattern::get_electron_track_length() const
    // {
    //   double length = datatools::invalid_real();
    //   if (has_electron_particle()) {
    //     const snemo::datamodel::particle_track & the_electron = get_electron_particle();
    //     if (the_electron.has_trajectory()) {
    //       const snemo::datamodel::tracker_trajectory & a_trajectory = the_electron.get_trajectory();
    //       const snemo::datamodel::base_trajectory_pattern & a_track_pattern = a_trajectory.get_pattern();
    //       length = a_track_pattern.get_shape().get_length();
    //     }
    //   }
    //   return length;
    // }

    // double topology_1e1a_pattern::get_electron_energy() const
    // {
    //   double energy = datatools::invalid_real();
    //   if (has_electron_particle()) {
    //     const snemo::datamodel::particle_track & the_electron = get_electron_particle();
    //     if (the_electron.has_associated_calorimeter_hits()) {
    //       const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
    //         calos = the_electron.get_associated_calorimeter_hits();
    //       if (calos.size() == 1) {
    //         energy = calos.front().get().get_energy();
    //       }
    //     }
    //   }
    //   return energy;
    // }

    void topology_1e1a_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta vertices Y : ";
      // if (has_delta_vertices_y()) {
      //   out_ << get_delta_vertices_y()/CLHEP::mm << " mm";
      // } else {
      //   out_ << "No value";
      // }
      // out_ << std::endl;

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta vertices Z : ";
      // if (has_delta_vertices_z()) {
      //   out_ << get_delta_vertices_z()/CLHEP::mm << " mm";
      // } else {
      //   out_ << "No value";
      // }
      // out_ << std::endl;

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Angle : ";
      // if (has_angle()) {
      //   out_ << get_angle()/CLHEP::degree << "°";
      // } else {
      //   out_ << "No value";
      // }
      // out_ << std::endl;

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Electron particle : ";
      // if (has_electron_particle()) {
      //   out_ << std::endl;
      //   get_electron_particle().tree_dump(out_, title_, indent_, inherit_);
      // } else {
      //   out_ << "<none>" << std::endl;
      // }

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Alpha particle : ";
      // if (has_alpha_particle()) {
      //   out_ << std::endl;
      //   get_alpha_particle().tree_dump(out_, title_, indent_, inherit_);
      // } else {
      //   out_ << "<none>" << std::endl;
      // }

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
