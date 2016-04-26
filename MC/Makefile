test_pp: test_Pythia8

test_PbPb: test_Hijing

test_Pythia8: clean
	./createSnapshots.sh 225106
	./dpgsim.sh --run 225106 --uid 1 --generator Pythia8 --simulation Default --reconstruction Default --nevents 2 --mode simrec 

test_Pythia6: clean
	./createSnapshots.sh 225106
	./dpgsim.sh --run 225106 --uid 1 --generator Pythia6 --simulation Default --reconstruction Default --nevents 2 --mode simrec 

test_EPOSLHC_pp: clean
	./createSnapshots.sh 225106
	./dpgsim.sh --run 225106 --uid 1 --generator EPOSLHC_pp --simulation Default --reconstruction Default --nevents 2 --mode simrec 

test_Hijing: clean
	./createSnapshots.sh 245064
	./dpgsim.sh --run 245064 --uid 1 --generator Hijing --simulation Default --reconstruction Default --nevents 2 --mode simrec 

tarball:
	tar zcvf dpgsim.$(shell date +"%Y%m%d").tgz dpgsim.sh sim.C rec.C Config.C SimulationConfig.C ReconstructionConfig.C DetectorConfig.C GeneratorConfig.C ConfigOCDB.C CreateSnapshot.C createSnapshots.sh EXTRA/gen_eposlhc_pp.sh Makefile

clean:
	rm -rf *~ *.root *.log *.inp *.dat stress* validation* GRP input
