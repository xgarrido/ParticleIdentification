// falaise/snemo/cuts/channel_1e1p_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/channel_1e1p_cut.h>

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
#include <falaise/snemo/datamodels/topology_1e1p_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(channel_1e1p_cut, "snemo::cut::channel_1e1p_cut");

    void channel_1e1p_cut::_set_defaults()
    {
      base_channel_cut::_set_defaults();
      return;
    }

    channel_1e1p_cut::channel_1e1p_cut(datatools::logger::priority logger_priority_)
      : base_channel_cut::base_channel_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    channel_1e1p_cut::~channel_1e1p_cut()
    {
      if (is_initialized()) this->channel_1e1p_cut::reset();
      return;
    }

    void channel_1e1p_cut::reset()
    {
      _set_defaults();
      this->base_channel_cut::_reset();
      this->base_channel_cut::_set_initialized(false);
      return;
    }

    void channel_1e1p_cut::initialize(const datatools::properties & configuration_,
                                      datatools::service_manager  & service_manager_,
                                      cuts::cut_handle_dict_type  & cut_dict_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");
      base_channel_cut::initialize(configuration_, service_manager_, cut_dict_);
      // this->i_cut::_set_initialized(true);
      return;
    }


    int channel_1e1p_cut::_accept()
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
      const std::string & a_pattern_id = a_pattern.pattern_id();

      std::string pattern_id_1e1p;
      {
        snemo::datamodel::topology_1e1p_pattern * a_1e1p_pattern = new snemo::datamodel::topology_1e1p_pattern;
        pattern_id_1e1p = a_1e1p_pattern->pattern_id();
      }
      if (a_pattern_id != pattern_id_1e1p) {
        DT_LOG_DEBUG(get_logging_priority(), "This cut is only applicable to '"
                     << pattern_id_1e1p << "' topology !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_1e1p_pattern & a_1e1p_pattern
        = dynamic_cast<const snemo::datamodel::topology_1e1p_pattern &>(a_pattern);

      // Check if event has internal probability
      bool check_has_internal_probability = true;
      if (is_mode_has_internal_probability()) {
        if (! a_1e1p_pattern.has_electron_positron_internal_probability()) {
          check_has_internal_probability = false;
        }
      }

      // Check if event has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (! a_1e1p_pattern.has_electron_positron_internal_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double pint = a_1e1p_pattern.get_electron_positron_internal_probability();
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
        if (! a_1e1p_pattern.has_electron_positron_external_probability()) {
          check_has_external_probability = false;
        }
      }


      // Check if event has common vertices probability
      bool check_has_vertices_probability = true;
      if (is_mode_has_vertices_probability()) {
        if (! a_1e1p_pattern.has_electron_positron_vertices_probability()) {
          check_has_vertices_probability = false;
        }
      }

      // Check if event has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (! a_1e1p_pattern.has_electron_positron_external_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double pext = a_1e1p_pattern.get_electron_positron_external_probability();
        if (datatools::is_valid(_prob_ext_min_)) {
          if (pext < _prob_ext_min_) check_range_external_probability = false;
        }
        if (datatools::is_valid(_prob_ext_max_)) {
          if (pext > _prob_ext_max_) check_range_external_probability = false;
        }
      }

      // Check if event has required common vertices probability
      bool check_range_vertices_probability = true;
      if (is_mode_range_vertices_probability()) {
        if (! a_1e1p_pattern.has_electron_positron_vertices_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing common vertices probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        // const std::string vertices_probability_location = a_1e1p_pattern.get_electron_positron_vertices_location();
        // if (vertices_probability_location != _vertices_probability_location_) {
        //   DT_LOG_DEBUG(get_logging_priority(),
        //                "Common vertices location (" << vertices_probability_location << ") different than " << _vertices_probability_location_);
        //   check_range_vertices_probability = false;
        // }
        const double vertices_probability = a_1e1p_pattern.get_electron_positron_vertices_probability();
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
        if (! a_1e1p_pattern.has_electron_positron_angle()) {
            check_has_angle = false;
        }
      }

      // Check if event has required angle
      bool check_range_angle = true;
      if (is_mode_range_angle()) {
        if (! a_1e1p_pattern.has_electron_positron_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing angle !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double angle = a_1e1p_pattern.get_electron_positron_angle();
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

      // Check if event has minimal energy
      bool check_has_minimal_energy = true;
      if (is_mode_has_minimal_energy()) {
        if (! a_1e1p_pattern.has_electron_positron_minimal_energy()) {
            check_has_minimal_energy = false;
        }
      }

      // Check if event has required electron minimal energy
      bool check_range_minimal_energy = true;
      if (is_mode_range_minimal_energy()) {
        if (! a_1e1p_pattern.has_electron_positron_minimal_energy()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing minimal energy !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double minimal_energy = a_1e1p_pattern.get_electron_positron_minimal_energy();
        if (datatools::is_valid(_minimal_energy_min_)) {
          if ( minimal_energy < _minimal_energy_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Minimal energy (" << minimal_energy << ") lower than " << _minimal_energy_min_);
            check_range_minimal_energy = false;
          }
        }
        if (datatools::is_valid(_minimal_energy_max_)) {
          if (minimal_energy > _minimal_energy_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Minimal energy (" << minimal_energy << ") greater than " << _minimal_energy_max_);
            check_range_minimal_energy = false;
          }
        }
      }

      // Check if event has maximal energy
      bool check_has_maximal_energy = true;
      if (is_mode_has_maximal_energy()) {
        if (! a_1e1p_pattern.has_electron_positron_maximal_energy()) {
            check_has_maximal_energy = false;
        }
      }

      // Check if event has required electron maximal energy
      bool check_range_maximal_energy = true;
      if (is_mode_range_maximal_energy()) {
        if (! a_1e1p_pattern.has_electron_positron_maximal_energy()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing maximal energy !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double maximal_energy = a_1e1p_pattern.get_electron_positron_maximal_energy();
        if (datatools::is_valid(_maximal_energy_min_)) {
          if ( maximal_energy < _maximal_energy_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Maximal energy (" << maximal_energy << ") lower than " << _maximal_energy_min_);
            check_range_maximal_energy = false;
          }
        }
        if (datatools::is_valid(_maximal_energy_max_)) {
          if (maximal_energy > _maximal_energy_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Maximal energy (" << maximal_energy << ") greater than " << _maximal_energy_max_);
            check_range_maximal_energy = false;
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
          check_range_angle &&
          check_has_minimal_energy &&
          check_range_minimal_energy &&
          check_has_maximal_energy &&
          check_range_maximal_energy) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by channel 1e1p cut!");
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
