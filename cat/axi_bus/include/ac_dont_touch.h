////////////////////////////////////////////////////////////////////////////////
// Catapult Synthesis
// 
// Copyright (c) 2018 Mentor Graphics Corp.
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

#ifndef __AC_DONT_TOUCH_H
#define __AC_DONT_TOUCH_H

#ifdef __AC_NAMESPACE
namespace __AC_NAMESPACE {
#endif

namespace ac {
  enum ac_dt_until_step {
    dt_dont_dissolve      = 0,
    dt_until_libraries    = 5,
    dt_until_assembly     = 6,
    dt_until_loops        = 7,
    dt_until_memories     = 8,
    dt_until_cluster      = 9,
    dt_until_architect    = 10,
    dt_until_allocate     = 11,
    dt_until_schedule     = 12,
    dt_until_dpfsm        = 13,
    dt_until_instance     = 14,
    dt_until_extract      = 15,
    dt_until_dsp          = 32
  };


  template <typename Dtype>
  inline Dtype dont_touch(Dtype data, ac_dt_until_step dissolve_step=dt_dont_dissolve)
  {
    // Causes a wire to be inserted during synthesis where this function is called
    // prevent all optimization through this wire
    return data;
  }

  template <typename Dtype>
  inline Dtype incr_anchor(Dtype data, const char *name="")
  {
    // This is meant to be used for incremental flows to provide some boundaries for optimizations
    // Causes a wire to be inserted during synthesis where this function is called
    // prevent some optimization through this wire
    return data;
  }

}

#ifdef __AC_NAMESPACE
}
#endif

#endif
