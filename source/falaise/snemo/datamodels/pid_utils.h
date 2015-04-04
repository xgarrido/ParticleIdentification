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

    struct pid_utils {

      /// The label of electron particle
      static const std::string & electron_label();

      /// The label of positron particle
      static const std::string & positron_label();

      /// The label of gamma particle
      static const std::string & gamma_label();

      /// The label of alpha particle
      static const std::string & alpha_label();

      /// The name of a string property representing the Particle Identification label
      static const std::string & pid_label_key();

    };

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_PID_UTILS_H
