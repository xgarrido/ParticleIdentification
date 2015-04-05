// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/pid_utils.h
/* Author(s) :    Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-03-05
 * Last modified: 2015-03-05
 */

#ifndef FALAISE_SNEMO_DATAMODEL_PID_UTILS_H
#define FALAISE_SNEMO_DATAMODEL_PID_UTILS_H 1

// Standard library:
#include <string>

namespace snemo {

  namespace datamodel {

    class particle_track;

    struct pid_utils {

      /// The default prefix value for all pid_utils property keys
      static const std::string & pid_prefix_key();

      /// The name of a string property representing the Particle Identification label
      static const std::string & pid_label_key();

      /// The label of electron particle
      static const std::string & electron_label();

      /// The label of positron particle
      static const std::string & positron_label();

      /// The label of gamma particle
      static const std::string & gamma_label();

      /// The label of alpha particle
      static const std::string & alpha_label();

      /// The label of undefined particle
      static const std::string & undefined_label();

      /// Check a particle pid label
      static bool particle_is(const snemo::datamodel::particle_track &, const std::string &);

      /// Check a particle is electron
      static bool particle_is_electron(const snemo::datamodel::particle_track &);

      /// Check a particle is positron
      static bool particle_is_positron(const snemo::datamodel::particle_track &);

      /// Check a particle is alpha
      static bool particle_is_alpha(const snemo::datamodel::particle_track &);

      /// Check a particle is gamma
      static bool particle_is_gamma(const snemo::datamodel::particle_track &);

    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_PID_UTILS_H
