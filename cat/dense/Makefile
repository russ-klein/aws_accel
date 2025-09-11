
CATAPULT_HOME = /wv/hlsb/CATAPULT/2025.2/PRODUCTION/aol/Mgc_home

CAT           = $(CATAPULT_HOME)/bin/catapult
CAT_FLAGS    += -prod ultra
CAT_FLAGS    += -shell

CRUFT        += Catapult* 
CRUFT        += catapult.log

FUNCTION      = dense

GCC           = g++
LD            = g++

#GCC_FLAGS    += -I $(CATAPULT_HOME)/shared/include
GCC_FLAGS    += -I ../cat_include
GCC_FLAGS    += -I ../axi_interface
GCC_FLAGS    += -std=c++11
GCC_FLAGS    += -D C_SIMULATION
GCC_FLAGS    += -g

OBJS         += testbench.o 
OBJS         += dense.o 
OBJS         += axi_master_if.o 
#OBJS         += aws_dense.o
OBJS         += timer.o 

testbench: $(OBJS)
	$(LD) -o testbench $(OBJS)

testbench.o: testbench.cpp defines.h
	$(GCC) $(GCC_FLAGS) -c testbench.cpp

axi_master_if.o: ../axi_interface/axi_master_if.cpp defines.h
	$(GCC) $(GCC_FLAGS) -c ../axi_interface/axi_master_if.cpp

dense.o: dense.cpp defines.h
	$(GCC) $(GCC_FLAGS) -c dense.cpp

aws_dense.o: aws_dense.cpp
	$(GCC) $(GCC_FLAGS) -c aws_dense.cpp 

timer.o: timer.cpp
	$(GCC) $(GCC_FLAGS) -c timer.cpp

$(FUNCTION).sv: $(FUNCTION).cpp $(FUNCTION).tcl 
	rm -rf $(CRUFT)_
	$(CAT) $(CAT_FLAGS) -file $(FUNCTION).tcl
	cp Catapult/$(FUNCTION).v1/concat_rtl.v $(FUNCTION).sv
	#cp Catapult/$(FUNCTION).v1/rtl_csr_wrapper.v $(FUNCTION)_wrapper.sv
	#cp Catapult/$(FUNCTION).v1/rtl_csr_blocks.v rtl_csr_blocks.sv
	#cp $(CATAPULT_HOME)/pkgs/sif/userware/En_na/flows/ccs_axi4lite2adc.v ccs_axi4lite2adc.sv

clean:
	rm -rf $(CRUFT)
	rm -rf $(FUNCTION).sv
	rm -rf $(FUNCTION)_wrapper.sv
	rm -rf rtl_csr_blocks.sv
	rm -rf ccs_axi4lite2adc.sv
	rm -rf $(OBJS)
	rm -rf testbench
