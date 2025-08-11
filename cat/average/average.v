
//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/siflibs/ccs_out_buf_wait_v5.v 
//------------------------------------------------------------------------------
// Catapult Synthesis - Sample I/O Port Library
//
// Copyright (c) 2003-2017 Mentor Graphics Corp.
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

module ccs_out_buf_wait_v5 (clk, en, arst, srst, ivld, irdy, idat, rdy, vld, dat, is_idle);

    parameter integer  rscid   = 1;
    parameter integer  width   = 8;
    parameter integer  ph_clk  = 1;
    parameter integer  ph_en   = 1;
    parameter integer  ph_arst = 1;
    parameter integer  ph_srst = 1;
    parameter integer  rst_val = 0;

    input              clk;
    input              en;
    input              arst;
    input              srst;
    output             irdy;
    input              ivld;
    input  [width-1:0] idat;
    input              rdy;
    output             vld;
    output [width-1:0] dat;
    output             is_idle;

    reg                filled;
    wire               filled_next;
    wire               lbuf;
    reg    [width-1:0] abuf;

    assign irdy = ~filled_next;

    assign vld = filled | ivld;
    assign dat = filled ? abuf : idat;

    assign lbuf = ivld & ~filled & ~rdy;
    assign filled_next = filled ? ~rdy : lbuf;

    assign is_idle = ~lbuf & (filled ~^ filled_next);

    // Output registers:
    generate
    if (ph_arst == 0 && ph_clk == 1)
    begin: POS_CLK_NEG_ARST
        always @(posedge clk or negedge arst)
        if (arst == 1'b0)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            if (lbuf == 1'b1)
                abuf <= idat;
        end
    end
    else if (ph_arst == 1 && ph_clk == 1)
    begin: POS_CLK_POS_ARST
        always @(posedge clk or posedge arst)
        if (arst == 1'b1)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            if (lbuf == 1'b1)
                abuf <= idat;
        end
    end
    else if (ph_arst == 0 && ph_clk == 0)
    begin: NEG_CLK_NEG_ARST
        always @(negedge clk or negedge arst)
        if (arst == 1'b0)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            if (lbuf == 1'b1)
                abuf <= idat;
        end
    end
    else if (ph_arst == 1 && ph_clk == 0)
    begin: NEG_CLK_POS_ARST
        always @(negedge clk or posedge arst)
        if (arst == 1'b1)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            abuf <= {width{rst_val}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            if (lbuf == 1'b1)
                abuf <= idat;
        end
    end
    endgenerate
endmodule

//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/siflibs/ccs_ctrl_in_buf_wait_v4.v 
//------------------------------------------------------------------------------
// Catapult Synthesis - Sample I/O Port Library
//
// Copyright (c) 2003-2017 Mentor Graphics Corp.
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
// Change History:
//    2019-01-24 - Add assertion to verify rdy signal behavior under reset.
//                 Fix bug in that behavior.
//    2019-01-04 - Fixed bug 54073 - rdy signal should not be asserted during
//                 reset
//    2018-11-19 - Improved code coverage for is_idle
//    2018-08-22 - Added is_idle to interface (as compare to 
//                 ccs_ctrl_in_buf_wait_v2)
//------------------------------------------------------------------------------


