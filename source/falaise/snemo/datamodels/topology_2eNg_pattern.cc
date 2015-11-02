/** \file falaise/snemo/datamodels/topology_2eNg_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_2eNg_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_2eNg_pattern,
                                                      "snemo::datamodel::topology_2eNg_pattern")

    std::string topology_2eNg_pattern::pattern_id() const
    {
      const std::string id("2eNg");
      return id;
    }

    topology_2eNg_pattern::topology_2eNg_pattern()
      : topology_2e_pattern()
    {
      _number_of_gammas_ = -1;
      return;
    }

    topology_2eNg_pattern::~topology_2eNg_pattern()
    {
      return;
    }

    bool topology_2eNg_pattern::has_number_of_gammas() const
    {
      return _number_of_gammas_ != -1;
    }

    void topology_2eNg_pattern::set_number_of_gammas(const int ngammas_)
    {
      _number_of_gammas_ = ngammas_;
      return;
    }

    int topology_2eNg_pattern::get_number_of_gammas() const
    {
      return _number_of_gammas_;
    }

    bool topology_2eNg_pattern::has_electrons_gammas_internal_probabilities() const
    {
      return has_measurement("tof_e1_g1") && has_measurement("tof_e2_g1");
    }

    bool topology_2eNg_pattern::has_electrons_gammas_external_probabilities() const
    {
      return has_measurement("tof_e1_g1") && has_measurement("tof_e2_g1");
    }

    const topology_2eNg_pattern::tof_collection_type & topology_2eNg_pattern::get_electrons_gammas_internal_probabilities() const {
      DT_THROW_IF(! has_electrons_gammas_internal_probabilities(), std::logic_error, "No electrons-gammas TOF measurement stored !");
      topology_2eNg_pattern::tof_collection_type tofs_int;
      for(size_t i_gamma = 1; i_gamma <= get_number_of_gammas(); ++i_gamma) {
        std::ostringstream oss;
        oss << "tof_e1_g" << i_gamma;
        std::ostringstream oss2;
        oss2 << "tof_e2_g" << i_gamma;
        tofs_int.push_back(dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement(oss.str())).get_internal_probabilities());
        tofs_int.push_back(dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement(oss2.str())).get_internal_probabilities());
      }
      return tofs_int;
    }

    const topology_2eNg_pattern::tof_collection_type & topology_2eNg_pattern::get_electrons_gammas_external_probabilities() const {
      DT_THROW_IF(! has_electrons_gammas_external_probabilities(), std::logic_error, "No electrons-gammas TOF measurement stored !");
      topology_2eNg_pattern::tof_collection_type tofs_ext;
      for(size_t i_gamma = 1; i_gamma <= get_number_of_gammas(); ++i_gamma) {
        std::ostringstream oss;
        oss << "tof_e1_g" << i_gamma;
        std::ostringstream oss2;
        oss2 << "tof_e2_g" << i_gamma;
        tofs_ext.push_back(dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement(oss.str())).get_external_probabilities());
        tofs_ext.push_back(dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement(oss2.str())).get_external_probabilities());
      }
      return tofs_ext;
    }

    void topology_2eNg_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      // for(size_t i=0; i<get_internal_probability().size();++i) {
      //   out_ << indent << datatools::i_tree_dumpable::tag
      //        << "Internal probability : " << get_internal_probability().at(i) << std::endl;
      //   out_ << indent << datatools::i_tree_dumpable::tag
      //        << "External probability : " << get_external_probability().at(i) << std::endl;
      // }

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
