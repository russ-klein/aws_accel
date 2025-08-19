module testbench;

  // Parameters
  localparam ADDR_WIDTH = 12;
  localparam DATA_WIDTH = 32;
  localparam NUM_TRANSACTIONS = 32;

  // Clock and active-low reset
  logic clk;
  logic rst_n;

  // AXI Lite signals
  logic [ADDR_WIDTH-1:0] awaddr;
  logic awvalid;
  logic awready;

  logic [DATA_WIDTH-1:0] wdata;
  logic [3:0] wstrb;
  logic wvalid;
  logic wready;

  logic bvalid;
  logic bready;
  logic [1:0] bresp;

  logic [ADDR_WIDTH-1:0] araddr;
  logic arvalid;
  logic arready;

  logic [DATA_WIDTH-1:0] rdata;
  logic rvalid;
  logic rready;
  logic [1:0] rresp;

  logic [108:0]  axi_aw_data;
  logic          axi_aw_valid;
  logic          axi_aw_ready;
  logic [576:0]  axi_w_data;
  logic          axi_w_valid;
  logic          axi_w_ready;
  logic [17:0]   axi_b_data;
  logic          axi_b_valid;
  logic          axi_b_ready;
  logic [108:0]  axi_ar_data;
  logic          axi_ar_valid;
  logic          axi_ar_ready;
  logic [530:0]  axi_r_data;
  logic          axi_r_valid;
  logic          axi_r_ready;

  logic [9:0]    sram_read_address;
  logic [511:0]  sram_read_data;
  logic          sram_output_enable;

  logic [9:0]    sram_write_address;
  logic [511:0]  sram_write_data;
  logic [63:0]   sram_write_byte_enable;
  logic          sram_write_enable;

  // Clock generation
  always #5 clk = ~clk;

  task write_to_csr(int addr, data);

    awaddr = addr;
    awvalid = 1;

    @(posedge clk);
    while (!awready) @(posedge clk);
    awvalid = 0;

    wdata = data;
    wvalid = 1;

    @(posedge clk);
    while (!wready) @(posedge clk);
    wvalid = 0;

    bready = 1;
 
    @(posedge clk);
    while (!bvalid) @(posedge clk);
    bready = 0;

    @(posedge clk);

  endtask

  task read_from_csr(int addr, output int data);

    araddr = addr;
    arvalid = 1;

    @(posedge clk);
    while (!arready) @(posedge clk);
    arvalid = 0;

    rready = 1;
    
    @(posedge clk);
    while (!rvalid) @(posedge clk);
    data = rdata;
    rready = 0;

  endtask

  logic [31:0] return_data;
  logic ready_to_go;
  logic done;

  // Test procedure
  initial begin
    clk = 0;
    rst_n = 0;  // Assert reset (active-low)
    awvalid = 0;
    wvalid = 0;
    bready = 1;
    arvalid = 0;
    rready = 1;
    wstrb = 4'b1111;

    awaddr = '0;
    araddr = '0;

    #20 rst_n = 1;  // Deassert reset

    // Write phase
    for (int i = 8; i < NUM_TRANSACTIONS; i++) begin
      write_to_csr(i*4, i);
      repeat(2) @(posedge clk);
    end

    // Read and compare phase
    for (int i = 8; i < NUM_TRANSACTIONS; i++) begin
      read_from_csr(i*4, return_data);

      if (return_data !== i) begin
        $display("ERROR: Mismatch at address %0h. Expected %0h, got %0h", araddr, i, return_data);
      end else begin
        $display("PASS: Address %0h matched expected value %0h", i, return_data);
      end
      repeat (2) @(posedge clk);
    end

    $display("RW Test completed.");

    // set input parameters

    write_to_csr(4 * 4, 32); // add up 32 numbers
    write_to_csr(5 * 4, 0);  // addr lo
    write_to_csr(6 * 4, 0);  // addr hi

    $display("Set inputs");

    // start the process

    read_from_csr(1 * 4, ready_to_go);
    write_to_csr(0 * 4, 1);

    while (!ready_to_go) read_from_csr(1 * 4, ready_to_go);
    write_to_csr(0 * 4, 0);

    $display("computation started ");

    // wait for completion

    read_from_csr(3 * 4, done);

    while (!done) read_from_csr(3 * 4, done);
    write_to_csr(2 * 4, 1);

    $display("computation done "); 
    $finish;
  end

  // instantiate DUT

  average_wrap2 u_average_wrap(

  .clock       (clk),
  .resetn      (rst_n),

  // AXI Lite slave signals

  .axil_aw_valid  (awvalid),
  .axil_aw_ready  (awready),
  .axil_aw_addr   (awaddr),

  .axil_w_valid   (wvalid),
  .axil_w_ready   (wready),
  .axil_w_data    (wdata),
  .axil_w_strb    (wstrb),

  .axil_b_valid   (bvalid),
  .axil_b_ready   (bready),
  .axil_b_resp    (bresp),

  .axil_ar_valid  (arvalid),
  .axil_ar_ready  (arready),
  .axil_ar_addr   (araddr),

  .axil_r_valid   (rvalid),
  .axil_r_ready   (rready),
  .axil_r_data    (rdata),
  .axil_r_resp    (rresp),

  // AXI master from accelerator

  .axi_aw_data    (axi_aw_data),
  .axi_aw_valid   (axi_aw_valid),
  .axi_aw_ready   (axi_aw_ready),

  .axi_w_data     (axi_w_data),
  .axi_w_valid    (axi_w_valid),
  .axi_w_ready    (axi_w_ready),

  .axi_b_data     (axi_b_data),
  .axi_b_valid    (axi_b_valid),
  .axi_b_ready    (axi_b_ready),

  .axi_ar_data    (axi_ar_data),
  .axi_ar_valid   (axi_ar_valid),
  .axi_ar_ready   (axi_ar_ready),

  .axi_r_data     (axi_r_data),
  .axi_r_valid    (axi_r_valid),
  .axi_r_ready    (axi_r_ready)
  );


  axi_slave_if #(.p_size(6), .b_size(6)) sram_if (
    .ACLK        (clk),
    .ARESETn     (rst_n),

    .AWMASTER    (4'b0),
    .AWID        (axi_aw_data[108:93]),
    .AWADDR      (axi_aw_data[44:29]),  // 92-29
    .AWLEN       (axi_aw_data[28:21]),
    .AWSIZE      (axi_aw_data[20:18]),
    .AWBURST     (axi_aw_data[17:16]),
    .AWLOCK      (axi_aw_data[15]),
    .AWCACHE     (axi_aw_data[14:11]),
    .AWPROT      (axi_aw_data[10:8]),
    .AWVALID     (axi_aw_valid),
    .AWREADY     (axi_aw_ready),

    .WMASTER     (4'b0),
    .WID         (16'b0),
    .WDATA       (axi_w_data[576:65]),
    .WSTRB       (axi_w_data[64:1]),
    .WLAST       (axi_w_data[0]),
    .WVALID      (axi_w_valid),
    .WREADY      (axi_w_ready),

    .BMASTER     ( ),
    .BID         (axi_b_data[17:2]),
    .BRESP       (axi_b_data[1:0]),
    .BVALID      (axi_b_valid),
    .BREADY      (axi_b_ready),

    .ARMASTER    (4'b0),
    .ARID        (axi_ar_data[108:93]),
    .ARADDR      (axi_ar_data[44:29]),  // 92-29
    .ARLEN       (axi_ar_data[28:21]),
    .ARSIZE      (axi_ar_data[20:18]),
    .ARBURST     (axi_ar_data[17:16]),
    .ARLOCK      (axi_ar_data[15]),
    .ARCACHE     (axi_ar_data[14:11]),
    .ARPROT      (axi_ar_data[10:8]),
    .ARVALID     (axi_ar_valid),
    .ARREADY     (axi_ar_ready),

    .RMASTER     ( ),
    .RID         (axi_r_data[530:515]),
    .RDATA       (axi_r_data[514:3]),
    .RRESP       (axi_r_data[2:1]),
    .RLAST       (axi_r_data[0]),
    .RVALID      (axi_r_valid),
    .RREADY      (axi_r_ready),

// SRAM interface

    .SRAM_READ_ADDRESS       (sram_read_address),
    .SRAM_READ_DATA          (sram_read_data),
    .SRAM_OUTPUT_ENABLE      (sram_output_enable),

    .SRAM_WRITE_ADDRESS      (sram_write_address),
    .SRAM_WRITE_DATA         (sram_write_data),
    .SRAM_WRITE_BYTE_ENABLE  (sram_write_byte_enable),
    .SRAM_WRITE_STROBE       (sram_write_enable)
  );

  sram u_sram(
    .CLK          (clk),
    .RSTn         (rst_n),
    .READ_ADDR    (sram_read_address),
    .DATA_OUT     (sram_read_data),
    .OE           (sram_output_enable),
    .WRITE_ADDR   (sram_write_address),
    .DATA_IN      (sram_write_data),
    .BE           (sram_write_byte_enable),
    .WE           (sram_write_enable)
  );

endmodule

