#!/bin/bash

# check ALIDPG_ROOT is set
if [ -z "$ALIDPG_ROOT" ]; then
    echo "ALIDPG_ROOT not set, abort."
    exit 1
fi

# check ALICER_ROOT is set
if [ -z "$ALICE_ROOT" ]; then
    echo "ALICE_ROOT not set, abort."
    exit 1
fi

# set job and simulation variables as :
COMMAND_HELP="./dpgsim.sh --mode <mode> --run <run> --generator <generatorConfig> --energy <energy> --system <system> --detector <detectorConfig> --magnet <magnetConfig> --simulation <simulationConfig> --reconstruction <reconstructionConfig> --uid <uniqueID> --nevents <numberOfEvents> --qa <qaConfig> --aod <aodConfig> --ocdb <ocdbConfig> --hlt <hltConfig>"

function runcommand(){
    echo -e "\n"
    echo -e "\n" >&2

    echo "* $1 : $2"
    echo "* $1 : output log in $3"
    echo "* $1 : $2" >&2
    echo "* $1 : output log in $3" >&2

    START=`date "+%s"`
    time aliroot -b -q -x $2 >>$3 2>&1
    exitcode=$?
    END=`date "+%s"`
    echo "$1 TIME: $((END-START))"
    
    expectedCode=${5-0}

    # check exit code
    if [ "$exitcode" -ne "$expectedCode" ]; then
        echo "*! $2 failed with exitcode $exitcode, expecting $expectedCode"
        echo "*! $2 failed with exitcode $exitcode, expecting $expectedCode" >&2
        echo "$2 failed with exitcode $exitcode, expecting $expectedCode" >> validation_error.message
        exit ${4-$exitcode}
    fi

    # check potential sign of errors in the log file
    errorStrings=(
	"*** Interpreter error recovered ***" 
	"*** Break *** segmentation violation" 
	"*** Break *** floating point exception" 
	"*** Break *** bus error")
   
    for I in "${errorStrings[@]}"; do
	grep -F "$I" $3 > /dev/null;
	if [ "$?" -ne "1" ]; then
            echo "*! $2 failed, detected error in $3: ""$I"
            echo "*! $2 failed, detected error in $3: ""$I" >&2
            echo "$2 failed, detected error in $3: ""$I" >> validation_error.message
            exit ${4-$exitcode}
	fi
    done

    # all right, success
    echo "* $2 finished with the expected exit code ($expectedCode), moving on"
    echo "* $2 finished with the expected exit code ($expectedCode), moving on" >&2

}

function runBenchmark(){
    if [ ! -x "$ROOTSYS/test/stressHepix" ]; then
        (cd "$ROOTSYS/test" && make) &>/dev/null
    fi

    if [ ! -x "$ROOTSYS/test/stressHepix" ]; then
        echo "ERROR: No stressHepix in this ROOT package : $ROOTSYS"
    else
        $ROOTSYS/test/stress -b &> /dev/null
        $ROOTSYS/test/stress -b &> stress.output
        FOOTER=`cat stress.output | tail -n6`

        ROOTMARKS=`echo "$FOOTER" | grep ROOTMARKS  | cut -d= -f2 | cut '-d*' -f1`

        ROOTVER=`echo "$FOOTER" | grep ROOTMARKS  | cut -d= -f2 | cut '-d*' -f2`

        read REALTIME CPUTIME < <(echo "$FOOTER" | grep "Real Time" | awk '{print $6 " " $11}')

        if [ -z "$ROOTMARKS" ]; then
            echo "ERROR: empty output of stressHepix"
        else
            echo "ROOTMARKS : $ROOTMARKS"
            echo "ROOTVER : $ROOTVER"
            echo "Real time : $REALTIME"
            echo "CPU time : $CPUTIME"
        fi

        rm stress.output
    fi

    grep "model name" /proc/cpuinfo | uniq -c
    
    if [ -x /cvmfs/atlas.cern.ch/repo/benchmarks/htcondor/x86_64/slc6/stripped/8.2.8/libexec/condor_kflops ]; then
        /cvmfs/atlas.cern.ch/repo/benchmarks/htcondor/x86_64/slc6/stripped/8.2.8/libexec/condor_kflops &>/dev/null
        /cvmfs/atlas.cern.ch/repo/benchmarks/htcondor/x86_64/slc6/stripped/8.2.8/libexec/condor_kflops
    else
        echo "kFlops executable is not available on `hostname -f`"
    fi
    
    if [ -f lhcb.py ]; then
        python lhcb.py
    fi
}

