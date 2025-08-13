// ////////////////////////////////////////////////////////
// File: rtl_csr_blocks.v
//
// Contents:
//    Memory Mapped regsiters for interfaces 

// ////////////////////////////////////////////////////////
// Design Unit: mac_slave_0
// Description: Implements memory-mapped register

module mac_slave_0 
(
  clk, 
  arst_n, 
  a1_rsc_dat, 
  f1_rsc_dat, 
  f2_rsc_dat, 
  result_rsc_dat, 
  
  addr, 
  ren, 
  rdata, 
  wen, 
  wdata, 
  waddr_error, 
  raddr_error 
); 

  parameter integer address_bits = 12; // address width of bus
  parameter integer data_bits = 32; // data width of bus
  parameter integer debug = 0; // enable sim debug messages (0=silent,1=errors,2=verbose)

  input  clk;
  input  arst_n;
  output [32-1 : 0] a1_rsc_dat;
  output [32-1 : 0] f1_rsc_dat;
  output [32-1 : 0] f2_rsc_dat;
  input [32-1 : 0] result_rsc_dat;
     // External MMR Interface
  input [12-1 : 0] addr;   // Common Read/Write Address
  input  ren;   // Read Enable
  output [32-1 : 0] rdata;   // Read Data
  input  wen;   // Write Enable
  input [32-1 : 0] wdata;   // Write Data
  output  waddr_error;   // Out of bounds address error
  output  raddr_error;   // Out of bounds address error

  // Flags
  reg reg_waddr_error;
  reg reg_raddr_error;

  reg [data_bits-1 : 0] tmp_rdata;

  // Configuration Input Registers 
  reg [32-1 : 0] reg_a1_rsc_dat;   
  reg [32-1 : 0] reg_f1_rsc_dat;   
  reg [32-1 : 0] reg_f2_rsc_dat;   
  reg [32-1 : 0] reg_result_rsc_dat;  

  // ============================================================
  // Host side

  always @ (posedge clk or negedge arst_n)
  begin: write_proc
    if (arst_n == 1'b0)
    begin
      // RESET ACTION
      reg_waddr_error <= 1'b0; 
      reg_a1_rsc_dat <= 32'd0; // requested reset value 0 
      reg_f1_rsc_dat <= 32'd0; // requested reset value 0 
      reg_f2_rsc_dat <= 32'd0; // requested reset value 0 
    end
    else
    begin
      // DEFAULT ASSIGNMENTS (may be overwritten below)
      reg_waddr_error <= 1'b0;    
      if (wen == 1'b1)
      begin
        if (debug > 2'd1)
          $display ("Write event: addr = 0x%h  wdata = 0x%h", addr, wdata);

        case(addr)
          12'd0 : begin    // HADDR: X"0"   
              reg_f1_rsc_dat[31 : 0] <= wdata[31 : 0]; // Register : f1_rsc_dat     
          end  
          12'd4 : begin    // HADDR: X"4"   
              reg_f2_rsc_dat[31 : 0] <= wdata[31 : 0]; // Register : f2_rsc_dat     
          end  
          12'd8 : begin    // HADDR: X"8"   
              reg_a1_rsc_dat[31 : 0] <= wdata[31 : 0]; // Register : a1_rsc_dat     
          end   
         12'd12 : begin    // HADDR: X"c"
            reg_waddr_error <= 1'b1; // write to read-only register, Register : result_rsc_dat
              if (debug > 2'd0)
                $display ("Write address addr = 0x%h  to read-only register", addr); 
          end  
          default : begin
            reg_waddr_error <= 1'b1; // write address out of bounds
            if (debug > 2'd0)
              $display ("Write address addr = 0x%h  out of range", addr);
          end
        endcase
      end
    end 
  end


  always @ (posedge clk or negedge arst_n)
  begin: read_mux
    if (arst_n == 1'b0)
    begin
      // RESET ACTION
      tmp_rdata <= 0;
      reg_raddr_error <= 1'b0;        
    end
    else
    begin
      reg_raddr_error <= 1'b0; // default assignment (may be overwritten below)    
      if (ren == 1'b1)
      begin
        case(addr)  
          12'd0 : begin     // HADDR: X"0"    
            tmp_rdata[31 : 0] <= reg_f1_rsc_dat[31 : 0]; // read back register value    
          end // close begin haddr   
          12'd4 : begin     // HADDR: X"4"    
            tmp_rdata[31 : 0] <= reg_f2_rsc_dat[31 : 0]; // read back register value    
          end // close begin haddr   
          12'd8 : begin     // HADDR: X"8"    
            tmp_rdata[31 : 0] <= reg_a1_rsc_dat[31 : 0]; // read back register value    
          end // close begin haddr   
          12'd12 : begin     // HADDR: X"c"   
            tmp_rdata[31 : 0] <= result_rsc_dat[31 : 0]; // read output value   
          end // close begin haddr 
          default : begin
            reg_raddr_error <= 1'b1; // read address out of bounds
            if (debug > 2'd0) 
              $display ("Read address addr = 0x%h  out of range", addr);
          end
        endcase
        if (debug > 2'd1)
          $display ("Read  event: addr = 0x%h  rdata = 0x%h", addr, tmp_rdata);
      end
    end
  end
  assign rdata = tmp_rdata;

  // Drive output flags
  assign waddr_error = reg_waddr_error;
  assign raddr_error = reg_raddr_error;

  // Drive Input Registers to design
  assign a1_rsc_dat = reg_a1_rsc_dat; 
  assign f1_rsc_dat = reg_f1_rsc_dat; 
  assign f2_rsc_dat = reg_f2_rsc_dat;  

endmodule
