/** \file falaise/snemo/datamodels/topology_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(TOF_measurement,
                                                      "snemo::datamodel::TOF_measurement")

    TOF_measurement::TOF_measurement()
    {
      return;
    }

    TOF_measurement::~TOF_measurement()
    {
      return;
    }

    const TOF_measurement::probability_type & TOF_measurement::get_internal_probabilities() const
    {
      return _internal_probabilities_;
    }

    TOF_measurement::probability_type & TOF_measurement::grab_internal_probabilities()
    {
      return _internal_probabilities_;
    }

    const TOF_measurement::probability_type & TOF_measurement::get_external_probabilities() const
    {
      return _external_probabilities_;
    }

    TOF_measurement::probability_type & TOF_measurement::grab_external_probabilities()
    {
      return _external_probabilities_;
    }

    bool TOF_measurement::has_particle_tracks() const
    {
      return true;
    }

    const TOF_measurement::particle_pair_type & TOF_measurement::get_particle_tracks() const
    {
      return _particle_track_pair_;
    }

    TOF_measurement::particle_pair_type & TOF_measurement::grab_particle_tracks()
    {
      return _particle_track_pair_;
    }

    void TOF_measurement::set_particle_tracks(const snemo::datamodel::particle_track::handle_type & hpt1_,
                                              const snemo::datamodel::particle_track::handle_type & hpt2_)
    {
      _particle_track_pair_ = std::make_pair(hpt1_, hpt2_);
      return;
    }

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(delta_vertices_measurement,
                                                      "snemo::datamodel::delta_vertices_measurement")

    delta_vertices_measurement::delta_vertices_measurement()
    {
      return;
    }

    delta_vertices_measurement::~delta_vertices_measurement()
    {
      return;
    }

    const double & delta_vertices_measurement::get_delta_vertices_y() const
    {
      return _delta_vertices_y_;
    }

    double & delta_vertices_measurement::grab_delta_vertices_y()
    {
      return _delta_vertices_y_;
    }

    const double & delta_vertices_measurement::get_delta_vertices_z() const
    {
      return _delta_vertices_z_;
    }

    double & delta_vertices_measurement::grab_delta_vertices_z()
    {
      return _delta_vertices_z_;
    }

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(angle_measurement,
                                                      "snemo::datamodel::angle_measurement")

    angle_measurement::angle_measurement()
    {
      return;
    }

    angle_measurement::~angle_measurement()
    {
      return;
    }

    const double & angle_measurement::get_angle() const
    {
      return _angle_;
    }

    double & angle_measurement::grab_angle()
    {
      return _angle_;
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
