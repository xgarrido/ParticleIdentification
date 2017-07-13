/// \file falaise/snemo/datamodels/tof_measurement_cut.h
/* Author(s) :    Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-11-02
 * Last modified: 2015-11-02
 *
 * Description:
 *
 *   The cut on the Time-Of-Flight measurement
 */

#ifndef FALAISE_SNEMO_CUT_TOF_MEASUREMENT_CUT_H
#define FALAISE_SNEMO_CUT_TOF_MEASUREMENT_CUT_H 1

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/real_range.h>
// - Bayeux/cuts:
#include <bayeux/cuts/i_cut.h>

namespace snemo {

  namespace cut {

    /// \brief A cut performed on individual 'tof measurement'
    class tof_measurement_cut : public cuts::i_cut
    {
    public:

      /// Mode of the cut
      enum mode_type {
        MODE_UNDEFINED                  = 0,
        MODE_HAS_INTERNAL_PROBABILITY   = datatools::bit_mask::bit01,
        MODE_HAS_EXTERNAL_PROBABILITY   = datatools::bit_mask::bit02,
        MODE_RANGE_INTERNAL_PROBABILITY = datatools::bit_mask::bit03,
        MODE_RANGE_EXTERNAL_PROBABILITY = datatools::bit_mask::bit04,
      };

      enum mode_range_type {
        MODE_RANGE_UNDEFINED = 0,
        MODE_RANGE_STRICT    = 1, //!< Require at least one probability
        MODE_RANGE_ALL       = 2  //!< Require all probabilities
      };

      /// Return the cut mode
      uint32_t get_mode() const;

      /// Check mode HAS_INTERNAL_PROBABILITY
      bool is_mode_has_internal_probability() const;

      /// Check mode RANGE_INTERNAL_PROBABILITY
      bool is_mode_range_internal_probability() const;

      /// Check mode HAS_EXTERNAL_PROBABILITY
      bool is_mode_has_external_probability() const;

      /// Check mode RANGE_EXTERNAL_PROBABILITY
      bool is_mode_range_external_probability() const;

      /// Constructor
      tof_measurement_cut(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~tof_measurement_cut();

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
      uint32_t _int_prob_range_mode_; //!< Mode when ranging internal probability
      uint32_t _ext_prob_range_mode_; //!< Mode when ranging internal probability
      datatools::real_range _int_prob_range_; //!< Internal probability range
      datatools::real_range _ext_prob_range_; //!< External probability range

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(tof_measurement_cut)
    };

  }  // end of namespace cut

}  // end of namespace snemo

// OCD support::
#include <bayeux/datatools/ocd_macros.h>

// @arg snemo::cut::tof_measurement_cut the name the registered class in the OCD system
DOCD_CLASS_DECLARATION(snemo::cut::tof_measurement_cut)

#endif // FALAISE_SNEMO_CUT_TOF_MEASUREMENT_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
