// falaise/snemo/cuts/topology_data_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/topology_data_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <regex>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(topology_data_cut, "snemo::cut::topology_data_cut")

    void topology_data_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      return;
    }

    uint32_t topology_data_cut::get_mode() const
    {
      return _mode_;
    }

    bool topology_data_cut::is_mode_has_pattern() const
    {
      return _mode_ & MODE_HAS_PATTERN;
    }

    bool topology_data_cut::is_mode_has_classification() const
    {
      return _mode_ & MODE_HAS_CLASSIFICATION;
    }

    bool topology_data_cut::is_mode_classification() const
    {
      return _mode_ & MODE_CLASSIFICATION;
    }

    bool topology_data_cut::is_mode_no_pile_up() const
    {
      return _mode_ & MODE_NO_PILE_UP;
    }

    topology_data_cut::topology_data_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    topology_data_cut::~topology_data_cut()
    {
      if (is_initialized()) this->topology_data_cut::reset();
      return;
    }

    void topology_data_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void topology_data_cut::initialize(const datatools::properties & configuration_,
                                       datatools::service_manager  & /* service_manager_ */,
                                       cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }
      if (configuration_.has_flag("mode.has_pattern")) {
        _mode_ |= MODE_HAS_PATTERN;
      }
      if (configuration_.has_flag("mode.has_classification")) {
        _mode_ |= MODE_HAS_CLASSIFICATION;
      }
      if (configuration_.has_flag("mode.classification")) {
        _mode_ |= MODE_CLASSIFICATION;
      }
      if (configuration_.has_flag("mode.no_pile_up")) {
        _mode_ |= MODE_NO_PILE_UP;
      }
      DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                  "Missing at least a 'mode.XXX' property !");

      if (is_mode_classification()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using CLASSIFICATION mode...");
        DT_THROW_IF(! configuration_.has_key("classification.label"), std::logic_error,
                    "Missing 'classification.label' !");
        _classification_label_ = configuration_.fetch_string("classification.label");
      }

      this->i_cut::_set_initialized(true);
      return;
    }

    int topology_data_cut::_accept()
    {
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cut_returned;
      }

      const snemo::datamodel::topology_data & TD = ER.get<snemo::datamodel::topology_data>(_TD_label_);

      // Check if event has pattern
      bool check_has_pattern = true;
      if (is_mode_has_pattern()) {
        DT_LOG_DEBUG(get_logging_priority(), "Running HAS_PATTERN mode...");
        if (! TD.has_pattern()) check_has_pattern = false;
      }

      // Check if event has a classification
      bool check_has_classification = true;
      if (is_mode_has_classification()) {
        DT_LOG_DEBUG(get_logging_priority(), "Running HAS_CLASSIFICATION mode...");
        const datatools::properties & td_aux = TD.get_auxiliaries();
        if (! td_aux.has_key(snemo::datamodel::pid_utils::classification_label_key()))
          check_has_classification = false;
      }

      // Check if event has the correct classification label
      bool check_classification = true;
      if (is_mode_classification()) {
        DT_LOG_DEBUG(get_logging_priority(), "Running CLASSIFICATION mode...");
        const datatools::properties & td_aux = TD.get_auxiliaries();
        if (! td_aux.has_key(snemo::datamodel::pid_utils::classification_label_key())) {
          DT_LOG_DEBUG(get_logging_priority(), "The event does not have associated classification !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const std::string & a_classification = td_aux.fetch_string(snemo::datamodel::pid_utils::classification_label_key());
        DT_LOG_TRACE(get_logging_priority(), "Looking for " << _classification_label_
                     << " (current classification is '" << a_classification << "')");
        if (! std::regex_match(a_classification, std::regex(_classification_label_))) {
          check_classification = false;
        }
      }

      // Check if event has no pile ups
      bool check_no_pile_up = true;
      if (is_mode_no_pile_up()) {
        DT_LOG_DEBUG(get_logging_priority(), "Running NO_PILE_UP mode...");
        std::set<geomtools::geom_id> gids;
        const snemo::datamodel::base_topology_pattern::particle_track_dict_type a_particle_track_dict =
          TD.get_pattern_handle().get().get_particle_track_dictionary();
        for(snemo::datamodel::base_topology_pattern::particle_track_dict_type::const_iterator it = a_particle_track_dict.begin(); it != a_particle_track_dict.end(); ++it) {
          if (! (std::regex_match(it->first, std::regex("e[0-9]")) ||
                 std::regex_match(it->first, std::regex("p[0-9]"))))
            continue;

          const snemo::datamodel::particle_track & a_particle = it->second.get();
          if (! a_particle.has_associated_calorimeter_hits()) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Particle track is not associated to any calorimeter block !");
            continue;
          }

          const snemo::datamodel::calibrated_calorimeter_hit::collection_type &
            the_calorimeters = a_particle.get_associated_calorimeter_hits ();

          if (the_calorimeters.size() > 2) {
            DT_LOG_WARNING(get_logging_priority(),
                           "The particle is associated to more than 2 calorimeters !");
            continue;
          }

          for (size_t i = 0; i < the_calorimeters.size(); ++i) {
            const geomtools::geom_id & gid = the_calorimeters.at(i).get().get_geom_id();
            if (gids.find(gid) != gids.end()) {
              check_no_pile_up = false;
            }
            gids.insert(gid);
          }
        }
      }

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_pattern &&
          check_has_classification &&
          check_classification &&
          check_no_pile_up) {
        DT_LOG_DEBUG(get_logging_priority(), "Event accepted by topology cut!");
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
