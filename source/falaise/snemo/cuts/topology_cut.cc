// falaise/snemo/cuts/topology_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/topology_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(topology_cut, "snemo::cut::topology_cut");

    void topology_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _PTD_label_ = snemo::datamodel::data_info::default_particle_track_data_label();
      return;
    }

    uint32_t topology_cut::get_mode() const
    {
      return _mode_;
    }

    topology_cut::topology_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    topology_cut::~topology_cut()
    {
      if (is_initialized()) this->topology_cut::reset();
      return;
    }

    void topology_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void topology_cut::initialize(const datatools::properties & configuration_,
                                        datatools::service_manager  & /* service_manager_ */,
                                        cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("PTD_label")) {
        _PTD_label_ = configuration_.fetch_string("PTD_label");
      }

      if (_mode_ == MODE_UNDEFINED) {
      }

      this->i_cut::_set_initialized(true);
      return;
    }


    int topology_cut::_accept()
    {
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_PTD_label_)) {
        DT_LOG_DEBUG(get_logging_priority(), "Event record has no '" << _PTD_label_ << "' bank !");
        return cut_returned;
      }
      cut_returned = cuts::SELECTION_REJECTED;
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