CONFIG_NEVENTS="200"
CONFIG_NBKG=""
CONFIG_BGEVDIR=""
CONFIG_SEED="0"
CONFIG_GENERATOR=""
CONFIG_BACKGROUND=""
CONFIG_PROCESS=""
CONFIG_PROCESSBIN=""
CONFIG_MAGNET=""
CONFIG_ENERGY=""
OVERRIDE_ENERGY=""
CONFIG_SYSTEM=""
OVERRIDE_SYSTEM=""
CONFIG_TRIGGER=""
OVERRIDE_TRIGGER=""
CONFIG_DETECTOR=""
CONFIG_DETECTORMASK="0x0"
CONFIG_PHYSICSLIST=""
CONFIG_BMIN=""
CONFIG_BMAX=""
CONFIG_YMIN=""
CONFIG_YMAX=""
CONFIG_PTHARDBIN=""
CONFIG_PTHARDMIN=""
CONFIG_PTHARDMAX=""
CONFIG_QUENCHING=""
CONFIG_QHAT=""
CONFIG_RUN=""
CONFIG_UID="1"
CONFIG_SIMULATION=""
CONFIG_RECONSTRUCTION=""
CONFIG_QA=""
CONFIG_AOD=""
CONFIG_MODE="ocdb,full"
CONFIG_OCDB="snapshot"
CONFIG_HLT=""
CONFIG_GEANT4=""
CONFIG_MATERIAL=""

RUNMODE=""

while [ ! -z "$1" ]; do
    option="$1"
    shift
    
    if [ "$option" = "--mode" ]; then
	CONFIG_MODE="$1"
	export CONFIG_MODE
        shift
    elif [ "$option" = "--run" ]; then
	CONFIG_RUN="$1"
	export CONFIG_RUN
        shift
    elif [ "$option" = "--uid" ]; then
        CONFIG_UID="$1"
	export CONFIG_UID
        shift
    elif [ "$option" = "--seed" ]; then
        CONFIG_SEED="$1"
	export CONFIG_SEED
        shift
    elif [ "$option" = "--generator" ]; then
        CONFIG_GENERATOR="$1"
	export CONFIG_GENERATOR
        shift
    elif [ "$option" = "--background" ]; then
        CONFIG_BACKGROUND="$1"
	export CONFIG_BACKGROUND
	CONFIG_BGEVDIR="BKG"
	export CONFIG_BGEVDIR
        shift
    elif [ "$option" = "--process" ]; then
        CONFIG_PROCESS="$1"
	export CONFIG_PROCESS
        shift
    elif [ "$option" = "--processbin" ]; then
        CONFIG_PROCESSBIN="$1"
	export CONFIG_PROCESSBIN
        shift  
#    elif [ "$option" = "--magnet" ]; then
#        CONFIG_MAGNET="$1"
#	export CONFIG_MAGNET	
#        shift
    elif [ "$option" = "--detector" ]; then
        CONFIG_DETECTOR="$1"
	export CONFIG_DETECTOR
        shift
    elif [ "$option" = "--system" ]; then
        OVERRIDE_SYSTEM="$1"
        OVERRIDE_TRIGGER="$1"
        shift
    elif [ "$option" = "--trigger" ]; then
        OVERRIDE_TRIGGER="$1"
        shift
    elif [ "$option" = "--energy" ]; then
        OVERRIDE_ENERGY="$1"
        shift
    elif [ "$option" = "--simulation" ]; then
        CONFIG_SIMULATION="$1"
	export CONFIG_SIMULATION
        shift
    elif [ "$option" = "--reconstruction" ]; then
        CONFIG_RECONSTRUCTION="$1"
	export CONFIG_RECONSTRUCTION
        shift
    elif [ "$option" = "--qa" ]; then
        CONFIG_QA="$1"
	export CONFIG_QA
        shift
    elif [ "$option" = "--aod" ]; then
        CONFIG_AOD="$1"
	export CONFIG_AOD
        shift
