/// \file falaise/snemo/datamodels/topology_2eNg_pattern.h
/* Author(s) :    François Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-03-19
 * Last modified: 2014-01-27
 *
 * Description: The 2eNg class of trajectory patterns
 */

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2ENG_PATTERN_H
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2ENG_PATTERN_H 1

// Standard library:
#include <string>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_2e_pattern.h>
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The 2 election - N gammas class of reconstructed topology
    class topology_2eNg_pattern : public topology_2e_pattern
    {
    public:

      /// Return pattern identifier of the pattern
      virtual std::string pattern_id() const;

      /// Constructor
      topology_2eNg_pattern();

      /// Destructor
      virtual ~topology_2eNg_pattern();

      // /// Set number of gammas
      // void set_number_of_gammas(const size_t ngammas_);

      // /// Return internal probability
      // size_t get_number_of_gammas() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      // size_t _number_of_gammas_;

      DATATOOLS_SERIALIZATION_DECLARATION();

    };

  } // end of namespace datamodel

} // end of namespace snemo

// #include <boost/serialization/export.hpp>
// BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::topology_2eNg_pattern,
//                         "snemo::datamodel::topology_2eNg_pattern")

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2ENG_PATTERN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
