#pragma once


#include <ac_std_float.h>
#include <ac_blackbox.h>

//
// Define a namespace to hold the implementations that will map
// to DesignWare blocks using ac_blackbox()

namespace ccs_dw_bb
{
  template<int W, int E>
  static void recode_nan(ac_std_float<W,E> &z) {
    // make changes to factor real behavior of the RTL
    //   IEEE allows different ways to encode NaN
    //      DW has different encoding than generic implementation in ac_std_float.h
    if (z.isnan()) {
      ac_int<W,false> d = 1;
      d.set_slc(W-E-1, ac_int<E,false>(-1));
      z.set_data(d);
    }
  }

  //--------------------------------------------------------------------------------------
  // Map to DW_fp_add
  template<int W, int E>
  class fp_add
  {
  public:
    fp_add() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false> a, ac_int<W,false> b, ac_int<3,false> rnd, ac_int<W,false> &z, ac_int<8,false> &status) {
      enum { sig_width = W-E-1, exp_width = E, ieee_compliance = 1 };
      ac_blackbox()
      .entity("ccs_dw_fp_add_v1")
      .architecture("bb")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_add_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_add_v1.vhd")
      .library("ccs_dw_fp_add_v1_lib")
      .package("ccs_dw_fp_add_v1_pkg")
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")    // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver")       // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02")     // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("ieee_compliance", ieee_compliance)
      .area(W*10000.0)
      .delay(1.75)
      .end();

