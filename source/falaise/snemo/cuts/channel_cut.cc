// falaise/snemo/cuts/channel_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/channel_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(channel_cut, "snemo::cut::channel_cut");

    void channel_cut::_set_defaults()
    {
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      _cuts_.clear();
      return;
    }
    channel_cut::channel_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    channel_cut::~channel_cut()
    {
      if (is_initialized()) this->channel_cut::reset();
      return;
    }

    void channel_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void channel_cut::initialize(const datatools::properties & configuration_,
                                 datatools::service_manager  & /*service_manager_*/,
                                 cuts::cut_handle_dict_type  & cut_dict_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }

      DT_THROW_IF(! configuration_.has_key("cuts"), std::logic_error, "Missing 'cuts' list !");
      std::vector<std::string> cuts;
      configuration_.fetch("cuts", cuts);
      for (size_t i = 0; i < cuts.size(); i++) {
        const std::string & a_name = cuts.at(i);
        DT_THROW_IF(! configuration_.has_key(a_name + ".cut_label"), std::logic_error,
                    "Missing associated cut label to '" << a_name << "' cut!");
        const std::string a_cut_label = configuration_.fetch_string(a_name + ".cut_label");
        DT_THROW_IF(cut_dict_.find(a_cut_label) == cut_dict_.end(), std::logic_error,
                    "No cut '" << a_cut_label << "' has been registered !");
        cuts::cut_handle_type & a_cut_handle = cut_dict_[a_cut_label].grab_initialized_cut_handle();
        DT_THROW_IF(! configuration_.has_key(a_name + ".measurement_label"), std::logic_error,
                    "Missing associated measurement label to '" << a_name << "' cut!");
        const std::string a_meas_label = configuration_.fetch_string(a_name + ".measurement_label");
        _cuts_.push_back(std::make_pair(a_meas_label, a_cut_handle));
        DT_LOG_DEBUG(get_logging_priority(),
                     "Adding cut '" << a_name << " for measurement '" << a_meas_label << "'");
      }

      this->i_cut::_set_initialized(true);
      return;
    }

    int channel_cut::_accept()
    {
      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_data & TD = ER.get<snemo::datamodel::topology_data>(_TD_label_);
      if (! TD.has_pattern()) {
        DT_LOG_DEBUG(get_logging_priority(), "Missing topology pattern !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::base_topology_pattern & a_pattern = TD.get_pattern();

      // Loop over cuts
      for (cut_collection_type::iterator icut = _cuts_.begin();
           icut != _cuts_.end(); ++icut) {
        const std::string & a_meas_label = icut->first;
        if (! a_pattern.has_measurement(a_meas_label)) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing '" << a_meas_label << "' measurement !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::base_topology_measurement & a_meas = a_pattern.get_measurement(a_meas_label);
        cuts::i_cut & a_cut = icut->second.grab();
        a_cut.set_user_data(a_meas);
        const int status = a_cut.process();
        if (status == cuts::SELECTION_REJECTED) {
          DT_LOG_DEBUG(get_logging_priority(), "Event rejected by '"
                       << a_cut.get_name() << "' cut !");
          return cuts::SELECTION_REJECTED;
        } else if (status == cuts::SELECTION_INAPPLICABLE) {
          DT_LOG_DEBUG(get_logging_priority(), "Cut '" << a_cut.get_name() << "' can not be applied to '"
                       << a_meas_label << "' measurement !");
          return cuts::SELECTION_INAPPLICABLE;
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Event accepted by '" << a_cut.get_name() << "'");
        }
      }

      DT_LOG_DEBUG(get_logging_priority(), "Event accepted by channel '" << get_name() << "'cut !");
      return cuts::SELECTION_ACCEPTED;
    }

  }  // end of namespace cut

}  // end of namespace snemo

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::cut::channel_cut, ocd_)
{
  ocd_.set_class_name("snemo::cut::channel_cut");
  ocd_.set_class_description("Cut used to select physics channel");
  ocd_.set_class_library("falaise");
  // ocd_.set_class_documentation("");

  cuts::i_cut::common_ocd(ocd_);

  {
    // Description of the 'cuts' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("cuts")
      .set_terse_description("The list of name of the cuts to be combined")
      .set_traits(datatools::TYPE_STRING,
                  datatools::configuration_property_description::ARRAY)
      .set_mandatory(true)
      .set_long_description("The list of all cuts' to be combined with a logical AND.")
      .add_example("Combine 2 cuts: ::                               \n"
                   "                                                 \n"
                   "    cuts : string[2] = \"int_prob\" \"ext_prob\" \n"
                   "                                                 \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("${cuts}.cut_label")
      .set_terse_description("The cut label name to be associated to ${cuts}")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(true)
      .add_example("Set the cut labels::                                              \n"
                   "                                                                  \n"
                   "  cuts : string[2] = \"int_prob\" \"ext_prob\"                    \n"
                   "  int_prob.cut_label : string = \"good_internal_probability_cut\" \n"
                   "  ext_prob.cut_label : string = \"bad_external_probability_cut\"  \n"
                   "                                                                  \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("${cuts}.measurement_label")
      .set_terse_description("The measurement label name to be associated to ${cuts}")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(true)
      .add_example("Set the measurement labels::                          \n"
                   "                                                      \n"
                   "  cuts : string[2] = \"int_prob\" \"ext_prob\"        \n"
                   "  int_prob.measurement_label : string = \"tof_e1_e2\" \n"
                   "  ext_prob.measurement_label : string = \"tof_e1_e2\" \n"
                   "                                                      \n"
                   )
      ;
  }

  // Additional configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the                               \n"
                               "``datatools::properties`` ASCII format::                                  \n"
                               "                                                                          \n"
                               "  cuts : string[2] = \"int_prob\" \"ext_prob\"                            \n"
                               "  int_prob.cut_label         : string = \"good_internal_probability_cut\" \n"
                               "  int_prob.measurement_label : string = \"tof_e1_e2\"                     \n"
                               "  ext_prob.cut_label         : string = \"bad_external_probability_cut\"  \n"
                               "  ext_prob.measurement_label : string = \"tof_e1_e2\"                     \n"
                               "                                                                          \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation

// Registration macro for class 'snemo::cut::channel_cut' :
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::cut::channel_cut, "snemo::cut::channel_cut")

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
