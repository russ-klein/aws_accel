//------------------------------------------------------------------------------
// Catapult Synthesis - Sample I/O Port Library
//
// Copyright (c) 2003-2015 Mentor Graphics Corp.
//       All Rights Reserved
//
// This document may be used and distributed without restriction provided that
// this copyright statement is not removed from the file and that any derivative
// work contains this copyright notice.
//
// The design information contained in this file is intended to be an example
// of the functionality which the end user may study in preparation for creating
// their own custom interfaces. This design does not necessarily present a 
// complete implementation of the named protocol or standard.
//
//------------------------------------------------------------------------------

#ifndef __CCS_P2P_CHECKER_H
#define __CCS_P2P_CHECKER_H

#include <systemc.h>
#include <sstream>

// Helper Classes for checking that reset was called and all ports are bound
class ccs_p2p_checker {
  mutable bool is_ok;
  #ifndef __SYNTHESIS__
  const char *objname;
  std::stringstream error_string;
  #endif
 
public:
  ccs_p2p_checker (const char *name, const char *func_name, const char *operation) : 
    is_ok(false) 
    {
    #ifndef __SYNTHESIS__
      objname = name;
      error_string << "You must " << func_name << " before you can " << operation << ".";
    #endif
    }
 
  inline void ok () {
    is_ok = true;
  }

  inline void test () const {
  #ifndef __SYNTHESIS__
    if ( !is_ok ) {
      SC_REPORT_ERROR(objname, error_string.str().c_str());
      is_ok = true; // Only report error message one time
    }
  #endif
  } 
};

#endif

