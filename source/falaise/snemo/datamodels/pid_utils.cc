// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/pid_utils.cc

// Ourselves:
#include <falaise/snemo/datamodels/pid_utils.h>

namespace snemo {

  namespace datamodel {

    const std::string & pid_utils::pid_prefix_key()
    {
      static const std::string s("pid_utils");
      return s;
    }

    const std::string & pid_utils::pid_label_key()
    {
      static const std::string s(snemo::datamodel::pid_utils::pid_prefix_key() +
                                 ".label");
      return s;
    }

    const std::string & pid_utils::electron_label()
    {
      static const std::string s("electron");
      return s;
    }

    const std::string & pid_utils::positron_label()
    {
      static const std::string s("positron");
      return s;
    }

    const std::string & pid_utils::gamma_label()
    {
      static const std::string s("gamma");
      return s;
    }

    const std::string & pid_utils::alpha_label()
    {
      static const std::string s("alpha");
      return s;
    }

    const std::string & pid_utils::undefined_label()
    {
      static const std::string s("undefined");
      return s;
    }

    bool pid_utils::particle_is(const particle_track & pt_, const std::string & label_)
    {
      const datatools::properties & aux = pt_.get_auxiliaries();
      if (! aux.has_key(pid_label_key())) {
        DT_LOG_WARNING(datatools::logger::PRIO_ALWAYS,
                       "Missing '" << pid_label_key() << "' property !");
        return false;
      }
      return aux.fetch_string(pid_label_key()) == label_;
    }

    bool pid_utils::particle_is_electron(const particle_track & pt_)
    {
      return particle_is(pt_, electron_label());
    }

    bool pid_utils::particle_is_positron(const particle_track & pt_)
    {
      return particle_is(pt_, positron_label());
    }

    bool pid_utils::particle_is_alpha(const particle_track & pt_)
    {
      return particle_is(pt_, alpha_label());
    }

    bool pid_utils::particle_is_gamma(const particle_track & pt_)
    {
      return particle_is(pt_, gamma_label());
    }

    size_t pid_utils::fetch_particles(const snemo::datamodel::particle_track_data & ptd_,
                                      snemo::datamodel::particle_track_data::particle_collection_type & particles_,
                                      const std::string & label_,
                                      const bool clear_)
    {
      if (clear_) particles_.clear();
      size_t ipart = 0;

      if (! ptd_.has_particles()) return ipart;
      const snemo::datamodel::particle_track_data::particle_collection_type & the_particles
        = ptd_.get_particles();

      for (snemo::datamodel::particle_track_data::particle_collection_type::const_iterator
             i = the_particles.begin(); i != the_particles.end(); ++i) {
        const snemo::datamodel::particle_track::handle_type & a_particle = *i;
        if (pid_utils::particle_is(a_particle.get(), label_)) {
          particles_.push_back(a_particle);
          ipart++;
        }
      }
      return ipart;
    }

  } // end of namespace datamodel

} // end of namespace snemo
