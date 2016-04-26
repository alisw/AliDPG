test_Pythia6: clean
	chmod +x dpgsim.sh
	./dpgsim.sh --run 225106 --uid 1 --generator Pythia6 --simulation Default --reconstruction Default --nevents 2 --mode simrec 

test_EPOSLHC_pp: clean
	chmod +x dpgsim.sh
	./dpgsim.sh --run 225106 --uid 1 --generator EPOSLHC_pp --simulation Default --reconstruction Default --nevents 2 --mode simrec 

test_Hijing: clean
	chmod +x dpgsim.sh
	./dpgsim.sh --run 245064 --uid 1 --generator Hijing --simulation Default --reconstruction Default --nevents 2 --mode simrec 

tarball:
	tar zcvf dpgsim.$(shell date +"%Y%m%d").tgz dpgsim.sh sim.C rec.C Config.C SimulationConfig.C ReconstructionConfig.C DetectorConfig.C GeneratorConfig.C EXTRA/gen_eposlhc_pp.sh Makefile

clean:
	rm -rf *~ *.root *.log *.inp *.dat stress* validation* GRP input