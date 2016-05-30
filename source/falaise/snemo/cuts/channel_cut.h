/// \file falaise/snemo/cuts/channel_cut.h
/* Author (s) :   Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-11-18
 * Last modified: 2015-11-16
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
 *  Channel cut to select topology data given cuts on topology measurement
 *
 * History:
 *
 */

#ifndef FALAISE_SNEMO_CUT_CHANNEL_CUT_H
#define FALAISE_SNEMO_CUT_CHANNEL_CUT_H 1

// Third party:
// - Bayeux/cuts
#include <bayeux/cuts/i_cut.h>

namespace snemo {

  namespace cut {

    /// \brief A channel cut
    class channel_cut : public cuts::i_cut
    {
    public:

      /// Alias type associating measurement with cut
      typedef std::pair<std::string, cuts::cut_handle_type> pair_type;

      /// Alias to collection of meas./cut association
      typedef std::vector<pair_type> cut_collection_type;

      /// Constructor
      channel_cut(datatools::logger::priority a_logging_priority = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~channel_cut();

      /// Initilization
      virtual void initialize(const datatools::properties & configuration_,
                              datatools::service_manager & service_manager_,
                              cuts::cut_handle_dict_type & cut_dict_);

      /// Reset
      virtual void reset();

    protected :

      /// Default values
      void _set_defaults();

      /// Selection
      virtual int _accept();

    private:

      std::string _TD_label_; //!< Topology Data bank label
      cut_collection_type _cuts_; //!< Collection of cut/meas.

      /// Macro to automate the registration of the cut
      CUT_REGISTRATION_INTERFACE(channel_cut)

    };

  } // end of namespace cut

}  // end of namespace snemo

// OCD support::
#include <datatools/ocd_macros.h>

// @arg snemo::cut::channel_cut the name the registered class in the OCD system
DOCD_CLASS_DECLARATION(snemo::cut::channel_cut)

#endif // FALAISE_SNEMO_CUT_CHANNEL_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