#    elif [ "$option" = "--physicslist" ]; then
#        CONFIG_PHYSICSLIST="$1"
#	export CONFIG_PHYSICSLIST
#        shift
    elif [ "$option" = "--bmin" ]; then
        CONFIG_BMIN="$1"
	export CONFIG_BMIN
        shift
    elif [ "$option" = "--bmax" ]; then
        CONFIG_BMAX="$1"
	export CONFIG_BMAX
        shift
    elif [ "$option" = "--ymin" ]; then
        CONFIG_YMIN="$1"
	export CONFIG_YMIN
        shift
    elif [ "$option" = "--ymax" ]; then
        CONFIG_YMAX="$1"
	export CONFIG_YMAX
        shift
    elif [ "$option" = "--pthardbin" ]; then
        CONFIG_PTHARDBIN="$1"
	export CONFIG_PTHARDBIN
        shift  
    elif [ "$option" = "--pthardmin" ]; then
        CONFIG_PTHARDMIN="$1"
	export CONFIG_PTHARDMIN
        shift  
    elif [ "$option" = "--pthardmax" ]; then
        CONFIG_PTHARDMAX="$1"
	export CONFIG_PTHARDMAX
        shift  
    elif [ "$option" = "--quenching" ]; then
        CONFIG_QUENCHING="$1"
	export CONFIG_QUENCHING
        shift 
    elif [ "$option" = "--qhat" ]; then
        CONFIG_QHAT="$1"
	export CONFIG_QHAT
        shift 
    elif [ "$option" = "--nevents" ]; then
        CONFIG_NEVENTS="$1"
	export CONFIG_NEVENTS
        shift 
    elif [ "$option" = "--nbkg" ]; then
        CONFIG_NBKG="$1"
	export CONFIG_NBKG
        shift 
    elif [ "$option" = "--ocdb" ]; then
        CONFIG_OCDB="$1"
	export CONFIG_OCDB
        shift 
    elif [ "$option" = "--hlt" ]; then
        CONFIG_HLT="$1"
	export CONFIG_HLT
        shift 
    elif [ "$option" = "--material" ]; then
        CONFIG_MATERIAL="$1"
	export CONFIG_MATERIAL
        shift 
    elif [ "$option" = "--geant4" ]; then
        CONFIG_GEANT4="on"
	export CONFIG_GEANT4
        shift 
#    elif [ "$option" = "--sdd" ]; then
#        RUNMODE="SDD"
#	export RUNMODE
#	shift
    fi
done

DC_RUN=$CONFIG_RUN
DC_EVENT=$CONFIG_UID
export DC_RUN DC_EVENT

if [ "$CONFIG_SEED" -eq 0 ]; then
    CONFIG_SEED=$((ALIEN_PROC_ID%1000000000))
fi
if [ "$CONFIG_SEED" -eq 0 ]; then
    CONFIG_SEED=$(((CONFIG_RUN*100000+CONFIG_UID)%1000000000))
    CONFIG_SEED_BASED="run / unique-id : $CONFIG_RUN / $CONFIG_UID"
else
    CONFIG_SEED_BASED="AliEn job ID"
fi
export CONFIG_SEED

if [ "$CONFIG_SEED" -eq 0 ]; then
    echo "*!  WARNING! Seeding variable for MC is 0 !" >&2
fi

if [ ! -z "$CONFIG_PTHARDBIN" ]; then

    # Define the pt hard bin arrays
    pthardbin_loweredges=(0 5 7 9 12 16 21 28 36 45 57 70 85 99 115 132 150 169 190 212 235)
    pthardbin_higheredges=( 5 7 9 12 16 21 28 36 45 57 70 85 99 115 132 150 169 190 212 235 -1)

    # Define environmental vars for pt binning
    CONFIG_PTHARDMIN=${pthardbin_loweredges[$CONFIG_PTHARDBIN]}
    CONFIG_PTHARDMAX=${pthardbin_higheredges[$CONFIG_PTHARDBIN]}
    export CONFIG_PTHARDMIN CONFIG_PTHARDMAX

fi

if [ ! -z "$CONFIG_PROCESSBIN" ]; then

    # Define process array
    if [[ $CONFIG_GENERATOR == *"Starlight"* ]]; then
	process_names=(TwoGammaToMuLow TwoGammaToElLow TwoGammaToMuMedium TwoGammaToElMedium TwoGammaToMuHigh TwoGammaToElHigh CohRhoToPi CohJpsiToMu CohJpsiToEl CohPsi2sToMu CohPsi2sToEl CohPsi2sToMuPi CohPsi2sToElPi CohUpsilonToMu CohUpsilonToEl IncohRhoToPi IncohJpsiToMu IncohJpsiToEl IncohPsi2sToMu IncohPsi2sToEl IncohPsi2sToMuPi IncohPsi2sToElPi IncohUpsilonToMu IncohUpsilonToEl)
	CONFIG_PROCESS=${process_names[$CONFIG_PROCESSBIN]}
    fi

    # check if well defined
    if [ -z "$CONFIG_PROCESS" ]; then
	echo "Undefined processbin $CONFIG_PROCESSBIN for generator $CONFIG_GENERATOR"
	exit 1;
    fi
    export CONFIG_PROCESS

