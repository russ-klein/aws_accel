
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


//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/hls_pkgs/mgc_comps_src/mgc_mul_pipe_beh.v 
//
// File:      $Mgc_home/pkgs/hls_pkgs/mgc_comps_src/mgc_mul_pipe_beh.v
//
// BASELINE:  Catapult-C version 2006b.63
// MODIFIED:  2007-04-03, tnagler
//
// Note: this file uses Verilog2001 features; 
//       please enable Verilog2001 in the flow!

module mgc_mul_pipe (a, b, clk, en, a_rst, s_rst, z);

    // Parameters:
    parameter integer width_a = 32'd4;  // input a bit width
    parameter         signd_a =  1'b1;  // input a type (1=signed, 0=unsigned)
    parameter integer width_b = 32'd4;  // input b bit width
    parameter         signd_b =  1'b1;  // input b type (1=signed, 0=unsigned)
    parameter integer width_z = 32'd8;  // result bit width (= width_a + width_b)
    parameter      clock_edge =  1'b0;  // clock polarity (1=posedge, 0=negedge)
    parameter   enable_active =  1'b0;  // enable polarity (1=posedge, 0=negedge)
    parameter    a_rst_active =  1'b1;  // unused
    parameter    s_rst_active =  1'b1;  // unused
    parameter integer  stages = 32'd2;  // number of output registers + 1 (careful!)
    parameter integer n_inreg = 32'd0;  // number of input registers
   
    localparam integer width_ab = width_a + width_b;  // multiplier result width
    localparam integer n_inreg_min = (n_inreg > 1) ? (n_inreg-1) : 0; // for Synopsys DC
   
    // I/O ports:
    input  [width_a-1:0] a;      // input A
    input  [width_b-1:0] b;      // input B
    input                clk;    // clock
    input                en;     // enable
    input                a_rst;  // spyglass disable SYNTH_5121,W240
    input                s_rst;  // spyglass disable SYNTH_5121,W240
    output [width_z-1:0] z;      // output


    // Input registers:

    wire [width_a-1:0] a_f;
    wire [width_b-1:0] b_f;

    integer i;

    generate
    if (clock_edge == 1'b0)
    begin: NEG_EDGE1
        case (n_inreg)
        32'd0: begin: B1
            assign a_f = a, 
                   b_f = b;
        end
        default: begin: B2
            reg [width_a-1:0] a_reg [n_inreg_min:0];
            reg [width_b-1:0] b_reg [n_inreg_min:0];
            always @(negedge clk)
            if (en == enable_active)
            begin: B21
                a_reg[0] <= a;
                b_reg[0] <= b;
                for (i = 0; i < n_inreg_min; i = i + 1)
                begin: B3
                    a_reg[i+1] <= a_reg[i]; //spyglass disable FlopEConst
                    b_reg[i+1] <= b_reg[i]; //spyglass disable FlopEConst
                end
            end
            assign a_f = a_reg[n_inreg_min],
                   b_f = b_reg[n_inreg_min];
        end
        endcase
    end
    else
    begin: POS_EDGE1
        case (n_inreg)
        32'd0: begin: B1
            assign a_f = a, 
                   b_f = b;
        end
        default: begin: B2
            reg [width_a-1:0] a_reg [n_inreg_min:0];
            reg [width_b-1:0] b_reg [n_inreg_min:0];
            always @(posedge clk)
            if (en == enable_active)
            begin: B21
                a_reg[0] <= a; //spyglass disable FlopEConst
                b_reg[0] <= b; //spyglass disable FlopEConst
                for (i = 0; i < n_inreg_min; i = i + 1)
                begin: B3
                    a_reg[i+1] <= a_reg[i]; //spyglass disable FlopEConst
                    b_reg[i+1] <= b_reg[i]; //spyglass disable FlopEConst
                end
            end
            assign a_f = a_reg[n_inreg_min],
                   b_f = b_reg[n_inreg_min];
        end
        endcase
    end
    endgenerate


    // Output:
    wire [width_z-1:0]  xz;

    function signed [width_z-1:0] conv_signed;
      input signed [width_ab-1:0] res;
      conv_signed = res[width_z-1:0];
    endfunction

    generate
      wire signed [width_ab-1:0] res;
      if ( (signd_a == 1'b1) && (signd_b == 1'b1) )
      begin: SIGNED_AB
              assign res = $signed(a_f) * $signed(b_f);
              assign xz = conv_signed(res);
      end
      else if ( (signd_a == 1'b1) && (signd_b == 1'b0) )
      begin: SIGNED_A
              assign res = $signed(a_f) * $signed({1'b0, b_f});
              assign xz = conv_signed(res);
      end
      else if ( (signd_a == 1'b0) && (signd_b == 1'b1) )
      begin: SIGNED_B
              assign res = $signed({1'b0,a_f}) * $signed(b_f);
              assign xz = conv_signed(res);
      end
      else
      begin: UNSIGNED_AB
              assign res = a_f * b_f;
	      assign xz = res[width_z-1:0];
      end
    endgenerate


    // Output registers:

    reg  [width_z-1:0] reg_array[stages-2:0];
    wire [width_z-1:0] z;

    generate
    if (clock_edge == 1'b0)
    begin: NEG_EDGE2
        always @(negedge clk)
        if (en == enable_active)
            for (i = stages-2; i >= 0; i = i-1)
                if (i == 0)
                    reg_array[i] <= xz; //spyglass disable FlopEConst
                else
                    reg_array[i] <= reg_array[i-1]; //spyglass disable FlopEConst
    end
    else
    begin: POS_EDGE2
        always @(posedge clk)
        if (en == enable_active)
            for (i = stages-2; i >= 0; i = i-1)
                if (i == 0)
                    reg_array[i] <= xz; //spyglass disable FlopEConst
                else
                    reg_array[i] <= reg_array[i-1]; //spyglass disable FlopEConst
    end
    endgenerate

    assign z = reg_array[stages-2];
endmodule

//------> ./rtl.v 
// ----------------------------------------------------------------------
//  HLS HDL:        Verilog Netlister
//  HLS Version:    2025.2/1190995 Production Release
//  HLS Date:       Wed May 14 16:03:56 PDT 2025
// 
//  Generated by:   russk@orw-russk-vm
//  Generated date: Mon Aug 11 17:12:42 2025
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
  output [3:0] fsm_output;
  reg [3:0] fsm_output;


  // FSM State Type Declaration for mac_core_core_fsm_1
  parameter
    main_C_0 = 2'd0,
    main_C_1 = 2'd1,
    main_C_2 = 2'd2,
    main_C_3 = 2'd3;

  reg [1:0] state_var;
  reg [1:0] state_var_NS;


  // Interconnect Declarations for Component Instantiations 
  always @(*)
  begin : mac_core_core_fsm_1
    case (state_var)
      main_C_1 : begin
        fsm_output = 4'b0010;
        state_var_NS = main_C_2;
      end
      main_C_2 : begin
        fsm_output = 4'b0100;
        state_var_NS = main_C_3;
      end
      main_C_3 : begin
        fsm_output = 4'b1000;
        state_var_NS = main_C_0;
      end
      // main_C_0
      default : begin
        fsm_output = 4'b0001;
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
  input [31:0] f1_rsc_dat;
  output f1_triosy_lz;
  input [31:0] f2_rsc_dat;
  output f2_triosy_lz;
  input [31:0] a1_rsc_dat;
  output a1_triosy_lz;
  output [31:0] result_rsc_dat;
  output result_triosy_lz;


  // Interconnect Declarations
  wire [31:0] f1_rsci_idat;
  wire [31:0] f2_rsci_idat;
  wire [31:0] a1_rsci_idat;
  reg result_triosy_obj_ld;
  wire [23:0] mul_cmp_z;
  reg [24:0] result_rsci_idat_24_0;
  wire [25:0] nl_result_rsci_idat_24_0;
  wire [3:0] fsm_output;
  reg reg_f1_triosy_obj_ld_cse;
  reg [24:0] a1_slc_24_0_itm;


  // Interconnect Declarations for Component Instantiations 
  wire [31:0] nl_result_rsci_idat;
  assign nl_result_rsci_idat = {{7{result_rsci_idat_24_0[24]}}, result_rsci_idat_24_0};
  wire [23:0] nl_mul_cmp_a;
  assign nl_mul_cmp_a = f1_rsci_idat[23:0];
  wire [23:0] nl_mul_cmp_b;
  assign nl_mul_cmp_b = f2_rsci_idat[23:0];
  ccs_in_v1 #(.rscid(32'sd5),
  .width(32'sd32)) f1_rsci (
      .dat(f1_rsc_dat),
      .idat(f1_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd6),
  .width(32'sd32)) f2_rsci (
      .dat(f2_rsc_dat),
      .idat(f2_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd7),
  .width(32'sd32)) a1_rsci (
      .dat(a1_rsc_dat),
      .idat(a1_rsci_idat)
    );
  ccs_out_v1 #(.rscid(32'sd8),
  .width(32'sd32)) result_rsci (
      .idat(nl_result_rsci_idat[31:0]),
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
      .ld(result_triosy_obj_ld),
      .lz(result_triosy_lz)
    );
  mgc_mul_pipe #(.width_a(32'sd24),
  .signd_a(32'sd0),
  .width_b(32'sd24),
  .signd_b(32'sd0),
  .width_z(32'sd24),
  .clock_edge(32'sd1),
  .enable_active(32'sd1),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) mul_cmp (
      .a(nl_mul_cmp_a[23:0]),
      .b(nl_mul_cmp_b[23:0]),
      .clk(clk),
      .en(1'b1),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(mul_cmp_z)
    );
  mac_core_core_fsm mac_core_core_fsm_inst (
      .clk(clk),
      .arst_n(arst_n),
      .fsm_output(fsm_output)
    );
  always @(posedge clk) begin
    if ( fsm_output[0] ) begin
      a1_slc_24_0_itm <= a1_rsci_idat[24:0];
    end
  end
  always @(posedge clk) begin
    if ( fsm_output[2] ) begin
      result_rsci_idat_24_0 <= nl_result_rsci_idat_24_0[24:0];
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_f1_triosy_obj_ld_cse <= 1'b0;
      result_triosy_obj_ld <= 1'b0;
    end
    else begin
      reg_f1_triosy_obj_ld_cse <= fsm_output[0];
      result_triosy_obj_ld <= fsm_output[2];
    end
  end
  assign nl_result_rsci_idat_24_0  = (conv_s2s_24_25(mul_cmp_z) + a1_slc_24_0_itm);

  function automatic [24:0] conv_s2s_24_25 ;
    input [23:0]  vector ;
  begin
    conv_s2s_24_25 = {vector[23], vector};
  end
  endfunction

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
  input [31:0] f1_rsc_dat;
  output f1_triosy_lz;
  input [31:0] f2_rsc_dat;
  output f2_triosy_lz;
  input [31:0] a1_rsc_dat;
  output a1_triosy_lz;
  output [31:0] result_rsc_dat;
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



