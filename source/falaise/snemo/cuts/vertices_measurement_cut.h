/// \file falaise/snemo/datamodels/vertices_measurement_cut.h
/* Author(s) :    Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-11-02
 * Last modified: 2015-11-02
 *
 * Description:
 *
 *   The cut on the common vertices measurement
 */

#ifndef FALAISE_SNEMO_CUT_VERTICES_MEASUREMENT_CUT_H
#define FALAISE_SNEMO_CUT_VERTICES_MEASUREMENT_CUT_H 1

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/bit_mask.h>
// - Bayeux/cuts:
#include <cuts/i_cut.h>

namespace snemo {

  namespace cut {

    /// \brief A cut performed on individual 'vertices measurement'
    class vertices_measurement_cut : public cuts::i_cut
    {
    public:

      /// Mode of the cut
      enum mode_type {
        MODE_UNDEFINED                  = 0,
        MODE_HAS_VERTICES_PROBABILITY   = datatools::bit_mask::bit01,
        MODE_RANGE_VERTICES_PROBABILITY = datatools::bit_mask::bit02,
        MODE_HAS_VERTICES_DISTANCE      = datatools::bit_mask::bit03,
        MODE_RANGE_VERTICES_DISTANCE_X  = datatools::bit_mask::bit04,
        MODE_RANGE_VERTICES_DISTANCE_Y  = datatools::bit_mask::bit05,
        MODE_RANGE_VERTICES_DISTANCE_Z  = datatools::bit_mask::bit06,
      };

      /// Return the cut mode
      uint32_t get_mode() const;

      /// Check mode FLAG
      bool is_mode_flag() const;

      /// Check mode HAS_VERTICES_PROBABILITY
      bool is_mode_has_vertices_probability() const;

      /// Check mode RANGE_VERTICES_PROBABILITY
      bool is_mode_range_vertices_probability() const;

      /// Check mode HAS_VERTICES_DISTANCE
      bool is_mode_has_vertices_distance() const;

      /// Check mode RANGE_VERTICES_DISTANCE_X
      bool is_mode_range_vertices_distance_x() const;

      /// Check mode RANGE_VERTICES_DISTANCE_y
      bool is_mode_range_vertices_distance_y() const;

      /// Check mode RANGE_VERTICES_DISTANCE_z
      bool is_mode_range_vertices_distance_z() const;

      /// Constructor
      vertices_measurement_cut(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~vertices_measurement_cut();

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

      uint32_t _mode_;             //!< Mode of the cut
      double _vertices_prob_range_min_; //!< Minimal vertices probability
      double _vertices_prob_range_max_; //!< Maximal vertices probability
      double _vertices_dist_x_range_min_; //!< Minimal vertices distance in x
      double _vertices_dist_x_range_max_; //!< Maximal vertices distance in x
      double _vertices_dist_y_range_min_; //!< Minimal vertices distance in y
      double _vertices_dist_y_range_max_; //!< Maximal vertices distance in y
      double _vertices_dist_z_range_min_; //!< Minimal vertices distance in z
      double _vertices_dist_z_range_max_; //!< Maximal vertices distance in z

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(vertices_measurement_cut);
    };

  }  // end of namespace cut

}  // end of namespace snemo

// OCD support::
#include <datatools/ocd_macros.h>

// @arg snemo::cut::vertices_measurement_cut the name the registered class in the OCD system
DOCD_CLASS_DECLARATION(snemo::cut::vertices_measurement_cut)

#endif // FALAISE_SNEMO_CUT_VERTICES_MEASUREMENT_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
