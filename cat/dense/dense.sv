
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
//  Generated by:   russk@orw-vistapult
//  Generated date: Tue Sep  2 11:51:18 2025
// ----------------------------------------------------------------------

// 
// ------------------------------------------------------------------
//  Design Unit:    dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_18_5_32_32_1_32_32_1_gen
// ------------------------------------------------------------------


module dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_18_5_32_32_1_32_32_1_gen (
  clken, q, re, radr, we, d, wadr, clken_d, d_d, q_d, radr_d, re_d, wadr_d, we_d,
      writeA_w_ram_ir_internal_WMASK_B_d, readA_r_ram_ir_internal_RMASK_B_d
);
  output clken;
  input [31:0] q;
  output re;
  output [4:0] radr;
  output we;
  output [31:0] d;
  output [4:0] wadr;
  input clken_d;
  input [31:0] d_d;
  output [31:0] q_d;
  input [4:0] radr_d;
  input re_d;
  input [4:0] wadr_d;
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
//  Design Unit:    dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_17_8_512_256_1_256_512_1_gen
// ------------------------------------------------------------------


module dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_17_8_512_256_1_256_512_1_gen (
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
//  Design Unit:    dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_16_15_512_32768_1_32768_512_1_gen
// ------------------------------------------------------------------


module dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_16_15_512_32768_1_32768_512_1_gen
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
  clk, arst_n, core_wen, fsm_output, memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0,
      do_C_2_tr0, memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0, sum_array_vinit_C_1_tr0,
      main_C_3_tr0, while_while_C_35_tr0, memory_axi_burst_read_base_axi_u512_512_2_for_C_0_tr0,
      while_while_C_36_tr0
);
  input clk;
  input arst_n;
  input core_wen;
  output [6:0] fsm_output;
  reg [6:0] fsm_output;
  input memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0;
  input do_C_2_tr0;
  input memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0;
  input sum_array_vinit_C_1_tr0;
  input main_C_3_tr0;
  input while_while_C_35_tr0;
  input memory_axi_burst_read_base_axi_u512_512_2_for_C_0_tr0;
  input while_while_C_36_tr0;


  // FSM State Type Declaration for dense_core_core_fsm_1
  parameter
    core_rlp_C_0 = 7'd0,
    main_C_0 = 7'd1,
    do_C_0 = 7'd2,
    do_C_1 = 7'd3,
    memory_axi_burst_read_base_axi_u512_512_for_C_0 = 7'd4,
    do_C_2 = 7'd5,
    main_C_1 = 7'd6,
    memory_axi_burst_read_base_axi_u512_512_1_for_C_0 = 7'd7,
    main_C_2 = 7'd8,
    sum_array_vinit_C_0 = 7'd9,
    sum_array_vinit_C_1 = 7'd10,
    main_C_3 = 7'd11,
    while_while_C_0 = 7'd12,
    while_while_C_1 = 7'd13,
    while_while_C_2 = 7'd14,
    while_while_C_3 = 7'd15,
    while_while_C_4 = 7'd16,
    while_while_C_5 = 7'd17,
    while_while_C_6 = 7'd18,
    while_while_C_7 = 7'd19,
    while_while_C_8 = 7'd20,
    while_while_C_9 = 7'd21,
    while_while_C_10 = 7'd22,
    while_while_C_11 = 7'd23,
    while_while_C_12 = 7'd24,
    while_while_C_13 = 7'd25,
    while_while_C_14 = 7'd26,
    while_while_C_15 = 7'd27,
    while_while_C_16 = 7'd28,
    while_while_C_17 = 7'd29,
    while_while_C_18 = 7'd30,
    while_while_C_19 = 7'd31,
    while_while_C_20 = 7'd32,
    while_while_C_21 = 7'd33,
    while_while_C_22 = 7'd34,
    while_while_C_23 = 7'd35,
    while_while_C_24 = 7'd36,
    while_while_C_25 = 7'd37,
    while_while_C_26 = 7'd38,
    while_while_C_27 = 7'd39,
    while_while_C_28 = 7'd40,
    while_while_C_29 = 7'd41,
    while_while_C_30 = 7'd42,
    while_while_C_31 = 7'd43,
    while_while_C_32 = 7'd44,
    while_while_C_33 = 7'd45,
    while_while_C_34 = 7'd46,
    while_while_C_35 = 7'd47,
    memory_axi_burst_read_base_axi_u512_512_2_for_C_0 = 7'd48,
    while_while_C_36 = 7'd49,
    main_C_4 = 7'd50,
    main_C_5 = 7'd51,
    main_C_6 = 7'd52,
    main_C_7 = 7'd53,
    main_C_8 = 7'd54,
    main_C_9 = 7'd55,
    main_C_10 = 7'd56,
    main_C_11 = 7'd57,
    main_C_12 = 7'd58,
    main_C_13 = 7'd59,
    main_C_14 = 7'd60,
    main_C_15 = 7'd61,
    main_C_16 = 7'd62,
    main_C_17 = 7'd63,
    main_C_18 = 7'd64,
    main_C_19 = 7'd65,
    main_C_20 = 7'd66,
    main_C_21 = 7'd67,
    main_C_22 = 7'd68,
    main_C_23 = 7'd69,
    main_C_24 = 7'd70,
    main_C_25 = 7'd71,
    main_C_26 = 7'd72,
    main_C_27 = 7'd73,
    main_C_28 = 7'd74,
    main_C_29 = 7'd75,
    main_C_30 = 7'd76,
    main_C_31 = 7'd77,
    main_C_32 = 7'd78,
    main_C_33 = 7'd79,
    main_C_34 = 7'd80,
    main_C_35 = 7'd81,
    main_C_36 = 7'd82,
    main_C_37 = 7'd83,
    main_C_38 = 7'd84,
    main_C_39 = 7'd85;

  reg [6:0] state_var;
  reg [6:0] state_var_NS;


  // Interconnect Declarations for Component Instantiations 
  always @(*)
  begin : dense_core_core_fsm_1
    case (state_var)
      main_C_0 : begin
        fsm_output = 7'b0000001;
        state_var_NS = do_C_0;
      end
      do_C_0 : begin
        fsm_output = 7'b0000010;
        state_var_NS = do_C_1;
      end
      do_C_1 : begin
        fsm_output = 7'b0000011;
        state_var_NS = memory_axi_burst_read_base_axi_u512_512_for_C_0;
      end
      memory_axi_burst_read_base_axi_u512_512_for_C_0 : begin
        fsm_output = 7'b0000100;
        if ( memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0 ) begin
          state_var_NS = do_C_2;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_for_C_0;
        end
      end
      do_C_2 : begin
        fsm_output = 7'b0000101;
        if ( do_C_2_tr0 ) begin
          state_var_NS = main_C_1;
        end
        else begin
          state_var_NS = do_C_0;
        end
      end
      main_C_1 : begin
        fsm_output = 7'b0000110;
        state_var_NS = memory_axi_burst_read_base_axi_u512_512_1_for_C_0;
      end
      memory_axi_burst_read_base_axi_u512_512_1_for_C_0 : begin
        fsm_output = 7'b0000111;
        if ( memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0 ) begin
          state_var_NS = main_C_2;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_1_for_C_0;
        end
      end
      main_C_2 : begin
        fsm_output = 7'b0001000;
        state_var_NS = sum_array_vinit_C_0;
      end
      sum_array_vinit_C_0 : begin
        fsm_output = 7'b0001001;
        state_var_NS = sum_array_vinit_C_1;
      end
      sum_array_vinit_C_1 : begin
        fsm_output = 7'b0001010;
        if ( sum_array_vinit_C_1_tr0 ) begin
          state_var_NS = main_C_3;
        end
        else begin
          state_var_NS = sum_array_vinit_C_0;
        end
      end
      main_C_3 : begin
        fsm_output = 7'b0001011;
        if ( main_C_3_tr0 ) begin
          state_var_NS = main_C_4;
        end
        else begin
          state_var_NS = while_while_C_0;
        end
      end
      while_while_C_0 : begin
        fsm_output = 7'b0001100;
        state_var_NS = while_while_C_1;
      end
      while_while_C_1 : begin
        fsm_output = 7'b0001101;
        state_var_NS = while_while_C_2;
      end
      while_while_C_2 : begin
        fsm_output = 7'b0001110;
        state_var_NS = while_while_C_3;
      end
      while_while_C_3 : begin
        fsm_output = 7'b0001111;
        state_var_NS = while_while_C_4;
      end
      while_while_C_4 : begin
        fsm_output = 7'b0010000;
        state_var_NS = while_while_C_5;
      end
      while_while_C_5 : begin
        fsm_output = 7'b0010001;
        state_var_NS = while_while_C_6;
      end
      while_while_C_6 : begin
        fsm_output = 7'b0010010;
        state_var_NS = while_while_C_7;
      end
      while_while_C_7 : begin
        fsm_output = 7'b0010011;
        state_var_NS = while_while_C_8;
      end
      while_while_C_8 : begin
        fsm_output = 7'b0010100;
        state_var_NS = while_while_C_9;
      end
      while_while_C_9 : begin
        fsm_output = 7'b0010101;
        state_var_NS = while_while_C_10;
      end
      while_while_C_10 : begin
        fsm_output = 7'b0010110;
        state_var_NS = while_while_C_11;
      end
      while_while_C_11 : begin
        fsm_output = 7'b0010111;
        state_var_NS = while_while_C_12;
      end
      while_while_C_12 : begin
        fsm_output = 7'b0011000;
        state_var_NS = while_while_C_13;
      end
      while_while_C_13 : begin
        fsm_output = 7'b0011001;
        state_var_NS = while_while_C_14;
      end
      while_while_C_14 : begin
        fsm_output = 7'b0011010;
        state_var_NS = while_while_C_15;
      end
      while_while_C_15 : begin
        fsm_output = 7'b0011011;
        state_var_NS = while_while_C_16;
      end
      while_while_C_16 : begin
        fsm_output = 7'b0011100;
        state_var_NS = while_while_C_17;
      end
      while_while_C_17 : begin
        fsm_output = 7'b0011101;
        state_var_NS = while_while_C_18;
      end
      while_while_C_18 : begin
        fsm_output = 7'b0011110;
        state_var_NS = while_while_C_19;
      end
      while_while_C_19 : begin
        fsm_output = 7'b0011111;
        state_var_NS = while_while_C_20;
      end
      while_while_C_20 : begin
        fsm_output = 7'b0100000;
        state_var_NS = while_while_C_21;
      end
      while_while_C_21 : begin
        fsm_output = 7'b0100001;
        state_var_NS = while_while_C_22;
      end
      while_while_C_22 : begin
        fsm_output = 7'b0100010;
        state_var_NS = while_while_C_23;
      end
      while_while_C_23 : begin
        fsm_output = 7'b0100011;
        state_var_NS = while_while_C_24;
      end
      while_while_C_24 : begin
        fsm_output = 7'b0100100;
        state_var_NS = while_while_C_25;
      end
      while_while_C_25 : begin
        fsm_output = 7'b0100101;
        state_var_NS = while_while_C_26;
      end
      while_while_C_26 : begin
        fsm_output = 7'b0100110;
        state_var_NS = while_while_C_27;
      end
      while_while_C_27 : begin
        fsm_output = 7'b0100111;
        state_var_NS = while_while_C_28;
      end
      while_while_C_28 : begin
        fsm_output = 7'b0101000;
        state_var_NS = while_while_C_29;
      end
      while_while_C_29 : begin
        fsm_output = 7'b0101001;
        state_var_NS = while_while_C_30;
      end
      while_while_C_30 : begin
        fsm_output = 7'b0101010;
        state_var_NS = while_while_C_31;
      end
      while_while_C_31 : begin
        fsm_output = 7'b0101011;
        state_var_NS = while_while_C_32;
      end
      while_while_C_32 : begin
        fsm_output = 7'b0101100;
        state_var_NS = while_while_C_33;
      end
      while_while_C_33 : begin
        fsm_output = 7'b0101101;
        state_var_NS = while_while_C_34;
      end
      while_while_C_34 : begin
        fsm_output = 7'b0101110;
        state_var_NS = while_while_C_35;
      end
      while_while_C_35 : begin
        fsm_output = 7'b0101111;
        if ( while_while_C_35_tr0 ) begin
          state_var_NS = while_while_C_36;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_2_for_C_0;
        end
      end
      memory_axi_burst_read_base_axi_u512_512_2_for_C_0 : begin
        fsm_output = 7'b0110000;
        if ( memory_axi_burst_read_base_axi_u512_512_2_for_C_0_tr0 ) begin
          state_var_NS = while_while_C_36;
        end
        else begin
          state_var_NS = memory_axi_burst_read_base_axi_u512_512_2_for_C_0;
        end
      end
      while_while_C_36 : begin
        fsm_output = 7'b0110001;
        if ( while_while_C_36_tr0 ) begin
          state_var_NS = main_C_4;
        end
        else begin
          state_var_NS = while_while_C_0;
        end
      end
      main_C_4 : begin
        fsm_output = 7'b0110010;
        state_var_NS = main_C_5;
      end
      main_C_5 : begin
        fsm_output = 7'b0110011;
        state_var_NS = main_C_6;
      end
      main_C_6 : begin
        fsm_output = 7'b0110100;
        state_var_NS = main_C_7;
      end
      main_C_7 : begin
        fsm_output = 7'b0110101;
        state_var_NS = main_C_8;
      end
      main_C_8 : begin
        fsm_output = 7'b0110110;
        state_var_NS = main_C_9;
      end
      main_C_9 : begin
        fsm_output = 7'b0110111;
        state_var_NS = main_C_10;
      end
      main_C_10 : begin
        fsm_output = 7'b0111000;
        state_var_NS = main_C_11;
      end
      main_C_11 : begin
        fsm_output = 7'b0111001;
        state_var_NS = main_C_12;
      end
      main_C_12 : begin
        fsm_output = 7'b0111010;
        state_var_NS = main_C_13;
      end
      main_C_13 : begin
        fsm_output = 7'b0111011;
        state_var_NS = main_C_14;
      end
      main_C_14 : begin
        fsm_output = 7'b0111100;
        state_var_NS = main_C_15;
      end
      main_C_15 : begin
        fsm_output = 7'b0111101;
        state_var_NS = main_C_16;
      end
      main_C_16 : begin
        fsm_output = 7'b0111110;
        state_var_NS = main_C_17;
      end
      main_C_17 : begin
        fsm_output = 7'b0111111;
        state_var_NS = main_C_18;
      end
      main_C_18 : begin
        fsm_output = 7'b1000000;
        state_var_NS = main_C_19;
      end
      main_C_19 : begin
        fsm_output = 7'b1000001;
        state_var_NS = main_C_20;
      end
      main_C_20 : begin
        fsm_output = 7'b1000010;
        state_var_NS = main_C_21;
      end
      main_C_21 : begin
        fsm_output = 7'b1000011;
        state_var_NS = main_C_22;
      end
      main_C_22 : begin
        fsm_output = 7'b1000100;
        state_var_NS = main_C_23;
      end
      main_C_23 : begin
        fsm_output = 7'b1000101;
        state_var_NS = main_C_24;
      end
      main_C_24 : begin
        fsm_output = 7'b1000110;
        state_var_NS = main_C_25;
      end
      main_C_25 : begin
        fsm_output = 7'b1000111;
        state_var_NS = main_C_26;
      end
      main_C_26 : begin
        fsm_output = 7'b1001000;
        state_var_NS = main_C_27;
      end
      main_C_27 : begin
        fsm_output = 7'b1001001;
        state_var_NS = main_C_28;
      end
      main_C_28 : begin
        fsm_output = 7'b1001010;
        state_var_NS = main_C_29;
      end
      main_C_29 : begin
        fsm_output = 7'b1001011;
        state_var_NS = main_C_30;
      end
      main_C_30 : begin
        fsm_output = 7'b1001100;
        state_var_NS = main_C_31;
      end
      main_C_31 : begin
        fsm_output = 7'b1001101;
        state_var_NS = main_C_32;
      end
      main_C_32 : begin
        fsm_output = 7'b1001110;
        state_var_NS = main_C_33;
      end
      main_C_33 : begin
        fsm_output = 7'b1001111;
        state_var_NS = main_C_34;
      end
      main_C_34 : begin
        fsm_output = 7'b1010000;
        state_var_NS = main_C_35;
      end
      main_C_35 : begin
        fsm_output = 7'b1010001;
        state_var_NS = main_C_36;
      end
      main_C_36 : begin
        fsm_output = 7'b1010010;
        state_var_NS = main_C_37;
      end
      main_C_37 : begin
        fsm_output = 7'b1010011;
        state_var_NS = main_C_38;
      end
      main_C_38 : begin
        fsm_output = 7'b1010100;
        state_var_NS = main_C_39;
      end
      main_C_39 : begin
        fsm_output = 7'b1010101;
        state_var_NS = main_C_0;
      end
      // core_rlp_C_0
      default : begin
        fsm_output = 7'b0000000;
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
//  Design Unit:    dense_core_out_vector_length_triosy_obj_out_vector_length_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_out_vector_length_triosy_obj_out_vector_length_triosy_wait_ctrl
    (
  core_wten, out_vector_length_triosy_obj_iswt0, out_vector_length_triosy_obj_biwt
);
  input core_wten;
  input out_vector_length_triosy_obj_iswt0;
  output out_vector_length_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign out_vector_length_triosy_obj_biwt = (~ core_wten) & out_vector_length_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_in_vector_length_triosy_obj_in_vector_length_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_in_vector_length_triosy_obj_in_vector_length_triosy_wait_ctrl (
  core_wten, in_vector_length_triosy_obj_iswt0, in_vector_length_triosy_obj_biwt
);
  input core_wten;
  input in_vector_length_triosy_obj_iswt0;
  output in_vector_length_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign in_vector_length_triosy_obj_biwt = (~ core_wten) & in_vector_length_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_output_addr_triosy_obj_output_addr_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_output_addr_triosy_obj_output_addr_triosy_wait_ctrl (
  core_wten, output_addr_triosy_obj_iswt0, output_addr_triosy_obj_biwt
);
  input core_wten;
  input output_addr_triosy_obj_iswt0;
  output output_addr_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign output_addr_triosy_obj_biwt = (~ core_wten) & output_addr_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_weight_addr_triosy_obj_weight_addr_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_weight_addr_triosy_obj_weight_addr_triosy_wait_ctrl (
  core_wten, weight_addr_triosy_obj_iswt0, weight_addr_triosy_obj_biwt
);
  input core_wten;
  input weight_addr_triosy_obj_iswt0;
  output weight_addr_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign weight_addr_triosy_obj_biwt = (~ core_wten) & weight_addr_triosy_obj_iswt0;
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_feature_addr_triosy_obj_feature_addr_triosy_wait_ctrl
// ------------------------------------------------------------------


module dense_core_feature_addr_triosy_obj_feature_addr_triosy_wait_ctrl (
  core_wten, feature_addr_triosy_obj_iswt0, feature_addr_triosy_obj_biwt
);
  input core_wten;
  input feature_addr_triosy_obj_iswt0;
  output feature_addr_triosy_obj_biwt;



  // Interconnect Declarations for Component Instantiations 
  assign feature_addr_triosy_obj_biwt = (~ core_wten) & feature_addr_triosy_obj_iswt0;
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
//  Design Unit:    dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_dp
// ------------------------------------------------------------------


module dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_dp
    (
  clk, arst_n, memory_channels_b_channel_rsci_oswt, memory_channels_b_channel_rsci_wen_comp,
      memory_channels_b_channel_rsci_biwt, memory_channels_b_channel_rsci_bdwt, memory_channels_b_channel_rsci_bcwt,
      memory_channels_b_channel_rsci_wen_comp_pff, memory_channels_b_channel_rsci_oswt_pff,
      memory_channels_b_channel_rsci_biwt_pff, memory_channels_b_channel_rsci_bcwt_pff
);
  input clk;
  input arst_n;
  input memory_channels_b_channel_rsci_oswt;
  output memory_channels_b_channel_rsci_wen_comp;
  input memory_channels_b_channel_rsci_biwt;
  input memory_channels_b_channel_rsci_bdwt;
  output memory_channels_b_channel_rsci_bcwt;
  output memory_channels_b_channel_rsci_wen_comp_pff;
  input memory_channels_b_channel_rsci_oswt_pff;
  input memory_channels_b_channel_rsci_biwt_pff;
  output memory_channels_b_channel_rsci_bcwt_pff;


  // Interconnect Declarations
  reg memory_channels_b_channel_rsci_bcwt_reg;
  wire memory_get_b_1_nor_rmff;


  // Interconnect Declarations for Component Instantiations 
  assign memory_get_b_1_nor_rmff = ~((~(memory_channels_b_channel_rsci_bcwt | memory_channels_b_channel_rsci_biwt))
      | memory_channels_b_channel_rsci_bdwt);
  assign memory_channels_b_channel_rsci_wen_comp = (~ memory_channels_b_channel_rsci_oswt)
      | memory_channels_b_channel_rsci_biwt | memory_channels_b_channel_rsci_bcwt;
  assign memory_channels_b_channel_rsci_wen_comp_pff = (~ memory_channels_b_channel_rsci_oswt_pff)
      | memory_channels_b_channel_rsci_biwt_pff | memory_channels_b_channel_rsci_bcwt_pff;
  assign memory_channels_b_channel_rsci_bcwt = memory_channels_b_channel_rsci_bcwt_reg;
  assign memory_channels_b_channel_rsci_bcwt_pff = memory_get_b_1_nor_rmff;
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      memory_channels_b_channel_rsci_bcwt_reg <= 1'b0;
    end
    else begin
      memory_channels_b_channel_rsci_bcwt_reg <= memory_get_b_1_nor_rmff;
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl
// ------------------------------------------------------------------


module dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl
    (
  core_wen, memory_channels_b_channel_rsci_oswt, memory_channels_b_channel_rsci_ivld_oreg,
      memory_channels_b_channel_rsci_biwt, memory_channels_b_channel_rsci_bdwt, memory_channels_b_channel_rsci_bcwt,
      memory_channels_b_channel_rsci_irdy_core_sct, memory_channels_b_channel_rsci_biwt_pff,
      memory_channels_b_channel_rsci_oswt_pff, memory_channels_b_channel_rsci_bcwt_pff,
      memory_channels_b_channel_rsci_ivld_oreg_pff
);
  input core_wen;
  input memory_channels_b_channel_rsci_oswt;
  input memory_channels_b_channel_rsci_ivld_oreg;
  output memory_channels_b_channel_rsci_biwt;
  output memory_channels_b_channel_rsci_bdwt;
  input memory_channels_b_channel_rsci_bcwt;
  output memory_channels_b_channel_rsci_irdy_core_sct;
  output memory_channels_b_channel_rsci_biwt_pff;
  input memory_channels_b_channel_rsci_oswt_pff;
  input memory_channels_b_channel_rsci_bcwt_pff;
  input memory_channels_b_channel_rsci_ivld_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_b_channel_rsci_ogwt;


  // Interconnect Declarations for Component Instantiations 
  assign memory_channels_b_channel_rsci_bdwt = memory_channels_b_channel_rsci_oswt
      & core_wen;
  assign memory_channels_b_channel_rsci_ogwt = memory_channels_b_channel_rsci_oswt
      & (~ memory_channels_b_channel_rsci_bcwt);
  assign memory_channels_b_channel_rsci_irdy_core_sct = memory_channels_b_channel_rsci_ogwt;
  assign memory_channels_b_channel_rsci_biwt = memory_channels_b_channel_rsci_ogwt
      & memory_channels_b_channel_rsci_ivld_oreg;
  assign memory_channels_b_channel_rsci_biwt_pff = memory_channels_b_channel_rsci_oswt_pff
      & (~ memory_channels_b_channel_rsci_bcwt_pff) & memory_channels_b_channel_rsci_ivld_oreg_pff;
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
//  Design Unit:    dense_core_done_rsci_done_wait_dp
// ------------------------------------------------------------------


module dense_core_done_rsci_done_wait_dp (
  clk, arst_n, done_rsci_oswt, done_rsci_wen_comp, done_rsci_biwt, done_rsci_bdwt,
      done_rsci_bcwt, done_rsci_wen_comp_pff, done_rsci_oswt_pff, done_rsci_biwt_pff,
      done_rsci_bcwt_pff
);
  input clk;
  input arst_n;
  input done_rsci_oswt;
  output done_rsci_wen_comp;
  input done_rsci_biwt;
  input done_rsci_bdwt;
  output done_rsci_bcwt;
  output done_rsci_wen_comp_pff;
  input done_rsci_oswt_pff;
  input done_rsci_biwt_pff;
  output done_rsci_bcwt_pff;


  // Interconnect Declarations
  reg done_rsci_bcwt_reg;
  wire nor_rmff;


  // Interconnect Declarations for Component Instantiations 
  assign nor_rmff = ~((~(done_rsci_bcwt | done_rsci_biwt)) | done_rsci_bdwt);
  assign done_rsci_wen_comp = (~ done_rsci_oswt) | done_rsci_biwt | done_rsci_bcwt;
  assign done_rsci_wen_comp_pff = (~ done_rsci_oswt_pff) | done_rsci_biwt_pff | done_rsci_bcwt_pff;
  assign done_rsci_bcwt = done_rsci_bcwt_reg;
  assign done_rsci_bcwt_pff = nor_rmff;
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      done_rsci_bcwt_reg <= 1'b0;
    end
    else begin
      done_rsci_bcwt_reg <= nor_rmff;
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_done_rsci_done_wait_ctrl
// ------------------------------------------------------------------


module dense_core_done_rsci_done_wait_ctrl (
  core_wen, done_rsci_oswt, done_rsci_irdy_oreg, done_rsci_biwt, done_rsci_bdwt,
      done_rsci_bcwt, done_rsci_ivld_core_sct, done_rsci_biwt_pff, done_rsci_oswt_pff,
      done_rsci_bcwt_pff, done_rsci_irdy_oreg_pff
);
  input core_wen;
  input done_rsci_oswt;
  input done_rsci_irdy_oreg;
  output done_rsci_biwt;
  output done_rsci_bdwt;
  input done_rsci_bcwt;
  output done_rsci_ivld_core_sct;
  output done_rsci_biwt_pff;
  input done_rsci_oswt_pff;
  input done_rsci_bcwt_pff;
  input done_rsci_irdy_oreg_pff;


  // Interconnect Declarations
  wire done_rsci_ogwt;


  // Interconnect Declarations for Component Instantiations 
  assign done_rsci_bdwt = done_rsci_oswt & core_wen;
  assign done_rsci_ogwt = done_rsci_oswt & (~ done_rsci_bcwt);
  assign done_rsci_ivld_core_sct = done_rsci_ogwt;
  assign done_rsci_biwt = done_rsci_ogwt & done_rsci_irdy_oreg;
  assign done_rsci_biwt_pff = done_rsci_oswt_pff & (~ done_rsci_bcwt_pff) & done_rsci_irdy_oreg_pff;
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
//  Design Unit:    dense_core_out_vector_length_triosy_obj
// ------------------------------------------------------------------


module dense_core_out_vector_length_triosy_obj (
  out_vector_length_triosy_lz, core_wten, out_vector_length_triosy_obj_iswt0
);
  output out_vector_length_triosy_lz;
  input core_wten;
  input out_vector_length_triosy_obj_iswt0;


  // Interconnect Declarations
  wire out_vector_length_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) out_vector_length_triosy_obj (
      .ld(out_vector_length_triosy_obj_biwt),
      .lz(out_vector_length_triosy_lz)
    );
  dense_core_out_vector_length_triosy_obj_out_vector_length_triosy_wait_ctrl dense_core_out_vector_length_triosy_obj_out_vector_length_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .out_vector_length_triosy_obj_iswt0(out_vector_length_triosy_obj_iswt0),
      .out_vector_length_triosy_obj_biwt(out_vector_length_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_in_vector_length_triosy_obj
// ------------------------------------------------------------------


module dense_core_in_vector_length_triosy_obj (
  in_vector_length_triosy_lz, core_wten, in_vector_length_triosy_obj_iswt0
);
  output in_vector_length_triosy_lz;
  input core_wten;
  input in_vector_length_triosy_obj_iswt0;


  // Interconnect Declarations
  wire in_vector_length_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) in_vector_length_triosy_obj (
      .ld(in_vector_length_triosy_obj_biwt),
      .lz(in_vector_length_triosy_lz)
    );
  dense_core_in_vector_length_triosy_obj_in_vector_length_triosy_wait_ctrl dense_core_in_vector_length_triosy_obj_in_vector_length_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .in_vector_length_triosy_obj_iswt0(in_vector_length_triosy_obj_iswt0),
      .in_vector_length_triosy_obj_biwt(in_vector_length_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_output_addr_triosy_obj
// ------------------------------------------------------------------


module dense_core_output_addr_triosy_obj (
  output_addr_triosy_lz, core_wten, output_addr_triosy_obj_iswt0
);
  output output_addr_triosy_lz;
  input core_wten;
  input output_addr_triosy_obj_iswt0;


  // Interconnect Declarations
  wire output_addr_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) output_addr_triosy_obj (
      .ld(output_addr_triosy_obj_biwt),
      .lz(output_addr_triosy_lz)
    );
  dense_core_output_addr_triosy_obj_output_addr_triosy_wait_ctrl dense_core_output_addr_triosy_obj_output_addr_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .output_addr_triosy_obj_iswt0(output_addr_triosy_obj_iswt0),
      .output_addr_triosy_obj_biwt(output_addr_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_weight_addr_triosy_obj
// ------------------------------------------------------------------


module dense_core_weight_addr_triosy_obj (
  weight_addr_triosy_lz, core_wten, weight_addr_triosy_obj_iswt0
);
  output weight_addr_triosy_lz;
  input core_wten;
  input weight_addr_triosy_obj_iswt0;


  // Interconnect Declarations
  wire weight_addr_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) weight_addr_triosy_obj (
      .ld(weight_addr_triosy_obj_biwt),
      .lz(weight_addr_triosy_lz)
    );
  dense_core_weight_addr_triosy_obj_weight_addr_triosy_wait_ctrl dense_core_weight_addr_triosy_obj_weight_addr_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .weight_addr_triosy_obj_iswt0(weight_addr_triosy_obj_iswt0),
      .weight_addr_triosy_obj_biwt(weight_addr_triosy_obj_biwt)
    );
endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_core_feature_addr_triosy_obj
// ------------------------------------------------------------------


