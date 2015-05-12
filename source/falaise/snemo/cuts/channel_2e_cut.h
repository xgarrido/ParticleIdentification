/// \file falaise/snemo/cuts/channel_2e_cut.h
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
 *   Channel_2e event cut.
 *
 * History:
 *
 */

#ifndef FALAISE_SNEMO_CUT_CHANNEL_2E_CUT_H
#define FALAISE_SNEMO_CUT_CHANNEL_2E_CUT_H 1

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

    /// \brief A channel_2e event cut
    class channel_2e_cut : public cuts::i_cut
    {
    public:

      /// Mode of the cut
      enum mode_type {
        MODE_UNDEFINED = 0,
        MODE_HAS_INTERNAL_PROBABILITY   = datatools::bit_mask::bit01,
        MODE_HAS_EXTERNAL_PROBABILITY   = datatools::bit_mask::bit02,
        MODE_RANGE_INTERNAL_PROBABILITY = datatools::bit_mask::bit03,
        MODE_RANGE_EXTERNAL_PROBABILITY = datatools::bit_mask::bit04,
        MODE_HAS_DELTA_VERTICES_Y       = datatools::bit_mask::bit05,
        MODE_HAS_DELTA_VERTICES_Z       = datatools::bit_mask::bit06,
        MODE_RANGE_DELTA_VERTICES_Y     = datatools::bit_mask::bit07,
        MODE_RANGE_DELTA_VERTICES_Z     = datatools::bit_mask::bit08,
        MODE_HAS_ANGLE                  = datatools::bit_mask::bit09,
        MODE_RANGE_ANGLE                = datatools::bit_mask::bit10
      };

      /// Return the cut mode
      uint32_t get_mode() const;

      /// Check mode HAS_INTERNAL_PROBABILITY
      bool is_mode_has_internal_probability() const;

      /// Check mode HAS_EXTERNAL_PROBABILITY
      bool is_mode_has_external_probability() const;

      /// Check mode RANGE_INTERNAL_PROBABILITY
      bool is_mode_range_internal_probability() const;

      /// Check mode RANGE_EXTERNAL_PROBABILITY
      bool is_mode_range_external_probability() const;

      /// Check mode HAS_DELTA_VERTICES_Y
      bool is_mode_has_delta_vertices_y() const;

      /// Check mode HAS_DELTA_VERTICES_Z
      bool is_mode_has_delta_vertices_z() const;

      /// Check mode RANGE_DELTA_VERTICES_Y
      bool is_mode_range_delta_vertices_y() const;

      /// Check mode RANGE_DELTA_VERTICES_Z
      bool is_mode_range_delta_vertices_z() const;

      /// Check mode HAS_ANGLE
      bool is_mode_has_angle() const;

      /// Check mode RANGE_ANGLE
      bool is_mode_range_angle() const;

      /// Constructor
      channel_2e_cut(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~channel_2e_cut();

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

      double _prob_int_min_;//!< Minimal internal probability
      double _prob_int_max_;//!< Maximal internal probability
      double _prob_ext_min_;//!< Minimal external probability
      double _prob_ext_max_;//!< Maximal external probability
      double _delta_vertices_y_min_;
      double _delta_vertices_y_max_;
      double _delta_vertices_z_min_;
      double _delta_vertices_z_max_;
      double _angle_min_;
      double _angle_max_;

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(channel_2e_cut);
    };

  }  // end of namespace cut

}  // end of namespace snemo

#endif // FALAISE_SNEMO_CUT_CHANNEL_2E_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
