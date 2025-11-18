
CATAPULT_HOME = /wv/hlsb/CATAPULT/2025.2/PRODUCTION/aol/Mgc_home
AWS_SDK       = /home/ubuntu/aws-fpga/sdk

CAT           = $(CATAPULT_HOME)/bin/catapult
CAT_FLAGS    += -prod ultra
CAT_FLAGS    += -shell

CRUFT        += Catapult* 
CRUFT        += catapult.log

FUNCTION      = dense

G++           = g++
GCC           = gcc
LD            = g++

#GCC_FLAGS    += -I $(CATAPULT_HOME)/shared/include
G++_FLAGS    += -I ../cat_include
G++_FLAGS    += -I ../axi_interface
G++_FLAGS    += -std=c++11
G++_FLAGS    += -D C_SIMULATION
G++_FLAGS    += -D AWS

GCC_FLAGS    += -D AWS
GCC_FLAGS    += -g
GCC_FLAGS    += -I $(AWS_SDK)/userspace/include/

OBJS         += testbench.o 
OBJS         += dense.o 
OBJS         += axi_master_if.o 
OBJS         += aws_dense.o
OBJS         += timer.o 

testbench: $(OBJS)
	$(LD) -o testbench $(OBJS) -lfpga_mgmt

testbench.o: testbench.cpp defines.h
	$(G++) $(G++_FLAGS) -c testbench.cpp

axi_master_if.o: ../axi_interface/axi_master_if.cpp defines.h
	$(G++) $(G++_FLAGS) -c ../axi_interface/axi_master_if.cpp

dense.o: dense.cpp defines.h
	$(G++) $(G++_FLAGS) -c dense.cpp

aws_dense.o: aws_dense.c
	$(GCC) $(GCC_FLAGS) -c aws_dense.c 

timer.o: timer.c
	$(GCC) $(GCC_FLAGS) -c timer.c

$(FUNCTION).sv: $(FUNCTION).cpp $(FUNCTION).tcl 
	rm -rf $(CRUFT)_
	$(CAT) $(CAT_FLAGS) -file $(FUNCTION).tcl
	cp Catapult/$(FUNCTION).v1/concat_rtl.v $(FUNCTION).sv
	#cp Catapult/$(FUNCTION).v1/rtl_csr_wrapper.v $(FUNCTION)_wrapper.sv
	#cp Catapult/$(FUNCTION).v1/rtl_csr_blocks.v rtl_csr_blocks.sv
	cp $(CATAPULT_HOME)/pkgs/sif/userware/En_na/flows/ccs_axi4lite2adc.v ccs_axi4lite2adc.sv

clean:
	rm -rf $(CRUFT)
	rm -rf $(FUNCTION).sv
	rm -rf $(FUNCTION)_wrapper.sv
	rm -rf rtl_csr_blocks.sv
	rm -rf ccs_axi4lite2adc.sv
	rm -rf $(OBJS)
	rm -rf testbench
