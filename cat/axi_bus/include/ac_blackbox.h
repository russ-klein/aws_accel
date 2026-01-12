////////////////////////////////////////////////////////////////////////////////
// Catapult Synthesis
// 
// Copyright (c) 2019-2020 Mentor Graphics Corp.
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

#ifndef __AC_BLACKBOX_H
#define __AC_BLACKBOX_H

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
# define GCC_HAS_PRAGMA_DIAGNOSTIC
#endif

#ifdef  GCC_HAS_PRAGMA_DIAGNOSTIC
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-value"
#endif

#if defined(_MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4100 )  // unreferenced formal parameter
# pragma warning( disable : 4512 )  // assignment operator could not be generated
#endif

#ifdef __AC_NAMESPACE
namespace __AC_NAMESPACE {
#endif

/* -------------------------------------------

//
// Example usage of ac_blackbox :
//

#include <ac_int.h>
#include <ac_blackbox.h>

class adder {
  public:
    adder() { }

#   pragma design interface ccore blackbox
    void run ( ac_int<8> a, ac_int<8> b, ac_int<8> &z ) {

      ac_blackbox()
          .entity("adder")
          .architecture("bb")
          .add_file("adder.vhd").set_do_not_touch().end_file()
          .verilog_files("adder.v")
          .outputs("z")
          .area(102.5)
          .delay(0.50)
          .end();

      z = a + b;
    }
};

*/

class ac_blackbox {
public:
    //
    // Category: Usage
    // ---------------

    // Defines the blackbox class object.  Must be placed at the start of
    // the class based interface function entry point.  The pragma design
    // interface specification also *requires* the blackbox keyword.
    ac_blackbox() : d(0) { }

    // Defines the end of the blackbox specification.  *required*
    void end() { }

    //
    // Category: Netlist / Reporting
    // -----------------------------

    // Defines the name of the module in RTL. Default is the typename 
    // of the class.
    ac_blackbox &entity(const char *name) { return *this; }

    //  Defines the library containing the RTL module, only needed for VHDL
    ac_blackbox &library(const char *name) { return *this; }

    //  Defines the component package containing the RTL module, only needed for
    //  VHDL modules with parameters.  Default for parameterized module
    //  is <entity>_pkg.
    ac_blackbox &package(const char *name) { return *this; }

    // Defines the name of the architecture in the RTL, only needed for VHDL
    ac_blackbox &architecture(const char *name) { return *this; }

    // Defines which of the ports on the black box are outputs.  Items not in
    // this list are assumed to be inputs.  Must match an argument name in 
    // the run function or Catapult will generate an error.
    ac_blackbox &outputs(const char *ports) { return *this; }

    // Defines the area of the component
    ac_blackbox &area(const float &value) { return *this; }

    // All inputs registered, default true
    ac_blackbox &inputs_registered(const bool &value) { return *this; }

    // Defines HDL parameter, call method for each parameter
    ac_blackbox &parameter(const char *name, const int &value) { return *this; }

    // Defines library module variable, call method for each library variable
    ac_blackbox &library_variable(const char *name, const char *value) { return *this; }

