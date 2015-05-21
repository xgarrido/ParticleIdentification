/// \file falaise/snemo/reconstruction/delta_vertices_driver.cc

// Ourselves:
#include <snemo/reconstruction/delta_vertices_driver.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// This project:
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track.h>

namespace snemo {

  namespace reconstruction {

    const std::string & delta_vertices_driver::get_id()
    {
      static const std::string _id("DVD");
      return _id;
    }

    bool delta_vertices_driver::is_initialized() const
    {
      return _initialized_;
    }

    void delta_vertices_driver::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    void delta_vertices_driver::set_logging_priority(const datatools::logger::priority priority_)
    {
      _logging_priority_ = priority_;
      return;
    }

    datatools::logger::priority delta_vertices_driver::get_logging_priority() const
    {
      return _logging_priority_;
    }

    // Constructor
    delta_vertices_driver::delta_vertices_driver()
    {
      _set_defaults();
      return;
    }

    // Destructor
    delta_vertices_driver::~delta_vertices_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void delta_vertices_driver::_set_defaults()
    {

      _initialized_ = false;
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      return;
    }

    // Initialization :
    void delta_vertices_driver::initialize(const datatools::properties  & setup_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Driver '" << get_id() << "' is already initialized !");

      // Logging priority
      datatools::logger::priority lp = datatools::logger::extract_logging_configuration(setup_);
      DT_THROW_IF(lp == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority level !");
      set_logging_priority(lp);

      _set_initialized(true);
      return;
    }

    void delta_vertices_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

    void delta_vertices_driver::process(const snemo::datamodel::particle_track & pt1_,
                                        const snemo::datamodel::particle_track & pt2_,
                                        double & delta_vertices_y, double & delta_vertices_z)
    {
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << get_id() << "' is already initialized !");
      this->_process_algo(pt1_, pt2_, delta_vertices_y, delta_vertices_z);
      return;
    }

    void delta_vertices_driver::_process_algo(const snemo::datamodel::particle_track & pt1_,
                                              const snemo::datamodel::particle_track & pt2_,
                                              double & delta_vertices_y_,
                                              double & delta_vertices_z_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      // Invalidate results
      datatools::invalidate(delta_vertices_y_);
      datatools::invalidate(delta_vertices_z_);

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) &&
          snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        DT_LOG_WARNING(get_logging_priority(),
                       "Delta vertices cannot be computed if one particle is a gamma!");
        return;
      }

      geomtools::vector_3d v1;
      geomtools::invalidate(v1);
      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_1
        = pt1_.get_vertices();
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivertex = the_vertices_1.begin();
           ivertex != the_vertices_1.end(); ++ivertex) {
        const geomtools::blur_spot & a_vertex = ivertex->get();
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(a_vertex)) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Vertex " << a_vertex.get_position() << " is not on the source foil !");
          continue;
        }
        v1 = a_vertex.get_position();
        break;
      }

      geomtools::vector_3d v2;
      geomtools::invalidate(v2);
      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_2
        = pt2_.get_vertices();
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivertex = the_vertices_2.begin();
           ivertex != the_vertices_2.end(); ++ivertex) {
        const geomtools::blur_spot & a_vertex = ivertex->get();
        if (! snemo::datamodel::particle_track::vertex_is_on_source_foil(a_vertex)) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Vertex " << a_vertex.get_position() << " is not on the source foil !");
          continue;
        }
        v2 = a_vertex.get_position();
        break;
      }

      if (geomtools::is_valid(v1) && geomtools::is_valid(v2)) {
        delta_vertices_y_ = v1.y() - v2.y();
        delta_vertices_z_ = v1.z() - v2.z();
      }

      DT_LOG_DEBUG(get_logging_priority(), "Delta vertex y = " << delta_vertices_y_/CLHEP::mm << " mm");
      DT_LOG_DEBUG(get_logging_priority(), "Delta vertex z = " << delta_vertices_z_/CLHEP::mm << " mm");

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return;
    }

    // static
    void delta_vertices_driver::init_ocd(datatools::object_configuration_description & ocd_)
    {
      // Prefix "DVD" stands for "Delta Vertices Driver" :
      datatools::logger::declare_ocd_logging_configuration(ocd_, "fatal", "DVD.");
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::delta_vertices_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::delta_vertices_driver");
  ocd_.set_class_description("A driver class for the Delta Vertices algorithm");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("The driver determines the spatial difference between vertices");
  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::delta_vertices_driver,
                               "snemo::reconstruction::delta_vertices_driver")
