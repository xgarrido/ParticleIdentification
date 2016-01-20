// falaise/snemo/cuts/channel_2p_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/channel_2p_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_2p_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(channel_2p_cut, "snemo::cut::channel_2p_cut");

    void channel_2p_cut::_set_defaults()
    {
      base_channel_cut::_set_defaults();
      return;
    }
    channel_2p_cut::channel_2p_cut(datatools::logger::priority logger_priority_)
      : base_channel_cut::base_channel_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    channel_2p_cut::~channel_2p_cut()
    {
      if (is_initialized()) this->channel_2p_cut::reset();
      return;
    }

    void channel_2p_cut::reset()
    {
      _set_defaults();
      this->base_channel_cut::_reset();
      this->base_channel_cut::_set_initialized(false);
      return;
    }

    void channel_2p_cut::initialize(const datatools::properties & configuration_,
                                    datatools::service_manager  & service_manager_,
                                    cuts::cut_handle_dict_type  & cut_dict_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");
      base_channel_cut::initialize(configuration_, service_manager_, cut_dict_);
      // this->base_channel_cut::initialize(configuration_);
      return;
    }

    int channel_2p_cut::_accept()
    {
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cut_returned;
      }

      const snemo::datamodel::topology_data & TD = ER.get<snemo::datamodel::topology_data>(_TD_label_);
      if (! TD.has_pattern()) {
        DT_LOG_DEBUG(get_logging_priority(), "Missing topology pattern !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::base_topology_pattern & a_pattern = TD.get_pattern();
      if (a_pattern.get_pattern_id() != snemo::datamodel::topology_2p_pattern::pattern_id()) {
        DT_LOG_DEBUG(get_logging_priority(), "This cut is only applicable to '"
                     << snemo::datamodel::topology_2p_pattern::pattern_id() << "' topology !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_2p_pattern & a_2p_pattern
        = dynamic_cast<const snemo::datamodel::topology_2p_pattern &>(a_pattern);

      // Check if event has internal probability
      bool check_has_internal_probability = true;
      if (is_mode_has_internal_probability()) {
        if (! a_2p_pattern.has_electrons_internal_probability()) {
          check_has_internal_probability = false;
        }
      }

      // Check if event has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (! a_2p_pattern.has_electrons_internal_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double pint = a_2p_pattern.get_electrons_internal_probability();
        if (datatools::is_valid(_prob_int_min_)) {
          if (pint < _prob_int_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Internal probability (" << pint << ") lower than " << _prob_int_min_);
            check_range_internal_probability = false;
          }
        }
        if (datatools::is_valid(_prob_int_max_)) {
          if (pint > _prob_int_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Internal probability (" << pint << ") greater than " << _prob_int_max_);
            check_range_internal_probability = false;
          }
        }
      }

      // Check if event has external probability
      bool check_has_external_probability = true;
      if (is_mode_has_external_probability()) {
        if (! a_2p_pattern.has_electrons_external_probability()) {
          check_has_external_probability = false;
        }
      }

      // Check if event has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (! a_2p_pattern.has_electrons_external_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double pext = a_2p_pattern.get_electrons_external_probability();
        if (datatools::is_valid(_prob_ext_min_)) {
          if (pext < _prob_ext_min_) check_range_external_probability = false;
        }
        if (datatools::is_valid(_prob_ext_max_)) {
          if (pext > _prob_ext_max_) check_range_external_probability = false;
        }
      }

      // Check if event has common vertices probability
      bool check_has_vertices_probability = true;
      if (is_mode_has_vertices_probability()) {
        if (! a_2p_pattern.has_electrons_vertices_probability()) {
          check_has_vertices_probability = false;
        }
      }

      // Check if event has required common vertices probability
      bool check_range_vertices_probability = true;
      if (is_mode_range_vertices_probability()) {
        if (! a_2p_pattern.has_electrons_vertices_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing common vertices probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        // const std::string vertices_probability_location = a_2p_pattern.get_electrons_vertices_location();
        // if (vertices_probability_location != _vertices_probability_location_) {
        //   DT_LOG_DEBUG(get_logging_priority(),
        //                "Common vertices location (" << vertices_probability_location << ") different than " << _vertices_probability_location_);
        //   check_range_vertices_probability = false;
        // }
        const double vertices_probability = a_2p_pattern.get_electrons_vertices_probability();
        if (datatools::is_valid(_vertices_probability_min_)) {
          if (vertices_probability < _vertices_probability_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Common vertices probability (" << vertices_probability << ") lower than " << _vertices_probability_min_);
            check_range_vertices_probability = false;
          }
        }
        if (datatools::is_valid(_vertices_probability_max_)) {
          if (vertices_probability > _vertices_probability_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices probability (" << vertices_probability << ") greater than " << _vertices_probability_max_);
            check_range_vertices_probability = false;
          }
        }
      }

      // Check if event has angle
      bool check_has_angle = true;
      if (is_mode_has_angle()) {
        if (! a_2p_pattern.has_electrons_angle()) {
            check_has_angle = false;
        }
      }

      // Check if event has required angle
      bool check_range_angle = true;
      if (is_mode_range_angle()) {
        if (! a_2p_pattern.has_electrons_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing angle !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double angle = a_2p_pattern.get_electrons_angle();
        if (datatools::is_valid(_angle_min_)) {
          if (angle < _angle_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle << ") lower than " << _angle_min_);
            check_range_angle = false;
          }
        }
        if (datatools::is_valid(_angle_max_)) {
          if (angle > _angle_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle << ") greater than " << _angle_max_);
            check_range_angle = false;
          }
        }
      }


      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_internal_probability &&
          check_has_external_probability &&
          check_range_internal_probability &&
          check_range_external_probability &&
          check_has_vertices_probability &&
          check_range_vertices_probability &&
          check_has_angle &&
          check_range_angle) {
        DT_LOG_DEBUG(get_logging_priority(), "Event accepted by channel 2p cut!");
        cut_returned = cuts::SELECTION_ACCEPTED;
      }

      return cut_returned;
    }

  }  // end of namespace cut

}  // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
