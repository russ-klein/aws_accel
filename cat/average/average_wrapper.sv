// ////////////////////////////////////////////////////////
// File: rtl_csr_wrapper.v
//
// Contents:
//    Top Verilog/SysetmVerilog wrapper around the design 'average' along with
//    CSR instances and adaptors

// ////////////////////////////////////////////////////////
// Design Unit: average_wrap
// Description: Instantiates design and CSR instance(s)

module average_wrap
(
  clk, 
  arst_n, 
  count_triosy_lz, 
  index_hi_triosy_lz, 
  index_lo_triosy_lz, 
  result_rsc_zout, 
  result_rsc_lzout, 
  result_rsc_zin, 
  result_triosy_lz, 
  memory_channels_aw_channel_rsc_dat, 
  memory_channels_aw_channel_rsc_vld, 
  memory_channels_aw_channel_rsc_rdy, 
  memory_channels_w_channel_rsc_dat, 
  memory_channels_w_channel_rsc_vld, 
  memory_channels_w_channel_rsc_rdy, 
  memory_channels_b_channel_rsc_dat, 
  memory_channels_b_channel_rsc_vld, 
  memory_channels_b_channel_rsc_rdy, 
  memory_channels_ar_channel_rsc_dat, 
  memory_channels_ar_channel_rsc_vld, 
  memory_channels_ar_channel_rsc_rdy, 
  memory_channels_r_channel_rsc_dat, 
  memory_channels_r_channel_rsc_vld, 
  memory_channels_r_channel_rsc_rdy, 
  average_slave_0_ACLK, 
  average_slave_0_ARESETn, 
  average_slave_0_AWADDR, 
  average_slave_0_AWVALID, 
  average_slave_0_AWREADY, 
  average_slave_0_WDATA, 
  average_slave_0_WSTRB, 
  average_slave_0_WVALID, 
  average_slave_0_WREADY, 
  average_slave_0_BRESP, 
  average_slave_0_BVALID, 
  average_slave_0_BREADY, 
  average_slave_0_ARADDR, 
  average_slave_0_ARVALID, 
  average_slave_0_ARREADY, 
  average_slave_0_RDATA, 
  average_slave_0_RRESP, 
  average_slave_0_RVALID, 
  average_slave_0_RREADY 
);

  parameter integer debug = 2'd0;     // enable sim debug messages (0=silent,1=errors,2=verbose)

  input  clk;
  input  arst_n;
  output  count_triosy_lz;
  output  index_hi_triosy_lz;
  output  index_lo_triosy_lz;
  output [32-1 :0] result_rsc_zout;
  output  result_rsc_lzout;
  input [32-1 :0] result_rsc_zin;
  output  result_triosy_lz;
  output [97-1 :0] memory_channels_aw_channel_rsc_dat;
  output  memory_channels_aw_channel_rsc_vld;
  input  memory_channels_aw_channel_rsc_rdy;
  output [577-1 :0] memory_channels_w_channel_rsc_dat;
  output  memory_channels_w_channel_rsc_vld;
  input  memory_channels_w_channel_rsc_rdy;
  input [6-1 :0] memory_channels_b_channel_rsc_dat;
  input  memory_channels_b_channel_rsc_vld;
  output  memory_channels_b_channel_rsc_rdy;
  output [97-1 :0] memory_channels_ar_channel_rsc_dat;
  output  memory_channels_ar_channel_rsc_vld;
  input  memory_channels_ar_channel_rsc_rdy;
  input [519-1 :0] memory_channels_r_channel_rsc_dat;
  input  memory_channels_r_channel_rsc_vld;
  output  memory_channels_r_channel_rsc_rdy;
  input  average_slave_0_ACLK;
  input  average_slave_0_ARESETn;
  input [12-1 :0] average_slave_0_AWADDR;
  input  average_slave_0_AWVALID;
  output  average_slave_0_AWREADY;
  input [32-1 :0] average_slave_0_WDATA;
  input [4-1 :0] average_slave_0_WSTRB;
  input  average_slave_0_WVALID;
  output  average_slave_0_WREADY;
  output [2-1 :0] average_slave_0_BRESP;
  output  average_slave_0_BVALID;
  input  average_slave_0_BREADY;
  input [12-1 :0] average_slave_0_ARADDR;
  input  average_slave_0_ARVALID;
  output  average_slave_0_ARREADY;
  output [32-1 :0] average_slave_0_RDATA;
  output [2-1 :0] average_slave_0_RRESP;
  output  average_slave_0_RVALID;
  input  average_slave_0_RREADY;

  // Local signals for interconnect between HLS RTL average and CSR instance(s)
  wire  [32-1 : 0] count_rsc_dat;
  wire  [32-1 : 0] index_hi_rsc_dat;
  wire  [32-1 : 0] index_lo_rsc_dat;
  // Local signals for interconnect between CSR instance(s) and protocol adaptors
  wire  [12-1 : 0] adc_average_slave_0_addr;
  wire  adc_average_slave_0_ren;
  wire  adc_average_slave_0_wen;
  wire  [32-1 : 0] adc_average_slave_0_rdata;
  wire  [32-1 : 0] adc_average_slave_0_wdata;
  wire  adc_average_slave_0_waddr_error;
  wire  adc_average_slave_0_raddr_error; 


  ccs_axi4lite2adc #(
    .ADDR_WIDTH(12),
    .DATA_WIDTH(32)
  ) adaptor_average_slave_0
  (
     .ACLK (average_slave_0_ACLK),
     .ARESETn (average_slave_0_ARESETn),
     .AWADDR (average_slave_0_AWADDR),
     .AWVALID (average_slave_0_AWVALID),
     .AWREADY (average_slave_0_AWREADY),
     .WDATA (average_slave_0_WDATA),
     .WSTRB (average_slave_0_WSTRB),
     .WVALID (average_slave_0_WVALID),
     .WREADY (average_slave_0_WREADY),
     .BRESP (average_slave_0_BRESP),
     .BVALID (average_slave_0_BVALID),
     .BREADY (average_slave_0_BREADY),
     .ARADDR (average_slave_0_ARADDR),
     .ARVALID (average_slave_0_ARVALID),
     .ARREADY (average_slave_0_ARREADY),
     .RDATA (average_slave_0_RDATA),
     .RRESP (average_slave_0_RRESP),
     .RVALID (average_slave_0_RVALID),
     .RREADY (average_slave_0_RREADY),
     .csr_addr (adc_average_slave_0_addr),
     .csr_ren (adc_average_slave_0_ren),
     .csr_wen (adc_average_slave_0_wen),
     .csr_rdata (adc_average_slave_0_rdata),
     .csr_wdata (adc_average_slave_0_wdata),
     .csr_waddr_error (adc_average_slave_0_waddr_error),
     .csr_raddr_error (adc_average_slave_0_raddr_error)
  );

  // Drive interrupt ports

  // Instance of Catapult RTL design
  average
  HLS_RTL
  (
    .clk (clk),
    .arst_n (arst_n),
    .count_rsc_dat (count_rsc_dat),
    .count_triosy_lz (count_triosy_lz),
    .index_hi_rsc_dat (index_hi_rsc_dat),
    .index_hi_triosy_lz (index_hi_triosy_lz),
    .index_lo_rsc_dat (index_lo_rsc_dat),
    .index_lo_triosy_lz (index_lo_triosy_lz),
    .result_rsc_zout (result_rsc_zout),
    .result_rsc_lzout (result_rsc_lzout),
    .result_rsc_zin (result_rsc_zin),
    .result_triosy_lz (result_triosy_lz),
    .memory_channels_aw_channel_rsc_dat (memory_channels_aw_channel_rsc_dat),
    .memory_channels_aw_channel_rsc_vld (memory_channels_aw_channel_rsc_vld),
    .memory_channels_aw_channel_rsc_rdy (memory_channels_aw_channel_rsc_rdy),
    .memory_channels_w_channel_rsc_dat (memory_channels_w_channel_rsc_dat),
    .memory_channels_w_channel_rsc_vld (memory_channels_w_channel_rsc_vld),
    .memory_channels_w_channel_rsc_rdy (memory_channels_w_channel_rsc_rdy),
    .memory_channels_b_channel_rsc_dat (memory_channels_b_channel_rsc_dat),
    .memory_channels_b_channel_rsc_vld (memory_channels_b_channel_rsc_vld),
    .memory_channels_b_channel_rsc_rdy (memory_channels_b_channel_rsc_rdy),
    .memory_channels_ar_channel_rsc_dat (memory_channels_ar_channel_rsc_dat),
    .memory_channels_ar_channel_rsc_vld (memory_channels_ar_channel_rsc_vld),
    .memory_channels_ar_channel_rsc_rdy (memory_channels_ar_channel_rsc_rdy),
    .memory_channels_r_channel_rsc_dat (memory_channels_r_channel_rsc_dat),
    .memory_channels_r_channel_rsc_vld (memory_channels_r_channel_rsc_vld),
    .memory_channels_r_channel_rsc_rdy (memory_channels_r_channel_rsc_rdy)
  );
   

  // CSR Instances

  // average_slave_0
  average_slave_0
  #
  (
    .address_bits (12),
    .data_bits (32),
    .debug (debug)
  )
  CSR_0_average_slave_0
  (
    .clk (clk),
    .arst_n (arst_n),
    .count_rsc_dat (count_rsc_dat),
    .index_hi_rsc_dat (index_hi_rsc_dat),
    .index_lo_rsc_dat (index_lo_rsc_dat),
    .addr (adc_average_slave_0_addr),
    .ren (adc_average_slave_0_ren),
    .wen (adc_average_slave_0_wen),
    .rdata (adc_average_slave_0_rdata),
    .wdata (adc_average_slave_0_wdata),
    .waddr_error (adc_average_slave_0_waddr_error),
    .raddr_error (adc_average_slave_0_raddr_error)
  );
  
endmodule