    // ------------------------------------------------------------------
    // Simple interface to specify list of files for netlist dependencies.
    // ------------------------------------------------------------------
    //
    // Defines one or more files that contain the VHDL RTL code.  Path
    // relative to the C module source file.
    ac_blackbox &vhdl_files(const char *files) { return *this; }
    //
    // Defines one or more files that contain the Verilog RTL code.  Path 
    // relative to the C module source file.
    ac_blackbox &verilog_files(const char *files) { return *this; }
    //
    // ------------------------------------------------------------------------
    // **Advanced** interface to specify additional details on a per file basis.
    // ------------------------------------------------------------------------
    // Example :
    //    ac_blackbox()
    //      ...
    //    .add_file("adder.vhd")
    //    .   set_static_file(false)
    //    .   set_do_not_touch(false)
    //    .end_file()
    //    .add_file("adder_syn.v").set_model_type(ac_blackbox::Synthesis_File).end_file()
    //    .add_file("adder_sim.v").set_model_type(ac_blackbox::Simulation_File).end_file()
    //      ...
    //    .end();
    //
    // Defines Netlist dependencies allowing for additional configuration options
    enum File_Netlist_Mode {
      VHDL_Netlist,            // file only included with VHDL output netlist
      Verilog_Netlist,         // file only included with Verilog output netlist
      Any_Netlist,             // file can be included with either VHDL or Verilog output netlists
    };
    enum File_Model_Type {
      All_Model_Types,         // file included for both synthesis and simulation dependency
      Synthesis_File,          // file only included for synthesis dependency 
      Simulation_File          // file only included for simulation dependency
    };
    enum File_Language {
      VHDL_File,               // VHDL file type for language
      Verilog_File,            // Verilog file type language
      SystemVerilog_File,      // SystemVerilog file type language
    };
    enum File_Language_Version {
      Any_Version,
      VHDL_Numeric_1987,
      VHDL_Numeric_1993,
      VHDL_Arith_1993,
      Verilog_1995,
      Verilog_2001
    };
    class File {
      public:
        // Defines HDL language type of file.
        // -- If not called the default based on file extension:
        //      ac_blackbox::VHDL_File           => .vhd .vhdl .hdl
        //      ac_blackbox::Verilog_File        => .v .ver .vo .vg .vlg .inc
        //      ac_blackbox::SystemVerilog_File  => .sv
        // -- Choices :
        //      ac_blackbox::VHDL_File           => VHDL file
        //      ac_blackbox::Verilog_File        => Verilog file
        //      ac_blackbox::SystemVerilog_File  => SystemVerilog file
        File &language(const File_Language &language) { return *this; }
        // Define supplemental file type information.
        // -- If not called the default is Any_Version
        // -- Choices :
        //      ac_blackbox::Any_Version
        //      ac_blackbox::VHDL_Numeric_1987
        //      ac_blackbox::VHDL_Numeric_1993
        //      ac_blackbox::VHDL_Arith_1993
        //      ac_blackbox::Verilog_1995
        //      ac_blackbox::Verilog_2001
        File &version(const File_Language_Version &version) { return *this; }
        // Defines if file should be included with synthesis or simulation scripts.
        // -- If not called the default will be both synthesis and simulation scripts.
        // -- Choices :
        //      ac_blackbox::All_Model_Types   => file included for both synthesis and simulation dependency
        //      ac_blackbox::Synthesis_File    => file only included for synthesis dependency
        //      ac_blackbox::Simulation_File   => file only included for simulation dependency
        File &model_type(const File_Model_Type &modeltype) { return *this; }
        // Defines HDL language of netlister when file is included as a dependency.
        // -- If not called the default will be based on the file language.
        // -- Choices :
        //      ac_blackbox::VHDL_Netlist      => file only included with VHDL output netlist
        //      ac_blackbox::Verilog_Netlist   => file only included with Verilog output netlist
        //      ac_blackbox::Any_Netlist       => file can be included with either VHDL or Verilog output netlists
        File &netlist_mode(const File_Netlist_Mode &netlist_mode) { return *this; }
        // Defines the library for this netlist file dependency.
        File &library(const char *name) { return *this; }
        // Defines the entity name for this netlist file dependency.
        File &entity(const char *name) { return *this; }
        // Defines the VHDL architecture for this netlist file dependency.
        File &architecture(const char *name) { return *this; }
        // Netlister will not prefix the file if true.
        // -- If not called the default will be true.
        File &static_file(const bool is_static_file=true) { return *this; }
        // Netlister will not prefix the file, include it in the concat netlist, or 
        // perform power optimizations on the file if true.
        // -- If not called the default will be true.
        File &do_not_touch(const bool is_do_not_touch=true) { return *this; }
        // Call at of add_file specification. *required*
        ac_blackbox &end_file() { ac_blackbox &res = d; delete this; return res; }
        //
      private:
        File(ac_blackbox &blackbox, const char *file) : d(blackbox) { }
        ~File() { }
        ac_blackbox &d;
        friend class ac_blackbox;
    };
    // Add new file and start file specification. Must call end_file()
    // method when all File specification methods have been called.
    // Path relative to the C module source file.
    File &add_file(const char *file) { return *(new File(*this, file)); }
    //
    // End of netlist dependencies
    ///////////////////////////////////////////////////////////////////////////////

