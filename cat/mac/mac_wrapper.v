// ////////////////////////////////////////////////////////
// File: rtl_csr_wrapper.v
//
// Contents:
//    Top Verilog/SysetmVerilog wrapper around the design 'mac' along with
//    CSR instances and adaptors

// ////////////////////////////////////////////////////////
// Design Unit: mac_wrap
// Description: Instantiates design and CSR instance(s)

module mac_wrap
(
  clk, 
  arst_n, 
  f1_triosy_lz, 
  f2_triosy_lz, 
  a1_triosy_lz, 
  result_triosy_lz, 
  mac_slave_0_ACLK, 
  mac_slave_0_ARESETn, 
  mac_slave_0_AWADDR, 
  mac_slave_0_AWVALID, 
  mac_slave_0_AWREADY, 
  mac_slave_0_WDATA, 
  mac_slave_0_WSTRB, 
  mac_slave_0_WVALID, 
  mac_slave_0_WREADY, 
  mac_slave_0_BRESP, 
  mac_slave_0_BVALID, 
  mac_slave_0_BREADY, 
  mac_slave_0_ARADDR, 
  mac_slave_0_ARVALID, 
  mac_slave_0_ARREADY, 
  mac_slave_0_RDATA, 
  mac_slave_0_RRESP, 
  mac_slave_0_RVALID, 
  mac_slave_0_RREADY 
);

  parameter integer debug = 2'd0;     // enable sim debug messages (0=silent,1=errors,2=verbose)

  input  clk;
  input  arst_n;
  output  f1_triosy_lz;
  output  f2_triosy_lz;
  output  a1_triosy_lz;
  output  result_triosy_lz;
  input  mac_slave_0_ACLK;
  input  mac_slave_0_ARESETn;
  input [12-1 :0] mac_slave_0_AWADDR;
  input  mac_slave_0_AWVALID;
  output  mac_slave_0_AWREADY;
  input [64-1 :0] mac_slave_0_WDATA;
  input [8-1 :0] mac_slave_0_WSTRB;
  input  mac_slave_0_WVALID;
  output  mac_slave_0_WREADY;
  output [2-1 :0] mac_slave_0_BRESP;
  output  mac_slave_0_BVALID;
  input  mac_slave_0_BREADY;
  input [12-1 :0] mac_slave_0_ARADDR;
  input  mac_slave_0_ARVALID;
  output  mac_slave_0_ARREADY;
  output [64-1 :0] mac_slave_0_RDATA;
  output [2-1 :0] mac_slave_0_RRESP;
  output  mac_slave_0_RVALID;
  input  mac_slave_0_RREADY;

  // Local signals for interconnect between HLS RTL mac and CSR instance(s)
  wire  [12-1 : 0] f1_rsc_dat;
  wire  [12-1 : 0] f2_rsc_dat;
  wire  [24-1 : 0] a1_rsc_dat;
  wire  [25-1 : 0] result_rsc_dat;
  // Local signals for interconnect between CSR instance(s) and protocol adaptors
  wire  [12-1 : 0] adc_mac_slave_0_addr;
  wire  adc_mac_slave_0_ren;
  wire  adc_mac_slave_0_wen;
  wire  [64-1 : 0] adc_mac_slave_0_rdata;
  wire  [64-1 : 0] adc_mac_slave_0_wdata;
  wire  adc_mac_slave_0_waddr_error;
  wire  adc_mac_slave_0_raddr_error; 


  ccs_axi4lite2adc #(
    .ADDR_WIDTH(12),
    .DATA_WIDTH(64)
  ) adaptor_mac_slave_0
  (
     .ACLK (mac_slave_0_ACLK),
     .ARESETn (mac_slave_0_ARESETn),
     .AWADDR (mac_slave_0_AWADDR),
     .AWVALID (mac_slave_0_AWVALID),
     .AWREADY (mac_slave_0_AWREADY),
     .WDATA (mac_slave_0_WDATA),
     .WSTRB (mac_slave_0_WSTRB),
     .WVALID (mac_slave_0_WVALID),
     .WREADY (mac_slave_0_WREADY),
     .BRESP (mac_slave_0_BRESP),
     .BVALID (mac_slave_0_BVALID),
     .BREADY (mac_slave_0_BREADY),
     .ARADDR (mac_slave_0_ARADDR),
     .ARVALID (mac_slave_0_ARVALID),
     .ARREADY (mac_slave_0_ARREADY),
     .RDATA (mac_slave_0_RDATA),
     .RRESP (mac_slave_0_RRESP),
     .RVALID (mac_slave_0_RVALID),
     .RREADY (mac_slave_0_RREADY),
     .csr_addr (adc_mac_slave_0_addr),
     .csr_ren (adc_mac_slave_0_ren),
     .csr_wen (adc_mac_slave_0_wen),
     .csr_rdata (adc_mac_slave_0_rdata),
     .csr_wdata (adc_mac_slave_0_wdata),
     .csr_waddr_error (adc_mac_slave_0_waddr_error),
     .csr_raddr_error (adc_mac_slave_0_raddr_error)
  );

  // Drive interrupt ports

  // Instance of Catapult RTL design
  mac
  HLS_RTL
  (
    .clk (clk),
    .arst_n (arst_n),
    .f1_rsc_dat (f1_rsc_dat),
    .f1_triosy_lz (f1_triosy_lz),
    .f2_rsc_dat (f2_rsc_dat),
    .f2_triosy_lz (f2_triosy_lz),
    .a1_rsc_dat (a1_rsc_dat),
    .a1_triosy_lz (a1_triosy_lz),
    .result_rsc_dat (result_rsc_dat),
    .result_triosy_lz (result_triosy_lz)
  );
   

  // CSR Instances

  // mac_slave_0
  mac_slave_0
  #
  (
    .address_bits (12),
    .data_bits (64),
    .debug (debug)
  )
  CSR_0_mac_slave_0
  (
    .clk (clk),
    .arst_n (arst_n),
    .a1_rsc_dat (a1_rsc_dat),
    .f1_rsc_dat (f1_rsc_dat),
    .f2_rsc_dat (f2_rsc_dat),
    .result_rsc_dat (result_rsc_dat),
    .addr (adc_mac_slave_0_addr),
    .ren (adc_mac_slave_0_ren),
    .wen (adc_mac_slave_0_wen),
    .rdata (adc_mac_slave_0_rdata),
    .wdata (adc_mac_slave_0_wdata),
    .waddr_error (adc_mac_slave_0_waddr_error),
    .raddr_error (adc_mac_slave_0_raddr_error)
  );
  
endmodule