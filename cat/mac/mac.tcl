solution file add ./mac.cpp

flow package require /BusSlaveGen

go analyze
go compile

solution library add mgc_Xilinx-VIRTEX-uplus-1_beh --          \
                         -rtlsyntool      Vivado               \
			 -manufacturer    Xilinx               \
			 -family          VIRTEX-uplus         \
			 -speed           -1                   \
			 -part            xcvu7p-flvc2104-1-i

solution library add Xilinx_RAMS
solution library add Xilinx_ROMS
solution library add Xilinx_FIFO

go libraries

directive set -CLOCKS {
  clk {
    -CLOCK_PERIOD          4.0 
    -CLOCK_EDGE         rising 
    -CLOCK_HIGH_TIME       2.0 
    -CLOCK_OFFSET     0.000000 
    -CLOCK_UNCERTAINTY     0.0 
    -RESET_KIND          async 
    -RESET_SYNC_NAME       rst 
    -RESET_SYNC_ACTIVE    high 
    -RESET_ASYNC_NAME   arst_n 
    -RESET_ASYNC_ACTIVE    low 
    -ENABLE_NAME            {} 
    -ENABLE_ACTIVE        high
   }
}

go assembly
go architect
go allocate
go extract
