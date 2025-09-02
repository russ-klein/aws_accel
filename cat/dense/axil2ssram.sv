module axi_lite_to_ssram #(
    parameter DATA_WIDTH = 32,
    parameter ADDR_WIDTH = 16
)(
    input  wire                   ACLK,
    input  wire                   ARESETN,

    // AXI Lite Write Address Channel
    input  wire [ADDR_WIDTH-1:0]  AWADDR,
    input  wire                   AWVALID,
    output wire                   AWREADY,

    // AXI Lite Write Data Channel
    input  wire [DATA_WIDTH-1:0]  WDATA,
    input  wire [(DATA_WIDTH/8)-1:0] WSTRB,
    input  wire                   WVALID,
    output wire                   WREADY,

    // AXI Lite Write Response Channel
    output reg  [1:0]             BRESP,
    output reg                    BVALID,
    input  wire                   BREADY,

    // AXI Lite Read Address Channel
    input  wire [ADDR_WIDTH-1:0]  ARADDR,
    input  wire                   ARVALID,
    output wire                   ARREADY,

    // AXI Lite Read Data Channel
    output reg  [DATA_WIDTH-1:0]  RDATA,
    output reg  [1:0]             RRESP,
    output reg                    RVALID,
    input  wire                   RREADY,

    // SSRAM Write Interface
    output reg  [ADDR_WIDTH-3:0]  ssram_write_addr,
    output reg  [DATA_WIDTH-1:0]  ssram_write_data,
    output reg                    ssram_write_en,
    output reg  [DATA_WIDTH/8-1:0] ssram_write_strb,

    // SSRAM Read Interface
    output reg  [ADDR_WIDTH-3:0]  ssram_read_addr,
    input  wire [DATA_WIDTH-1:0]  ssram_read_data,
    output reg                    ssram_read_en
);

    // Internal registers to track write address/data readiness
    reg aw_ready_reg;
    reg w_ready_reg;
    reg [ADDR_WIDTH-1:0] awaddr_reg;
    reg [DATA_WIDTH-1:0] wdata_reg;
    reg [DATA_WIDTH/8-1:0] wstrb_reg;

    assign AWREADY = !aw_ready_reg;
    assign WREADY  = !w_ready_reg;
    assign ARREADY = 1'b1;

    // Register AWADDR
    always_ff @(posedge ACLK) begin
        if (!ARESETN) begin
            aw_ready_reg <= 1'b0;
            awaddr_reg   <= {ADDR_WIDTH{1'b0}};
        end else if (AWVALID && AWREADY) begin
            awaddr_reg   <= AWADDR;
            aw_ready_reg <= 1'b1;
        end else if (BVALID && BREADY) begin
            aw_ready_reg <= 1'b0;
        end
    end

    // Register WDATA
    always_ff @(posedge ACLK) begin
        if (!ARESETN) begin
            w_ready_reg <= 1'b0;
            wdata_reg   <= {DATA_WIDTH{1'b0}};
            wstrb_reg   <= '0;
        end else if (WVALID && WREADY) begin
            wdata_reg   <= WDATA;
            wstrb_reg   <= WSTRB;
            w_ready_reg <= 1'b1;
        end else if (BVALID && BREADY) begin
            w_ready_reg <= 1'b0;
        end
    end

    // Write logic: trigger when both address and data are registered
    always_ff @(posedge ACLK) begin
        if (!ARESETN) begin
            ssram_write_en   <= 1'b0;
            ssram_write_data <= {DATA_WIDTH{1'b0}};
            ssram_write_addr <= {ADDR_WIDTH-3{1'b0}};
            BRESP            <= 2'b00;
            BVALID           <= 1'b0;
        end else begin
            if (aw_ready_reg && w_ready_reg && !BVALID) begin
                if (awaddr_reg[1:0] != 2'b00) begin
                    BRESP            <= 2'b10; // SLVERR
                    ssram_write_en   <= 1'b0;
                end else begin
                    ssram_write_addr <= awaddr_reg[ADDR_WIDTH-1:2];
                    ssram_write_data <= wdata_reg;
                    ssram_write_strb <= wstrb_reg;
                    ssram_write_en   <= 1'b1;
                    BRESP            <= 2'b00; // OKAY
                end
                BVALID <= 1'b1;
            end else begin
                ssram_write_en <= 1'b0;
                if (BVALID && BREADY)
                    BVALID <= 1'b0;
            end
        end
    end

    // Read logic

    assign ssram_read_addr = ARADDR[ADDR_WIDTH-1:2];
    assign RDATA = ssram_read_data;
    assign RRESP = (ARADDR[1:0] == 2'b0) ? 2'b00 : 2'b10;

    always @(posedge ACLK) begin
      if (!ARESETN) RVALID <= 0;
      else          RVALID <= ssram_read_en;
    end

    always_ff @(posedge ACLK) begin
        if (!ARESETN) begin
            ssram_read_en   <= 1'b0;
            // ssram_read_addr <= {ADDR_WIDTH-3{1'b0}};
        end else begin
            if (ARVALID & ARREADY) begin
                if (ARADDR[1:0] != 2'b00) begin
                    ssram_read_en   <= 1'b0;
                end else begin
                    // ssram_read_addr <= ARADDR[ADDR_WIDTH-1:2];
                    ssram_read_en   <= 1'b1;
                end
            end else begin
                ssram_read_en <= 1'b0;
            end
        end
    end

endmodule