module dense_core_feature_addr_triosy_obj (
  feature_addr_triosy_lz, core_wten, feature_addr_triosy_obj_iswt0
);
  output feature_addr_triosy_lz;
  input core_wten;
  input feature_addr_triosy_obj_iswt0;


  // Interconnect Declarations
  wire feature_addr_triosy_obj_biwt;


  // Interconnect Declarations for Component Instantiations 
  mgc_io_sync_v2 #(.valid(32'sd0)) feature_addr_triosy_obj (
      .ld(feature_addr_triosy_obj_biwt),
      .lz(feature_addr_triosy_lz)
    );
  dense_core_feature_addr_triosy_obj_feature_addr_triosy_wait_ctrl dense_core_feature_addr_triosy_obj_feature_addr_triosy_wait_ctrl_inst
      (
      .core_wten(core_wten),
      .feature_addr_triosy_obj_iswt0(feature_addr_triosy_obj_iswt0),
      .feature_addr_triosy_obj_biwt(feature_addr_triosy_obj_biwt)
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
  ccs_ctrl_in_buf_wait_v4 #(.rscid(32'sd14),
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
  assign nl_memory_channels_ar_channel_rsci_idat = {16'b0000000000000100 , (memory_channels_ar_channel_rsci_idat[92:29])
      , 29'b11111111110010000000000000000};
  ccs_out_buf_wait_v5 #(.rscid(32'sd13),
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
      memory_channels_b_channel_rsc_rdy, core_wen, memory_channels_b_channel_rsci_oswt,
      memory_channels_b_channel_rsci_wen_comp, memory_channels_b_channel_rsci_ivld,
      memory_channels_b_channel_rsci_ivld_oreg, memory_channels_b_channel_rsci_wen_comp_pff,
      memory_channels_b_channel_rsci_oswt_pff, memory_channels_b_channel_rsci_ivld_oreg_pff
);
  input clk;
  input arst_n;
  input [17:0] memory_channels_b_channel_rsc_dat;
  input memory_channels_b_channel_rsc_vld;
  output memory_channels_b_channel_rsc_rdy;
  input core_wen;
  input memory_channels_b_channel_rsci_oswt;
  output memory_channels_b_channel_rsci_wen_comp;
  output memory_channels_b_channel_rsci_ivld;
  input memory_channels_b_channel_rsci_ivld_oreg;
  output memory_channels_b_channel_rsci_wen_comp_pff;
  input memory_channels_b_channel_rsci_oswt_pff;
  input memory_channels_b_channel_rsci_ivld_oreg_pff;


  // Interconnect Declarations
  wire memory_channels_b_channel_rsci_biwt;
  wire memory_channels_b_channel_rsci_bdwt;
  wire memory_channels_b_channel_rsci_bcwt;
  wire memory_channels_b_channel_rsci_irdy_core_sct;
  wire [17:0] memory_channels_b_channel_rsci_idat;
  wire memory_channels_b_channel_rsc_is_idle;
  wire memory_channels_b_channel_rsci_wen_comp_reg;
  wire memory_channels_b_channel_rsci_wen_comp_iff;
  wire memory_channels_b_channel_rsci_biwt_iff;
  wire memory_channels_b_channel_rsci_bcwt_iff;


  // Interconnect Declarations for Component Instantiations 
  ccs_ctrl_in_buf_wait_v4 #(.rscid(32'sd12),
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
      .irdy(memory_channels_b_channel_rsci_irdy_core_sct),
      .ivld(memory_channels_b_channel_rsci_ivld),
      .idat(memory_channels_b_channel_rsci_idat),
      .is_idle(memory_channels_b_channel_rsc_is_idle)
    );
  dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_ctrl_inst
      (
      .core_wen(core_wen),
      .memory_channels_b_channel_rsci_oswt(memory_channels_b_channel_rsci_oswt),
      .memory_channels_b_channel_rsci_ivld_oreg(memory_channels_b_channel_rsci_ivld_oreg),
      .memory_channels_b_channel_rsci_biwt(memory_channels_b_channel_rsci_biwt),
      .memory_channels_b_channel_rsci_bdwt(memory_channels_b_channel_rsci_bdwt),
      .memory_channels_b_channel_rsci_bcwt(memory_channels_b_channel_rsci_bcwt),
      .memory_channels_b_channel_rsci_irdy_core_sct(memory_channels_b_channel_rsci_irdy_core_sct),
      .memory_channels_b_channel_rsci_biwt_pff(memory_channels_b_channel_rsci_biwt_iff),
      .memory_channels_b_channel_rsci_oswt_pff(memory_channels_b_channel_rsci_oswt_pff),
      .memory_channels_b_channel_rsci_bcwt_pff(memory_channels_b_channel_rsci_bcwt_iff),
      .memory_channels_b_channel_rsci_ivld_oreg_pff(memory_channels_b_channel_rsci_ivld_oreg_pff)
    );
  dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_dp dense_core_memory_channels_b_channel_rsci_memory_channels_b_channel_wait_dp_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_b_channel_rsci_oswt(memory_channels_b_channel_rsci_oswt),
      .memory_channels_b_channel_rsci_wen_comp(memory_channels_b_channel_rsci_wen_comp_reg),
      .memory_channels_b_channel_rsci_biwt(memory_channels_b_channel_rsci_biwt),
      .memory_channels_b_channel_rsci_bdwt(memory_channels_b_channel_rsci_bdwt),
      .memory_channels_b_channel_rsci_bcwt(memory_channels_b_channel_rsci_bcwt),
      .memory_channels_b_channel_rsci_wen_comp_pff(memory_channels_b_channel_rsci_wen_comp_iff),
      .memory_channels_b_channel_rsci_oswt_pff(memory_channels_b_channel_rsci_oswt_pff),
      .memory_channels_b_channel_rsci_biwt_pff(memory_channels_b_channel_rsci_biwt_iff),
      .memory_channels_b_channel_rsci_bcwt_pff(memory_channels_b_channel_rsci_bcwt_iff)
    );
  assign memory_channels_b_channel_rsci_wen_comp = memory_channels_b_channel_rsci_wen_comp_reg;
  assign memory_channels_b_channel_rsci_wen_comp_pff = memory_channels_b_channel_rsci_wen_comp_iff;
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
  ccs_out_buf_wait_v5 #(.rscid(32'sd11),
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
  assign nl_memory_channels_aw_channel_rsci_idat = {74'b00000000000000000000000000000000000000000000000000000000000000000000000000
      , (memory_channels_aw_channel_rsci_idat[34:29]) , 29'b00000000110011000000000000000};
  ccs_out_buf_wait_v5 #(.rscid(32'sd10),
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
  clk, arst_n, done_rsc_dat, done_rsc_vld, done_rsc_rdy, core_wen, done_rsci_oswt,
      done_rsci_wen_comp, done_rsci_irdy, done_rsci_irdy_oreg, done_rsci_wen_comp_pff,
      done_rsci_oswt_pff, done_rsci_irdy_oreg_pff
);
  input clk;
  input arst_n;
  output done_rsc_dat;
  output done_rsc_vld;
  input done_rsc_rdy;
  input core_wen;
  input done_rsci_oswt;
  output done_rsci_wen_comp;
  output done_rsci_irdy;
  input done_rsci_irdy_oreg;
  output done_rsci_wen_comp_pff;
  input done_rsci_oswt_pff;
  input done_rsci_irdy_oreg_pff;


  // Interconnect Declarations
  wire done_rsci_biwt;
  wire done_rsci_bdwt;
  wire done_rsci_bcwt;
  wire done_rsci_ivld_core_sct;
  wire done_rsc_is_idle;
  wire done_rsci_wen_comp_reg;
  wire done_rsci_wen_comp_iff;
  wire done_rsci_biwt_iff;
  wire done_rsci_bcwt_iff;


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
      .ivld(done_rsci_ivld_core_sct),
      .idat(1'b1),
      .rdy(done_rsc_rdy),
      .vld(done_rsc_vld),
      .dat(done_rsc_dat),
      .is_idle(done_rsc_is_idle)
    );
  dense_core_done_rsci_done_wait_ctrl dense_core_done_rsci_done_wait_ctrl_inst (
      .core_wen(core_wen),
      .done_rsci_oswt(done_rsci_oswt),
      .done_rsci_irdy_oreg(done_rsci_irdy_oreg),
      .done_rsci_biwt(done_rsci_biwt),
      .done_rsci_bdwt(done_rsci_bdwt),
      .done_rsci_bcwt(done_rsci_bcwt),
      .done_rsci_ivld_core_sct(done_rsci_ivld_core_sct),
      .done_rsci_biwt_pff(done_rsci_biwt_iff),
      .done_rsci_oswt_pff(done_rsci_oswt_pff),
      .done_rsci_bcwt_pff(done_rsci_bcwt_iff),
      .done_rsci_irdy_oreg_pff(done_rsci_irdy_oreg_pff)
    );
  dense_core_done_rsci_done_wait_dp dense_core_done_rsci_done_wait_dp_inst (
      .clk(clk),
      .arst_n(arst_n),
      .done_rsci_oswt(done_rsci_oswt),
      .done_rsci_wen_comp(done_rsci_wen_comp_reg),
      .done_rsci_biwt(done_rsci_biwt),
      .done_rsci_bdwt(done_rsci_bdwt),
      .done_rsci_bcwt(done_rsci_bcwt),
      .done_rsci_wen_comp_pff(done_rsci_wen_comp_iff),
      .done_rsci_oswt_pff(done_rsci_oswt_pff),
      .done_rsci_biwt_pff(done_rsci_biwt_iff),
      .done_rsci_bcwt_pff(done_rsci_bcwt_iff)
    );
  assign done_rsci_wen_comp = done_rsci_wen_comp_reg;
  assign done_rsci_wen_comp_pff = done_rsci_wen_comp_iff;
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
      done_rsc_rdy, use_relu_triosy_lz, addr_hi_rsc_dat, addr_hi_triosy_lz, feature_addr_rsc_dat,
      feature_addr_triosy_lz, weight_addr_rsc_dat, weight_addr_triosy_lz, output_addr_rsc_dat,
      output_addr_triosy_lz, in_vector_length_rsc_dat, in_vector_length_triosy_lz,
      out_vector_length_triosy_lz, memory_channels_aw_channel_rsc_dat, memory_channels_aw_channel_rsc_vld,
      memory_channels_aw_channel_rsc_rdy, memory_channels_w_channel_rsc_dat, memory_channels_w_channel_rsc_vld,
      memory_channels_w_channel_rsc_rdy, memory_channels_b_channel_rsc_dat, memory_channels_b_channel_rsc_vld,
      memory_channels_b_channel_rsc_rdy, memory_channels_ar_channel_rsc_dat, memory_channels_ar_channel_rsc_vld,
      memory_channels_ar_channel_rsc_rdy, memory_channels_r_channel_rsc_dat, memory_channels_r_channel_rsc_vld,
      memory_channels_r_channel_rsc_rdy, feature_buffer_rsci_clken_d, feature_buffer_rsci_q_d,
      feature_buffer_rsci_radr_d, feature_buffer_rsci_wadr_d, weight_buffer_rsci_q_d,
      weight_buffer_rsci_radr_d, weight_buffer_rsci_wadr_d, sum_array_rsci_d_d, sum_array_rsci_q_d,
      sum_array_rsci_radr_d, sum_array_rsci_wadr_d, feature_buffer_rsci_d_d_pff,
      feature_buffer_rsci_re_d_pff, feature_buffer_rsci_we_d_pff, weight_buffer_rsci_we_d_pff,
      sum_array_rsci_re_d_pff, sum_array_rsci_we_d_pff
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
  input [31:0] feature_addr_rsc_dat;
  output feature_addr_triosy_lz;
  input [31:0] weight_addr_rsc_dat;
  output weight_addr_triosy_lz;
  input [31:0] output_addr_rsc_dat;
  output output_addr_triosy_lz;
  input [31:0] in_vector_length_rsc_dat;
  output in_vector_length_triosy_lz;
  output out_vector_length_triosy_lz;
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
  output feature_buffer_rsci_clken_d;
  input [511:0] feature_buffer_rsci_q_d;
  output [14:0] feature_buffer_rsci_radr_d;
  output [14:0] feature_buffer_rsci_wadr_d;
  input [511:0] weight_buffer_rsci_q_d;
  output [7:0] weight_buffer_rsci_radr_d;
  output [7:0] weight_buffer_rsci_wadr_d;
  output [31:0] sum_array_rsci_d_d;
  input [31:0] sum_array_rsci_q_d;
  output [4:0] sum_array_rsci_radr_d;
  output [4:0] sum_array_rsci_wadr_d;
  output [511:0] feature_buffer_rsci_d_d_pff;
  output feature_buffer_rsci_re_d_pff;
  output feature_buffer_rsci_we_d_pff;
  output weight_buffer_rsci_we_d_pff;
  output sum_array_rsci_re_d_pff;
  output sum_array_rsci_we_d_pff;


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
  wire [31:0] feature_addr_rsci_idat;
  wire [31:0] weight_addr_rsci_idat;
  wire [31:0] output_addr_rsci_idat;
  wire [31:0] in_vector_length_rsci_idat;
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
  wire [31:0] while_while_for_1_while_while_for_acc_3_cmp_z;
  reg [5:0] memory_channels_aw_channel_rsci_idat_34_29;
  reg [511:0] memory_channels_w_channel_rsci_idat_576_65;
  reg [63:0] memory_channels_w_channel_rsci_idat_64_1;
  reg [13:0] memory_channels_ar_channel_rsci_idat_42_29;
  wire [6:0] fsm_output;
  wire and_dcpl_3;
  wire nor_tmp_1;
  wire mux_tmp_2;
  wire not_tmp_11;
  wire or_dcpl_8;
  wire and_dcpl_21;
  wire and_dcpl_22;
  wire and_dcpl_23;
  wire and_dcpl_24;
  wire and_dcpl_25;
  wire mux_tmp_31;
  wire and_dcpl_31;
  wire and_dcpl_32;
  wire or_tmp_14;
  wire or_tmp_17;
  wire and_dcpl_38;
  wire and_dcpl_39;
  wire and_dcpl_40;
  wire and_dcpl_44;
  wire and_dcpl_46;
  wire and_dcpl_47;
  wire and_dcpl_48;
  wire and_dcpl_49;
  wire and_dcpl_51;
  wire xor_dcpl_1;
  wire and_dcpl_59;
  wire and_dcpl_74;
  wire and_dcpl_76;
  wire and_dcpl_79;
  wire not_tmp_48;
  wire nor_tmp_13;
  wire and_dcpl_105;
  wire or_tmp_44;
  wire and_dcpl_107;
  wire not_tmp_53;
  wire and_dcpl_111;
  wire mux_tmp_52;
  wire mux_tmp_60;
  wire and_dcpl_129;
  wire and_dcpl_131;
  wire and_dcpl_132;
  wire and_dcpl_133;
  wire and_dcpl_134;
  wire and_dcpl_135;
  wire and_dcpl_136;
  wire and_dcpl_137;
  wire and_dcpl_138;
  wire and_dcpl_139;
  wire and_dcpl_140;
  wire and_dcpl_141;
  wire and_dcpl_142;
  wire and_dcpl_143;
  wire and_dcpl_144;
  wire and_dcpl_145;
  wire and_dcpl_146;
  wire and_dcpl_147;
  wire and_dcpl_148;
  wire and_dcpl_149;
  wire and_dcpl_150;
  wire and_dcpl_151;
  wire and_dcpl_152;
  wire and_dcpl_153;
  wire and_dcpl_154;
  wire and_dcpl_155;
  wire and_dcpl_156;
  wire and_dcpl_157;
  wire and_dcpl_158;
  wire and_dcpl_159;
  wire and_dcpl_160;
  wire and_dcpl_161;
  wire and_dcpl_162;
  wire and_dcpl_163;
  wire and_dcpl_164;
  wire and_dcpl_165;
  wire and_dcpl_166;
  wire and_dcpl_167;
  wire and_dcpl_168;
  wire mux_tmp_69;
  wire or_tmp_60;
  wire mux_tmp_70;
  wire and_dcpl_170;
  wire mux_tmp_73;
  wire and_dcpl_172;
  wire and_dcpl_175;
  wire and_dcpl_178;
  wire or_dcpl_13;
  wire or_dcpl_20;
  wire or_dcpl_22;
  wire or_dcpl_27;
  wire and_dcpl_179;
  wire mux_tmp_89;
  wire or_tmp_67;
  wire or_tmp_68;
  wire mux_tmp_90;
  wire mux_tmp_93;
  wire mux_tmp_94;
  wire mux_tmp_96;
  wire and_dcpl_190;
  wire not_tmp_71;
  wire and_dcpl_201;
  wire or_dcpl_38;
  wire or_dcpl_41;
  wire or_dcpl_47;
  wire or_tmp_87;
  wire or_tmp_89;
  wire mux_tmp_124;
  wire mux_tmp_125;
  wire mux_tmp_130;
  wire or_tmp_94;
  wire or_tmp_96;
  wire and_dcpl_206;
  wire and_dcpl_207;
  wire and_dcpl_208;
  reg for_for_nor_itm;
  wire exit_while_while_sva_mx0;
  wire xor_cse_1;
  reg [8:0] weight_index_8_0_sva_1;
  wire [49:0] operator_64_false_1_acc_sdt;
  wire [50:0] nl_operator_64_false_1_acc_sdt;
  reg [31:0] memory_channels_ar_channel_rsci_idat_92_61;
  reg [10:0] memory_channels_ar_channel_rsci_idat_60_50;
  reg [6:0] memory_channels_ar_channel_rsci_idat_49_43;
  wire nor_81_ssc;
  wire i_and_ssc;
  wire memory_send_ar_and_cse;
  wire memory_send_w_1_and_cse;
  wire outputs_and_cse;
  wire or_67_cse;
  reg reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse;
  wire while_while_for_and_cse;
  wire and_1_cse;
  wire nor_27_cse;
  wire or_156_cse;
  wire and_242_cse;
  wire nor_31_cse;
  wire and_243_cse;
  wire or_68_cse;
  wire or_159_cse;
  wire and_231_cse;
  wire mux_3_cse;
  wire mux_23_cse;
  wire memory_axi_burst_read_base_axi_u512_512_1_for_beat_and_ssc;
  wire mux_24_cse;
  wire memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c2;
  reg [511:0] while_while_for_read_mem_feature_buffer_rsc_cse_sva;
  reg [511:0] while_while_for_read_mem_weight_buffer_rsc_cse_sva;
  reg [31:0] features_io_read_feature_addr_rsc_cse_sva;
  reg [31:0] in_vector_length_sva;
  reg [2:0] memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_5;
  reg [4:0] memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0;
  reg [31:0] input_index_sva;
  wire core_wen_rtff;
  reg reg_start_rsci_oswt_tmp;
  reg reg_done_rsci_oswt_tmp;
  reg reg_memory_channels_aw_channel_rsci_oswt_tmp;
  reg reg_memory_channels_w_channel_rsci_oswt_tmp;
  reg reg_memory_channels_ar_channel_rsci_oswt_tmp;
  reg reg_memory_channels_r_channel_rsci_oswt_tmp;
  wire start_rsci_wen_comp_iff;
  wire mux_rmff;
  wire done_rsci_wen_comp_iff;
  wire mux_152_rmff;
  wire memory_channels_aw_channel_rsci_wen_comp_iff;
  wire memory_send_aw_1_mux_rmff;
  wire memory_channels_w_channel_rsci_wen_comp_iff;
  wire memory_send_w_1_mux_rmff;
  wire memory_channels_b_channel_rsci_wen_comp_iff;
  wire memory_channels_ar_channel_rsci_wen_comp_iff;
  wire memory_send_ar_mux_rmff;
  wire memory_channels_r_channel_rsci_wen_comp_iff;
  wire memory_get_r_mux_rmff;
  reg [31:0] weights_io_read_weight_addr_rsc_cse_sva;
  wire [511:0] memory_axi_write_base_axi_u512_512_1_lshift_itm;
  wire [63:0] memory_encode_strb_1_if_6_lshift_itm_1;
  wire mux_tmp;
  wire [15:0] z_out;
  wire [17:0] nl_z_out;
  wire and_dcpl_232;
  wire and_dcpl_236;
  wire and_dcpl_239;
  wire and_dcpl_244;
  wire [42:0] z_out_1;
  wire [43:0] nl_z_out_1;
  wire and_dcpl_251;
  wire [8:0] z_out_2;
  wire [9:0] nl_z_out_2;
  wire and_dcpl_253;
  wire and_dcpl_254;
  wire and_dcpl_255;
  wire [15:0] z_out_3;
  wire [16:0] nl_z_out_3;
  reg [31:0] weights_acc_psp_sva;
  wire [32:0] nl_weights_acc_psp_sva;
  reg [5:0] outputs_slc_output_addr_30_0_psp_5_0_sva;
  reg [26:0] operator_32_true_operator_32_true_acc_psp_sva;
  wire [27:0] nl_operator_32_true_operator_32_true_acc_psp_sva;
  reg [15:0] while_for_slc_23_8_11_itm;
  reg [15:0] while_for_acc_10_itm;
  reg [15:0] while_for_acc_14_itm;
  reg [15:0] while_for_acc_13_itm;
  reg [15:0] while_for_acc_17_itm;
  reg [63:0] memory_encode_strb_1_if_6_lshift_itm;
  wire in_vector_length_sva_mx0c1;
  wire i_1_31_13_sva_mx0c1;
  wire i_1_31_13_sva_mx0c2;
  wire [18:0] i_1_31_13_sva_2;
  wire [19:0] nl_i_1_31_13_sva_2;
  wire features_io_read_feature_addr_rsc_cse_sva_mx0c1;
  wire features_io_read_feature_addr_rsc_cse_sva_mx0c2;
  wire input_index_sva_mx0c1;
  wire memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c1;
  wire [17:0] weight_page_17_0_sva_1_mx0w1;
  wire [18:0] nl_weight_page_17_0_sva_1_mx0w1;
  wire while_for_acc_10_itm_mx0c1;
  wire [15:0] while_for_acc_8_itm_mx0w1;
  wire [17:0] nl_while_for_acc_8_itm_mx0w1;
  wire while_for_acc_13_itm_mx0c1;
  wire while_for_acc_17_itm_mx0c0;
  wire while_for_acc_17_itm_mx0c2;
  wire while_for_acc_17_itm_mx0c3;
  reg [15:0] i_1_31_13_sva_15_0;
  reg i_1_31_13_sva_18;
  reg [1:0] i_1_31_13_sva_17_16;
  wire nand_2_cse;
  wire or_119_cse;
  wire or_160_cse;
  wire or_dcpl;
  wire xnor_2_tmp;
  wire or_tmp_115;

  wire while_nor_nl;
  wire mux_30_nl;
  wire or_170_nl;
  wire nand_16_nl;
  wire and_26_nl;
  wire and_30_nl;
  wire and_33_nl;
  wire and_36_nl;
  wire mux_35_nl;
  wire mux_34_nl;
  wire mux_33_nl;
  wire mux_32_nl;
  wire or_3_nl;
  wire or_26_nl;
  wire asn_memory_channels_r_channel_rsci_oswt_nor_nl;
  wire mux_40_nl;
  wire mux_39_nl;
  wire mux_38_nl;
  wire or_157_nl;
  wire mux_37_nl;
  wire mux_36_nl;
  wire or_158_nl;
  wire or_162_nl;
  wire or_nl;
  wire operator_32_true_and_nl;
  wire mux_120_nl;
  wire mux_119_nl;
  wire mux_118_nl;
  wire or_127_nl;
  wire mux_99_nl;
  wire mux_98_nl;
  wire mux_97_nl;
  wire mux_95_nl;
  wire mux_91_nl;
  wire[1:0] i_i_mux1h_nl;
  wire i_and_5_nl;
  wire not_364_nl;
  wire[15:0] i_mux1h_2_nl;
  wire and_206_nl;
  wire mux_140_nl;
  wire nand_23_nl;
  wire not_365_nl;
  wire mux_166_nl;
  wire mux_165_nl;
  wire mux_164_nl;
  wire mux_163_nl;
  wire mux_162_nl;
  wire input_index_not_1_nl;
  wire mux_113_nl;
  wire mux_112_nl;
  wire[2:0] memory_axi_burst_read_base_axi_u512_512_1_for_beat_memory_axi_burst_read_base_axi_u512_512_1_for_beat_mux_nl;
  wire memory_axi_burst_read_base_axi_u512_512_1_for_beat_not_1_nl;
  wire[4:0] memory_axi_burst_read_base_axi_u512_512_1_for_beat_and_2_nl;
  wire[4:0] memory_axi_burst_read_base_axi_u512_512_1_for_beat_memory_axi_burst_read_base_axi_u512_512_1_for_beat_mux_1_nl;
  wire[4:0] for_for_or_nl;
  wire or_125_nl;
  wire memory_axi_burst_read_base_axi_u512_512_1_for_beat_not_nl;
  wire mux_170_nl;
  wire mux_169_nl;
  wire mux_168_nl;
  wire mux_167_nl;
  wire nor_101_nl;
  wire and_298_nl;
  wire mux_128_nl;
  wire mux_127_nl;
  wire mux_126_nl;
  wire mux_123_nl;
  wire mux_122_nl;
  wire mux_121_nl;
  wire mux_132_nl;
  wire mux_131_nl;
  wire mux_129_nl;
  wire or_141_nl;
  wire and_214_nl;
  wire mux_146_nl;
  wire or_115_nl;
  wire while_for_or_4_nl;
  wire or_24_nl;
  wire or_23_nl;
  wire mux_44_nl;
  wire nand_14_nl;
  wire or_169_nl;
  wire nor_44_nl;
  wire nor_45_nl;
  wire mux_48_nl;
  wire nor_48_nl;
  wire nor_49_nl;
  wire mux_68_nl;
  wire nor_17_nl;
  wire mux_66_nl;
  wire or_71_nl;
  wire or_74_nl;
  wire mux_92_nl;
  wire or_140_nl;
  wire mux_79_nl;
  wire mux_78_nl;
  wire mux_77_nl;
  wire mux_76_nl;
  wire mux_75_nl;
  wire mux_88_nl;
  wire mux_85_nl;
  wire mux_84_nl;
  wire or_95_nl;
  wire mux_117_nl;
  wire mux_116_nl;
  wire mux_134_nl;
  wire mux_133_nl;
  wire mux_41_nl;
  wire or_39_nl;
  wire nor_55_nl;
  wire[4:0] nor_nl;
  wire[4:0] mux1h_nl;
  wire and_64_nl;
  wire mux_42_nl;
  wire nand_18_nl;
  wire or_171_nl;
  wire and_66_nl;
  wire nor_60_nl;
  wire mux_43_nl;
  wire or_172_nl;
  wire or_173_nl;
  wire and_72_nl;
  wire and_75_nl;
  wire and_78_nl;
  wire and_81_nl;
  wire and_83_nl;
  wire and_85_nl;
  wire and_87_nl;
  wire and_88_nl;
  wire mux_46_nl;
  wire nor_61_nl;
  wire nor_62_nl;
  wire and_91_nl;
  wire and_93_nl;
  wire and_95_nl;
  wire and_97_nl;
  wire and_99_nl;
  wire and_101_nl;
  wire and_103_nl;
  wire nor_64_nl;
  wire mux_47_nl;
  wire or_174_nl;
  wire nand_21_nl;
  wire and_108_nl;
  wire and_110_nl;
  wire and_113_nl;
  wire and_114_nl;
  wire mux_50_nl;
  wire nor_65_nl;
  wire nor_66_nl;
  wire and_116_nl;
  wire and_118_nl;
  wire and_120_nl;
  wire nor_69_nl;
  wire mux_51_nl;
  wire or_175_nl;
  wire or_176_nl;
  wire and_124_nl;
  wire and_126_nl;
  wire and_128_nl;
  wire mux_58_nl;
  wire mux_57_nl;
  wire mux_56_nl;
  wire and_240_nl;
  wire mux_55_nl;
  wire mux_54_nl;
  wire mux_53_nl;
  wire or_66_nl;
  wire and_61_nl;
  wire mux_65_nl;
  wire mux_64_nl;
  wire mux_63_nl;
  wire mux_62_nl;
  wire mux_61_nl;
  wire mux_59_nl;
  wire[4:0] for_and_nl;
  wire[4:0] for_mux1h_nl;
  wire and_130_nl;
  wire for_nor_nl;
  wire mux_71_nl;
  wire mux_72_nl;
  wire mux_160_nl;
  wire mux_159_nl;
  wire or_192_nl;
  wire mux_158_nl;
  wire or_195_nl;
  wire[15:0] while_for_mux1h_5_nl;
  wire and_250_nl;
  wire and_255_nl;
  wire mux_157_nl;
  wire mux_156_nl;
  wire and_nl;
  wire nor_86_nl;
  wire mux_155_nl;
  wire nand_27_nl;
  wire or_182_nl;
  wire mux_154_nl;
  wire and_293_nl;
  wire nor_87_nl;
  wire operator_64_false_operator_64_false_and_nl;
  wire[9:0] operator_64_false_operator_64_false_and_1_nl;
  wire[9:0] operator_64_false_mux_nl;
  wire not_367_nl;
  wire operator_64_false_mux1h_nl;
  wire[19:0] operator_64_false_mux1h_3_nl;
  wire[10:0] operator_64_false_mux1h_2_nl;
  wire[10:0] operator_64_false_mux1h_1_nl;
  wire[2:0] for_mux_2_nl;
  wire[15:0] while_for_mux1h_6_nl;
  wire and_283_nl;
  wire and_286_nl;
  wire and_291_nl;

  // Interconnect Declarations for Component Instantiations 
  wire [15:0] nl_while_while_for_1_while_while_for_acc_3_cmp_a;
  assign nl_while_while_for_1_while_while_for_acc_3_cmp_a = MUX1HOT_v_16_32_2((while_while_for_read_mem_feature_buffer_rsc_cse_sva[511:496]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[495:480]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[479:464]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[463:448]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[447:432]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[431:416]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[415:400]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[399:384]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[383:368]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[367:352]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[351:336]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[335:320]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[319:304]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[303:288]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[287:272]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[271:256]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[255:240]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[239:224]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[223:208]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[207:192]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[191:176]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[175:160]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[159:144]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[143:128]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[127:112]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[111:96]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[95:80]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[79:64]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[63:48]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[47:32]), (while_while_for_read_mem_feature_buffer_rsc_cse_sva[31:16]),
      (while_while_for_read_mem_feature_buffer_rsc_cse_sva[15:0]), {and_dcpl_170
      , and_dcpl_131 , and_dcpl_134 , and_dcpl_135 , and_dcpl_137 , and_dcpl_138
      , and_dcpl_139 , and_dcpl_141 , and_dcpl_143 , and_dcpl_144 , and_dcpl_146
      , and_dcpl_147 , and_dcpl_148 , and_dcpl_149 , and_dcpl_150 , and_dcpl_151
      , and_dcpl_152 , and_dcpl_153 , and_dcpl_155 , and_dcpl_156 , and_dcpl_157
      , and_dcpl_158 , and_dcpl_159 , and_dcpl_160 , and_dcpl_161 , and_dcpl_162
      , and_dcpl_163 , and_dcpl_164 , and_dcpl_165 , and_dcpl_166 , and_dcpl_167
      , and_dcpl_168});
  wire [15:0] nl_while_while_for_1_while_while_for_acc_3_cmp_b;
  assign nl_while_while_for_1_while_while_for_acc_3_cmp_b = MUX1HOT_v_16_32_2((while_while_for_read_mem_weight_buffer_rsc_cse_sva[511:496]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[495:480]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[479:464]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[463:448]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[447:432]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[431:416]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[415:400]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[399:384]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[383:368]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[367:352]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[351:336]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[335:320]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[319:304]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[303:288]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[287:272]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[271:256]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[255:240]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[239:224]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[223:208]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[207:192]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[191:176]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[175:160]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[159:144]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[143:128]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[127:112]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[111:96]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[95:80]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[79:64]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[63:48]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[47:32]), (while_while_for_read_mem_weight_buffer_rsc_cse_sva[31:16]),
      (while_while_for_read_mem_weight_buffer_rsc_cse_sva[15:0]), {and_dcpl_170 ,
      and_dcpl_131 , and_dcpl_134 , and_dcpl_135 , and_dcpl_137 , and_dcpl_138 ,
      and_dcpl_139 , and_dcpl_141 , and_dcpl_143 , and_dcpl_144 , and_dcpl_146 ,
      and_dcpl_147 , and_dcpl_148 , and_dcpl_149 , and_dcpl_150 , and_dcpl_151 ,
      and_dcpl_152 , and_dcpl_153 , and_dcpl_155 , and_dcpl_156 , and_dcpl_157 ,
      and_dcpl_158 , and_dcpl_159 , and_dcpl_160 , and_dcpl_161 , and_dcpl_162 ,
      and_dcpl_163 , and_dcpl_164 , and_dcpl_165 , and_dcpl_166 , and_dcpl_167 ,
      and_dcpl_168});
  wire nor_71_nl;
  wire mux_74_nl;
  wire [31:0] nl_while_while_for_1_while_while_for_acc_3_cmp_c;
  assign mux_74_nl = MUX_s_1_2_2(mux_tmp_70, mux_tmp_73, fsm_output[0]);
  assign nor_71_nl = ~(mux_74_nl | (fsm_output[6]));
  assign nl_while_while_for_1_while_while_for_acc_3_cmp_c = MUX_v_32_2_2(features_io_read_feature_addr_rsc_cse_sva,
      in_vector_length_sva, nor_71_nl);
  wire[15:0] while_for_acc_nl;
  wire[16:0] nl_while_for_acc_nl;
  wire[15:0] while_for_acc_12_nl;
  wire[16:0] nl_while_for_acc_12_nl;
  wire [511:0] nl_memory_axi_write_base_axi_u512_512_1_lshift_rg_a;
  assign nl_while_for_acc_12_nl = while_for_acc_13_itm + while_for_acc_17_itm;
  assign while_for_acc_12_nl = nl_while_for_acc_12_nl[15:0];
  assign nl_while_for_acc_nl = i_1_31_13_sva_15_0 + while_for_acc_12_nl;
  assign while_for_acc_nl = nl_while_for_acc_nl[15:0];
  assign nl_memory_axi_write_base_axi_u512_512_1_lshift_rg_a = {496'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
      , while_for_acc_nl};
  wire [8:0] nl_memory_axi_write_base_axi_u512_512_1_lshift_rg_s;
  assign nl_memory_axi_write_base_axi_u512_512_1_lshift_rg_s = {outputs_slc_output_addr_30_0_psp_5_0_sva
      , 3'b000};
  wire [108:0] nl_dense_core_memory_channels_aw_channel_rsci_inst_memory_channels_aw_channel_rsci_idat;
  assign nl_dense_core_memory_channels_aw_channel_rsci_inst_memory_channels_aw_channel_rsci_idat
      = {74'b00000000000000000000000000000000000000000000000000000000000000000000000000
      , memory_channels_aw_channel_rsci_idat_34_29 , 29'b00000000110011000000000000000};
  wire [576:0] nl_dense_core_memory_channels_w_channel_rsci_inst_memory_channels_w_channel_rsci_idat;
  assign nl_dense_core_memory_channels_w_channel_rsci_inst_memory_channels_w_channel_rsci_idat
      = {memory_channels_w_channel_rsci_idat_576_65 , memory_channels_w_channel_rsci_idat_64_1
      , 1'b1};
  wire [108:0] nl_dense_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat;
  assign nl_dense_core_memory_channels_ar_channel_rsci_inst_memory_channels_ar_channel_rsci_idat
      = {16'b0000000000000100 , memory_channels_ar_channel_rsci_idat_92_61 , memory_channels_ar_channel_rsci_idat_60_50
      , memory_channels_ar_channel_rsci_idat_49_43 , memory_channels_ar_channel_rsci_idat_42_29
      , 29'b11111111110010000000000000000};
  wire  nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0;
  assign nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0
      = ~ reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse;
  wire  nl_dense_core_core_fsm_inst_do_C_2_tr0;
  assign nl_dense_core_core_fsm_inst_do_C_2_tr0 = ~ xor_cse_1;
  wire  nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0;
  assign nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0
      = ~ reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse;
  wire  nl_dense_core_core_fsm_inst_while_while_C_35_tr0;
  assign nl_dense_core_core_fsm_inst_while_while_C_35_tr0 = ~ (weight_index_8_0_sva_1[8]);
  wire  nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_2_for_C_0_tr0;
  assign nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_2_for_C_0_tr0
      = ~ reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse;
  ccs_in_v1 #(.rscid(32'sd4),
  .width(32'sd32)) addr_hi_rsci (
      .dat(addr_hi_rsc_dat),
      .idat(addr_hi_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd5),
  .width(32'sd32)) feature_addr_rsci (
      .dat(feature_addr_rsc_dat),
      .idat(feature_addr_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd6),
  .width(32'sd32)) weight_addr_rsci (
      .dat(weight_addr_rsc_dat),
      .idat(weight_addr_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd7),
  .width(32'sd32)) output_addr_rsci (
      .dat(output_addr_rsc_dat),
      .idat(output_addr_rsci_idat)
    );
  ccs_in_v1 #(.rscid(32'sd8),
  .width(32'sd32)) in_vector_length_rsci (
      .dat(in_vector_length_rsc_dat),
      .idat(in_vector_length_rsci_idat)
    );
  mgc_muladd1 #(.width_a(32'sd16),
  .signd_a(32'sd1),
  .width_b(32'sd16),
  .signd_b(32'sd1),
  .width_c(32'sd32),
  .signd_c(32'sd1),
  .width_cst(32'sd1),
  .signd_cst(32'sd0),
  .width_d(32'sd0),
  .signd_d(32'sd1),
  .width_z(32'sd32),
  .add_axb(32'sd1),
  .add_c(32'sd1),
  .add_d(32'sd1),
  .use_keep_d(32'sd1),
  .use_const(32'sd1)) while_while_for_1_while_while_for_acc_3_cmp (
      .a(nl_while_while_for_1_while_while_for_acc_3_cmp_a[15:0]),
      .b(nl_while_while_for_1_while_while_for_acc_3_cmp_b[15:0]),
      .c(nl_while_while_for_1_while_while_for_acc_3_cmp_c[31:0]),
      .cst(1'b0),
      .z(while_while_for_1_while_while_for_acc_3_cmp_z),
      .d(2'b0)
    );
  mgc_shift_l_v5 #(.width_a(32'sd512),
  .signd_a(32'sd0),
  .width_s(32'sd9),
  .width_z(32'sd512)) memory_axi_write_base_axi_u512_512_1_lshift_rg (
      .a(nl_memory_axi_write_base_axi_u512_512_1_lshift_rg_a[511:0]),
      .s(nl_memory_axi_write_base_axi_u512_512_1_lshift_rg_s[8:0]),
      .z(memory_axi_write_base_axi_u512_512_1_lshift_itm)
    );
  mgc_shift_l_v5 #(.width_a(32'sd64),
  .signd_a(32'sd0),
  .width_s(32'sd6),
  .width_z(32'sd64)) memory_encode_strb_1_if_6_lshift_rg (
      .a(64'b1111111111111111111111111111111111111111111111111111111111111111),
      .s(outputs_slc_output_addr_30_0_psp_5_0_sva),
      .z(memory_encode_strb_1_if_6_lshift_itm_1)
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
      .core_wen(core_wen),
      .done_rsci_oswt(reg_done_rsci_oswt_tmp),
      .done_rsci_wen_comp(done_rsci_wen_comp),
      .done_rsci_irdy(done_rsci_irdy),
      .done_rsci_irdy_oreg(done_rsci_irdy_oreg),
      .done_rsci_wen_comp_pff(done_rsci_wen_comp_iff),
      .done_rsci_oswt_pff(mux_152_rmff),
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
      .memory_channels_aw_channel_rsci_oswt_pff(memory_send_aw_1_mux_rmff),
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
      .memory_channels_w_channel_rsci_oswt_pff(memory_send_w_1_mux_rmff),
      .memory_channels_w_channel_rsci_irdy_oreg_pff(memory_channels_w_channel_rsci_irdy)
    );
  dense_core_memory_channels_b_channel_rsci dense_core_memory_channels_b_channel_rsci_inst
      (
      .clk(clk),
      .arst_n(arst_n),
      .memory_channels_b_channel_rsc_dat(memory_channels_b_channel_rsc_dat),
      .memory_channels_b_channel_rsc_vld(memory_channels_b_channel_rsc_vld),
      .memory_channels_b_channel_rsc_rdy(memory_channels_b_channel_rsc_rdy),
      .core_wen(core_wen),
      .memory_channels_b_channel_rsci_oswt(reg_done_rsci_oswt_tmp),
      .memory_channels_b_channel_rsci_wen_comp(memory_channels_b_channel_rsci_wen_comp),
      .memory_channels_b_channel_rsci_ivld(memory_channels_b_channel_rsci_ivld),
      .memory_channels_b_channel_rsci_ivld_oreg(memory_channels_b_channel_rsci_ivld_oreg),
      .memory_channels_b_channel_rsci_wen_comp_pff(memory_channels_b_channel_rsci_wen_comp_iff),
      .memory_channels_b_channel_rsci_oswt_pff(mux_152_rmff),
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
      .use_relu_triosy_obj_iswt0(reg_memory_channels_aw_channel_rsci_oswt_tmp)
    );
  dense_core_addr_hi_triosy_obj dense_core_addr_hi_triosy_obj_inst (
      .addr_hi_triosy_lz(addr_hi_triosy_lz),
      .core_wten(core_wten),
      .addr_hi_triosy_obj_iswt0(reg_memory_channels_aw_channel_rsci_oswt_tmp)
    );
  dense_core_feature_addr_triosy_obj dense_core_feature_addr_triosy_obj_inst (
      .feature_addr_triosy_lz(feature_addr_triosy_lz),
      .core_wten(core_wten),
      .feature_addr_triosy_obj_iswt0(reg_memory_channels_aw_channel_rsci_oswt_tmp)
    );
  dense_core_weight_addr_triosy_obj dense_core_weight_addr_triosy_obj_inst (
      .weight_addr_triosy_lz(weight_addr_triosy_lz),
      .core_wten(core_wten),
      .weight_addr_triosy_obj_iswt0(reg_memory_channels_aw_channel_rsci_oswt_tmp)
    );
  dense_core_output_addr_triosy_obj dense_core_output_addr_triosy_obj_inst (
      .output_addr_triosy_lz(output_addr_triosy_lz),
      .core_wten(core_wten),
      .output_addr_triosy_obj_iswt0(reg_memory_channels_aw_channel_rsci_oswt_tmp)
    );
  dense_core_in_vector_length_triosy_obj dense_core_in_vector_length_triosy_obj_inst
      (
      .in_vector_length_triosy_lz(in_vector_length_triosy_lz),
      .core_wten(core_wten),
      .in_vector_length_triosy_obj_iswt0(reg_memory_channels_aw_channel_rsci_oswt_tmp)
    );
  dense_core_out_vector_length_triosy_obj dense_core_out_vector_length_triosy_obj_inst
      (
      .out_vector_length_triosy_lz(out_vector_length_triosy_lz),
      .core_wten(core_wten),
      .out_vector_length_triosy_obj_iswt0(reg_memory_channels_aw_channel_rsci_oswt_tmp)
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
      .memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0(nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_for_C_0_tr0),
      .do_C_2_tr0(nl_dense_core_core_fsm_inst_do_C_2_tr0),
      .memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0(nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_1_for_C_0_tr0),
      .sum_array_vinit_C_1_tr0(for_for_nor_itm),
      .main_C_3_tr0(exit_while_while_sva_mx0),
      .while_while_C_35_tr0(nl_dense_core_core_fsm_inst_while_while_C_35_tr0),
      .memory_axi_burst_read_base_axi_u512_512_2_for_C_0_tr0(nl_dense_core_core_fsm_inst_memory_axi_burst_read_base_axi_u512_512_2_for_C_0_tr0),
      .while_while_C_36_tr0(exit_while_while_sva_mx0)
    );
  assign feature_buffer_rsci_clken_d = core_wen;
  assign or_170_nl = (fsm_output[2]) | (fsm_output[4]) | (fsm_output[6]);
  assign nand_16_nl = ~((fsm_output[2]) & (fsm_output[4]) & (fsm_output[6]));
  assign mux_30_nl = MUX_s_1_2_2(or_170_nl, nand_16_nl, fsm_output[0]);
  assign while_nor_nl = ~(mux_30_nl | or_dcpl_8 | (fsm_output[1]));
  assign mux_rmff = MUX_s_1_2_2(reg_start_rsci_oswt_tmp, while_nor_nl, core_wen);
  assign and_26_nl = and_dcpl_25 & and_dcpl_22;
  assign mux_152_rmff = MUX_s_1_2_2(reg_done_rsci_oswt_tmp, and_26_nl, core_wen);
  assign and_30_nl = (~ mux_tmp_31) & (~((fsm_output[6]) | (fsm_output[2]))) & (fsm_output[0])
      & exit_while_while_sva_mx0;
  assign memory_send_aw_1_mux_rmff = MUX_s_1_2_2(reg_memory_channels_aw_channel_rsci_oswt_tmp,
      and_30_nl, core_wen);
  assign and_33_nl = and_dcpl_25 & and_dcpl_32;
  assign memory_send_w_1_mux_rmff = MUX_s_1_2_2(reg_memory_channels_w_channel_rsci_oswt_tmp,
      and_33_nl, core_wen);
  assign mux_34_nl = MUX_s_1_2_2(or_tmp_17, or_tmp_14, fsm_output[0]);
  assign or_3_nl = (~ (fsm_output[1])) | (fsm_output[3]);
  assign or_26_nl = (fsm_output[1]) | (~ (fsm_output[3]));
  assign mux_32_nl = MUX_s_1_2_2(or_3_nl, or_26_nl, fsm_output[2]);
  assign mux_33_nl = MUX_s_1_2_2(mux_32_nl, or_tmp_14, fsm_output[0]);
  assign mux_35_nl = MUX_s_1_2_2(mux_34_nl, mux_33_nl, z_out_2[8]);
  assign and_36_nl = (~ mux_35_nl) & and_dcpl_3 & (~ (fsm_output[4]));
  assign memory_send_ar_mux_rmff = MUX_s_1_2_2(reg_memory_channels_ar_channel_rsci_oswt_tmp,
      and_36_nl, core_wen);
  assign nand_2_cse = ~((fsm_output[5:4]==2'b11));
  assign or_160_cse = (~ (fsm_output[1])) | (fsm_output[4]) | (fsm_output[5]);
  assign or_157_nl = (fsm_output[2]) | (~ (fsm_output[1])) | (fsm_output[4]) | (fsm_output[5]);
  assign mux_38_nl = MUX_s_1_2_2(or_156_cse, or_157_nl, fsm_output[0]);
  assign or_158_nl = (fsm_output[1]) | nand_2_cse;
  assign mux_36_nl = MUX_s_1_2_2(or_158_nl, or_159_cse, fsm_output[2]);
  assign mux_37_nl = MUX_s_1_2_2(mux_36_nl, or_160_cse, fsm_output[0]);
  assign mux_39_nl = MUX_s_1_2_2(mux_38_nl, mux_37_nl, and_1_cse);
  assign or_162_nl = (~((fsm_output[0]) & (fsm_output[2]) & (fsm_output[1]) & (~
      (fsm_output[4])))) | (~((weight_index_8_0_sva_1[8]) & (fsm_output[5])));
  assign mux_40_nl = MUX_s_1_2_2(mux_39_nl, or_162_nl, fsm_output[3]);
  assign asn_memory_channels_r_channel_rsci_oswt_nor_nl = ~(mux_40_nl | (fsm_output[6]));
  assign memory_get_r_mux_rmff = MUX_s_1_2_2(reg_memory_channels_r_channel_rsci_oswt_tmp,
      asn_memory_channels_r_channel_rsci_oswt_nor_nl, core_wen);
  assign or_67_cse = (fsm_output[3]) | (fsm_output[1]);
  assign or_68_cse = (fsm_output[6:5]!=2'b10);
  assign memory_send_ar_and_cse = core_wen & (and_dcpl_172 | and_dcpl_175 | and_dcpl_178);
  assign nl_operator_64_false_1_acc_sdt = conv_s2u_18_50(weight_page_17_0_sva_1_mx0w1)
      + ({weights_acc_psp_sva , (weights_io_read_weight_addr_rsc_cse_sva[31:14])});
  assign operator_64_false_1_acc_sdt = nl_operator_64_false_1_acc_sdt[49:0];
  assign memory_send_w_1_and_cse = core_wen & (~(or_68_cse | or_dcpl_22 | or_dcpl_20
      | (~ (fsm_output[0]))));
  assign outputs_and_cse = core_wen & (~(or_dcpl_13 | or_dcpl_27 | (fsm_output[2:0]!=3'b001)));
  assign and_242_cse = (fsm_output[1]) & (fsm_output[3]);
  assign mux_118_nl = MUX_s_1_2_2(mux_tmp_2, nor_tmp_1, and_242_cse);
  assign mux_119_nl = MUX_s_1_2_2(mux_23_cse, mux_118_nl, fsm_output[2]);
  assign or_127_nl = (~ (z_out_2[8])) | (fsm_output[0]);
  assign mux_120_nl = MUX_s_1_2_2(mux_119_nl, mux_24_cse, or_127_nl);
  assign nor_81_ssc = ~(mux_120_nl | (fsm_output[6]));
  assign mux_3_cse = MUX_s_1_2_2(mux_tmp_2, nor_tmp_1, fsm_output[3]);
  assign mux_23_cse = MUX_s_1_2_2(not_tmp_11, mux_tmp_2, or_67_cse);
  assign mux_24_cse = MUX_s_1_2_2(mux_23_cse, mux_3_cse, fsm_output[2]);
  assign mux_97_nl = MUX_s_1_2_2(or_tmp_67, mux_tmp_96, fsm_output[1]);
  assign mux_98_nl = MUX_s_1_2_2(mux_tmp_93, mux_97_nl, fsm_output[2]);
  assign mux_91_nl = MUX_s_1_2_2(mux_tmp_90, mux_tmp_89, fsm_output[1]);
  assign mux_95_nl = MUX_s_1_2_2(mux_tmp_94, mux_91_nl, fsm_output[2]);
  assign mux_99_nl = MUX_s_1_2_2(mux_98_nl, mux_95_nl, fsm_output[0]);
  assign i_and_ssc = core_wen & (and_dcpl_179 | i_1_31_13_sva_mx0c1 | i_1_31_13_sva_mx0c2
      | (~ mux_99_nl));
  assign and_1_cse = (~((memory_channels_r_channel_rsci_idat_mxwt[1:0]!=2'b00) |
      (z_out_2[8]))) & reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse;
  assign nor_31_cse = ~((fsm_output[2:1]!=2'b00));
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_and_ssc = core_wen &
      (reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse | memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c1
      | memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c2 | and_dcpl_201);
  assign or_119_cse = (fsm_output[1]) | (fsm_output[4]) | (fsm_output[5]);
  assign xnor_2_tmp = ~((fsm_output[1]) ^ (fsm_output[0]));
  assign while_while_for_and_cse = core_wen & (~(or_dcpl_38 | or_dcpl_41 | (~ (fsm_output[0]))));
  assign and_243_cse = (fsm_output[4:3]==2'b11);
  assign or_156_cse = (~ (fsm_output[2])) | (~ (fsm_output[1])) | (fsm_output[4])
      | (fsm_output[5]);
  assign or_159_cse = (fsm_output[5:4]!=2'b00);
  assign nl_i_1_31_13_sva_2 = ({i_1_31_13_sva_18 , i_1_31_13_sva_17_16 , i_1_31_13_sva_15_0})
      + 19'b0000000000000000001;
  assign i_1_31_13_sva_2 = nl_i_1_31_13_sva_2[18:0];
  assign exit_while_while_sva_mx0 = MUX_s_1_2_2((~ ($signed(27'b000000000000000000000000000)
      < $signed(operator_32_true_operator_32_true_acc_psp_sva))), (~ ($signed(input_index_sva)
      < $signed(conv_s2s_27_32(operator_32_true_operator_32_true_acc_psp_sva)))),
      and_dcpl_201);
  assign nl_weight_page_17_0_sva_1_mx0w1 = ({i_1_31_13_sva_17_16 , i_1_31_13_sva_15_0})
      + 18'b000000000000000001;
  assign weight_page_17_0_sva_1_mx0w1 = nl_weight_page_17_0_sva_1_mx0w1[17:0];
  assign nl_while_for_acc_8_itm_mx0w1 = while_for_acc_14_itm + while_for_acc_10_itm
      + i_1_31_13_sva_15_0;
  assign while_for_acc_8_itm_mx0w1 = nl_while_for_acc_8_itm_mx0w1[15:0];
  assign and_dcpl_3 = ~((fsm_output[6:5]!=2'b00));
  assign nor_27_cse = ~((fsm_output[4:3]!=2'b00));
  assign nor_tmp_1 = (fsm_output[5:4]==2'b11);
  assign mux_tmp_2 = MUX_s_1_2_2((~ (fsm_output[5])), (fsm_output[5]), fsm_output[4]);
  assign not_tmp_11 = ~((fsm_output[5:4]!=2'b00));
  assign or_dcpl_8 = (fsm_output[5]) | (fsm_output[3]);
  assign and_dcpl_21 = (fsm_output[2:1]==2'b10);
  assign and_dcpl_22 = and_dcpl_21 & (~ (fsm_output[0]));
  assign and_dcpl_23 = (fsm_output[4:3]==2'b10);
  assign and_dcpl_24 = (fsm_output[6:5]==2'b10);
  assign and_dcpl_25 = and_dcpl_24 & and_dcpl_23;
  assign or_24_nl = (fsm_output[3]) | (~ nor_tmp_1);
  assign or_23_nl = (fsm_output[5:3]!=3'b001);
  assign mux_tmp_31 = MUX_s_1_2_2(or_24_nl, or_23_nl, fsm_output[1]);
  assign and_dcpl_31 = (fsm_output[2:1]==2'b01);
  assign and_dcpl_32 = and_dcpl_31 & (fsm_output[0]);
  assign or_tmp_14 = xor_cse_1 | (fsm_output[3:1]!=3'b010);
  assign or_tmp_17 = (fsm_output[3:1]!=3'b001);
  assign and_dcpl_38 = (fsm_output[4:3]==2'b01);
  assign and_dcpl_39 = and_dcpl_3 & and_dcpl_38;
  assign and_dcpl_40 = and_dcpl_39 & and_dcpl_22;
  assign and_dcpl_44 = and_dcpl_3 & nor_27_cse;
  assign and_dcpl_46 = (fsm_output[2:1]==2'b11);
  assign and_dcpl_47 = and_dcpl_46 & (fsm_output[0]);
  assign and_dcpl_48 = (fsm_output[6:5]==2'b01);
  assign and_dcpl_49 = and_dcpl_48 & and_dcpl_38;
  assign and_dcpl_51 = ~((fsm_output[6]) | (fsm_output[3]));
  assign and_dcpl_59 = not_tmp_11 & (fsm_output[3]);
  assign nand_14_nl = ~((fsm_output[1]) & (fsm_output[5]));
  assign or_169_nl = (fsm_output[1]) | (fsm_output[5]);
  assign mux_44_nl = MUX_s_1_2_2(nand_14_nl, or_169_nl, fsm_output[0]);
  assign and_dcpl_74 = ~(mux_44_nl | (fsm_output[6]));
  assign and_dcpl_76 = ~(((fsm_output[5]) ^ (fsm_output[0])) | (fsm_output[6]));
  assign and_dcpl_79 = (~ (fsm_output[6])) & (fsm_output[4]);
  assign nor_44_nl = ~((~ (fsm_output[2])) | (fsm_output[1]) | (~ (fsm_output[5])));
  assign nor_45_nl = ~((fsm_output[2]) | (~ (fsm_output[1])) | (fsm_output[5]));
  assign not_tmp_48 = MUX_s_1_2_2(nor_44_nl, nor_45_nl, fsm_output[0]);
  assign nor_tmp_13 = (fsm_output[3]) & (fsm_output[5]);
  assign and_dcpl_105 = ~((fsm_output[3]) | (fsm_output[1]));
  assign or_tmp_44 = (fsm_output[6:5]!=2'b01);
  assign mux_48_nl = MUX_s_1_2_2(or_tmp_44, or_68_cse, fsm_output[0]);
  assign and_dcpl_107 = ~(mux_48_nl | (fsm_output[4]));
  assign nor_48_nl = ~((~ (fsm_output[1])) | (fsm_output[5]) | (~ (fsm_output[6])));
  assign nor_49_nl = ~((fsm_output[1]) | (~ (fsm_output[5])) | (fsm_output[6]));
  assign not_tmp_53 = MUX_s_1_2_2(nor_48_nl, nor_49_nl, fsm_output[0]);
  assign and_dcpl_111 = (~ (fsm_output[3])) & (fsm_output[1]);
  assign mux_tmp_52 = MUX_s_1_2_2((~ (fsm_output[5])), (fsm_output[5]), fsm_output[6]);
  assign mux_tmp_60 = MUX_s_1_2_2((~ (fsm_output[6])), (fsm_output[6]), or_159_cse);
  assign and_dcpl_129 = nor_31_cse & (fsm_output[0]);
  assign and_dcpl_131 = and_dcpl_39 & and_dcpl_47;
  assign and_dcpl_132 = nor_31_cse & (~ (fsm_output[0]));
  assign and_dcpl_133 = and_dcpl_3 & and_dcpl_23;
  assign and_dcpl_134 = and_dcpl_133 & and_dcpl_132;
  assign and_dcpl_135 = and_dcpl_133 & and_dcpl_129;
  assign and_dcpl_136 = and_dcpl_31 & (~ (fsm_output[0]));
  assign and_dcpl_137 = and_dcpl_133 & and_dcpl_136;
  assign and_dcpl_138 = and_dcpl_133 & and_dcpl_32;
  assign and_dcpl_139 = and_dcpl_133 & and_dcpl_22;
  assign and_dcpl_140 = and_dcpl_21 & (fsm_output[0]);
  assign and_dcpl_141 = and_dcpl_133 & and_dcpl_140;
  assign and_dcpl_142 = and_dcpl_46 & (~ (fsm_output[0]));
  assign and_dcpl_143 = and_dcpl_133 & and_dcpl_142;
  assign and_dcpl_144 = and_dcpl_133 & and_dcpl_47;
  assign and_dcpl_145 = and_dcpl_3 & and_243_cse;
  assign and_dcpl_146 = and_dcpl_145 & and_dcpl_132;
  assign and_dcpl_147 = and_dcpl_145 & and_dcpl_129;
  assign and_dcpl_148 = and_dcpl_145 & and_dcpl_136;
  assign and_dcpl_149 = and_dcpl_145 & and_dcpl_32;
  assign and_dcpl_150 = and_dcpl_145 & and_dcpl_22;
  assign and_dcpl_151 = and_dcpl_145 & and_dcpl_140;
  assign and_dcpl_152 = and_dcpl_145 & and_dcpl_142;
  assign and_dcpl_153 = and_dcpl_145 & and_dcpl_47;
  assign and_dcpl_154 = and_dcpl_48 & nor_27_cse;
  assign and_dcpl_155 = and_dcpl_154 & and_dcpl_132;
  assign and_dcpl_156 = and_dcpl_154 & and_dcpl_129;
  assign and_dcpl_157 = and_dcpl_154 & and_dcpl_136;
  assign and_dcpl_158 = and_dcpl_154 & and_dcpl_32;
  assign and_dcpl_159 = and_dcpl_154 & and_dcpl_22;
  assign and_dcpl_160 = and_dcpl_154 & and_dcpl_140;
  assign and_dcpl_161 = and_dcpl_154 & and_dcpl_142;
  assign and_dcpl_162 = and_dcpl_154 & and_dcpl_47;
  assign and_dcpl_163 = and_dcpl_49 & and_dcpl_132;
  assign and_dcpl_164 = and_dcpl_49 & and_dcpl_129;
  assign and_dcpl_165 = and_dcpl_49 & and_dcpl_136;
  assign and_dcpl_166 = and_dcpl_49 & and_dcpl_32;
  assign and_dcpl_167 = and_dcpl_49 & and_dcpl_22;
  assign and_dcpl_168 = and_dcpl_49 & and_dcpl_140;
  assign nor_17_nl = ~((fsm_output[1]) | (~ (fsm_output[3])));
  assign mux_68_nl = MUX_s_1_2_2(mux_tmp_2, nor_tmp_1, nor_17_nl);
  assign or_71_nl = and_242_cse | (fsm_output[4]);
  assign mux_66_nl = MUX_s_1_2_2((~ (fsm_output[5])), (fsm_output[5]), or_71_nl);
  assign mux_tmp_69 = MUX_s_1_2_2(mux_68_nl, mux_66_nl, fsm_output[2]);
  assign or_tmp_60 = (fsm_output[5:4]!=2'b01);
  assign and_231_cse = (fsm_output[3:1]==3'b111);
  assign mux_tmp_70 = MUX_s_1_2_2(mux_tmp_2, or_tmp_60, and_231_cse);
  assign and_dcpl_170 = and_dcpl_39 & and_dcpl_142;
  assign or_74_nl = and_231_cse | (fsm_output[4]);
  assign mux_tmp_73 = MUX_s_1_2_2((~ (fsm_output[5])), (fsm_output[5]), or_74_nl);
  assign and_dcpl_172 = and_dcpl_44 & and_dcpl_136;
  assign and_dcpl_175 = and_dcpl_44 & and_dcpl_21 & (~ xor_cse_1) & (fsm_output[0]);
  assign and_dcpl_178 = and_dcpl_39 & and_dcpl_21 & (~ (fsm_output[0])) & (z_out_2[8]);
  assign or_dcpl_13 = (fsm_output[6:5]!=2'b00);
  assign or_dcpl_20 = (fsm_output[2:1]!=2'b01);
  assign or_dcpl_22 = (fsm_output[4:3]!=2'b10);
  assign or_dcpl_27 = (fsm_output[4:3]!=2'b00);
  assign and_dcpl_179 = and_dcpl_44 & and_dcpl_129;
  assign mux_tmp_89 = MUX_s_1_2_2(or_68_cse, or_tmp_44, fsm_output[4]);
  assign or_tmp_67 = (fsm_output[6:4]!=3'b100);
  assign or_tmp_68 = (fsm_output[6:4]!=3'b011);
  assign mux_tmp_90 = MUX_s_1_2_2(or_tmp_68, or_tmp_67, fsm_output[3]);
  assign mux_tmp_93 = MUX_s_1_2_2(or_68_cse, or_tmp_44, and_243_cse);
  assign mux_92_nl = MUX_s_1_2_2(or_tmp_67, mux_tmp_89, fsm_output[3]);
  assign mux_tmp_94 = MUX_s_1_2_2(mux_tmp_93, mux_92_nl, fsm_output[1]);
  assign mux_tmp_96 = MUX_s_1_2_2(or_tmp_68, mux_tmp_89, fsm_output[3]);
  assign and_dcpl_190 = (~((fsm_output[6]) | (fsm_output[4]))) & (fsm_output[1]);
  assign not_tmp_71 = ~((~ (fsm_output[2])) | (fsm_output[3]) | (fsm_output[5]));
  assign and_dcpl_201 = and_dcpl_48 & and_dcpl_23 & and_dcpl_129;
  assign or_dcpl_38 = or_dcpl_13 | (fsm_output[4:3]!=2'b01);
  assign or_dcpl_41 = (fsm_output[2:1]!=2'b10);
  assign or_dcpl_47 = or_tmp_44 | or_dcpl_22;
  assign or_tmp_87 = ~((fsm_output[6:3]==4'b0111));
  assign or_tmp_89 = (fsm_output[6:3]!=4'b1000);
  assign mux_tmp_124 = MUX_s_1_2_2(or_tmp_87, or_dcpl_47, fsm_output[1]);
  assign or_140_nl = (fsm_output[6:4]!=3'b101);
  assign mux_tmp_125 = MUX_s_1_2_2(or_140_nl, or_tmp_67, fsm_output[3]);
  assign mux_tmp_130 = MUX_s_1_2_2(or_dcpl_47, or_tmp_89, fsm_output[1]);
  assign or_tmp_94 = (fsm_output[1]) | (fsm_output[3]) | (fsm_output[4]) | (fsm_output[5])
      | (~ (fsm_output[6]));
  assign or_tmp_96 = (fsm_output[6:3]!=4'b1001);
  assign and_dcpl_206 = and_dcpl_24 & and_dcpl_38;
  assign and_dcpl_207 = and_dcpl_206 & and_dcpl_142;
  assign and_dcpl_208 = and_dcpl_48 & and_243_cse;
  assign mux_76_nl = MUX_s_1_2_2(mux_tmp_2, or_tmp_60, fsm_output[3]);
  assign mux_75_nl = MUX_s_1_2_2((~ (fsm_output[5])), (fsm_output[5]), or_dcpl_27);
  assign mux_77_nl = MUX_s_1_2_2(mux_76_nl, mux_75_nl, fsm_output[1]);
  assign mux_78_nl = MUX_s_1_2_2(mux_tmp_2, mux_77_nl, fsm_output[2]);
  assign mux_79_nl = MUX_s_1_2_2(mux_tmp_73, mux_78_nl, fsm_output[0]);
  assign in_vector_length_sva_mx0c1 = ~(mux_79_nl | (fsm_output[6]));
  assign i_1_31_13_sva_mx0c1 = and_dcpl_44 & and_dcpl_140;
  assign mux_84_nl = MUX_s_1_2_2(not_tmp_11, mux_tmp_2, fsm_output[3]);
  assign or_95_nl = (fsm_output[2:1]!=2'b00);
  assign mux_85_nl = MUX_s_1_2_2(mux_84_nl, mux_3_cse, or_95_nl);
  assign mux_88_nl = MUX_s_1_2_2(mux_24_cse, mux_85_nl, fsm_output[0]);
  assign i_1_31_13_sva_mx0c2 = ~(mux_88_nl | (fsm_output[6]));
  assign features_io_read_feature_addr_rsc_cse_sva_mx0c1 = and_dcpl_39 & and_dcpl_140;
  assign features_io_read_feature_addr_rsc_cse_sva_mx0c2 = ~((~(((and_242_cse & (fsm_output[2]))
      | (fsm_output[4])) ^ (fsm_output[5]))) | (fsm_output[6]));
  assign input_index_sva_mx0c1 = and_dcpl_39 & and_dcpl_32;
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c1 = and_dcpl_3
      & (~((fsm_output[4]) | (fsm_output[2]))) & (~((fsm_output[1:0]==2'b11))) &
      (fsm_output[3]);
  assign mux_116_nl = MUX_s_1_2_2((~ (fsm_output[5])), nor_tmp_13, fsm_output[2]);
  assign mux_117_nl = MUX_s_1_2_2(not_tmp_71, mux_116_nl, fsm_output[0]);
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c2 = mux_117_nl
      & and_dcpl_190;
  assign mux_133_nl = MUX_s_1_2_2(or_tmp_87, or_tmp_96, fsm_output[1]);
  assign mux_134_nl = MUX_s_1_2_2(mux_133_nl, or_tmp_94, fsm_output[0]);
  assign while_for_acc_10_itm_mx0c1 = ~(mux_134_nl | (fsm_output[2]));
  assign while_for_acc_13_itm_mx0c1 = and_dcpl_208 & and_dcpl_22;
  assign while_for_acc_17_itm_mx0c0 = and_dcpl_24 & nor_27_cse & and_dcpl_140;
  assign while_for_acc_17_itm_mx0c2 = and_dcpl_206 & and_dcpl_47;
  assign while_for_acc_17_itm_mx0c3 = and_dcpl_25 & and_dcpl_136;
  assign xor_cse_1 = $signed(({i_1_31_13_sva_2 , 13'b0000000000000})) < $signed(in_vector_length_sva);
  assign xor_dcpl_1 = ~((fsm_output[6]) ^ (fsm_output[0]));
  assign feature_buffer_rsci_d_d_pff = memory_channels_r_channel_rsci_idat_mxwt[513:2];
  assign feature_buffer_rsci_radr_d = input_index_sva[14:0];
  assign feature_buffer_rsci_re_d_pff = and_dcpl_40;
  assign feature_buffer_rsci_wadr_d = {7'b0000000 , memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_5
      , memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0};
  assign feature_buffer_rsci_we_d_pff = and_dcpl_44 & and_dcpl_21 & (~ (fsm_output[0]))
      & reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse;
  assign weight_buffer_rsci_radr_d = {memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_5
      , memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0};
  assign weight_buffer_rsci_wadr_d = {memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_5
      , memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0};
  assign or_39_nl = (fsm_output[2:1]!=2'b00) | (~ nor_tmp_1);
  assign mux_41_nl = MUX_s_1_2_2(or_39_nl, or_156_cse, fsm_output[0]);
  assign weight_buffer_rsci_we_d_pff = (~ mux_41_nl) & and_dcpl_51 & reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse;
  assign nor_55_nl = ~((~(((and_242_cse & (fsm_output[2]) & (fsm_output[0])) | (fsm_output[4]))
      ^ (fsm_output[5]))) | (fsm_output[6]));
  assign sum_array_rsci_d_d = MUX_v_32_2_2(32'b00000000000000000000000000000000,
      features_io_read_feature_addr_rsc_cse_sva, nor_55_nl);
  assign nand_18_nl = ~((fsm_output[1]) & (fsm_output[6]));
  assign or_171_nl = (fsm_output[1]) | (fsm_output[6]);
  assign mux_42_nl = MUX_s_1_2_2(nand_18_nl, or_171_nl, fsm_output[0]);
  assign and_64_nl = (~(mux_42_nl | (fsm_output[5]))) & and_dcpl_38 & (fsm_output[2]);
  assign and_66_nl = and_dcpl_59 & and_dcpl_46 & xor_dcpl_1;
  assign or_172_nl = (fsm_output[2]) | (fsm_output[1]) | (fsm_output[3]) | (~((fsm_output[4])
      & (fsm_output[6])));
  assign or_173_nl = (~ (fsm_output[2])) | (~ (fsm_output[1])) | (~ (fsm_output[3]))
      | (fsm_output[4]) | (fsm_output[6]);
  assign mux_43_nl = MUX_s_1_2_2(or_172_nl, or_173_nl, fsm_output[0]);
  assign nor_60_nl = ~(mux_43_nl | (fsm_output[5]));
  assign and_72_nl = (fsm_output[5:3]==3'b010) & nor_31_cse & xor_dcpl_1;
  assign and_75_nl = and_dcpl_74 & and_dcpl_23 & (~ (fsm_output[2]));
  assign and_78_nl = and_dcpl_23 & and_dcpl_31 & and_dcpl_76;
  assign and_81_nl = not_tmp_48 & and_dcpl_79 & (~ (fsm_output[3]));
  assign and_83_nl = and_dcpl_23 & and_dcpl_21 & and_dcpl_76;
  assign and_85_nl = and_dcpl_74 & and_dcpl_23 & (fsm_output[2]);
  assign and_87_nl = and_dcpl_23 & and_dcpl_46 & and_dcpl_76;
  assign nor_61_nl = ~((fsm_output[2:1]!=2'b00) | (~ nor_tmp_13));
  assign nor_62_nl = ~((~ (fsm_output[2])) | (~ (fsm_output[1])) | (fsm_output[3])
      | (fsm_output[5]));
  assign mux_46_nl = MUX_s_1_2_2(nor_61_nl, nor_62_nl, fsm_output[0]);
  assign and_88_nl = mux_46_nl & and_dcpl_79;
  assign and_91_nl = and_243_cse & nor_31_cse & and_dcpl_76;
  assign and_93_nl = and_dcpl_74 & and_243_cse & (~ (fsm_output[2]));
  assign and_95_nl = and_243_cse & and_dcpl_31 & and_dcpl_76;
  assign and_97_nl = not_tmp_48 & and_dcpl_79 & (fsm_output[3]);
  assign and_99_nl = and_243_cse & and_dcpl_21 & and_dcpl_76;
  assign and_101_nl = and_dcpl_74 & and_243_cse & (fsm_output[2]);
  assign and_103_nl = and_243_cse & and_dcpl_46 & and_dcpl_76;
  assign or_174_nl = (fsm_output[2]) | (fsm_output[1]) | (fsm_output[3]) | (fsm_output[4])
      | (~ (fsm_output[6]));
  assign nand_21_nl = ~((fsm_output[2]) & (fsm_output[1]) & (fsm_output[3]) & (fsm_output[4])
      & (~ (fsm_output[6])));
  assign mux_47_nl = MUX_s_1_2_2(or_174_nl, nand_21_nl, fsm_output[0]);
  assign nor_64_nl = ~(mux_47_nl | (fsm_output[5]));
  assign and_108_nl = and_dcpl_107 & and_dcpl_105 & (~ (fsm_output[2]));
  assign and_110_nl = not_tmp_53 & nor_27_cse & (~ (fsm_output[2]));
  assign and_113_nl = and_dcpl_107 & and_dcpl_111 & (~ (fsm_output[2]));
  assign nor_65_nl = ~((~ (fsm_output[2])) | (fsm_output[1]) | (fsm_output[5]) |
      (~ (fsm_output[6])));
  assign nor_66_nl = ~((fsm_output[2]) | (~ (fsm_output[1])) | (~ (fsm_output[5]))
      | (fsm_output[6]));
  assign mux_50_nl = MUX_s_1_2_2(nor_65_nl, nor_66_nl, fsm_output[0]);
  assign and_114_nl = mux_50_nl & nor_27_cse;
  assign and_116_nl = and_dcpl_107 & and_dcpl_105 & (fsm_output[2]);
  assign and_118_nl = not_tmp_53 & nor_27_cse & (fsm_output[2]);
  assign and_120_nl = and_dcpl_107 & and_dcpl_111 & (fsm_output[2]);
  assign or_175_nl = (fsm_output[2]) | (fsm_output[1]) | (~ (fsm_output[3])) | (fsm_output[5])
      | (~ (fsm_output[6]));
  assign or_176_nl = (~ (fsm_output[2])) | (~ (fsm_output[1])) | (fsm_output[3])
      | (~ (fsm_output[5])) | (fsm_output[6]);
  assign mux_51_nl = MUX_s_1_2_2(or_175_nl, or_176_nl, fsm_output[0]);
  assign nor_69_nl = ~(mux_51_nl | (fsm_output[4]));
  assign and_124_nl = and_dcpl_107 & (fsm_output[3:1]==3'b100);
  assign and_126_nl = not_tmp_53 & and_dcpl_38 & (~ (fsm_output[2]));
  assign and_128_nl = and_dcpl_107 & and_242_cse & (~ (fsm_output[2]));
  assign mux1h_nl = MUX1HOT_v_5_30_2(5'b00001, 5'b00010, 5'b00011, 5'b00100, 5'b00101,
      5'b00110, 5'b00111, 5'b01000, 5'b01001, 5'b01010, 5'b01011, 5'b01100, 5'b01101,
      5'b01110, 5'b01111, 5'b10000, 5'b10001, 5'b10010, 5'b10011, 5'b10100, 5'b10101,
      5'b10110, 5'b10111, 5'b11000, 5'b11001, 5'b11010, 5'b11011, 5'b11100, 5'b11101,
      5'b11110, {and_64_nl , and_66_nl , nor_60_nl , and_72_nl , and_75_nl , and_78_nl
      , and_81_nl , and_83_nl , and_85_nl , and_87_nl , and_88_nl , and_91_nl , and_93_nl
      , and_95_nl , and_97_nl , and_99_nl , and_101_nl , and_103_nl , nor_64_nl ,
      and_108_nl , and_110_nl , and_113_nl , and_114_nl , and_116_nl , and_118_nl
      , and_120_nl , nor_69_nl , and_124_nl , and_126_nl , and_128_nl});
  assign and_240_nl = (fsm_output[3]) & (fsm_output[0]) & (fsm_output[1]);
  assign mux_56_nl = MUX_s_1_2_2(mux_tmp_52, or_68_cse, and_240_nl);
  assign mux_55_nl = MUX_s_1_2_2((fsm_output[5]), (fsm_output[6]), or_67_cse);
  assign mux_57_nl = MUX_s_1_2_2(mux_56_nl, mux_55_nl, fsm_output[4]);
  assign or_66_nl = (~((fsm_output[0]) | (fsm_output[1]) | (~ (fsm_output[6]))))
      | (fsm_output[5]);
  assign mux_53_nl = MUX_s_1_2_2(mux_tmp_52, or_66_nl, fsm_output[3]);
  assign mux_54_nl = MUX_s_1_2_2(mux_53_nl, (fsm_output[6]), fsm_output[4]);
  assign mux_58_nl = MUX_s_1_2_2(mux_57_nl, mux_54_nl, fsm_output[2]);
  assign nor_nl = ~(MUX_v_5_2_2(mux1h_nl, 5'b11111, mux_58_nl));
  assign and_61_nl = and_dcpl_59 & and_dcpl_21 & xor_dcpl_1;
  assign sum_array_rsci_radr_d = MUX_v_5_2_2(nor_nl, 5'b11111, and_61_nl);
  assign mux_62_nl = MUX_s_1_2_2((~ (fsm_output[6])), (fsm_output[6]), fsm_output[5]);
  assign mux_63_nl = MUX_s_1_2_2(mux_62_nl, (fsm_output[5]), fsm_output[4]);
  assign mux_64_nl = MUX_s_1_2_2(mux_63_nl, mux_tmp_60, or_67_cse);
  assign mux_59_nl = MUX_s_1_2_2((fsm_output[5]), (fsm_output[6]), fsm_output[4]);
  assign mux_61_nl = MUX_s_1_2_2(mux_tmp_60, mux_59_nl, fsm_output[3]);
  assign mux_65_nl = MUX_s_1_2_2(mux_64_nl, mux_61_nl, fsm_output[2]);
  assign sum_array_rsci_re_d_pff = ~ mux_65_nl;
  assign and_130_nl = and_dcpl_39 & and_dcpl_129;
  assign for_mux1h_nl = MUX1HOT_v_5_31_2(memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0,
      5'b11110, 5'b11101, 5'b11100, 5'b11011, 5'b11010, 5'b11001, 5'b11000, 5'b10111,
      5'b10110, 5'b10101, 5'b10100, 5'b10011, 5'b10010, 5'b10001, 5'b10000, 5'b01111,
      5'b01110, 5'b01101, 5'b01100, 5'b01011, 5'b01010, 5'b01001, 5'b01000, 5'b00111,
      5'b00110, 5'b00101, 5'b00100, 5'b00011, 5'b00010, 5'b00001, {and_130_nl , and_dcpl_134
      , and_dcpl_135 , and_dcpl_137 , and_dcpl_138 , and_dcpl_139 , and_dcpl_141
      , and_dcpl_143 , and_dcpl_144 , and_dcpl_146 , and_dcpl_147 , and_dcpl_148
      , and_dcpl_149 , and_dcpl_150 , and_dcpl_151 , and_dcpl_152 , and_dcpl_153
      , and_dcpl_155 , and_dcpl_156 , and_dcpl_157 , and_dcpl_158 , and_dcpl_159
      , and_dcpl_160 , and_dcpl_161 , and_dcpl_162 , and_dcpl_163 , and_dcpl_164
      , and_dcpl_165 , and_dcpl_166 , and_dcpl_167 , and_dcpl_168});
  assign mux_71_nl = MUX_s_1_2_2(mux_tmp_70, mux_tmp_69, fsm_output[0]);
  assign for_nor_nl = ~(mux_71_nl | (fsm_output[6]));
  assign for_and_nl = MUX_v_5_2_2(5'b00000, for_mux1h_nl, for_nor_nl);
  assign sum_array_rsci_wadr_d = MUX_v_5_2_2(for_and_nl, 5'b11111, and_dcpl_131);
  assign mux_72_nl = MUX_s_1_2_2(mux_tmp_2, mux_tmp_69, fsm_output[0]);
  assign sum_array_rsci_we_d_pff = ~(mux_72_nl | (fsm_output[6]));
  assign mux_tmp = MUX_s_1_2_2((~ (fsm_output[2])), (fsm_output[2]), fsm_output[1]);
  assign and_dcpl_232 = ~((fsm_output[2]) | (fsm_output[6]) | (fsm_output[3]));
  assign and_dcpl_236 = (fsm_output[1:0]==2'b10) & not_tmp_11 & and_dcpl_232;
  assign and_dcpl_239 = (fsm_output[1:0]==2'b01) & not_tmp_11 & and_dcpl_232;
  assign and_dcpl_244 = (fsm_output[1:0]==2'b00) & not_tmp_11 & (fsm_output[2]) &
      (~ (fsm_output[6])) & (fsm_output[3]);
  assign or_192_nl = (fsm_output[1:0]!=2'b00) | nand_2_cse;
  assign mux_158_nl = MUX_s_1_2_2(or_119_cse, or_160_cse, fsm_output[0]);
  assign mux_159_nl = MUX_s_1_2_2(or_192_nl, mux_158_nl, fsm_output[2]);
  assign or_195_nl = (~ (fsm_output[2])) | (fsm_output[0]) | (fsm_output[1]) | (fsm_output[5])
      | (fsm_output[4]);
  assign mux_160_nl = MUX_s_1_2_2(mux_159_nl, or_195_nl, fsm_output[3]);
  assign and_dcpl_251 = ~(mux_160_nl | (fsm_output[6]));
  assign and_dcpl_253 = (fsm_output[2]) & (fsm_output[6]) & (fsm_output[3]);
  assign and_dcpl_254 = ~((fsm_output[5]) | (fsm_output[0]));
  assign and_dcpl_255 = (fsm_output[1]) & (~ (fsm_output[4]));
  assign or_dcpl = (i_1_31_13_sva_mx0c2 & (mux_24_cse | (fsm_output[6]))) | and_dcpl_179;
  assign or_tmp_115 = (and_dcpl_39 & and_dcpl_136) | xnor_2_tmp | (~ memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c1);
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_start_rsci_oswt_tmp <= 1'b0;
      reg_done_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_aw_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_w_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_ar_channel_rsci_oswt_tmp <= 1'b0;
      reg_memory_channels_r_channel_rsci_oswt_tmp <= 1'b0;
      core_wen <= 1'b1;
    end
    else begin
      reg_start_rsci_oswt_tmp <= mux_rmff;
      reg_done_rsci_oswt_tmp <= mux_152_rmff;
      reg_memory_channels_aw_channel_rsci_oswt_tmp <= memory_send_aw_1_mux_rmff;
      reg_memory_channels_w_channel_rsci_oswt_tmp <= memory_send_w_1_mux_rmff;
      reg_memory_channels_ar_channel_rsci_oswt_tmp <= memory_send_ar_mux_rmff;
      reg_memory_channels_r_channel_rsci_oswt_tmp <= memory_get_r_mux_rmff;
      core_wen <= core_wen_rtff;
    end
  end
  always @(posedge clk) begin
    if ( memory_send_ar_and_cse ) begin
      memory_channels_ar_channel_rsci_idat_42_29 <= MUX_v_14_2_2((features_io_read_feature_addr_rsc_cse_sva[13:0]),
          (weights_io_read_weight_addr_rsc_cse_sva[13:0]), or_nl);
      memory_channels_ar_channel_rsci_idat_92_61 <= MUX1HOT_v_32_3_2((z_out_1[42:11]),
          weights_acc_psp_sva, (operator_64_false_1_acc_sdt[49:18]), {and_dcpl_172
          , and_dcpl_175 , and_dcpl_178});
      memory_channels_ar_channel_rsci_idat_60_50 <= MUX1HOT_v_11_3_2((z_out_1[10:0]),
          (weights_io_read_weight_addr_rsc_cse_sva[31:21]), (operator_64_false_1_acc_sdt[17:7]),
          {and_dcpl_172 , and_dcpl_175 , and_dcpl_178});
      memory_channels_ar_channel_rsci_idat_49_43 <= MUX1HOT_v_7_3_2((features_io_read_feature_addr_rsc_cse_sva[20:14]),
          (weights_io_read_weight_addr_rsc_cse_sva[20:14]), (operator_64_false_1_acc_sdt[6:0]),
          {and_dcpl_172 , and_dcpl_175 , and_dcpl_178});
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (~(mux_tmp_31 | (fsm_output[6]) | (fsm_output[2]) | (~((fsm_output[0])
        & exit_while_while_sva_mx0)))) ) begin
      memory_channels_aw_channel_rsci_idat_34_29 <= outputs_slc_output_addr_30_0_psp_5_0_sva;
    end
  end
  always @(posedge clk) begin
    if ( memory_send_w_1_and_cse ) begin
      memory_channels_w_channel_rsci_idat_64_1 <= memory_encode_strb_1_if_6_lshift_itm;
      memory_channels_w_channel_rsci_idat_576_65 <= memory_axi_write_base_axi_u512_512_1_lshift_itm;
    end
  end
  always @(posedge clk) begin
    if ( outputs_and_cse ) begin
      outputs_slc_output_addr_30_0_psp_5_0_sva <= output_addr_rsci_idat[5:0];
      weights_acc_psp_sva <= nl_weights_acc_psp_sva[31:0];
      weights_io_read_weight_addr_rsc_cse_sva <= weight_addr_rsci_idat;
      operator_32_true_operator_32_true_acc_psp_sva <= nl_operator_32_true_operator_32_true_acc_psp_sva[26:0];
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (and_dcpl_179 | in_vector_length_sva_mx0c1) ) begin
      in_vector_length_sva <= MUX_v_32_2_2(in_vector_length_rsci_idat, sum_array_rsci_q_d,
          in_vector_length_sva_mx0c1);
    end
  end
  always @(posedge clk) begin
    if ( i_and_ssc ) begin
      i_1_31_13_sva_18 <= (i_1_31_13_sva_2[18]) & (~ and_dcpl_179);
      i_1_31_13_sva_17_16 <= MUX_v_2_2_2(2'b00, i_i_mux1h_nl, not_364_nl);
    end
  end
  always @(posedge clk) begin
    if ( i_and_ssc & mux_166_nl & (~ nor_81_ssc) ) begin
      i_1_31_13_sva_15_0 <= MUX_v_16_2_2(16'b0000000000000000, i_mux1h_2_nl, not_365_nl);
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (and_dcpl_179 | features_io_read_feature_addr_rsc_cse_sva_mx0c1
        | features_io_read_feature_addr_rsc_cse_sva_mx0c2) ) begin
      features_io_read_feature_addr_rsc_cse_sva <= MUX1HOT_v_32_3_2(feature_addr_rsci_idat,
          sum_array_rsci_q_d, while_while_for_1_while_while_for_acc_3_cmp_z, {and_dcpl_179
          , features_io_read_feature_addr_rsc_cse_sva_mx0c1 , features_io_read_feature_addr_rsc_cse_sva_mx0c2});
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (and_dcpl_179 | input_index_sva_mx0c1 | and_dcpl_40) ) begin
      input_index_sva <= MUX_v_32_2_2(32'b00000000000000000000000000000000, (z_out_1[31:0]),
          input_index_not_1_nl);
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse <= 1'b0;
      for_for_nor_itm <= 1'b0;
    end
    else if ( core_wen ) begin
      reg_memory_axi_burst_read_base_axi_u512_512_1_for_stage_0_cse <= and_1_cse
          | (mux_113_nl & and_dcpl_190);
      for_for_nor_itm <= ~((memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0!=5'b00000));
    end
  end
  always @(posedge clk) begin
    if ( memory_axi_burst_read_base_axi_u512_512_1_for_beat_and_ssc ) begin
      memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_5 <= MUX_v_3_2_2(3'b000,
          memory_axi_burst_read_base_axi_u512_512_1_for_beat_memory_axi_burst_read_base_axi_u512_512_1_for_beat_mux_nl,
          memory_axi_burst_read_base_axi_u512_512_1_for_beat_not_1_nl);
      memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0 <= MUX_v_5_2_2((z_out_2[4:0]),
          memory_axi_burst_read_base_axi_u512_512_1_for_beat_and_2_nl, mux_170_nl);
    end
  end
  always @(posedge clk or negedge arst_n) begin
    if ( ~ arst_n ) begin
      weight_index_8_0_sva_1 <= 9'b000000000;
    end
    else if ( core_wen & (~(or_dcpl_38 | or_dcpl_41 | (fsm_output[0]))) ) begin
      weight_index_8_0_sva_1 <= z_out_2;
    end
  end
  always @(posedge clk) begin
    if ( while_while_for_and_cse ) begin
      while_while_for_read_mem_weight_buffer_rsc_cse_sva <= weight_buffer_rsci_q_d;
      while_while_for_read_mem_feature_buffer_rsc_cse_sva <= feature_buffer_rsci_q_d;
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (~(or_dcpl_47 | or_dcpl_20 | (fsm_output[0]))) ) begin
      memory_encode_strb_1_if_6_lshift_itm <= memory_encode_strb_1_if_6_lshift_itm_1;
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (~ mux_128_nl) ) begin
      while_for_slc_23_8_11_itm <= sum_array_rsci_q_d[23:8];
    end
  end
  always @(posedge clk) begin
    if ( core_wen & ((~ mux_132_nl) | while_for_acc_10_itm_mx0c1) ) begin
      while_for_acc_10_itm <= MUX_v_16_2_2((sum_array_rsci_q_d[23:8]), z_out, while_for_acc_10_itm_mx0c1);
    end
  end
  always @(posedge clk) begin
    if ( core_wen & ((and_dcpl_208 & and_dcpl_136) | while_for_acc_13_itm_mx0c1)
        ) begin
      while_for_acc_13_itm <= MUX_v_16_2_2((sum_array_rsci_q_d[23:8]), while_for_acc_8_itm_mx0w1,
          while_for_acc_13_itm_mx0c1);
    end
  end
  always @(posedge clk) begin
    if ( core_wen ) begin
      while_for_acc_14_itm <= MUX_v_16_2_2(z_out, (sum_array_rsci_q_d[23:8]), and_214_nl);
    end
  end
  always @(posedge clk) begin
    if ( core_wen & (while_for_acc_17_itm_mx0c0 | and_dcpl_207 | while_for_acc_17_itm_mx0c2
        | while_for_acc_17_itm_mx0c3) ) begin
      while_for_acc_17_itm <= MUX1HOT_v_16_3_2(while_for_acc_8_itm_mx0w1, (sum_array_rsci_q_d[23:8]),
          z_out_3, {while_for_acc_17_itm_mx0c0 , and_dcpl_207 , while_for_or_4_nl});
    end
  end
  assign or_nl = and_dcpl_178 | and_dcpl_175;
  assign nl_weights_acc_psp_sva  = (addr_hi_rsci_idat + conv_s2u_1_32(weight_addr_rsci_idat[31]));
  assign operator_32_true_and_nl = (in_vector_length_rsci_idat[31]) & ((in_vector_length_rsci_idat[4:0]!=5'b00000));
  assign nl_operator_32_true_operator_32_true_acc_psp_sva  = ((in_vector_length_rsci_idat[31:5])
      + conv_u2s_1_27(operator_32_true_and_nl));
  assign i_and_5_nl = (~ nor_81_ssc) & i_1_31_13_sva_mx0c2;
  assign i_i_mux1h_nl = MUX1HOT_v_2_3_2((i_1_31_13_sva_2[17:16]), (weight_page_17_0_sva_1_mx0w1[17:16]),
      i_1_31_13_sva_17_16, {(~ i_1_31_13_sva_mx0c2) , i_and_5_nl , nor_81_ssc});
  assign not_364_nl = ~ or_dcpl;
  assign nand_23_nl = ~((fsm_output[1]) & (fsm_output[3]) & (fsm_output[4]) & (fsm_output[5])
      & (~ (fsm_output[6])));
  assign mux_140_nl = MUX_s_1_2_2(nand_23_nl, mux_tmp_130, fsm_output[0]);
  assign and_206_nl = (~ mux_140_nl) & (fsm_output[2]);
  assign i_mux1h_2_nl = MUX1HOT_v_16_4_2((i_1_31_13_sva_2[15:0]), (weight_page_17_0_sva_1_mx0w1[15:0]),
      z_out, z_out_3, {i_1_31_13_sva_mx0c1 , i_1_31_13_sva_mx0c2 , and_206_nl , and_dcpl_207});
  assign not_365_nl = ~ or_dcpl;
  assign mux_164_nl = MUX_s_1_2_2(or_tmp_67, or_dcpl_47, fsm_output[1]);
  assign mux_165_nl = MUX_s_1_2_2(mux_tmp_93, mux_164_nl, fsm_output[2]);
  assign mux_162_nl = MUX_s_1_2_2(or_tmp_96, mux_tmp_96, fsm_output[1]);
  assign mux_163_nl = MUX_s_1_2_2(mux_tmp_94, mux_162_nl, fsm_output[2]);
  assign mux_166_nl = MUX_s_1_2_2(mux_165_nl, mux_163_nl, fsm_output[0]);
  assign input_index_not_1_nl = ~ input_index_sva_mx0c1;
  assign mux_112_nl = MUX_s_1_2_2((~ or_dcpl_8), nor_tmp_13, fsm_output[2]);
  assign mux_113_nl = MUX_s_1_2_2(not_tmp_71, mux_112_nl, fsm_output[0]);
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_memory_axi_burst_read_base_axi_u512_512_1_for_beat_mux_nl
      = MUX_v_3_2_2((z_out_2[7:5]), (weight_index_8_0_sva_1[7:5]), and_dcpl_201);
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_not_1_nl = ~ memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c2;
  assign or_125_nl = or_dcpl_38 | xnor_2_tmp | (fsm_output[2]);
  assign for_for_or_nl = MUX_v_5_2_2(memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0,
      5'b11111, or_125_nl);
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_memory_axi_burst_read_base_axi_u512_512_1_for_beat_mux_1_nl
      = MUX_v_5_2_2(for_for_or_nl, (weight_index_8_0_sva_1[4:0]), and_dcpl_201);
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_not_nl = ~ memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_0_mx0c2;
  assign memory_axi_burst_read_base_axi_u512_512_1_for_beat_and_2_nl = MUX_v_5_2_2(5'b00000,
      memory_axi_burst_read_base_axi_u512_512_1_for_beat_memory_axi_burst_read_base_axi_u512_512_1_for_beat_mux_1_nl,
      memory_axi_burst_read_base_axi_u512_512_1_for_beat_not_nl);
  assign nor_101_nl = ~(nor_tmp_1 | (~ or_tmp_115));
  assign mux_167_nl = MUX_s_1_2_2(nor_101_nl, or_tmp_115, fsm_output[1]);
  assign mux_168_nl = MUX_s_1_2_2(mux_167_nl, or_119_cse, fsm_output[2]);
  assign and_298_nl = or_156_cse & or_tmp_115;
  assign mux_169_nl = MUX_s_1_2_2(mux_168_nl, and_298_nl, fsm_output[0]);
  assign mux_170_nl = MUX_s_1_2_2(or_tmp_115, mux_169_nl, and_dcpl_51);
  assign mux_126_nl = MUX_s_1_2_2(mux_tmp_125, or_tmp_89, fsm_output[1]);
  assign mux_127_nl = MUX_s_1_2_2(mux_126_nl, mux_tmp_124, fsm_output[2]);
  assign mux_122_nl = MUX_s_1_2_2(or_tmp_87, mux_tmp_90, fsm_output[1]);
  assign mux_121_nl = MUX_s_1_2_2(or_tmp_89, or_tmp_87, fsm_output[1]);
  assign mux_123_nl = MUX_s_1_2_2(mux_122_nl, mux_121_nl, fsm_output[2]);
  assign mux_128_nl = MUX_s_1_2_2(mux_127_nl, mux_123_nl, fsm_output[0]);
  assign mux_131_nl = MUX_s_1_2_2(or_tmp_94, mux_tmp_130, fsm_output[2]);
  assign or_141_nl = (fsm_output[1]) | mux_tmp_125;
  assign mux_129_nl = MUX_s_1_2_2(or_141_nl, mux_tmp_124, fsm_output[2]);
  assign mux_132_nl = MUX_s_1_2_2(mux_131_nl, mux_129_nl, fsm_output[0]);
  assign or_115_nl = (fsm_output[3:1]!=3'b110);
  assign mux_146_nl = MUX_s_1_2_2(or_115_nl, or_tmp_17, fsm_output[0]);
  assign and_214_nl = (~ mux_146_nl) & and_dcpl_24 & (~ (fsm_output[4]));
  assign while_for_or_4_nl = while_for_acc_17_itm_mx0c2 | while_for_acc_17_itm_mx0c3;
  assign and_250_nl = (fsm_output==7'b0111011);
  assign and_255_nl = (fsm_output[2]) & (~ (fsm_output[4])) & (~ (fsm_output[5]))
      & (fsm_output[6]) & (~(((fsm_output[0]) ^ (fsm_output[3])) | (fsm_output[1])));
  assign and_nl = (fsm_output[5]) & (fsm_output[4]) & (fsm_output[0]) & (~ (fsm_output[1]))
      & (fsm_output[2]);
  assign nand_27_nl = ~((fsm_output[0]) & mux_tmp);
  assign or_182_nl = (fsm_output[2:0]!=3'b010);
  assign mux_155_nl = MUX_s_1_2_2(nand_27_nl, or_182_nl, fsm_output[4]);
  assign nor_86_nl = ~((fsm_output[5]) | mux_155_nl);
  assign mux_156_nl = MUX_s_1_2_2(and_nl, nor_86_nl, fsm_output[6]);
  assign and_293_nl = (fsm_output[5]) & (fsm_output[4]) & (~ (fsm_output[0])) & mux_tmp;
  assign nor_87_nl = ~((fsm_output[5]) | (fsm_output[4]) | (fsm_output[0]) | (~ (fsm_output[1]))
      | (fsm_output[2]));
  assign mux_154_nl = MUX_s_1_2_2(and_293_nl, nor_87_nl, fsm_output[6]);
  assign mux_157_nl = MUX_s_1_2_2(mux_156_nl, mux_154_nl, fsm_output[3]);
  assign while_for_mux1h_5_nl = MUX1HOT_v_16_3_2(while_for_acc_13_itm, while_for_acc_14_itm,
      while_for_acc_10_itm, {and_250_nl , and_255_nl , mux_157_nl});
  assign nl_z_out = (sum_array_rsci_q_d[23:8]) + while_for_mux1h_5_nl + while_for_slc_23_8_11_itm;
  assign z_out = nl_z_out[15:0];
  assign operator_64_false_operator_64_false_and_nl = (input_index_sva[31]) & (~
      and_dcpl_239);
  assign operator_64_false_mux_nl = MUX_v_10_2_2((input_index_sva[30:21]), (signext_10_1(input_index_sva[31])),
      and_dcpl_244);
  assign not_367_nl = ~ and_dcpl_239;
  assign operator_64_false_operator_64_false_and_1_nl = MUX_v_10_2_2(10'b0000000000,
      operator_64_false_mux_nl, not_367_nl);
  assign operator_64_false_mux1h_nl = MUX1HOT_s_1_3_2((input_index_sva[20]), (addr_hi_rsci_idat[31]),
      (input_index_sva[31]), {and_dcpl_236 , and_dcpl_239 , and_dcpl_244});
  assign operator_64_false_mux1h_3_nl = MUX1HOT_v_20_3_2((input_index_sva[19:0]),
      (addr_hi_rsci_idat[30:11]), (input_index_sva[30:11]), {and_dcpl_236 , and_dcpl_239
      , and_dcpl_244});
  assign operator_64_false_mux1h_2_nl = MUX1HOT_v_11_3_2((features_io_read_feature_addr_rsc_cse_sva[31:21]),
      (addr_hi_rsci_idat[10:0]), (input_index_sva[10:0]), {and_dcpl_236 , and_dcpl_239
      , and_dcpl_244});
  assign operator_64_false_mux1h_1_nl = MUX1HOT_v_11_3_2((i_1_31_13_sva_15_0[10:0]),
      (signext_11_1(feature_addr_rsci_idat[31])), 11'b00000000001, {and_dcpl_236
      , and_dcpl_239 , and_dcpl_244});
  assign nl_z_out_1 = ({operator_64_false_operator_64_false_and_nl , operator_64_false_operator_64_false_and_1_nl
      , operator_64_false_mux1h_nl , operator_64_false_mux1h_3_nl , operator_64_false_mux1h_2_nl})
      + conv_s2u_11_43(operator_64_false_mux1h_1_nl);
  assign z_out_1 = nl_z_out_1[42:0];
  assign for_mux_2_nl = MUX_v_3_2_2((signext_3_1(memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0[4])),
      memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_7_5, and_dcpl_251);
  assign nl_z_out_2 = conv_u2u_8_9({for_mux_2_nl , memory_axi_burst_read_base_axi_u512_512_1_for_beat_8_0_sva_4_0})
      + conv_s2u_2_9({(~ and_dcpl_251) , 1'b1});
  assign z_out_2 = nl_z_out_2[8:0];
  assign and_283_nl = and_dcpl_255 & and_dcpl_254 & and_dcpl_253;
  assign and_286_nl = and_dcpl_255 & (~ (fsm_output[5])) & (fsm_output[0]) & and_dcpl_253;
  assign and_291_nl = (fsm_output[1]) & (fsm_output[4]) & and_dcpl_254 & (~ (fsm_output[2]))
      & (fsm_output[6]) & (~ (fsm_output[3]));
  assign while_for_mux1h_6_nl = MUX1HOT_v_16_3_2(while_for_acc_8_itm_mx0w1, (sum_array_rsci_q_d[23:8]),
      z_out, {and_283_nl , and_286_nl , and_291_nl});
  assign nl_z_out_3 = while_for_mux1h_6_nl + while_for_acc_17_itm;
  assign z_out_3 = nl_z_out_3[15:0];

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


  function automatic [10:0] MUX1HOT_v_11_3_2;
    input [10:0] input_2;
    input [10:0] input_1;
    input [10:0] input_0;
    input [2:0] sel;
    reg [10:0] result;
  begin
    result = input_0 & {11{sel[0]}};
    result = result | (input_1 & {11{sel[1]}});
    result = result | (input_2 & {11{sel[2]}});
    MUX1HOT_v_11_3_2 = result;
  end
  endfunction


  function automatic [15:0] MUX1HOT_v_16_32_2;
    input [15:0] input_31;
    input [15:0] input_30;
    input [15:0] input_29;
    input [15:0] input_28;
    input [15:0] input_27;
    input [15:0] input_26;
    input [15:0] input_25;
    input [15:0] input_24;
    input [15:0] input_23;
    input [15:0] input_22;
    input [15:0] input_21;
    input [15:0] input_20;
    input [15:0] input_19;
    input [15:0] input_18;
    input [15:0] input_17;
    input [15:0] input_16;
    input [15:0] input_15;
    input [15:0] input_14;
    input [15:0] input_13;
    input [15:0] input_12;
    input [15:0] input_11;
    input [15:0] input_10;
    input [15:0] input_9;
    input [15:0] input_8;
    input [15:0] input_7;
    input [15:0] input_6;
    input [15:0] input_5;
    input [15:0] input_4;
    input [15:0] input_3;
    input [15:0] input_2;
    input [15:0] input_1;
    input [15:0] input_0;
    input [31:0] sel;
    reg [15:0] result;
  begin
    result = input_0 & {16{sel[0]}};
    result = result | (input_1 & {16{sel[1]}});
    result = result | (input_2 & {16{sel[2]}});
    result = result | (input_3 & {16{sel[3]}});
    result = result | (input_4 & {16{sel[4]}});
    result = result | (input_5 & {16{sel[5]}});
    result = result | (input_6 & {16{sel[6]}});
    result = result | (input_7 & {16{sel[7]}});
    result = result | (input_8 & {16{sel[8]}});
    result = result | (input_9 & {16{sel[9]}});
    result = result | (input_10 & {16{sel[10]}});
    result = result | (input_11 & {16{sel[11]}});
    result = result | (input_12 & {16{sel[12]}});
    result = result | (input_13 & {16{sel[13]}});
    result = result | (input_14 & {16{sel[14]}});
    result = result | (input_15 & {16{sel[15]}});
    result = result | (input_16 & {16{sel[16]}});
    result = result | (input_17 & {16{sel[17]}});
    result = result | (input_18 & {16{sel[18]}});
    result = result | (input_19 & {16{sel[19]}});
    result = result | (input_20 & {16{sel[20]}});
    result = result | (input_21 & {16{sel[21]}});
    result = result | (input_22 & {16{sel[22]}});
    result = result | (input_23 & {16{sel[23]}});
    result = result | (input_24 & {16{sel[24]}});
    result = result | (input_25 & {16{sel[25]}});
    result = result | (input_26 & {16{sel[26]}});
    result = result | (input_27 & {16{sel[27]}});
    result = result | (input_28 & {16{sel[28]}});
    result = result | (input_29 & {16{sel[29]}});
    result = result | (input_30 & {16{sel[30]}});
    result = result | (input_31 & {16{sel[31]}});
    MUX1HOT_v_16_32_2 = result;
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


  function automatic [15:0] MUX1HOT_v_16_4_2;
    input [15:0] input_3;
    input [15:0] input_2;
    input [15:0] input_1;
    input [15:0] input_0;
    input [3:0] sel;
    reg [15:0] result;
  begin
    result = input_0 & {16{sel[0]}};
    result = result | (input_1 & {16{sel[1]}});
    result = result | (input_2 & {16{sel[2]}});
    result = result | (input_3 & {16{sel[3]}});
    MUX1HOT_v_16_4_2 = result;
  end
  endfunction


  function automatic [19:0] MUX1HOT_v_20_3_2;
    input [19:0] input_2;
    input [19:0] input_1;
    input [19:0] input_0;
    input [2:0] sel;
    reg [19:0] result;
  begin
    result = input_0 & {20{sel[0]}};
    result = result | (input_1 & {20{sel[1]}});
    result = result | (input_2 & {20{sel[2]}});
    MUX1HOT_v_20_3_2 = result;
  end
  endfunction


  function automatic [1:0] MUX1HOT_v_2_3_2;
    input [1:0] input_2;
    input [1:0] input_1;
    input [1:0] input_0;
    input [2:0] sel;
    reg [1:0] result;
  begin
    result = input_0 & {2{sel[0]}};
    result = result | (input_1 & {2{sel[1]}});
    result = result | (input_2 & {2{sel[2]}});
    MUX1HOT_v_2_3_2 = result;
  end
  endfunction


  function automatic [31:0] MUX1HOT_v_32_3_2;
    input [31:0] input_2;
    input [31:0] input_1;
    input [31:0] input_0;
    input [2:0] sel;
    reg [31:0] result;
  begin
    result = input_0 & {32{sel[0]}};
    result = result | (input_1 & {32{sel[1]}});
    result = result | (input_2 & {32{sel[2]}});
    MUX1HOT_v_32_3_2 = result;
  end
  endfunction


  function automatic [4:0] MUX1HOT_v_5_30_2;
    input [4:0] input_29;
    input [4:0] input_28;
    input [4:0] input_27;
    input [4:0] input_26;
    input [4:0] input_25;
    input [4:0] input_24;
    input [4:0] input_23;
    input [4:0] input_22;
    input [4:0] input_21;
    input [4:0] input_20;
    input [4:0] input_19;
    input [4:0] input_18;
    input [4:0] input_17;
    input [4:0] input_16;
    input [4:0] input_15;
    input [4:0] input_14;
    input [4:0] input_13;
    input [4:0] input_12;
    input [4:0] input_11;
    input [4:0] input_10;
    input [4:0] input_9;
    input [4:0] input_8;
    input [4:0] input_7;
    input [4:0] input_6;
    input [4:0] input_5;
    input [4:0] input_4;
    input [4:0] input_3;
    input [4:0] input_2;
    input [4:0] input_1;
    input [4:0] input_0;
    input [29:0] sel;
    reg [4:0] result;
  begin
    result = input_0 & {5{sel[0]}};
    result = result | (input_1 & {5{sel[1]}});
    result = result | (input_2 & {5{sel[2]}});
    result = result | (input_3 & {5{sel[3]}});
    result = result | (input_4 & {5{sel[4]}});
    result = result | (input_5 & {5{sel[5]}});
    result = result | (input_6 & {5{sel[6]}});
    result = result | (input_7 & {5{sel[7]}});
    result = result | (input_8 & {5{sel[8]}});
    result = result | (input_9 & {5{sel[9]}});
    result = result | (input_10 & {5{sel[10]}});
    result = result | (input_11 & {5{sel[11]}});
    result = result | (input_12 & {5{sel[12]}});
    result = result | (input_13 & {5{sel[13]}});
    result = result | (input_14 & {5{sel[14]}});
    result = result | (input_15 & {5{sel[15]}});
    result = result | (input_16 & {5{sel[16]}});
    result = result | (input_17 & {5{sel[17]}});
    result = result | (input_18 & {5{sel[18]}});
    result = result | (input_19 & {5{sel[19]}});
    result = result | (input_20 & {5{sel[20]}});
    result = result | (input_21 & {5{sel[21]}});
    result = result | (input_22 & {5{sel[22]}});
    result = result | (input_23 & {5{sel[23]}});
    result = result | (input_24 & {5{sel[24]}});
    result = result | (input_25 & {5{sel[25]}});
    result = result | (input_26 & {5{sel[26]}});
    result = result | (input_27 & {5{sel[27]}});
    result = result | (input_28 & {5{sel[28]}});
    result = result | (input_29 & {5{sel[29]}});
    MUX1HOT_v_5_30_2 = result;
  end
  endfunction


  function automatic [4:0] MUX1HOT_v_5_31_2;
    input [4:0] input_30;
    input [4:0] input_29;
    input [4:0] input_28;
    input [4:0] input_27;
    input [4:0] input_26;
    input [4:0] input_25;
    input [4:0] input_24;
    input [4:0] input_23;
    input [4:0] input_22;
    input [4:0] input_21;
    input [4:0] input_20;
    input [4:0] input_19;
    input [4:0] input_18;
    input [4:0] input_17;
    input [4:0] input_16;
    input [4:0] input_15;
    input [4:0] input_14;
    input [4:0] input_13;
    input [4:0] input_12;
    input [4:0] input_11;
    input [4:0] input_10;
    input [4:0] input_9;
    input [4:0] input_8;
    input [4:0] input_7;
    input [4:0] input_6;
    input [4:0] input_5;
    input [4:0] input_4;
    input [4:0] input_3;
    input [4:0] input_2;
    input [4:0] input_1;
    input [4:0] input_0;
    input [30:0] sel;
    reg [4:0] result;
  begin
    result = input_0 & {5{sel[0]}};
    result = result | (input_1 & {5{sel[1]}});
    result = result | (input_2 & {5{sel[2]}});
    result = result | (input_3 & {5{sel[3]}});
    result = result | (input_4 & {5{sel[4]}});
    result = result | (input_5 & {5{sel[5]}});
    result = result | (input_6 & {5{sel[6]}});
    result = result | (input_7 & {5{sel[7]}});
    result = result | (input_8 & {5{sel[8]}});
    result = result | (input_9 & {5{sel[9]}});
    result = result | (input_10 & {5{sel[10]}});
    result = result | (input_11 & {5{sel[11]}});
    result = result | (input_12 & {5{sel[12]}});
    result = result | (input_13 & {5{sel[13]}});
    result = result | (input_14 & {5{sel[14]}});
    result = result | (input_15 & {5{sel[15]}});
    result = result | (input_16 & {5{sel[16]}});
    result = result | (input_17 & {5{sel[17]}});
    result = result | (input_18 & {5{sel[18]}});
    result = result | (input_19 & {5{sel[19]}});
    result = result | (input_20 & {5{sel[20]}});
    result = result | (input_21 & {5{sel[21]}});
    result = result | (input_22 & {5{sel[22]}});
    result = result | (input_23 & {5{sel[23]}});
    result = result | (input_24 & {5{sel[24]}});
    result = result | (input_25 & {5{sel[25]}});
    result = result | (input_26 & {5{sel[26]}});
    result = result | (input_27 & {5{sel[27]}});
    result = result | (input_28 & {5{sel[28]}});
    result = result | (input_29 & {5{sel[29]}});
    result = result | (input_30 & {5{sel[30]}});
    MUX1HOT_v_5_31_2 = result;
  end
  endfunction


  function automatic [6:0] MUX1HOT_v_7_3_2;
    input [6:0] input_2;
    input [6:0] input_1;
    input [6:0] input_0;
    input [2:0] sel;
    reg [6:0] result;
  begin
    result = input_0 & {7{sel[0]}};
    result = result | (input_1 & {7{sel[1]}});
    result = result | (input_2 & {7{sel[2]}});
    MUX1HOT_v_7_3_2 = result;
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


  function automatic [9:0] signext_10_1;
    input  vector;
  begin
    signext_10_1= {{9{vector}}, vector};
  end
  endfunction


  function automatic [10:0] signext_11_1;
    input  vector;
  begin
    signext_11_1= {{10{vector}}, vector};
  end
  endfunction


  function automatic [2:0] signext_3_1;
    input  vector;
  begin
    signext_3_1= {{2{vector}}, vector};
  end
  endfunction


  function automatic [31:0] conv_s2s_27_32 ;
    input [26:0]  vector ;
  begin
    conv_s2s_27_32 = {{5{vector[26]}}, vector};
  end
  endfunction


  function automatic [31:0] conv_s2u_1_32 ;
    input  vector ;
  begin
    conv_s2u_1_32 = {{31{vector}}, vector};
  end
  endfunction


  function automatic [8:0] conv_s2u_2_9 ;
    input [1:0]  vector ;
  begin
    conv_s2u_2_9 = {{7{vector[1]}}, vector};
  end
  endfunction


  function automatic [42:0] conv_s2u_11_43 ;
    input [10:0]  vector ;
  begin
    conv_s2u_11_43 = {{32{vector[10]}}, vector};
  end
  endfunction


  function automatic [49:0] conv_s2u_18_50 ;
    input [17:0]  vector ;
  begin
    conv_s2u_18_50 = {{32{vector[17]}}, vector};
  end
  endfunction


  function automatic [26:0] conv_u2s_1_27 ;
    input  vector ;
  begin
    conv_u2s_1_27 = {{26{1'b0}}, vector};
  end
  endfunction


  function automatic [8:0] conv_u2u_8_9 ;
    input [7:0]  vector ;
  begin
    conv_u2u_8_9 = {1'b0, vector};
  end
  endfunction

endmodule

// ------------------------------------------------------------------
//  Design Unit:    dense_struct
// ------------------------------------------------------------------


module dense_struct (
  clk, arst_n, start_rsc_dat, start_rsc_vld, start_rsc_rdy, done_rsc_dat, done_rsc_vld,
      done_rsc_rdy, use_relu_triosy_lz, addr_hi_rsc_dat, addr_hi_triosy_lz, feature_addr_rsc_dat,
      feature_addr_triosy_lz, weight_addr_rsc_dat, weight_addr_triosy_lz, output_addr_rsc_dat,
      output_addr_triosy_lz, in_vector_length_rsc_dat, in_vector_length_triosy_lz,
      out_vector_length_triosy_lz, memory_channels_aw_channel_rsc_dat_id, memory_channels_aw_channel_rsc_dat_address,
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
  input [31:0] feature_addr_rsc_dat;
  output feature_addr_triosy_lz;
  input [31:0] weight_addr_rsc_dat;
  output weight_addr_triosy_lz;
  input [31:0] output_addr_rsc_dat;
  output output_addr_triosy_lz;
  input [31:0] in_vector_length_rsc_dat;
  output in_vector_length_triosy_lz;
  output out_vector_length_triosy_lz;
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
  wire feature_buffer_rsci_clken_d;
  wire [511:0] feature_buffer_rsci_q_d;
  wire [14:0] feature_buffer_rsci_radr_d;
  wire [14:0] feature_buffer_rsci_wadr_d;
  wire [511:0] weight_buffer_rsci_q_d;
  wire [7:0] weight_buffer_rsci_radr_d;
  wire [7:0] weight_buffer_rsci_wadr_d;
  wire [31:0] sum_array_rsci_d_d;
  wire [31:0] sum_array_rsci_q_d;
  wire [4:0] sum_array_rsci_radr_d;
  wire [4:0] sum_array_rsci_wadr_d;
  wire feature_buffer_rsc_clken;
  wire [511:0] feature_buffer_rsc_q;
  wire feature_buffer_rsc_re;
  wire [14:0] feature_buffer_rsc_radr;
  wire feature_buffer_rsc_we;
  wire [511:0] feature_buffer_rsc_d;
  wire [14:0] feature_buffer_rsc_wadr;
  wire weight_buffer_rsc_clken;
  wire [511:0] weight_buffer_rsc_q;
  wire weight_buffer_rsc_re;
  wire [7:0] weight_buffer_rsc_radr;
  wire weight_buffer_rsc_we;
  wire [511:0] weight_buffer_rsc_d;
  wire [7:0] weight_buffer_rsc_wadr;
  wire sum_array_rsc_clken;
  wire [31:0] sum_array_rsc_q;
  wire sum_array_rsc_re;
  wire [4:0] sum_array_rsc_radr;
  wire sum_array_rsc_we;
  wire [31:0] sum_array_rsc_d;
  wire [4:0] sum_array_rsc_wadr;
  wire [108:0] memory_channels_aw_channel_rsc_dat;
  wire [576:0] memory_channels_w_channel_rsc_dat;
  wire [108:0] memory_channels_ar_channel_rsc_dat;
  wire [511:0] feature_buffer_rsci_d_d_iff;
  wire feature_buffer_rsci_re_d_iff;
  wire feature_buffer_rsci_we_d_iff;
  wire weight_buffer_rsci_we_d_iff;
  wire sum_array_rsci_re_d_iff;
  wire sum_array_rsci_we_d_iff;


  // Interconnect Declarations for Component Instantiations 
  wire [17:0] nl_dense_core_inst_memory_channels_b_channel_rsc_dat;
  assign nl_dense_core_inst_memory_channels_b_channel_rsc_dat = {memory_channels_b_channel_rsc_dat_id
      , memory_channels_b_channel_rsc_dat_resp};
  wire [530:0] nl_dense_core_inst_memory_channels_r_channel_rsc_dat;
  assign nl_dense_core_inst_memory_channels_r_channel_rsc_dat = {memory_channels_r_channel_rsc_dat_id
      , memory_channels_r_channel_rsc_dat_data , memory_channels_r_channel_rsc_dat_resp
      , memory_channels_r_channel_rsc_dat_last};
  BLOCK_1R1W_RBW #(.addr_width(32'sd15),
  .data_width(32'sd512),
  .depth(32'sd32768),
  .latency(32'sd1),
  .suppress_sim_read_addr_range_errs(32'sd1)) feature_buffer_rsc_comp (
      .clk(clk),
      .clken(feature_buffer_rsc_clken),
      .d(feature_buffer_rsc_d),
      .q(feature_buffer_rsc_q),
      .radr(feature_buffer_rsc_radr),
      .re(feature_buffer_rsc_re),
      .wadr(feature_buffer_rsc_wadr),
      .we(feature_buffer_rsc_we)
    );
  BLOCK_1R1W_RBW #(.addr_width(32'sd8),
  .data_width(32'sd512),
  .depth(32'sd256),
  .latency(32'sd1),
  .suppress_sim_read_addr_range_errs(32'sd1)) weight_buffer_rsc_comp (
      .clk(clk),
      .clken(weight_buffer_rsc_clken),
      .d(weight_buffer_rsc_d),
      .q(weight_buffer_rsc_q),
      .radr(weight_buffer_rsc_radr),
      .re(weight_buffer_rsc_re),
      .wadr(weight_buffer_rsc_wadr),
      .we(weight_buffer_rsc_we)
    );
  BLOCK_1R1W_RBW #(.addr_width(32'sd5),
  .data_width(32'sd32),
  .depth(32'sd32),
  .latency(32'sd1),
  .suppress_sim_read_addr_range_errs(32'sd1)) sum_array_rsc_comp (
      .clk(clk),
      .clken(sum_array_rsc_clken),
      .d(sum_array_rsc_d),
      .q(sum_array_rsc_q),
      .radr(sum_array_rsc_radr),
      .re(sum_array_rsc_re),
      .wadr(sum_array_rsc_wadr),
      .we(sum_array_rsc_we)
    );
  dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_16_15_512_32768_1_32768_512_1_gen feature_buffer_rsci
      (
      .clken(feature_buffer_rsc_clken),
      .q(feature_buffer_rsc_q),
      .re(feature_buffer_rsc_re),
      .radr(feature_buffer_rsc_radr),
      .we(feature_buffer_rsc_we),
      .d(feature_buffer_rsc_d),
      .wadr(feature_buffer_rsc_wadr),
      .clken_d(feature_buffer_rsci_clken_d),
      .d_d(feature_buffer_rsci_d_d_iff),
      .q_d(feature_buffer_rsci_q_d),
      .radr_d(feature_buffer_rsci_radr_d),
      .re_d(feature_buffer_rsci_re_d_iff),
      .wadr_d(feature_buffer_rsci_wadr_d),
      .we_d(feature_buffer_rsci_we_d_iff),
      .writeA_w_ram_ir_internal_WMASK_B_d(feature_buffer_rsci_we_d_iff),
      .readA_r_ram_ir_internal_RMASK_B_d(feature_buffer_rsci_re_d_iff)
    );
  dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_17_8_512_256_1_256_512_1_gen weight_buffer_rsci
      (
      .clken(weight_buffer_rsc_clken),
      .q(weight_buffer_rsc_q),
      .re(weight_buffer_rsc_re),
      .radr(weight_buffer_rsc_radr),
      .we(weight_buffer_rsc_we),
      .d(weight_buffer_rsc_d),
      .wadr(weight_buffer_rsc_wadr),
      .clken_d(feature_buffer_rsci_clken_d),
      .d_d(feature_buffer_rsci_d_d_iff),
      .q_d(weight_buffer_rsci_q_d),
      .radr_d(weight_buffer_rsci_radr_d),
      .re_d(feature_buffer_rsci_re_d_iff),
      .wadr_d(weight_buffer_rsci_wadr_d),
      .we_d(weight_buffer_rsci_we_d_iff),
      .writeA_w_ram_ir_internal_WMASK_B_d(weight_buffer_rsci_we_d_iff),
      .readA_r_ram_ir_internal_RMASK_B_d(feature_buffer_rsci_re_d_iff)
    );
  dense_Xilinx_RAMS_BLOCK_1R1W_RBW_rwport_en_18_5_32_32_1_32_32_1_gen sum_array_rsci
      (
      .clken(sum_array_rsc_clken),
      .q(sum_array_rsc_q),
      .re(sum_array_rsc_re),
      .radr(sum_array_rsc_radr),
      .we(sum_array_rsc_we),
      .d(sum_array_rsc_d),
      .wadr(sum_array_rsc_wadr),
      .clken_d(feature_buffer_rsci_clken_d),
      .d_d(sum_array_rsci_d_d),
      .q_d(sum_array_rsci_q_d),
      .radr_d(sum_array_rsci_radr_d),
      .re_d(sum_array_rsci_re_d_iff),
      .wadr_d(sum_array_rsci_wadr_d),
      .we_d(sum_array_rsci_we_d_iff),
      .writeA_w_ram_ir_internal_WMASK_B_d(sum_array_rsci_we_d_iff),
      .readA_r_ram_ir_internal_RMASK_B_d(sum_array_rsci_re_d_iff)
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
      .feature_addr_rsc_dat(feature_addr_rsc_dat),
      .feature_addr_triosy_lz(feature_addr_triosy_lz),
      .weight_addr_rsc_dat(weight_addr_rsc_dat),
      .weight_addr_triosy_lz(weight_addr_triosy_lz),
      .output_addr_rsc_dat(output_addr_rsc_dat),
      .output_addr_triosy_lz(output_addr_triosy_lz),
      .in_vector_length_rsc_dat(in_vector_length_rsc_dat),
      .in_vector_length_triosy_lz(in_vector_length_triosy_lz),
      .out_vector_length_triosy_lz(out_vector_length_triosy_lz),
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
      .feature_buffer_rsci_clken_d(feature_buffer_rsci_clken_d),
      .feature_buffer_rsci_q_d(feature_buffer_rsci_q_d),
      .feature_buffer_rsci_radr_d(feature_buffer_rsci_radr_d),
      .feature_buffer_rsci_wadr_d(feature_buffer_rsci_wadr_d),
      .weight_buffer_rsci_q_d(weight_buffer_rsci_q_d),
      .weight_buffer_rsci_radr_d(weight_buffer_rsci_radr_d),
      .weight_buffer_rsci_wadr_d(weight_buffer_rsci_wadr_d),
      .sum_array_rsci_d_d(sum_array_rsci_d_d),
      .sum_array_rsci_q_d(sum_array_rsci_q_d),
      .sum_array_rsci_radr_d(sum_array_rsci_radr_d),
      .sum_array_rsci_wadr_d(sum_array_rsci_wadr_d),
      .feature_buffer_rsci_d_d_pff(feature_buffer_rsci_d_d_iff),
      .feature_buffer_rsci_re_d_pff(feature_buffer_rsci_re_d_iff),
      .feature_buffer_rsci_we_d_pff(feature_buffer_rsci_we_d_iff),
      .weight_buffer_rsci_we_d_pff(weight_buffer_rsci_we_d_iff),
      .sum_array_rsci_re_d_pff(sum_array_rsci_re_d_iff),
      .sum_array_rsci_we_d_pff(sum_array_rsci_we_d_iff)
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
      feature_addr_rsc_dat, feature_addr_triosy_lz, weight_addr_rsc_dat, weight_addr_triosy_lz,
      output_addr_rsc_dat, output_addr_triosy_lz, in_vector_length_rsc_dat, in_vector_length_triosy_lz,
      out_vector_length_rsc_dat, out_vector_length_triosy_lz, memory_channels_aw_channel_rsc_dat,
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
  input [31:0] use_relu_rsc_dat;
  output use_relu_triosy_lz;
  input [31:0] addr_hi_rsc_dat;
  output addr_hi_triosy_lz;
  input [31:0] feature_addr_rsc_dat;
  output feature_addr_triosy_lz;
  input [31:0] weight_addr_rsc_dat;
  output weight_addr_triosy_lz;
  input [31:0] output_addr_rsc_dat;
  output output_addr_triosy_lz;
  input [31:0] in_vector_length_rsc_dat;
  output in_vector_length_triosy_lz;
  input [31:0] out_vector_length_rsc_dat;
  output out_vector_length_triosy_lz;
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
      .feature_addr_rsc_dat(feature_addr_rsc_dat),
      .feature_addr_triosy_lz(feature_addr_triosy_lz),
      .weight_addr_rsc_dat(weight_addr_rsc_dat),
      .weight_addr_triosy_lz(weight_addr_triosy_lz),
      .output_addr_rsc_dat(output_addr_rsc_dat),
      .output_addr_triosy_lz(output_addr_triosy_lz),
      .in_vector_length_rsc_dat(in_vector_length_rsc_dat),
      .in_vector_length_triosy_lz(in_vector_length_triosy_lz),
      .out_vector_length_triosy_lz(out_vector_length_triosy_lz),
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



