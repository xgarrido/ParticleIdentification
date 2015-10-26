/// \file falaise/snemo/datamodels/topology_1e1Ng_pattern.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-24
 * Last modified: 2015-10-24
 *
 * Description: The 1e1Ng class of trajectory pattern
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1ENG_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1ENG_PATTERN_H 1

// This project:
#include <falaise/snemo/datamodels/topology_1e_pattern.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 1e1Ng class of reconstructed topology
    class topology_1eNg_pattern : public topology_1e_pattern
    {
    public:

      // /// Typedef for TOF dictionnary
      // typedef std::vector<tof_measurement> tof_collection_type;

      // /// Typedef for TOF dictionnary
      // typedef std::vector<angle_measurement> angle_collection_type;

      /// Return pattern identifier of the pattern
      virtual std::string pattern_id() const;

      /// Constructor
      topology_1eNg_pattern();

      /// Destructor
      virtual ~topology_1eNg_pattern();

      // /// Set number of gammas
      // void set_number_of_gammas(const size_t ngammas_);

      // /// Return internal probability
      // size_t get_number_of_gammas() const;

      // /// Return a non mutable reference to the TOF collection
      // const TOF_collection_type & get_TOF_collection() const;

      // /// Return a mutable reference to the TOF dictionary
      // TOF_collection_type & grab_TOF_collection();

      // /// Return a non mutable reference to the angle collection
      // const angle_collection_type & get_angle_collection() const;

      // /// Return a mutable reference to the angle dictionary
      // angle_collection_type & grab_angle_collection();

      // bool has_internal_probabilities() const;

      // bool has_external_probabilities() const;

      // /// Check electron energy validity
      // bool has_electron_energy() const;

      // /// Set maximal energy
      // void set_electron_energy(double);

      // /// Get electron energy
      // double get_electron_energy() const;

      // /// Check gamma maximal energy validity
      // bool has_gamma_max_energy() const;

      // /// Set gamma maximal energy
      // void set_gamma_max_energy(double);

      // /// Get gamma maximal energy
      // double get_gamma_max_energy() const;

      // /// Check gamma middle energy validity
      // bool has_gamma_mid_energy() const;

      // /// Set gamma middle energy
      // void set_gamma_mid_energy(double);

      // /// Get gamma middle energy
      // double get_gamma_mid_energy() const;

      // /// Check gamma minimal energy validity
      // bool has_gamma_min_energy() const;

      // /// Set gamma minimal energy
      // void set_gamma_min_energy(double);

      // /// Get gamma minimal energy
      // double get_gamma_min_energy() const;

      // /// Check total energy validity
      // bool has_total_energy() const;

      // /// Return total energy
      // double get_total_energy() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      // size_t _number_of_gammas_;
      // TOF_collection_type _tofs_;
      // angle_collection_type _angles_;
      // double _electron_energy_;
      // double _gamma_max_energy_;
      // double _gamma_mid_energy_;
      // double _gamma_min_energy_;

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

// #include <boost/serialization/export.hpp>
// BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_1eNg_pattern,
//                         "snemo::datamodel::topology_1eNg_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_1ENG_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
