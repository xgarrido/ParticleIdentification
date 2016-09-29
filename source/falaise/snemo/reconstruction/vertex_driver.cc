/// \file falaise/snemo/reconstruction/vertex_driver.cc

// Ourselves:
#include <snemo/reconstruction/vertex_driver.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
//- GSL:
#include <gsl/gsl_cdf.h>
// - Bayeux/geomtools:
#include <bayeux/geomtools/blur_spot.h>

// This project:
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/vertex_measurement.h>

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

    void vertex_driver::process(const snemo::datamodel::particle_track & pt_,
                                snemo::datamodel::vertex_measurement & vertex_)
    {
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << get_id() << "' is already initialized !");
      this->_process_algo(pt_, vertex_);
      return;
    }

    void vertex_driver::process(const snemo::datamodel::particle_track & pt1_,
                                const snemo::datamodel::particle_track & pt2_,
                                snemo::datamodel::vertex_measurement & vertex_)
    {
      DT_THROW_IF(! is_initialized(), std::logic_error, "Driver '" << get_id() << "' is already initialized !");
      this->_process_algo(pt1_, pt2_, vertex_);
      return;
    }

    void vertex_driver::_process_algo(const snemo::datamodel::particle_track & pt_,
                                      snemo::datamodel::vertex_measurement & vertex_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt_)) {
        DT_LOG_WARNING(get_logging_priority(),
                       "Vertex measurement cannot be computed if the particle is a gamma!");
        return;
      }

      geomtools::blur_spot & a_spot = vertex_.grab_vertex();

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices
        = pt_.get_vertices();

      std::string location = "";
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivtx = the_vertices.begin();
           ivtx != the_vertices.end(); ++ivtx) {
        const geomtools::blur_spot & vtx = ivtx->get();

        if(vtx.get_geom_id() == pt_.get_associated_calorimeter_hits().front().get().get_geom_id())
          continue;
        else {
          if(snemo::datamodel::particle_track::vertex_is_on_source_foil(vtx))
            location = snemo::datamodel::particle_track::vertex_on_source_foil_label();
          else if(snemo::datamodel::particle_track::vertex_is_on_wire(vtx))
            location = snemo::datamodel::particle_track::vertex_on_wire_label();
          else if(snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(vtx))
            location = snemo::datamodel::particle_track::vertex_on_main_calorimeter_label();
          else if(snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(vtx))
            location = snemo::datamodel::particle_track::vertex_on_x_calorimeter_label();
          else if(snemo::datamodel::particle_track::vertex_is_on_gamma_veto(vtx))
            location = snemo::datamodel::particle_track::vertex_on_gamma_veto_label();
          else
            DT_LOG_WARNING(get_logging_priority(),
                           "Single particle vertex location is different from the locations available !");

          a_spot.set_blur_dimension(vtx.get_blur_dimension());

          break; // Stop at the first (and supposedly only vertex different from the calorimeter hit)
        }
      }

      if (location == "") {
        DT_LOG_WARNING(get_logging_priority(), "No valid vertex was found for the particle !");
      }
      else
        a_spot.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(), location);

      vertex_.set_probability(1);
      const double epsilon = 1e-13;
      a_spot.set_x_error(epsilon);
      a_spot.set_y_error(epsilon);
      a_spot.set_z_error(epsilon);

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return;
    }

    void vertex_driver::_process_algo(const snemo::datamodel::particle_track & pt1_,
                                      const snemo::datamodel::particle_track & pt2_,
                                      snemo::datamodel::vertex_measurement & vertex_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) ||
          snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        DT_LOG_WARNING(get_logging_priority(),
                       "Vertex measurement cannot be computed if one particle is a gamma!");
        return;
      }

      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_1
        = pt1_.get_vertices();
      const snemo::datamodel::particle_track::vertex_collection_type & the_vertices_2
        = pt2_.get_vertices();
      for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
             ivtx1 = the_vertices_1.begin();
           ivtx1 != the_vertices_1.end(); ++ivtx1) {
        for (snemo::datamodel::particle_track::vertex_collection_type::const_iterator
               ivtx2 = the_vertices_2.begin();
             ivtx2 != the_vertices_2.end(); ++ivtx2) {
          const geomtools::blur_spot & vtx1 = ivtx1->get();
          const geomtools::blur_spot & vtx2 = ivtx2->get();

          auto have_same_origin = [] (const geomtools::blur_spot & vtx1_,
                                      const geomtools::blur_spot & vtx2_) -> bool
            {
              if (snemo::datamodel::particle_track::vertex_is_on_source_foil(vtx1_) &&
                  snemo::datamodel::particle_track::vertex_is_on_source_foil(vtx2_)) {
                return true;
              }
              if (snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(vtx1_) &&
                  snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(vtx2_)) {
                return true;
              }
              if (snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(vtx1_) &&
                  snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(vtx2_)) {
                return true;
              }
              if (snemo::datamodel::particle_track::vertex_is_on_gamma_veto(vtx1_) &&
                  snemo::datamodel::particle_track::vertex_is_on_gamma_veto(vtx2_)) {
                return true;
              }
              if (snemo::datamodel::particle_track::vertex_is_on_wire(vtx1_) &&
                  snemo::datamodel::particle_track::vertex_is_on_wire(vtx2_)) {
                return true;
              }
              return false;
            };

          if (! have_same_origin(vtx1, vtx2)) {
            DT_LOG_TRACE(get_logging_priority(), "Vertices do not come from the same origin !");
            continue;
          }

          _find_common_vertex(vtx1, vtx2, vertex_);
        }
      }

      DT_LOG_TRACE(get_logging_priority(), "Exiting...");
      return;
    }

    void vertex_driver::_find_common_vertex(const geomtools::blur_spot & vtx1_,
                                            const geomtools::blur_spot & vtx2_,
                                            snemo::datamodel::vertex_measurement & vertex_)

    {
      DT_THROW_IF(vtx1_.get_blur_dimension() != vtx2_.get_blur_dimension(),
                  std::logic_error, "Blur dimensions are differents !");

      auto sigma = [] (const geomtools::blur_spot & vtx_) -> double
        {
          double a_sigma = 0.0;
          if (vtx_.get_blur_dimension() >= geomtools::blur_spot::DIMENSION_ONE) {
            a_sigma += std::pow(vtx_.get_x_error(), 2);
          }
          if (vtx_.get_blur_dimension() >= geomtools::blur_spot::DIMENSION_TWO) {
            a_sigma += std::pow(vtx_.get_y_error(), 2);
          }
          if (vtx_.get_blur_dimension() >= geomtools::blur_spot::DIMENSION_THREE) {
            a_sigma += std::pow(vtx_.get_z_error(), 2);
          }
          return datatools::is_valid(a_sigma) ? a_sigma : 1.0;
        };

      const geomtools::vector_3d & pos1 = vtx1_.get_position();
      const geomtools::vector_3d & pos2 = vtx2_.get_position();
      if (! geomtools::is_valid(pos1) || ! geomtools::is_valid(pos2)) {
        DT_LOG_DEBUG(get_logging_priority(), "Vertex position is invalid !");
        return;
      }

      const double sigma1 = sigma(vtx1_);
      const double sigma2 = sigma(vtx2_);
      const geomtools::vector_3d bary = (pos1/sigma1 + pos2/sigma2)/(1/sigma1 + 1/sigma2);

      const double sigma1_x = vtx1_.get_x_error();
      const double sigma1_y = vtx1_.get_y_error();
      const double sigma1_z = vtx1_.get_z_error();
      const double sigma2_x = vtx2_.get_x_error();
      const double sigma2_y = vtx2_.get_y_error();
      const double sigma2_z = vtx2_.get_z_error();

      const double chi2_x = (std::pow(bary.x()-pos1.x(),2) + std::pow(bary.x()-pos2.x(),2))/(sigma1_x*sigma1_x + sigma2_x*sigma2_x);
      const double chi2_y = (std::pow(bary.y()-pos1.y(),2) + std::pow(bary.y()-pos2.y(),2))/(sigma1_y*sigma1_y + sigma2_y*sigma2_y);
      const double chi2_z = (std::pow(bary.z()-pos1.z(),2) + std::pow(bary.z()-pos2.z(),2))/(sigma1_z*sigma1_z + sigma2_z*sigma2_z);

      const double probability = gsl_cdf_chisq_Q(chi2_x+chi2_y+chi2_z, 1);

      if (! vertex_.has_probability() || vertex_.get_probability() < probability) {
        // Update vertex value
        vertex_.set_probability(probability);
        geomtools::blur_spot & a_spot = vertex_.grab_vertex();
        a_spot.set_blur_dimension(vtx1_.get_blur_dimension());
        a_spot.set_position(bary);
        // temporary store the vertices distance in the barycenter errors
        const double epsilon = 1e-13;
        const double dx = std::abs(pos1.x()-pos2.x());
        a_spot.set_x_error(dx < epsilon ? epsilon : dx);
        const double dy = std::abs(pos1.y()-pos2.y());
        a_spot.set_y_error(dy < epsilon ? epsilon : dy);
        const double dz = std::abs(pos1.z()-pos2.z());
        a_spot.set_z_error(dz < epsilon ? epsilon : dz);

        std::string location;
        if(snemo::datamodel::particle_track::vertex_is_on_source_foil(vtx1_))
          location = snemo::datamodel::particle_track::vertex_on_source_foil_label();
        else if (snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(vtx1_))
          location = snemo::datamodel::particle_track::vertex_on_main_calorimeter_label();
        else if (snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(vtx1_))
          location = snemo::datamodel::particle_track::vertex_on_x_calorimeter_label();
        else if (snemo::datamodel::particle_track::vertex_is_on_gamma_veto(vtx1_))
          location = snemo::datamodel::particle_track::vertex_on_gamma_veto_label();
        else if (snemo::datamodel::particle_track::vertex_is_on_wire(vtx1_))
          location = snemo::datamodel::particle_track::vertex_on_wire_label();
        else
          location = snemo::datamodel::particle_track::vertex_none_label();

        a_spot.grab_auxiliaries().update(snemo::datamodel::particle_track::vertex_type_key(),
                                       location);
      }

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
