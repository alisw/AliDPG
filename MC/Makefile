RUN     = 117222
NEVENTS = 1
MODE    = "sim,rec"
EXTRA   = "--bmin 14.5 --bmax 15.5 --pthardbin 1"

OBJECTS = Pythia6_Perugia2011 \
	  Pythia8_Monash2013 \
	  Pythia8Jets_Monash2013 \
	  Phojet \
	  EPOSLHC \
	  Hijing

SOURCEDIR   = $(ALIDPG_ROOT)/MC/CustomGenerators/
SOURCES    := $(shell find $(SOURCEDIR) -name '*.C')
OBJECTS    += $(SOURCES:$(SOURCEDIR)%.C=%)

all: $(OBJECTS)

$(OBJECTS): OCDBsim.root OCDBrec.root
	$(eval GEN := $(subst /,:,$@))
	@echo "=== Running test simulation run ${RUN} with generator ${GEN} ==="
	@mkdir -p $@
	@ln -s $(PWD)/OCDBsim.root $@/.
	@ln -s $(PWD)/OCDBrec.root $@/.
	@cd $@ && $(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --generator ${GEN} --mode ${MODE} --nevents ${NEVENTS} ${EXTRA} &> dpgsim.log && echo " [SUCCESS] Running test simulation run ${RUN} with generator ${GEN}" || echo " [FAILURE] Running test simulation run ${RUN} with generator ${GEN}"

OCDBsim.root:
	@echo "=== Running OCDB snapshot creation for run ${RUN} ==="
	@$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --mode ocdb &> snapshot.log && echo " [SUCCESS] Running OCDB snapshot creation for run ${RUN}" || echo " [FAILURE] Running OCDB snapshot creation for run ${RUN}"

OCDBrec.root:
	@echo "=== Running OCDB snapshot creation for run ${RUN} ==="
	@$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --mode ocdb &> snapshot.log && echo " [SUCCESS] Running OCDB snapshot creation for run ${RUN}" || echo " [FAILURE] Running OCDB snapshot creation for run ${RUN}"

clean:
	rm -rf *~ *.log validation_error.message *.xml $(OBJECTS)
