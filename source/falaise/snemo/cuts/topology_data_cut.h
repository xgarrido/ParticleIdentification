/// \file falaise/snemo/cuts/topology_data_cut.h
/* Author(s)     : Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date : 2015-03-18
 * Last modified : 2015-03-18
 *
 * Copyright (C) 2015 Xavier Garrido <garrido@lal.in2p3.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Description:
 *
 *   Topology_Data event cut.
 *
 * History:
 *
 */

#ifndef FALAISE_SNEMO_CUT_TOPOLOGY_DATA_CUT_H
#define FALAISE_SNEMO_CUT_TOPOLOGY_DATA_CUT_H 1

// Standard library:
#include <string>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/bit_mask.h>
// - Bayeux/cuts:
#include <cuts/i_cut.h>

namespace snemo {

  namespace cut {

    /// \brief A topology_data event cut
    class topology_data_cut : public cuts::i_cut
    {
    public:

      /// Mode of the cut
      enum mode_type {
        MODE_UNDEFINED              = 0,
        MODE_HAS_PATTERN_ID         = datatools::bit_mask::bit01,
        MODE_PATTERN_ID             = datatools::bit_mask::bit02,
        MODE_RANGE_NUMBER_OF_GAMMAS = datatools::bit_mask::bit03
      };

      /// Return the cut mode
      uint32_t get_mode() const;

      /// Check mode HAS_PATTERN_ID
      bool is_mode_has_pattern_id() const;

      /// Check mode PATTERN_ID
      bool is_mode_pattern_id() const;

      /// Check mode RANGE_NUMBER_OF_GAMMAS
      bool is_mode_range_number_of_gammas() const;

      /// Constructor
      topology_data_cut(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~topology_data_cut();

      /// Initilization
      virtual void initialize(const datatools::properties & configuration_,
                              datatools::service_manager & service_manager_,
                              cuts::cut_handle_dict_type & cut_dict_);

      /// Reset
      virtual void reset();

    protected:

      /// Default values
      void _set_defaults();

      /// Selection
      virtual int _accept();

    private:

      std::string _TD_label_; //!< Name of the "Topology data" bank
      uint32_t    _mode_;     //!< Mode of the cut

      std::string _pattern_id_label_; //!< Pattern ID label
      int _number_of_gammas_min_; //!< Minimal number of gammas
      int _number_of_gammas_max_; //!< Maximal number of gammas

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(topology_data_cut);
    };

  }  // end of namespace cut

}  // end of namespace snemo

#endif // FALAISE_SNEMO_CUT_TOPOLOGY_DATA_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