fi

# mkdir input
# mv galice.root ./input/galice.root
# mv Kinematics.root ./input/Kinematics.root
# ls input

export ALIMDC_RAWDB1="./mdc1"
export ALIMDC_RAWDB2="./mdc2"
export ALIMDC_TAGDB="./mdc1/tag"
export ALIMDC_RUNDB="./mdc1/meta"

export PYTHIA8DATA="$ALICE_ROOT/PYTHIA8/pythia8/xmldoc"
export LHAPDF="$ALICE_ROOT/LHAPDF"
export LHAPATH="$ALICE_ROOT/LHAPDF/PDFsets"

if [ -f "$G4INSTALL/bin/geant4.sh" ]; then
    echo "* Sourcing G4 environment from $G4INSTALL/bin/geant4.sh"
    source $G4INSTALL/bin/geant4.sh
fi

### check whether we are in OCDB generation job 

if [[ $ALIEN_JDL_LPMOCDBJOB == "true" ]]; then
    echo ">>>>> OCDB generation: force MODE to 'ocdb'"
    export CONFIG_MODE="ocdb"
fi
    
### createSnapshot.C

if [[ $CONFIG_MODE == *"ocdb"* ]]; then

echo
echo "============================================"
echo " DPGSIM - Snapshot"
echo "============================================"
echo

    OCDBC=$ALIDPG_ROOT/MC/CreateSnapshot.C
    if [ -f CreateSnapshot.C ]; then
	SIMC=CreateSnapshot.C
    fi
    
    runcommand "OCDB SIM SNAPSHOT" $OCDBC\(0\) ocdbsim.log 500
    mv -f syswatch.log ocdbsimwatch.log
    if [ ! -f OCDBsim.root ]; then
	echo "*! Could not find OCDBsim.root, the snapshot creation chain failed!"
	echo "Could not find OCDBsim.root, the snapshot creation chain failed!" >> validation_error.message
	exit 2
    fi

    runcommand "OCDB REC SNAPSHOT" $OCDBC\(1\) ocdbrec.log 500
    mv -f syswatch.log ocdbrecwatch.log
    if [ ! -f OCDBrec.root ]; then
	echo "*! Could not find OCDBrec.root, the snapshot creation chain failed!"
	echo "Could not find OCDBrec.root, the snapshot creation chain failed!" >> validation_error.message
	exit 2
    fi

fi

### check basic requirememts
    
    if [[ $CONFIG_MODE == "" ]]; then
	echo ">>>>> ERROR: mode is required!"
	echo $COMMAND_HELP
	exit 2
    fi

if [[ $CONFIG_RUN == "" ]]; then
    echo ">>>>> ERROR: run number is required!"
    echo $COMMAND_HELP
    exit 2
fi

if [[ $CONFIG_OCDB == *"snapshot"* ]]; then
    
    if [ ! -f OCDBsim.root ]; then
	echo ">>>>> ERROR: Could not find OCDBsim.root"
    fi
    if [ ! -f OCDBrec.root ]; then
	echo ">>>>> ERROR: Could not find OCDBrec.root"
	exit 2
    fi    
fi

### automatic settings from CONFIG_MODE

if [[ $CONFIG_MODE == *"Muon"* ]]; then
    if [[ $CONFIG_DETECTOR == "" ]]; then
	export CONFIG_DETECTOR="Muon"
    fi
    if [[ $CONFIG_SIMULATION == "" ]]; then
	export CONFIG_SIMULATION="Muon"
    fi
    if [[ $CONFIG_RECONSTRUCTION == "" ]]; then
	export CONFIG_RECONSTRUCTION="Muon"
    fi
    if [[ $CONFIG_QA == "" ]]; then
	export CONFIG_QA="Muon"
    fi
    if [[ $CONFIG_AOD == "" ]]; then
	export CONFIG_AOD="Muon"
    fi
fi

### automatic settings from GRP info

