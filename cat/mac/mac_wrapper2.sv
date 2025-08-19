module average_wrap2 (                                                  

  input          clock,                                                
  input          resetn,                                               

  // AXI Lite slave signals

  input          axil_aw_valid,
  output         axil_aw_ready,
  input  [11:0]  axil_aw_addr,

  input          axil_w_valid,
  output         axil_w_ready,
  input  [31:0]  axil_w_data,
  input  [3:0]   axil_w_strb,

  output         axil_b_valid,
  input          axil_b_ready,
  output [1:0]   axil_b_resp,

  input          axil_ar_valid,
  output         axil_ar_ready,
  input  [11:0]  axil_ar_addr,

  output         axil_r_valid,
  input          axil_r_ready,
  output [31:0]  axil_r_data,
  output [1:0]   axil_r_resp,

  // AXI master signals

  output [108:0] axi_aw_data,
  output         axi_aw_valid,
  input          axi_aw_ready,

  output [576:0] axi_w_data,
  output         axi_w_valid,
  input          axi_w_ready,

  input  [17:0]  axi_b_data,
  input          axi_b_valid,
  output         axi_b_ready,

  output [108:0] axi_ar_data,
  output         axi_ar_valid,
  input          axi_ar_ready,

  input  [530:0] axi_r_data,
  input          axi_r_valid,
  output         axi_r_ready
);                                                                     
                                                                       
  logic  [9:0]   read_addr;                                            
  logic  [31:0]  read_data;                                            
  logic          oe;                                                   

  logic  [9:0]   write_addr;                                           
  logic  [31:0]  write_data;                                           
  logic  [3:0]   be;                                                   
  logic          we;         
                                                                       
 reg     [32:0]  register_bank[31:0];                                  
 reg     [32:0]  rd_reg;                                               
                                                                       
 reg             ready_out = 1'b1;                                     
 reg             resp_out = 2'b00;                                     
                                                                       
 wire    [11:0]  read_address;                                
 wire    [11:0]  write_address;                               
 wire            read_enable = oe;                                     
 wire            write_enable = we;                                    
                                                                       
 assign read_data = rd_reg;                                            
                                                                       
 assign read_address = read_addr;                                
 assign write_address = write_addr;                              
                                                                       
 
 // interface signals 
 
 wire [ 31:0] f1; 
 wire         f1_tz; 
 wire [ 31:0] f2; 
 wire         f2_tz; 
 wire [ 31:0] a1; 
 wire         a1_tz; 
 wire [ 31:0] result; 
 wire         result_tz; 
 
 // register map 
 
 `define F1                     0 
 `define F2                     1 
 `define A1                     2 
 `define RESULT                 4 
 
 
 // assignments for inputs 
 
 assign f1                   = register_bank[`F1]; 
 assign f2                   = register_bank[`F2]; 
 assign a1                   = register_bank[`A1]; 
                                                                       
 always @(posedge clock or negedge resetn) begin                       
   if (resetn == 1'b0) begin                                           
     rd_reg <= 32'h00000000;                                           
   end else begin                                                      
     if (read_enable) begin                                            
       rd_reg <= register_bank[read_address];                          
     end                                                               
   end                                                                 
 end                                                                   
                                                                       
 always @(posedge clock or negedge resetn) begin                       
   if (resetn == 1'b0) begin                                           
       register_bank[`F1] <= 32'h00000000; 
       register_bank[`F2] <= 32'h00000000; 
       register_bank[`A1] <= 32'h00000000; 
       register_bank[`RESULT] <= 32'h00000000; 
   end else begin                                                      
     if (write_enable) begin                                           
       if (write_address < 32) begin                                   
         register_bank[write_address] <= write_data;                   
       end                                                             
     end                                                               
     register_bank[`RESULT] <= result; 
   end                                                                 
 end                                                                   
                                                                       
 axi_lite_to_ssram #(.ADDR_WIDTH(12)) 
    u_axi_lite_to_ssram (

  .ACLK             (clock),
  .ARESETN          (resetn),

  .AWVALID          (axil_aw_valid),
  .AWREADY          (axil_aw_ready),
  .AWADDR           (axil_aw_addr),

  .WVALID           (axil_w_valid),
  .WREADY           (axil_w_ready),
  .WDATA            (axil_w_data),
  .WSTRB            (axil_w_strb),

  .BVALID           (axil_b_valid),
  .BREADY           (axil_b_ready),
  .BRESP            (axil_b_resp),

  .ARVALID          (axil_ar_valid),
  .ARREADY          (axil_ar_ready),
  .ARADDR           (axil_ar_addr),

  .RVALID           (axil_r_valid),
  .RREADY           (axil_r_ready),
  .RDATA            (axil_r_data),
  .RRESP            (axil_r_resp),

  .ssram_read_addr  (read_addr),
  .ssram_read_data  (read_data),
  .ssram_read_en    (oe),

  .ssram_write_addr (write_addr),
  .ssram_write_data (write_data),
  .ssram_write_strb (be),
  .ssram_write_en   (we)
 );
 
 mac cat_accel ( 
    .clk (clock), 
    .arst_n (resetn), 
 
    .count_rsc_dat (count), 
    .count_triosy_lz (count_tz), 
 
    .f1_rsc_dat (index_hi), 
    .f1_triosy_lz (index_hi_tz), 
 
    .f2_rsc_dat (index_hi), 
    .f2_triosy_lz (index_hi_tz), 
 
    .a1_rsc_dat (index_hi), 
    .a1_triosy_lz (index_hi_tz), 
 
    .result_rsc_dat (result), 
    .result_triosy_lz (result_tz)
 ); 

 // deassert AXI output bus -- unused in this design

 assign axi_aw_valid = 0;
 assign axi_aw_data = '0;
 
 assign axi_w_valid = 0;
 assign axi_w_data = '0;

 assign axi_b_ready = 0;

 assign axi_ar_valid = 0;
 assign axi_ar_data = '0;

 assign axi_r_ready = 0;

endmodule 
