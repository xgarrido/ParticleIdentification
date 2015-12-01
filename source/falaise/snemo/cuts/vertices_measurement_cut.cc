// falaise/snemo/cuts/vertices_measurement_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/vertices_measurement_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>
#include <datatools/clhep_units.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/vertex_measurement.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(vertices_measurement_cut, "snemo::cut::vertices_measurement_cut");

    void vertices_measurement_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      datatools::invalidate(_vertices_prob_range_min_);
      datatools::invalidate(_vertices_prob_range_max_);
      datatools::invalidate(_vertices_dist_x_range_min_);
      datatools::invalidate(_vertices_dist_x_range_max_);
      datatools::invalidate(_vertices_dist_y_range_min_);
      datatools::invalidate(_vertices_dist_y_range_max_);
      datatools::invalidate(_vertices_dist_z_range_min_);
      datatools::invalidate(_vertices_dist_z_range_max_);
      return;
    }

    uint32_t vertices_measurement_cut::get_mode() const
    {
      return _mode_;
    }

    bool vertices_measurement_cut::is_mode_has_vertices_probability() const
    {
      return _mode_ & MODE_HAS_VERTICES_PROBABILITY;
    }

    bool vertices_measurement_cut::is_mode_range_vertices_probability() const
    {
      return _mode_ & MODE_RANGE_VERTICES_PROBABILITY;
    }

    bool vertices_measurement_cut::is_mode_has_vertices_distance() const
    {
      return _mode_ & MODE_HAS_VERTICES_DISTANCE;
    }

    bool vertices_measurement_cut::is_mode_range_vertices_distance_x() const
    {
      return _mode_ & MODE_RANGE_VERTICES_DISTANCE_X;
    }

    bool vertices_measurement_cut::is_mode_range_vertices_distance_y() const
    {
      return _mode_ & MODE_RANGE_VERTICES_DISTANCE_Y;
    }

    bool vertices_measurement_cut::is_mode_range_vertices_distance_z() const
    {
      return _mode_ & MODE_RANGE_VERTICES_DISTANCE_Z;
    }

    vertices_measurement_cut::vertices_measurement_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      this->register_supported_user_data_type<snemo::datamodel::base_topology_measurement>();
      this->register_supported_user_data_type<snemo::datamodel::vertex_measurement>();
      return;
    }

    vertices_measurement_cut::~vertices_measurement_cut()
    {
      if (is_initialized()) this->vertices_measurement_cut::reset();
      return;
    }

    void vertices_measurement_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void vertices_measurement_cut::initialize(const datatools::properties & configuration_,
                                              datatools::service_manager  & /* service_manager_ */,
                                              cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (_mode_ == MODE_UNDEFINED) {
        if (configuration_.has_flag("mode.has_vertices_probability")) {
          _mode_ |= MODE_HAS_VERTICES_PROBABILITY;
        }
        if (configuration_.has_flag("mode.range_vertices_probability")) {
          _mode_ |= MODE_RANGE_VERTICES_PROBABILITY;
        }
        if (configuration_.has_flag("mode.has_vertices_distance")) {
          _mode_ |= MODE_HAS_VERTICES_DISTANCE;
        }
        if (configuration_.has_flag("mode.range_vertices_distance_x")) {
          _mode_ |= MODE_RANGE_VERTICES_DISTANCE_X;
        }
        if (configuration_.has_flag("mode.range_vertices_distance_y")) {
          _mode_ |= MODE_RANGE_VERTICES_DISTANCE_Y;
        }
        if (configuration_.has_flag("mode.range_vertices_distance_z")) {
          _mode_ |= MODE_RANGE_VERTICES_DISTANCE_Z;
        }
        DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                    "Missing at least a 'mode.XXX' property !");

        // mode HAS_VERTICES_PROBABILITY:
        if (is_mode_has_vertices_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using HAS_VERTICES_PROBABILITY mode...");
        } // end if is_mode_has_vertices_probability

        // mode PARTICLE_RANGE_VERTICES_PROBABILITY:
        if (is_mode_range_vertices_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_VERTICES_PROBABILITY mode...");

          size_t count = 0;
          if (configuration_.has_key("range_vertices_probability.min")) {
            double pmin = configuration_.fetch_real("range_vertices_probability.min");
            if (! configuration_.has_explicit_unit("range_vertices_probability.min")) {
              pmin *= CLHEP::perCent;
            }
            DT_THROW_IF(pmin < 0.0*CLHEP::perCent || pmin > 100.0*CLHEP::perCent,
                        std::range_error,
                        "Invalid minimal vertices probability (" << pmin << ") !");
            _vertices_prob_range_min_ = pmin;
            count++;
          }
          if (configuration_.has_key("range_vertices_probability.max")) {
            double pmax = configuration_.fetch_real("range_vertices_probability.max");
            if (! configuration_.has_explicit_unit("range_vertices_probability.max")) {
              pmax *= CLHEP::perCent;
            }
            DT_THROW_IF(pmax < 0.0*CLHEP::perCent || pmax > 100.0*CLHEP::perCent,
                        std::range_error,
                        "Invalid maximal vertices probability (" << pmax << ") !");
            _vertices_prob_range_max_ = pmax;
            count++;
          }
          DT_THROW_IF(count == 0, std::logic_error,
                      "Missing 'range_vertices_probability.min' or 'range_vertices_probability.max' property !");
          if (count == 2 && _vertices_prob_range_min_ >= 0 && _vertices_prob_range_max_ >= 0) {
            DT_THROW_IF(_vertices_prob_range_min_ > _vertices_prob_range_max_, std::logic_error,
                        "Invalid 'range_vertices_probability.min' > 'range_vertices_probability.max' values !");
          }
        } // end if is_mode_range_vertices_probability

        // mode HAS_VERTICES_PROBABILITY:
        if (is_mode_has_vertices_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using HAS_VERTICES_PROBABILITY mode...");
        } // end if is_mode_has_vertices_probability

        // mode PARTICLE_RANGE_VERTICES_DISTANCE_X:
        if (is_mode_range_vertices_distance_x()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_VERTICES_DISTANCE_X mode...");

          size_t count = 0;
          if (configuration_.has_key("range_vertices_distance_x.min")) {
            double vtx_dist_x_min = configuration_.fetch_real("range_vertices_distance_x.min");
            if (! configuration_.has_explicit_unit("range_vertices_distance_x.min")) {
              vtx_dist_x_min *= CLHEP::mm;
            }
            DT_THROW_IF(vtx_dist_x_min < 0.0*CLHEP::mm,
                        std::range_error,
                        "Invalid minimal vertices distance in X (" << vtx_dist_x_min << "mm) !");
            _vertices_dist_x_range_min_ = vtx_dist_x_min;
            count++;
          }
          if (configuration_.has_key("range_vertices_distance_x.max")) {
            double vtx_dist_x_max = configuration_.fetch_real("range_vertices_distance_x.max");
            if (! configuration_.has_explicit_unit("range_vertices_distance_x.max")) {
              vtx_dist_x_max *= CLHEP::mm;
            }
            DT_THROW_IF(vtx_dist_x_max < 0.0*CLHEP::mm,
                        std::range_error,
                        "Invalid maximal vertices distance in X (" << vtx_dist_x_max << ") !");
            _vertices_dist_x_range_max_ = vtx_dist_x_max;
            count++;
          }
          DT_THROW_IF(count == 0, std::logic_error,
                      "Missing 'range_vertices_distance_x.min' or 'range_vertices_distance_x.max' property !");
          if (count == 2 && _vertices_dist_x_range_min_ >= 0 && _vertices_dist_x_range_max_ >= 0) {
            DT_THROW_IF(_vertices_dist_x_range_min_ > _vertices_dist_x_range_max_, std::logic_error,
                        "Invalid 'range_vertices_distance_x.min' > 'range_vertices_distance_x.max' values !");
          }
        } // end if is_mode_range_vertices_distance_x

        // mode PARTICLE_RANGE_VERTICES_DISTANCE_Y:
        if (is_mode_range_vertices_distance_y()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_VERTICES_DISTANCE_Y mode...");

          size_t count = 0;
          if (configuration_.has_key("range_vertices_distance_y.min")) {
            double vtx_dist_y_min = configuration_.fetch_real("range_vertices_distance_y.min");
            if (! configuration_.has_explicit_unit("range_vertices_distance_y.min")) {
              vtx_dist_y_min *= CLHEP::mm;
            }
            DT_THROW_IF(vtx_dist_y_min < 0.0*CLHEP::mm,
                        std::range_error,
                        "Invalid minimal vertices distance in Y (" << vtx_dist_y_min << ") !");
            _vertices_dist_y_range_min_ = vtx_dist_y_min;
            count++;
          }
          if (configuration_.has_key("range_vertices_distance_y.max")) {
            double vtx_dist_y_max = configuration_.fetch_real("range_vertices_distance_y.max");
            if (! configuration_.has_explicit_unit("range_vertices_distance_y.max")) {
              vtx_dist_y_max *= CLHEP::mm;
            }
            DT_THROW_IF(vtx_dist_y_max < 0.0*CLHEP::mm,
                        std::range_error,
                        "Invalid maximal vertices distance in Y (" << vtx_dist_y_max << ") !");
            _vertices_dist_y_range_max_ = vtx_dist_y_max;
            count++;
          }
          DT_THROW_IF(count == 0, std::logic_error,
                      "Missing 'range_vertices_distance_y.min' or 'range_vertices_distance_y.max' property !");
          if (count == 2 && _vertices_dist_y_range_min_ >= 0 && _vertices_dist_y_range_max_ >= 0) {
            DT_THROW_IF(_vertices_dist_y_range_min_ > _vertices_dist_y_range_max_, std::logic_error,
                        "Invalid 'range_vertices_distance_y.min' > 'range_vertices_distance_y.max' values !");
          }
        } // end if is_mode_range_vertices_distance_y

        // mode PARTICLE_RANGE_VERTICES_DISTANCE_Z:
        if (is_mode_range_vertices_distance_z()) {
          DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_VERTICES_DISTANCE_Z mode...");

          size_t count = 0;
          if (configuration_.has_key("range_vertices_distance_z.min")) {
            double vtx_dist_z_min = configuration_.fetch_real("range_vertices_distance_z.min");
            if (! configuration_.has_explicit_unit("range_vertices_distance_z.min")) {
              vtx_dist_z_min *= CLHEP::mm;
            }
            DT_THROW_IF(vtx_dist_z_min < 0.0*CLHEP::mm,
                        std::range_error,
                        "Invalid minimal vertices distance in Z (" << vtx_dist_z_min << ") !");
            _vertices_dist_z_range_min_ = vtx_dist_z_min;
            count++;
          }
          if (configuration_.has_key("range_vertices_distance_z.max")) {
            double vtx_dist_z_max = configuration_.fetch_real("range_vertices_distance_z.max");
            if (! configuration_.has_explicit_unit("range_vertices_distance_z.max")) {
              vtx_dist_z_max *= CLHEP::mm;
            }
            DT_THROW_IF(vtx_dist_z_max < 0.0*CLHEP::mm,
                        std::range_error,
                        "Invalid maximal vertices distance in Z (" << vtx_dist_z_max << ") !");
            _vertices_dist_z_range_max_ = vtx_dist_z_max;
            count++;
          }
          DT_THROW_IF(count == 0, std::logic_error,
                      "Missing 'range_vertices_distance_z.min' or 'range_vertices_distance_z.max' property !");
          if (count == 2 && _vertices_dist_z_range_min_ >= 0 && _vertices_dist_z_range_max_ >= 0) {
            DT_THROW_IF(_vertices_dist_z_range_min_ > _vertices_dist_z_range_max_, std::logic_error,
                        "Invalid 'range_vertices_distance_z.min' > 'range_vertices_distance_z.max' values !");
          }
        } // end if is_mode_range_vertices_distance_z

      }

      this->i_cut::_set_initialized(true);
      return;
    }

    int vertices_measurement_cut::_accept()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get vertices measurement
      const snemo::datamodel::vertex_measurement * ptr_meas = 0;
      if (is_user_data<snemo::datamodel::vertex_measurement>()) {
        ptr_meas = &(get_user_data<snemo::datamodel::vertex_measurement>());
      } else if (is_user_data<snemo::datamodel::base_topology_measurement>()) {
        const snemo::datamodel::base_topology_measurement & btm
          = get_user_data<snemo::datamodel::base_topology_measurement>();
        ptr_meas = dynamic_cast<const snemo::datamodel::vertex_measurement *>(&btm);
      } else {
        DT_THROW_IF(true, std::logic_error, "Invalid data type !");
      }
      const snemo::datamodel::vertex_measurement & a_vertices_meas = *ptr_meas;

      // Check if measurement has vertices probability
      bool check_has_vertices_probability = true;
      if (is_mode_has_vertices_probability()) {
        if (! a_vertices_meas.has_probability()) {
          check_has_vertices_probability = false;
        }
      }

      // Check if measurement has correct vertices probability
      bool check_range_vertices_probability = true;
      if (is_mode_range_vertices_probability()) {
        if (! a_vertices_meas.has_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing vertices probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double & proba = a_vertices_meas.get_probability();
        if (datatools::is_valid(_vertices_prob_range_min_)) {
          if (proba < _vertices_prob_range_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices probability (" << proba/CLHEP::perCent << "%) lower than "
                         << _vertices_prob_range_min_/CLHEP::perCent << "%");
            check_range_vertices_probability = false;
          }
        }

        if (datatools::is_valid(_vertices_prob_range_max_)) {
          if (proba > _vertices_prob_range_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices probability (" << proba/CLHEP::perCent << "%) greater than "
                         << _vertices_prob_range_max_/CLHEP::perCent << "%");
            check_range_vertices_probability = false;
          }
        }
      } // end of is_mode_range_vertices_probability

      // Check if measurement has vertices distance
      bool check_has_vertices_distance = true;
      if (is_mode_has_vertices_distance()) {
        if (! a_vertices_meas.has_vertices_distance()) {
          check_has_vertices_distance = false;
        }
      }

      // Check if measurement has correct vertices distance in X
      bool check_range_vertices_distance_x = true;
      if (is_mode_range_vertices_distance_x()) {
        if (! a_vertices_meas.has_vertices_distance()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing vertices distance !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double & vtx_dist_x = a_vertices_meas.get_vertices_distance_x();
        if (datatools::is_valid(_vertices_dist_x_range_min_)) {
          if (vtx_dist_x < _vertices_dist_x_range_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices distance in X (" << vtx_dist_x/CLHEP::mm << "mm) lower than "
                         << _vertices_dist_x_range_min_/CLHEP::mm << "mm");
            check_range_vertices_distance_x = false;
          }
        }

        if (datatools::is_valid(_vertices_dist_x_range_max_)) {
          if (vtx_dist_x > _vertices_dist_x_range_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices distance in X (" << vtx_dist_x/CLHEP::mm << "mm) greater than "
                         << _vertices_dist_x_range_max_/CLHEP::mm << "mm");
            check_range_vertices_distance_x = false;
          }
        }
      } // end of is_mode_range_vertices_distance_x

      // Check if measurement has correct vertices distance in Y
      bool check_range_vertices_distance_y = true;
      if (is_mode_range_vertices_distance_y()) {
        if (! a_vertices_meas.has_vertices_distance()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing vertices distance !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double & vtx_dist_y = a_vertices_meas.get_vertices_distance_y();

        if (datatools::is_valid(_vertices_dist_y_range_min_)) {
          if (vtx_dist_y < _vertices_dist_y_range_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices distance in Y (" << vtx_dist_y/CLHEP::mm << "mm) lower than "
                         << _vertices_dist_y_range_min_/CLHEP::mm << "mm");
            check_range_vertices_distance_y = false;
          }
        }

        if (datatools::is_valid(_vertices_dist_y_range_max_)) {
          if (vtx_dist_y > _vertices_dist_y_range_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices distance in Y (" << vtx_dist_y/CLHEP::mm << "mm) greater than "
                         << _vertices_dist_y_range_max_/CLHEP::mm << "mm");
            check_range_vertices_distance_y = false;
          }
        }
      } // end of is_mode_range_vertices_distance_y

      // Check if measurement has correct vertices distance in Z
      bool check_range_vertices_distance_z = true;
      if (is_mode_range_vertices_distance_z()) {
        if (! a_vertices_meas.has_vertices_distance()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing vertices distance !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double & vtx_dist_z = a_vertices_meas.get_vertices_distance_z();
        if (datatools::is_valid(_vertices_dist_z_range_min_)) {
          if (vtx_dist_z < _vertices_dist_z_range_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices distance in Z (" << vtx_dist_z/CLHEP::mm << "mm) lower than "
                         << _vertices_dist_z_range_min_/CLHEP::mm << "mm");
            check_range_vertices_distance_z = false;
          }
        }

        if (datatools::is_valid(_vertices_dist_z_range_max_)) {
          if (vtx_dist_z > _vertices_dist_z_range_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Vertices distance in Z (" << vtx_dist_z/CLHEP::mm << "mm) greater than "
                         << _vertices_dist_z_range_max_/CLHEP::mm << "mm");
            check_range_vertices_distance_z = false;
          }
        }
      } // end of is_mode_range_vertices_distance_y

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_vertices_probability   &&
          check_range_vertices_probability &&
          check_has_vertices_distance      &&
          check_range_vertices_distance_x  &&
          check_range_vertices_distance_y  &&
          check_range_vertices_distance_z
          ) {
        DT_LOG_DEBUG(get_logging_priority(), "Event accepted by VERTICES measurement cut!");
        cut_returned = cuts::SELECTION_ACCEPTED;
      }
      return cut_returned;
    }

  }  // end of namespace cut

}  // end of namespace snemo

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::cut::vertices_measurement_cut, ocd_)
{
  ocd_.set_class_name("snemo::cut::vertices_measurement_cut");
  ocd_.set_class_description("Cut based on criteria applied to a common vertices measurement");
  ocd_.set_class_library("falaise");
  // ocd_.set_class_documentation("");

  cuts::i_cut::common_ocd(ocd_);

  {
    // Description of the 'mode.has_vertices_probability' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.has_vertices_probability")
      .set_terse_description("Mode requiring vertices probability availability")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the requested vertices probability mode:: \n"
                   "                                                   \n"
                   "  mode.has_vertices_probability : boolean = true   \n"
                   "                                                   \n"
                   )
      ;
  }

  {
    // Description of the 'mode.range_vertices_probability' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.range_vertices_probability")
      .set_terse_description("Mode with a special requested range of vertices probability value")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the mode::                                \n"
                   "                                                   \n"
                   "  mode.range_vertices_probability : boolean = true \n"
                   "                                                   \n"
                   )
      ;
  }

  {
    // Description of the 'range_vertices_probability.min' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_vertices_probability.min")
      .set_terse_description("Minimal value of the requested range vertices probability")
      .set_triggered_by_flag("mode.range_vertices_probability")
      .set_traits(datatools::TYPE_REAL)
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("fraction")
      .set_unit_symbol("%")
      .add_example("Set a specific minimal value of the vertices probability:: \n"
                   "                                                           \n"
                   "  range_vertices_probability.min : real as fraction = 4 %  \n"
                   "                                                           \n"
                   )
      ;
  }

  {
    // Description of the 'range_vertices_probability.max' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_vertices_probability.max")
      .set_terse_description("Maximal value of the requested ranged vertices probability")
      .set_triggered_by_flag("mode.range_vertices_probability")
      .set_traits(datatools::TYPE_REAL)
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("fraction")
      .set_unit_symbol("%")
      .add_example("Set a specific maximal value of the vertices probability:: \n"
                   "                                                           \n"
                   "  range_vertices_probability.max : real as fraction = 4 %  \n"
                   "                                                           \n"
                   )
      ;
  }

  // Additional configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the                  \n"
                               "``datatools::properties`` ASCII format::                     \n"
                               "                                                             \n"
                               "   mode.has_internal_probability : boolean = true            \n"
                               "   mode.range_internal_probability : boolean = true          \n"
                               "   range_internal_probability.mode : string = \"all\"        \n"
                               "   range_internal_probability.min : real as fraction = 4 %   \n"
                               "   range_internal_probability.max : real as fraction = 100 % \n"
                               "                                                             \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation

// Registration macro for class 'snemo::cut::simulated_data_cut' :
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::cut::vertices_measurement_cut, "snemo::cut::vertices_measurement_cut")

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
