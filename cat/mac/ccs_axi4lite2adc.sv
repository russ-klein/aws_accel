// placeholder for ccs_axi4lite2adc Verilog implementation

`define AXI4_xRESP_OKAY           2'b00
`define AXI4_xRESP_EXOKAY         2'b01
`define AXI4_xRESP_SLVERR         2'b10
`define AXI4_xRESP_DECERR         2'b11

module ccs_axi4lite2adc ( ACLK, ARESETn,
    AWADDR, AWVALID, AWREADY, 
    WDATA, WSTRB, WVALID, WREADY,
    BRESP, BVALID, BREADY,
    ARADDR, ARVALID, ARREADY,
    RDATA, RRESP, RVALID, RREADY,
    csr_addr, csr_ren, csr_rdata, csr_wen, csr_wdata, csr_waddr_error, csr_raddr_error);

  parameter ADDR_WIDTH = 12;
  parameter DATA_WIDTH = 32;

  input                       ACLK;        // Rising edge clock
  input                       ARESETn;     // Active LOW asynchronous reset
  input [ADDR_WIDTH-1:0]      AWADDR;      // Write address
  input                       AWVALID;     // Write address valid
  output                      AWREADY;     // Write address ready
  input [DATA_WIDTH-1:0]      WDATA;       // Write data
  input [DATA_WIDTH/8 - 1:0]  WSTRB;       // Write strobe (bytewise) - not used in LITE 
  input                       WVALID;      // Write data is valid
  output                      WREADY;      // Write ready
  output [1:0]                BRESP;       // Write response (of slave)
  output                      BVALID;      // Write response valid
  input                       BREADY;      // Response ready
  input [ADDR_WIDTH-1:0]      ARADDR;      // Read address
  input                       ARVALID;     // Read address valid
  output                      ARREADY;     // Read address ready
  output [DATA_WIDTH-1:0]     RDATA;       // Read data 
  output [1:0]                RRESP;       // Write response (of slave)
  output                      RVALID;      // Read data valid
  input                       RREADY;      // Read for read data

  output [ADDR_WIDTH-1:0]     csr_addr;
  output                      csr_ren;
  input  [DATA_WIDTH-1:0]     csr_rdata;
  output                      csr_wen;
  output [DATA_WIDTH-1:0]     csr_wdata;
  input                       csr_waddr_error;
  input                       csr_raddr_error;

  reg                         AWREADY_reg;     // Write address ready
  reg                         WREADY_reg;      // Write ready
  reg    [1:0]                BRESP_reg;       // Write response (of slave)
  reg                         BVALID_reg;      // Write response valid
  reg                         ARREADY_reg;     // Read address ready
  reg    [DATA_WIDTH-1:0]     RDATA_reg;       // Read data 
  reg    [1:0]                RRESP_reg;       // Write response (of slave)
  reg                         RVALID_reg;      // Read data valid
  reg    [ADDR_WIDTH-1:0]     csr_addr_reg;
  reg                         csr_ren_reg;
  reg                         csr_wen_reg;
  reg    [DATA_WIDTH-1:0]     csr_wdata_reg;

  assign AWREADY = AWREADY_reg;
  assign WREADY = WREADY_reg;
  assign BRESP = BRESP_reg;
  assign BVALID = BVALID_reg;
  assign ARREADY = ARREADY_reg;
  assign RDATA = RDATA_reg;
  assign RRESP = RRESP_reg;
  assign RVALID = RVALID_reg;
  assign csr_addr = csr_addr_reg;
  assign csr_ren = csr_ren_reg;
  assign csr_wen = csr_wen_reg;
  assign csr_wdata = csr_wdata_reg;

  parameter [2:0] s_idle=0, s_read=1, s_read_wait=2, s_write=3, s_write_wait=4;
  reg [2:0]                           state_var;

  // Drive AWREADY/ARREADY based on state:
  // If idle, these signals follow the requests from the master (pre-emptive AxREADY)
  // If not idle, then disallow any new write/read address requests.
  // pre_emptive_addr_ready
  always@(AWVALID or ARVALID or state_var)
  begin
    AWREADY_reg = 1'b0; // by default model is not ready to receive new write/read address
    ARREADY_reg = 1'b0;
    if (state_var == s_idle)
    begin
      AWREADY_reg = AWVALID; // pre-emptive write address transaction
      ARREADY_reg = ARVALID & ~AWVALID; // pre-emptive read allowed only if not writing
    end
  end

  // fsm_seq
  always@(ACLK or ARESETn or WVALID)
  begin
    if (ARESETn == 1'b0)
    begin
      state_var = s_idle;
		csr_addr_reg <= 0;
		csr_ren_reg <= 0;
		csr_wen_reg <= 0;
		csr_wdata_reg <= 0;
		WREADY_reg <= 0;
		BRESP_reg <= `AXI4_xRESP_OKAY;
		BVALID_reg <= 0;
		RDATA_reg <= 0;
		RVALID_reg <= 0;
    end
    else
    begin
      if (ACLK == 1'b1)
      begin
        csr_ren_reg <= 1'b0;
        csr_wen_reg <= 1'b0;
        case (state_var)
          s_idle : 
          begin
            RVALID_reg <= 1'b0; // slave is NOT providing RDATA
            WREADY_reg <= 1'b0; // slave is NOT ready for WDATA
            BVALID_reg <= 1'b0; // 
            RRESP_reg  <= `AXI4_xRESP_OKAY;
            BRESP_reg  <= `AXI4_xRESP_OKAY;

            // ---------------------------------------------
            // Check for AXI write request
            if (AWVALID == 1'b1)
            begin
              state_var     = s_write;
              csr_addr_reg <= AWADDR; 
              WREADY_reg <= 1'b1;
            end
            else
            begin
            // ---------------------------------------------
            // Check for AXI read request
              if (ARVALID == 1'b1)
              begin
                state_var = s_read;
                csr_addr_reg <= ARADDR; 
					 csr_ren_reg <= 1'b1;
              end
            end
          end // s_idle

          s_read : 
          begin
            state_var = s_read_wait; // one cycle for read
          end

          s_read_wait : 
          begin
            if (RREADY == 1'b1) // Master acknowledged data. Done
            begin
              RVALID_reg  <= 1'b1; // mark read as valid (RDATA driven for read process)
				  RRESP_reg <= {csr_raddr_error,1'b0};
              state_var = s_idle;
            end
            RDATA_reg <= csr_rdata;
          end // s_read

          s_write : 
          begin
            if (WVALID == 1'b1)
            begin
              WREADY_reg <= 1'b0;
				  csr_wdata_reg <= WDATA;
				  csr_wen_reg <= 1'b1;
              state_var  = s_write_wait;
            end
          end // s_write

          s_write_wait : 
          begin
            if (BREADY == 1'b1)
            begin
              BVALID_reg <= 1'b1;
				  BRESP_reg <= {csr_waddr_error,1'b0};
              state_var = s_idle;
            end
          end // s_write_wait
        endcase
      end // if aclk
    end // if aresetn
  end // always

endmodule

