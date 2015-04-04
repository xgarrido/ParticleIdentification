// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/pid_utils.cc

// Ourselves:
#include <falaise/snemo/datamodels/pid_utils.h>

namespace snemo {

  namespace datamodel {

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

    const std::string & pid_utils::pid_label_key()
    {
      static const std::string s("pid_utils.label");
      return s;
    }


  } // end of namespace datamodel

} // end of namespace snemo
