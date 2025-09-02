module dense_wrapper2 (                                                  

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
 
 wire         go; 
 wire         go_ready; 
 reg          go_valid; 
 wire         done; 
 reg          done_ready; 
 wire         done_valid; 
 wire [ 31:0] use_relu; 
 wire         use_relu_tz; 
 wire [ 31:0] addr_hi; 
 wire         addr_hi_tz; 
 wire [ 31:0] feature_addr; 
 wire         feature_addr_tz; 
 wire [ 31:0] weight_addr; 
 wire         weight_addr_tz; 
 wire [ 31:0] output_addr; 
 wire         output_addr_tz; 
 wire [ 31:0] in_vector_length; 
 wire         in_vector_length_tz; 
 wire [ 31:0] out_vector_length; 
 wire         out_vector_length_tz; 
 
 // register map 
 
 `define GO                     0 
 `define GO_READY               1 
 `define DONE                   2 
 `define DONE_VALID             3 
 `define USE_RELU               4
 `define ADDR_HI                5
 `define FEATURE_ADDR           6
 `define WEIGHT_ADDR            7
 `define OUTPUT_ADDR            8
 `define IN_VECTOR_LENGTH       9
 `define OUT_VECTOR_LENGTH     10
 
 // assignments for inputs 
 
 assign go                     = register_bank[`GO][0]; 
 assign use_relu               = register_bank[`USE_RELU]; 
 assign addr_hi                = register_bank[`ADDR_HI]; 
 assign feature_addr           = register_bank[`FEATURE_ADDR]; 
 assign weight_addr            = register_bank[`WEIGHT_ADDR]; 
 assign output_addr            = register_bank[`OUTPUT_ADDR]; 
 assign in_vector_length       = register_bank[`IN_VECTOR_LENGTH]; 
 assign out_vector_length      = register_bank[`OUT_VECTOR_LENGTH];
                                                                       
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
       register_bank[`GO]                 <= 32'h00000000; 
       register_bank[`USE_RELU]           <= 32'h00000000; 
       register_bank[`ADDR_HI]            <= 32'h00000000; 
       register_bank[`FEATURE_ADDR]       <= 32'h00000000; 
       register_bank[`WEIGHT_ADDR]        <= 32'h00000000; 
       register_bank[`OUTPUT_ADDR]        <= 32'h00000000; 
       register_bank[`IN_VECTOR_LENGTH]   <= 32'h00000000; 
       register_bank[`OUT_VECTOR_LENGTH]  <= 32'h00000000;
   end else begin                                                      
     if (write_enable) begin                                           
       if (write_address < 32) begin                                   
         register_bank[write_address] <= write_data;                   
       end                                                             
     end                                                               
       register_bank[`GO_READY] <= go_ready; 
       register_bank[`DONE] <= done; 
       register_bank[`DONE_VALID] <= done_valid; 
   end                                                                 
 end                                                                   
                                                                       
 
 always @(posedge clock or negedge resetn) begin 
    if (!resetn) begin 
       go_valid <= 1'b0; 
    end else begin 
       if (write_enable) begin 
          if (write_address == `GO) begin 
             go_valid <= 1'b1; 
          end 
       end 
       if (go_valid && go_ready) begin 
          go_valid <= 1'b0; 
       end 
    end 
 end 
 
 
 always @(posedge clock or negedge resetn) begin 
    if (!resetn) begin 
       done_ready <= 1'b0; 
    end else begin 
       if (read_enable) begin 
          if (read_address == `DONE) begin 
             done_ready <= 1'b1; 
          end 
       end 
       if (done_valid && done_ready) begin 
          done_ready <= 1'b0; 
       end 
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
 
 dense cat_accel ( 
    .clk (clock), 
    .arst_n (resetn), 
 
    .start_rsc_dat (go), 
    .start_rsc_vld (go_valid), 
    .start_rsc_rdy (go_ready), 
 
    .done_rsc_dat (done), 
    .done_rsc_vld (done_valid), 
    .done_rsc_rdy (done_ready), 
 
    .use_relu_rsc_dat             (use_relu), 
    .use_relu_triosy_lz           (use_relu_tz), 

    .addr_hi_rsc_dat              (addr_hi), 
    .addr_hi_triosy_lz            (addr_hi_tz), 

    .feature_addr_rsc_dat         (feature_addr), 
    .feature_addr_triosy_lz       (feature_addr_tz), 

    .weight_addr_rsc_dat          (weight_addr), 
    .weight_addr_triosy_lz        (weight_addr_tz), 

    .output_addr_rsc_dat          (output_addr), 
    .output_addr_triosy_lz        (output_addr_tz), 

    .in_vector_length_rsc_dat     (in_vector_length), 
    .in_vector_length_triosy_lz   (in_vector_length_tz), 

    .out_vector_length_rsc_dat    (out_vector_length), 
    .out_vector_length_triosy_lz  (out_vector_length_tz), 

    .memory_channels_aw_channel_rsc_dat  (axi_aw_data),
    .memory_channels_aw_channel_rsc_vld  (axi_aw_valid), 
    .memory_channels_aw_channel_rsc_rdy  (axi_aw_ready), 

    .memory_channels_w_channel_rsc_dat   (axi_w_data),
    .memory_channels_w_channel_rsc_vld   (axi_w_valid), 
    .memory_channels_w_channel_rsc_rdy   (axi_w_ready), 

    .memory_channels_b_channel_rsc_dat   (axi_b_data),
    .memory_channels_b_channel_rsc_vld   (axi_b_valid), 
    .memory_channels_b_channel_rsc_rdy   (axi_b_ready), 

    .memory_channels_ar_channel_rsc_dat  (axi_ar_data),
    .memory_channels_ar_channel_rsc_vld  (axi_ar_valid), 
    .memory_channels_ar_channel_rsc_rdy  (axi_ar_ready), 

    .memory_channels_r_channel_rsc_dat   (axi_r_data),
    .memory_channels_r_channel_rsc_vld   (axi_r_valid), 
    .memory_channels_r_channel_rsc_rdy   (axi_r_ready)
 ); 
endmodule 
