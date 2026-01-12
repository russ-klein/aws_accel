//==================================================================
// File: ccs_dw_lib.h
// Description: provides a C++ interface to Designware(r) FP blocks.
// 
// NOTE: This header file is now depricated and the contents of
//       the associated ccs_designware.lib Catapult Library
//       are now included in the base Catapult Library when it is
//       created.
//       Please use the header ccs_dw_fp_lib.h and update your
//       base library to add designware support.
//
// BETA version
//==================================================================

#pragma once

#warning This header file 'ccs_dw_lib.h' is being depricated in a future Catapult release.
#warning Please switch to include ccs_dw_fp_lib.h after updating your base.
#warning Catapult Library to include dware support.

// Define this macro to enable existing ccs_designware.lib libraries
// to work with new Catapult release.
#ifndef USING_CCS_LEGACY_DW_LIB
#define USING_CCS_LEGACY_DW_LIB
#endif
#include <ccs_dw_fp_lib.h>
