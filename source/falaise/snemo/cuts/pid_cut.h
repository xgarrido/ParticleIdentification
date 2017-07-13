/// \file falaise/snemo/cuts/pid_cut.h
/* Author(s)     : Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date : 2015-05-15
 * Last modified : 2015-05-15
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
 *   PID event cut.
 *
 * History:
 *
 */

#ifndef FALAISE_SNEMO_CUT_PID_CUT_H
#define FALAISE_SNEMO_CUT_PID_CUT_H 1

// Standard library:
#include <string>

// Third party:
#include <bayeux/datatools/integer_range.h>
// - Bayeux/cuts:
#include <bayeux/cuts/i_cut.h>

namespace snemo {

  namespace cut {

    /// \brief A pid event cut
    class pid_cut : public cuts::i_cut
    {
    public:

      /// Constructor
      pid_cut(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~pid_cut();

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

      std::string _PTD_label_; //!< Name of the "Particle track data" bank

      datatools::integer_range _electron_range_; //!< Number of electrons
      datatools::integer_range _positron_range_; //!< Number of positrons
      datatools::integer_range _gamma_range_;    //!< Number of gammas
      datatools::integer_range _alpha_range_;    //!< Number of alphas
      datatools::integer_range _undefined_range_;//!< Number of undefined particles

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(pid_cut)
    };

  }  // end of namespace cut

}  // end of namespace snemo

#endif // FALAISE_SNEMO_CUT_PID_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
