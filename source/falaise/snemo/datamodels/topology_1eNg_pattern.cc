/** \file falaise/snemo/datamodels/topology_1eNg_pattern.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_1eNg_pattern.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(topology_1eNg_pattern,
                                                      "snemo::datamodel::topology_1eNg_pattern")

    std::string topology_1eNg_pattern::pattern_id() const
    {
      const std::string id("1eNg");
      return id;
    }

    topology_1eNg_pattern::topology_1eNg_pattern()
      : topology_1e_pattern()
    {
      _number_of_gammas_ = -1;
      return;
    }

    topology_1eNg_pattern::~topology_1eNg_pattern()
    {
      return;
    }

    bool topology_1eNg_pattern::has_number_of_gammas() const
    {
      return _number_of_gammas_ != -1;
    }

    void topology_1eNg_pattern::set_number_of_gammas(const int ngammas_)
    {
      _number_of_gammas_ = ngammas_;
      return;
    }

    int topology_1eNg_pattern::get_number_of_gammas() const
    {
      return _number_of_gammas_;
    }

    bool topology_1eNg_pattern::has_electron_gammas_internal_probabilities() const
    {
      // bool check_pair_has_internal_proba = true;
      // for(unsigned int i=0; i<get_tof_collection().size(); i++)
      //   if(get_tof_collection().at(i).get_internal_probabilities().empty())
      //     check_pair_has_internal_proba = false;

      // return check_pair_has_internal_proba;

      return has_measurement("tof_e1_g1");
    }

    bool topology_1eNg_pattern::has_electron_gammas_external_probabilities() const
    {
      // bool check_pair_has_external_proba = true;
      // for(unsigned int i=0; i<get_tof_collection().size(); i++)
      //   if(get_tof_collection().at(i).get_external_probabilities().empty())
      //     check_pair_has_external_proba = false;

      // return check_pair_has_external_proba;
      return has_measurement("tof_e1_g1");
    }

    const topology_1eNg_pattern::tof_collection_type & topology_1eNg_pattern::get_electron_gammas_internal_probabilities() const {
      DT_THROW_IF(! has_electron_gammas_internal_probabilities(), std::logic_error, "No electron-gammas TOF measurement stored !");
      topology_1eNg_pattern::tof_collection_type tofs_int;
      for(size_t i_gamma = 1; i_gamma <= get_number_of_gammas(); ++i_gamma) {
        std::ostringstream oss;
        oss << "tof_e1_g" << i_gamma;
        tofs_int.push_back(dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement(oss.str())).get_internal_probabilities());
      }
      return tofs_int;
    }

    const topology_1eNg_pattern::tof_collection_type & topology_1eNg_pattern::get_electron_gammas_external_probabilities() const {
      DT_THROW_IF(! has_electron_gammas_external_probabilities(), std::logic_error, "No electron-gammas TOF measurement stored !");
      topology_1eNg_pattern::tof_collection_type tofs_ext;
      for(size_t i_gamma = 1; i_gamma <= get_number_of_gammas(); ++i_gamma) {
        std::ostringstream oss;
        oss << "tof_e1_g" << i_gamma;
        tofs_ext.push_back(dynamic_cast<const snemo::datamodel::tof_measurement&> (get_measurement(oss.str())).get_external_probabilities());
      }
      return tofs_ext;
    }

    // const topology_1eNg_pattern::angle_collection_type & topology_1eNg_pattern::get_angle_collection() const
    // {
    //   return _angles_;
    // }

    // topology_1eNg_pattern::angle_collection_type & topology_1eNg_pattern::grab_angle_collection()
    // {
    //   return _angles_;
    // }

    // bool topology_1eNg_pattern::has_electron_energy() const
    // {
    //   return datatools::is_valid(_electron_energy_);
    // }

    // void topology_1eNg_pattern::set_electron_energy(double energy_)
    // {
    //   _electron_energy_ = energy_;
    //   return;
    // }

    // double topology_1eNg_pattern::get_electron_energy() const
    // {
    //   return _electron_energy_;
    // }

    // bool topology_1eNg_pattern::has_gamma_max_energy() const
    // {
    //   return datatools::is_valid(_gamma_max_energy_);
    // }

    // void topology_1eNg_pattern::set_gamma_max_energy(double energy_)
    // {
    //   _gamma_max_energy_ = energy_;
    //   return;
    // }

    // double topology_1eNg_pattern::get_gamma_max_energy() const
    // {
    //   return _gamma_max_energy_;
    // }

    // bool topology_1eNg_pattern::has_gamma_mid_energy() const
    // {
    //   return datatools::is_valid(_gamma_mid_energy_);
    // }

    // void topology_1eNg_pattern::set_gamma_mid_energy(double energy_)
    // {
    //   _gamma_mid_energy_ = energy_;
    //   return;
    // }

    // double topology_1eNg_pattern::get_gamma_mid_energy() const
    // {
    //   return _gamma_mid_energy_;
    // }

    // bool topology_1eNg_pattern::has_gamma_min_energy() const
    // {
    //   return datatools::is_valid(_gamma_min_energy_);
    // }

    // void topology_1eNg_pattern::set_gamma_min_energy(double energy_)
    // {
    //   _gamma_min_energy_ = energy_;
    //   return;
    // }

    // double topology_1eNg_pattern::get_gamma_min_energy() const
    // {
    //   return _gamma_min_energy_;
    // }

    // bool topology_1eNg_pattern::has_total_energy() const
    // {
    //   if(get_number_of_gammas() == 1)
    //     return has_electron_energy() && has_gamma_max_energy();
    //   else if(get_number_of_gammas() == 2)
    //     return has_electron_energy() && has_gamma_max_energy()
    //       && has_gamma_min_energy();
    //   else if(get_number_of_gammas() == 3)
    //     return get_electron_energy() && get_gamma_max_energy()
    //       && get_gamma_mid_energy() && get_gamma_min_energy();
    //   else
    //     return false;
    // }

    // double topology_1eNg_pattern::get_total_energy() const
    // {
    //   if(get_number_of_gammas() == 1)
    //     return get_electron_energy() + get_gamma_max_energy();
    //   else if(get_number_of_gammas() == 2)
    //     return get_electron_energy() + get_gamma_max_energy()
    //       + get_gamma_min_energy();
    //   else if(get_number_of_gammas() == 3)
    //     return get_electron_energy() + get_gamma_max_energy()
    //       + get_gamma_mid_energy() + get_gamma_min_energy();
    //   else
    //     return 0;
    // }

    // // bool topology_1eNg_pattern::has_tof_measurement() const
    // // {
    // //   return has_internal_probability() && has_external_probability();
    // // }

    void topology_1eNg_pattern::tree_dump(std::ostream      & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool /*inherit_*/) const
    {
      std::string indent;
      if (! indent_.empty()) indent = indent_;
      base_topology_pattern::tree_dump(out_, title_, indent_, true);

      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "Internal probability : " << get_internal_probability() << std::endl;
      // out_ << indent << datatools::i_tree_dumpable::tag
      //      << "External probability : " << get_external_probability() << std::endl;

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
