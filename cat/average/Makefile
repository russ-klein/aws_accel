
CATAPULT_HOME = /wv/hlsb/CATAPULT/2025.2/PRODUCTION/aol/Mgc_home

CAT           = $(CATAPULT_HOME)/bin/catapult
CAT_FLAGS    += -prod ultra
CAT_FLAGS    += -shell

CRUFT        += Catapult* 
CRUFT        += catapult.log

FUNCTION      = average

$(FUNCTION).v: $(FUNCTION).cpp $(FUNCTION).tcl 
	rm -rf $(CRUFT)_
	$(CAT) $(CAT_FLAGS) -file $(FUNCTION).tcl
	cp Catapult/$(FUNCTION).v1/concat_rtl.v $(FUNCTION).sv
	cp Catapult/$(FUNCTION).v1/rtl_csr_wrapper.v $(FUNCTION)_wrapper.sv
	cp Catapult/$(FUNCTION).v1/rtl_csr_blocks.v rtl_csr_blocks.sv
	cp $(CATAPULT_HOME)/pkgs/sif/userware/En_na/flows/ccs_axi4lite2adc.v ccs_axi4lite2adc.sv

clean:
	rm -rf $(CRUFT)
	rm -rf $(FUNCTION).sv
	rm -rf $(FUNCTION)_wrapper.sv
	rm -rf rtl_csr_blocks.sv
	rm -rf ccs_axi4lite2adc.sv