aliroot -b -q $ALIDPG_ROOT/MC/ExportGRPinfo.C\($CONFIG_RUN\) 2>/dev/null | grep export > grpdump.sh && source grpdump.sh # && rm grpdump.sh

### override automatic settings from GRP info if requested 

if [[ $OVERRIDE_ENERGY != "" ]]; then
    export CONFIG_ENERGY=$OVERRIDE_ENERGY
fi

if [[ $OVERRIDE_SYSTEM != "" ]]; then
    export CONFIG_SYSTEM=$OVERRIDE_SYSTEM
fi

if [[ $OVERRIDE_TRIGGER != "" ]]; then
    export CONFIG_TRIGGER=$OVERRIDE_TRIGGER
fi

### check if custom trigger configuration file Custom.cfg is present
if [ $CONFIG_TRIGGER = "Custom.cfg" ] && [ ! -f Custom.cfg ]; then
    echo "Custom.cfg trigger configuration requested, file not present"
    exit 1
fi

##########################################

echo
echo "============================================"
echo " DPGSIM"
echo "============================================"
echo "Run.............. $CONFIG_RUN"
echo "Mode............. $CONFIG_MODE"
echo "QA train......... $CONFIG_QA"
echo "AOD train........ $CONFIG_AOD"
echo "============================================"
echo "Year............. $CONFIG_YEAR"
echo "Period........... $CONFIG_PERIOD"
echo "Beam type........ $CONFIG_BEAMTYPE"
echo "Energy........... $CONFIG_ENERGY"
echo "Detector mask.... $CONFIG_DETECTORMASK"
echo "============================================"
echo "Generator........ $CONFIG_GENERATOR"
echo "Process.......... $CONFIG_PROCESS"
echo "No. Events....... $CONFIG_NEVENTS"
echo "Unique-ID........ $CONFIG_UID"
echo "MC seed.......... $CONFIG_SEED"
echo "============================================"
echo "Background....... $CONFIG_BACKGROUND"
echo "No. Events....... $CONFIG_NBKG"
#echo "MC seed.......... $CONFIG_SEED (based on $CONFIG_SEED_BASED)"
echo "============================================"
echo "Detector......... $CONFIG_DETECTOR"
echo "GEANT4........... $CONFIG_GEANT4"
echo "Material Budget.. $CONFIG_MATERIAL"
echo "Simulation....... $CONFIG_SIMULATION"
echo "Reconstruction... $CONFIG_RECONSTRUCTION"
echo "System........... $CONFIG_SYSTEM"
echo "Trigger.......... $CONFIG_TRIGGER"
echo "OCDB............. $CONFIG_OCDB"
echo "HLT.............. $CONFIG_HLT"
echo "============================================"
#echo "B-field.......... $CONFIG_MAGNET"
#echo "Physicslist...... $CONFIG_PHYSICSLIST"
echo "b-min............ $CONFIG_BMIN"
echo "b-max............ $CONFIG_BMAX"
echo "y-min............ $CONFIG_YMIN"
echo "y-max............ $CONFIG_YMAX"
echo "============================================"
echo "pT-hard bin...... $CONFIG_PTHARDBIN"
echo "pT-hard min...... $CONFIG_PTHARDMIN"
echo "pT-hard max...... $CONFIG_PTHARDMAX"
echo "quenching........ $CONFIG_QUENCHING"
echo "q-hat............ $CONFIG_QHAT"
echo "============================================"
echo

### sim.C

