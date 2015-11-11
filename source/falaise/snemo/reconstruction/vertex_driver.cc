/// \file falaise/snemo/reconstruction/vertex_driver.cc

// Ourselves:
#include <snemo/reconstruction/vertex_driver.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
//- GSL:
#include <gsl/gsl_cdf.h>

// This project:
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track.h>

namespace snemo {

  namespace reconstruction {

    const std::string & vertex_driver::get_id()
    {
      static const std::string _id("VD");
      return _id;
    }

    bool vertex_driver::is_initialized() const
    {
      return _initialized_;
    }

    void vertex_driver::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    void vertex_driver::set_logging_priority(const datatools::logger::priority priority_)
    {
      _logging_priority_ = priority_;
      return;
    }

    datatools::logger::priority vertex_driver::get_logging_priority() const
    {
      return _logging_priority_;
    }

    // Constructor
    vertex_driver::vertex_driver()
    {
      _set_defaults();
      return;
    }

    // Destructor
    vertex_driver::~vertex_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void vertex_driver::_set_defaults()
    {

      _initialized_ = false;
      _logging_priority_ = datatools::logger::PRIO_WARNING;
      return;
    }

    // Initialization :
    void vertex_driver::initialize(const datatools::properties  & setup_)
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

    void vertex_driver::reset()
    {
      _set_defaults();
      _set_initialized(false);
      return;
    }

    void vertex_driver::process(const snemo::datamodel::particle_track & pt1_,
                                        const snemo::datamodel::particle_track & pt2_,
                                        geomtools::blur_spot & barycenter_)
    {
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << get_id() << "' is already initialized !");
      this->_process_algo(pt1_, pt2_, barycenter_);
      return;
    }

    void vertex_driver::_process_algo(const snemo::datamodel::particle_track & pt1_,
                                              const snemo::datamodel::particle_track & pt2_,
                                              geomtools::blur_spot & barycenter_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // Invalidate results
      // set to +infinity later...
      // probability_.invalidate();

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ||
          snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        DT_LOG_WARNING(get_logging_priority(),
                       "Delta vertices cannot be computed if one particle is a gamma!");
        return;
      }

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_1
        = pt1_.get_vertices();
      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_2
        = pt2_.get_vertices();
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivertex_1 = the_vertices_1.begin();
           ivertex_1 != the_vertices_1.end(); ++ivertex_1) {

        const geomtools::blur_spot & a_vertex_1 = ivertex_1->get();

        for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
               ivertex_2 = the_vertices_2.begin();
             ivertex_2 != the_vertices_2.end(); ++ivertex_2) {

          const geomtools::blur_spot & a_vertex_2 = ivertex_2->get();

          if (!((snemo::datamodel::particle_track::vertex_is_on_source_foil(a_vertex_1) &&
                 snemo::datamodel::particle_track::vertex_is_on_source_foil(a_vertex_2)) ||
                (snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(a_vertex_1) &&
                 snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(a_vertex_2)) ||
                (snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(a_vertex_1) &&
                 snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(a_vertex_2)) ||
                (snemo::datamodel::particle_track::vertex_is_on_gamma_veto(a_vertex_1) &&
                 snemo::datamodel::particle_track::vertex_is_on_gamma_veto(a_vertex_2)))
              )
            continue;

          _find_vertex(a_vertex_1, a_vertex_2, barycenter_);
        }
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return;
    }

    void vertex_driver::_find_vertex(const geomtools::blur_spot & vertex_1_,
                                     const geomtools::blur_spot & vertex_2_,
                                     geomtools::blur_spot & barycenter_)

    {
      const double sigma_x_1 = vertex_1_.get_x_error();
      const double sigma_y_1 = vertex_1_.get_y_error();
      const double sigma_z_1 = vertex_1_.get_z_error();
      const double sigma_1 = sigma_x_1*sigma_x_1+ sigma_y_1*sigma_y_1+sigma_z_1*sigma_z_1;

      const double sigma_x_2 = vertex_2_.get_x_error();
      const double sigma_y_2 = vertex_2_.get_y_error();
      const double sigma_z_2 = vertex_2_.get_z_error();
      const double sigma_2 = sigma_x_2*sigma_x_2+ sigma_y_2*sigma_y_2+sigma_z_2*sigma_z_2;

      geomtools::vector_3d v_barycenter = (vertex_1_.get_position()/(sigma_1) +
                                           vertex_2_.get_position()/(sigma_2))/(1/sigma_1+1/sigma_2);

      barycenter_.set_blur_dimension(geomtools::blur_spot::DIMENSION_THREE);
      barycenter_.set_position(v_barycenter);
      //no errors for now

      const double chi_2 = ((v_barycenter-vertex_1_.get_position())*(v_barycenter-vertex_1_.get_position()) +
                            (v_barycenter-vertex_2_.get_position())*(v_barycenter-vertex_2_.get_position()))/(sigma_1 + sigma_2);
      const double probability = gsl_cdf_chisq_Q(chi_2, 1);

      barycenter_.grab_auxiliaries().store("Probability",probability);
      return ;
    }

    // static
    void vertex_driver::init_ocd(datatools::object_configuration_description & ocd_)
    {
      // Prefix "VD" stands for "Vertex Driver" :
      datatools::logger::declare_ocd_logging_configuration(ocd_, "fatal", "VD.");
    }

  } // end of namespace reconstruction

} // end of namespace snemo

  /* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::vertex_driver, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::vertex_driver");
  ocd_.set_class_description("A driver class for the Delta Vertices algorithm");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("The driver determines the spatial difference between vertices");
  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::vertex_driver,
                               "snemo::reconstruction::vertex_driver")
