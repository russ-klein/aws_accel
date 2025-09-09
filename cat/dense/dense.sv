
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




//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/siflibs/mgc_shift_l_beh_v5.v 
module mgc_shift_l_v5(a,s,z);
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
      assign z = fshl_u(a,s,a[width_a-1]);
   end
   else
   begin: UNSGNED
      assign z = fshl_u(a,s,1'b0);
   end
   endgenerate

   //Shift-left - unsigned shift argument one bit more
   function [width_z-1:0] fshl_u_1;
      input [width_a  :0] arg1;
      input [width_s-1:0] arg2;
      input sbit;
      parameter olen = width_z;
      parameter ilen = width_a+1;
      parameter len = (ilen >= olen) ? ilen : olen;
      reg [len-1:0] result;
      reg [len-1:0] result_t;
      begin
        result_t = {(len){sbit}};
        result_t[ilen-1:0] = arg1;
        result = result_t <<< arg2;
        fshl_u_1 =  result[olen-1:0];
      end
   endfunction // fshl_u

   //Shift-left - unsigned shift argument
   function [width_z-1:0] fshl_u;
      input [width_a-1:0] arg1;
      input [width_s-1:0] arg2;
      input sbit;
      fshl_u = fshl_u_1({sbit,arg1} ,arg2, sbit);
   endfunction // fshl_u

endmodule

//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/hls_pkgs/mgc_comps_src/mgc_mulacc_pipe_beh.v 
//mulacc
module mgc_mulacc_pipe(a,b,c,d,load,datavalid,clk,en,a_rst,s_rst,z);

  parameter width_a = 0;
  parameter signd_a = 0;
  parameter width_b = 0;
  parameter signd_b = 0;
  parameter width_c = 0;
  parameter signd_c = 0;
  parameter width_d = 0;
  parameter signd_d = 0;
  parameter width_z = 0;
  parameter add_d = 1;
  parameter is_square = 0;
  parameter      clock_edge =  1'b0;  // clock polarity (1=posedge, 0=negedge)
  parameter   enable_active =  1'b0;  // enable polarity (1=posedge, 0=negedge)
  parameter    a_rst_active =  1'b1;  // unused
  parameter    s_rst_active =  1'b1;  // unused
  parameter integer  stages = 32'd2;  // number of output registers + 1 (careful!)
  parameter integer n_inreg = 32'd0;  // number of input registers

  //pragma coverage off
  function integer max_len;
    input integer a, b;
  begin
    if (a > b) max_len = a;
    else       max_len = b;
  end endfunction

  function integer min_len;
    input integer a, b;
  begin
    if (a > b) min_len = b;
    else       min_len = a;
  end endfunction
  //pragma coverage on
  
  localparam axb_stages = (stages>2) ? 1 : 0;
  
  localparam preadd_stages = (n_inreg>1) ? 1 : 0;
  localparam bb_stages = n_inreg - preadd_stages;
  localparam cc_stages = n_inreg + axb_stages;
  localparam cc_len = min_len(width_c-signd_c+1, width_z);
  
  localparam zz_stages = stages - axb_stages;
  
  localparam width_bd = (width_d>0) ? (1+ ((width_b-signd_b>width_d-signd_d) ? width_b - signd_b : width_d - signd_d)) : width_b - signd_b;
  localparam axb_len = (is_square)?width_bd+1+width_bd+1:width_a-signd_a+1+width_bd+1;
  
  localparam zz_len = max_len(axb_len, max_len(cc_len, width_z));
  
  reg [width_bd:0] bd [preadd_stages:0];

  input  [width_a-1:0] a;
  input  [width_b-1:0] b;
  input  [width_c-1:0] c;
  input  [width_d-1:0] d; // spyglass disable SYNTH_5121,W240
  input                load;
  input                datavalid;

  input                clk;    // clock
  input                en;     // enable
  input                a_rst;  // spyglass disable SYNTH_5121,W240
  input                s_rst;  // spyglass disable SYNTH_5121,W240

  output [width_z-1:0] z;

  reg [width_a-signd_a:0] aa [n_inreg:0];
  reg [width_b-signd_b:0] bb [n_inreg:0];
  reg [width_c-signd_c:0] cc [cc_stages:0];
  reg [width_d-signd_d:0] dd [bb_stages:0];
  reg                     accum [cc_stages:0];
  reg                     vl [cc_stages:0];

  genvar i;

  // make all inputs signed
  always @(*) aa[n_inreg]   = signd_a ? a : {1'b0, a};
  always @(*) bb[bb_stages]   = signd_b ? b : {1'b0, b};
  generate if (width_d>0) begin
    always @(*) dd[bb_stages]   = signd_d ? d : {1'b0, d};
  end endgenerate
  always @(*) cc[cc_stages] = (signd_c | width_c >= width_z) ? c : {1'b0, c};
  always @(*) accum[cc_stages] = !load;
  always @(*) vl[cc_stages] = datavalid;

  // input registers
  generate if (n_inreg>0) begin
  for(i = n_inreg-1; i >= 0; i=i-1) begin:ab_pipe
    if (clock_edge == 1'b1) begin:pos
      always @(posedge(clk)) if (en == enable_active) aa[i] <= aa[i+1];//spyglass disable FlopEConst
    end else begin:neg
      always @(negedge(clk)) if (en == enable_active) aa[i] <= aa[i+1];//spyglass disable FlopEConst
    end
  end end endgenerate
  generate if (bb_stages>0) begin
  for(i = bb_stages-1; i >= 0; i=i-1) begin:in_pipe_bd
    if (clock_edge == 1'b1) begin:pos
      always @(posedge(clk)) if (en == enable_active) bb[i] <= bb[i+1];
      if (width_d>0) begin  always @(posedge(clk)) if (en == enable_active) dd[i] <= dd[i+1]; end //spyglass disable FlopEConst
    end else begin:neg
      always @(negedge(clk)) if (en == enable_active) bb[i] <= bb[i+1];
      if (width_d>0) begin  always @(negedge(clk)) if (en == enable_active) dd[i] <= dd[i+1]; end //spyglass disable FlopEConst
    end
  end end endgenerate
  generate if (cc_stages>0) begin
  for(i = cc_stages-1; i >= 0; i=i-1) begin:c_pipe
    if (clock_edge == 1'b1) begin:pos
      always @(posedge(clk)) if (en == enable_active) cc[i] <= cc[i+1];//spyglass disable FlopEConst
      always @(posedge(clk)) if (en == enable_active) accum[i] <= accum[i+1];//spyglass disable FlopEConst
      always @(posedge(clk)) if (en == enable_active) vl[i] <= vl[i+1];//spyglass disable FlopEConst
    end else begin:neg
      always @(negedge(clk)) if (en == enable_active) cc[i] <= cc[i+1];//spyglass disable FlopEConst
      always @(negedge(clk)) if (en == enable_active) accum[i] <= accum[i+1];//spyglass disable FlopEConst
      always @(negedge(clk)) if (en == enable_active) vl[i] <= vl[i+1];
    end
  end end endgenerate
  
  // perform pre-adder
  generate
    if (width_d>0) begin
      if (add_d != 0) begin always @(*)  bd[preadd_stages] = $signed(bb[0]) + $signed(dd[0]); end
      else            begin always @(*)  bd[preadd_stages] = $signed(bb[0]) - $signed(dd[0]); end
    end else          begin always @(*)  bd[preadd_stages] = $signed(bb[0]); end
  endgenerate
  generate if (preadd_stages>0) begin
  for(i = preadd_stages-1; i >= 0; i=i-1) begin:preadd_pipe
    if (clock_edge == 1'b1) begin:pos
      always @(posedge(clk)) if (en == enable_active) bd[i] <= bd[i+1];//spyglass disable FlopEConst
    end else begin:neg
      always @(negedge(clk)) if (en == enable_active) bd[i] <= bd[i+1];//spyglass disable FlopEConst
    end
  end end endgenerate

  // perform muladd1
  reg [zz_len-1:0]  zz[zz_stages-1:0];
  wire [zz_len-1:0] z_or_c;
  reg [axb_len-1:0] axb[axb_stages:0];
  generate
    if (is_square>0) 
      always @(*) axb[axb_stages] = $signed(bd[0]) * $signed(bd[0]);
    else
      always @(*) axb[axb_stages] = $signed(aa[0]) * $signed(bd[0]);
  endgenerate
  
  generate if (axb_stages>0) begin 
  for(i = axb_stages-1; i >= 0; i=i-1) begin:axb_pipe
    if (clock_edge == 1'b1) begin:pos
      always @(posedge(clk)) if (en == enable_active) axb[i] <= axb[i+1];//spyglass disable FlopEConst
    end else begin:neg
      always @(negedge(clk)) if (en == enable_active) axb[i] <= axb[i+1];//spyglass disable FlopEConst
    end
  end end endgenerate

  assign z_or_c = accum[0] ? $signed(zz[zz_stages-2]): $signed(cc[0]);
  always @(*) zz[zz_stages-1] = $signed(axb[0]) + $signed(z_or_c);

  // Output registers:
  generate for(i = zz_stages-2; i >= 0; i=i-1) begin:out_pipe
    if (clock_edge == 1'b1) begin:pos
      always @(posedge(clk)) if (en == enable_active && (vl[0] || i != zz_stages-2)) zz[i] <= zz[i+1];//spyglass disable FlopEConst
    end else begin:neg
      always @(negedge(clk)) if (en == enable_active && (vl[0] || i != zz_stages-2)) zz[i] <= zz[i+1];//spyglass disable FlopEConst
    end
  end endgenerate

  // adjust output
  // use a tmp var to satisfy W164a lint violations
  wire [width_z-1:0] z_out_tmp;
  assign z_out_tmp = zz[0][width_z-1:0];
  assign z = z_out_tmp;

endmodule // mgc_mulacc_pipe

//------> /wv/hlsb/CATAPULT/2025.2/2025-05-14/aol/Mgc_home/pkgs/ccs_xilinx/hdl/BLOCK_1R1W_RBW.v 
// Memory Type:            BLOCK
// Operating Mode:         Simple Dual Port (2-Port)
// Clock Mode:             Single Clock
// 
// RTL Code RW Resolution: RBW
// Catapult RW Resolution: RBW
// 
// HDL Work Library:       Xilinx_RAMS_lib
// Component Name:         BLOCK_1R1W_RBW
// Latency = 1:            RAM with no registers on inputs or outputs
//         = 2:            adds embedded register on RAM output
//         = 3:            adds fabric registers to non-clock input RAM pins
//         = 4:            adds fabric register to output (driven by embedded register from latency=2)
//         = 5:            adds fabric register to input (driven by fabric register from latency=3)
// suppress_sim_read_addr_range_errs:  0 - report errors  1 - suppress errors

module BLOCK_1R1W_RBW #(
  parameter addr_width = 8 ,
  parameter data_width = 7 ,
  parameter depth = 256 ,
  parameter latency = 1 ,
  parameter suppress_sim_read_addr_range_errs = 1 
  
)( clk,clken,d,q,radr,re,wadr,we);

  input  clk;
  input  clken;
  input [data_width-1:0] d;
  output [data_width-1:0] q;
  input [addr_width-1:0] radr;
  input  re;
  input [addr_width-1:0] wadr;
  input  we;
  
  (* ram_style = "block" , syn_ramstyle = "block_ram" *)
  reg [data_width-1:0] mem [depth-1:0];
  integer j;
  initial for (j = 0; j < depth; j = j + 1) mem[j] = 0;
  
  reg [data_width-1:0] ramq;
  
  // Port Map
  // readA :: CLOCK clk ENABLE clken DATA_OUT q ADDRESS radr READ_ENABLE re
  // writeA :: CLOCK clk ENABLE clken DATA_IN d ADDRESS wadr WRITE_ENABLE we

  generate
    if (latency > 4 ) begin
      reg [addr_width-1:0] radr_reg1;
      reg re_reg1;
      reg [data_width-1:0] d_reg1;
      reg [addr_width-1:0] wadr_reg1;
      reg we_reg1;
      
      always @(posedge clk) begin
        if (clken) begin
          radr_reg1 <= radr;
          re_reg1 <= re;
        end
      end
      always @(posedge clk) begin
        if (clken) begin
          d_reg1 <= d;
          wadr_reg1 <= wadr;
          we_reg1 <= we;
        end
      end
      
      reg [addr_width-1:0] radr_reg2;
      reg re_reg2;
      reg [data_width-1:0] d_reg2;
      reg [addr_width-1:0] wadr_reg2;
      reg we_reg2;
      
      always @(posedge clk) begin
        if (clken) begin
          radr_reg2 <= radr_reg1;
          re_reg2 <= re_reg1;
        end
      end
      always @(posedge clk) begin
        if (clken) begin
          d_reg2 <= d_reg1;
          wadr_reg2 <= wadr_reg1;
          we_reg2 <= we_reg1;
        end
      end
      
    // Access memory with registered inputs
      always @(posedge clk) begin
        if (clken) begin
            if (re_reg2) begin
              ramq <= mem[radr_reg2];
            end
            if (we_reg2) begin
              mem[wadr_reg2] <= d_reg2;
            end
        end
      end
      
    end // END register inputs

    // Register all non-clock inputs (latency < 3)
    else if (latency > 2 ) begin
      reg [addr_width-1:0] radr_reg;
      reg re_reg;
      reg [data_width-1:0] d_reg;
      reg [addr_width-1:0] wadr_reg;
      reg we_reg;
      
      always @(posedge clk) begin
        if (clken) begin
          radr_reg <= radr;
          re_reg <= re;
        end
      end
      always @(posedge clk) begin
        if (clken) begin
          d_reg <= d;
          wadr_reg <= wadr;
          we_reg <= we;
        end
      end
      
    // Access memory with registered inputs
      always @(posedge clk) begin
        if (clken) begin
            if (re_reg) begin
              ramq <= mem[radr_reg];
            end
            if (we_reg) begin
              mem[wadr_reg] <= d_reg;
            end
        end
      end
      
    end // END register inputs

    else begin
    // latency = 1||2: Access memory with non-registered inputs
      always @(posedge clk) begin
        if (clken) begin
            if (re) begin
              ramq <= mem[radr];
            end
            if (we) begin
              mem[wadr] <= d;
            end
        end
      end
      
    end
  endgenerate //END input port generate 

  generate
    // latency=1: sequential RAM outputs drive module outputs
    if (latency == 1) begin
      assign q = ramq;
      
    end

    else if (latency == 2 || latency == 3) begin
    // latency=2: sequential (RAM output => tmp register => module output)
      reg [data_width-1:0] tmpq;
      
      always @(posedge clk) begin
        if (clken) begin
          tmpq <= ramq;
        end
      end
      
      assign q = tmpq;
      
    end
    else if (latency == 4 || latency == 5) begin
    // latency=4: (RAM => tmp1 register => tmp2 fabric register => module output)
      reg [data_width-1:0] tmp1q;
      
      reg [data_width-1:0] tmp2q;
      
      always @(posedge clk) begin
        if (clken) begin
          tmp1q <= ramq;
        end
      end
      
      always @(posedge clk) begin
        if (clken) begin
          tmp2q <= tmp1q;
        end
      end
      
      assign q = tmp2q;
      
    end
    else begin
      //Add error check if latency > 5 or add N-pipeline regs
    end
  endgenerate //END output port generate

endmodule

//------> ./rtl.v 
// ----------------------------------------------------------------------
//  HLS HDL:        Verilog Netlister
//  HLS Version:    2025.2/1190995 Production Release
//  HLS Date:       Wed May 14 16:03:56 PDT 2025
// 
//  Generated by:   russk@orw-russk-vm
//  Generated date: Tue Sep  9 10:12:54 2025
// ----------------------------------------------------------------------

// 
// ------------------------------------------------------------------
//  Design Unit:    dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_18_8_512_256_1_256_512_1_gen
// ------------------------------------------------------------------


module dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_18_8_512_256_1_256_512_1_gen (
  clken, q, re, radr, we, d, wadr, clken_d, d_d, q_d, radr_d, re_d, wadr_d, we_d,
      writeA_w_ram_ir_internal_WMASK_B_d, readA_r_ram_ir_internal_RMASK_B_d
);
  output clken;
  input [511:0] q;
  output re;
  output [7:0] radr;
  output we;
  output [511:0] d;
  output [7:0] wadr;
  input clken_d;
  input [511:0] d_d;
  output [511:0] q_d;
  input [7:0] radr_d;
  input re_d;
  input [7:0] wadr_d;
  input we_d;
  input writeA_w_ram_ir_internal_WMASK_B_d;
  input readA_r_ram_ir_internal_RMASK_B_d;



  // Interconnect Declarations for Component Instantiations 
  assign clken = (clken_d);
  assign q_d = q;
  assign re = (readA_r_ram_ir_internal_RMASK_B_d);
  assign radr = (radr_d);
  assign we = (writeA_w_ram_ir_internal_WMASK_B_d);
  assign d = (d_d);
  assign wadr = (wadr_d);
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_17_15_512_32768_1_32768_512_1_gen
// ------------------------------------------------------------------


module dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_17_15_512_32768_1_32768_512_1_gen
    (
  clken, q, re, radr, we, d, wadr, clken_d, d_d, q_d, radr_d, re_d, wadr_d, we_d,
      writeA_w_ram_ir_internal_WMASK_B_d, readA_r_ram_ir_internal_RMASK_B_d
);
  output clken;
  input [511:0] q;
  output re;
  output [14:0] radr;
  output we;
  output [511:0] d;
  output [14:0] wadr;
  input clken_d;
  input [511:0] d_d;
  output [511:0] q_d;
  input [14:0] radr_d;
  input re_d;
  input [14:0] wadr_d;
  input we_d;
  input writeA_w_ram_ir_internal_WMASK_B_d;
  input readA_r_ram_ir_internal_RMASK_B_d;



  // Interconnect Declarations for Component Instantiations 
  assign clken = (clken_d);
  assign q_d = q;
  assign re = (readA_r_ram_ir_internal_RMASK_B_d);
  assign radr = (radr_d);
  assign we = (writeA_w_ram_ir_internal_WMASK_B_d);
  assign d = (d_d);
  assign wadr = (wadr_d);
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_core_fsm
//  FSM Module
// ------------------------------------------------------------------


module dense_core_core_fsm (
  clk, arst_n, core_wen, fsm_output, main_C_0_tr0, for_C_1_tr0, memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0,
      for_C_2_tr0, for_1_C_2_tr0, main_C_1_tr0, while_C_0_tr0, while_for_1_C_1_tr0,
      memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0, while_for_1_C_5_tr0,
      while_for_2_C_0_tr0, while_C_4_tr0
);
  input clk;
  input arst_n;
  input core_wen;
  output [23:0] fsm_output;
  reg [23:0] fsm_output;
  input main_C_0_tr0;
  input for_C_1_tr0;
  input memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0;
  input for_C_2_tr0;
  input for_1_C_2_tr0;
  input main_C_1_tr0;
  input while_C_0_tr0;
  input while_for_1_C_1_tr0;
  input memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0;
  input while_for_1_C_5_tr0;
  input while_for_2_C_0_tr0;
  input while_C_4_tr0;


  // FSM State Type Declaration for dense_core_core_fsm_1
  parameter
    core_rlp_C_0 = 5'd0,
    main_C_0 = 5'd1,
    for_C_0 = 5'd2,
    for_C_1 = 5'd3,
    memory_axi_burst_read_base_axi_u512_512_for_C_0 = 5'd4,
    for_C_2 = 5'd5,
    for_1_C_0 = 5'd6,
    for_1_C_1 = 5'd7,
    for_1_C_2 = 5'd8,
    main_C_1 = 5'd9,
    while_C_0 = 5'd10,
    while_for_1_C_0 = 5'd11,
    while_for_1_C_1 = 5'd12,
    memory_axi_burst_read_base_axi_u512_512_1_for_C_0 = 5'd13,
    while_for_1_C_2 = 5'd14,
    while_for_1_C_3 = 5'd15,
    while_for_1_C_4 = 5'd16,
    while_for_1_C_5 = 5'd17,
    while_for_2_C_0 = 5'd18,
    while_C_1 = 5'd19,
    while_C_2 = 5'd20,
    while_C_3 = 5'd21,
    while_C_4 = 5'd22,
    main_C_2 = 5'd23;

  reg [4:0] state_var;
  reg [4:0] state_var_NS;


  // Interconnect Declarations for Component Instantiations 
  always @(*)
  begin : dense_core_core_fsm_1
    case (state_var)
      main_C_0 : begin
        fsm_output = 24'b000000000000000000000010;
        if ( main_C_0_tr0 ) begin
          state_var_NS = for_1_C_0;
        end
        else begin
          state_var_NS = for_C_0;
        end
      end
      for_C_0 : begin
        fsm_output = 24'b000000000000000000000100;
        state_var_NS = for_C_1;
      end
      for_C_1 : begin
        fsm_output = 24'b000000000000000000001000;
        if ( for_C_1_tr0 ) begin
          state_var_NS = for_C_2;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_for_C_0;
        end
      end
      memory_axi_burst_read_base_axi_u512_512_for_C_0 : begin
        fsm_output = 24'b000000000000000000010000;
        if ( memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0 ) begin
          state_var_NS = for_C_2;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_for_C_0;
        end
      end
      for_C_2 : begin
        fsm_output = 24'b000000000000000000100000;
        if ( for_C_2_tr0 ) begin
          state_var_NS = for_1_C_0;
        end
        else begin
          state_var_NS = for_C_0;
        end
      end
      for_1_C_0 : begin
        fsm_output = 24'b000000000000000001000000;
        state_var_NS = for_1_C_1;
      end
      for_1_C_1 : begin
        fsm_output = 24'b000000000000000010000000;
        state_var_NS = for_1_C_2;
      end
      for_1_C_2 : begin
        fsm_output = 24'b000000000000000100000000;
        if ( for_1_C_2_tr0 ) begin
          state_var_NS = main_C_1;
        end
        else begin
          state_var_NS = for_1_C_0;
        end
      end
      main_C_1 : begin
        fsm_output = 24'b000000000000001000000000;
        if ( main_C_1_tr0 ) begin
          state_var_NS = main_C_2;
        end
        else begin
          state_var_NS = while_C_0;
        end
      end
      while_C_0 : begin
        fsm_output = 24'b000000000000010000000000;
        if ( while_C_0_tr0 ) begin
          state_var_NS = while_for_2_C_0;
        end
        else begin
          state_var_NS = while_for_1_C_0;
        end
      end
      while_for_1_C_0 : begin
        fsm_output = 24'b000000000000100000000000;
        state_var_NS = while_for_1_C_1;
      end
      while_for_1_C_1 : begin
        fsm_output = 24'b000000000001000000000000;
        if ( while_for_1_C_1_tr0 ) begin
          state_var_NS = while_for_1_C_2;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_1_for_C_0;
        end
      end
      memory_axi_burst_read_base_axi_u512_512_1_for_C_0 : begin
        fsm_output = 24'b000000000010000000000000;
        if ( memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0 ) begin
          state_var_NS = while_for_1_C_2;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_1_for_C_0;
        end
      end
      while_for_1_C_2 : begin
        fsm_output = 24'b000000000100000000000000;
        state_var_NS = while_for_1_C_3;
      end
      while_for_1_C_3 : begin
        fsm_output = 24'b000000001000000000000000;
        state_var_NS = while_for_1_C_4;
      end
      while_for_1_C_4 : begin
        fsm_output = 24'b000000010000000000000000;
        state_var_NS = while_for_1_C_5;
      end
      while_for_1_C_5 : begin
        fsm_output = 24'b000000100000000000000000;
        if ( while_for_1_C_5_tr0 ) begin
          state_var_NS = while_for_2_C_0;
        end
        else begin
          state_var_NS = while_for_1_C_0;
        end
      end
      while_for_2_C_0 : begin
        fsm_output = 24'b000001000000000000000000;
        if ( while_for_2_C_0_tr0 ) begin
          state_var_NS = while_C_1;
        end
        else begin
          state_var_NS = while_for_2_C_0;
        end
      end
      while_C_1 : begin
        fsm_output = 24'b000010000000000000000000;
        state_var_NS = while_C_2;
      end
      while_C_2 : begin
        fsm_output = 24'b000100000000000000000000;
        state_var_NS = while_C_3;
      end
      while_C_3 : begin
        fsm_output = 24'b001000000000000000000000;
        state_var_NS = while_C_4;
      end
      while_C_4 : begin
        fsm_output = 24'b010000000000000000000000;
        if ( while_C_4_tr0 ) begin
          state_var_NS = main_C_2;
        end
        else begin
          state_var_NS = while_C_0;
        end
      end
      main_C_2 : begin
        fsm_output = 24'b100000000000000000000000;
        state_var_NS = main_C_0;
      end
      // core_rlp_C_0
      default : begin
        fsm_output = 24'b000000000000000000000001;
        state_var_NS = main_C_0;
      end
    endcase
  end

  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      state_var <= core_rlp_C_0;
    end
    else if ( core_wen ) begin
      state_var <= state_var_NS;
    end
  end

endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_staller
// ------------------------------------------------------------------


module dense_core_staller (
  clk, arst_n, core_wen, core_wten, start_rsci_wen_comp, done_rsci_wen_comp, memory_channels_aw_channel_rsci_wen_comp,
      memory_channels_w_channel_rsci_wen_comp, memory_channels_b_channel_rsci_wen_comp,
      memory_channels_ar_channel_rsci_wen_comp, memory_channels_r_channel_rsci_wen_comp,
      start_rsci_wen_comp_pff, done_rsci_wen_comp_pff, memory_channels_aw_channel_rsci_wen_comp_pff,
      memory_channels_w_channel_rsci_wen_comp_pff, memory_channels_b_channel_rsci_wen_comp_pff,
      memory_channels_ar_channel_rsci_wen_comp_pff, memory_channels_r_channel_rsci_wen_comp_pff
);
  input clk;
  input arst_n;
  output core_wen;
  output core_wten;
  reg core_wten;
  input start_rsci_wen_comp;
  input done_rsci_wen_comp;
  input memory_channels_aw_channel_rsci_wen_comp;
  input memory_channels_w_channel_rsci_wen_comp;
  input memory_channels_b_channel_rsci_wen_comp;
  input memory_channels_ar_channel_rsci_wen_comp;
  input memory_channels_r_channel_rsci_wen_comp;
  input start_rsci_wen_comp_pff;
  input done_rsci_wen_comp_pff;
  input memory_channels_aw_channel_rsci_wen_comp_pff;
  input memory_channels_w_channel_rsci_wen_comp_pff;
  input memory_channels_b_channel_rsci_wen_comp_pff;
  input memory_channels_ar_channel_rsci_wen_comp_pff;
  input memory_channels_r_channel_rsci_wen_comp_pff;



  // Interconnect Declarations for Component Instantiations 
  assign core_wen = start_rsci_wen_comp_pff & done_rsci_wen_comp_pff & memory_channels_aw_channel_rsci_wen_comp_pff
      & memory_channels_w_channel_rsci_wen_comp_pff & memory_channels_b_channel_rsci_wen_comp_pff
      & memory_channels_ar_channel_rsci_wen_comp_pff & memory_channels_r_channel_rsci_wen_comp_pff;
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      core_wten <= 1'b0;
    end
    else begin
      core_wten <= ~(start_rsci_wen_comp & done_rsci_wen_comp & memory_channels_aw_channel_rsci_wen_comp
          & memory_channels_w_channel_rsci_wen_comp & memory_channels_b_channel_rsci_wen_comp
          & memory_channels_ar_channel_rsci_wen_comp & memory_channels_r_channel_rsci_wen_comp);
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_debug_triosy_obj_debug_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_debug_triosy_obj_debug_triosy_wait_ctrl (
  core_wten, debug_triosy_obj_iswt0, debug_triosy_obj_biwt
);
  input core_wten;
  input debug_triosy_obj_iswt0;
  output debug_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign debug_triosy_obj_biwt = (~ core_wten) & debug_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_output_vector_len_triosy_obj_output_vector_len_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_output_vector_len_triosy_obj_output_vector_len_triosy_wait_ctrl
    (
  core_wten, output_vector_len_triosy_obj_iswt0, output_vector_len_triosy_obj_biwt
);
  input core_wten;
  input output_vector_len_triosy_obj_iswt0;
  output output_vector_len_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign output_vector_len_triosy_obj_biwt = (~ core_wten) & output_vector_len_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_input_vector_len_triosy_obj_input_vector_len_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_input_vector_len_triosy_obj_input_vector_len_triosy_wait_ctrl (
  core_wten, input_vector_len_triosy_obj_iswt0, input_vector_len_triosy_obj_biwt
);
  input core_wten;
  input input_vector_len_triosy_obj_iswt0;
  output input_vector_len_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign input_vector_len_triosy_obj_biwt = (~ core_wten) & input_vector_len_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_output_addr_lo_triosy_obj_output_addr_lo_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_output_addr_lo_triosy_obj_output_addr_lo_triosy_wait_ctrl (
  core_wten, output_addr_lo_triosy_obj_iswt0, output_addr_lo_triosy_obj_biwt
);
  input core_wten;
  input output_addr_lo_triosy_obj_iswt0;
  output output_addr_lo_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign output_addr_lo_triosy_obj_biwt = (~ core_wten) & output_addr_lo_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_weight_addr_lo_triosy_obj_weight_addr_lo_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_weight_addr_lo_triosy_obj_weight_addr_lo_triosy_wait_ctrl (
  core_wten, weight_addr_lo_triosy_obj_iswt0, weight_addr_lo_triosy_obj_biwt
);
  input core_wten;
  input weight_addr_lo_triosy_obj_iswt0;
  output weight_addr_lo_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign weight_addr_lo_triosy_obj_biwt = (~ core_wten) & weight_addr_lo_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_feature_addr_lo_triosy_obj_feature_addr_lo_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_feature_addr_lo_triosy_obj_feature_addr_lo_triosy_wait_ctrl (
  core_wten, feature_addr_lo_triosy_obj_iswt0, feature_addr_lo_triosy_obj_biwt
);
  input core_wten;
  input feature_addr_lo_triosy_obj_iswt0;
  output feature_addr_lo_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign feature_addr_lo_triosy_obj_biwt = (~ core_wten) & feature_addr_lo_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_addr_hi_triosy_obj_addr_hi_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_addr_hi_triosy_obj_addr_hi_triosy_wait_ctrl (
  core_wten, addr_hi_triosy_obj_iswt0, addr_hi_triosy_obj_biwt
);
  input core_wten;
  input addr_hi_triosy_obj_iswt0;
  output addr_hi_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign addr_hi_triosy_obj_biwt = (~ core_wten) & addr_hi_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_use_relu_triosy_obj_use_relu_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_use_relu_triosy_obj_use_relu_triosy_wait_ctrl (
  core_wten, use_relu_triosy_obj_iswt0, use_relu_triosy_obj_biwt
);
  input core_wten;
  input use_relu_triosy_obj_iswt0;
  output use_relu_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign use_relu_triosy_obj_biwt = (~ core_wten) & use_relu_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_dp
// ------------------------------------------------------------------


module dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_dp
    (
  clk, arst_n, memory_channels_r_channel_rsci_oswt, memory_channels_r_channel_rsci_wen_comp,
      memory_channels_r_channel_rsci_idat_mxwt, memory_channels_r_channel_rsci_biwt,
      memory_channels_r_channel_rsci_bdwt, memory_channels_r_channel_rsci_bcwt, memory_channels_r_channel_rsci_idat,
      memory_channels_r_channel_rsci_wen_comp_pff, memory_channels_r_channel_rsci_oswt_pff,
      memory_channels_r_channel_rsci_biwt_pff, memory_channels_r_channel_rsci_bcwt_pff
);
  input clk;
  input arst_n;
  input memory_channels_r_channel_rsci_oswt;
  output memory_channels_r_channel_rsci_wen_comp;
  output [513:0] memory_channels_r_channel_rsci_idat_mxwt;
  input memory_channels_r_channel_rsci_biwt;
  input memory_channels_r_channel_rsci_bdwt;
  output memory_channels_r_channel_rsci_bcwt;
  input [530:0] memory_channels_r_channel_rsci_idat;
  output memory_channels_r_channel_rsci_wen_comp_pff;
  input memory_channels_r_channel_rsci_oswt_pff;
  input memory_channels_r_channel_rsci_biwt_pff;
  output memory_channels_r_channel_rsci_bcwt_pff;


  // Interconnect Declarations
  reg [513:0] memory_channels_r_channel_rsci_idat_bfwt_514_1;
  reg memory_channels_r_channel_rsci_bcwt_reg;
  wire memory_get_r_nor_rmff;


  // Interconnect Declarations for Component Instantiations 
  assign memory_get_r_nor_rmff = ~((~(memory_channels_r_channel_rsci_bcwt | memory_channels_r_channel_rsci_biwt))
      | memory_channels_r_channel_rsci_bdwt);
  assign memory_channels_r_channel_rsci_idat_mxwt = MUX_v_514_2_2((memory_channels_r_channel_rsci_idat[514:1]),
      memory_channels_r_channel_rsci_idat_bfwt_514_1, memory_channels_r_channel_rsci_bcwt);
  assign memory_channels_r_channel_rsci_wen_comp = (~ memory_channels_r_channel_rsci_oswt)
      | memory_channels_r_channel_rsci_biwt | memory_channels_r_channel_rsci_bcwt;
  assign memory_channels_r_channel_rsci_wen_comp_pff = (~ memory_channels_r_channel_rsci_oswt_pff)
      | memory_channels_r_channel_rsci_biwt_pff | memory_channels_r_channel_rsci_bcwt_pff;
  assign memory_channels_r_channel_rsci_bcwt = memory_channels_r_channel_rsci_bcwt_reg;
  assign memory_channels_r_channel_rsci_bcwt_pff = memory_get_r_nor_rmff;
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      memory_channels_r_channel_rsci_bcwt_reg <= 1'b0;
    end
    else begin
      memory_channels_r_channel_rsci_bcwt_reg <= memory_get_r_nor_rmff;
    end
  end
  always @(posedge clk) begin
    if ( memory_channels_r_channel_rsci_biwt ) begin
      memory_channels_r_channel_rsci_idat_bfwt_514_1 <= memory_channels_r_channel_rsci_idat[514:1];
    end
  end

  function automatic [513:0] MUX_v_514_2_2;
    input [513:0] input_0;
    input [513:0] input_1;
    input  sel;
    reg [513:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_514_2_2 = result;
  end
  endfunction

endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl
// ------------------------------------------------------------------


module dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl
    (
  core_wen, memory_channels_r_channel_rsci_oswt, memory_channels_r_channel_rsci_ivld_oreg,
      memory_channels_r_channel_rsci_biwt, memory_channels_r_channel_rsci_bdwt, memory_channels_r_channel_rsci_bcwt,
      memory_channels_r_channel_rsci_irdy_core_sct, memory_channels_r_channel_rsci_biwt_pff,
      memory_channels_r_channel_rsci_oswt_pff, memory_channels_r_channel_rsci_bcwt_pff,
      memory_channels_r_channel_rsci_ivld_oreg_pff
);
  input core_wen;
  input memory_channels_r_channel_rsci_oswt;
  input memory_channels_r_channel_rsci_ivld_oreg;
  output memory_channels_r_channel_rsci_biwt;
  output memory_channels_r_channel_rsci_bdwt;
  input memory_channels_r_channel_rsci_bcwt;
  output memory_channels_r_channel_rsci_irdy_core_sct;
  output memory_channels_r_channel_rsci_biwt_pff;
  input memory_channels_r_channel_rsci_oswt_pff;
  input memory_channels_r_channel_rsci_bcwt_pff;
  input memory_channels_r_channel_rsci_ivld_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_r_channel_rsci_ogwt;


  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_r_channel_rsci_bdwt = memory_channels_r_channel_rsci_oswt
      & core_wen;
  assign memory_channels_r_channel_rsci_ogwt = memory_channels_r_channel_rsci_oswt
      & (~ memory_channels_r_channel_rsci_bcwt);
  assign memory_channels_r_channel_rsci_irdy_core_sct = memory_channels_r_channel_rsci_ogwt;
  assign memory_channels_r_channel_rsci_biwt = memory_channels_r_channel_rsci_ogwt
      & memory_channels_r_channel_rsci_ivld_oreg;
  assign memory_channels_r_channel_rsci_biwt_pff = memory_channels_r_channel_rsci_oswt_pff
      & (~ memory_channels_r_channel_rsci_bcwt_pff) & memory_channels_r_channel_rsci_ivld_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl
// ------------------------------------------------------------------


module dense_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl
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
//  Design Unit:    dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl
// ------------------------------------------------------------------


module dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl
    (
  memory_channels_b_channel_rsci_iswt0, memory_channels_b_channel_rsci_ivld_oreg,
      memory_channels_b_channel_rsci_biwt, memory_channels_b_channel_rsci_biwt_pff,
      memory_channels_b_channel_rsci_iswt0_pff, memory_channels_b_channel_rsci_ivld_oreg_pff
);
  input memory_channels_b_channel_rsci_iswt0;
  input memory_channels_b_channel_rsci_ivld_oreg;
  output memory_channels_b_channel_rsci_biwt;
  output memory_channels_b_channel_rsci_biwt_pff;
  input memory_channels_b_channel_rsci_iswt0_pff;
  input memory_channels_b_channel_rsci_ivld_oreg_pff;



  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_b_channel_rsci_biwt = memory_channels_b_channel_rsci_iswt0
      & memory_channels_b_channel_rsci_ivld_oreg;
  assign memory_channels_b_channel_rsci_biwt_pff = memory_channels_b_channel_rsci_iswt0_pff
      & memory_channels_b_channel_rsci_ivld_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_w_channel_rsci_memory_channels_w_channel_wait_ctrl
// ------------------------------------------------------------------


module dense_core_memory_channels_w_channel_rsci_memory_channels_w_channel_wait_ctrl
    (
  memory_channels_w_channel_rsci_iswt0, memory_channels_w_channel_rsci_irdy_oreg,
      memory_channels_w_channel_rsci_biwt, memory_channels_w_channel_rsci_biwt_pff,
      memory_channels_w_channel_rsci_iswt0_pff, memory_channels_w_channel_rsci_irdy_oreg_pff
);
  input memory_channels_w_channel_rsci_iswt0;
  input memory_channels_w_channel_rsci_irdy_oreg;
  output memory_channels_w_channel_rsci_biwt;
  output memory_channels_w_channel_rsci_biwt_pff;
  input memory_channels_w_channel_rsci_iswt0_pff;
  input memory_channels_w_channel_rsci_irdy_oreg_pff;



  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_w_channel_rsci_biwt = memory_channels_w_channel_rsci_iswt0
      & memory_channels_w_channel_rsci_irdy_oreg;
  assign memory_channels_w_channel_rsci_biwt_pff = memory_channels_w_channel_rsci_iswt0_pff
      & memory_channels_w_channel_rsci_irdy_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_aw_channel_rsci_memory_channels_aw_channel_wait_ctrl
// ------------------------------------------------------------------


module dense_core_memory_channels_aw_channel_rsci_memory_channels_aw_channel_wait_ctrl
    (
  memory_channels_aw_channel_rsci_iswt0, memory_channels_aw_channel_rsci_irdy_oreg,
      memory_channels_aw_channel_rsci_biwt, memory_channels_aw_channel_rsci_biwt_pff,
      memory_channels_aw_channel_rsci_iswt0_pff, memory_channels_aw_channel_rsci_irdy_oreg_pff
);
  input memory_channels_aw_channel_rsci_iswt0;
  input memory_channels_aw_channel_rsci_irdy_oreg;
  output memory_channels_aw_channel_rsci_biwt;
  output memory_channels_aw_channel_rsci_biwt_pff;
  input memory_channels_aw_channel_rsci_iswt0_pff;
  input memory_channels_aw_channel_rsci_irdy_oreg_pff;



  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_aw_channel_rsci_biwt = memory_channels_aw_channel_rsci_iswt0
      & memory_channels_aw_channel_rsci_irdy_oreg;
  assign memory_channels_aw_channel_rsci_biwt_pff = memory_channels_aw_channel_rsci_iswt0_pff
      & memory_channels_aw_channel_rsci_irdy_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_done_rsci_done_wait_ctrl
// ------------------------------------------------------------------


module dense_core_done_rsci_done_wait_ctrl (
  done_rsci_iswt0, done_rsci_irdy_oreg, done_rsci_biwt, done_rsci_biwt_pff, done_rsci_iswt0_pff,
      done_rsci_irdy_oreg_pff
);
  input done_rsci_iswt0;
  input done_rsci_irdy_oreg;
  output done_rsci_biwt;
  output done_rsci_biwt_pff;
  input done_rsci_iswt0_pff;
  input done_rsci_irdy_oreg_pff;



  // Interconnect Declarations for Component Instantiations 
  assign done_rsci_biwt = done_rsci_iswt0 & done_rsci_irdy_oreg;
  assign done_rsci_biwt_pff = done_rsci_iswt0_pff & done_rsci_irdy_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_wait_dp
// ------------------------------------------------------------------


module dense_core_wait_dp (
  clk, arst_n, start_rsci_ivld, start_rsci_ivld_oreg, done_rsci_irdy, done_rsci_irdy_oreg,
      memory_channels_aw_channel_rsci_irdy, memory_channels_aw_channel_rsci_irdy_oreg,
      memory_channels_w_channel_rsci_irdy, memory_channels_w_channel_rsci_irdy_oreg,
      memory_channels_b_channel_rsci_ivld, memory_channels_b_channel_rsci_ivld_oreg,
      memory_channels_ar_channel_rsci_irdy, memory_channels_ar_channel_rsci_irdy_oreg,
      memory_channels_r_channel_rsci_ivld, memory_channels_r_channel_rsci_ivld_oreg
);
  input clk;
  input arst_n;
  input start_rsci_ivld;
  output start_rsci_ivld_oreg;
  input done_rsci_irdy;
  output done_rsci_irdy_oreg;
  input memory_channels_aw_channel_rsci_irdy;
  output memory_channels_aw_channel_rsci_irdy_oreg;
  input memory_channels_w_channel_rsci_irdy;
  output memory_channels_w_channel_rsci_irdy_oreg;
  input memory_channels_b_channel_rsci_ivld;
  output memory_channels_b_channel_rsci_ivld_oreg;
  input memory_channels_ar_channel_rsci_irdy;
  output memory_channels_ar_channel_rsci_irdy_oreg;
  input memory_channels_r_channel_rsci_ivld;
  output memory_channels_r_channel_rsci_ivld_oreg;


  // Interconnect Declarations
  reg start_rsci_ivld_oreg_rneg;
  reg done_rsci_irdy_oreg_rneg;
  reg memory_channels_aw_channel_rsci_irdy_oreg_rneg;
  reg memory_channels_w_channel_rsci_irdy_oreg_rneg;
  reg memory_channels_b_channel_rsci_ivld_oreg_rneg;
  reg memory_channels_ar_channel_rsci_irdy_oreg_rneg;
  reg memory_channels_r_channel_rsci_ivld_oreg_rneg;


  // Interconnect Declarations for Component Instantiations 
  assign start_rsci_ivld_oreg = ~ start_rsci_ivld_oreg_rneg;
  assign done_rsci_irdy_oreg = ~ done_rsci_irdy_oreg_rneg;
  assign memory_channels_aw_channel_rsci_irdy_oreg = ~ memory_channels_aw_channel_rsci_irdy_oreg_rneg;
  assign memory_channels_w_channel_rsci_irdy_oreg = ~ memory_channels_w_channel_rsci_irdy_oreg_rneg;
  assign memory_channels_b_channel_rsci_ivld_oreg = ~ memory_channels_b_channel_rsci_ivld_oreg_rneg;
  assign memory_channels_ar_channel_rsci_irdy_oreg = ~ memory_channels_ar_channel_rsci_irdy_oreg_rneg;
  assign memory_channels_r_channel_rsci_ivld_oreg = ~ memory_channels_r_channel_rsci_ivld_oreg_rneg;
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      start_rsci_ivld_oreg_rneg <= 1'b0;
      done_rsci_irdy_oreg_rneg <= 1'b0;
      memory_channels_aw_channel_rsci_irdy_oreg_rneg <= 1'b0;
      memory_channels_w_channel_rsci_irdy_oreg_rneg <= 1'b0;
      memory_channels_b_channel_rsci_ivld_oreg_rneg <= 1'b0;
      memory_channels_ar_channel_rsci_irdy_oreg_rneg <= 1'b0;
      memory_channels_r_channel_rsci_ivld_oreg_rneg <= 1'b0;
    end
    else begin
      start_rsci_ivld_oreg_rneg <= ~ start_rsci_ivld;
      done_rsci_irdy_oreg_rneg <= ~ done_rsci_irdy;
      memory_channels_aw_channel_rsci_irdy_oreg_rneg <= ~ memory_channels_aw_channel_rsci_irdy;
      memory_channels_w_channel_rsci_irdy_oreg_rneg <= ~ memory_channels_w_channel_rsci_irdy;
      memory_channels_b_channel_rsci_ivld_oreg_rneg <= ~ memory_channels_b_channel_rsci_ivld;
      memory_channels_ar_channel_rsci_irdy_oreg_rneg <= ~ memory_channels_ar_channel_rsci_irdy;
      memory_channels_r_channel_rsci_ivld_oreg_rneg <= ~ memory_channels_r_channel_rsci_ivld;
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_start_rsci_start_wait_ctrl
// ------------------------------------------------------------------


module dense_core_start_rsci_start_wait_ctrl (
  start_rsci_iswt0, start_rsci_ivld_oreg, start_rsci_biwt, start_rsci_biwt_pff, start_rsci_iswt0_pff,
      start_rsci_ivld_oreg_pff
);
  input start_rsci_iswt0;
  input start_rsci_ivld_oreg;
  output start_rsci_biwt;
  output start_rsci_biwt_pff;
  input start_rsci_iswt0_pff;
  input start_rsci_ivld_oreg_pff;



  // Interconnect Declarations for Component Instantiations 
  assign start_rsci_biwt = start_rsci_iswt0 & start_rsci_ivld_oreg;
  assign start_rsci_biwt_pff = start_rsci_iswt0_pff & start_rsci_ivld_oreg_pff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_debug_triosy_obj
// ------------------------------------------------------------------


module dense_core_debug_triosy_obj (
  debug_triosy_lz, core_wten, debug_triosy_obj_iswt0
);
  output debug_triosy_lz;
  input core_wten;
  input debug_triosy_obj_iswt0;


  // Interconnect Declarations
  wire debug_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) debug_triosy_obj (
      .ld(debug_triosy_obj_biwt),
      .lz(debug_triosy_lz)
    );
  dense_core_debug_triosy_obj_debug_triosy_wait_ctrl dense_core_debug_triosy_obj_debug_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .debug_triosy_obj_iswt0(debug_triosy_obj_iswt0),
      .debug_triosy_obj_biwt(debug_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_output_vector_len_triosy_obj
// ------------------------------------------------------------------


module dense_core_output_vector_len_triosy_obj (
  output_vector_len_triosy_lz, core_wten, output_vector_len_triosy_obj_iswt0
);
  output output_vector_len_triosy_lz;
  input core_wten;
  input output_vector_len_triosy_obj_iswt0;


  // Interconnect Declarations
  wire output_vector_len_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) output_vector_len_triosy_obj (
      .ld(output_vector_len_triosy_obj_biwt),
      .lz(output_vector_len_triosy_lz)
    );
  dense_core_output_vector_len_triosy_obj_output_vector_len_triosy_wait_ctrl dense_core_output_vector_len_triosy_obj_output_vector_len_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .output_vector_len_triosy_obj_iswt0(output_vector_len_triosy_obj_iswt0),
      .output_vector_len_triosy_obj_biwt(output_vector_len_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_input_vector_len_triosy_obj
// ------------------------------------------------------------------


module dense_core_input_vector_len_triosy_obj (
  input_vector_len_triosy_lz, core_wten, input_vector_len_triosy_obj_iswt0
);
  output input_vector_len_triosy_lz;
  input core_wten;
  input input_vector_len_triosy_obj_iswt0;


  // Interconnect Declarations
  wire input_vector_len_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) input_vector_len_triosy_obj (
      .ld(input_vector_len_triosy_obj_biwt),
      .lz(input_vector_len_triosy_lz)
    );
  dense_core_input_vector_len_triosy_obj_input_vector_len_triosy_wait_ctrl dense_core_input_vector_len_triosy_obj_input_vector_len_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .input_vector_len_triosy_obj_iswt0(input_vector_len_triosy_obj_iswt0),
      .input_vector_len_triosy_obj_biwt(input_vector_len_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_output_addr_lo_triosy_obj
// ------------------------------------------------------------------


module dense_core_output_addr_lo_triosy_obj (
  output_addr_lo_triosy_lz, core_wten, output_addr_lo_triosy_obj_iswt0
);
  output output_addr_lo_triosy_lz;
  input core_wten;
  input output_addr_lo_triosy_obj_iswt0;


  // Interconnect Declarations
  wire output_addr_lo_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) output_addr_lo_triosy_obj (
      .ld(output_addr_lo_triosy_obj_biwt),
      .lz(output_addr_lo_triosy_lz)
    );
  dense_core_output_addr_lo_triosy_obj_output_addr_lo_triosy_wait_ctrl dense_core_output_addr_lo_triosy_obj_output_addr_lo_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .output_addr_lo_triosy_obj_iswt0(output_addr_lo_triosy_obj_iswt0),
      .output_addr_lo_triosy_obj_biwt(output_addr_lo_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_weight_addr_lo_triosy_obj
// ------------------------------------------------------------------


module dense_core_weight_addr_lo_triosy_obj (
  weight_addr_lo_triosy_lz, core_wten, weight_addr_lo_triosy_obj_iswt0
);
  output weight_addr_lo_triosy_lz;
  input core_wten;
  input weight_addr_lo_triosy_obj_iswt0;


  // Interconnect Declarations
  wire weight_addr_lo_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) weight_addr_lo_triosy_obj (
      .ld(weight_addr_lo_triosy_obj_biwt),
      .lz(weight_addr_lo_triosy_lz)
    );
  dense_core_weight_addr_lo_triosy_obj_weight_addr_lo_triosy_wait_ctrl dense_core_weight_addr_lo_triosy_obj_weight_addr_lo_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .weight_addr_lo_triosy_obj_iswt0(weight_addr_lo_triosy_obj_iswt0),
      .weight_addr_lo_triosy_obj_biwt(weight_addr_lo_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_feature_addr_lo_triosy_obj
// ------------------------------------------------------------------


module dense_core_feature_addr_lo_triosy_obj (
  feature_addr_lo_triosy_lz, core_wten, feature_addr_lo_triosy_obj_iswt0
);
  output feature_addr_lo_triosy_lz;
  input core_wten;
  input feature_addr_lo_triosy_obj_iswt0;


  // Interconnect Declarations
  wire feature_addr_lo_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) feature_addr_lo_triosy_obj (
      .ld(feature_addr_lo_triosy_obj_biwt),
      .lz(feature_addr_lo_triosy_lz)
    );
  dense_core_feature_addr_lo_triosy_obj_feature_addr_lo_triosy_wait_ctrl dense_core_feature_addr_lo_triosy_obj_feature_addr_lo_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .feature_addr_lo_triosy_obj_iswt0(feature_addr_lo_triosy_obj_iswt0),
      .feature_addr_lo_triosy_obj_biwt(feature_addr_lo_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_addr_hi_triosy_obj
// ------------------------------------------------------------------


module dense_core_addr_hi_triosy_obj (
  addr_hi_triosy_lz, core_wten, addr_hi_triosy_obj_iswt0
);
  output addr_hi_triosy_lz;
  input core_wten;
  input addr_hi_triosy_obj_iswt0;


  // Interconnect Declarations
  wire addr_hi_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) addr_hi_triosy_obj (
      .ld(addr_hi_triosy_obj_biwt),
      .lz(addr_hi_triosy_lz)
    );
  dense_core_addr_hi_triosy_obj_addr_hi_triosy_wait_ctrl dense_core_addr_hi_triosy_obj_addr_hi_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .addr_hi_triosy_obj_iswt0(addr_hi_triosy_obj_iswt0),
      .addr_hi_triosy_obj_biwt(addr_hi_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_use_relu_triosy_obj
// ------------------------------------------------------------------


module dense_core_use_relu_triosy_obj (
  use_relu_triosy_lz, core_wten, use_relu_triosy_obj_iswt0
);
  output use_relu_triosy_lz;
  input core_wten;
  input use_relu_triosy_obj_iswt0;


  // Interconnect Declarations
  wire use_relu_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) use_relu_triosy_obj (
      .ld(use_relu_triosy_obj_biwt),
      .lz(use_relu_triosy_lz)
    );
  dense_core_use_relu_triosy_obj_use_relu_triosy_wait_ctrl dense_core_use_relu_triosy_obj_use_relu_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .use_relu_triosy_obj_iswt0(use_relu_triosy_obj_iswt0),
      .use_relu_triosy_obj_biwt(use_relu_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_r_channel_rsci
// ------------------------------------------------------------------


module dense_core_memory_channels_r_channel_rsci (
  clk, arst_n, memory_channels_r_channel_rsc_dat, memory_channels_r_channel_rsc_vld,
      memory_channels_r_channel_rsc_rdy, core_wen, memory_channels_r_channel_rsci_oswt,
      memory_channels_r_channel_rsci_wen_comp, memory_channels_r_channel_rsci_ivld,
      memory_channels_r_channel_rsci_ivld_oreg, memory_channels_r_channel_rsci_idat_mxwt,
      memory_channels_r_channel_rsci_wen_comp_pff, memory_channels_r_channel_rsci_oswt_pff,
      memory_channels_r_channel_rsci_ivld_oreg_pff
);
  input clk;
  input arst_n;
  input [530:0] memory_channels_r_channel_rsc_dat;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;
  input core_wen;
  input memory_channels_r_channel_rsci_oswt;
  output memory_channels_r_channel_rsci_wen_comp;
  output memory_channels_r_channel_rsci_ivld;
  input memory_channels_r_channel_rsci_ivld_oreg;
  output [513:0] memory_channels_r_channel_rsci_idat_mxwt;
  output memory_channels_r_channel_rsci_wen_comp_pff;
  input memory_channels_r_channel_rsci_oswt_pff;
  input memory_channels_r_channel_rsci_ivld_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_r_channel_rsci_biwt;
  wire memory_channels_r_channel_rsci_bdwt;
  wire memory_channels_r_channel_rsci_bcwt;
  wire memory_channels_r_channel_rsci_irdy_core_sct;
  wire [530:0] memory_channels_r_channel_rsci_idat;
  wire memory_channels_r_channel_rsc_is_idle;
  wire [513:0] memory_channels_r_channel_rsci_idat_mxwt_pconst;
  wire memory_channels_r_channel_rsci_wen_comp_reg;
  wire memory_channels_r_channel_rsci_wen_comp_iff;
  wire memory_channels_r_channel_rsci_biwt_iff;
  wire memory_channels_r_channel_rsci_bcwt_iff;


  // Interconnect Declarations for Component Instantiations 
  ccs_ctrl_in_buf_wait_v4 #(.rscid(32'sd15),
  .width(32'sd531),
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
      .irdy(memory_channels_r_channel_rsci_irdy_core_sct),
      .ivld(memory_channels_r_channel_rsci_ivld),
      .idat(memory_channels_r_channel_rsci_idat),
      .is_idle(memory_channels_r_channel_rsc_is_idle)
    );
  dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_ctrl_inst
      (
      .core_wen(core_wen),
      .memory_channels_r_channel_rsci_oswt(memory_channels_r_channel_rsci_oswt),
      .memory_channels_r_channel_rsci_ivld_oreg(memory_channels_r_channel_rsci_ivld_oreg),
      .memory_channels_r_channel_rsci_biwt(memory_channels_r_channel_rsci_biwt),
      .memory_channels_r_channel_rsci_bdwt(memory_channels_r_channel_rsci_bdwt),
      .memory_channels_r_channel_rsci_bcwt(memory_channels_r_channel_rsci_bcwt),
      .memory_channels_r_channel_rsci_irdy_core_sct(memory_channels_r_channel_rsci_irdy_core_sct),
      .memory_channels_r_channel_rsci_biwt_pff(memory_channels_r_channel_rsci_biwt_iff),
      .memory_channels_r_channel_rsci_oswt_pff(memory_channels_r_channel_rsci_oswt_pff),
      .memory_channels_r_channel_rsci_bcwt_pff(memory_channels_r_channel_rsci_bcwt_iff),
      .memory_channels_r_channel_rsci_ivld_oreg_pff(memory_channels_r_channel_rsci_ivld_oreg_pff)
    );
  dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_dp dense_core_memory_channels_r_channel_rsci_memory_channels_r_channel_wait_dp_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_r_channel_rsci_oswt(memory_channels_r_channel_rsci_oswt),
      .memory_channels_r_channel_rsci_wen_comp(memory_channels_r_channel_rsci_wen_comp_reg),
      .memory_channels_r_channel_rsci_idat_mxwt(memory_channels_r_channel_rsci_idat_mxwt_pconst),
      .memory_channels_r_channel_rsci_biwt(memory_channels_r_channel_rsci_biwt),
      .memory_channels_r_channel_rsci_bdwt(memory_channels_r_channel_rsci_bdwt),
      .memory_channels_r_channel_rsci_bcwt(memory_channels_r_channel_rsci_bcwt),
      .memory_channels_r_channel_rsci_idat(memory_channels_r_channel_rsci_idat),
      .memory_channels_r_channel_rsci_wen_comp_pff(memory_channels_r_channel_rsci_wen_comp_iff),
      .memory_channels_r_channel_rsci_oswt_pff(memory_channels_r_channel_rsci_oswt_pff),
      .memory_channels_r_channel_rsci_biwt_pff(memory_channels_r_channel_rsci_biwt_iff),
      .memory_channels_r_channel_rsci_bcwt_pff(memory_channels_r_channel_rsci_bcwt_iff)
    );
  assign memory_channels_r_channel_rsci_idat_mxwt = memory_channels_r_channel_rsci_idat_mxwt_pconst;
  assign memory_channels_r_channel_rsci_wen_comp = memory_channels_r_channel_rsci_wen_comp_reg;
  assign memory_channels_r_channel_rsci_wen_comp_pff = memory_channels_r_channel_rsci_wen_comp_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_ar_channel_rsci
// ------------------------------------------------------------------


module dense_core_memory_channels_ar_channel_rsci (
  clk, arst_n, memory_channels_ar_channel_rsc_dat, memory_channels_ar_channel_rsc_vld,
      memory_channels_ar_channel_rsc_rdy, memory_channels_ar_channel_rsci_oswt, memory_channels_ar_channel_rsci_wen_comp,
      memory_channels_ar_channel_rsci_irdy, memory_channels_ar_channel_rsci_irdy_oreg,
      memory_channels_ar_channel_rsci_idat, memory_channels_ar_channel_rsci_wen_comp_pff,
      memory_channels_ar_channel_rsci_oswt_pff, memory_channels_ar_channel_rsci_irdy_oreg_pff
);
  input clk;
  input arst_n;
  output [108:0] memory_channels_ar_channel_rsc_dat;
  output memory_channels_ar_channel_rsc_vld;
  input memory_channels_ar_channel_rsc_rdy;
  input memory_channels_ar_channel_rsci_oswt;
  output memory_channels_ar_channel_rsci_wen_comp;
  output memory_channels_ar_channel_rsci_irdy;
  input memory_channels_ar_channel_rsci_irdy_oreg;
  input [108:0] memory_channels_ar_channel_rsci_idat;
  output memory_channels_ar_channel_rsci_wen_comp_pff;
  input memory_channels_ar_channel_rsci_oswt_pff;
  input memory_channels_ar_channel_rsci_irdy_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_ar_channel_rsci_biwt;
  wire memory_channels_ar_channel_rsc_is_idle;
  wire memory_channels_ar_channel_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  wire [108:0] nl_memory_channels_ar_channel_rsci_idat;
  assign nl_memory_channels_ar_channel_rsci_idat = {16'b0000000000000100 , (memory_channels_ar_channel_rsci_idat[92:21])
      , 21'b110010000000000000000};
  ccs_out_buf_wait_v5 #(.rscid(32'sd14),
  .width(32'sd109),
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
      .idat(nl_memory_channels_ar_channel_rsci_idat[108:0]),
      .rdy(memory_channels_ar_channel_rsc_rdy),
      .vld(memory_channels_ar_channel_rsc_vld),
      .dat(memory_channels_ar_channel_rsc_dat),
      .is_idle(memory_channels_ar_channel_rsc_is_idle)
    );
  dense_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl
      dense_core_memory_channels_ar_channel_rsci_memory_channels_ar_channel_wait_ctrl_inst
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
//  Design Unit:    dense_core_memory_channels_b_channel_rsci
// ------------------------------------------------------------------


module dense_core_memory_channels_b_channel_rsci (
  clk, arst_n, memory_channels_b_channel_rsc_dat, memory_channels_b_channel_rsc_vld,
      memory_channels_b_channel_rsc_rdy, memory_channels_b_channel_rsci_oswt, memory_channels_b_channel_rsci_wen_comp,
      memory_channels_b_channel_rsci_ivld, memory_channels_b_channel_rsci_ivld_oreg,
      memory_channels_b_channel_rsci_wen_comp_pff, memory_channels_b_channel_rsci_oswt_pff,
      memory_channels_b_channel_rsci_ivld_oreg_pff
);
  input clk;
  input arst_n;
  input [17:0] memory_channels_b_channel_rsc_dat;
  input memory_channels_b_channel_rsc_vld;
  output memory_channels_b_channel_rsc_rdy;
  input memory_channels_b_channel_rsci_oswt;
  output memory_channels_b_channel_rsci_wen_comp;
  output memory_channels_b_channel_rsci_ivld;
  input memory_channels_b_channel_rsci_ivld_oreg;
  output memory_channels_b_channel_rsci_wen_comp_pff;
  input memory_channels_b_channel_rsci_oswt_pff;
  input memory_channels_b_channel_rsci_ivld_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_b_channel_rsci_biwt;
  wire [17:0] memory_channels_b_channel_rsci_idat;
  wire memory_channels_b_channel_rsc_is_idle;
  wire memory_channels_b_channel_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  ccs_ctrl_in_buf_wait_v4 #(.rscid(32'sd13),
  .width(32'sd18),
  .ph_clk(32'sd1),
  .ph_en(32'sd0),
  .ph_arst(32'sd0),
  .ph_srst(32'sd0)) memory_channels_b_channel_rsci (
      .clk(clk),
      .en(1'b0),
      .arst(arst_n),
      .srst(1'b1),
      .rdy(memory_channels_b_channel_rsc_rdy),
      .vld(memory_channels_b_channel_rsc_vld),
      .dat(memory_channels_b_channel_rsc_dat),
      .irdy(memory_channels_b_channel_rsci_oswt),
      .ivld(memory_channels_b_channel_rsci_ivld),
      .idat(memory_channels_b_channel_rsci_idat),
      .is_idle(memory_channels_b_channel_rsc_is_idle)
    );
  dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl_inst
      (
      .memory_channels_b_channel_rsci_iswt0(memory_channels_b_channel_rsci_oswt),
      .memory_channels_b_channel_rsci_ivld_oreg(memory_channels_b_channel_rsci_ivld_oreg),
      .memory_channels_b_channel_rsci_biwt(memory_channels_b_channel_rsci_biwt),
      .memory_channels_b_channel_rsci_biwt_pff(memory_channels_b_channel_rsci_biwt_iff),
      .memory_channels_b_channel_rsci_iswt0_pff(memory_channels_b_channel_rsci_oswt_pff),
      .memory_channels_b_channel_rsci_ivld_oreg_pff(memory_channels_b_channel_rsci_ivld_oreg_pff)
    );
  assign memory_channels_b_channel_rsci_wen_comp = (~ memory_channels_b_channel_rsci_oswt)
      | memory_channels_b_channel_rsci_biwt;
  assign memory_channels_b_channel_rsci_wen_comp_pff = (~ memory_channels_b_channel_rsci_oswt_pff)
      | memory_channels_b_channel_rsci_biwt_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_w_channel_rsci
// ------------------------------------------------------------------


module dense_core_memory_channels_w_channel_rsci (
  clk, arst_n, memory_channels_w_channel_rsc_dat, memory_channels_w_channel_rsc_vld,
      memory_channels_w_channel_rsc_rdy, memory_channels_w_channel_rsci_oswt, memory_channels_w_channel_rsci_wen_comp,
      memory_channels_w_channel_rsci_irdy, memory_channels_w_channel_rsci_irdy_oreg,
      memory_channels_w_channel_rsci_idat, memory_channels_w_channel_rsci_wen_comp_pff,
      memory_channels_w_channel_rsci_oswt_pff, memory_channels_w_channel_rsci_irdy_oreg_pff
);
  input clk;
  input arst_n;
  output [576:0] memory_channels_w_channel_rsc_dat;
  output memory_channels_w_channel_rsc_vld;
  input memory_channels_w_channel_rsc_rdy;
  input memory_channels_w_channel_rsci_oswt;
  output memory_channels_w_channel_rsci_wen_comp;
  output memory_channels_w_channel_rsci_irdy;
  input memory_channels_w_channel_rsci_irdy_oreg;
  input [576:0] memory_channels_w_channel_rsci_idat;
  output memory_channels_w_channel_rsci_wen_comp_pff;
  input memory_channels_w_channel_rsci_oswt_pff;
  input memory_channels_w_channel_rsci_irdy_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_w_channel_rsci_biwt;
  wire memory_channels_w_channel_rsc_is_idle;
  wire memory_channels_w_channel_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  wire [576:0] nl_memory_channels_w_channel_rsci_idat;
  assign nl_memory_channels_w_channel_rsci_idat = {(memory_channels_w_channel_rsci_idat[576:1])
      , 1'b1};
  ccs_out_buf_wait_v5 #(.rscid(32'sd12),
  .width(32'sd577),
  .ph_clk(32'sd1),
  .ph_en(32'sd0),
  .ph_arst(32'sd0),
  .ph_srst(32'sd0),
  .rst_val(32'sd0)) memory_channels_w_channel_rsci (
      .clk(clk),
      .en(1'b0),
      .arst(arst_n),
      .srst(1'b1),
      .irdy(memory_channels_w_channel_rsci_irdy),
      .ivld(memory_channels_w_channel_rsci_oswt),
      .idat(nl_memory_channels_w_channel_rsci_idat[576:0]),
      .rdy(memory_channels_w_channel_rsc_rdy),
      .vld(memory_channels_w_channel_rsc_vld),
      .dat(memory_channels_w_channel_rsc_dat),
      .is_idle(memory_channels_w_channel_rsc_is_idle)
    );
  dense_core_memory_channels_w_channel_rsci_memory_channels_w_channel_wait_ctrl dense_core_memory_channels_w_channel_rsci_memory_channels_w_channel_wait_ctrl_inst
      (
      .memory_channels_w_channel_rsci_iswt0(memory_channels_w_channel_rsci_oswt),
      .memory_channels_w_channel_rsci_irdy_oreg(memory_channels_w_channel_rsci_irdy_oreg),
      .memory_channels_w_channel_rsci_biwt(memory_channels_w_channel_rsci_biwt),
      .memory_channels_w_channel_rsci_biwt_pff(memory_channels_w_channel_rsci_biwt_iff),
      .memory_channels_w_channel_rsci_iswt0_pff(memory_channels_w_channel_rsci_oswt_pff),
      .memory_channels_w_channel_rsci_irdy_oreg_pff(memory_channels_w_channel_rsci_irdy_oreg_pff)
    );
  assign memory_channels_w_channel_rsci_wen_comp = (~ memory_channels_w_channel_rsci_oswt)
      | memory_channels_w_channel_rsci_biwt;
  assign memory_channels_w_channel_rsci_wen_comp_pff = (~ memory_channels_w_channel_rsci_oswt_pff)
      | memory_channels_w_channel_rsci_biwt_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_aw_channel_rsci
// ------------------------------------------------------------------


module dense_core_memory_channels_aw_channel_rsci (
  clk, arst_n, memory_channels_aw_channel_rsc_dat, memory_channels_aw_channel_rsc_vld,
      memory_channels_aw_channel_rsc_rdy, memory_channels_aw_channel_rsci_oswt, memory_channels_aw_channel_rsci_wen_comp,
      memory_channels_aw_channel_rsci_irdy, memory_channels_aw_channel_rsci_irdy_oreg,
      memory_channels_aw_channel_rsci_idat, memory_channels_aw_channel_rsci_wen_comp_pff,
      memory_channels_aw_channel_rsci_oswt_pff, memory_channels_aw_channel_rsci_irdy_oreg_pff
);
  input clk;
  input arst_n;
  output [108:0] memory_channels_aw_channel_rsc_dat;
  output memory_channels_aw_channel_rsc_vld;
  input memory_channels_aw_channel_rsc_rdy;
  input memory_channels_aw_channel_rsci_oswt;
  output memory_channels_aw_channel_rsci_wen_comp;
  output memory_channels_aw_channel_rsci_irdy;
  input memory_channels_aw_channel_rsci_irdy_oreg;
  input [108:0] memory_channels_aw_channel_rsci_idat;
  output memory_channels_aw_channel_rsci_wen_comp_pff;
  input memory_channels_aw_channel_rsci_oswt_pff;
  input memory_channels_aw_channel_rsci_irdy_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_aw_channel_rsci_biwt;
  wire memory_channels_aw_channel_rsc_is_idle;
  wire memory_channels_aw_channel_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  wire [108:0] nl_memory_channels_aw_channel_rsci_idat;
  assign nl_memory_channels_aw_channel_rsci_idat = {16'b0000000000000000 , (memory_channels_aw_channel_rsci_idat[92:29])
      , 8'b00000000 , (memory_channels_aw_channel_rsci_idat[20]) , 20'b10011000000000000000};
  ccs_out_buf_wait_v5 #(.rscid(32'sd11),
  .width(32'sd109),
  .ph_clk(32'sd1),
  .ph_en(32'sd0),
  .ph_arst(32'sd0),
  .ph_srst(32'sd0),
  .rst_val(32'sd0)) memory_channels_aw_channel_rsci (
      .clk(clk),
      .en(1'b0),
      .arst(arst_n),
      .srst(1'b1),
      .irdy(memory_channels_aw_channel_rsci_irdy),
      .ivld(memory_channels_aw_channel_rsci_oswt),
      .idat(nl_memory_channels_aw_channel_rsci_idat[108:0]),
      .rdy(memory_channels_aw_channel_rsc_rdy),
      .vld(memory_channels_aw_channel_rsc_vld),
      .dat(memory_channels_aw_channel_rsc_dat),
      .is_idle(memory_channels_aw_channel_rsc_is_idle)
    );
  dense_core_memory_channels_aw_channel_rsci_memory_channels_aw_channel_wait_ctrl
      dense_core_memory_channels_aw_channel_rsci_memory_channels_aw_channel_wait_ctrl_inst
      (
      .memory_channels_aw_channel_rsci_iswt0(memory_channels_aw_channel_rsci_oswt),
      .memory_channels_aw_channel_rsci_irdy_oreg(memory_channels_aw_channel_rsci_irdy_oreg),
      .memory_channels_aw_channel_rsci_biwt(memory_channels_aw_channel_rsci_biwt),
      .memory_channels_aw_channel_rsci_biwt_pff(memory_channels_aw_channel_rsci_biwt_iff),
      .memory_channels_aw_channel_rsci_iswt0_pff(memory_channels_aw_channel_rsci_oswt_pff),
      .memory_channels_aw_channel_rsci_irdy_oreg_pff(memory_channels_aw_channel_rsci_irdy_oreg_pff)
    );
  assign memory_channels_aw_channel_rsci_wen_comp = (~ memory_channels_aw_channel_rsci_oswt)
      | memory_channels_aw_channel_rsci_biwt;
  assign memory_channels_aw_channel_rsci_wen_comp_pff = (~ memory_channels_aw_channel_rsci_oswt_pff)
      | memory_channels_aw_channel_rsci_biwt_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_done_rsci
// ------------------------------------------------------------------


module dense_core_done_rsci (
  clk, arst_n, done_rsc_dat, done_rsc_vld, done_rsc_rdy, done_rsci_oswt, done_rsci_wen_comp,
      done_rsci_irdy, done_rsci_irdy_oreg, done_rsci_wen_comp_pff, done_rsci_oswt_pff,
      done_rsci_irdy_oreg_pff
);
  input clk;
  input arst_n;
  output done_rsc_dat;
  output done_rsc_vld;
  input done_rsc_rdy;
  input done_rsci_oswt;
  output done_rsci_wen_comp;
  output done_rsci_irdy;
  input done_rsci_irdy_oreg;
  output done_rsci_wen_comp_pff;
  input done_rsci_oswt_pff;
  input done_rsci_irdy_oreg_pff;


  // Interconnect Declarations
  wire done_rsci_biwt;
  wire done_rsc_is_idle;
  wire done_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  ccs_out_buf_wait_v5 #(.rscid(32'sd2),
  .width(32'sd1),
  .ph_clk(32'sd1),
  .ph_en(32'sd0),
  .ph_arst(32'sd0),
  .ph_srst(32'sd0),
  .rst_val(32'sd0)) done_rsci (
      .clk(clk),
      .en(1'b0),
      .arst(arst_n),
      .srst(1'b1),
      .irdy(done_rsci_irdy),
      .ivld(done_rsci_oswt),
      .idat(1'b1),
      .rdy(done_rsc_rdy),
      .vld(done_rsc_vld),
      .dat(done_rsc_dat),
      .is_idle(done_rsc_is_idle)
    );
  dense_core_done_rsci_done_wait_ctrl dense_core_done_rsci_done_wait_ctrl_inst (
      .done_rsci_iswt0(done_rsci_oswt),
      .done_rsci_irdy_oreg(done_rsci_irdy_oreg),
      .done_rsci_biwt(done_rsci_biwt),
      .done_rsci_biwt_pff(done_rsci_biwt_iff),
      .done_rsci_iswt0_pff(done_rsci_oswt_pff),
      .done_rsci_irdy_oreg_pff(done_rsci_irdy_oreg_pff)
    );
  assign done_rsci_wen_comp = (~ done_rsci_oswt) | done_rsci_biwt;
  assign done_rsci_wen_comp_pff = (~ done_rsci_oswt_pff) | done_rsci_biwt_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_start_rsci
// ------------------------------------------------------------------


module dense_core_start_rsci (
  clk, arst_n, start_rsc_dat, start_rsc_vld, start_rsc_rdy, start_rsci_oswt, start_rsci_wen_comp,
      start_rsci_ivld, start_rsci_ivld_oreg, start_rsci_wen_comp_pff, start_rsci_oswt_pff,
      start_rsci_ivld_oreg_pff
);
  input clk;
  input arst_n;
  input start_rsc_dat;
  input start_rsc_vld;
  output start_rsc_rdy;
  input start_rsci_oswt;
  output start_rsci_wen_comp;
  output start_rsci_ivld;
  input start_rsci_ivld_oreg;
  output start_rsci_wen_comp_pff;
  input start_rsci_oswt_pff;
  input start_rsci_ivld_oreg_pff;


  // Interconnect Declarations
  wire start_rsci_biwt;
  wire start_rsci_idat;
  wire start_rsc_is_idle;
  wire start_rsci_biwt_iff;


  // Interconnect Declarations for Component Instantiations 
  ccs_ctrl_in_buf_wait_v4 #(.rscid(32'sd1),
  .width(32'sd1),
  .ph_clk(32'sd1),
  .ph_en(32'sd0),
  .ph_arst(32'sd0),
  .ph_srst(32'sd0)) start_rsci (
      .clk(clk),
      .en(1'b0),
      .arst(arst_n),
      .srst(1'b1),
      .rdy(start_rsc_rdy),
      .vld(start_rsc_vld),
      .dat(start_rsc_dat),
      .irdy(start_rsci_oswt),
      .ivld(start_rsci_ivld),
      .idat(start_rsci_idat),
      .is_idle(start_rsc_is_idle)
    );
  dense_core_start_rsci_start_wait_ctrl dense_core_start_rsci_start_wait_ctrl_inst
      (
      .start_rsci_iswt0(start_rsci_oswt),
      .start_rsci_ivld_oreg(start_rsci_ivld_oreg),
      .start_rsci_biwt(start_rsci_biwt),
      .start_rsci_biwt_pff(start_rsci_biwt_iff),
      .start_rsci_iswt0_pff(start_rsci_oswt_pff),
      .start_rsci_ivld_oreg_pff(start_rsci_ivld_oreg_pff)
    );
  assign start_rsci_wen_comp = (~ start_rsci_oswt) | start_rsci_biwt;
  assign start_rsci_wen_comp_pff = (~ start_rsci_oswt_pff) | start_rsci_biwt_iff;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core
// ------------------------------------------------------------------


module dense_core (
  clk, arst_n, start_rsc_dat, start_rsc_vld, start_rsc_rdy, done_rsc_dat, done_rsc_vld,
      done_rsc_rdy, use_relu_triosy_lz, addr_hi_rsc_dat, addr_hi_triosy_lz, feature_addr_lo_rsc_dat,
      feature_addr_lo_triosy_lz, weight_addr_lo_rsc_dat, weight_addr_lo_triosy_lz,
      output_addr_lo_rsc_dat, output_addr_lo_triosy_lz, input_vector_len_rsc_dat,
      input_vector_len_triosy_lz, output_vector_len_rsc_dat, output_vector_len_triosy_lz,
      debug_rsc_dat, debug_triosy_lz, memory_channels_aw_channel_rsc_dat, memory_channels_aw_channel_rsc_vld,
      memory_channels_aw_channel_rsc_rdy, memory_channels_w_channel_rsc_dat, memory_channels_w_channel_rsc_vld,
      memory_channels_w_channel_rsc_rdy, memory_channels_b_channel_rsc_dat, memory_channels_b_channel_rsc_vld,
      memory_channels_b_channel_rsc_rdy, memory_channels_ar_channel_rsc_dat, memory_channels_ar_channel_rsc_vld,
      memory_channels_ar_channel_rsc_rdy, memory_channels_r_channel_rsc_dat, memory_channels_r_channel_rsc_vld,
      memory_channels_r_channel_rsc_rdy, feature_memory_rsci_clken_d, feature_memory_rsci_q_d,
      feature_memory_rsci_radr_d, feature_memory_rsci_wadr_d, weight_memory_rsci_q_d,
      weight_memory_rsci_radr_d, weight_memory_rsci_wadr_d, while_for_1_for_32_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_32_while_for_1_for_acc_2_cmp_b, while_for_1_for_32_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_31_while_for_1_for_acc_2_cmp_a, while_for_1_for_31_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_31_while_for_1_for_acc_2_cmp_z, while_for_1_for_30_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_30_while_for_1_for_acc_2_cmp_b, while_for_1_for_30_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_29_while_for_1_for_acc_2_cmp_a, while_for_1_for_29_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_29_while_for_1_for_acc_2_cmp_z, while_for_1_for_28_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_28_while_for_1_for_acc_2_cmp_b, while_for_1_for_28_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_27_while_for_1_for_acc_2_cmp_a, while_for_1_for_27_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_27_while_for_1_for_acc_2_cmp_z, while_for_1_for_26_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_26_while_for_1_for_acc_2_cmp_b, while_for_1_for_26_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_25_while_for_1_for_acc_2_cmp_a, while_for_1_for_25_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_25_while_for_1_for_acc_2_cmp_z, while_for_1_for_24_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_24_while_for_1_for_acc_2_cmp_b, while_for_1_for_24_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_23_while_for_1_for_acc_2_cmp_a, while_for_1_for_23_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_23_while_for_1_for_acc_2_cmp_z, while_for_1_for_22_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_22_while_for_1_for_acc_2_cmp_b, while_for_1_for_22_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_21_while_for_1_for_acc_2_cmp_a, while_for_1_for_21_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_21_while_for_1_for_acc_2_cmp_z, while_for_1_for_20_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_20_while_for_1_for_acc_2_cmp_b, while_for_1_for_20_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_19_while_for_1_for_acc_2_cmp_a, while_for_1_for_19_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_19_while_for_1_for_acc_2_cmp_z, while_for_1_for_18_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_18_while_for_1_for_acc_2_cmp_b, while_for_1_for_18_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_17_while_for_1_for_acc_2_cmp_a, while_for_1_for_17_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_17_while_for_1_for_acc_2_cmp_z, while_for_1_for_16_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_16_while_for_1_for_acc_2_cmp_b, while_for_1_for_16_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_15_while_for_1_for_acc_2_cmp_a, while_for_1_for_15_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_15_while_for_1_for_acc_2_cmp_z, while_for_1_for_14_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_14_while_for_1_for_acc_2_cmp_b, while_for_1_for_14_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_13_while_for_1_for_acc_2_cmp_a, while_for_1_for_13_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_13_while_for_1_for_acc_2_cmp_z, while_for_1_for_12_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_12_while_for_1_for_acc_2_cmp_b, while_for_1_for_12_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_11_while_for_1_for_acc_2_cmp_a, while_for_1_for_11_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_11_while_for_1_for_acc_2_cmp_z, while_for_1_for_10_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_10_while_for_1_for_acc_2_cmp_b, while_for_1_for_10_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_9_while_for_1_for_acc_2_cmp_a, while_for_1_for_9_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_9_while_for_1_for_acc_2_cmp_z, while_for_1_for_8_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_8_while_for_1_for_acc_2_cmp_b, while_for_1_for_8_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_7_while_for_1_for_acc_2_cmp_a, while_for_1_for_7_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_7_while_for_1_for_acc_2_cmp_z, while_for_1_for_6_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_6_while_for_1_for_acc_2_cmp_b, while_for_1_for_6_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_5_while_for_1_for_acc_2_cmp_a, while_for_1_for_5_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_5_while_for_1_for_acc_2_cmp_z, while_for_1_for_4_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_4_while_for_1_for_acc_2_cmp_b, while_for_1_for_4_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_3_while_for_1_for_acc_2_cmp_a, while_for_1_for_3_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_3_while_for_1_for_acc_2_cmp_z, while_for_1_for_2_while_for_1_for_acc_2_cmp_a,
      while_for_1_for_2_while_for_1_for_acc_2_cmp_b, while_for_1_for_2_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_1_while_for_1_for_acc_2_cmp_a, while_for_1_for_1_while_for_1_for_acc_2_cmp_b,
      while_for_1_for_1_while_for_1_for_acc_2_cmp_load, while_for_1_for_1_while_for_1_for_acc_2_cmp_z,
      while_for_1_for_32_while_for_1_for_acc_2_cmp_load_pff, while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_pff,
      feature_memory_rsci_d_d_pff, feature_memory_rsci_re_d_pff, feature_memory_rsci_we_d_pff,
      weight_memory_rsci_we_d_pff
);
  input clk;
  input arst_n;
  input start_rsc_dat;
  input start_rsc_vld;
  output start_rsc_rdy;
  output done_rsc_dat;
  output done_rsc_vld;
  input done_rsc_rdy;
  output use_relu_triosy_lz;
  input [31:0] addr_hi_rsc_dat;
  output addr_hi_triosy_lz;
  input [31:0] feature_addr_lo_rsc_dat;
  output feature_addr_lo_triosy_lz;
  input [31:0] weight_addr_lo_rsc_dat;
  output weight_addr_lo_triosy_lz;
  input [31:0] output_addr_lo_rsc_dat;
  output output_addr_lo_triosy_lz;
  input [31:0] input_vector_len_rsc_dat;
  output input_vector_len_triosy_lz;
  input [31:0] output_vector_len_rsc_dat;
  output output_vector_len_triosy_lz;
  output [31:0] debug_rsc_dat;
  output debug_triosy_lz;
  output [108:0] memory_channels_aw_channel_rsc_dat;
  output memory_channels_aw_channel_rsc_vld;
  input memory_channels_aw_channel_rsc_rdy;
  output [576:0] memory_channels_w_channel_rsc_dat;
  output memory_channels_w_channel_rsc_vld;
  input memory_channels_w_channel_rsc_rdy;
  input [17:0] memory_channels_b_channel_rsc_dat;
  input memory_channels_b_channel_rsc_vld;
  output memory_channels_b_channel_rsc_rdy;
  output [108:0] memory_channels_ar_channel_rsc_dat;
  output memory_channels_ar_channel_rsc_vld;
  input memory_channels_ar_channel_rsc_rdy;
  input [530:0] memory_channels_r_channel_rsc_dat;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;
  output feature_memory_rsci_clken_d;
  input [511:0] feature_memory_rsci_q_d;
  output [14:0] feature_memory_rsci_radr_d;
  output [14:0] feature_memory_rsci_wadr_d;
  input [511:0] weight_memory_rsci_q_d;
  output [7:0] weight_memory_rsci_radr_d;
  output [7:0] weight_memory_rsci_wadr_d;
  output [15:0] while_for_1_for_32_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_32_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_32_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_31_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_31_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_31_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_30_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_30_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_30_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_29_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_29_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_29_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_28_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_28_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_28_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_27_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_27_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_27_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_26_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_26_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_26_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_25_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_25_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_25_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_24_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_24_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_24_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_23_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_23_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_23_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_22_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_22_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_22_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_21_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_21_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_21_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_20_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_20_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_20_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_19_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_19_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_19_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_18_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_18_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_18_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_17_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_17_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_17_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_16_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_16_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_16_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_15_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_15_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_15_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_14_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_14_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_14_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_13_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_13_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_13_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_12_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_12_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_12_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_11_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_11_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_11_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_10_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_10_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_10_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_9_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_9_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_9_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_8_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_8_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_8_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_7_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_7_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_7_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_6_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_6_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_6_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_5_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_5_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_5_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_4_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_4_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_4_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_3_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_3_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_3_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_2_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_2_while_for_1_for_acc_2_cmp_b;
  input [31:0] while_for_1_for_2_while_for_1_for_acc_2_cmp_z;
  output [15:0] while_for_1_for_1_while_for_1_for_acc_2_cmp_a;
  output [15:0] while_for_1_for_1_while_for_1_for_acc_2_cmp_b;
  output while_for_1_for_1_while_for_1_for_acc_2_cmp_load;
  input [31:0] while_for_1_for_1_while_for_1_for_acc_2_cmp_z;
  output while_for_1_for_32_while_for_1_for_acc_2_cmp_load_pff;
  output while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_pff;
  output [511:0] feature_memory_rsci_d_d_pff;
  output feature_memory_rsci_re_d_pff;
  output feature_memory_rsci_we_d_pff;
  output weight_memory_rsci_we_d_pff;


  // Interconnect Declarations
  reg core_wen;
  wire core_wten;
  wire start_rsci_wen_comp;
  wire start_rsci_ivld;
  wire start_rsci_ivld_oreg;
  wire done_rsci_wen_comp;
  wire done_rsci_irdy;
  wire done_rsci_irdy_oreg;
  wire [31:0] addr_hi_rsci_idat;
  wire [31:0] feature_addr_lo_rsci_idat;
  wire [31:0] weight_addr_lo_rsci_idat;
  wire [31:0] output_addr_lo_rsci_idat;
  wire [31:0] input_vector_len_rsci_idat;
  wire [31:0] output_vector_len_rsci_idat;
  wire memory_channels_aw_channel_rsci_wen_comp;
  wire memory_channels_aw_channel_rsci_irdy;
  wire memory_channels_aw_channel_rsci_irdy_oreg;
  wire memory_channels_w_channel_rsci_wen_comp;
  wire memory_channels_w_channel_rsci_irdy;
  wire memory_channels_w_channel_rsci_irdy_oreg;
  wire memory_channels_b_channel_rsci_wen_comp;
  wire memory_channels_b_channel_rsci_ivld;
  wire memory_channels_b_channel_rsci_ivld_oreg;
  wire memory_channels_ar_channel_rsci_wen_comp;
  wire memory_channels_ar_channel_rsci_irdy;
  wire memory_channels_ar_channel_rsci_irdy_oreg;
  wire memory_channels_r_channel_rsci_wen_comp;
  wire memory_channels_r_channel_rsci_ivld;
  wire memory_channels_r_channel_rsci_ivld_oreg;
  wire [513:0] memory_channels_r_channel_rsci_idat_mxwt;
  reg [15:0] debug_rsci_idat_15_0;
  reg [57:0] memory_channels_aw_channel_rsci_idat_92_35;
  reg [3:0] memory_channels_aw_channel_rsci_idat_34_31;
  reg [1:0] memory_channels_aw_channel_rsci_idat_30_29;
  reg [511:0] memory_channels_w_channel_rsci_idat_576_65;
  reg [63:0] memory_channels_w_channel_rsci_idat_64_1;
  reg [13:0] memory_channels_ar_channel_rsci_idat_42_29;
  reg [7:0] memory_channels_ar_channel_rsci_idat_28_21;
  reg memory_channels_aw_channel_rsci_idat_20;
  reg [25:0] memory_channels_ar_channel_rsci_idat_68_43;
  wire [23:0] fsm_output;
  wire memory_axi_burst_read_base_axi_u512_512_1_for_memory_axi_burst_read_base_axi_u512_512_1_for_if_or_tmp;
  wire operator_16_false_operator_16_false_nor_tmp;
  wire or_dcpl;
  wire or_dcpl_41;
  wire and_dcpl_42;
  wire or_dcpl_81;
  wire or_tmp_40;
  wire and_143_cse;
  wire and_134_cse;
  wire and_135_cse;
  wire and_133_cse;
  wire and_138_cse;
  reg exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1;
  reg [31:0] sum_sva;
  reg [31:0] weight_index_lpi_3;
  wire xor_cse;
  wire exit_for_sva_mx0;
  reg [15:0] out_index_sva;
  reg memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1;
  reg memory_axi_burst_read_base_axi_u512_512_for_stage_0_2;
  wire [8:0] memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3;
  wire [9:0] nl_memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3;
  wire [24:0] while_sum_out_slc_32_8_sat_sva_1;
  wire [25:0] nl_while_sum_out_slc_32_8_sat_sva_1;
  wire memory_send_ar_and_cse;
  wire memory_send_aw_and_1_cse;
  wire memory_send_w_and_cse;
  wire output_vector_len_and_cse;
  wire sum_array_and_cse;
  wire and_25_cse;
  wire nand_cse;
  wire for_i_and_ssc;
  reg [1:0] for_i_31_14_sva_17_16;
  reg [15:0] for_i_31_14_sva_15_0;
  wire [32:0] acc_1_sdt;
  wire [33:0] nl_acc_1_sdt;
  wire and_745_ssc;
  reg [9:0] memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva;
  reg sum_array_31_sva_load;
  wire core_wen_rtff;
  reg reg_start_rsci_oswt_tmp;
  reg reg_done_rsci_oswt_tmp;
  reg reg_memory_channels_aw_channel_rsci_oswt_tmp;
  reg reg_memory_channels_w_channel_rsci_oswt_tmp;
  reg reg_memory_channels_b_channel_rsci_oswt_tmp;
  reg reg_memory_channels_ar_channel_rsci_oswt_tmp;
  reg reg_memory_channels_r_channel_rsci_oswt_tmp;
  wire start_rsci_wen_comp_iff;
  wire mux_rmff;
  wire done_rsci_wen_comp_iff;
  wire mux_4_rmff;
  wire memory_channels_aw_channel_rsci_wen_comp_iff;
  wire memory_send_aw_mux_rmff;
  wire memory_channels_w_channel_rsci_wen_comp_iff;
  wire memory_send_w_mux_rmff;
  wire memory_channels_b_channel_rsci_wen_comp_iff;
  wire memory_get_b_mux_rmff;
  wire memory_channels_ar_channel_rsci_wen_comp_iff;
  wire memory_send_ar_mux_rmff;
  wire memory_channels_r_channel_rsci_wen_comp_iff;
  wire memory_get_r_mux_rmff;
  wire or_111_cse;
  wire or_108_cse;
  wire or_109_cse;
  wire while_for_1_for_or_cse;
  wire sum_array_or_32_cse;
  wire output_line_and_itm;
  wire [13:0] while_sum_out_while_sum_out_nor_itm;
  wire while_sum_out_while_sum_out_nor_1_itm;
  wire while_and_stg_1_3;
  wire while_and_stg_2_7;
  wire while_and_stg_3_15;
  wire while_and_stg_1_2;
  wire while_and_stg_2_6;
  wire while_and_stg_3_14;
  wire while_and_stg_1_1;
  wire while_and_stg_2_5;
  wire while_and_stg_3_13;
  wire while_and_stg_1_0;
  wire while_and_stg_2_4;
  wire while_and_stg_3_12;
  wire while_and_stg_2_3;
  wire while_and_stg_3_11;
  wire while_and_stg_2_2;
  wire while_and_stg_3_10;
  wire while_and_stg_2_1;
  wire while_and_stg_3_9;
  wire while_and_stg_2_0;
  wire while_and_stg_3_8;
  wire while_and_stg_3_7;
  wire while_and_stg_3_6;
  wire while_and_stg_3_5;
  wire while_and_stg_3_4;
  wire while_and_stg_3_3;
  wire while_and_stg_3_2;
  wire while_and_stg_3_1;
  wire while_and_stg_3_0;
  wire [63:0] z_out;
  wire [511:0] z_out_1;
  wire [15:0] z_out_2;
  wire [17:0] nl_z_out_2;
  wire [7:0] z_out_3;
  wire [8:0] nl_z_out_3;
  reg [31:0] output_vector_len_sva;
  reg [32:0] acc_psp_sva;
  wire [33:0] nl_acc_psp_sva;
  reg [15:0] num_feature_lines_sva;
  reg operator_16_false_slc_operator_16_false_acc_8_mdf_sva;
  reg [31:0] sum_array_15_sva;
  reg [31:0] sum_array_16_sva;
  reg [31:0] sum_array_14_sva;
  reg [31:0] sum_array_17_sva;
  reg [31:0] sum_array_13_sva;
  reg [31:0] sum_array_18_sva;
  reg [31:0] sum_array_12_sva;
  reg [31:0] sum_array_19_sva;
  reg [31:0] sum_array_11_sva;
  reg [31:0] sum_array_20_sva;
  reg [31:0] sum_array_10_sva;
  reg [31:0] sum_array_21_sva;
  reg [31:0] sum_array_9_sva;
  reg [31:0] sum_array_22_sva;
  reg [31:0] sum_array_8_sva;
  reg [31:0] sum_array_23_sva;
  reg [31:0] sum_array_7_sva;
  reg [31:0] sum_array_24_sva;
  reg [31:0] sum_array_6_sva;
  reg [31:0] sum_array_25_sva;
  reg [31:0] sum_array_5_sva;
  reg [31:0] sum_array_26_sva;
  reg [31:0] sum_array_4_sva;
  reg [31:0] sum_array_27_sva;
  reg [31:0] sum_array_3_sva;
  reg [31:0] sum_array_28_sva;
  reg [31:0] sum_array_2_sva;
  reg [31:0] sum_array_29_sva;
  reg [31:0] sum_array_1_sva;
  reg [31:0] sum_array_30_sva;
  reg [31:0] sum_array_31_sva;
  reg [63:0] memory_encode_strb_1_if_6_lshift_itm;
  wire debug_rsci_idat_15_0_mx0c1;
  wire out_index_sva_mx0c0;
  wire for_i_31_14_sva_mx0c2;
  wire [17:0] for_i_31_14_sva_2;
  wire [18:0] nl_for_i_31_14_sva_2;
  wire acc_1_psp_sva_mx0c1;
  wire sum_sva_mx0c1;
  wire [7:0] for_feature_burst_size_qr_7_0_lpi_2_dfm_1;
  wire for_feature_burst_size_qr_7_0_lpi_2_dfm_mx0c1;
  wire operator_16_false_slc_operator_16_false_acc_8_mdf_sva_mx0w0;
  wire exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1_mx0w3;
  wire [9:0] memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva_mx1;
  wire memory_axi_burst_read_base_axi_u512_512_for_stage_0_2_mx0w1;
  wire while_sum_out_nor_ovfl_sva_1;
  wire while_sum_out_and_unfl_sva_1;
  reg acc_1_psp_sva_32;
  reg [31:0] acc_1_psp_sva_31_0;
  reg output_line_sva_1_511;
  reg [13:0] output_line_sva_1_510_497;
  reg output_line_sva_1_496;
  reg output_line_sva_1_495;
  reg [13:0] output_line_sva_1_494_481;
  reg output_line_sva_1_480;
  reg output_line_sva_1_479;
  reg [13:0] output_line_sva_1_478_465;
  reg output_line_sva_1_464;
  reg output_line_sva_1_463;
  reg [13:0] output_line_sva_1_462_449;
  reg output_line_sva_1_448;
  reg output_line_sva_1_447;
  reg [13:0] output_line_sva_1_446_433;
  reg output_line_sva_1_432;
  reg output_line_sva_1_431;
  reg [13:0] output_line_sva_1_430_417;
  reg output_line_sva_1_416;
  reg output_line_sva_1_415;
  reg [13:0] output_line_sva_1_414_401;
  reg output_line_sva_1_400;
  reg output_line_sva_1_399;
  reg [13:0] output_line_sva_1_398_385;
  reg output_line_sva_1_384;
  reg output_line_sva_1_383;
  reg [13:0] output_line_sva_1_382_369;
  reg output_line_sva_1_368;
  reg output_line_sva_1_367;
  reg [13:0] output_line_sva_1_366_353;
  reg output_line_sva_1_352;
  reg output_line_sva_1_351;
  reg [13:0] output_line_sva_1_350_337;
  reg output_line_sva_1_336;
  reg output_line_sva_1_335;
  reg [13:0] output_line_sva_1_334_321;
  reg output_line_sva_1_320;
  reg output_line_sva_1_319;
  reg [13:0] output_line_sva_1_318_305;
  reg output_line_sva_1_304;
  reg output_line_sva_1_303;
  reg [13:0] output_line_sva_1_302_289;
  reg output_line_sva_1_288;
  reg output_line_sva_1_287;
  reg [13:0] output_line_sva_1_286_273;
  reg output_line_sva_1_272;
  reg output_line_sva_1_271;
  reg [13:0] output_line_sva_1_270_257;
  reg output_line_sva_1_256;
  reg output_line_sva_1_255;
  reg [13:0] output_line_sva_1_254_241;
  reg output_line_sva_1_240;
  reg output_line_sva_1_239;
  reg [13:0] output_line_sva_1_238_225;
  reg output_line_sva_1_224;
  reg output_line_sva_1_223;
  reg [13:0] output_line_sva_1_222_209;
  reg output_line_sva_1_208;
  reg output_line_sva_1_207;
  reg [13:0] output_line_sva_1_206_193;
  reg output_line_sva_1_192;
  reg output_line_sva_1_191;
  reg [13:0] output_line_sva_1_190_177;
  reg output_line_sva_1_176;
  reg output_line_sva_1_175;
  reg [13:0] output_line_sva_1_174_161;
  reg output_line_sva_1_160;
  reg output_line_sva_1_159;
  reg [13:0] output_line_sva_1_158_145;
  reg output_line_sva_1_144;
  reg output_line_sva_1_143;
  reg [13:0] output_line_sva_1_142_129;
  reg output_line_sva_1_128;
  reg output_line_sva_1_127;
  reg [13:0] output_line_sva_1_126_113;
  reg output_line_sva_1_112;
  reg output_line_sva_1_111;
  reg [13:0] output_line_sva_1_110_97;
  reg output_line_sva_1_96;
  reg output_line_sva_1_95;
  reg [13:0] output_line_sva_1_94_81;
  reg output_line_sva_1_80;
  reg output_line_sva_1_79;
  reg [13:0] output_line_sva_1_78_65;
  reg output_line_sva_1_64;
  reg output_line_sva_1_63;
  reg [13:0] output_line_sva_1_62_49;
  reg output_line_sva_1_48;
  reg output_line_sva_1_47;
  reg [13:0] output_line_sva_1_46_33;
  reg output_line_sva_1_32;
  reg output_line_sva_1_31;
  reg [13:0] output_line_sva_1_30_17;
  reg output_line_sva_1_16;
  reg output_line_sva_1_15;
  reg [13:0] output_line_sva_1_14_1;
  reg output_line_sva_1_0;
  wire for_nor_1_cse;
  wire id_or_cse;
  wire output_line_and_1_cse;
  wire output_line_and_4_cse;
  wire output_line_and_7_cse;
  wire output_line_and_10_cse;
  wire output_line_and_13_cse;
  wire output_line_and_16_cse;
  wire output_line_and_19_cse;
  wire output_line_and_22_cse;
  wire output_line_and_25_cse;
  wire output_line_and_28_cse;
  wire output_line_and_31_cse;
  wire output_line_and_34_cse;
  wire output_line_and_37_cse;
  wire output_line_and_40_cse;
  wire output_line_and_43_cse;
  wire output_line_and_46_cse;
  wire output_line_and_49_cse;
  wire output_line_and_52_cse;
  wire output_line_and_55_cse;
  wire output_line_and_58_cse;
  wire output_line_and_61_cse;
  wire output_line_and_64_cse;
  wire output_line_and_67_cse;
  wire output_line_and_70_cse;
  wire output_line_and_73_cse;
  wire output_line_and_76_cse;
  wire output_line_and_79_cse;
  wire output_line_and_82_cse;
  wire output_line_and_85_cse;
  wire output_line_and_88_cse;
  wire output_line_and_91_cse;
  wire output_line_and_94_cse;
  wire [32:0] acc_2_sdt;
  wire [33:0] nl_acc_2_sdt;
  wire output_address_and_ssc;
  reg [57:0] output_address_sva_63_6;
  reg [5:0] output_address_sva_5_0;
  wire output_address_and_1_rgt;
  wire operator_16_false_1_acc_itm_10_1_10_1_10;
  wire for_feature_burst_size_qelse_and_ssc;
  reg [1:0] reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd;
  reg reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1;
  reg [4:0] reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2;
  wire for_or_4_seb;
  wire for_for_or_cse;

  wire or_107_nl;
  wire or_110_nl;
  wire or_112_nl;
  wire[19:0] operator_64_false_acc_nl;
  wire[20:0] nl_operator_64_false_acc_nl;
  wire[25:0] while_for_1_if_acc_nl;
  wire[26:0] nl_while_for_1_if_acc_nl;
  wire[3:0] for_1_i_mux_nl;
  wire memory_send_aw_not_4_nl;
  wire memory_send_aw_not_5_nl;
  wire[57:0] operator_64_false_acc_nl_1;
  wire[58:0] nl_operator_64_false_acc_nl_1;
  wire out_index_not_1_nl;
  wire not_229_nl;
  wire[15:0] for_i_for_i_mux1h_nl;
  wire for_i_and_1_nl;
  wire nor_nl;
  wire[31:0] while_for_while_for_and_nl;
  wire[31:0] while_for_mux_nl;
  wire[31:0] while_for_1_acc_1_nl;
  wire[32:0] nl_while_for_1_acc_1_nl;
  wire[31:0] sum_mux_nl;
  wire[31:0] while_for_2_acc_1_nl;
  wire[32:0] nl_while_for_2_acc_1_nl;
  wire[31:0] while_for_2_mux_nl;
  wire sum_not_1_nl;
  wire[1:0] for_feature_burst_size_qelse_for_feature_burst_size_qelse_mux_nl;
  wire not_222_nl;
  wire for_feature_burst_size_qelse_mux1h_5_nl;
  wire[4:0] for_feature_burst_size_qelse_mux1h_6_nl;
  wire[4:0] for_1_i_and_nl;
  wire[4:0] for_1_i_for_1_i_mux_nl;
  wire or_436_nl;
  wire not_241_nl;
  wire operator_16_false_mux1h_nl;
  wire memory_axi_burst_read_base_axi_u512_512_for_mux_nl;
  wire[13:0] while_sum_out_nor_2_nl;
  wire[10:0] operator_16_false_1_acc_nl;
  wire[11:0] nl_operator_16_false_1_acc_nl;
  wire[9:0] memory_axi_burst_read_base_axi_u512_512_for_acc_nl;
  wire[10:0] nl_memory_axi_burst_read_base_axi_u512_512_for_acc_nl;
  wire[15:0] for_mux1h_nl;
  wire for_or_nl;
  wire for_for_and_nl;
  wire[1:0] for_for_for_nor_nl;
  wire for_for_for_nor_1_nl;
  wire[3:0] for_for_for_nor_2_nl;
  wire for_for_or_1_nl;
  wire for_mux_1_nl;
  wire operator_33_true_1_or_nl;
  wire[2:0] operator_16_false_operator_16_false_and_nl;
  wire operator_16_false_nor_nl;
  wire operator_16_false_operator_16_false_and_1_nl;
  wire operator_16_false_mux_nl;
  wire[3:0] operator_16_false_mux1h_3_nl;

  // Interconnect Declarations for Component Instantiations 
  wire [31:0] nl_debug_rsci_idat;
  assign nl_debug_rsci_idat = {16'b0000000000000000 , debug_rsci_idat_15_0};
  wire [63:0] nl_memory_encode_strb_1_if_6_lshift_rg_a;
  assign nl_memory_encode_strb_1_if_6_lshift_rg_a = signext_64_5({(fsm_output[19])
      , 4'b1111});
  wire[3:0] memory_encode_strb_if_2_mux_nl;
  wire[1:0] memory_encode_strb_if_2_memory_encode_strb_if_2_and_nl;
  wire [5:0] nl_memory_encode_strb_1_if_6_lshift_rg_s;
  assign memory_encode_strb_if_2_mux_nl = MUX_v_4_2_2(memory_channels_aw_channel_rsci_idat_34_31,
      (output_address_sva_5_0[5:2]), fsm_output[19]);
  assign memory_encode_strb_if_2_memory_encode_strb_if_2_and_nl = MUX_v_2_2_2(2'b00,
      (output_address_sva_5_0[1:0]), (fsm_output[19]));
  assign nl_memory_encode_strb_1_if_6_lshift_rg_s = {memory_encode_strb_if_2_mux_nl
      , memory_encode_strb_if_2_memory_encode_strb_if_2_and_nl};
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_1_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_2_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_3_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_4_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_5_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_6_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_7_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_8_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_9_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_10_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_11_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_12_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_13_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_14_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_15_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_16_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_17_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_18_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_19_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_20_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_21_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_22_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_23_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_24_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_25_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_26_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_27_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_28_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_29_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_30_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_31_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_32_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_33_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_34_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_35_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_36_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_37_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_38_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_39_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_40_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_41_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_42_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_43_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_44_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_45_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_46_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_47_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_48_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_49_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_50_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_51_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_52_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_53_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_54_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_55_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_56_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_57_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_58_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_59_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_60_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_61_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_62_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_63_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_64_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_65_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_66_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_67_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_68_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_69_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_70_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_71_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_72_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_73_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_74_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_75_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_76_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_77_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_78_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_79_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_80_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_81_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_82_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_83_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_84_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_85_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_86_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_87_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_88_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_89_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_90_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_91_nl;
  wire[13:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_92_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_93_nl;
  wire memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_94_nl;
  wire[10:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_95_nl;
  wire[2:0] memory_axi_write_base_axi_32_32_mux_nl;
  wire memory_axi_write_base_axi_32_32_mux_4_nl;
  wire [511:0] nl_memory_axi_write_base_axi_32_32_lshift_rg_a;
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_1_nl
      = output_line_sva_1_511 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_2_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_510_497, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_3_nl
      = output_line_sva_1_496 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_4_nl
      = output_line_sva_1_495 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_5_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_494_481, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_6_nl
      = output_line_sva_1_480 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_7_nl
      = output_line_sva_1_479 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_8_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_478_465, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_9_nl
      = output_line_sva_1_464 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_10_nl
      = output_line_sva_1_463 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_11_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_462_449, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_12_nl
      = output_line_sva_1_448 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_13_nl
      = output_line_sva_1_447 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_14_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_446_433, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_15_nl
      = output_line_sva_1_432 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_16_nl
      = output_line_sva_1_431 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_17_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_430_417, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_18_nl
      = output_line_sva_1_416 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_19_nl
      = output_line_sva_1_415 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_20_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_414_401, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_21_nl
      = output_line_sva_1_400 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_22_nl
      = output_line_sva_1_399 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_23_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_398_385, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_24_nl
      = output_line_sva_1_384 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_25_nl
      = output_line_sva_1_383 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_26_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_382_369, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_27_nl
      = output_line_sva_1_368 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_28_nl
      = output_line_sva_1_367 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_29_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_366_353, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_30_nl
      = output_line_sva_1_352 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_31_nl
      = output_line_sva_1_351 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_32_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_350_337, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_33_nl
      = output_line_sva_1_336 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_34_nl
      = output_line_sva_1_335 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_35_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_334_321, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_36_nl
      = output_line_sva_1_320 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_37_nl
      = output_line_sva_1_319 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_38_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_318_305, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_39_nl
      = output_line_sva_1_304 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_40_nl
      = output_line_sva_1_303 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_41_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_302_289, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_42_nl
      = output_line_sva_1_288 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_43_nl
      = output_line_sva_1_287 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_44_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_286_273, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_45_nl
      = output_line_sva_1_272 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_46_nl
      = output_line_sva_1_271 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_47_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_270_257, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_48_nl
      = output_line_sva_1_256 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_49_nl
      = output_line_sva_1_255 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_50_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_254_241, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_51_nl
      = output_line_sva_1_240 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_52_nl
      = output_line_sva_1_239 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_53_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_238_225, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_54_nl
      = output_line_sva_1_224 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_55_nl
      = output_line_sva_1_223 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_56_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_222_209, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_57_nl
      = output_line_sva_1_208 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_58_nl
      = output_line_sva_1_207 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_59_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_206_193, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_60_nl
      = output_line_sva_1_192 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_61_nl
      = output_line_sva_1_191 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_62_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_190_177, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_63_nl
      = output_line_sva_1_176 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_64_nl
      = output_line_sva_1_175 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_65_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_174_161, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_66_nl
      = output_line_sva_1_160 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_67_nl
      = output_line_sva_1_159 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_68_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_158_145, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_69_nl
      = output_line_sva_1_144 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_70_nl
      = output_line_sva_1_143 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_71_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_142_129, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_72_nl
      = output_line_sva_1_128 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_73_nl
      = output_line_sva_1_127 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_74_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_126_113, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_75_nl
      = output_line_sva_1_112 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_76_nl
      = output_line_sva_1_111 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_77_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_110_97, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_78_nl
      = output_line_sva_1_96 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_79_nl
      = output_line_sva_1_95 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_80_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_94_81, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_81_nl
      = output_line_sva_1_80 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_82_nl
      = output_line_sva_1_79 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_83_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_78_65, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_84_nl
      = output_line_sva_1_64 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_85_nl
      = output_line_sva_1_63 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_86_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_62_49, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_87_nl
      = output_line_sva_1_48 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_88_nl
      = output_line_sva_1_47 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_89_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_46_33, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_90_nl
      = output_line_sva_1_32 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_91_nl
      = output_line_sva_1_31 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_92_nl
      = MUX_v_14_2_2(14'b00000000000000, output_line_sva_1_30_17, (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_93_nl
      = output_line_sva_1_16 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_94_nl
      = output_line_sva_1_15 & (fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_95_nl
      = MUX_v_11_2_2(11'b00000000000, (output_line_sva_1_14_1[13:3]), (fsm_output[20]));
  assign memory_axi_write_base_axi_32_32_mux_nl = MUX_v_3_2_2((memory_channels_aw_channel_rsci_idat_34_31[3:1]),
      (output_line_sva_1_14_1[2:0]), fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_mux_4_nl = MUX_s_1_2_2((memory_channels_aw_channel_rsci_idat_34_31[0]),
      output_line_sva_1_0, fsm_output[20]);
  assign nl_memory_axi_write_base_axi_32_32_lshift_rg_a = {memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_1_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_2_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_3_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_4_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_5_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_6_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_7_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_8_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_9_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_10_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_11_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_12_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_13_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_14_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_15_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_16_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_17_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_18_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_19_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_20_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_21_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_22_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_23_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_24_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_25_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_26_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_27_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_28_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_29_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_30_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_31_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_32_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_33_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_34_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_35_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_36_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_37_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_38_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_39_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_40_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_41_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_42_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_43_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_44_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_45_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_46_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_47_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_48_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_49_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_50_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_51_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_52_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_53_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_54_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_55_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_56_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_57_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_58_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_59_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_60_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_61_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_62_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_63_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_64_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_65_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_66_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_67_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_68_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_69_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_70_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_71_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_72_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_73_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_74_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_75_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_76_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_77_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_78_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_79_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_80_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_81_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_82_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_83_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_84_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_85_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_86_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_87_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_88_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_89_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_90_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_91_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_92_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_93_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_94_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_95_nl
      , memory_axi_write_base_axi_32_32_mux_nl , memory_axi_write_base_axi_32_32_mux_4_nl};
  wire[3:0] memory_axi_write_base_axi_32_32_mux_1_nl;
  wire[1:0] memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_nl;
  wire [8:0] nl_memory_axi_write_base_axi_32_32_lshift_rg_s;
  assign memory_axi_write_base_axi_32_32_mux_1_nl = MUX_v_4_2_2(memory_channels_aw_channel_rsci_idat_34_31,
      ({reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1 , (reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[4:2])}),
      fsm_output[20]);
  assign memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_nl =
      MUX_v_2_2_2(2'b00, (reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[1:0]),
      (fsm_output[20]));
  assign nl_memory_axi_write_base_axi_32_32_lshift_rg_s = {memory_axi_write_base_axi_32_32_mux_1_nl
      , memory_axi_write_base_axi_32_32_memory_axi_write_base_axi_32_32_and_nl ,
      3'b000};
  wire [108:0] nl_dense_core_memory_channels_aw_channel_rsci_inst_memory_channels_aw_channel_rsci_idat;
  assign nl_dense_core_memory_channels_aw_channel_rsci_inst_memory_channels_aw_channel_rsci_idat
      = {16'b0000000000000000 , memory_channels_aw_channel_rsci_idat_92_35 , memory_channels_aw_channel_rsci_idat_34_31
      , memory_channels_aw_channel_rsci_idat_30_29 , 8'b00000000 , memory_channels_aw_channel_rsci_idat_20
      , 20'b10011000000000000000};
  wire [576:0] nl_dense_core_memory_channels_w_channel_rsci_inst_memory_channels_w_channel_rsci_idat;
  assign nl_dense_core_memory_channels_w_channel_rsci_inst_memory_channels_w_channel_rsci_idat
      = {memory_channels_w_channel_rsci_idat_576_65 , memory_channels_w_channel_rsci_idat_64_1
      , 1'b1};
  wire [108:0] nl_dense_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat;
  assign nl_dense_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat
      = {16'b0000000000000100 , ({{24{memory_channels_ar_channel_rsci_idat_68_43[25]}},
      memory_channels_ar_channel_rsci_idat_68_43}) , memory_channels_ar_channel_rsci_idat_42_29
      , memory_channels_ar_channel_rsci_idat_28_21 , 21'b110010000000000000000};
  wire  nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0;
  assign nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0
      = ~(memory_axi_burst_read_base_axi_u512_512_for_stage_0_2 | memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1);
  wire  nl_dense_core_core_fsm_inst_for_1_C_2_tr0;
  assign nl_dense_core_core_fsm_inst_for_1_C_2_tr0 = reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[4];
  wire  nl_dense_core_core_fsm_inst_main_C_1_tr0;
  assign nl_dense_core_core_fsm_inst_main_C_1_tr0 = ~ xor_cse;
  wire  nl_dense_core_core_fsm_inst_while_C_0_tr0;
  assign nl_dense_core_core_fsm_inst_while_C_0_tr0 = ~((num_feature_lines_sva!=16'b0000000000000000));
  wire  nl_dense_core_core_fsm_inst_while_for_1_C_1_tr0;
  assign nl_dense_core_core_fsm_inst_while_for_1_C_1_tr0 = (weight_index_lpi_3[7:0]!=8'b00000000);
  wire  nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0;
  assign nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0
      = ~(memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1 | exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1);
  wire  nl_dense_core_core_fsm_inst_while_for_2_C_0_tr0;
  assign nl_dense_core_core_fsm_inst_while_for_2_C_0_tr0 = z_out_3[5];
  ccs_in_v1 #(.rscid(32'sd4),
  .width(32'sd32)) addr_hi_rsci (
      .dat(addr_hi_rsc_dat),
      .idat(addr_hi_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd5),
  .width(32'sd32)) feature_addr_lo_rsci (
      .dat(feature_addr_lo_rsc_dat),
      .idat(feature_addr_lo_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd6),
  .width(32'sd32)) weight_addr_lo_rsci (
      .dat(weight_addr_lo_rsc_dat),
      .idat(weight_addr_lo_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd7),
  .width(32'sd32)) output_addr_lo_rsci (
      .dat(output_addr_lo_rsc_dat),
      .idat(output_addr_lo_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd8),
  .width(32'sd32)) input_vector_len_rsci (
      .dat(input_vector_len_rsc_dat),
      .idat(input_vector_len_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd9),
  .width(32'sd32)) output_vector_len_rsci (
      .dat(output_vector_len_rsc_dat),
      .idat(output_vector_len_rsci_idat)
    );
  ccs_out_v1 #(.rscid(32'sd10),
  .width(32'sd32)) debug_rsci (
      .idat(nl_debug_rsci_idat[31:0]),
      .dat(debug_rsc_dat)
    );
  mgc_shift_l_v5 #(.width_a(32'sd64),
  .signd_a(32'sd0),
  .width_s(32'sd6),
  .width_z(32'sd64)) memory_encode_strb_1_if_6_lshift_rg (
      .a(nl_memory_encode_strb_1_if_6_lshift_rg_a[63:0]),
      .s(nl_memory_encode_strb_1_if_6_lshift_rg_s[5:0]),
      .z(z_out)
    );
  mgc_shift_l_v5 #(.width_a(32'sd512),
  .signd_a(32'sd0),
  .width_s(32'sd9),
  .width_z(32'sd512)) memory_axi_write_base_axi_32_32_lshift_rg (
      .a(nl_memory_axi_write_base_axi_32_32_lshift_rg_a[511:0]),
      .s(nl_memory_axi_write_base_axi_32_32_lshift_rg_s[8:0]),
      .z(z_out_1)
    );
  dense_core_start_rsci dense_core_start_rsci_inst (
      .clk(clk),
      .arst_n(arst_n),
      .start_rsc_dat(start_rsc_dat),
      .start_rsc_vld(start_rsc_vld),
      .start_rsc_rdy(start_rsc_rdy),
      .start_rsci_oswt(reg_start_rsci_oswt_tmp),
      .start_rsci_wen_comp(start_rsci_wen_comp),
      .start_rsci_ivld(start_rsci_ivld),
      .start_rsci_ivld_oreg(start_rsci_ivld_oreg),
      .start_rsci_wen_comp_pff(start_rsci_wen_comp_iff),
      .start_rsci_oswt_pff(mux_rmff),
      .start_rsci_ivld_oreg_pff(start_rsci_ivld)
    );
  dense_core_wait_dp dense_core_wait_dp_inst (
      .clk(clk),
      .arst_n(arst_n),
      .start_rsci_ivld(start_rsci_ivld),
      .start_rsci_ivld_oreg(start_rsci_ivld_oreg),
      .done_rsci_irdy(done_rsci_irdy),
      .done_rsci_irdy_oreg(done_rsci_irdy_oreg),
      .memory_channels_aw_channel_rsci_irdy(memory_channels_aw_channel_rsci_irdy),
      .memory_channels_aw_channel_rsci_irdy_oreg(memory_channels_aw_channel_rsci_irdy_oreg),
      .memory_channels_w_channel_rsci_irdy(memory_channels_w_channel_rsci_irdy),
      .memory_channels_w_channel_rsci_irdy_oreg(memory_channels_w_channel_rsci_irdy_oreg),
      .memory_channels_b_channel_rsci_ivld(memory_channels_b_channel_rsci_ivld),
      .memory_channels_b_channel_rsci_ivld_oreg(memory_channels_b_channel_rsci_ivld_oreg),
      .memory_channels_ar_channel_rsci_irdy(memory_channels_ar_channel_rsci_irdy),
      .memory_channels_ar_channel_rsci_irdy_oreg(memory_channels_ar_channel_rsci_irdy_oreg),
      .memory_channels_r_channel_rsci_ivld(memory_channels_r_channel_rsci_ivld),
      .memory_channels_r_channel_rsci_ivld_oreg(memory_channels_r_channel_rsci_ivld_oreg)
    );
  dense_core_done_rsci dense_core_done_rsci_inst (
      .clk(clk),
      .arst_n(arst_n),
      .done_rsc_dat(done_rsc_dat),
      .done_rsc_vld(done_rsc_vld),
      .done_rsc_rdy(done_rsc_rdy),
      .done_rsci_oswt(reg_done_rsci_oswt_tmp),
      .done_rsci_wen_comp(done_rsci_wen_comp),
      .done_rsci_irdy(done_rsci_irdy),
      .done_rsci_irdy_oreg(done_rsci_irdy_oreg),
      .done_rsci_wen_comp_pff(done_rsci_wen_comp_iff),
      .done_rsci_oswt_pff(mux_4_rmff),
      .done_rsci_irdy_oreg_pff(done_rsci_irdy)
    );
  dense_core_memory_channels_aw_channel_rsci dense_core_memory_channels_aw_channel_rsci_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_aw_channel_rsc_dat(memory_channels_aw_channel_rsc_dat),
      .memory_channels_aw_channel_rsc_vld(memory_channels_aw_channel_rsc_vld),
      .memory_channels_aw_channel_rsc_rdy(memory_channels_aw_channel_rsc_rdy),
      .memory_channels_aw_channel_rsci_oswt(reg_memory_channels_aw_channel_rsci_oswt_tmp),
      .memory_channels_aw_channel_rsci_wen_comp(memory_channels_aw_channel_rsci_wen_comp),
      .memory_channels_aw_channel_rsci_irdy(memory_channels_aw_channel_rsci_irdy),
      .memory_channels_aw_channel_rsci_irdy_oreg(memory_channels_aw_channel_rsci_irdy_oreg),
      .memory_channels_aw_channel_rsci_idat(nl_dense_core_memory_channels_aw_channel_rsci_inst_memory_channels_aw_channel_rsci_idat[108:0]),
      .memory_channels_aw_channel_rsci_wen_comp_pff(memory_channels_aw_channel_rsci_wen_comp_iff),
      .memory_channels_aw_channel_rsci_oswt_pff(memory_send_aw_mux_rmff),
      .memory_channels_aw_channel_rsci_irdy_oreg_pff(memory_channels_aw_channel_rsci_irdy)
    );
  dense_core_memory_channels_w_channel_rsci dense_core_memory_channels_w_channel_rsci_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_w_channel_rsc_dat(memory_channels_w_channel_rsc_dat),
      .memory_channels_w_channel_rsc_vld(memory_channels_w_channel_rsc_vld),
      .memory_channels_w_channel_rsc_rdy(memory_channels_w_channel_rsc_rdy),
      .memory_channels_w_channel_rsci_oswt(reg_memory_channels_w_channel_rsci_oswt_tmp),
      .memory_channels_w_channel_rsci_wen_comp(memory_channels_w_channel_rsci_wen_comp),
      .memory_channels_w_channel_rsci_irdy(memory_channels_w_channel_rsci_irdy),
      .memory_channels_w_channel_rsci_irdy_oreg(memory_channels_w_channel_rsci_irdy_oreg),
      .memory_channels_w_channel_rsci_idat(nl_dense_core_memory_channels_w_channel_rsci_inst_memory_channels_w_channel_rsci_idat[576:0]),
      .memory_channels_w_channel_rsci_wen_comp_pff(memory_channels_w_channel_rsci_wen_comp_iff),
      .memory_channels_w_channel_rsci_oswt_pff(memory_send_w_mux_rmff),
      .memory_channels_w_channel_rsci_irdy_oreg_pff(memory_channels_w_channel_rsci_irdy)
    );
  dense_core_memory_channels_b_channel_rsci dense_core_memory_channels_b_channel_rsci_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_b_channel_rsc_dat(memory_channels_b_channel_rsc_dat),
      .memory_channels_b_channel_rsc_vld(memory_channels_b_channel_rsc_vld),
      .memory_channels_b_channel_rsc_rdy(memory_channels_b_channel_rsc_rdy),
      .memory_channels_b_channel_rsci_oswt(reg_memory_channels_b_channel_rsci_oswt_tmp),
      .memory_channels_b_channel_rsci_wen_comp(memory_channels_b_channel_rsci_wen_comp),
      .memory_channels_b_channel_rsci_ivld(memory_channels_b_channel_rsci_ivld),
      .memory_channels_b_channel_rsci_ivld_oreg(memory_channels_b_channel_rsci_ivld_oreg),
      .memory_channels_b_channel_rsci_wen_comp_pff(memory_channels_b_channel_rsci_wen_comp_iff),
      .memory_channels_b_channel_rsci_oswt_pff(memory_get_b_mux_rmff),
      .memory_channels_b_channel_rsci_ivld_oreg_pff(memory_channels_b_channel_rsci_ivld)
    );
  dense_core_memory_channels_ar_channel_rsci dense_core_memory_channels_ar_channel_rsci_inst
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
      .memory_channels_ar_channel_rsci_idat(nl_dense_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat[108:0]),
      .memory_channels_ar_channel_rsci_wen_comp_pff(memory_channels_ar_channel_rsci_wen_comp_iff),
      .memory_channels_ar_channel_rsci_oswt_pff(memory_send_ar_mux_rmff),
      .memory_channels_ar_channel_rsci_irdy_oreg_pff(memory_channels_ar_channel_rsci_irdy)
    );
  dense_core_memory_channels_r_channel_rsci dense_core_memory_channels_r_channel_rsci_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_r_channel_rsc_dat(memory_channels_r_channel_rsc_dat),
      .memory_channels_r_channel_rsc_vld(memory_channels_r_channel_rsc_vld),
      .memory_channels_r_channel_rsc_rdy(memory_channels_r_channel_rsc_rdy),
      .core_wen(core_wen),
      .memory_channels_r_channel_rsci_oswt(reg_memory_channels_r_channel_rsci_oswt_tmp),
      .memory_channels_r_channel_rsci_wen_comp(memory_channels_r_channel_rsci_wen_comp),
      .memory_channels_r_channel_rsci_ivld(memory_channels_r_channel_rsci_ivld),
      .memory_channels_r_channel_rsci_ivld_oreg(memory_channels_r_channel_rsci_ivld_oreg),
      .memory_channels_r_channel_rsci_idat_mxwt(memory_channels_r_channel_rsci_idat_mxwt),
      .memory_channels_r_channel_rsci_wen_comp_pff(memory_channels_r_channel_rsci_wen_comp_iff),
      .memory_channels_r_channel_rsci_oswt_pff(memory_get_r_mux_rmff),
      .memory_channels_r_channel_rsci_ivld_oreg_pff(memory_channels_r_channel_rsci_ivld)
    );
  dense_core_use_relu_triosy_obj dense_core_use_relu_triosy_obj_inst (
      .use_relu_triosy_lz(use_relu_triosy_lz),
      .core_wten(core_wten),
      .use_relu_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_addr_hi_triosy_obj dense_core_addr_hi_triosy_obj_inst (
      .addr_hi_triosy_lz(addr_hi_triosy_lz),
      .core_wten(core_wten),
      .addr_hi_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_feature_addr_lo_triosy_obj dense_core_feature_addr_lo_triosy_obj_inst
      (
      .feature_addr_lo_triosy_lz(feature_addr_lo_triosy_lz),
      .core_wten(core_wten),
      .feature_addr_lo_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_weight_addr_lo_triosy_obj dense_core_weight_addr_lo_triosy_obj_inst
      (
      .weight_addr_lo_triosy_lz(weight_addr_lo_triosy_lz),
      .core_wten(core_wten),
      .weight_addr_lo_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_output_addr_lo_triosy_obj dense_core_output_addr_lo_triosy_obj_inst
      (
      .output_addr_lo_triosy_lz(output_addr_lo_triosy_lz),
      .core_wten(core_wten),
      .output_addr_lo_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_input_vector_len_triosy_obj dense_core_input_vector_len_triosy_obj_inst
      (
      .input_vector_len_triosy_lz(input_vector_len_triosy_lz),
      .core_wten(core_wten),
      .input_vector_len_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_output_vector_len_triosy_obj dense_core_output_vector_len_triosy_obj_inst
      (
      .output_vector_len_triosy_lz(output_vector_len_triosy_lz),
      .core_wten(core_wten),
      .output_vector_len_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_debug_triosy_obj dense_core_debug_triosy_obj_inst (
      .debug_triosy_lz(debug_triosy_lz),
      .core_wten(core_wten),
      .debug_triosy_obj_iswt0(reg_done_rsci_oswt_tmp)
    );
  dense_core_staller dense_core_staller_inst (
      .clk(clk),
      .arst_n(arst_n),
      .core_wen(core_wen_rtff),
      .core_wten(core_wten),
      .start_rsci_wen_comp(start_rsci_wen_comp),
      .done_rsci_wen_comp(done_rsci_wen_comp),
      .memory_channels_aw_channel_rsci_wen_comp(memory_channels_aw_channel_rsci_wen_comp),
      .memory_channels_w_channel_rsci_wen_comp(memory_channels_w_channel_rsci_wen_comp),
      .memory_channels_b_channel_rsci_wen_comp(memory_channels_b_channel_rsci_wen_comp),
      .memory_channels_ar_channel_rsci_wen_comp(memory_channels_ar_channel_rsci_wen_comp),
      .memory_channels_r_channel_rsci_wen_comp(memory_channels_r_channel_rsci_wen_comp),
      .start_rsci_wen_comp_pff(start_rsci_wen_comp_iff),
      .done_rsci_wen_comp_pff(done_rsci_wen_comp_iff),
      .memory_channels_aw_channel_rsci_wen_comp_pff(memory_channels_aw_channel_rsci_wen_comp_iff),
      .memory_channels_w_channel_rsci_wen_comp_pff(memory_channels_w_channel_rsci_wen_comp_iff),
      .memory_channels_b_channel_rsci_wen_comp_pff(memory_channels_b_channel_rsci_wen_comp_iff),
      .memory_channels_ar_channel_rsci_wen_comp_pff(memory_channels_ar_channel_rsci_wen_comp_iff),
      .memory_channels_r_channel_rsci_wen_comp_pff(memory_channels_r_channel_rsci_wen_comp_iff)
    );
  dense_core_core_fsm dense_core_core_fsm_inst (
      .clk(clk),
      .arst_n(arst_n),
      .core_wen(core_wen),
      .fsm_output(fsm_output),
      .main_C_0_tr0(exit_for_sva_mx0),
      .for_C_1_tr0(exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1),
      .memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0(nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0),
      .for_C_2_tr0(exit_for_sva_mx0),
      .for_1_C_2_tr0(nl_dense_core_core_fsm_inst_for_1_C_2_tr0),
      .main_C_1_tr0(nl_dense_core_core_fsm_inst_main_C_1_tr0),
      .while_C_0_tr0(nl_dense_core_core_fsm_inst_while_C_0_tr0),
      .while_for_1_C_1_tr0(nl_dense_core_core_fsm_inst_while_for_1_C_1_tr0),
      .memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0(nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0),
      .while_for_1_C_5_tr0(exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1),
      .while_for_2_C_0_tr0(nl_dense_core_core_fsm_inst_while_for_2_C_0_tr0),
      .while_C_4_tr0(exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1)
    );
  assign feature_memory_rsci_clken_d = core_wen;
  assign while_for_1_for_or_cse = (fsm_output[0]) | (fsm_output[23]);
  assign mux_rmff = MUX_s_1_2_2(reg_start_rsci_oswt_tmp, while_for_1_for_or_cse,
      core_wen);
  assign or_107_nl = ((~ xor_cse) & (fsm_output[9])) | (exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1
      & (fsm_output[22]));
  assign mux_4_rmff = MUX_s_1_2_2(reg_done_rsci_oswt_tmp, or_107_nl, core_wen);
  assign or_108_cse = and_133_cse | and_134_cse | and_135_cse;
  assign memory_send_aw_mux_rmff = MUX_s_1_2_2(reg_memory_channels_aw_channel_rsci_oswt_tmp,
      or_108_cse, core_wen);
  assign or_109_cse = (fsm_output[6]) | and_138_cse;
  assign memory_send_w_mux_rmff = MUX_s_1_2_2(reg_memory_channels_w_channel_rsci_oswt_tmp,
      or_109_cse, core_wen);
  assign or_110_nl = (fsm_output[7]) | (and_dcpl_42 & (fsm_output[21]));
  assign memory_get_b_mux_rmff = MUX_s_1_2_2(reg_memory_channels_b_channel_rsci_oswt_tmp,
      or_110_nl, core_wen);
  assign or_111_cse = and_143_cse | ((~ operator_16_false_operator_16_false_nor_tmp)
      & (fsm_output[2]));
  assign memory_send_ar_mux_rmff = MUX_s_1_2_2(reg_memory_channels_ar_channel_rsci_oswt_tmp,
      or_111_cse, core_wen);
  assign or_112_nl = (nand_cse & exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1
      & (fsm_output[13])) | (and_25_cse & (fsm_output[12])) | (or_dcpl & memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1
      & operator_16_false_1_acc_itm_10_1_10_1_10 & (fsm_output[4]));
  assign memory_get_r_mux_rmff = MUX_s_1_2_2(reg_memory_channels_r_channel_rsci_oswt_tmp,
      or_112_nl, core_wen);
  assign memory_send_ar_and_cse = core_wen & or_111_cse;
  assign memory_send_aw_and_1_cse = core_wen & (or_tmp_40 | and_133_cse);
  assign memory_send_w_and_cse = core_wen & or_109_cse;
  assign output_line_and_itm = core_wen & (fsm_output[19]);
  assign output_line_and_1_cse = output_line_and_itm & while_and_stg_3_15 & (out_index_sva[4]);
  assign output_line_and_4_cse = output_line_and_itm & while_and_stg_3_14 & (out_index_sva[4]);
  assign output_line_and_7_cse = output_line_and_itm & while_and_stg_3_13 & (out_index_sva[4]);
  assign output_line_and_10_cse = output_line_and_itm & while_and_stg_3_12 & (out_index_sva[4]);
  assign output_line_and_13_cse = output_line_and_itm & while_and_stg_3_11 & (out_index_sva[4]);
  assign output_line_and_16_cse = output_line_and_itm & while_and_stg_3_10 & (out_index_sva[4]);
  assign output_line_and_19_cse = output_line_and_itm & while_and_stg_3_9 & (out_index_sva[4]);
  assign output_line_and_22_cse = output_line_and_itm & while_and_stg_3_8 & (out_index_sva[4]);
  assign output_line_and_25_cse = output_line_and_itm & while_and_stg_3_7 & (out_index_sva[4]);
  assign output_line_and_28_cse = output_line_and_itm & while_and_stg_3_6 & (out_index_sva[4]);
  assign output_line_and_31_cse = output_line_and_itm & while_and_stg_3_5 & (out_index_sva[4]);
  assign output_line_and_34_cse = output_line_and_itm & while_and_stg_3_4 & (out_index_sva[4]);
  assign output_line_and_37_cse = output_line_and_itm & while_and_stg_3_3 & (out_index_sva[4]);
  assign output_line_and_40_cse = output_line_and_itm & while_and_stg_3_2 & (out_index_sva[4]);
  assign output_line_and_43_cse = output_line_and_itm & while_and_stg_3_1 & (out_index_sva[4]);
  assign output_line_and_46_cse = output_line_and_itm & while_and_stg_3_0 & (out_index_sva[4]);
  assign output_line_and_49_cse = output_line_and_itm & while_and_stg_3_15 & (~ (out_index_sva[4]));
  assign output_line_and_52_cse = output_line_and_itm & while_and_stg_3_14 & (~ (out_index_sva[4]));
  assign output_line_and_55_cse = output_line_and_itm & while_and_stg_3_13 & (~ (out_index_sva[4]));
  assign output_line_and_58_cse = output_line_and_itm & while_and_stg_3_12 & (~ (out_index_sva[4]));
  assign output_line_and_61_cse = output_line_and_itm & while_and_stg_3_11 & (~ (out_index_sva[4]));
  assign output_line_and_64_cse = output_line_and_itm & while_and_stg_3_10 & (~ (out_index_sva[4]));
  assign output_line_and_67_cse = output_line_and_itm & while_and_stg_3_9 & (~ (out_index_sva[4]));
  assign output_line_and_70_cse = output_line_and_itm & while_and_stg_3_8 & (~ (out_index_sva[4]));
  assign output_line_and_73_cse = output_line_and_itm & while_and_stg_3_7 & (~ (out_index_sva[4]));
  assign output_line_and_76_cse = output_line_and_itm & while_and_stg_3_6 & (~ (out_index_sva[4]));
  assign output_line_and_79_cse = output_line_and_itm & while_and_stg_3_5 & (~ (out_index_sva[4]));
  assign output_line_and_82_cse = output_line_and_itm & while_and_stg_3_4 & (~ (out_index_sva[4]));
  assign output_line_and_85_cse = output_line_and_itm & while_and_stg_3_3 & (~ (out_index_sva[4]));
  assign output_line_and_88_cse = output_line_and_itm & while_and_stg_3_2 & (~ (out_index_sva[4]));
  assign output_line_and_91_cse = output_line_and_itm & while_and_stg_3_1 & (~ (out_index_sva[4]));
  assign output_line_and_94_cse = output_line_and_itm & while_and_stg_3_0 & (~ (out_index_sva[4]));
  assign nl_acc_2_sdt = conv_s2s_32_33(addr_hi_rsci_idat) + conv_s2s_32_33(output_addr_lo_rsci_idat);
  assign acc_2_sdt = nl_acc_2_sdt[32:0];
  assign output_address_and_ssc = core_wen & ((fsm_output[19]) | (fsm_output[1]));
  assign output_address_and_1_rgt = (z_out_2[4:0]==5'b00000) & (fsm_output[19]);
  assign output_vector_len_and_cse = core_wen & (fsm_output[1]);
  assign for_i_and_ssc = core_wen & ((fsm_output[1]) | (fsm_output[5]) | for_i_31_14_sva_mx0c2);
  assign id_or_cse = (fsm_output[14]) | (fsm_output[19]);
  assign nl_acc_1_sdt = conv_s2s_32_33(addr_hi_rsci_idat) + conv_s2s_32_33(feature_addr_lo_rsci_idat);
  assign acc_1_sdt = nl_acc_1_sdt[32:0];
  assign and_745_ssc = core_wen & ((fsm_output[1]) | acc_1_psp_sva_mx0c1);
  assign sum_array_or_32_cse = (fsm_output[18:17]!=2'b00);
  assign nand_cse = ~(((memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3[8])
      | memory_axi_burst_read_base_axi_u512_512_1_for_memory_axi_burst_read_base_axi_u512_512_1_for_if_or_tmp)
      & memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1);
  assign for_feature_burst_size_qelse_and_ssc = core_wen & ((memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1
      & ((fsm_output[13]) | (fsm_output[17]) | (fsm_output[18]) | (fsm_output[7])
      | (fsm_output[6]) | (fsm_output[10]))) | (fsm_output[2]) | for_feature_burst_size_qr_7_0_lpi_2_dfm_mx0c1
      | (fsm_output[12]) | (fsm_output[19]));
  assign sum_array_and_cse = core_wen & ((fsm_output[17]) | (fsm_output[10]));
  assign while_sum_out_nor_2_nl = ~(MUX_v_14_2_2((while_sum_out_slc_32_8_sat_sva_1[14:1]),
      14'b11111111111111, while_sum_out_nor_ovfl_sva_1));
  assign while_sum_out_while_sum_out_nor_itm = ~(MUX_v_14_2_2(while_sum_out_nor_2_nl,
      14'b11111111111111, while_sum_out_and_unfl_sva_1));
  assign while_sum_out_while_sum_out_nor_1_itm = ~((~((while_sum_out_slc_32_8_sat_sva_1[0])
      | while_sum_out_nor_ovfl_sva_1)) | while_sum_out_and_unfl_sva_1);
  assign exit_for_sva_mx0 = MUX_s_1_2_2((~ ($signed(({(~ input_vector_len_rsci_idat)
      , 1'b1})) < $signed(33'b111111111111111111111111111111111))), (~ ($signed(conv_u2s_32_33({for_i_31_14_sva_2
      , 14'b00000000000000})) < $signed(({sum_sva , 1'b0})))), fsm_output[5]);
  assign nl_for_i_31_14_sva_2 = ({for_i_31_14_sva_17_16 , for_i_31_14_sva_15_0})
      + 18'b000000000000000001;
  assign for_i_31_14_sva_2 = nl_for_i_31_14_sva_2[17:0];
  assign for_feature_burst_size_qr_7_0_lpi_2_dfm_1 = MUX_v_8_2_2(8'b00000000, (out_index_sva[7:0]),
      operator_16_false_slc_operator_16_false_acc_8_mdf_sva_mx0w0);
  assign operator_16_false_slc_operator_16_false_acc_8_mdf_sva_mx0w0 = (out_index_sva[15:8])
      < 8'b00000001;
  assign operator_16_false_operator_16_false_nor_tmp = ~((~ operator_16_false_slc_operator_16_false_acc_8_mdf_sva_mx0w0)
      | (for_feature_burst_size_qr_7_0_lpi_2_dfm_1!=8'b00000000));
  assign exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1_mx0w3 = nand_cse
      & exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1;
  assign nl_operator_16_false_1_acc_nl = conv_u2s_10_11(memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva_mx1)
      + conv_s2s_10_11({1'b1 , operator_16_false_slc_operator_16_false_acc_8_mdf_sva
      , (~ reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd) , (~ reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1)
      , (~ reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2)}) + 11'b00000000001;
  assign operator_16_false_1_acc_nl = nl_operator_16_false_1_acc_nl[10:0];
  assign operator_16_false_1_acc_itm_10_1_10_1_10 = readslicef_11_1_10(operator_16_false_1_acc_nl);
  assign nl_memory_axi_burst_read_base_axi_u512_512_for_acc_nl = conv_u2s_9_10(memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva[8:0])
      + 10'b0000000001;
  assign memory_axi_burst_read_base_axi_u512_512_for_acc_nl = nl_memory_axi_burst_read_base_axi_u512_512_for_acc_nl[9:0];
  assign memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva_mx1 = MUX_v_10_2_2(memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva,
      memory_axi_burst_read_base_axi_u512_512_for_acc_nl, memory_axi_burst_read_base_axi_u512_512_for_stage_0_2);
  assign memory_axi_burst_read_base_axi_u512_512_for_stage_0_2_mx0w1 = or_dcpl &
      memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1;
  assign nl_memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3 = conv_u2s_8_9({reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd
      , reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1 , reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2})
      + 9'b000000001;
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3 = nl_memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3[8:0];
  assign nl_while_sum_out_slc_32_8_sat_sva_1 = conv_s2u_24_25(sum_sva[31:8]) + conv_u2u_1_25(sum_sva[7]);
  assign while_sum_out_slc_32_8_sat_sva_1 = nl_while_sum_out_slc_32_8_sat_sva_1[24:0];
  assign while_sum_out_nor_ovfl_sva_1 = ~((while_sum_out_slc_32_8_sat_sva_1[24])
      | (~((while_sum_out_slc_32_8_sat_sva_1[23:15]!=9'b000000000))));
  assign while_sum_out_and_unfl_sva_1 = (while_sum_out_slc_32_8_sat_sva_1[24]) &
      (~((while_sum_out_slc_32_8_sat_sva_1[23:15]==9'b111111111)));
  assign memory_axi_burst_read_base_axi_u512_512_1_for_memory_axi_burst_read_base_axi_u512_512_1_for_if_or_tmp
      = (memory_channels_r_channel_rsci_idat_mxwt[1:0]!=2'b00);
  assign or_dcpl = ~((memory_axi_burst_read_base_axi_u512_512_1_for_memory_axi_burst_read_base_axi_u512_512_1_for_if_or_tmp
      | exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1) & memory_axi_burst_read_base_axi_u512_512_for_stage_0_2);
  assign and_25_cse = (weight_index_lpi_3[7:0]==8'b00000000);
  assign or_dcpl_41 = (fsm_output[5]) | (fsm_output[1]);
  assign and_dcpl_42 = (for_i_31_14_sva_15_0[4:0]==5'b00000);
  assign or_dcpl_81 = (fsm_output[17]) | (fsm_output[3]);
  assign and_134_cse = exit_for_sva_mx0 & or_dcpl_41;
  assign and_135_cse = (~ (reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[4]))
      & (fsm_output[8]);
  assign and_133_cse = (z_out_2[4:0]==5'b00000) & (fsm_output[19]);
  assign and_138_cse = and_dcpl_42 & (fsm_output[20]);
  assign and_143_cse = and_25_cse & (fsm_output[11]);
  assign or_tmp_40 = and_134_cse | and_135_cse;
  assign debug_rsci_idat_15_0_mx0c1 = (xor_cse & (fsm_output[9])) | ((~ exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1)
      & (fsm_output[22]));
  assign out_index_sva_mx0c0 = while_for_1_for_or_cse | and_134_cse;
  assign for_i_31_14_sva_mx0c2 = (fsm_output[16]) | (fsm_output[13]) | (fsm_output[15])
      | (fsm_output[11]) | (fsm_output[12]) | (fsm_output[20]) | (fsm_output[17])
      | (fsm_output[14]) | (fsm_output[19]) | (fsm_output[10]);
  assign acc_1_psp_sva_mx0c1 = sum_array_or_32_cse | (fsm_output[10]);
  assign sum_sva_mx0c1 = while_for_1_for_or_cse | (fsm_output[22]) | (fsm_output[9])
      | (fsm_output[21]) | (fsm_output[20]) | (fsm_output[7]) | (fsm_output[6]) |
      (fsm_output[19]) | (fsm_output[8]) | (fsm_output[10]);
  assign for_feature_burst_size_qr_7_0_lpi_2_dfm_mx0c1 = sum_array_or_32_cse | (fsm_output[7])
      | (fsm_output[6]) | (fsm_output[10]);
  assign xor_cse = $signed(32'b00000000000000000000000000000000) < $signed(output_vector_len_sva);
  assign while_for_1_for_32_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[511:496];
  assign while_for_1_for_32_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[511:496];
  assign while_for_1_for_32_while_for_1_for_acc_2_cmp_load_pff = sum_array_31_sva_load;
  assign while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_pff = fsm_output[15];
  assign while_for_1_for_31_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[495:480];
  assign while_for_1_for_31_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[495:480];
  assign while_for_1_for_30_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[479:464];
  assign while_for_1_for_30_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[479:464];
  assign while_for_1_for_29_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[463:448];
  assign while_for_1_for_29_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[463:448];
  assign while_for_1_for_28_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[447:432];
  assign while_for_1_for_28_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[447:432];
  assign while_for_1_for_27_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[431:416];
  assign while_for_1_for_27_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[431:416];
  assign while_for_1_for_26_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[415:400];
  assign while_for_1_for_26_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[415:400];
  assign while_for_1_for_25_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[399:384];
  assign while_for_1_for_25_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[399:384];
  assign while_for_1_for_24_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[383:368];
  assign while_for_1_for_24_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[383:368];
  assign while_for_1_for_23_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[367:352];
  assign while_for_1_for_23_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[367:352];
  assign while_for_1_for_22_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[351:336];
  assign while_for_1_for_22_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[351:336];
  assign while_for_1_for_21_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[335:320];
  assign while_for_1_for_21_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[335:320];
  assign while_for_1_for_20_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[319:304];
  assign while_for_1_for_20_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[319:304];
  assign while_for_1_for_19_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[303:288];
  assign while_for_1_for_19_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[303:288];
  assign while_for_1_for_18_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[287:272];
  assign while_for_1_for_18_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[287:272];
  assign while_for_1_for_17_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[271:256];
  assign while_for_1_for_17_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[271:256];
  assign while_for_1_for_16_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[255:240];
  assign while_for_1_for_16_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[255:240];
  assign while_for_1_for_15_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[239:224];
  assign while_for_1_for_15_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[239:224];
  assign while_for_1_for_14_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[223:208];
  assign while_for_1_for_14_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[223:208];
  assign while_for_1_for_13_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[207:192];
  assign while_for_1_for_13_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[207:192];
  assign while_for_1_for_12_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[191:176];
  assign while_for_1_for_12_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[191:176];
  assign while_for_1_for_11_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[175:160];
  assign while_for_1_for_11_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[175:160];
  assign while_for_1_for_10_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[159:144];
  assign while_for_1_for_10_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[159:144];
  assign while_for_1_for_9_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[143:128];
  assign while_for_1_for_9_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[143:128];
  assign while_for_1_for_8_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[127:112];
  assign while_for_1_for_8_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[127:112];
  assign while_for_1_for_7_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[111:96];
  assign while_for_1_for_7_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[111:96];
  assign while_for_1_for_6_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[95:80];
  assign while_for_1_for_6_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[95:80];
  assign while_for_1_for_5_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[79:64];
  assign while_for_1_for_5_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[79:64];
  assign while_for_1_for_4_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[63:48];
  assign while_for_1_for_4_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[63:48];
  assign while_for_1_for_3_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[47:32];
  assign while_for_1_for_3_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[47:32];
  assign while_for_1_for_2_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[31:16];
  assign while_for_1_for_2_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[31:16];
  assign while_for_1_for_1_while_for_1_for_acc_2_cmp_a = feature_memory_rsci_q_d[15:0];
  assign while_for_1_for_1_while_for_1_for_acc_2_cmp_b = weight_memory_rsci_q_d[15:0];
  assign while_for_1_for_1_while_for_1_for_acc_2_cmp_load = memory_axi_burst_read_base_axi_u512_512_for_stage_0_2;
  assign feature_memory_rsci_d_d_pff = memory_channels_r_channel_rsci_idat_mxwt[513:2];
  assign feature_memory_rsci_radr_d = for_i_31_14_sva_15_0[14:0];
  assign feature_memory_rsci_re_d_pff = fsm_output[14];
  assign feature_memory_rsci_wadr_d = {(for_i_31_14_sva_15_0[0]) , 5'b00000 , (memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva[8:0])};
  assign feature_memory_rsci_we_d_pff = memory_axi_burst_read_base_axi_u512_512_for_stage_0_2
      & (~ exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1) & (fsm_output[4]);
  assign weight_memory_rsci_radr_d = weight_index_lpi_3[7:0];
  assign weight_memory_rsci_wadr_d = {reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd
      , reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1 , reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2};
  assign weight_memory_rsci_we_d_pff = memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1
      & (fsm_output[13]);
  assign while_and_stg_1_3 = (out_index_sva[1:0]==2'b11);
  assign while_and_stg_2_7 = while_and_stg_1_3 & (out_index_sva[2]);
  assign while_and_stg_3_15 = while_and_stg_2_7 & (out_index_sva[3]);
  assign while_and_stg_1_2 = (out_index_sva[1:0]==2'b10);
  assign while_and_stg_2_6 = while_and_stg_1_2 & (out_index_sva[2]);
  assign while_and_stg_3_14 = while_and_stg_2_6 & (out_index_sva[3]);
  assign while_and_stg_1_1 = (out_index_sva[1:0]==2'b01);
  assign while_and_stg_2_5 = while_and_stg_1_1 & (out_index_sva[2]);
  assign while_and_stg_3_13 = while_and_stg_2_5 & (out_index_sva[3]);
  assign while_and_stg_1_0 = ~((out_index_sva[1:0]!=2'b00));
  assign while_and_stg_2_4 = while_and_stg_1_0 & (out_index_sva[2]);
  assign while_and_stg_3_12 = while_and_stg_2_4 & (out_index_sva[3]);
  assign while_and_stg_2_3 = while_and_stg_1_3 & (~ (out_index_sva[2]));
  assign while_and_stg_3_11 = while_and_stg_2_3 & (out_index_sva[3]);
  assign while_and_stg_2_2 = while_and_stg_1_2 & (~ (out_index_sva[2]));
  assign while_and_stg_3_10 = while_and_stg_2_2 & (out_index_sva[3]);
  assign while_and_stg_2_1 = while_and_stg_1_1 & (~ (out_index_sva[2]));
  assign while_and_stg_3_9 = while_and_stg_2_1 & (out_index_sva[3]);
  assign while_and_stg_2_0 = while_and_stg_1_0 & (~ (out_index_sva[2]));
  assign while_and_stg_3_8 = while_and_stg_2_0 & (out_index_sva[3]);
  assign while_and_stg_3_7 = while_and_stg_2_7 & (~ (out_index_sva[3]));
  assign while_and_stg_3_6 = while_and_stg_2_6 & (~ (out_index_sva[3]));
  assign while_and_stg_3_5 = while_and_stg_2_5 & (~ (out_index_sva[3]));
  assign while_and_stg_3_4 = while_and_stg_2_4 & (~ (out_index_sva[3]));
  assign while_and_stg_3_3 = while_and_stg_2_3 & (~ (out_index_sva[3]));
  assign while_and_stg_3_2 = while_and_stg_2_2 & (~ (out_index_sva[3]));
  assign while_and_stg_3_1 = while_and_stg_2_1 & (~ (out_index_sva[3]));
  assign while_and_stg_3_0 = while_and_stg_2_0 & (~ (out_index_sva[3]));
  assign for_nor_1_cse = ~((fsm_output[1]) | (fsm_output[19]) | (fsm_output[14]));
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_start_rsci_oswt_tmp <= 1'b0;
      reg_done_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_aw_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_w_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_b_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_ar_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_r_channel_rsci_oswt_tmp <= 1'b0;
      core_wen <= 1'b1;
    end
    else begin
      reg_start_rsci_oswt_tmp <= mux_rmff;
      reg_done_rsci_oswt_tmp <= mux_4_rmff;
      reg_memory_channels_aw_channel_rsci_oswt_tmp <= memory_send_aw_mux_rmff;
      reg_memory_channels_w_channel_rsci_oswt_tmp <= memory_send_w_mux_rmff;
      reg_memory_channels_b_channel_rsci_oswt_tmp <= memory_get_b_mux_rmff;
      reg_memory_channels_ar_channel_rsci_oswt_tmp <= memory_send_ar_mux_rmff;
      reg_memory_channels_r_channel_rsci_oswt_tmp <= memory_get_r_mux_rmff;
      core_wen <= core_wen_rtff;
    end
  end
  always @(posedge clk) begin
    if ( memory_send_ar_and_cse ) begin
      memory_channels_ar_channel_rsci_idat_42_29 <= MUX_v_14_2_2((acc_1_psp_sva_31_0[13:0]),
          (acc_psp_sva[13:0]), and_143_cse);
      memory_channels_ar_channel_rsci_idat_28_21 <= MUX_v_8_2_2(z_out_3, 8'b11111111,
          and_143_cse);
      memory_channels_ar_channel_rsci_idat_68_43 <= MUX_v_26_2_2((signext_26_20(operator_64_false_acc_nl)),
          while_for_1_if_acc_nl, and_143_cse);
    end
  end
  always @(posedge clk) begin
    if ( core_wen & or_108_cse ) begin
      memory_channels_aw_channel_rsci_idat_34_31 <= MUX_v_4_2_2(4'b0000, for_1_i_mux_nl,
          memory_send_aw_not_4_nl);
    end
  end
  always @(posedge clk) begin
    if ( memory_send_aw_and_1_cse ) begin
      memory_channels_aw_channel_rsci_idat_30_29 <= MUX_v_2_2_2(2'b00, (output_address_sva_5_0[1:0]),
          memory_send_aw_not_5_nl);
      memory_channels_aw_channel_rsci_idat_92_35 <= MUX_v_58_2_2(58'b0000000000000000000000000000000000000001000000000000000000,
          output_address_sva_63_6, and_133_cse);
      memory_channels_aw_channel_rsci_idat_20 <= ~ or_tmp_40;
    end
  end
  always @(posedge clk) begin
    if ( memory_send_w_and_cse ) begin
      memory_channels_w_channel_rsci_idat_64_1 <= MUX_v_64_2_2(z_out, memory_encode_strb_1_if_6_lshift_itm,
          and_138_cse);
      memory_channels_w_channel_rsci_idat_576_65 <= z_out_1;
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (((reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[4]) & (fsm_output[8]))
        | debug_rsci_idat_15_0_mx0c1) ) begin
      debug_rsci_idat_15_0 <= MUX_v_16_2_2(16'b0000000001011010, out_index_sva, debug_rsci_idat_15_0_mx0c1);
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_1_cse ) begin
      output_line_sva_1_511 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_510_497 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_496 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_4_cse ) begin
      output_line_sva_1_495 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_494_481 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_480 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_7_cse ) begin
      output_line_sva_1_479 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_478_465 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_464 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_10_cse ) begin
      output_line_sva_1_463 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_462_449 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_448 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_13_cse ) begin
      output_line_sva_1_447 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_446_433 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_432 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_16_cse ) begin
      output_line_sva_1_431 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_430_417 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_416 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_19_cse ) begin
      output_line_sva_1_415 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_414_401 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_400 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_22_cse ) begin
      output_line_sva_1_399 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_398_385 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_384 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_25_cse ) begin
      output_line_sva_1_383 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_382_369 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_368 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_28_cse ) begin
      output_line_sva_1_367 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_366_353 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_352 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_31_cse ) begin
      output_line_sva_1_351 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_350_337 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_336 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_34_cse ) begin
      output_line_sva_1_335 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_334_321 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_320 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_37_cse ) begin
      output_line_sva_1_319 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_318_305 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_304 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_40_cse ) begin
      output_line_sva_1_303 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_302_289 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_288 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_43_cse ) begin
      output_line_sva_1_287 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_286_273 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_272 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_46_cse ) begin
      output_line_sva_1_271 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_270_257 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_256 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_49_cse ) begin
      output_line_sva_1_255 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_254_241 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_240 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_52_cse ) begin
      output_line_sva_1_239 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_238_225 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_224 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_55_cse ) begin
      output_line_sva_1_223 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_222_209 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_208 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_58_cse ) begin
      output_line_sva_1_207 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_206_193 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_192 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_61_cse ) begin
      output_line_sva_1_191 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_190_177 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_176 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_64_cse ) begin
      output_line_sva_1_175 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_174_161 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_160 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_67_cse ) begin
      output_line_sva_1_159 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_158_145 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_144 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_70_cse ) begin
      output_line_sva_1_143 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_142_129 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_128 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_73_cse ) begin
      output_line_sva_1_127 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_126_113 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_112 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_76_cse ) begin
      output_line_sva_1_111 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_110_97 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_96 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_79_cse ) begin
      output_line_sva_1_95 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_94_81 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_80 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_82_cse ) begin
      output_line_sva_1_79 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_78_65 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_64 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_85_cse ) begin
      output_line_sva_1_63 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_62_49 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_48 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_88_cse ) begin
      output_line_sva_1_47 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_46_33 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_32 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_91_cse ) begin
      output_line_sva_1_31 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_30_17 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_16 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_line_and_94_cse ) begin
      output_line_sva_1_15 <= while_sum_out_slc_32_8_sat_sva_1[24];
      output_line_sva_1_14_1 <= while_sum_out_while_sum_out_nor_itm;
      output_line_sva_1_0 <= while_sum_out_while_sum_out_nor_1_itm;
    end
  end
  always @(posedge clk) begin
    if ( output_address_and_ssc & ((~ (fsm_output[19])) | output_address_and_1_rgt)
        ) begin
      output_address_sva_63_6 <= MUX_v_58_2_2((signext_58_27(acc_2_sdt[32:6])), operator_64_false_acc_nl_1,
          output_address_and_1_rgt);
    end
  end
  always @(posedge clk) begin
    if ( output_address_and_ssc & (~ (fsm_output[19])) ) begin
      output_address_sva_5_0 <= acc_2_sdt[5:0];
    end
  end
  always @(posedge clk) begin
    if ( output_vector_len_and_cse ) begin
      output_vector_len_sva <= output_vector_len_rsci_idat;
      acc_psp_sva <= nl_acc_psp_sva[32:0];
      num_feature_lines_sva <= z_out_2;
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (out_index_sva_mx0c0 | ((~ exit_for_sva_mx0) & (fsm_output[1]))
        | ((~ exit_for_sva_mx0) & (fsm_output[5])) | (fsm_output[19])) ) begin
      out_index_sva <= MUX_v_16_2_2(16'b0000000000000000, z_out_2, out_index_not_1_nl);
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      for_i_31_14_sva_17_16 <= 2'b00;
      for_i_31_14_sva_15_0 <= 16'b0000000000000000;
    end
    else if ( for_i_and_ssc ) begin
      for_i_31_14_sva_17_16 <= MUX_v_2_2_2(2'b00, (for_i_31_14_sva_2[17:16]), not_229_nl);
      for_i_31_14_sva_15_0 <= MUX_v_16_2_2(16'b0000000000000000, for_i_for_i_mux1h_nl,
          nor_nl);
    end
  end
  always @(posedge clk) begin
    if ( and_745_ssc ) begin
      acc_1_psp_sva_32 <= acc_1_sdt[32];
      acc_1_psp_sva_31_0 <= MUX_v_32_2_2((acc_1_sdt[31:0]), while_for_while_for_and_nl,
          acc_1_psp_sva_mx0c1);
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      weight_index_lpi_3 <= 32'b00000000000000000000000000000000;
    end
    else if ( core_wen & ((fsm_output[14]) | or_dcpl_41) ) begin
      weight_index_lpi_3 <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_acc_1_nl,
          (fsm_output[14]));
    end
  end
  always @(posedge clk) begin
    if ( core_wen & ((fsm_output[1]) | sum_sva_mx0c1 | (fsm_output[18])) ) begin
      sum_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, sum_mux_nl, sum_not_1_nl);
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd <= 2'b00;
      reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1 <= 1'b0;
      reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2 <= 5'b00000;
    end
    else if ( for_feature_burst_size_qelse_and_ssc ) begin
      reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd <= MUX_v_2_2_2(2'b00, for_feature_burst_size_qelse_for_feature_burst_size_qelse_mux_nl,
          not_222_nl);
      reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1 <= for_feature_burst_size_qelse_mux1h_5_nl
          & (~ (fsm_output[12]));
      reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2 <= MUX_v_5_2_2(5'b00000,
          for_feature_burst_size_qelse_mux1h_6_nl, not_241_nl);
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      operator_16_false_slc_operator_16_false_acc_8_mdf_sva <= 1'b0;
    end
    else if ( core_wen & (fsm_output[2]) ) begin
      operator_16_false_slc_operator_16_false_acc_8_mdf_sva <= operator_16_false_slc_operator_16_false_acc_8_mdf_sva_mx0w0;
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1 <= 1'b0;
    end
    else if ( core_wen & ((fsm_output[2]) | (fsm_output[4]) | (fsm_output[12]) |
        (fsm_output[13]) | (fsm_output[14]) | (fsm_output[19])) ) begin
      exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1 <= operator_16_false_mux1h_nl
          | (fsm_output[12]);
    end
  end
  always @(posedge clk) begin
    if ( core_wen ) begin
      memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva <= MUX_v_10_2_2(10'b0000000000,
          memory_axi_burst_read_base_axi_u512_512_for_beat_9_0_sva_mx1, (fsm_output[4]));
      memory_encode_strb_1_if_6_lshift_itm <= z_out;
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      memory_axi_burst_read_base_axi_u512_512_for_stage_0_2 <= 1'b0;
    end
    else if ( core_wen & (or_dcpl_81 | (fsm_output[4]) | (fsm_output[10])) ) begin
      memory_axi_burst_read_base_axi_u512_512_for_stage_0_2 <= (memory_axi_burst_read_base_axi_u512_512_for_stage_0_2_mx0w1
          & (~ or_dcpl_81)) | (fsm_output[10]);
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1 <= 1'b0;
    end
    else if ( core_wen ) begin
      memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_1 <= memory_axi_burst_read_base_axi_u512_512_for_mux_nl
          | (fsm_output[12]) | (fsm_output[3]);
    end
  end
  always @(posedge clk) begin
    if ( sum_array_and_cse ) begin
      sum_array_1_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_2_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_2_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_3_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_3_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_4_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_4_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_5_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_5_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_6_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_6_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_7_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_7_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_8_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_8_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_9_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_9_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_10_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_10_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_11_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_11_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_12_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_12_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_13_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_13_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_14_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_14_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_15_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_15_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_16_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_16_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_17_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_17_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_18_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_18_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_19_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_19_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_20_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_20_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_21_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_21_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_22_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_22_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_23_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_23_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_24_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_24_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_25_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_25_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_26_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_26_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_27_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_27_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_28_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_28_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_29_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_29_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_30_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_30_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_31_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_31_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, while_for_1_for_32_while_for_1_for_acc_2_cmp_z,
          (fsm_output[17]));
      sum_array_31_sva_load <= ~ (fsm_output[17]);
    end
  end
  assign nl_operator_64_false_acc_nl = conv_u2u_18_20({for_i_31_14_sva_17_16 , for_i_31_14_sva_15_0})
      + conv_s2u_19_20({acc_1_psp_sva_32 , (acc_1_psp_sva_31_0[31:14])});
  assign operator_64_false_acc_nl = nl_operator_64_false_acc_nl[19:0];
  assign nl_while_for_1_if_acc_nl = conv_u2u_24_26(weight_index_lpi_3[31:8]) + conv_s2u_19_26(acc_psp_sva[32:14]);
  assign while_for_1_if_acc_nl = nl_while_for_1_if_acc_nl[25:0];
  assign for_1_i_mux_nl = MUX_v_4_2_2((reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[3:0]),
      (output_address_sva_5_0[5:2]), and_133_cse);
  assign memory_send_aw_not_4_nl = ~ and_134_cse;
  assign memory_send_aw_not_5_nl = ~ or_tmp_40;
  assign nl_operator_64_false_acc_nl_1 = output_address_sva_63_6 + 58'b0000000000000000000000000000000000000000000000000000000001;
  assign operator_64_false_acc_nl_1 = nl_operator_64_false_acc_nl_1[57:0];
  assign nl_acc_psp_sva  = (conv_s2s_32_33(addr_hi_rsci_idat) + conv_s2s_32_33(weight_addr_lo_rsci_idat));
  assign out_index_not_1_nl = ~ out_index_sva_mx0c0;
  assign not_229_nl = ~ (fsm_output[1]);
  assign for_i_and_1_nl = (~ id_or_cse) & for_i_31_14_sva_mx0c2;
  assign for_i_for_i_mux1h_nl = MUX1HOT_v_16_3_2((for_i_31_14_sva_2[15:0]), for_i_31_14_sva_15_0,
      z_out_2, {(~ for_i_31_14_sva_mx0c2) , for_i_and_1_nl , id_or_cse});
  assign nor_nl = ~((for_i_31_14_sva_mx0c2 & (~((fsm_output[13:11]!=3'b000))) & (~((fsm_output[15:14]!=2'b00)))
      & (~((fsm_output[17:16]!=2'b00))) & (~((fsm_output[20:19]!=2'b00)))) | (fsm_output[1]));
  assign while_for_mux_nl = MUX_v_32_2_2(while_for_1_for_1_while_for_1_for_acc_2_cmp_z,
      acc_1_psp_sva_31_0, fsm_output[18]);
  assign while_for_while_for_and_nl = MUX_v_32_2_2(32'b00000000000000000000000000000000,
      while_for_mux_nl, sum_array_or_32_cse);
  assign nl_while_for_1_acc_1_nl = weight_index_lpi_3 + 32'b00000000000000000000000000000001;
  assign while_for_1_acc_1_nl = nl_while_for_1_acc_1_nl[31:0];
  assign while_for_2_mux_nl = MUX_v_32_32_2(acc_1_psp_sva_31_0, sum_array_1_sva,
      sum_array_2_sva, sum_array_3_sva, sum_array_4_sva, sum_array_5_sva, sum_array_6_sva,
      sum_array_7_sva, sum_array_8_sva, sum_array_9_sva, sum_array_10_sva, sum_array_11_sva,
      sum_array_12_sva, sum_array_13_sva, sum_array_14_sva, sum_array_15_sva, sum_array_16_sva,
      sum_array_17_sva, sum_array_18_sva, sum_array_19_sva, sum_array_20_sva, sum_array_21_sva,
      sum_array_22_sva, sum_array_23_sva, sum_array_24_sva, sum_array_25_sva, sum_array_26_sva,
      sum_array_27_sva, sum_array_28_sva, sum_array_29_sva, sum_array_30_sva, sum_array_31_sva,
      reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2);
  assign nl_while_for_2_acc_1_nl = sum_sva + while_for_2_mux_nl;
  assign while_for_2_acc_1_nl = nl_while_for_2_acc_1_nl[31:0];
  assign sum_mux_nl = MUX_v_32_2_2(input_vector_len_rsci_idat, while_for_2_acc_1_nl,
      fsm_output[18]);
  assign sum_not_1_nl = ~ sum_sva_mx0c1;
  assign for_feature_burst_size_qelse_for_feature_burst_size_qelse_mux_nl = MUX_v_2_2_2((for_feature_burst_size_qr_7_0_lpi_2_dfm_1[7:6]),
      (memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3[7:6]), fsm_output[13]);
  assign not_222_nl = ~ (fsm_output[12]);
  assign for_feature_burst_size_qelse_mux1h_5_nl = MUX1HOT_s_1_3_2((for_feature_burst_size_qr_7_0_lpi_2_dfm_1[5]),
      (memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3[5]), (output_address_sva_5_0[5]),
      {(fsm_output[2]) , (fsm_output[13]) , (fsm_output[19])});
  assign for_1_i_for_1_i_mux_nl = MUX_v_5_2_2((z_out_3[4:0]), reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2,
      fsm_output[7]);
  assign or_436_nl = (fsm_output[18]) | (fsm_output[7]) | (fsm_output[6]);
  assign for_1_i_and_nl = MUX_v_5_2_2(5'b00000, for_1_i_for_1_i_mux_nl, or_436_nl);
  assign for_feature_burst_size_qelse_mux1h_6_nl = MUX1HOT_v_5_4_2((for_feature_burst_size_qr_7_0_lpi_2_dfm_1[4:0]),
      for_1_i_and_nl, (memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_3[4:0]),
      (output_address_sva_5_0[4:0]), {(fsm_output[2]) , for_feature_burst_size_qr_7_0_lpi_2_dfm_mx0c1
      , (fsm_output[13]) , (fsm_output[19])});
  assign not_241_nl = ~ (fsm_output[12]);
  assign operator_16_false_mux1h_nl = MUX1HOT_s_1_5_2(operator_16_false_operator_16_false_nor_tmp,
      (~ operator_16_false_1_acc_itm_10_1_10_1_10), exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1_mx0w3,
      (~ (z_out_2 < num_feature_lines_sva)), (~ ($signed(conv_u2s_16_32(z_out_2))
      < $signed(output_vector_len_sva))), {(fsm_output[2]) , (fsm_output[4]) , (fsm_output[13])
      , (fsm_output[14]) , (fsm_output[19])});
  assign memory_axi_burst_read_base_axi_u512_512_for_mux_nl = MUX_s_1_2_2(memory_axi_burst_read_base_axi_u512_512_for_stage_0_2_mx0w1,
      exit_memory_axi_burst_read_base_axi_u512_512_for_sva_1_mx0w3, fsm_output[13]);
  assign for_or_4_seb = (fsm_output[1]) | id_or_cse;
  assign for_for_or_cse = for_nor_1_cse | (fsm_output[5]);
  assign for_or_nl = (fsm_output[5]) | (fsm_output[19]);
  assign for_mux1h_nl = MUX1HOT_v_16_3_2(out_index_sva, (input_vector_len_rsci_idat[20:5]),
      for_i_31_14_sva_15_0, {for_or_nl , (fsm_output[1]) , (fsm_output[14])});
  assign for_for_and_nl = operator_16_false_slc_operator_16_false_acc_8_mdf_sva &
      for_nor_1_cse;
  assign for_for_for_nor_nl = ~(MUX_v_2_2_2(reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd,
      2'b11, for_or_4_seb));
  assign for_for_for_nor_1_nl = ~(reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_1
      | for_or_4_seb);
  assign for_for_for_nor_2_nl = ~(MUX_v_4_2_2((reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[4:1]),
      4'b1111, for_or_4_seb));
  assign operator_33_true_1_or_nl = (input_vector_len_rsci_idat[4:0]!=5'b00000);
  assign for_mux_1_nl = MUX_s_1_2_2((~ (reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[0])),
      operator_33_true_1_or_nl, fsm_output[1]);
  assign for_for_or_1_nl = for_mux_1_nl | id_or_cse;
  assign nl_z_out_2 = for_mux1h_nl + conv_s2u_10_16({for_for_or_cse , for_for_and_nl
      , for_for_for_nor_nl , for_for_for_nor_1_nl , for_for_for_nor_2_nl , for_for_or_1_nl})
      + conv_u2u_1_16(for_for_or_cse);
  assign z_out_2 = nl_z_out_2[15:0];
  assign operator_16_false_nor_nl = ~((fsm_output[6]) | (fsm_output[18]));
  assign operator_16_false_operator_16_false_and_nl = MUX_v_3_2_2(3'b000, (for_feature_burst_size_qr_7_0_lpi_2_dfm_1[7:5]),
      operator_16_false_nor_nl);
  assign operator_16_false_mux_nl = MUX_s_1_2_2((for_feature_burst_size_qr_7_0_lpi_2_dfm_1[4]),
      (reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[4]), fsm_output[18]);
  assign operator_16_false_operator_16_false_and_1_nl = operator_16_false_mux_nl
      & (~ (fsm_output[6]));
  assign operator_16_false_mux1h_3_nl = MUX1HOT_v_4_3_2((for_feature_burst_size_qr_7_0_lpi_2_dfm_1[3:0]),
      memory_channels_aw_channel_rsci_idat_34_31, (reg_for_feature_burst_size_qr_7_0_lpi_2_dfm_ftd_2[3:0]),
      {(fsm_output[2]) , (fsm_output[6]) , (fsm_output[18])});
  assign nl_z_out_3 = ({operator_16_false_operator_16_false_and_nl , operator_16_false_operator_16_false_and_1_nl
      , operator_16_false_mux1h_3_nl}) + conv_s2u_2_8({(fsm_output[2]) , 1'b1});
  assign z_out_3 = nl_z_out_3[7:0];

  function automatic  MUX1HOT_s_1_3_2;
    input  input_2;
    input  input_1;
    input  input_0;
    input [2:0] sel;
    reg  result;
  begin
    result = input_0 & sel[0];
    result = result | (input_1 & sel[1]);
    result = result | (input_2 & sel[2]);
    MUX1HOT_s_1_3_2 = result;
  end
  endfunction


  function automatic  MUX1HOT_s_1_5_2;
    input  input_4;
    input  input_3;
    input  input_2;
    input  input_1;
    input  input_0;
    input [4:0] sel;
    reg  result;
  begin
    result = input_0 & sel[0];
    result = result | (input_1 & sel[1]);
    result = result | (input_2 & sel[2]);
    result = result | (input_3 & sel[3]);
    result = result | (input_4 & sel[4]);
    MUX1HOT_s_1_5_2 = result;
  end
  endfunction


  function automatic [15:0] MUX1HOT_v_16_3_2;
    input [15:0] input_2;
    input [15:0] input_1;
    input [15:0] input_0;
    input [2:0] sel;
    reg [15:0] result;
  begin
    result = input_0 & {16{sel[0]}};
    result = result | (input_1 & {16{sel[1]}});
    result = result | (input_2 & {16{sel[2]}});
    MUX1HOT_v_16_3_2 = result;
  end
  endfunction


  function automatic [3:0] MUX1HOT_v_4_3_2;
    input [3:0] input_2;
    input [3:0] input_1;
    input [3:0] input_0;
    input [2:0] sel;
    reg [3:0] result;
  begin
    result = input_0 & {4{sel[0]}};
    result = result | (input_1 & {4{sel[1]}});
    result = result | (input_2 & {4{sel[2]}});
    MUX1HOT_v_4_3_2 = result;
  end
  endfunction


  function automatic [4:0] MUX1HOT_v_5_4_2;
    input [4:0] input_3;
    input [4:0] input_2;
    input [4:0] input_1;
    input [4:0] input_0;
    input [3:0] sel;
    reg [4:0] result;
  begin
    result = input_0 & {5{sel[0]}};
    result = result | (input_1 & {5{sel[1]}});
    result = result | (input_2 & {5{sel[2]}});
    result = result | (input_3 & {5{sel[3]}});
    MUX1HOT_v_5_4_2 = result;
  end
  endfunction


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


  function automatic [9:0] MUX_v_10_2_2;
    input [9:0] input_0;
    input [9:0] input_1;
    input  sel;
    reg [9:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_10_2_2 = result;
  end
  endfunction


  function automatic [10:0] MUX_v_11_2_2;
    input [10:0] input_0;
    input [10:0] input_1;
    input  sel;
    reg [10:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_11_2_2 = result;
  end
  endfunction


  function automatic [13:0] MUX_v_14_2_2;
    input [13:0] input_0;
    input [13:0] input_1;
    input  sel;
    reg [13:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_14_2_2 = result;
  end
  endfunction


  function automatic [15:0] MUX_v_16_2_2;
    input [15:0] input_0;
    input [15:0] input_1;
    input  sel;
    reg [15:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_16_2_2 = result;
  end
  endfunction


  function automatic [25:0] MUX_v_26_2_2;
    input [25:0] input_0;
    input [25:0] input_1;
    input  sel;
    reg [25:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_26_2_2 = result;
  end
  endfunction


  function automatic [1:0] MUX_v_2_2_2;
    input [1:0] input_0;
    input [1:0] input_1;
    input  sel;
    reg [1:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_2_2_2 = result;
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


  function automatic [31:0] MUX_v_32_32_2;
    input [31:0] input_0;
    input [31:0] input_1;
    input [31:0] input_2;
    input [31:0] input_3;
    input [31:0] input_4;
    input [31:0] input_5;
    input [31:0] input_6;
    input [31:0] input_7;
    input [31:0] input_8;
    input [31:0] input_9;
    input [31:0] input_10;
    input [31:0] input_11;
    input [31:0] input_12;
    input [31:0] input_13;
    input [31:0] input_14;
    input [31:0] input_15;
    input [31:0] input_16;
    input [31:0] input_17;
    input [31:0] input_18;
    input [31:0] input_19;
    input [31:0] input_20;
    input [31:0] input_21;
    input [31:0] input_22;
    input [31:0] input_23;
    input [31:0] input_24;
    input [31:0] input_25;
    input [31:0] input_26;
    input [31:0] input_27;
    input [31:0] input_28;
    input [31:0] input_29;
    input [31:0] input_30;
    input [31:0] input_31;
    input [4:0] sel;
    reg [31:0] result;
  begin
    case (sel)
      5'b00000 : begin
        result = input_0;
      end
      5'b00001 : begin
        result = input_1;
      end
      5'b00010 : begin
        result = input_2;
      end
      5'b00011 : begin
        result = input_3;
      end
      5'b00100 : begin
        result = input_4;
      end
      5'b00101 : begin
        result = input_5;
      end
      5'b00110 : begin
        result = input_6;
      end
      5'b00111 : begin
        result = input_7;
      end
      5'b01000 : begin
        result = input_8;
      end
      5'b01001 : begin
        result = input_9;
      end
      5'b01010 : begin
        result = input_10;
      end
      5'b01011 : begin
        result = input_11;
      end
      5'b01100 : begin
        result = input_12;
      end
      5'b01101 : begin
        result = input_13;
      end
      5'b01110 : begin
        result = input_14;
      end
      5'b01111 : begin
        result = input_15;
      end
      5'b10000 : begin
        result = input_16;
      end
      5'b10001 : begin
        result = input_17;
      end
      5'b10010 : begin
        result = input_18;
      end
      5'b10011 : begin
        result = input_19;
      end
      5'b10100 : begin
        result = input_20;
      end
      5'b10101 : begin
        result = input_21;
      end
      5'b10110 : begin
        result = input_22;
      end
      5'b10111 : begin
        result = input_23;
      end
      5'b11000 : begin
        result = input_24;
      end
      5'b11001 : begin
        result = input_25;
      end
      5'b11010 : begin
        result = input_26;
      end
      5'b11011 : begin
        result = input_27;
      end
      5'b11100 : begin
        result = input_28;
      end
      5'b11101 : begin
        result = input_29;
      end
      5'b11110 : begin
        result = input_30;
      end
      default : begin
        result = input_31;
      end
    endcase
    MUX_v_32_32_2 = result;
  end
  endfunction


  function automatic [2:0] MUX_v_3_2_2;
    input [2:0] input_0;
    input [2:0] input_1;
    input  sel;
    reg [2:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_3_2_2 = result;
  end
  endfunction


  function automatic [3:0] MUX_v_4_2_2;
    input [3:0] input_0;
    input [3:0] input_1;
    input  sel;
    reg [3:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_4_2_2 = result;
  end
  endfunction


  function automatic [57:0] MUX_v_58_2_2;
    input [57:0] input_0;
    input [57:0] input_1;
    input  sel;
    reg [57:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_58_2_2 = result;
  end
  endfunction


  function automatic [4:0] MUX_v_5_2_2;
    input [4:0] input_0;
    input [4:0] input_1;
    input  sel;
    reg [4:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_5_2_2 = result;
  end
  endfunction


  function automatic [63:0] MUX_v_64_2_2;
    input [63:0] input_0;
    input [63:0] input_1;
    input  sel;
    reg [63:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_64_2_2 = result;
  end
  endfunction


  function automatic [7:0] MUX_v_8_2_2;
    input [7:0] input_0;
    input [7:0] input_1;
    input  sel;
    reg [7:0] result;
  begin
    case (sel)
      1'b0 : begin
        result = input_0;
      end
      default : begin
        result = input_1;
      end
    endcase
    MUX_v_8_2_2 = result;
  end
  endfunction


  function automatic [0:0] readslicef_11_1_10;
    input [10:0] vector;
    reg [10:0] tmp;
  begin
    tmp = vector >> 10;
    readslicef_11_1_10 = tmp[0:0];
  end
  endfunction


  function automatic [25:0] signext_26_20;
    input [19:0] vector;
  begin
    signext_26_20= {{6{vector[19]}}, vector};
  end
  endfunction


  function automatic [57:0] signext_58_27;
    input [26:0] vector;
  begin
    signext_58_27= {{31{vector[26]}}, vector};
  end
  endfunction


  function automatic [63:0] signext_64_5;
    input [4:0] vector;
  begin
    signext_64_5= {{59{vector[4]}}, vector};
  end
  endfunction


  function automatic [10:0] conv_s2s_10_11 ;
    input [9:0]  vector ;
  begin
    conv_s2s_10_11 = {vector[9], vector};
  end
  endfunction


  function automatic [32:0] conv_s2s_32_33 ;
    input [31:0]  vector ;
  begin
    conv_s2s_32_33 = {vector[31], vector};
  end
  endfunction


  function automatic [7:0] conv_s2u_2_8 ;
    input [1:0]  vector ;
  begin
    conv_s2u_2_8 = {{6{vector[1]}}, vector};
  end
  endfunction


  function automatic [15:0] conv_s2u_10_16 ;
    input [9:0]  vector ;
  begin
    conv_s2u_10_16 = {{6{vector[9]}}, vector};
  end
  endfunction


  function automatic [19:0] conv_s2u_19_20 ;
    input [18:0]  vector ;
  begin
    conv_s2u_19_20 = {vector[18], vector};
  end
  endfunction


  function automatic [25:0] conv_s2u_19_26 ;
    input [18:0]  vector ;
  begin
    conv_s2u_19_26 = {{7{vector[18]}}, vector};
  end
  endfunction


  function automatic [24:0] conv_s2u_24_25 ;
    input [23:0]  vector ;
  begin
    conv_s2u_24_25 = {vector[23], vector};
  end
  endfunction


  function automatic [8:0] conv_u2s_8_9 ;
    input [7:0]  vector ;
  begin
    conv_u2s_8_9 =  {1'b0, vector};
  end
  endfunction


  function automatic [9:0] conv_u2s_9_10 ;
    input [8:0]  vector ;
  begin
    conv_u2s_9_10 =  {1'b0, vector};
  end
  endfunction


  function automatic [10:0] conv_u2s_10_11 ;
    input [9:0]  vector ;
  begin
    conv_u2s_10_11 =  {1'b0, vector};
  end
  endfunction


  function automatic [31:0] conv_u2s_16_32 ;
    input [15:0]  vector ;
  begin
    conv_u2s_16_32 = {{16{1'b0}}, vector};
  end
  endfunction


  function automatic [32:0] conv_u2s_32_33 ;
    input [31:0]  vector ;
  begin
    conv_u2s_32_33 =  {1'b0, vector};
  end
  endfunction


  function automatic [15:0] conv_u2u_1_16 ;
    input  vector ;
  begin
    conv_u2u_1_16 = {{15{1'b0}}, vector};
  end
  endfunction


  function automatic [24:0] conv_u2u_1_25 ;
    input  vector ;
  begin
    conv_u2u_1_25 = {{24{1'b0}}, vector};
  end
  endfunction


  function automatic [19:0] conv_u2u_18_20 ;
    input [17:0]  vector ;
  begin
    conv_u2u_18_20 = {{2{1'b0}}, vector};
  end
  endfunction


  function automatic [25:0] conv_u2u_24_26 ;
    input [23:0]  vector ;
  begin
    conv_u2u_24_26 = {{2{1'b0}}, vector};
  end
  endfunction

endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_struct
// ------------------------------------------------------------------


module dense_struct (
  clk, arst_n, start_rsc_dat, start_rsc_vld, start_rsc_rdy, done_rsc_dat, done_rsc_vld,
      done_rsc_rdy, use_relu_triosy_lz, addr_hi_rsc_dat, addr_hi_triosy_lz, feature_addr_lo_rsc_dat,
      feature_addr_lo_triosy_lz, weight_addr_lo_rsc_dat, weight_addr_lo_triosy_lz,
      output_addr_lo_rsc_dat, output_addr_lo_triosy_lz, input_vector_len_rsc_dat,
      input_vector_len_triosy_lz, output_vector_len_rsc_dat, output_vector_len_triosy_lz,
      debug_rsc_dat, debug_triosy_lz, memory_channels_aw_channel_rsc_dat_id, memory_channels_aw_channel_rsc_dat_address,
      memory_channels_aw_channel_rsc_dat_len, memory_channels_aw_channel_rsc_dat_size,
      memory_channels_aw_channel_rsc_dat_burst, memory_channels_aw_channel_rsc_dat_lock,
      memory_channels_aw_channel_rsc_dat_cache, memory_channels_aw_channel_rsc_dat_prot,
      memory_channels_aw_channel_rsc_dat_region, memory_channels_aw_channel_rsc_dat_qos,
      memory_channels_aw_channel_rsc_vld, memory_channels_aw_channel_rsc_rdy, memory_channels_w_channel_rsc_dat_data,
      memory_channels_w_channel_rsc_dat_strb, memory_channels_w_channel_rsc_dat_last,
      memory_channels_w_channel_rsc_vld, memory_channels_w_channel_rsc_rdy, memory_channels_b_channel_rsc_dat_id,
      memory_channels_b_channel_rsc_dat_resp, memory_channels_b_channel_rsc_vld,
      memory_channels_b_channel_rsc_rdy, memory_channels_ar_channel_rsc_dat_id, memory_channels_ar_channel_rsc_dat_address,
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
  input start_rsc_dat;
  input start_rsc_vld;
  output start_rsc_rdy;
  output done_rsc_dat;
  output done_rsc_vld;
  input done_rsc_rdy;
  output use_relu_triosy_lz;
  input [31:0] addr_hi_rsc_dat;
  output addr_hi_triosy_lz;
  input [31:0] feature_addr_lo_rsc_dat;
  output feature_addr_lo_triosy_lz;
  input [31:0] weight_addr_lo_rsc_dat;
  output weight_addr_lo_triosy_lz;
  input [31:0] output_addr_lo_rsc_dat;
  output output_addr_lo_triosy_lz;
  input [31:0] input_vector_len_rsc_dat;
  output input_vector_len_triosy_lz;
  input [31:0] output_vector_len_rsc_dat;
  output output_vector_len_triosy_lz;
  output [31:0] debug_rsc_dat;
  output debug_triosy_lz;
  output [15:0] memory_channels_aw_channel_rsc_dat_id;
  output [63:0] memory_channels_aw_channel_rsc_dat_address;
  output [7:0] memory_channels_aw_channel_rsc_dat_len;
  output [2:0] memory_channels_aw_channel_rsc_dat_size;
  output [1:0] memory_channels_aw_channel_rsc_dat_burst;
  output memory_channels_aw_channel_rsc_dat_lock;
  output [3:0] memory_channels_aw_channel_rsc_dat_cache;
  output [2:0] memory_channels_aw_channel_rsc_dat_prot;
  output [3:0] memory_channels_aw_channel_rsc_dat_region;
  output [3:0] memory_channels_aw_channel_rsc_dat_qos;
  output memory_channels_aw_channel_rsc_vld;
  input memory_channels_aw_channel_rsc_rdy;
  output [511:0] memory_channels_w_channel_rsc_dat_data;
  output [63:0] memory_channels_w_channel_rsc_dat_strb;
  output memory_channels_w_channel_rsc_dat_last;
  output memory_channels_w_channel_rsc_vld;
  input memory_channels_w_channel_rsc_rdy;
  input [15:0] memory_channels_b_channel_rsc_dat_id;
  input [1:0] memory_channels_b_channel_rsc_dat_resp;
  input memory_channels_b_channel_rsc_vld;
  output memory_channels_b_channel_rsc_rdy;
  output [15:0] memory_channels_ar_channel_rsc_dat_id;
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
  input [15:0] memory_channels_r_channel_rsc_dat_id;
  input [511:0] memory_channels_r_channel_rsc_dat_data;
  input [1:0] memory_channels_r_channel_rsc_dat_resp;
  input memory_channels_r_channel_rsc_dat_last;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;


  // Interconnect Declarations
  wire feature_memory_rsci_clken_d;
  wire [511:0] feature_memory_rsci_q_d;
  wire [14:0] feature_memory_rsci_radr_d;
  wire [14:0] feature_memory_rsci_wadr_d;
  wire [511:0] weight_memory_rsci_q_d;
  wire [7:0] weight_memory_rsci_radr_d;
  wire [7:0] weight_memory_rsci_wadr_d;
  wire [15:0] while_for_1_for_32_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_32_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_32_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_31_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_31_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_31_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_30_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_30_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_30_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_29_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_29_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_29_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_28_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_28_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_28_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_27_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_27_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_27_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_26_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_26_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_26_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_25_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_25_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_25_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_24_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_24_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_24_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_23_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_23_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_23_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_22_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_22_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_22_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_21_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_21_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_21_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_20_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_20_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_20_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_19_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_19_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_19_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_18_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_18_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_18_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_17_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_17_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_17_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_16_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_16_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_16_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_15_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_15_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_15_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_14_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_14_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_14_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_13_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_13_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_13_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_12_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_12_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_12_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_11_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_11_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_11_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_10_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_10_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_10_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_9_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_9_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_9_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_8_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_8_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_8_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_7_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_7_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_7_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_6_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_6_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_6_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_5_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_5_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_5_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_4_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_4_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_4_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_3_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_3_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_3_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_2_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_2_while_for_1_for_acc_2_cmp_b;
  wire [31:0] while_for_1_for_2_while_for_1_for_acc_2_cmp_z;
  wire [15:0] while_for_1_for_1_while_for_1_for_acc_2_cmp_a;
  wire [15:0] while_for_1_for_1_while_for_1_for_acc_2_cmp_b;
  wire while_for_1_for_1_while_for_1_for_acc_2_cmp_load;
  wire [31:0] while_for_1_for_1_while_for_1_for_acc_2_cmp_z;
  wire feature_memory_rsc_clken;
  wire [511:0] feature_memory_rsc_q;
  wire feature_memory_rsc_re;
  wire [14:0] feature_memory_rsc_radr;
  wire feature_memory_rsc_we;
  wire [511:0] feature_memory_rsc_d;
  wire [14:0] feature_memory_rsc_wadr;
  wire weight_memory_rsc_clken;
  wire [511:0] weight_memory_rsc_q;
  wire weight_memory_rsc_re;
  wire [7:0] weight_memory_rsc_radr;
  wire weight_memory_rsc_we;
  wire [511:0] weight_memory_rsc_d;
  wire [7:0] weight_memory_rsc_wadr;
  wire [108:0] memory_channels_aw_channel_rsc_dat;
  wire [576:0] memory_channels_w_channel_rsc_dat;
  wire [108:0] memory_channels_ar_channel_rsc_dat;
  wire while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff;
  wire while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff;
  wire [511:0] feature_memory_rsci_d_d_iff;
  wire feature_memory_rsci_re_d_iff;
  wire feature_memory_rsci_we_d_iff;
  wire weight_memory_rsci_we_d_iff;


  // Interconnect Declarations for Component Instantiations 
  wire  nl_while_for_1_for_32_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_32_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_31_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_31_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_30_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_30_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_29_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_29_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_28_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_28_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_27_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_27_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_26_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_26_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_25_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_25_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_24_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_24_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_23_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_23_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_22_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_22_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_21_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_21_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_20_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_20_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_19_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_19_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_18_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_18_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_17_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_17_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_16_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_16_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_15_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_15_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_14_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_14_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_13_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_13_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_12_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_12_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_11_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_11_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_10_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_10_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_9_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_9_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_8_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_8_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_7_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_7_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_6_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_6_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_5_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_5_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_4_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_4_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_3_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_3_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_2_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_2_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire  nl_while_for_1_for_1_while_for_1_for_acc_2_cmp_clk;
  assign nl_while_for_1_for_1_while_for_1_for_acc_2_cmp_clk = ~ feature_memory_rsci_clken_d;
  wire [17:0] nl_dense_core_inst_memory_channels_b_channel_rsc_dat;
  assign nl_dense_core_inst_memory_channels_b_channel_rsc_dat = {memory_channels_b_channel_rsc_dat_id
      , memory_channels_b_channel_rsc_dat_resp};
  wire [530:0] nl_dense_core_inst_memory_channels_r_channel_rsc_dat;
  assign nl_dense_core_inst_memory_channels_r_channel_rsc_dat = {memory_channels_r_channel_rsc_dat_id
      , memory_channels_r_channel_rsc_dat_data , memory_channels_r_channel_rsc_dat_resp
      , memory_channels_r_channel_rsc_dat_last};
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_32_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_32_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_32_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_32_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_32_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_31_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_31_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_31_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_31_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_31_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_30_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_30_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_30_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_30_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_30_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_29_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_29_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_29_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_29_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_29_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_28_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_28_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_28_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_28_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_28_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_27_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_27_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_27_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_27_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_27_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_26_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_26_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_26_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_26_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_26_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_25_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_25_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_25_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_25_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_25_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_24_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_24_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_24_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_24_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_24_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_23_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_23_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_23_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_23_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_23_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_22_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_22_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_22_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_22_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_22_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_21_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_21_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_21_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_21_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_21_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_20_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_20_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_20_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_20_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_20_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_19_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_19_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_19_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_19_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_19_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_18_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_18_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_18_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_18_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_18_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_17_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_17_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_17_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_17_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_17_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_16_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_16_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_16_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_16_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_16_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_15_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_15_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_15_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_15_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_15_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_14_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_14_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_14_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_14_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_14_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_13_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_13_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_13_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_13_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_13_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_12_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_12_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_12_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_12_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_12_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_11_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_11_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_11_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_11_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_11_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_10_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_10_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_10_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_10_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_10_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_9_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_9_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_9_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_9_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_9_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_8_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_8_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_8_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_8_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_8_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_7_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_7_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_7_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_7_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_7_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_6_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_6_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_6_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_6_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_6_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_5_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_5_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_5_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_5_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_5_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_4_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_4_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_4_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_4_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_4_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_3_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_3_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_3_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_3_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_3_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_2_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_2_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_2_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_2_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_2_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  mgc_mulacc_pipe #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_d(32'sd1),
  .is_square(32'sd0),
  .clock_edge(32'sd1),
  .enable_active(32'sd0),
  .a_rst_active(32'sd0),
  .s_rst_active(32'sd0),
  .stages(32'sd2),
  .n_inreg(32'sd1)) while_for_1_for_1_while_for_1_for_acc_2_cmp (
      .a(while_for_1_for_1_while_for_1_for_acc_2_cmp_a),
      .b(while_for_1_for_1_while_for_1_for_acc_2_cmp_b),
      .c(32'b00000000000000000000000000000000),
      .load(while_for_1_for_1_while_for_1_for_acc_2_cmp_load),
      .datavalid(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .clk(clk),
      .en(nl_while_for_1_for_1_while_for_1_for_acc_2_cmp_clk),
      .a_rst(arst_n),
      .s_rst(1'b1),
      .z(while_for_1_for_1_while_for_1_for_acc_2_cmp_z),
      .d(2'b0)
    );
  BLOCK_1R1W_RBW #(.addr_width(32'sd15),
  .data_width(32'sd512),
  .depth(32'sd32768),
  .latency(32'sd1),
  .suppress_sim_read_addr_range_errs(32'sd1)) feature_memory_rsc_comp (
      .clk(clk),
      .clken(feature_memory_rsc_clken),
      .d(feature_memory_rsc_d),
      .q(feature_memory_rsc_q),
      .radr(feature_memory_rsc_radr),
      .re(feature_memory_rsc_re),
      .wadr(feature_memory_rsc_wadr),
      .we(feature_memory_rsc_we)
    );
  BLOCK_1R1W_RBW #(.addr_width(32'sd8),
  .data_width(32'sd512),
  .depth(32'sd256),
  .latency(32'sd1),
  .suppress_sim_read_addr_range_errs(32'sd1)) weight_memory_rsc_comp (
      .clk(clk),
      .clken(weight_memory_rsc_clken),
      .d(weight_memory_rsc_d),
      .q(weight_memory_rsc_q),
      .radr(weight_memory_rsc_radr),
      .re(weight_memory_rsc_re),
      .wadr(weight_memory_rsc_wadr),
      .we(weight_memory_rsc_we)
    );
  dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_17_15_512_32768_1_32768_512_1_gen feature_memory_rsci
      (
      .clken(feature_memory_rsc_clken),
      .q(feature_memory_rsc_q),
      .re(feature_memory_rsc_re),
      .radr(feature_memory_rsc_radr),
      .we(feature_memory_rsc_we),
      .d(feature_memory_rsc_d),
      .wadr(feature_memory_rsc_wadr),
      .clken_d(feature_memory_rsci_clken_d),
      .d_d(feature_memory_rsci_d_d_iff),
      .q_d(feature_memory_rsci_q_d),
      .radr_d(feature_memory_rsci_radr_d),
      .re_d(feature_memory_rsci_re_d_iff),
      .wadr_d(feature_memory_rsci_wadr_d),
      .we_d(feature_memory_rsci_we_d_iff),
      .writeA_w_ram_ir_internal_WMASK_B_d(feature_memory_rsci_we_d_iff),
      .readA_r_ram_ir_internal_RMASK_B_d(feature_memory_rsci_re_d_iff)
    );
  dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_18_8_512_256_1_256_512_1_gen weight_memory_rsci
      (
      .clken(weight_memory_rsc_clken),
      .q(weight_memory_rsc_q),
      .re(weight_memory_rsc_re),
      .radr(weight_memory_rsc_radr),
      .we(weight_memory_rsc_we),
      .d(weight_memory_rsc_d),
      .wadr(weight_memory_rsc_wadr),
      .clken_d(feature_memory_rsci_clken_d),
      .d_d(feature_memory_rsci_d_d_iff),
      .q_d(weight_memory_rsci_q_d),
      .radr_d(weight_memory_rsci_radr_d),
      .re_d(feature_memory_rsci_re_d_iff),
      .wadr_d(weight_memory_rsci_wadr_d),
      .we_d(weight_memory_rsci_we_d_iff),
      .writeA_w_ram_ir_internal_WMASK_B_d(weight_memory_rsci_we_d_iff),
      .readA_r_ram_ir_internal_RMASK_B_d(feature_memory_rsci_re_d_iff)
    );
  dense_core dense_core_inst (
      .clk(clk),
      .arst_n(arst_n),
      .start_rsc_dat(start_rsc_dat),
      .start_rsc_vld(start_rsc_vld),
      .start_rsc_rdy(start_rsc_rdy),
      .done_rsc_dat(done_rsc_dat),
      .done_rsc_vld(done_rsc_vld),
      .done_rsc_rdy(done_rsc_rdy),
      .use_relu_triosy_lz(use_relu_triosy_lz),
      .addr_hi_rsc_dat(addr_hi_rsc_dat),
      .addr_hi_triosy_lz(addr_hi_triosy_lz),
      .feature_addr_lo_rsc_dat(feature_addr_lo_rsc_dat),
      .feature_addr_lo_triosy_lz(feature_addr_lo_triosy_lz),
      .weight_addr_lo_rsc_dat(weight_addr_lo_rsc_dat),
      .weight_addr_lo_triosy_lz(weight_addr_lo_triosy_lz),
      .output_addr_lo_rsc_dat(output_addr_lo_rsc_dat),
      .output_addr_lo_triosy_lz(output_addr_lo_triosy_lz),
      .input_vector_len_rsc_dat(input_vector_len_rsc_dat),
      .input_vector_len_triosy_lz(input_vector_len_triosy_lz),
      .output_vector_len_rsc_dat(output_vector_len_rsc_dat),
      .output_vector_len_triosy_lz(output_vector_len_triosy_lz),
      .debug_rsc_dat(debug_rsc_dat),
      .debug_triosy_lz(debug_triosy_lz),
      .memory_channels_aw_channel_rsc_dat(memory_channels_aw_channel_rsc_dat),
      .memory_channels_aw_channel_rsc_vld(memory_channels_aw_channel_rsc_vld),
      .memory_channels_aw_channel_rsc_rdy(memory_channels_aw_channel_rsc_rdy),
      .memory_channels_w_channel_rsc_dat(memory_channels_w_channel_rsc_dat),
      .memory_channels_w_channel_rsc_vld(memory_channels_w_channel_rsc_vld),
      .memory_channels_w_channel_rsc_rdy(memory_channels_w_channel_rsc_rdy),
      .memory_channels_b_channel_rsc_dat(nl_dense_core_inst_memory_channels_b_channel_rsc_dat[17:0]),
      .memory_channels_b_channel_rsc_vld(memory_channels_b_channel_rsc_vld),
      .memory_channels_b_channel_rsc_rdy(memory_channels_b_channel_rsc_rdy),
      .memory_channels_ar_channel_rsc_dat(memory_channels_ar_channel_rsc_dat),
      .memory_channels_ar_channel_rsc_vld(memory_channels_ar_channel_rsc_vld),
      .memory_channels_ar_channel_rsc_rdy(memory_channels_ar_channel_rsc_rdy),
      .memory_channels_r_channel_rsc_dat(nl_dense_core_inst_memory_channels_r_channel_rsc_dat[530:0]),
      .memory_channels_r_channel_rsc_vld(memory_channels_r_channel_rsc_vld),
      .memory_channels_r_channel_rsc_rdy(memory_channels_r_channel_rsc_rdy),
      .feature_memory_rsci_clken_d(feature_memory_rsci_clken_d),
      .feature_memory_rsci_q_d(feature_memory_rsci_q_d),
      .feature_memory_rsci_radr_d(feature_memory_rsci_radr_d),
      .feature_memory_rsci_wadr_d(feature_memory_rsci_wadr_d),
      .weight_memory_rsci_q_d(weight_memory_rsci_q_d),
      .weight_memory_rsci_radr_d(weight_memory_rsci_radr_d),
      .weight_memory_rsci_wadr_d(weight_memory_rsci_wadr_d),
      .while_for_1_for_32_while_for_1_for_acc_2_cmp_a(while_for_1_for_32_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_32_while_for_1_for_acc_2_cmp_b(while_for_1_for_32_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_32_while_for_1_for_acc_2_cmp_z(while_for_1_for_32_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_31_while_for_1_for_acc_2_cmp_a(while_for_1_for_31_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_31_while_for_1_for_acc_2_cmp_b(while_for_1_for_31_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_31_while_for_1_for_acc_2_cmp_z(while_for_1_for_31_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_30_while_for_1_for_acc_2_cmp_a(while_for_1_for_30_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_30_while_for_1_for_acc_2_cmp_b(while_for_1_for_30_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_30_while_for_1_for_acc_2_cmp_z(while_for_1_for_30_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_29_while_for_1_for_acc_2_cmp_a(while_for_1_for_29_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_29_while_for_1_for_acc_2_cmp_b(while_for_1_for_29_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_29_while_for_1_for_acc_2_cmp_z(while_for_1_for_29_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_28_while_for_1_for_acc_2_cmp_a(while_for_1_for_28_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_28_while_for_1_for_acc_2_cmp_b(while_for_1_for_28_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_28_while_for_1_for_acc_2_cmp_z(while_for_1_for_28_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_27_while_for_1_for_acc_2_cmp_a(while_for_1_for_27_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_27_while_for_1_for_acc_2_cmp_b(while_for_1_for_27_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_27_while_for_1_for_acc_2_cmp_z(while_for_1_for_27_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_26_while_for_1_for_acc_2_cmp_a(while_for_1_for_26_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_26_while_for_1_for_acc_2_cmp_b(while_for_1_for_26_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_26_while_for_1_for_acc_2_cmp_z(while_for_1_for_26_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_25_while_for_1_for_acc_2_cmp_a(while_for_1_for_25_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_25_while_for_1_for_acc_2_cmp_b(while_for_1_for_25_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_25_while_for_1_for_acc_2_cmp_z(while_for_1_for_25_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_24_while_for_1_for_acc_2_cmp_a(while_for_1_for_24_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_24_while_for_1_for_acc_2_cmp_b(while_for_1_for_24_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_24_while_for_1_for_acc_2_cmp_z(while_for_1_for_24_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_23_while_for_1_for_acc_2_cmp_a(while_for_1_for_23_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_23_while_for_1_for_acc_2_cmp_b(while_for_1_for_23_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_23_while_for_1_for_acc_2_cmp_z(while_for_1_for_23_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_22_while_for_1_for_acc_2_cmp_a(while_for_1_for_22_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_22_while_for_1_for_acc_2_cmp_b(while_for_1_for_22_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_22_while_for_1_for_acc_2_cmp_z(while_for_1_for_22_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_21_while_for_1_for_acc_2_cmp_a(while_for_1_for_21_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_21_while_for_1_for_acc_2_cmp_b(while_for_1_for_21_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_21_while_for_1_for_acc_2_cmp_z(while_for_1_for_21_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_20_while_for_1_for_acc_2_cmp_a(while_for_1_for_20_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_20_while_for_1_for_acc_2_cmp_b(while_for_1_for_20_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_20_while_for_1_for_acc_2_cmp_z(while_for_1_for_20_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_19_while_for_1_for_acc_2_cmp_a(while_for_1_for_19_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_19_while_for_1_for_acc_2_cmp_b(while_for_1_for_19_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_19_while_for_1_for_acc_2_cmp_z(while_for_1_for_19_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_18_while_for_1_for_acc_2_cmp_a(while_for_1_for_18_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_18_while_for_1_for_acc_2_cmp_b(while_for_1_for_18_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_18_while_for_1_for_acc_2_cmp_z(while_for_1_for_18_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_17_while_for_1_for_acc_2_cmp_a(while_for_1_for_17_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_17_while_for_1_for_acc_2_cmp_b(while_for_1_for_17_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_17_while_for_1_for_acc_2_cmp_z(while_for_1_for_17_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_16_while_for_1_for_acc_2_cmp_a(while_for_1_for_16_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_16_while_for_1_for_acc_2_cmp_b(while_for_1_for_16_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_16_while_for_1_for_acc_2_cmp_z(while_for_1_for_16_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_15_while_for_1_for_acc_2_cmp_a(while_for_1_for_15_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_15_while_for_1_for_acc_2_cmp_b(while_for_1_for_15_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_15_while_for_1_for_acc_2_cmp_z(while_for_1_for_15_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_14_while_for_1_for_acc_2_cmp_a(while_for_1_for_14_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_14_while_for_1_for_acc_2_cmp_b(while_for_1_for_14_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_14_while_for_1_for_acc_2_cmp_z(while_for_1_for_14_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_13_while_for_1_for_acc_2_cmp_a(while_for_1_for_13_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_13_while_for_1_for_acc_2_cmp_b(while_for_1_for_13_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_13_while_for_1_for_acc_2_cmp_z(while_for_1_for_13_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_12_while_for_1_for_acc_2_cmp_a(while_for_1_for_12_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_12_while_for_1_for_acc_2_cmp_b(while_for_1_for_12_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_12_while_for_1_for_acc_2_cmp_z(while_for_1_for_12_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_11_while_for_1_for_acc_2_cmp_a(while_for_1_for_11_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_11_while_for_1_for_acc_2_cmp_b(while_for_1_for_11_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_11_while_for_1_for_acc_2_cmp_z(while_for_1_for_11_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_10_while_for_1_for_acc_2_cmp_a(while_for_1_for_10_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_10_while_for_1_for_acc_2_cmp_b(while_for_1_for_10_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_10_while_for_1_for_acc_2_cmp_z(while_for_1_for_10_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_9_while_for_1_for_acc_2_cmp_a(while_for_1_for_9_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_9_while_for_1_for_acc_2_cmp_b(while_for_1_for_9_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_9_while_for_1_for_acc_2_cmp_z(while_for_1_for_9_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_8_while_for_1_for_acc_2_cmp_a(while_for_1_for_8_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_8_while_for_1_for_acc_2_cmp_b(while_for_1_for_8_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_8_while_for_1_for_acc_2_cmp_z(while_for_1_for_8_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_7_while_for_1_for_acc_2_cmp_a(while_for_1_for_7_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_7_while_for_1_for_acc_2_cmp_b(while_for_1_for_7_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_7_while_for_1_for_acc_2_cmp_z(while_for_1_for_7_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_6_while_for_1_for_acc_2_cmp_a(while_for_1_for_6_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_6_while_for_1_for_acc_2_cmp_b(while_for_1_for_6_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_6_while_for_1_for_acc_2_cmp_z(while_for_1_for_6_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_5_while_for_1_for_acc_2_cmp_a(while_for_1_for_5_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_5_while_for_1_for_acc_2_cmp_b(while_for_1_for_5_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_5_while_for_1_for_acc_2_cmp_z(while_for_1_for_5_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_4_while_for_1_for_acc_2_cmp_a(while_for_1_for_4_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_4_while_for_1_for_acc_2_cmp_b(while_for_1_for_4_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_4_while_for_1_for_acc_2_cmp_z(while_for_1_for_4_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_3_while_for_1_for_acc_2_cmp_a(while_for_1_for_3_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_3_while_for_1_for_acc_2_cmp_b(while_for_1_for_3_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_3_while_for_1_for_acc_2_cmp_z(while_for_1_for_3_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_2_while_for_1_for_acc_2_cmp_a(while_for_1_for_2_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_2_while_for_1_for_acc_2_cmp_b(while_for_1_for_2_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_2_while_for_1_for_acc_2_cmp_z(while_for_1_for_2_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_1_while_for_1_for_acc_2_cmp_a(while_for_1_for_1_while_for_1_for_acc_2_cmp_a),
      .while_for_1_for_1_while_for_1_for_acc_2_cmp_b(while_for_1_for_1_while_for_1_for_acc_2_cmp_b),
      .while_for_1_for_1_while_for_1_for_acc_2_cmp_load(while_for_1_for_1_while_for_1_for_acc_2_cmp_load),
      .while_for_1_for_1_while_for_1_for_acc_2_cmp_z(while_for_1_for_1_while_for_1_for_acc_2_cmp_z),
      .while_for_1_for_32_while_for_1_for_acc_2_cmp_load_pff(while_for_1_for_32_while_for_1_for_acc_2_cmp_load_iff),
      .while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_pff(while_for_1_for_32_while_for_1_for_acc_2_cmp_datavalid_iff),
      .feature_memory_rsci_d_d_pff(feature_memory_rsci_d_d_iff),
      .feature_memory_rsci_re_d_pff(feature_memory_rsci_re_d_iff),
      .feature_memory_rsci_we_d_pff(feature_memory_rsci_we_d_iff),
      .weight_memory_rsci_we_d_pff(weight_memory_rsci_we_d_iff)
    );
  assign memory_channels_aw_channel_rsc_dat_qos = memory_channels_aw_channel_rsc_dat[3:0];
  assign memory_channels_aw_channel_rsc_dat_region = memory_channels_aw_channel_rsc_dat[7:4];
  assign memory_channels_aw_channel_rsc_dat_prot = memory_channels_aw_channel_rsc_dat[10:8];
  assign memory_channels_aw_channel_rsc_dat_cache = memory_channels_aw_channel_rsc_dat[14:11];
  assign memory_channels_aw_channel_rsc_dat_lock = memory_channels_aw_channel_rsc_dat[15];
  assign memory_channels_aw_channel_rsc_dat_burst = memory_channels_aw_channel_rsc_dat[17:16];
  assign memory_channels_aw_channel_rsc_dat_size = memory_channels_aw_channel_rsc_dat[20:18];
  assign memory_channels_aw_channel_rsc_dat_len = memory_channels_aw_channel_rsc_dat[28:21];
  assign memory_channels_aw_channel_rsc_dat_address = memory_channels_aw_channel_rsc_dat[92:29];
  assign memory_channels_aw_channel_rsc_dat_id = memory_channels_aw_channel_rsc_dat[108:93];
  assign memory_channels_w_channel_rsc_dat_last = memory_channels_w_channel_rsc_dat[0];
  assign memory_channels_w_channel_rsc_dat_strb = memory_channels_w_channel_rsc_dat[64:1];
  assign memory_channels_w_channel_rsc_dat_data = memory_channels_w_channel_rsc_dat[576:65];
  assign memory_channels_ar_channel_rsc_dat_qos = memory_channels_ar_channel_rsc_dat[3:0];
  assign memory_channels_ar_channel_rsc_dat_region = memory_channels_ar_channel_rsc_dat[7:4];
  assign memory_channels_ar_channel_rsc_dat_prot = memory_channels_ar_channel_rsc_dat[10:8];
  assign memory_channels_ar_channel_rsc_dat_cache = memory_channels_ar_channel_rsc_dat[14:11];
  assign memory_channels_ar_channel_rsc_dat_lock = memory_channels_ar_channel_rsc_dat[15];
  assign memory_channels_ar_channel_rsc_dat_burst = memory_channels_ar_channel_rsc_dat[17:16];
  assign memory_channels_ar_channel_rsc_dat_size = memory_channels_ar_channel_rsc_dat[20:18];
  assign memory_channels_ar_channel_rsc_dat_len = memory_channels_ar_channel_rsc_dat[28:21];
  assign memory_channels_ar_channel_rsc_dat_address = memory_channels_ar_channel_rsc_dat[92:29];
  assign memory_channels_ar_channel_rsc_dat_id = memory_channels_ar_channel_rsc_dat[108:93];
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense
// ------------------------------------------------------------------


module dense (
  clk, arst_n, start_rsc_dat, start_rsc_vld, start_rsc_rdy, done_rsc_dat, done_rsc_vld,
      done_rsc_rdy, use_relu_rsc_dat, use_relu_triosy_lz, addr_hi_rsc_dat, addr_hi_triosy_lz,
      feature_addr_lo_rsc_dat, feature_addr_lo_triosy_lz, weight_addr_lo_rsc_dat,
      weight_addr_lo_triosy_lz, output_addr_lo_rsc_dat, output_addr_lo_triosy_lz,
      input_vector_len_rsc_dat, input_vector_len_triosy_lz, output_vector_len_rsc_dat,
      output_vector_len_triosy_lz, debug_rsc_dat, debug_triosy_lz, memory_channels_aw_channel_rsc_dat,
      memory_channels_aw_channel_rsc_vld, memory_channels_aw_channel_rsc_rdy, memory_channels_w_channel_rsc_dat,
      memory_channels_w_channel_rsc_vld, memory_channels_w_channel_rsc_rdy, memory_channels_b_channel_rsc_dat,
      memory_channels_b_channel_rsc_vld, memory_channels_b_channel_rsc_rdy, memory_channels_ar_channel_rsc_dat,
      memory_channels_ar_channel_rsc_vld, memory_channels_ar_channel_rsc_rdy, memory_channels_r_channel_rsc_dat,
      memory_channels_r_channel_rsc_vld, memory_channels_r_channel_rsc_rdy
);
  input clk;
  input arst_n;
  input start_rsc_dat;
  input start_rsc_vld;
  output start_rsc_rdy;
  output done_rsc_dat;
  output done_rsc_vld;
  input done_rsc_rdy;
  input use_relu_rsc_dat;
  output use_relu_triosy_lz;
  input [31:0] addr_hi_rsc_dat;
  output addr_hi_triosy_lz;
  input [31:0] feature_addr_lo_rsc_dat;
  output feature_addr_lo_triosy_lz;
  input [31:0] weight_addr_lo_rsc_dat;
  output weight_addr_lo_triosy_lz;
  input [31:0] output_addr_lo_rsc_dat;
  output output_addr_lo_triosy_lz;
  input [31:0] input_vector_len_rsc_dat;
  output input_vector_len_triosy_lz;
  input [31:0] output_vector_len_rsc_dat;
  output output_vector_len_triosy_lz;
  output [31:0] debug_rsc_dat;
  output debug_triosy_lz;
  output [108:0] memory_channels_aw_channel_rsc_dat;
  output memory_channels_aw_channel_rsc_vld;
  input memory_channels_aw_channel_rsc_rdy;
  output [576:0] memory_channels_w_channel_rsc_dat;
  output memory_channels_w_channel_rsc_vld;
  input memory_channels_w_channel_rsc_rdy;
  input [17:0] memory_channels_b_channel_rsc_dat;
  input memory_channels_b_channel_rsc_vld;
  output memory_channels_b_channel_rsc_rdy;
  output [108:0] memory_channels_ar_channel_rsc_dat;
  output memory_channels_ar_channel_rsc_vld;
  input memory_channels_ar_channel_rsc_rdy;
  input [530:0] memory_channels_r_channel_rsc_dat;
  input memory_channels_r_channel_rsc_vld;
  output memory_channels_r_channel_rsc_rdy;


  // Interconnect Declarations
  wire [15:0] memory_channels_aw_channel_rsc_dat_id;
  wire [63:0] memory_channels_aw_channel_rsc_dat_address;
  wire [7:0] memory_channels_aw_channel_rsc_dat_len;
  wire [2:0] memory_channels_aw_channel_rsc_dat_size;
  wire [1:0] memory_channels_aw_channel_rsc_dat_burst;
  wire memory_channels_aw_channel_rsc_dat_lock;
  wire [3:0] memory_channels_aw_channel_rsc_dat_cache;
  wire [2:0] memory_channels_aw_channel_rsc_dat_prot;
  wire [3:0] memory_channels_aw_channel_rsc_dat_region;
  wire [3:0] memory_channels_aw_channel_rsc_dat_qos;
  wire [511:0] memory_channels_w_channel_rsc_dat_data;
  wire [63:0] memory_channels_w_channel_rsc_dat_strb;
  wire memory_channels_w_channel_rsc_dat_last;
  wire [15:0] memory_channels_ar_channel_rsc_dat_id;
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
  wire [15:0] nl_dense_struct_inst_memory_channels_b_channel_rsc_dat_id;
  assign nl_dense_struct_inst_memory_channels_b_channel_rsc_dat_id = memory_channels_b_channel_rsc_dat[17:2];
  wire [1:0] nl_dense_struct_inst_memory_channels_b_channel_rsc_dat_resp;
  assign nl_dense_struct_inst_memory_channels_b_channel_rsc_dat_resp = memory_channels_b_channel_rsc_dat[1:0];
  wire [15:0] nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_id;
  assign nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_id = memory_channels_r_channel_rsc_dat[530:515];
  wire [511:0] nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_data;
  assign nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_data = memory_channels_r_channel_rsc_dat[514:3];
  wire [1:0] nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_resp;
  assign nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_resp = memory_channels_r_channel_rsc_dat[2:1];
  wire  nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_last;
  assign nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_last = memory_channels_r_channel_rsc_dat[0];
  dense_struct dense_struct_inst (
      .clk(clk),
      .arst_n(arst_n),
      .start_rsc_dat(start_rsc_dat),
      .start_rsc_vld(start_rsc_vld),
      .start_rsc_rdy(start_rsc_rdy),
      .done_rsc_dat(done_rsc_dat),
      .done_rsc_vld(done_rsc_vld),
      .done_rsc_rdy(done_rsc_rdy),
      .use_relu_triosy_lz(use_relu_triosy_lz),
      .addr_hi_rsc_dat(addr_hi_rsc_dat),
      .addr_hi_triosy_lz(addr_hi_triosy_lz),
      .feature_addr_lo_rsc_dat(feature_addr_lo_rsc_dat),
      .feature_addr_lo_triosy_lz(feature_addr_lo_triosy_lz),
      .weight_addr_lo_rsc_dat(weight_addr_lo_rsc_dat),
      .weight_addr_lo_triosy_lz(weight_addr_lo_triosy_lz),
      .output_addr_lo_rsc_dat(output_addr_lo_rsc_dat),
      .output_addr_lo_triosy_lz(output_addr_lo_triosy_lz),
      .input_vector_len_rsc_dat(input_vector_len_rsc_dat),
      .input_vector_len_triosy_lz(input_vector_len_triosy_lz),
      .output_vector_len_rsc_dat(output_vector_len_rsc_dat),
      .output_vector_len_triosy_lz(output_vector_len_triosy_lz),
      .debug_rsc_dat(debug_rsc_dat),
      .debug_triosy_lz(debug_triosy_lz),
      .memory_channels_aw_channel_rsc_dat_id(memory_channels_aw_channel_rsc_dat_id),
      .memory_channels_aw_channel_rsc_dat_address(memory_channels_aw_channel_rsc_dat_address),
      .memory_channels_aw_channel_rsc_dat_len(memory_channels_aw_channel_rsc_dat_len),
      .memory_channels_aw_channel_rsc_dat_size(memory_channels_aw_channel_rsc_dat_size),
      .memory_channels_aw_channel_rsc_dat_burst(memory_channels_aw_channel_rsc_dat_burst),
      .memory_channels_aw_channel_rsc_dat_lock(memory_channels_aw_channel_rsc_dat_lock),
      .memory_channels_aw_channel_rsc_dat_cache(memory_channels_aw_channel_rsc_dat_cache),
      .memory_channels_aw_channel_rsc_dat_prot(memory_channels_aw_channel_rsc_dat_prot),
      .memory_channels_aw_channel_rsc_dat_region(memory_channels_aw_channel_rsc_dat_region),
      .memory_channels_aw_channel_rsc_dat_qos(memory_channels_aw_channel_rsc_dat_qos),
      .memory_channels_aw_channel_rsc_vld(memory_channels_aw_channel_rsc_vld),
      .memory_channels_aw_channel_rsc_rdy(memory_channels_aw_channel_rsc_rdy),
      .memory_channels_w_channel_rsc_dat_data(memory_channels_w_channel_rsc_dat_data),
      .memory_channels_w_channel_rsc_dat_strb(memory_channels_w_channel_rsc_dat_strb),
      .memory_channels_w_channel_rsc_dat_last(memory_channels_w_channel_rsc_dat_last),
      .memory_channels_w_channel_rsc_vld(memory_channels_w_channel_rsc_vld),
      .memory_channels_w_channel_rsc_rdy(memory_channels_w_channel_rsc_rdy),
      .memory_channels_b_channel_rsc_dat_id(nl_dense_struct_inst_memory_channels_b_channel_rsc_dat_id[15:0]),
      .memory_channels_b_channel_rsc_dat_resp(nl_dense_struct_inst_memory_channels_b_channel_rsc_dat_resp[1:0]),
      .memory_channels_b_channel_rsc_vld(memory_channels_b_channel_rsc_vld),
      .memory_channels_b_channel_rsc_rdy(memory_channels_b_channel_rsc_rdy),
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
      .memory_channels_r_channel_rsc_dat_id(nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_id[15:0]),
      .memory_channels_r_channel_rsc_dat_data(nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_data[511:0]),
      .memory_channels_r_channel_rsc_dat_resp(nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_resp[1:0]),
      .memory_channels_r_channel_rsc_dat_last(nl_dense_struct_inst_memory_channels_r_channel_rsc_dat_last),
      .memory_channels_r_channel_rsc_vld(memory_channels_r_channel_rsc_vld),
      .memory_channels_r_channel_rsc_rdy(memory_channels_r_channel_rsc_rdy)
    );
  assign memory_channels_aw_channel_rsc_dat = {memory_channels_aw_channel_rsc_dat_id
      , memory_channels_aw_channel_rsc_dat_address , memory_channels_aw_channel_rsc_dat_len
      , memory_channels_aw_channel_rsc_dat_size , memory_channels_aw_channel_rsc_dat_burst
      , memory_channels_aw_channel_rsc_dat_lock , memory_channels_aw_channel_rsc_dat_cache
      , memory_channels_aw_channel_rsc_dat_prot , memory_channels_aw_channel_rsc_dat_region
      , memory_channels_aw_channel_rsc_dat_qos};
  assign memory_channels_w_channel_rsc_dat = {memory_channels_w_channel_rsc_dat_data
      , memory_channels_w_channel_rsc_dat_strb , memory_channels_w_channel_rsc_dat_last};
  assign memory_channels_ar_channel_rsc_dat = {memory_channels_ar_channel_rsc_dat_id
      , memory_channels_ar_channel_rsc_dat_address , memory_channels_ar_channel_rsc_dat_len
      , memory_channels_ar_channel_rsc_dat_size , memory_channels_ar_channel_rsc_dat_burst
      , memory_channels_ar_channel_rsc_dat_lock , memory_channels_ar_channel_rsc_dat_cache
      , memory_channels_ar_channel_rsc_dat_prot , memory_channels_ar_channel_rsc_dat_region
      , memory_channels_ar_channel_rsc_dat_qos};
endmodule