if [[ $CONFIG_MODE == *"sim"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then
    
    if [[ $CONFIG_GENERATOR == "" ]]; then
	echo ">>>>> ERROR: generator is required for full production mode!"
	echo $COMMAND_HELP
	exit 2
    fi

    if [[ $CONFIG_SYSTEM == "" ]]; then
	echo ">>>>> ERROR: system is required for full production mode!"
	echo $COMMAND_HELP
	exit 2
    fi

    SIMC=$ALIDPG_ROOT/MC/sim.C
    if [ -f sim.C ]; then
	SIMC=sim.C
    fi

    # embedding using already generated background
    if [[ $CONFIG_BACKGROUND == *galice.root ]]; then

	echo ">>>>> EMBEDDING: request to embed $CONFIG_GENERATOR signal in $CONFIG_BACKGROUND background"
	export CONFIG_SIMULATION="EmbedSig"	
	export CONFIG_BGEVDIR=${CONFIG_BACKGROUND%galice.root}
	
    # embedding using on-the-fly generated background
    elif [[ $CONFIG_BACKGROUND != "" ]]; then

	CONFIG_BGEVDIR=$CONFIG_BACKGROUND
	export CONFIG_BGEVDIR

	echo ">>>>> EMBEDDING: request to embed $CONFIG_GENERATOR signal in $CONFIG_BACKGROUND background"

	if [[ $CONFIG_NBKG == "" ]]; then
	    export CONFIG_NBKG=1
	fi
	    
	SAVE_CONFIG_GENERATOR=$CONFIG_GENERATOR
	SAVE_CONFIG_NEVENTS=$CONFIG_NEVENTS
	SAVE_CONFIG_SIMULATION=$CONFIG_SIMULATION
	export CONFIG_GENERATOR=$CONFIG_BACKGROUND
	export CONFIG_NEVENTS=$CONFIG_NBKG
	export CONFIG_SIMULATION="EmbedBkg"
	export CONFIG_BGEVDIR="BKG"
	
	mkdir $CONFIG_BGEVDIR
	cp OCDB*.root *.C $CONFIG_BGEVDIR/.
	cd $CONFIG_BGEVDIR

	runcommand "BACKGROUND" $SIMC sim.log 5
	mv -f syswatch.log simwatch.log

	cd ..

	export CONFIG_GENERATOR=$SAVE_CONFIG_GENERATOR
	export CONFIG_NEVENTS=$SAVE_CONFIG_NEVENTS
	export CONFIG_SIMULATION="EmbedSig"
	
    fi

    runcommand "SIMULATION" $SIMC sim.log 5
    mv -f syswatch.log simwatch.log

    runBenchmark

fi

### rec.C

if [[ $CONFIG_MODE == *"rec"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then

    RECC=$ALIDPG_ROOT/MC/rec.C
    if [ -f rec.C ]; then
	RECC=rec.C
    fi
    
    runcommand "RECONSTRUCTION" $RECC rec.log 10
    mv -f syswatch.log recwatch.log    
    if [ ! -f AliESDs.root ]; then
	echo "*! Could not find AliESDs.root, the simulation/reconstruction chain failed!"
	echo "Could not find AliESDs.root, the simulation/reconstruction chain failed!" >> validation_error.message
	exit 2
    fi

    TAGC=$ALIDPG_ROOT/MC/tag.C
    if [ -f tag.C ]; then
	TAGC=tag.C
    fi    
    runcommand "TAG" $TAGC tag.log 50

    CHECKESDC=$ALIDPG_ROOT/MC/CheckESD.C
    if [ -f CheckESD.C ]; then
	CHECKESDC=CheckESD.C
    fi    
    runcommand "CHECK ESD" $CHECKESDC check.log 60 1

    # delete files not needed anymore
    if [[ $CONFIG_SIMULATION == "EmbedBkg" ]]; then
	rm -f *.RecPoints.root *.Digits.root
	ls *.Hits.root | grep -v T0.Hits.root | xargs rm
    else
	rm -f *.RecPoints.root *.Hits.root *.Digits.root *.SDigits.root
    fi

fi

### QAtrainsim.C

if [[ $CONFIG_MODE == *"qa"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then
    
    echo "QAresults.root" >> validation_extrafiles.list

    QATRAINSIMC=$ALIDPG_ROOT/QA/QAtrainsim.C\($CONFIG_RUN\)
    if [ -f QAtrainsim.C ]; then
	QATRAINSIMC=QAtrainsim.C\($CONFIG_RUN\)
    fi
    
    runcommand "QA TRAIN" $QATRAINSIMC qa.log 1000
    mv -f syswatch.log qawatch.log

    for file in *.stat; do
	mv -f "$file" "$file.qa"
    done
    
fi

### AODtrainsim.C

if [[ $CONFIG_MODE == *"aod"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then

    echo "AliAOD.root" >> validation_extrafiles.list
    
    AODTRAINSIMC=$ALIDPG_ROOT/AOD/AODtrainsim.C
    if [ -f AODtrainsim.C ]; then
	AODTRAINSIMC=AODtrainsim.C
    fi

    rm -f outputs_valid &>/dev/null

    runcommand "AOD TRAIN" $AODTRAINSIMC aod.log 1000
    mv -f syswatch.log aodwatch.log

    for file in *.stat; do
	mv -f $file $file.aod
    done
    
fi


exit 0
