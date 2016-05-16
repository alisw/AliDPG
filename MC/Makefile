test_pp: test_Pythia8	
test_PbPb: test_Hijing	

test_Pythia8: clean
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 225106 --mode ocdb
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 225106 --mode full --nevents 2 --uid 1 --generator Pythia8

test_Rsn001: clean
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 225106 --mode ocdb
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 225106 --mode full --nevents 2 --uid 1 --generator Pythia8_Monash2013_Rsn001

test_EPOSLHC_pp: clean
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 225106 --mode ocdb
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 225106 --mode full --nevents 2 --uid 1 --generator EPOSLHC_pp

test_Hijing: clean
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 245064 --mode ocdb
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim --run 245064 --mode full --nevents 2 --uid 1 --generator --generator Hijing

clean:
	rm -rf *~ *.root *.log *.inp *.dat stress* validation* GRP input
