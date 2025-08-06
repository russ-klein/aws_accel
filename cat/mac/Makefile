
CATAPULT_HOME = /wv/hlsb/CATAPULT/2025.2/PRODUCTION/aol/Mgc_home

CAT           = $(CATAPULT_HOME)/bin/catapult
CAT_FLAGS    += -prod ultra
CAT_FLAGS    += -shell

CRUFT        += Catapult* 
CRUFT        += catapult.log

FUNCTION      = mac

$(FUNCTION).v: $(FUNCTION).cpp $(FUNCTION).tcl 
	rm -rf $(CRUFT)_
	$(CAT) $(CAT_FLAGS) -file $(FUNCTION).tcl
	cp Catapult/mac.v1/concat_rtl.v $(FUNCTION).v
	cp Catapult/mac.v1/rtl_csr_wrapper.v $(FUNCTION)_wrapper.v

clean:
	rm -rf $(CRUFT)
	rm -rf $(FUNCTION).v
	rm -rf $(FUNCTION)_wrapper.v