module ccs_ctrl_in_buf_wait_v4 (clk, en, arst, srst, irdy, ivld, idat, vld, rdy, dat, is_idle);

    parameter integer rscid   = 1;
    parameter integer width   = 8;
    parameter integer ph_clk  = 1;
    parameter integer ph_en   = 1;
    parameter integer ph_arst = 1;
    parameter integer ph_srst = 1;

    input              clk;
    input              en;
    input              arst;
    input              srst;
    output             rdy;
    input              vld;
    input  [width-1:0] dat;
    input              irdy;
    output             ivld;
    output [width-1:0] idat;
    output             is_idle;

    wire               rdy_int;
    wire               vld_int;
    reg                filled;
    wire               filled_next;
    wire               lbuf;
    reg    [width-1:0] abuf;
    reg                hs_init;

    assign rdy_int = ~filled | irdy;
    assign rdy = rdy_int & hs_init;
    assign vld_int = vld & hs_init;

    assign ivld = filled_next;
    assign idat = abuf;

    assign lbuf = vld_int & rdy_int;
    assign filled_next = vld_int | (filled & ~irdy);

    assign is_idle = ~lbuf & (filled ~^ filled_next) & hs_init;

    // Output registers:
    generate
    if (ph_arst == 0 && ph_clk == 1)
    begin: POS_CLK_NEG_ARST
        always @(posedge clk or negedge arst)
        if (arst == 1'b0)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            hs_init <= 1'b1;
            if (lbuf == 1'b1)
                abuf <= dat;
        end
    end
    else if (ph_arst == 1 && ph_clk == 1)
    begin: POS_CLK_POS_ARST
        always @(posedge clk or posedge arst)
        if (arst == 1'b1)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            hs_init <= 1'b1;
            if (lbuf == 1'b1)
                abuf <= dat;
        end
    end
    else if (ph_arst == 0 && ph_clk == 0)
    begin: NEG_CLK_NEG_ARST
        always @(negedge clk or negedge arst)
        if (arst == 1'b0)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            hs_init <= 1'b1;
            if (lbuf == 1'b1)
                abuf <= dat;
        end
    end
    else if (ph_arst == 1 && ph_clk == 0)
    begin: NEG_CLK_POS_ARST
        always @(negedge clk or posedge arst)
        if (arst == 1'b1)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (srst == ph_srst)
        begin
            filled <= 1'b0;
            hs_init <= 1'b0;
            abuf <= {width{1'b0}};
        end
        else if (en == ph_en)
        begin
            filled <= filled_next;
            hs_init <= 1'b1;
            if (lbuf == 1'b1)
                abuf <= dat;
        end
    end
    endgenerate

`ifdef RDY_ASRT 
    generate
    if (ph_clk==1) 
    begin: POS_CLK_ASSERT

       property rdyAsrt ;
         @(posedge clk) (srst==ph_srst) |=> (rdy==0);
       endproperty
       a1: assert property(rdyAsrt);

       property rdyAsrtASync ;
         @(posedge clk) (arst==ph_arst) |-> (rdy==0);
       endproperty
       a2: assert property(rdyAsrtASync);

    end else if (ph_clk==0) 
    begin: NEG_CLK_ASSERT

       property rdyAsrt ;
         @(negedge clk) ((srst==ph_srst) || (arst==ph_arst)) |=> (rdy==0);
       endproperty
       a1: assert property(rdyAsrt);

       property rdyAsrtASync ;
         @(negedge clk) (arst==ph_arst) |-> (rdy==0);
       endproperty
       a2: assert property(rdyAsrtASync);
    end
    endgenerate

`endif

endmodule



//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/siflibs/mgc_io_sync_v2.v 
//------------------------------------------------------------------------------
// Catapult Synthesis - Sample I/O Port Library
//
// Copyright (c) 2003-2017 Mentor Graphics Corp.
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


module mgc_io_sync_v2 (ld, lz);
    parameter valid = 0;

    input  ld;
    output lz;

    wire   lz;

    assign lz = ld;

endmodule


//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/siflibs/ccs_in_v1.v 
//------------------------------------------------------------------------------
// Catapult Synthesis - Sample I/O Port Library
//
// Copyright (c) 2003-2017 Mentor Graphics Corp.
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


module ccs_in_v1 (idat, dat);

  parameter integer rscid = 1;
  parameter integer width = 8;

  output [width-1:0] idat;
  input  [width-1:0] dat;

  wire   [width-1:0] idat;

  assign idat = dat;

endmodule


//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/siflibs/ccs_out_v1.v 
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

module ccs_out_v1 (dat, idat);

  parameter integer rscid = 1;
  parameter integer width = 8;

  output   [width-1:0] dat;
  input    [width-1:0] idat;

  wire     [width-1:0] dat;

  assign dat = idat;

endmodule




//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/siflibs/mgc_shift_r_beh_v5.v 
module mgc_shift_r_v5(a,s,z);
   parameter    width_a = 4;
   parameter    signd_a = 1;
   parameter    width_s = 2;
   parameter    width_z = 8;

   input [width_a-1:0] a;
   input [width_s-1:0] s;
   output [width_z -1:0] z;

   generate
     if (signd_a)
     begin: SGNED
       assign z = fshr_u(a,s,a[width_a-1]);
     end
     else
     begin: UNSGNED
       assign z = fshr_u(a,s,1'b0);
     end
   endgenerate

   //Shift right - unsigned shift argument
   function [width_z-1:0] fshr_u;
      input [width_a-1:0] arg1;
      input [width_s-1:0] arg2;
      input sbit;
      parameter olen = width_z;
      parameter ilen = signd_a ? width_a : width_a+1;
      parameter len = (ilen >= olen) ? ilen : olen;
      reg signed [len-1:0] result;
      reg signed [len-1:0] result_t;
      begin
        result_t = $signed( {(len){sbit}} );
        result_t[width_a-1:0] = arg1;
        result = result_t >>> arg2;
        fshr_u =  result[olen-1:0];
      end
   endfunction // fshl_u

endmodule

//------> ./rtl.v 
// ----------------------------------------------------------------------
//  HLS HDL:        Verilog Netlister
//  HLS Version:    2025.2/1190995 Production Release
//  HLS Date:       Wed May 14 16:03:56 PDT 2025
// 
//  Generated by:   russk@orw-russk-vm
//  Generated date: Mon Aug 11 12:40:28 2025
// ----------------------------------------------------------------------

// 
// ------------------------------------------------------------------
//  Design Unit:    average_core_core_fsm
//  FSM Module
// ------------------------------------------------------------------


module average_core_core_fsm (
  clk, arst_n, core_wen, fsm_output, main_C_0_tr0, for_C_2_tr0
);
  input clk;
  input arst_n;
  input core_wen;
  output [4:0] fsm_output;
  reg [4:0] fsm_output;
  input main_C_0_tr0;
  input for_C_2_tr0;


  // FSM State Type Declaration for average_core_core_fsm_1
  parameter
    main_C_0 = 3'd0,
    for_C_0 = 3'd1,
    for_C_1 = 3'd2,
    for_C_2 = 3'd3,
    main_C_1 = 3'd4;

  reg [2:0] state_var;
  reg [2:0] state_var_NS;


  // Interconnect Declarations for Component Instantiations 
  always @(*)
  begin : average_core_core_fsm_1
    case (state_var)
      for_C_0 : begin
        fsm_output = 5'b00010;
        state_var_NS = for_C_1;
      end
      for_C_1 : begin
        fsm_output = 5'b00100;
        state_var_NS = for_C_2;
      end
      for_C_2 : begin
        fsm_output = 5'b01000;
        if ( for_C_2_tr0 ) begin
          state_var_NS = main_C_1;
        end
        else begin
          state_var_NS = for_C_0;
        end
      end
      main_C_1 : begin
        fsm_output = 5'b10000;
        state_var_NS = main_C_0;
      end
      // main_C_0
      default : begin
        fsm_output = 5'b00001;
        if ( main_C_0_tr0 ) begin
          state_var_NS = main_C_1;
        end
        else begin
          state_var_NS = for_C_0;
        end
      end
    endcase
  end

  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      state_var <= main_C_0;
    end
    else if ( core_wen ) begin
      state_var <= state_var_NS;
    end
  end

endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_staller
// ------------------------------------------------------------------


module average_core_staller (
  clk, arst_n, core_wen, core_wten, memory_channels_ar_channel_rsci_wen_comp, memory_channels_r_channel_rsci_wen_comp,
      memory_channels_ar_channel_rsci_wen_comp_pff, memory_channels_r_channel_rsci_wen_comp_pff
);
  input clk;
  input arst_n;
  output core_wen;
  output core_wten;
  reg core_wten;
  input memory_channels_ar_channel_rsci_wen_comp;
  input memory_channels_r_channel_rsci_wen_comp;
  input memory_channels_ar_channel_rsci_wen_comp_pff;
  input memory_channels_r_channel_rsci_wen_comp_pff;



  // Interconnect Declarations for Component Instantiations 
  assign core_wen = memory_channels_ar_channel_rsci_wen_comp_pff & memory_channels_r_channel_rsci_wen_comp_pff;
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      core_wten <= 1'b0;
    end
    else begin
      core_wten <= ~(memory_channels_ar_channel_rsci_wen_comp & memory_channels_r_channel_rsci_wen_comp);
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_result_triosy_obj_result_triosy_wait_ctrl
// ------------------------------------------------------------------


module average_core_result_triosy_obj_result_triosy_wait_ctrl (
  core_wten, result_triosy_obj_iswt0, result_triosy_obj_biwt
);
  input core_wten;
  input result_triosy_obj_iswt0;
  output result_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign result_triosy_obj_biwt = (~ core_wten) & result_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_index_triosy_obj_index_triosy_wait_ctrl
// ------------------------------------------------------------------


module average_core_index_triosy_obj_index_triosy_wait_ctrl (
  core_wten, index_triosy_obj_iswt0, index_triosy_obj_biwt
);
  input core_wten;
  input index_triosy_obj_iswt0;
  output index_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign index_triosy_obj_biwt = (~ core_wten) & index_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_count_triosy_obj_count_triosy_wait_ctrl
// ------------------------------------------------------------------


module average_core_count_triosy_obj_count_triosy_wait_ctrl (
  core_wten, count_triosy_obj_iswt0, count_triosy_obj_biwt
);
  input core_wten;
  input count_triosy_obj_iswt0;
  output count_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign count_triosy_obj_biwt = (~ core_wten) & count_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl
// ------------------------------------------------------------------


module average_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl
    (
  memory_channels_r_channel_rsci_iswt0, memory_channels_r_channel_rsci_ivld_oreg,
      memory_channels_r_channel_rsci_biwt, memory_channels_r_channel_rsci_biwt_pff,
      memory_channels_r_channel_rsci_iswt0_pff, memory_channels_r_channel_rsci_ivld_oreg_pff
);
  input memory_channels_r_channel_rsci_iswt0;
  input memory_channels_r_channel_rsci_ivld_oreg;
  output memory_channels_r_channel_rsci_biwt;
  output memory_channels_r_channel_rsci_biwt_pff;
  input memory_channels_r_channel_rsci_iswt0_pff;
  input memory_channels_r_channel_rsci_ivld_oreg_pff;



  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_r_channel_rsci_biwt = memory_channels_r_channel_rsci_iswt0
      & memory_channels_r_channel_rsci_ivld_oreg;
  assign memory_channels_r_channel_rsci_biwt_pff = memory_channels_r_channel_rsci_iswt0_pff
      & memory_channels_r_channel_rsci_ivld_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_wait_dp
// ------------------------------------------------------------------


module average_core_wait_dp (
  clk, arst_n, memory_channels_ar_channel_rsci_irdy, memory_channels_ar_channel_rsci_irdy_oreg,
      memory_channels_r_channel_rsci_ivld, memory_channels_r_channel_rsci_ivld_oreg
);
  input clk;
  input arst_n;
  input memory_channels_ar_channel_rsci_irdy;
  output memory_channels_ar_channel_rsci_irdy_oreg;
  input memory_channels_r_channel_rsci_ivld;
  output memory_channels_r_channel_rsci_ivld_oreg;


  // Interconnect Declarations
  reg memory_channels_ar_channel_rsci_irdy_oreg_rneg;
  reg memory_channels_r_channel_rsci_ivld_oreg_rneg;


  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_ar_channel_rsci_irdy_oreg = ~ memory_channels_ar_channel_rsci_irdy_oreg_rneg;
  assign memory_channels_r_channel_rsci_ivld_oreg = ~ memory_channels_r_channel_rsci_ivld_oreg_rneg;
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      memory_channels_ar_channel_rsci_irdy_oreg_rneg <= 1'b0;
      memory_channels_r_channel_rsci_ivld_oreg_rneg <= 1'b0;
    end
    else begin
      memory_channels_ar_channel_rsci_irdy_oreg_rneg <= ~ memory_channels_ar_channel_rsci_irdy;
      memory_channels_r_channel_rsci_ivld_oreg_rneg <= ~ memory_channels_r_channel_rsci_ivld;
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl
// ------------------------------------------------------------------


module average_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl
    (
  memory_channels_ar_channel_rsci_iswt0, memory_channels_ar_channel_rsci_irdy_oreg,
      memory_channels_ar_channel_rsci_biwt, memory_channels_ar_channel_rsci_biwt_pff,
      memory_channels_ar_channel_rsci_iswt0_pff, memory_channels_ar_channel_rsci_irdy_oreg_pff
);
  input memory_channels_ar_channel_rsci_iswt0;
  input memory_channels_ar_channel_rsci_irdy_oreg;
  output memory_channels_ar_channel_rsci_biwt;
  output memory_channels_ar_channel_rsci_biwt_pff;
  input memory_channels_ar_channel_rsci_iswt0_pff;
  input memory_channels_ar_channel_rsci_irdy_oreg_pff;



  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_ar_channel_rsci_biwt = memory_channels_ar_channel_rsci_iswt0
      & memory_channels_ar_channel_rsci_irdy_oreg;
  assign memory_channels_ar_channel_rsci_biwt_pff = memory_channels_ar_channel_rsci_iswt0_pff
      & memory_channels_ar_channel_rsci_irdy_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_result_triosy_obj
// ------------------------------------------------------------------


module average_core_result_triosy_obj (
  result_triosy_lz, core_wten, result_triosy_obj_iswt0
);
  output result_triosy_lz;
  input core_wten;
  input result_triosy_obj_iswt0;


  // Interconnect Declarations
  wire result_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) result_triosy_obj (
      .ld(result_triosy_obj_biwt),
      .lz(result_triosy_lz)
    );
  average_core_result_triosy_obj_result_triosy_wait_ctrl average_core_result_triosy_obj_result_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .result_triosy_obj_iswt0(result_triosy_obj_iswt0),
      .result_triosy_obj_biwt(result_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_index_triosy_obj
// ------------------------------------------------------------------


module average_core_index_triosy_obj (
  index_triosy_lz, core_wten, index_triosy_obj_iswt0
);
  output index_triosy_lz;
  input core_wten;
  input index_triosy_obj_iswt0;


  // Interconnect Declarations
  wire index_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) index_triosy_obj (
      .ld(index_triosy_obj_biwt),
      .lz(index_triosy_lz)
    );
  average_core_index_triosy_obj_index_triosy_wait_ctrl average_core_index_triosy_obj_index_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .index_triosy_obj_iswt0(index_triosy_obj_iswt0),
      .index_triosy_obj_biwt(index_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_count_triosy_obj
// ------------------------------------------------------------------


module average_core_count_triosy_obj (
  count_triosy_lz, core_wten, count_triosy_obj_iswt0
);
  output count_triosy_lz;
  input core_wten;
  input count_triosy_obj_iswt0;


  // Interconnect Declarations
  wire count_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) count_triosy_obj (
      .ld(count_triosy_obj_biwt),
      .lz(count_triosy_lz)
    );
  average_core_count_triosy_obj_count_triosy_wait_ctrl average_core_count_triosy_obj_count_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .count_triosy_obj_iswt0(count_triosy_obj_iswt0),
      .count_triosy_obj_biwt(count_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_memory_channels_r_channel_rsci
// ------------------------------------------------------------------


module average_core_memory_channels_r_channel_rsci (
  clk, arst_n, memory_channels_r_channel_rsc_dat, memory_channels_r_channel_rsc_vld,
      memory_channels_r_channel_rsc_rdy, memory_channels_r_channel_rsci_oswt, memory_channels_r_channel_rsci_wen_comp,
      memory_channels_r_channel_rsci_ivld, memory_channels_r_channel_rsci_ivld_oreg,
      memory_channels_r_channel_rsci_idat_mxwt, memory_channels_r_channel_rsci_wen_comp_pff,
      memory_channels_r_channel_rsci_oswt_pff, memory_channels_r_channel_rsci_ivld_oreg_pff
);
  input clk;
  input arst_n;
  input [518:0] memory_channels_r_channel_rsc_dat;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;
  input memory_channels_r_channel_rsci_oswt;
  output memory_channels_r_channel_rsci_wen_comp;
  output memory_channels_r_channel_rsci_ivld;
  input memory_channels_r_channel_rsci_ivld_oreg;
  output [511:0] memory_channels_r_channel_rsci_idat_mxwt;
  output memory_channels_r_channel_rsci_wen_comp_pff;
  input memory_channels_r_channel_rsci_oswt_pff;
  input memory_channels_r_channel_rsci_ivld_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_r_channel_rsci_biwt;
  wire [518:0] memory_channels_r_channel_rsci_idat;
  wire memory_channels_r_channel_rsc_is_idle;
  wire memory_channels_r_channel_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  ccs_ctrl_in_buf_wait_v4 #(.rscid(32'sd11),
  .width(32'sd519),
  .ph_clk(32'sd1),
  .ph_en(32'sd0),
  .ph_arst(32'sd0),
  .ph_srst(32'sd0)) memory_channels_r_channel_rsci (
      .clk(clk),
      .en(1'b0),
      .arst(arst_n),
      .srst(1'b1),
      .rdy(memory_channels_r_channel_rsc_rdy),
      .vld(memory_channels_r_channel_rsc_vld),
      .dat(memory_channels_r_channel_rsc_dat),
      .irdy(memory_channels_r_channel_rsci_oswt),
      .ivld(memory_channels_r_channel_rsci_ivld),
      .idat(memory_channels_r_channel_rsci_idat),
      .is_idle(memory_channels_r_channel_rsc_is_idle)
    );
  average_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl
      average_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl_inst
      (
      .memory_channels_r_channel_rsci_iswt0(memory_channels_r_channel_rsci_oswt),
      .memory_channels_r_channel_rsci_ivld_oreg(memory_channels_r_channel_rsci_ivld_oreg),
      .memory_channels_r_channel_rsci_biwt(memory_channels_r_channel_rsci_biwt),
      .memory_channels_r_channel_rsci_biwt_pff(memory_channels_r_channel_rsci_biwt_iff),
      .memory_channels_r_channel_rsci_iswt0_pff(memory_channels_r_channel_rsci_oswt_pff),
      .memory_channels_r_channel_rsci_ivld_oreg_pff(memory_channels_r_channel_rsci_ivld_oreg_pff)
    );
  assign memory_channels_r_channel_rsci_idat_mxwt = memory_channels_r_channel_rsci_idat[514:3];
  assign memory_channels_r_channel_rsci_wen_comp = (~ memory_channels_r_channel_rsci_oswt)
      | memory_channels_r_channel_rsci_biwt;
  assign memory_channels_r_channel_rsci_wen_comp_pff = (~ memory_channels_r_channel_rsci_oswt_pff)
      | memory_channels_r_channel_rsci_biwt_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core_memory_channels_ar_channel_rsci
// ------------------------------------------------------------------


module average_core_memory_channels_ar_channel_rsci (
  clk, arst_n, memory_channels_ar_channel_rsc_dat, memory_channels_ar_channel_rsc_vld,
      memory_channels_ar_channel_rsc_rdy, memory_channels_ar_channel_rsci_oswt, memory_channels_ar_channel_rsci_wen_comp,
      memory_channels_ar_channel_rsci_irdy, memory_channels_ar_channel_rsci_irdy_oreg,
      memory_channels_ar_channel_rsci_idat, memory_channels_ar_channel_rsci_wen_comp_pff,
      memory_channels_ar_channel_rsci_oswt_pff, memory_channels_ar_channel_rsci_irdy_oreg_pff
);
  input clk;
  input arst_n;
  output [96:0] memory_channels_ar_channel_rsc_dat;
  output memory_channels_ar_channel_rsc_vld;
  input memory_channels_ar_channel_rsc_rdy;
  input memory_channels_ar_channel_rsci_oswt;
  output memory_channels_ar_channel_rsci_wen_comp;
  output memory_channels_ar_channel_rsci_irdy;
  input memory_channels_ar_channel_rsci_irdy_oreg;
  input [96:0] memory_channels_ar_channel_rsci_idat;
  output memory_channels_ar_channel_rsci_wen_comp_pff;
  input memory_channels_ar_channel_rsci_oswt_pff;
  input memory_channels_ar_channel_rsci_irdy_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_ar_channel_rsci_biwt;
  wire memory_channels_ar_channel_rsc_is_idle;
  wire memory_channels_ar_channel_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  wire [96:0] nl_memory_channels_ar_channel_rsci_idat;
  assign nl_memory_channels_ar_channel_rsci_idat = {(memory_channels_ar_channel_rsci_idat[96:29])
      , 29'b00000000010010000000000000000};
  ccs_out_buf_wait_v5 #(.rscid(32'sd10),
  .width(32'sd97),
  .ph_clk(32'sd1),
  .ph_en(32'sd0),
  .ph_arst(32'sd0),
  .ph_srst(32'sd0),
  .rst_val(32'sd0)) memory_channels_ar_channel_rsci (
      .clk(clk),
      .en(1'b0),
      .arst(arst_n),
      .srst(1'b1),
      .irdy(memory_channels_ar_channel_rsci_irdy),
      .ivld(memory_channels_ar_channel_rsci_oswt),
      .idat(nl_memory_channels_ar_channel_rsci_idat[96:0]),
      .rdy(memory_channels_ar_channel_rsc_rdy),
      .vld(memory_channels_ar_channel_rsc_vld),
      .dat(memory_channels_ar_channel_rsc_dat),
      .is_idle(memory_channels_ar_channel_rsc_is_idle)
    );
  average_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl
      average_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl_inst
      (
      .memory_channels_ar_channel_rsci_iswt0(memory_channels_ar_channel_rsci_oswt),
      .memory_channels_ar_channel_rsci_irdy_oreg(memory_channels_ar_channel_rsci_irdy_oreg),
      .memory_channels_ar_channel_rsci_biwt(memory_channels_ar_channel_rsci_biwt),
      .memory_channels_ar_channel_rsci_biwt_pff(memory_channels_ar_channel_rsci_biwt_iff),
      .memory_channels_ar_channel_rsci_iswt0_pff(memory_channels_ar_channel_rsci_oswt_pff),
      .memory_channels_ar_channel_rsci_irdy_oreg_pff(memory_channels_ar_channel_rsci_irdy_oreg_pff)
    );
  assign memory_channels_ar_channel_rsci_wen_comp = (~ memory_channels_ar_channel_rsci_oswt)
      | memory_channels_ar_channel_rsci_biwt;
  assign memory_channels_ar_channel_rsci_wen_comp_pff = (~ memory_channels_ar_channel_rsci_oswt_pff)
      | memory_channels_ar_channel_rsci_biwt_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_core
// ------------------------------------------------------------------


module average_core (
  clk, arst_n, count_rsc_dat, count_triosy_lz, index_rsc_dat, index_triosy_lz, result_rsc_dat,
      result_triosy_lz, memory_channels_ar_channel_rsc_dat, memory_channels_ar_channel_rsc_vld,
      memory_channels_ar_channel_rsc_rdy, memory_channels_r_channel_rsc_dat, memory_channels_r_channel_rsc_vld,
      memory_channels_r_channel_rsc_rdy
);
  input clk;
  input arst_n;
  input [22:0] count_rsc_dat;
  output count_triosy_lz;
  input [63:0] index_rsc_dat;
  output index_triosy_lz;
  output [22:0] result_rsc_dat;
  output result_triosy_lz;
  output [96:0] memory_channels_ar_channel_rsc_dat;
  output memory_channels_ar_channel_rsc_vld;
  input memory_channels_ar_channel_rsc_rdy;
  input [518:0] memory_channels_r_channel_rsc_dat;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;


  // Interconnect Declarations
  reg core_wen;
  wire core_wten;
  wire [22:0] count_rsci_idat;
  wire [63:0] index_rsci_idat;
  wire memory_channels_ar_channel_rsci_wen_comp;
  wire memory_channels_ar_channel_rsci_irdy;
  wire memory_channels_ar_channel_rsci_irdy_oreg;
  wire memory_channels_r_channel_rsci_wen_comp;
  wire memory_channels_r_channel_rsci_ivld;
  wire memory_channels_r_channel_rsci_ivld_oreg;
  wire [511:0] memory_channels_r_channel_rsci_idat_mxwt;
  reg [3:0] memory_channels_ar_channel_rsci_idat_96_93;
  reg [63:0] memory_channels_ar_channel_rsci_idat_92_29;
  wire [64:0] nl_memory_channels_ar_channel_rsci_idat_92_29;
  reg [17:0] result_rsci_idat_17_0;
  wire [4:0] fsm_output;
  wire or_tmp_2;
  reg exit_for_sva;
  wire xor_cse;
  wire memory_send_ar_and_cse;
  reg reg_count_triosy_obj_iswt0_cse;
  wire count_and_cse;
  wire core_wen_rtff;
  reg reg_memory_channels_ar_channel_rsci_oswt_tmp;
  reg reg_memory_channels_r_channel_rsci_oswt_tmp;
  wire memory_channels_ar_channel_rsci_wen_comp_iff;
  wire memory_send_ar_mux_rmff;
  wire memory_channels_r_channel_rsci_wen_comp_iff;
  wire memory_get_r_mux_rmff;
  wire [22:0] memory_axi_read_base_axi_32_32_rshift_itm;
  reg [3:0] memory_axi_read_base_axi_32_32_id_lpi_2;
  wire [4:0] nl_memory_axi_read_base_axi_32_32_id_lpi_2;
  reg [22:0] count_sva;
  reg [63:0] index_sva;
  reg [22:0] sum_sva;
  reg [31:0] for_i_sva;
  wire [22:0] sum_sva_mx0w1;
  wire [23:0] nl_sum_sva_mx0w1;
  wire [31:0] for_i_sva_2;
  wire [32:0] nl_for_i_sva_2;


  // Interconnect Declarations for Component Instantiations 
  wire [22:0] nl_result_rsci_idat;
  assign nl_result_rsci_idat = {{5{result_rsci_idat_17_0[17]}}, result_rsci_idat_17_0};
  wire [8:0] nl_memory_axi_read_base_axi_32_32_rshift_rg_s;
  assign nl_memory_axi_read_base_axi_32_32_rshift_rg_s = {(memory_channels_ar_channel_rsci_idat_92_29[5:0])
      , 3'b000};
  wire [96:0] nl_average_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat;
  assign nl_average_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat
      = {memory_channels_ar_channel_rsci_idat_96_93 , memory_channels_ar_channel_rsci_idat_92_29
      , 29'b00000000010010000000000000000};
  wire  nl_average_core_core_fsm_inst_main_C_0_tr0;
  assign nl_average_core_core_fsm_inst_main_C_0_tr0 = ~ xor_cse;
  ccs_in_v1 #(.rscid(32'sd4),
  .width(32'sd23)) count_rsci (
      .dat(count_rsc_dat),
      .idat(count_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd5),
  .width(32'sd64)) index_rsci (
      .dat(index_rsc_dat),
      .idat(index_rsci_idat)
    );
  ccs_out_v1 #(.rscid(32'sd6),
  .width(32'sd23)) result_rsci (
      .idat(nl_result_rsci_idat[22:0]),
      .dat(result_rsc_dat)
    );
  mgc_shift_r_v5 #(.width_a(32'sd512),
  .signd_a(32'sd0),
  .width_s(32'sd9),
  .width_z(32'sd23)) memory_axi_read_base_axi_32_32_rshift_rg (
      .a(memory_channels_r_channel_rsci_idat_mxwt),
      .s(nl_memory_axi_read_base_axi_32_32_rshift_rg_s[8:0]),
      .z(memory_axi_read_base_axi_32_32_rshift_itm)
    );
  average_core_memory_channels_ar_channel_rsci average_core_memory_channels_ar_channel_rsci_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_ar_channel_rsc_dat(memory_channels_ar_channel_rsc_dat),
      .memory_channels_ar_channel_rsc_vld(memory_channels_ar_channel_rsc_vld),
      .memory_channels_ar_channel_rsc_rdy(memory_channels_ar_channel_rsc_rdy),
      .memory_channels_ar_channel_rsci_oswt(reg_memory_channels_ar_channel_rsci_oswt_tmp),
      .memory_channels_ar_channel_rsci_wen_comp(memory_channels_ar_channel_rsci_wen_comp),
      .memory_channels_ar_channel_rsci_irdy(memory_channels_ar_channel_rsci_irdy),
      .memory_channels_ar_channel_rsci_irdy_oreg(memory_channels_ar_channel_rsci_irdy_oreg),
      .memory_channels_ar_channel_rsci_idat(nl_average_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat[96:0]),
      .memory_channels_ar_channel_rsci_wen_comp_pff(memory_channels_ar_channel_rsci_wen_comp_iff),
      .memory_channels_ar_channel_rsci_oswt_pff(memory_send_ar_mux_rmff),
      .memory_channels_ar_channel_rsci_irdy_oreg_pff(memory_channels_ar_channel_rsci_irdy)
    );
  average_core_wait_dp average_core_wait_dp_inst (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_ar_channel_rsci_irdy(memory_channels_ar_channel_rsci_irdy),
      .memory_channels_ar_channel_rsci_irdy_oreg(memory_channels_ar_channel_rsci_irdy_oreg),
      .memory_channels_r_channel_rsci_ivld(memory_channels_r_channel_rsci_ivld),
      .memory_channels_r_channel_rsci_ivld_oreg(memory_channels_r_channel_rsci_ivld_oreg)
    );
  average_core_memory_channels_r_channel_rsci average_core_memory_channels_r_channel_rsci_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_r_channel_rsc_dat(memory_channels_r_channel_rsc_dat),
      .memory_channels_r_channel_rsc_vld(memory_channels_r_channel_rsc_vld),
      .memory_channels_r_channel_rsc_rdy(memory_channels_r_channel_rsc_rdy),
      .memory_channels_r_channel_rsci_oswt(reg_memory_channels_r_channel_rsci_oswt_tmp),
      .memory_channels_r_channel_rsci_wen_comp(memory_channels_r_channel_rsci_wen_comp),
      .memory_channels_r_channel_rsci_ivld(memory_channels_r_channel_rsci_ivld),
      .memory_channels_r_channel_rsci_ivld_oreg(memory_channels_r_channel_rsci_ivld_oreg),
      .memory_channels_r_channel_rsci_idat_mxwt(memory_channels_r_channel_rsci_idat_mxwt),
      .memory_channels_r_channel_rsci_wen_comp_pff(memory_channels_r_channel_rsci_wen_comp_iff),
      .memory_channels_r_channel_rsci_oswt_pff(memory_get_r_mux_rmff),
      .memory_channels_r_channel_rsci_ivld_oreg_pff(memory_channels_r_channel_rsci_ivld)
    );
  average_core_count_triosy_obj average_core_count_triosy_obj_inst (
      .count_triosy_lz(count_triosy_lz),
      .core_wten(core_wten),
      .count_triosy_obj_iswt0(reg_count_triosy_obj_iswt0_cse)
    );
  average_core_index_triosy_obj average_core_index_triosy_obj_inst (
      .index_triosy_lz(index_triosy_lz),
      .core_wten(core_wten),
      .index_triosy_obj_iswt0(reg_count_triosy_obj_iswt0_cse)
    );
  average_core_result_triosy_obj average_core_result_triosy_obj_inst (
      .result_triosy_lz(result_triosy_lz),
      .core_wten(core_wten),
      .result_triosy_obj_iswt0(reg_count_triosy_obj_iswt0_cse)
    );
  average_core_staller average_core_staller_inst (
      .clk(clk),
      .arst_n(arst_n),
      .core_wen(core_wen_rtff),
      .core_wten(core_wten),
      .memory_channels_ar_channel_rsci_wen_comp(memory_channels_ar_channel_rsci_wen_comp),
      .memory_channels_r_channel_rsci_wen_comp(memory_channels_r_channel_rsci_wen_comp),
      .memory_channels_ar_channel_rsci_wen_comp_pff(memory_channels_ar_channel_rsci_wen_comp_iff),
      .memory_channels_r_channel_rsci_wen_comp_pff(memory_channels_r_channel_rsci_wen_comp_iff)
    );
  average_core_core_fsm average_core_core_fsm_inst (
      .clk(clk),
      .arst_n(arst_n),
      .core_wen(core_wen),
      .fsm_output(fsm_output),
      .main_C_0_tr0(nl_average_core_core_fsm_inst_main_C_0_tr0),
      .for_C_2_tr0(exit_for_sva)
    );
  assign memory_send_ar_and_cse = core_wen & (fsm_output[1]);
  assign memory_send_ar_mux_rmff = MUX_s_1_2_2(reg_memory_channels_ar_channel_rsci_oswt_tmp,
      (fsm_output[1]), core_wen);
  assign memory_get_r_mux_rmff = MUX_s_1_2_2(reg_memory_channels_r_channel_rsci_oswt_tmp,
      (fsm_output[2]), core_wen);
  assign count_and_cse = core_wen & (fsm_output[0]);
  assign nl_sum_sva_mx0w1 = memory_axi_read_base_axi_32_32_rshift_itm + sum_sva;
  assign sum_sva_mx0w1 = nl_sum_sva_mx0w1[22:0];
  assign nl_for_i_sva_2 = for_i_sva + 32'b00000000000000000000000000000001;
  assign for_i_sva_2 = nl_for_i_sva_2[31:0];
  assign or_tmp_2 = ((~ exit_for_sva) & (fsm_output[3])) | (~((fsm_output[3]) | (fsm_output[0])))
      | (xor_cse & (fsm_output[0]));
  assign xor_cse = $signed(23'b00000000000000000000000) < $signed((count_rsci_idat));
  always @(posedge clk) begin
    if ( memory_send_ar_and_cse ) begin
      memory_channels_ar_channel_rsci_idat_92_29 <= nl_memory_channels_ar_channel_rsci_idat_92_29[63:0];
      memory_channels_ar_channel_rsci_idat_96_93 <= memory_axi_read_base_axi_32_32_id_lpi_2;
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (~ or_tmp_2) ) begin
      result_rsci_idat_17_0 <= MUX_v_18_2_2(18'b000000000000000000, (sum_sva_mx0w1[22:5]),
          (fsm_output[3]));
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_memory_channels_ar_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_r_channel_rsci_oswt_tmp <= 1'b0;
      core_wen <= 1'b1;
    end
    else begin
      reg_memory_channels_ar_channel_rsci_oswt_tmp <= memory_send_ar_mux_rmff;
      reg_memory_channels_r_channel_rsci_oswt_tmp <= memory_get_r_mux_rmff;
      core_wen <= core_wen_rtff;
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_count_triosy_obj_iswt0_cse <= 1'b0;
    end
    else if ( core_wen ) begin
      reg_count_triosy_obj_iswt0_cse <= ~ or_tmp_2;
    end
  end
  always @(posedge clk) begin
    if ( core_wen & ((fsm_output[0]) | (fsm_output[3])) ) begin
      sum_sva <= MUX_v_23_2_2(23'b00000000000000000000000, sum_sva_mx0w1, (fsm_output[3]));
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      exit_for_sva <= 1'b0;
      memory_axi_read_base_axi_32_32_id_lpi_2 <= 4'b0000;
    end
    else if ( memory_send_ar_and_cse ) begin
      exit_for_sva <= ~ ($signed(for_i_sva_2) < $signed(conv_s2s_23_32(count_sva)));
      memory_axi_read_base_axi_32_32_id_lpi_2 <= nl_memory_axi_read_base_axi_32_32_id_lpi_2[3:0];
    end
  end
  always @(posedge clk) begin
    if ( count_and_cse ) begin
      count_sva <= count_rsci_idat;
      index_sva <= index_rsci_idat;
    end
  end
  always @(posedge clk) begin
    if ( core_wen & ((fsm_output[1:0]!=2'b00)) ) begin
      for_i_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, for_i_sva_2,
          (fsm_output[1]));
    end
  end
  assign nl_memory_channels_ar_channel_rsci_idat_92_29  = (index_sva + conv_s2s_32_64(for_i_sva));
  assign nl_memory_axi_read_base_axi_32_32_id_lpi_2  = (memory_axi_read_base_axi_32_32_id_lpi_2
      + 4'b0001);

  function automatic  MUX_s_1_2_2;
    input  input_0;
    input  input_1;
    input  sel;
    reg  result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_s_1_2_2 = result;
  end
  endfunction


  function automatic [17:0] MUX_v_18_2_2;
    input [17:0] input_0;
    input [17:0] input_1;
    input  sel;
    reg [17:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_18_2_2 = result;
  end
  endfunction


  function automatic [22:0] MUX_v_23_2_2;
    input [22:0] input_0;
    input [22:0] input_1;
    input  sel;
    reg [22:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_23_2_2 = result;
  end
  endfunction


  function automatic [31:0] MUX_v_32_2_2;
    input [31:0] input_0;
    input [31:0] input_1;
    input  sel;
    reg [31:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_32_2_2 = result;
  end
  endfunction


  function automatic [31:0] conv_s2s_23_32 ;
    input [22:0]  vector ;
  begin
    conv_s2s_23_32 = {{9{vector[22]}}, vector};
  end
  endfunction


  function automatic [63:0] conv_s2s_32_64 ;
    input [31:0]  vector ;
  begin
    conv_s2s_32_64 = {{32{vector[31]}}, vector};
  end
  endfunction

endmodule

// ------------------------------------------------------------------
//  Design Unit:    average_struct
// ------------------------------------------------------------------


module average_struct (
  clk, arst_n, count_rsc_dat, count_triosy_lz, index_rsc_dat, index_triosy_lz, result_rsc_dat,
      result_triosy_lz, memory_channels_ar_channel_rsc_dat_id, memory_channels_ar_channel_rsc_dat_address,
      memory_channels_ar_channel_rsc_dat_len, memory_channels_ar_channel_rsc_dat_size,
      memory_channels_ar_channel_rsc_dat_burst, memory_channels_ar_channel_rsc_dat_lock,
      memory_channels_ar_channel_rsc_dat_cache, memory_channels_ar_channel_rsc_dat_prot,
      memory_channels_ar_channel_rsc_dat_region, memory_channels_ar_channel_rsc_dat_qos,
      memory_channels_ar_channel_rsc_vld, memory_channels_ar_channel_rsc_rdy, memory_channels_r_channel_rsc_dat_id,
      memory_channels_r_channel_rsc_dat_data, memory_channels_r_channel_rsc_dat_resp,
      memory_channels_r_channel_rsc_dat_last, memory_channels_r_channel_rsc_vld,
      memory_channels_r_channel_rsc_rdy
);
  input clk;
  input arst_n;
  input [22:0] count_rsc_dat;
  output count_triosy_lz;
  input [63:0] index_rsc_dat;
  output index_triosy_lz;
  output [22:0] result_rsc_dat;
  output result_triosy_lz;
  output [3:0] memory_channels_ar_channel_rsc_dat_id;
  output [63:0] memory_channels_ar_channel_rsc_dat_address;
  output [7:0] memory_channels_ar_channel_rsc_dat_len;
  output [2:0] memory_channels_ar_channel_rsc_dat_size;
  output [1:0] memory_channels_ar_channel_rsc_dat_burst;
  output memory_channels_ar_channel_rsc_dat_lock;
  output [3:0] memory_channels_ar_channel_rsc_dat_cache;
  output [2:0] memory_channels_ar_channel_rsc_dat_prot;
  output [3:0] memory_channels_ar_channel_rsc_dat_region;
  output [3:0] memory_channels_ar_channel_rsc_dat_qos;
  output memory_channels_ar_channel_rsc_vld;
  input memory_channels_ar_channel_rsc_rdy;
  input [3:0] memory_channels_r_channel_rsc_dat_id;
  input [511:0] memory_channels_r_channel_rsc_dat_data;
  input [1:0] memory_channels_r_channel_rsc_dat_resp;
  input memory_channels_r_channel_rsc_dat_last;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;


  // Interconnect Declarations
  wire [96:0] memory_channels_ar_channel_rsc_dat;


  // Interconnect Declarations for Component Instantiations 
  wire [518:0] nl_average_core_inst_memory_channels_r_channel_rsc_dat;
  assign nl_average_core_inst_memory_channels_r_channel_rsc_dat = {memory_channels_r_channel_rsc_dat_id
      , memory_channels_r_channel_rsc_dat_data , memory_channels_r_channel_rsc_dat_resp
      , memory_channels_r_channel_rsc_dat_last};
  average_core average_core_inst (
      .clk(clk),
      .arst_n(arst_n),
      .count_rsc_dat(count_rsc_dat),
      .count_triosy_lz(count_triosy_lz),
      .index_rsc_dat(index_rsc_dat),
      .index_triosy_lz(index_triosy_lz),
      .result_rsc_dat(result_rsc_dat),
      .result_triosy_lz(result_triosy_lz),
      .memory_channels_ar_channel_rsc_dat(memory_channels_ar_channel_rsc_dat),
      .memory_channels_ar_channel_rsc_vld(memory_channels_ar_channel_rsc_vld),
      .memory_channels_ar_channel_rsc_rdy(memory_channels_ar_channel_rsc_rdy),
      .memory_channels_r_channel_rsc_dat(nl_average_core_inst_memory_channels_r_channel_rsc_dat[518:0]),
      .memory_channels_r_channel_rsc_vld(memory_channels_r_channel_rsc_vld),
      .memory_channels_r_channel_rsc_rdy(memory_channels_r_channel_rsc_rdy)
    );
  assign memory_channels_ar_channel_rsc_dat_qos = memory_channels_ar_channel_rsc_dat[3:0];
  assign memory_channels_ar_channel_rsc_dat_region = memory_channels_ar_channel_rsc_dat[7:4];
  assign memory_channels_ar_channel_rsc_dat_prot = memory_channels_ar_channel_rsc_dat[10:8];
  assign memory_channels_ar_channel_rsc_dat_cache = memory_channels_ar_channel_rsc_dat[14:11];
  assign memory_channels_ar_channel_rsc_dat_lock = memory_channels_ar_channel_rsc_dat[15];
  assign memory_channels_ar_channel_rsc_dat_burst = memory_channels_ar_channel_rsc_dat[17:16];
  assign memory_channels_ar_channel_rsc_dat_size = memory_channels_ar_channel_rsc_dat[20:18];
  assign memory_channels_ar_channel_rsc_dat_len = memory_channels_ar_channel_rsc_dat[28:21];
  assign memory_channels_ar_channel_rsc_dat_address = memory_channels_ar_channel_rsc_dat[92:29];
  assign memory_channels_ar_channel_rsc_dat_id = memory_channels_ar_channel_rsc_dat[96:93];
endmodule

// ------------------------------------------------------------------
//  Design Unit:    average
// ------------------------------------------------------------------


module average (
  clk, arst_n, count_rsc_dat, count_triosy_lz, index_rsc_dat, index_triosy_lz, result_rsc_dat,
      result_triosy_lz, memory_channels_ar_channel_rsc_dat, memory_channels_ar_channel_rsc_vld,
      memory_channels_ar_channel_rsc_rdy, memory_channels_r_channel_rsc_dat, memory_channels_r_channel_rsc_vld,
      memory_channels_r_channel_rsc_rdy
);
  input clk;
  input arst_n;
  input [22:0] count_rsc_dat;
  output count_triosy_lz;
  input [63:0] index_rsc_dat;
  output index_triosy_lz;
  output [22:0] result_rsc_dat;
  output result_triosy_lz;
  output [96:0] memory_channels_ar_channel_rsc_dat;
  output memory_channels_ar_channel_rsc_vld;
  input memory_channels_ar_channel_rsc_rdy;
  input [518:0] memory_channels_r_channel_rsc_dat;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;


  // Interconnect Declarations
  wire [3:0] memory_channels_ar_channel_rsc_dat_id;
  wire [63:0] memory_channels_ar_channel_rsc_dat_address;
  wire [7:0] memory_channels_ar_channel_rsc_dat_len;
  wire [2:0] memory_channels_ar_channel_rsc_dat_size;
  wire [1:0] memory_channels_ar_channel_rsc_dat_burst;
  wire memory_channels_ar_channel_rsc_dat_lock;
  wire [3:0] memory_channels_ar_channel_rsc_dat_cache;
  wire [2:0] memory_channels_ar_channel_rsc_dat_prot;
  wire [3:0] memory_channels_ar_channel_rsc_dat_region;
  wire [3:0] memory_channels_ar_channel_rsc_dat_qos;


  // Interconnect Declarations for Component Instantiations 
  wire [3:0] nl_average_struct_inst_memory_channels_r_channel_rsc_dat_id;
  assign nl_average_struct_inst_memory_channels_r_channel_rsc_dat_id = memory_channels_r_channel_rsc_dat[518:515];
  wire [511:0] nl_average_struct_inst_memory_channels_r_channel_rsc_dat_data;
  assign nl_average_struct_inst_memory_channels_r_channel_rsc_dat_data = memory_channels_r_channel_rsc_dat[514:3];
  wire [1:0] nl_average_struct_inst_memory_channels_r_channel_rsc_dat_resp;
  assign nl_average_struct_inst_memory_channels_r_channel_rsc_dat_resp = memory_channels_r_channel_rsc_dat[2:1];
  wire  nl_average_struct_inst_memory_channels_r_channel_rsc_dat_last;
  assign nl_average_struct_inst_memory_channels_r_channel_rsc_dat_last = memory_channels_r_channel_rsc_dat[0];
  average_struct average_struct_inst (
      .clk(clk),
      .arst_n(arst_n),
      .count_rsc_dat(count_rsc_dat),
      .count_triosy_lz(count_triosy_lz),
      .index_rsc_dat(index_rsc_dat),
      .index_triosy_lz(index_triosy_lz),
      .result_rsc_dat(result_rsc_dat),
      .result_triosy_lz(result_triosy_lz),
      .memory_channels_ar_channel_rsc_dat_id(memory_channels_ar_channel_rsc_dat_id),
      .memory_channels_ar_channel_rsc_dat_address(memory_channels_ar_channel_rsc_dat_address),
      .memory_channels_ar_channel_rsc_dat_len(memory_channels_ar_channel_rsc_dat_len),
      .memory_channels_ar_channel_rsc_dat_size(memory_channels_ar_channel_rsc_dat_size),
      .memory_channels_ar_channel_rsc_dat_burst(memory_channels_ar_channel_rsc_dat_burst),
      .memory_channels_ar_channel_rsc_dat_lock(memory_channels_ar_channel_rsc_dat_lock),
      .memory_channels_ar_channel_rsc_dat_cache(memory_channels_ar_channel_rsc_dat_cache),
      .memory_channels_ar_channel_rsc_dat_prot(memory_channels_ar_channel_rsc_dat_prot),
      .memory_channels_ar_channel_rsc_dat_region(memory_channels_ar_channel_rsc_dat_region),
      .memory_channels_ar_channel_rsc_dat_qos(memory_channels_ar_channel_rsc_dat_qos),
      .memory_channels_ar_channel_rsc_vld(memory_channels_ar_channel_rsc_vld),
      .memory_channels_ar_channel_rsc_rdy(memory_channels_ar_channel_rsc_rdy),
      .memory_channels_r_channel_rsc_dat_id(nl_average_struct_inst_memory_channels_r_channel_rsc_dat_id[3:0]),
      .memory_channels_r_channel_rsc_dat_data(nl_average_struct_inst_memory_channels_r_channel_rsc_dat_data[511:0]),
      .memory_channels_r_channel_rsc_dat_resp(nl_average_struct_inst_memory_channels_r_channel_rsc_dat_resp[1:0]),
      .memory_channels_r_channel_rsc_dat_last(nl_average_struct_inst_memory_channels_r_channel_rsc_dat_last),
      .memory_channels_r_channel_rsc_vld(memory_channels_r_channel_rsc_vld),
      .memory_channels_r_channel_rsc_rdy(memory_channels_r_channel_rsc_rdy)
    );
  assign memory_channels_ar_channel_rsc_dat = {memory_channels_ar_channel_rsc_dat_id
      , memory_channels_ar_channel_rsc_dat_address , memory_channels_ar_channel_rsc_dat_len
      , memory_channels_ar_channel_rsc_dat_size , memory_channels_ar_channel_rsc_dat_burst
      , memory_channels_ar_channel_rsc_dat_lock , memory_channels_ar_channel_rsc_dat_cache
      , memory_channels_ar_channel_rsc_dat_prot , memory_channels_ar_channel_rsc_dat_region
      , memory_channels_ar_channel_rsc_dat_qos};
endmodule



