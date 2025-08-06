
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


//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/hls_pkgs/mgc_comps_src/mgc_muladd1_beh.v 
//muladd1
module mgc_muladd1(a,b,c,cst,d,z);
  // operation is z = a * (b + d) + c + cst
  parameter width_a = 0;
  parameter signd_a = 0;
  parameter width_b = 0;
  parameter signd_b = 0;
  parameter width_c = 0;
  parameter signd_c = 0;
  parameter width_cst = 0;
  parameter signd_cst = 0;
  parameter width_d = 0;
  parameter signd_d = 0;
  parameter width_z = 0;
  parameter add_axb = 1;
  parameter add_c = 1;
  parameter add_d = 1;
  parameter use_keep_d  = 1;
  parameter use_const = 1;

  //pragma coverage off
  function integer is_square_op;
    input integer alen;
  begin
    if (alen > 1) is_square_op = 0;
    else       is_square_op = 1;
  end endfunction
  //pragma coverage on
  
  input  [width_a-1:0] a;
  input  [width_b-1:0] b;
  input  [width_c-1:0] c;
  input  [width_cst-1:0] cst; // spyglass disable SYNTH_5121,W240
  input  [width_d-1:0] d;
  output [width_z-1:0] z;

  reg [width_a-signd_a:0] aa;
  reg [width_b-signd_b:0] bb;
  reg [width_c-signd_c:0] cc;
  reg [width_d-signd_d:0] dd;
  reg [width_cst-signd_cst:0] cstin;
  
  localparam width_bd = (width_d) ? 1+ ((width_b-signd_b>width_d-signd_d) ? width_b - signd_b
                                                                          : width_d - signd_d)
                                  : width_b - signd_b;
  localparam is_square = is_square_op(width_a);
  localparam axb_len = (is_square)?width_bd+1+width_bd+1:width_a-signd_a+1+width_bd+1;

  reg [width_bd:0] bd;
  reg [axb_len-1:0] axb;

  

  // make all inputs signed
  always @(*) aa = signd_a ? a : {1'b0, a};
  always @(*) bb = signd_b ? b : {1'b0, b};
  generate if (width_c != 0) begin
    always @(*) cc = signd_c ? c : {1'b0, c};
  end endgenerate

  generate if (width_d) begin
    if ( !is_square ) begin
      if ( use_keep_d == 1) begin
        (* keep ="true" *) reg [width_d-signd_d:0] d_keep;
        always @(*) d_keep = signd_d ? d : {1'b0, d};
        always @(*) dd = d_keep;
      end else begin
        reg [width_d-signd_d:0] d_keep;
        always @(*) d_keep = signd_d ? d : {1'b0, d};
        always @(*) dd = d_keep;
      end
    end else begin
      always @(*) dd = signd_d ? d : {1'b0, d};
    end
  end endgenerate

  always @(*) cstin = signd_cst ? cst : {1'b0, cst};

  // perform pre-adder
  generate
    if (width_d != 0) begin
      if (add_d) begin always @(*)  bd = $signed(bb) + $signed(dd); end
      else       begin always @(*)  bd = $signed(bb) - $signed(dd); end
    end else     begin always @(*)  bd = $signed(bb); end
  endgenerate

  generate
    if (is_square) 
      always @(*) axb = $signed(bd) * $signed(bd);
    else
      always @(*) axb = $signed(aa) * $signed(bd);
  endgenerate
  
  // perform muladd1
  wire [width_z-1:0]  zz;
  
  generate
    if (use_const) begin
      if ( add_axb &&  add_c && width_c) begin assign zz = $signed(axb) + $signed(cc) + $signed(cstin); end else
      if ( add_axb && !add_c && width_c) begin assign zz = $signed(axb) - $signed(cc) + $signed(cstin); end else
      if (!add_axb &&  add_c && width_c) begin assign zz = $signed(cc) - $signed(axb) + $signed(cstin); end else
      if (!add_axb && !add_c && width_c) begin assign zz = $signed(cstin) - $signed(axb) - $signed(cc); end else
      if ( add_axb )                     begin assign zz = $signed(axb) + $signed(cstin); end else
                                         begin assign zz = $signed(cstin) - $signed(axb); end
    end  else begin
      if ( add_axb &&  add_c && width_c) begin assign zz = $signed(axb) + $signed(cc); end else
      if ( add_axb && !add_c && width_c) begin assign zz = $signed(axb) - $signed(cc); end else
      if (!add_axb &&  add_c && width_c) begin assign zz = $signed(cc) - $signed(axb); end else
      if (!add_axb && !add_c && width_c) begin assign zz = -$signed(axb) - $signed(cc); end else
      if ( add_axb )                     begin assign zz = $signed(axb); end else
                                         begin assign zz = -$signed(axb); end
    end
  endgenerate
  
  // adjust output
  assign z = zz;
endmodule // mgc_muladd1

//------> ./rtl.v 
// ----------------------------------------------------------------------
//  HLS HDL:        Verilog Netlister
//  HLS Version:    2025.2/1190995 Production Release
//  HLS Date:       Wed May 14 16:03:56 PDT 2025
// 
//  Generated by:   russk@orw-russk-rk
//  Generated date: Wed Aug  6 13:01:18 2025
// ----------------------------------------------------------------------

// 
// ------------------------------------------------------------------
//  Design Unit:    mac_core_core_fsm
//  FSM Module
// ------------------------------------------------------------------


module mac_core_core_fsm (
  clk, arst_n, fsm_output
);
  input clk;
  input arst_n;
  output [1:0] fsm_output;
  reg [1:0] fsm_output;


  // FSM State Type Declaration for mac_core_core_fsm_1
  parameter
    main_C_0 = 1'd0,
    main_C_1 = 1'd1;

  reg  state_var;
  reg  state_var_NS;


  // Interconnect Declarations for Component Instantiations 
  always @(*)
  begin : mac_core_core_fsm_1
    case (state_var)
      main_C_1 : begin
        fsm_output = 2'b10;
        state_var_NS = main_C_0;
      end
      // main_C_0
      default : begin
        fsm_output = 2'b01;
        state_var_NS = main_C_1;
      end
    endcase
  end

  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      state_var <= main_C_0;
    end
    else begin
      state_var <= state_var_NS;
    end
  end

endmodule

// ------------------------------------------------------------------
//  Design Unit:    mac_core
// ------------------------------------------------------------------


module mac_core (
  clk, arst_n, f1_rsc_dat, f1_triosy_lz, f2_rsc_dat, f2_triosy_lz, a1_rsc_dat, a1_triosy_lz,
      result_rsc_dat, result_triosy_lz
);
  input clk;
  input arst_n;
  input [11:0] f1_rsc_dat;
  output f1_triosy_lz;
  input [11:0] f2_rsc_dat;
  output f2_triosy_lz;
  input [23:0] a1_rsc_dat;
  output a1_triosy_lz;
  output [24:0] result_rsc_dat;
  output result_triosy_lz;


  // Interconnect Declarations
  wire [11:0] f1_rsci_idat;
  wire [11:0] f2_rsci_idat;
  wire [23:0] a1_rsci_idat;
  reg [24:0] result_rsci_idat;
  wire [24:0] acc_2_cmp_z;
  wire [1:0] fsm_output;
  reg reg_f1_triosy_obj_ld_cse;


  // Interconnect Declarations for Component Instantiations 
  ccs_in_v1 #(.rscid(32'sd5),
  .width(32'sd12)) f1_rsci (
      .dat(f1_rsc_dat),
      .idat(f1_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd6),
  .width(32'sd12)) f2_rsci (
      .dat(f2_rsc_dat),
      .idat(f2_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd7),
  .width(32'sd24)) a1_rsci (
      .dat(a1_rsc_dat),
      .idat(a1_rsci_idat)
    );
  ccs_out_v1 #(.rscid(32'sd8),
  .width(32'sd25)) result_rsci (
      .idat(result_rsci_idat),
      .dat(result_rsc_dat)
    );
  mgc_io_sync_v2 #(.valid(32'sd0)) f1_triosy_obj (
      .ld(reg_f1_triosy_obj_ld_cse),
      .lz(f1_triosy_lz)
    );
  mgc_io_sync_v2 #(.valid(32'sd0)) f2_triosy_obj (
      .ld(reg_f1_triosy_obj_ld_cse),
      .lz(f2_triosy_lz)
    );
  mgc_io_sync_v2 #(.valid(32'sd0)) a1_triosy_obj (
      .ld(reg_f1_triosy_obj_ld_cse),
      .lz(a1_triosy_lz)
    );
  mgc_io_sync_v2 #(.valid(32'sd0)) result_triosy_obj (
      .ld(reg_f1_triosy_obj_ld_cse),
      .lz(result_triosy_lz)
    );
  mgc_muladd1 #(.width_a(32'sd12),
  .signd_a(32'sd1),
  .width_b(32'sd12),
  .signd_b(32'sd1),
  .width_c(32'sd24),
  .signd_c(32'sd1),
  .width_cst(32'sd1),
  .signd_cst(32'sd0),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd25),
  .add_axb(32'sd1),
  .add_c(32'sd1),
  .add_d(32'sd1),
  .use_keep_d(32'sd1),
  .use_const(32'sd1)) acc_2_cmp (
      .a(f1_rsci_idat),
      .b(f2_rsci_idat),
      .c(a1_rsci_idat),
      .cst(1'b0),
      .z(acc_2_cmp_z),
      .d(2'b0)
    );
  mac_core_core_fsm mac_core_core_fsm_inst (
      .clk(clk),
      .arst_n(arst_n),
      .fsm_output(fsm_output)
    );
  always @(posedge clk) begin
    if ( ~ (fsm_output[1]) ) begin
      result_rsci_idat <= acc_2_cmp_z;
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_f1_triosy_obj_ld_cse <= 1'b0;
    end
    else begin
      reg_f1_triosy_obj_ld_cse <= ~ (fsm_output[1]);
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    mac
// ------------------------------------------------------------------


module mac (
  clk, arst_n, f1_rsc_dat, f1_triosy_lz, f2_rsc_dat, f2_triosy_lz, a1_rsc_dat, a1_triosy_lz,
      result_rsc_dat, result_triosy_lz
);
  input clk;
  input arst_n;
  input [11:0] f1_rsc_dat;
  output f1_triosy_lz;
  input [11:0] f2_rsc_dat;
  output f2_triosy_lz;
  input [23:0] a1_rsc_dat;
  output a1_triosy_lz;
  output [24:0] result_rsc_dat;
  output result_triosy_lz;



  // Interconnect Declarations for Component Instantiations 
  mac_core mac_core_inst (
      .clk(clk),
      .arst_n(arst_n),
      .f1_rsc_dat(f1_rsc_dat),
      .f1_triosy_lz(f1_triosy_lz),
      .f2_rsc_dat(f2_rsc_dat),
      .f2_triosy_lz(f2_triosy_lz),
      .a1_rsc_dat(a1_rsc_dat),
      .a1_triosy_lz(a1_triosy_lz),
      .result_rsc_dat(result_rsc_dat),
      .result_triosy_lz(result_triosy_lz)
    );
endmodule



