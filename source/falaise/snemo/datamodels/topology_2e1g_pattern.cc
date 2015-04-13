/** \file falaise/snemo/datamodels/topology_2e1g_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2e1g_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2e1g_pattern,
                                                      "snemo::datamodel::topology_2e1g_pattern")

    // static
    const std::string & topology_2e1g_pattern::pattern_id()
    {
      static const std::string _id("2e1g");
      return _id;
    }

    topology_2e1g_pattern::topology_2e1g_pattern()
      : base_topology_pattern(topology_2e1g_pattern::pattern_id())
    {
      _tof_.internal_probability = std::numeric_limits< std::vector<double> >::quiet_NaN();
      _tof_.external_probability = std::numeric_limits< std::vector<double> >::quiet_NaN();
      datatools::invalidate(_DeltaV_.delta_vertices_y);
      datatools::invalidate(_DeltaV_.delta_vertices_z);
      return;
    }

    topology_2e1g_pattern::~topology_2e1g_pattern()
    {
      return;
    }

    // bool topology_2e1g_pattern::has_TOF_measurement() const
    // {
    //   return has_internal_probability() && has_external_probability();
    // }

    bool topology_2e1g_pattern::has_internal_probability() const
    {
      for(std::vector<double>::const_iterator i_proba = _tof_.internal_probability.begin();
          i_proba != _tof_.internal_probability.end(); ++i_proba)
        if(!datatools::is_valid(*i_proba))
          return false;

      return true;
    }

    void topology_2e1g_pattern::set_internal_probability(const std::vector<double> prob_)
    {
      _tof_.internal_probability = prob_;
      return;
    }

    std::vector<double> topology_2e1g_pattern::get_internal_probability() const
    {
      return _tof_.internal_probability;
    }

    bool topology_2e1g_pattern::has_external_probability() const
    {
      for(std::vector<double>::const_iterator i_proba = _tof_.external_probability.begin();
          i_proba != _tof_.external_probability.end(); ++i_proba)
        if(!datatools::is_valid(*i_proba))
          return false;

      return true;
    }

    void topology_2e1g_pattern::set_external_probability(const std::vector<double> prob_)
    {
      _tof_.external_probability = prob_;
      return;
    }

    std::vector<double> topology_2e1g_pattern::get_external_probability() const
    {
      return _tof_.external_probability;
    }

    bool topology_2e1g_pattern::has_delta_vertices_y() const
    {
      return datatools::is_valid(_DeltaV_.delta_vertices_y);
    }

    void topology_2e1g_pattern::set_delta_vertices_y(const double deltaV_y_)
    {
      _DeltaV_.delta_vertices_y = deltaV_y_;
      return;
    }

    double topology_2e1g_pattern::get_delta_vertices_y() const
    {
      return _DeltaV_.delta_vertices_y;
    }

    bool topology_2e1g_pattern::has_delta_vertices_z() const
    {
      return datatools::is_valid(_DeltaV_.delta_vertices_z);
    }

    void topology_2e1g_pattern::set_delta_vertices_z(const double deltaV_z_)
    {
      _DeltaV_.delta_vertices_z = deltaV_z_;
      return;
    }

    double topology_2e1g_pattern::get_delta_vertices_z() const
    {
      return _DeltaV_.delta_vertices_z;
    }

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

    // bool topology_2e1g_pattern::has_particle_pair() const
    // {
    //   return (_tof_.particle_pair.first.get().has_track_id &&
    //           _tof_.particle_pair.second.get().has_track_id);
    // }

    // particle_pair_type topology_2e1g_pattern::get_particle_pair() const
    // {
    //   return _tof_.particle_pair;
    // }

    // void topology_2e1g_pattern::set_particle_pair(const snemo::datamodel::particle_track & pt1_,
    //                                               const snemo::datamodel::particle_track & pt2_) const
    // {
    //   snemo::datamodel::particle_track::handle_type hpt1;
    //   snemo::datamodel::particle_track::handle_type hpt2;
    //   hpt1.grab () = pt1_;
    //   hpt2.grab () = pt2_;
    //   return _tof_.particle_pair = std::make_pair (hpt1,hpt2);
    // }

    void topology_2e1g_pattern::set_tof_dict(const snemo::datamodel::particle_track & pt1_,
                                             const snemo::datamodel::particle_track & pt2_,
                                             const topology_2e1g_pattern::TOF_measurement & tof_) const
    {
      snemo::datamodel::particle_track::handle_type hpt1;
      snemo::datamodel::particle_track::handle_type hpt2;
      hpt1.grab () = pt1_;
      hpt2.grab () = pt2_;
      topology_2e1g_pattern::particle_pair_type pt_pair = std::make_pair (hpt1,hpt2);
      _tof_dict_.insert(std::pair<topology_2e1g_pattern::particle_pair_type, double>(pt_pair,tof_));
      return;
    }

    void topology_2e1g_pattern::tree_dump(std::ostream      & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      for(size_t i=0; i<get_internal_probability().size();++i) {
        out_ << indent << datatools::i_tree_dumpable::tag
             << "Internal probability : " << get_internal_probability().at(i) << std::endl;
        out_ << indent << datatools::i_tree_dumpable::tag
             << "External probability : " << get_external_probability().at(i) << std::endl;
      }

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta Vertices Y : " << get_delta_vertices_y() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Delta Vertices Z : " << get_delta_vertices_z() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Angle Z : " << get_angle() << std::endl;

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