    //
    // Category: Timing
    // ----------------

    // Defines the timing units in the library.  Allowed values are the same
    // as the available units in the library.  The timing units will be 
    // internally normalized to ns for Catapult's synthesis.
    ac_blackbox &timing_units(const char *value) { return *this; }

    // Defines the combinational delay of combinational components or output
    // delay of sequential components.
    ac_blackbox &delay(const float &value) { return *this; }

    // Defines the input delay of sequential components.
    ac_blackbox &input_delay(const float &value) { return *this; }

    // Defines the minimum clock period that is allowed when using the blackbox 
    // module.
    ac_blackbox &min_clock_period(const float &value) { return *this; }

    // Defines the latency of the sequential component. Number of register stages.
    // Internally this is converted to SeqDelay accounting for the input_register
    // setting.  If the input_register setting is true, then SeqDelay=latency-1
    // otherwise SeqDelay=latency
    ac_blackbox &latency(const unsigned int &value) { return *this; }

    // Defines the initiation interval for sequential components (Default calculated
    // SeqDelay+1)
    ac_blackbox &init_delay(const unsigned int &value) { return *this; }

    // Defines the clock-to-clock delay of the registers for sequential components
    // (excluding setup time).
    ac_blackbox &register_to_register_delay(const float &value) { return *this; }

    //
    // Category: Clock
    // ---------------

    // Defines the name of the clock port.  Multi-clock blackbox designs are not
    // supported in this flow.
    ac_blackbox &clock_name(const char *name) { return *this; }

    // Defines if the design is sensitive to the positive edge of the clock.
    // default is true (posedge)
    ac_blackbox &posedge_clock(const bool &value) { return *this; }

    // Defines the name of the synchronous reset port. Only one synchronous reset
    // port is allowed.
    ac_blackbox &sync_reset_name(const char *name) { return *this; }

    // Defines if the synchronous reset is active high or active low.
    // default is true (active high)
    ac_blackbox &active_high_sync_reset(const bool &value) { return *this; }

    // Defines the name of the asynchronous reset port. Only one asynchronous 
    // reset is allowed.
    ac_blackbox &async_reset_name(const char *name) { return *this; }

    // Defines if the asynchronous reset is sensitive to the posedge or negedge.
    // default is false (negedge)
    ac_blackbox &posedge_async_reset(const bool &value) { return *this; }

    // Defines the name of the enable port. Only one enable port is allowed.
    ac_blackbox &enable_name(const char *name) { return *this; }

    // Defines if the enable is active high or active low
    // default is true (active high)
    ac_blackbox &active_high_enable(const bool &value) { return *this; }

    //
    // Category: Handshake
    // -------------------

    // Defines the output port from the design that reports if the design is 
    // idle and the clock can be gated. Idle is always active high.
    ac_blackbox &idle_name(const char *name) { return *this; }

    // Defines if the HDL definition has state which prevents sharing the
    // instance. Default is false.  If true start_name is required.
    ac_blackbox &has_state(const bool &value) { return *this; }

    // Defines the start port in the HDL model.  Required for has_state true.
    ac_blackbox &start_name(const char *name) { return *this; }

    //Require ports to be netlisted as single bit datatypes instead of vectors
    ac_blackbox &scalar_ports(const char* ports) { return *this; }

private:
    void *const d;

protected:
    bool _Unused() { return !!d; } // silence clang unused datamember warning
};

#ifdef __AC_NAMESPACE
}
#endif

// restore warning level
#ifdef _MSC_VER
# pragma warning( pop )
#endif
#ifdef  GCC_HAS_PRAGMA_DIAGNOSTIC
# undef GCC_HAS_PRAGMA_DIAGNOSTIC
# pragma GCC diagnostic pop
#endif

#endif