      sim_model(a,b,rnd,z);
    }

    static void sim_model ( ac_std_float<W,E> a, ac_std_float<W,E> b, ac_int<3,false> rnd, ac_std_float<W,E> &z) {
      if (!rnd) {
        z = a.template add_generic<AC_RND_CONV,false>(b); // call generic implementation for simulation
      } else if (rnd == 1) {
        z = a.template add_generic<AC_TRN_ZERO,false>(b); // call generic implementation for simulation
      } else {
        AC_ASSERT(false, "Rounding type not supported");
      }
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<W,false> b_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, b, z;
      a.set_data(a_d);
      b.set_data(b_d);
      sim_model(a,b,rnd,z);
      z_d = z.data();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_lp_piped_fp_add
  template<int W, int E, int stages>
  class lp_piped_fp_add
  {
  public:
    lp_piped_fp_add() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false> a, ac_int<W,false> b, ac_int<3,false> rnd, ac_int<W,false> &z, ac_int<8,false> &status) {
    enum { sig_width       = W-E-1, // translate template parameter to DW parameter
           exp_width       = E,     // translate template parameter to DW parameter
           in_reg          = 1,     // configuration (0 or 1 allowed)
           //stages,                // a template parameter
           out_reg         = 0,     // configuration (0 or 1 allowed)
           no_pm           = 0,     // 0 means use pipe manager, 1 no pipe manager
           rst_mode        = 1      // 0 means async, 1 means sync reset. (always active low)
         };
      ac_blackbox()
      .entity("ccs_dw_lp_piped_fp_add_v1")
      .architecture("bb")
      .clock_name("clk")
      .posedge_clock(true)
      .sync_reset_name("rst_n")
      .active_high_sync_reset(false) // for sync reset - always active-low
      //.posedge_async_reset(false)  // for areset reset - always active-low
      .start_name("start")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_lp_piped_fp_add_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_lp_piped_fp_add_v1.vhd")
      .library("ccs_dw_lp_piped_fp_add_v1_lib")
      .package("ccs_dw_lp_piped_fp_add_v1_pkg")
      .inputs_registered(in_reg)
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")    // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver DW03_ver")       // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02 DW03")     // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("in_reg", in_reg)
      .parameter("stages", stages)
      .parameter("out_reg", out_reg)
      .parameter("no_pm", no_pm)
      .parameter("rst_mode", rst_mode)
      .area(W*10000.0)
      .delay(1.75)
      .latency(stages+in_reg+out_reg-1) // SeqDelay
      .init_delay(1)                    // allow pipeline initiation interval of 1 or greater
      .has_state(true)                  // must be true
      .end();

      sim_model(a,b,rnd,z);
    }

    static void sim_model ( ac_std_float<W,E> a, ac_std_float<W,E> b, ac_int<3,false> rnd, ac_std_float<W,E> &z) {
      if (!rnd) {
        z = a.template add_generic<AC_RND_CONV,false>(b); // call generic implementation for simulation
      } else if (rnd == 1) {
        z = a.template add_generic<AC_TRN_ZERO,false>(b); // call generic implementation for simulation
      } else {
        AC_ASSERT(false, "Rounding type not supported");
      }
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<W,false> b_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, b, z;
      a.set_data(a_d);
      b.set_data(b_d);
      sim_model(a,b,rnd,z);
      z_d = z.data();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_fp_mult
  template<int W, int E>
  class fp_mult
  {
  public:
    fp_mult() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false> a, ac_int<W,false> b, ac_int<3,false> rnd, ac_int<W,false> &z, ac_int<8,false> &status) {
      enum { sig_width = W-E-1, exp_width = E, ieee_compliance = 1 };
      ac_blackbox()
      .entity("ccs_dw_fp_mult_v1")
      .architecture("bb")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_mult_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_mult_v1.vhd")
      .library("ccs_dw_fp_mult_v1_lib")
      .package("ccs_dw_fp_mult_v1_pkg")
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")    // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver")       // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02")     // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("ieee_compliance", ieee_compliance)
      .area(W*200.0)
      .delay(0.75)
      .end();

      sim_model(a,b,rnd,z);
    }

    static void sim_model ( ac_std_float<W,E> a, ac_std_float<W,E> b, ac_int<3,false> rnd, ac_std_float<W,E> &z) {
      if (!rnd) {
        z = a.template mult_generic<AC_RND_CONV,false>(b); // call generic implementation for simulation
      } else if (rnd == 1) {
        z = a.template mult_generic<AC_TRN_ZERO,false>(b); // call generic implementation for simulation
      } else {
        AC_ASSERT(false, "Rounding type not supported");
      }
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<W,false> b_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, b, z;
      a.set_data(a_d);
      b.set_data(b_d);
      sim_model(a,b,rnd,z);
      z_d = z.data();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_lp_piped_fp_mult
  template<int W, int E, int stages>
  class lp_piped_fp_mult
  {
  public:
    lp_piped_fp_mult() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false> a, ac_int<W,false> b, ac_int<3,false> rnd, ac_int<W,false> &z, ac_int<8,false> &status) {
    enum { sig_width       = W-E-1, // translate template parameter to DW parameter
           exp_width       = E,     // translate template parameter to DW parameter
           in_reg          = 1,     // configuration (0 or 1 allowed)
           //stages,                // a template parameter
           out_reg         = 0,     // configuration (0 or 1 allowed)
           no_pm           = 0,     // 0 means use pipe manager, 1 no pipe manager
           rst_mode        = 1      // 0 means async, 1 means sync reset. (always active low)
         };
      ac_blackbox()
      .entity("ccs_dw_lp_piped_fp_mult_v1")
      .architecture("bb")
      .clock_name("clk")
      .posedge_clock(true)
      .sync_reset_name("rst_n")
      .active_high_sync_reset(false) // for sync reset - always active-low
      //.posedge_async_reset(false)  // for areset reset - always active-low
      .start_name("start")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_lp_piped_fp_mult_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_lp_piped_fp_mult_v1.vhd")
      .library("ccs_dw_lp_piped_fp_mult_v1_lib")
      .package("ccs_dw_lp_piped_fp_mult_v1_pkg")
      .inputs_registered(in_reg)
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")    // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver DW03_ver")       // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02 DW03")     // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("in_reg", in_reg)
      .parameter("stages", stages)
      .parameter("out_reg", out_reg)
      .parameter("no_pm", no_pm)
      .parameter("rst_mode", rst_mode)
      .area(W*200.0)
      .delay(0.75)
      .latency(stages+in_reg+out_reg-1) // SeqDelay
      .init_delay(1)                    // allow pipeline initiation interval of 1 or greater
      .has_state(true)                  // must be true
      .end();

      sim_model(a,b,rnd,z);
    }

    static void sim_model ( ac_std_float<W,E> a, ac_std_float<W,E> b, ac_int<3,false> rnd, ac_std_float<W,E> &z) {
      if (!rnd) {
        z = a.template mult_generic<AC_RND_CONV,false>(b); // call generic implementation for simulation
      } else if (rnd == 1) {
        z = a.template mult_generic<AC_TRN_ZERO,false>(b); // call generic implementation for simulation
      } else {
        AC_ASSERT(false, "Rounding type not supported");
      }
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<W,false> b_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, b, z;
      a.set_data(a_d);
      b.set_data(b_d);
      sim_model(a,b,rnd,z);
      z_d = z.data();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_fp_div
  template<int W, int E>
  class fp_div
  {
  public:
    fp_div() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false> a, ac_int<W,false> b, ac_int<3,false> rnd, ac_int<W,false> &z, ac_int<8,false> &status) {
      enum { sig_width = W-E-1, exp_width = E, ieee_compliance = 1 };
      ac_blackbox()
      .entity("ccs_dw_fp_div_v1")
      .architecture("bb")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_div_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_div_v1.vhd")
      .library("ccs_dw_fp_div_v1_lib")
      .package("ccs_dw_fp_div_v1_pkg")
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")    // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver")       // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02")     // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("ieee_compliance", ieee_compliance)
      .area(W*10000.0)
      .delay(1.75)
      .end();

      sim_model(a,b,rnd,z);
    }

    static void sim_model ( ac_std_float<W,E> a, ac_std_float<W,E> b, ac_int<3,false> rnd, ac_std_float<W,E> &z) {
      if (!rnd) {
        z = a.template div_generic<AC_RND_CONV,false>(b); // call generic implementation for simulation
      } else if (rnd == 1) {
        z = a.template div_generic<AC_TRN_ZERO,false>(b);    // call generic implementation for simulation
      } else {
        AC_ASSERT(false, "Rounding type not supported");
      }
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<W,false> b_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, b, z;
      a.set_data(a_d);
      b.set_data(b_d);
      sim_model(a,b,rnd,z);
      z_d = z.data();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_fp_recip
  template<int W, int E>
  class fp_recip
  {
  public:
    fp_recip() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false>      a,
             ac_int<3,false>      rnd, 
             ac_int<W,false>     &z, 
             ac_int<8,false>     &status) {
      enum { sig_width       = W-E-1, // translate template parameter to DW parameter
             exp_width       = E      // translate template parameter to DW parameter
           };
      ac_blackbox()
      .entity("ccs_dw_fp_recip_v1")
      .architecture("bb")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_recip_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_recip_v1.vhd")
      .library("ccs_dw_fp_recip_v1_lib")
      .package("ccs_dw_fp_recip_v1_pkg")
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")               // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02 DW03")           // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver DW03_ver") // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .area(W*10000.0)
      .delay(1.75)
      .end();

      sim_model(a,rnd,z);
    }

    static void sim_model (ac_std_float<W,E> a, ac_int<3,false> rnd, ac_std_float<W,E> &z) 
    {
      ac_std_float<W,E> one = ac_std_float<W,E>::one();
      if(!rnd)
        z = one.template div_generic<AC_RND_CONV,false>(a);   // call generic implementation for simulation
      else if(rnd == 1)
        z = one.template div_generic<AC_TRN_ZERO,false>(a);   // call generic implementation for simulation
      else
        AC_ASSERT(false, "Rounding type not supported");
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, z;
      a.set_data(a_d);
      sim_model(a,rnd,z);
      z_d = z.data_ac_int();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_lp_piped_fp_recip
  template<int W, int E, int stages>
  class lp_piped_fp_recip
  {
  public:
    lp_piped_fp_recip() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false>      a,
             ac_int<3,false>      rnd, 
             ac_int<W,false>     &z, 
             ac_int<8,false>     &status) {
      enum { sig_width       = W-E-1, // translate template parameter to DW parameter
             exp_width       = E,     // translate template parameter to DW parameter
             in_reg          = 1,     // configuration (0 or 1 allowed)
             //stages,                // a template parameter
             out_reg         = 0,     // configuration (0 or 1 allowed)
             no_pm           = 0,     // 0 means use pipe manager, 1 no pipe manager
             rst_mode        = 1      // 0 means async, 1 means sync reset. (always active low)
           };
      ac_blackbox()
      .entity("ccs_dw_lp_piped_fp_recip_v1")
      .architecture("bb")
      .clock_name("clk")
      .posedge_clock(true)
      .sync_reset_name("rst_n")
      .active_high_sync_reset(false) // for sync reset - always active-low
      //.posedge_async_reset(false)  // for areset reset - always active-low
      .start_name("start")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_lp_piped_fp_recip_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_lp_piped_fp_recip_v1.vhd")
      .library("ccs_dw_lp_piped_fp_recip_v1_lib")
      .package("ccs_dw_lp_piped_fp_recip_v1_pkg")
      .inputs_registered(in_reg)
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")               // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02 DW03")           // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver DW03_ver") // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("in_reg", in_reg)
      .parameter("stages", stages)
      .parameter("out_reg", out_reg)
      .parameter("no_pm", no_pm)
      .parameter("rst_mode", rst_mode)
      .area(W*10000.0)
      .delay(1.75)
      .latency(stages+in_reg+out_reg-1) // SeqDelay
      .init_delay(1)                    // allow pipeline initiation interval of 1 or greater
      .has_state(true)                  // must be true
      .end();

      sim_model(a,rnd,z);
    }

    static void sim_model (ac_std_float<W,E> a, ac_int<3,false> rnd, ac_std_float<W,E> &z) 
    {
      ac_std_float<W,E> one = ac_std_float<W,E>::one();
      if(!rnd)
        z = one.template div_generic<AC_RND_CONV,false>(a);   // call generic implementation for simulation
      else if(rnd == 1)
        z = one.template div_generic<AC_TRN_ZERO,false>(a);   // call generic implementation for simulation
      else
        AC_ASSERT(false, "Rounding type not supported");
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, z;
      a.set_data(a_d);
      sim_model(a,rnd,z);
      z_d = z.data_ac_int();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_fp_fma
  template<int W, int E>
  class fp_fma
  {
  public:
    fp_fma() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false> a, ac_int<W,false> b, ac_int<W,false> c, ac_int<3,false> rnd, ac_int<W,false> &z, ac_int<8,false> &status) {
      enum { sig_width = W-E-1, exp_width = E, ieee_compliance = 1 };
      ac_blackbox()
      .entity("ccs_dw_fp_mac_v1")
      .architecture("bb")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_mac_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_mac_v1.vhd")
      .library("ccs_dw_fp_mac_v1_lib")
      .package("ccs_dw_fp_mac_v1_pkg")
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")    // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver")       // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02")     // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("ieee_compliance", ieee_compliance)
      .area(W*10000.0)
      .delay(1.75)
      .end();

      sim_model(a,b,c,rnd,z);
    }

    static void sim_model ( ac_std_float<W,E> a, ac_std_float<W,E> b, ac_std_float<W,E> c, ac_int<3,false> rnd, ac_std_float<W,E> &z) {
      if (!rnd) {
        z = a.template fma_generic<AC_RND_CONV,false>(b,c);    // call generic implementation for simulation
      } else if (rnd == 1) {
        z = a.template fma_generic<AC_TRN_ZERO,false>(b,c);    // call generic implementation for simulation
      } else {
        AC_ASSERT(false, "Rounding type not supported");
      }
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<W,false> b_d, ac_int<W,false> c_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, b, c, z;
      a.set_data(a_d);
      b.set_data(b_d);
      c.set_data(c_d);
      sim_model(a,b,c,rnd,z);
      z_d = z.data();
    }
  };

  //--------------------------------------------------------------------------------------
  // Map to DW_fp_sqrt
  template<int W, int E>
  class fp_sqrt
  {
  public:
    fp_sqrt() {}

    // The interface below uses ac_int<> so that pin names will exactly match the pin names of the DesignWare block.
    // If the interface is changed to use ac_std_float<> or ac_ieee_float<> then the RTL wrapper will need to
    // be modified to match the resulting pin names a_d, b_d, z_d, etc.
    #pragma design interface ccore blackbox
    void run(ac_int<W,false> a, ac_int<3,false> rnd, ac_int<W,false> &z, ac_int<8,false> &status) {
      enum { sig_width = W-E-1, exp_width = E, ieee_compliance = 1 };
      ac_blackbox()
      .entity("ccs_dw_fp_sqrt_v1")
      .architecture("bb")
      .outputs("z status")
      .verilog_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_sqrt_v1.v")
      .vhdl_files("$MGC_HOME/pkgs/siflibs/dware/ccs_dw_fp_sqrt_v1.vhd")
      .library("ccs_dw_fp_sqrt_v1_lib")
      .package("ccs_dw_fp_sqrt_v1_pkg")
      .library_variable("SIMLIBS_MK","ccs_DWARE.mk")    // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_V","DWARE_ver DW02_ver")       // For SCVerify precompiled DWARE package
      .library_variable("SIMLIBS_VHD","DWARE DW02")     // For SCVerify precompiled DWARE package
      .parameter("sig_width", sig_width)
      .parameter("exp_width", exp_width)
      .parameter("ieee_compliance", ieee_compliance)
      .area(W*10000.0)
      .delay(1.75)
      .end();

      sim_model(a,rnd,z);
    }

    static void sim_model ( ac_std_float<W,E> a, ac_int<3,false> rnd, ac_std_float<W,E> &z) {
      if (!rnd) {
        z = a.template sqrt_generic<AC_RND_CONV,false>();    // call generic implementation for simulation
      } else if (rnd == 1) {
        z = a.template sqrt_generic<AC_TRN_ZERO,false>();     // call generic implementation for simulation
      } else {
        AC_ASSERT(false, "Rounding type not supported");
      }
      recode_nan(z);
    }

    static void sim_model ( ac_int<W,false> a_d, ac_int<3,false> rnd, ac_int<W,false> &z_d ) {
      // just wrapper to interface to ac_std_float sim_model
      ac_std_float<W,E> a, z;
      a.set_data(a_d);
      sim_model(a,rnd,z);
      z_d = z.data();
    }
  };

};
