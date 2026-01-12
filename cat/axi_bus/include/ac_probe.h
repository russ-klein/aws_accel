////////////////////////////////////////////////////////////////////////////////
// Catapult Synthesis
// 
// Copyright (c) 2003-2015 Mentor Graphics Corp.
//       All Rights Reserved
// 
// This document contains information that is proprietary to Mentor Graphics
// Corp. The original recipient of this document may duplicate this  
// document in whole or in part for internal business purposes only, provided  
// that this entire notice appears in all copies. In duplicating any part of  
// this document, the recipient agrees to make every reasonable effort to  
// prevent the unauthorized use and distribution of the proprietary information.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AC_PROBE_H
#define __AC_PROBE_H

#include <string>
#include <ac_int.h>

// SCVerify user probe monitors are enabled for the post extract RTL netlist only
#if defined(CCS_SCVERIFY) && defined(CCS_DUT_RTL) && !defined(CCS_DUT_SYSC) && !defined(CCS_SYSC) && !defined(CCS_DUT_POWER)
#define CCS_ENABLE_SCVERIFY_USER_PROBES 1
#endif

// Defining classes and functions to avoid compilation errors in scverify
#ifdef __AC_NAMESPACE
namespace __AC_NAMESPACE {
#endif
using std::string;
namespace ac {
  enum ac_probe_punch_t {
    ProbePunchNone = 0,
    ProbePunch     = 1,
    ProbePunchReg  = 2
  };

  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(string prbnm, T val, ac_probe_punch_t pnch = ac::ProbePunchNone);
  #if CCS_ENABLE_SCVERIFY_USER_PROBES
  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(string prbnm, T* val, ac_probe_punch_t pnch = ac::ProbePunchNone);
  #endif

  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(const char *prbnm, T val, ac_probe_punch_t pnch = ac::ProbePunchNone);
  #if CCS_ENABLE_SCVERIFY_USER_PROBES
  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(const char *prbnm, T* val, ac_probe_punch_t pnch = ac::ProbePunchNone);
  #endif

  template <typename T>
  void probe_map(const char*prbnm, T inp);

  template <typename T>
  T passthrough(T inp);

  template <class T, int size>
  struct probed_array;
}
#ifdef __AC_NAMESPACE
}
#endif


#if CCS_ENABLE_SCVERIFY_USER_PROBES
#include "ccs_probes.h"
#endif

#ifdef __AC_NAMESPACE
namespace __AC_NAMESPACE {
#endif
using std::string;
namespace ac {
  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(string prbnm, T val, ac_probe_punch_t pnch) {
  #if CCS_ENABLE_SCVERIFY_USER_PROBES
    enum {thislv = ccs_scverify_probe_traits<T>::lvwidth};
    sc_dt::sc_lv<thislv> tmp;
	  type_to_vector(val,thislv,tmp);
    ccs_probe_fifo_put<thislv>(prbnm, tmp);
  #endif
  }
  #if CCS_ENABLE_SCVERIFY_USER_PROBES
  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(string prbnm, T* val, ac_probe_punch_t pnch) {
    enum {thislv = ccs_scverify_probe_traits<T>::lvwidth};
    sc_dt::sc_lv<thislv> tmp;
	  type_to_vector(*val,thislv,tmp);
    ccs_probe_fifo_put<thislv>(prbnm, tmp);
  }
  #endif

  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(const char *prbnm, T val, ac_probe_punch_t pnch) {
  #if CCS_ENABLE_SCVERIFY_USER_PROBES
    enum {thislv = ccs_scverify_probe_traits<T>::lvwidth};
    sc_dt::sc_lv<thislv> tmp;
	  type_to_vector(val,thislv,tmp);
    ccs_probe_fifo_put<thislv>(prbnm, tmp);
  #endif
  }
  #if CCS_ENABLE_SCVERIFY_USER_PROBES
  #pragma calypto_flag AC_PROBE
  template <class T>
  inline void probe(const char *prbnm, T* val, ac_probe_punch_t pnch) {
    enum {thislv = ccs_scverify_probe_traits<T>::lvwidth};
    sc_dt::sc_lv<thislv> tmp;
	  type_to_vector(*val,thislv,tmp);
    ccs_probe_fifo_put<thislv>(prbnm, tmp);
  }
  #endif

  #pragma map_to_operator [CCORE]
  #pragma ccore_type combinational
  #pragma calypto_flag AC_PROBE_MAP
  #pragma preserve
  template <typename T>
  void probe_map(const char*prbnm, T inp) { }

  #pragma map_to_operator [CCORE]
  #pragma ccore_type combinational
  template <typename T>
  T passthrough(T inp) { return inp; }

  // This class allows all reads (but not writes) performed
  // via the [] operator (not via pointer dereferencing yet)
  // to be automatically probed.
  #define PROBED_ARRAY 1
  template <class T, int size>
  struct probed_array {
       T arr[size];

       inline T &operator[](int i) {
            T &t = arr[i];
            ac::probe_map("mem", t);
            return t;
       }
  };
}

#ifdef __AC_NAMESPACE
}
#endif

#endif
