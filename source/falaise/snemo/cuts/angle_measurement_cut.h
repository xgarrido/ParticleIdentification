/// \file falaise/snemo/datamodels/angle_measurement_cut.h
/* Author(s) :    Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-11-02
 * Last modified: 2017-07-07
 *
 * Description:
 *
 *   The cut on the angle measurement
 *
 * History:
 *
 *   2017-07-07: use datatools::real_range
 */

#ifndef FALAISE_SNEMO_CUT_ANGLE_MEASUREMENT_CUT_H
#define FALAISE_SNEMO_CUT_ANGLE_MEASUREMENT_CUT_H 1

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/real_range.h>
// - Bayeux/cuts:
#include <bayeux/cuts/i_cut.h>

namespace snemo {

  namespace cut {

    /// \brief A cut performed on individual 'angle measurement'
    class angle_measurement_cut : public cuts::i_cut
    {
    public:

      /// Mode of the cut
      enum mode_type {
        MODE_UNDEFINED   = 0,
        MODE_HAS_ANGLE   = datatools::bit_mask::bit01,
        MODE_RANGE_ANGLE = datatools::bit_mask::bit02,
      };

      /// Return the cut mode
      uint32_t get_mode() const;

      /// Check mode FLAG
      bool is_mode_flag() const;

      /// Check mode HAS_ANGLE
      bool is_mode_has_angle() const;

      /// Check mode RANGE_ANGLE
      bool is_mode_range_angle() const;

      /// Constructor
      angle_measurement_cut(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~angle_measurement_cut();

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

      uint32_t _mode_;                     //!< Mode of the cut
      datatools::real_range _angle_range_; //!< Allowed angle value

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(angle_measurement_cut)
    };

  }  // end of namespace cut

}  // end of namespace snemo

// OCD support::
#include <datatools/ocd_macros.h>

// @arg snemo::cut::angle_measurement_cut the name the registered class in the OCD system
DOCD_CLASS_DECLARATION(snemo::cut::angle_measurement_cut)

#endif // FALAISE_SNEMO_CUT_ANGLE_MEASUREMENT_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
