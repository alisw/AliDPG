#!/bin/bash

# check ALIDPG_ROOT is set
if [ -z "$ALIDPG_ROOT" ]; then
    echo "ALIDPG_ROOT not set, abort."
    exit 1
fi

# check ALICE_ROOT is set
if [ -z "$ALICE_ROOT" ]; then
    echo "ALICE_ROOT not set, abort."
    exit 1
fi

# enable vdt library

$ALIDPG_ROOT/DataProc/Common/EnableVDTUponPackageVersion.sh
###################

# set job and simulation variables as :
function COMMAND_HELP(){
    echo ""
    echo "More help can be found here: https://twiki.cern.ch/twiki/bin/view/ALICE/AliDPGMonteCarloTutorial"
    echo "How to use: ./dpgsim.sh"
    echo ""
    echo "--help                                    Print this help"
    echo "--debug                                   Debug option (has different implications, see dpgsim.sh for details)"
    echo "--mode <mode>                 (mandatory) Running mode (ocdb,sim,rec,qa,aod,full,Muon,MuonOnly,Run3,extractembedded)" 
    echo "--run <run>                   (mandatory) Anchor run number"    
    echo "--generator <generatorConfig>             Mandatory for sim mode, choose: general purpose, PWG specific, or Custom (see TWiki)"
    echo ""
    echo "Monte Carle generation control:"
    echo "--nevents <numberOfEvents>                Number of events to be generated in the simulation stage"
    echo "--uid <uniqueID>                          Unique identifier of the process"
    echo "--pdg <pdgcode>                           PDG value of particle. Specific use may vary according to generator configuration"
    echo "--bmin <bmin> --bmax <bmax>               Impact parameter range. Specific use may vary according to generator configuration"
    echo "--ymin <min> --ymax <max>                 Rapidity range. Specific use may vary according to generator configuration"
    echo "--phimin <min> --phimax <max>             Azimuthal angle range (in degree). Specific use may vary according to generator configuration"
    echo "--ptmin <min> --ptmax <max>               Transverse momentum range. Specific use may vary according to generator configuration"
    echo "--pthardmin <min> --pthardmax <max>       Range of pT hard bins (if applicable)" 
    echo "--pthardbin <bin number>                  For selection of predefined pT hard bins (see MC/dpgsim.sh for definition)"
    echo "--pttrigmin <min> --pttrigmax <max>       Range of pT for trigger particles (if applicable)"
    echo "--process <name>                          Process tag name. Specific use may vary according to generator configuration"
    echo "--processbin <bin                         Process bin for predefined process tag names. Specific use may vary according to generator configuration"
    echo "--seed <seed>                             Seed for MC (0: use alien ID if alien job or run ID and uid)"
    echo "--background <generatorConfig>            Generator used for background events in MC-to-MC embedding"
    echo "--nbkg <numberOfEvents>                   Number of background events to be generated in the simulation stag in MC-to-MC embedding mode"
    echo "--quenching <quenching>                   Switch on quenching for Pythia: --quenching 1"
    echo "--qhat <qhat>                             Modify qhat for Pythia: --quenching qhat (default = 1.7)"
    echo ""
    echo "Detector, simulation, reconstruction and other settings:"
    echo "--detector <detectorConfig>               tag name of the detector configuration, as defined in MC/DetectorConfig.C" 
    #echo "--magnet <magnetConfig>                  NOT USED AT THE MOMENT" 
    echo "--simulation <simulationConfig>           tag name of the simulation configuration, as defined in MC/SimulationConfig.C" 
    echo "--reconstruction <reconstructionConfig>   tag name of the reconstruction configuration, as defined in MC/ReconstructionConfig.C" 
    echo "--qa <qaConfig>                           tag name of the QA configuration to be used" 
    echo "--aod <aodConfig>                         tag name of the AOD configuration to be used" 
    echo "--hlt <hltConfig>                         HLT configuration" 
    echo "--ocdb <ocdbConfig>                       OCDB configuration: <snapshot> for OCDB from snapshots, <alien>for OCDB from AliEn" 
    echo "--OCDBTimeStamp <timeStamp>               Use time stamp in OCDB snapshot creation"
    echo "--ocdbCustom                              Use custom OCDB, OCDBCustom.C to be provided"
    echo "--keepTrackRefsFraction <percentage>      Percentage of subjobs that keeps the TrackRefs file"
    echo "--signalFilteringFraction <percentage>    Percentage of subjobs that use signal filtering (for embedding only)"
    echo "--material <densityFactor>                Modify material budget by a density factor"
    echo "--purifyKineOff                           Switch off the PurifyKine step in simulation" 
    echo "--fluka                                   Use FLUKA instead of GEANT3" 
    echo "--geant4                                  Use GEANT4 instead of GEANT3"
    echo "--nofastB                                 Switch off usage of fast B field"
    echo "--novdt                                   Switch off usage of VDT library"
    echo ""
    echo "Override automatic settings:"
    echo "--energy <energy>                         Centre-of-mass energy" 
    echo "--system <system>             (mandatory) Collision system"
    echo "--trigger <triggerconfig>                 Configuration of trigger inputs: ocdb, Pb-Pb (dummy configuration), p-p (dummy configuration), MUON or Custom.cfg configuration (needs Custom.cfg file)"
    echo ""
    echo "More help can be found here: https://twiki.cern.ch/twiki/bin/view/ALICE/AliDPGMonteCarloTutorial"
    echo ""

}

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
OVERRIDE_BKG_PATH_RECORD=""
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
CONFIG_PDG=""
CONFIG_BMIN=""
CONFIG_BMAX=""
CONFIG_YMIN=""
CONFIG_YMAX=""
CONFIG_PHIMIN=""
CONFIG_PHIMAX=""
CONFIG_PTMIN=""
CONFIG_PTMAX=""
CONFIG_PTHARDBIN=""
CONFIG_PTHARDMIN=""
CONFIG_PTHARDMAX=""
CONFIG_PTTRIGBIN=""
CONFIG_PTTRIGMIN=""
CONFIG_PTTRIGMAX=""
CONFIG_QUENCHING=""
CONFIG_QHAT=""
CONFIG_RUN=""
CONFIG_UID="1"
CONFIG_SIMULATION=""
CONFIG_RECONSTRUCTION=""
CONFIG_QA=""
CONFIG_AOD=""
CONFIG_MODE=""
CONFIG_OCDB="snapshot"
CONFIG_OCDBCUSTOM=""
CONFIG_OCDBRUN3=""
CONFIG_PURIFYKINEOFF=""
CONFIG_HLT=""
CONFIG_GEANT4=""
CONFIG_FLUKA=""
CONFIG_FASTB=""
CONFIG_VDT="on"
CONFIG_MATERIAL=""
CONFIG_KEEPTRACKREFSFRACTION="0"
CONFIG_REMOVETRACKREFS="off"
CONFIG_SIGNALFILTERINGFRACTION="0"
CONFIG_SIGNALFILTERING="off"
CONFIG_OCDBTIMESTAMP=""
CONFIG_DEBUG=""

RUNMODE=""

while [ ! -z "$1" ]; do
    option="$1"
    shift

    if [ "$option" = "--help" ]; then
	COMMAND_HELP
	exit 0
    elif [ "$option" = "--debug" ]; then
	CONFIG_DEBUG="on"
	export CONFIG_DEBUG
    elif [ "$option" = "--mode" ]; then
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
    elif [ "$option" = "--pdg" ]; then
        CONFIG_PDG="$1"
	export CONFIG_PDG
        shift
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
    elif [ "$option" = "--phimin" ]; then
        CONFIG_PHIMIN="$1"
	export CONFIG_PHIMIN
        shift
    elif [ "$option" = "--phimax" ]; then
        CONFIG_PHIMAX="$1"
	export CONFIG_PHIMAX
        shift
    elif [ "$option" = "--ptmin" ]; then
        CONFIG_PTMIN="$1"
	export CONFIG_PTMIN
        shift
    elif [ "$option" = "--ptmax" ]; then
        CONFIG_PTMAX="$1"
	export CONFIG_PTMAX
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
    elif [ "$option" = "--pttrigbin" ]; then
        CONFIG_PTTRIGBIN="$1"
	export CONFIG_PTTRIGBIN
        shift
    elif [ "$option" = "--pttrigmin" ]; then
        CONFIG_PTTRIGMIN="$1"
	export CONFIG_PTTRIGMIN
        shift
    elif [ "$option" = "--pttrigmax" ]; then
        CONFIG_PTTRIGMAX="$1"
	export CONFIG_PTTRIGMAX
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
        if [[ $CONFIG_OCDB == cvmfs* ]]; then
            OCDB_PATH=${CONFIG_OCDB#*:}
            [[ ! $OCDB_PATH || $OCDB_PATH == $CONFIG_OCDB ]] && OCDB_PATH=/cvmfs/alice-ocdb.cern.ch
            export OCDB_PATH
            CONFIG_OCDB=cvmfs
        fi
        export CONFIG_OCDB
        shift
    elif [ "$option" = "--ocdbCustom" ]; then
	CONFIG_OCDBCUSTOM="1"
	export CONFIG_OCDBCUSTOM
    elif [ "$option" = "--purifyKineOff" ]; then
	CONFIG_PURIFYKINEOFF="1"
	export CONFIG_PURIFYKINEOFF
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
    elif [ "$option" = "--fluka" ]; then
        CONFIG_FLUKA="on"
	export CONFIG_FLUKA
    elif [ "$option" = "--nofastB" ]; then
        CONFIG_FASTB=""
    elif [ "$option" = "--novdt" ]; then
        CONFIG_VDT=""
#    elif [ "$option" = "--removeTrackRefs" ]; then
#        CONFIG_REMOVETRACKREFS="on"
#	export CONFIG_REMOVETRACKREFS
    elif [ "$option" = "--keepTrackRefsFraction" ]; then
	CONFIG_KEEPTRACKREFSFRACTION="$1"
	export CONFIG_KEEPTRACKREFSFRACTION
        shift
    elif [ "$option" = "--signalFilteringFraction" ] && [[ $CONFIG_MODE == *"extractembedded"* ]]; then
	CONFIG_SIGNALFILTERINGFRACTION="$1"
	export CONFIG_SIGNALFILTERINGFRACTION
	shift
    elif [ "$option" = "--OCDBTimeStamp" ]; then
        CONFIG_OCDBTIMESTAMP="$1"
        export CONFIG_OCDBTIMESTAMP
#    elif [ "$option" = "--sdd" ]; then
#        RUNMODE="SDD"
#	export RUNMODE
#	shift
    fi
done

# export settings (needed, since some are set to on by default)
if [ "$CONFIG_FASTB" = "on" ]; then
    export CONFIG_FASTB
fi

if [ "$CONFIG_REMOVETRACKREFS" = "on" ]; then
    export CONFIG_REMOVETRACKREFS
fi

if [[ $CONFIG_VDT == "on" ]] && [ -f $ALICE_ROOT/lib/libalivdtwrapper.so ]; then
    export LD_PRELOAD=$LD_PRELOAD:$ALICE_ROOT/lib/libalivdtwrapper.so
fi

# detect VDT math library (if set elsewhere)
if [[ $LD_PRELOAD == *"libalivdtwrapper.so"* ]] && [ -f $ALICE_ROOT/lib/libalivdtwrapper.so ]; then
    CONFIG_VDT="on"
    export CONFIG_VDT
fi

# if VDT library not available, VDT cannot be used
if [ ! -f $ALICE_ROOT/lib/libalivdtwrapper.so ]; then
    CONFIG_VDT=""
fi

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

    if [ ! -z "$CONFIG_PTHARDMIN" ] || [ ! -z "$CONFIG_PTHARDMAX" ]; then
	echo "Simultaneous setting of pthardbin ($CONFIG_PTHARDBIN) and pthardmin/max ($CONFIG_PTHARDMIN / $CONFIG_PTHARDMAX) not allowed"
	exit 1;
    fi

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

# >>>------------------ decide if TrackRefs.root should be removed ----------------->>>

if [[ -z $ALIEN_PROC_ID ]] ; then
    export CONFIG_PROCID=$RANDOM
    echo "*!  WARNING! ALIEN_PROC_ID is not set, will use random number"
else
    export CONFIG_PROCID=$ALIEN_PROC_ID
fi
# check consistency of provided (if any) TrackRefs fraction to keep
if [[ ! $CONFIG_KEEPTRACKREFSFRACTION =~ ^[0-9]+$ ]] ; then
    echo "Invalid value $CONFIG_KEEPTRACKREFSFRACTION provided for keepTrackRefsFraction"
    exit 1
fi
[[ $CONFIG_KEEPTRACKREFSFRACTION -gt 100 ]] && CONFIG_KEEPTRACKREFSFRACTION="100"
[[ $((CONFIG_PROCID%100)) -ge $CONFIG_KEEPTRACKREFSFRACTION ]] && CONFIG_REMOVETRACKREFS="on" || CONFIG_REMOVETRACKREFS="off"
export CONFIG_REMOVETRACKREFS

# <<<------------------ decide if TrackRefs.root should be removed -----------------<<<

# >>>------------------ decide if signal should be filtered (for embedding) -------->>>

# check consistency of provided (if any) fraction of subjobs where signal filtering is used
if [[ ! $CONFIG_SIGNALFILTERINGFRACTION =~ ^[0-9]+$ ]] ; then
    echo "Invalid value $CONFIG_SIGNALFILTERINGFRACTION provided for signalFilteringFraction"
    exit 1
fi

[[ $CONFIG_SIGNALFILTERINGFRACTION -gt 100 ]] && CONFIG_SIGNALFILTERINGFRACTION="100"
[[ $((CONFIG_PROCID%100)) -ge $CONFIG_SIGNALFILTERINGFRACTION ]] && CONFIG_SIGNALFILTERING="off" || CONFIG_SIGNALFILTERING="on"
export CONFIG_SIGNALFILTERING

# <<<------------------ decide if signal should be filtered (for embedding) --------<<<

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

### check if Run 3 (needed for snapshot creation)

if [[ $CONFIG_MODE == *"Run3"* ]]; then
    if [[ $CONFIG_OCDBRUN3 == "" ]]; then
	CONFIG_OCDBRUN3="1"
	export CONFIG_OCDBRUN3
    fi
fi

### check for Fluka and configure it if needed

if [[ $CONFIG_FLUKA == "on" ]]; then
    echo "* Linking files needed for FLUKA"
    source $ALIDPG_ROOT/MC/FlukaConfig.sh
    if [[ $FLUKA_CONFIG_ERROR ]]; then
        echo "FATAL: Errors configuring FLUKA: aborting!"
        exit 1
    fi
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
	OCDBC=CreateSnapshot.C
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
	COMMAND_HELP
	exit 2
    fi

if [[ $CONFIG_RUN == "" ]]; then
    echo ">>>>> ERROR: run number is required!"
    COMMAND_HELP
    exit 2
fi

if [[ $CONFIG_OCDB == *"snapshot"* ]]; then

    if [[ $CONFIG_MODE == *"sim"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then
	if [ ! -f OCDBsim.root ]; then
	    echo ">>>>> ERROR: Could not find OCDBsim.root"
	    exit 2
	fi
    fi
    if [[ $CONFIG_MODE == *"rec"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then
	if [ ! -f OCDBrec.root ]; then
	    echo ">>>>> ERROR: Could not find OCDBrec.root"
	    exit 2
	fi
    fi
fi

### automatic settings from CONFIG_MODE

if [[ $CONFIG_MODE == *"MuonOnly"* ]]; then

    if [[ $CONFIG_DETECTOR == "" ]]; then
        CONFIG_DETECTOR="Muon"
        export CONFIG_DETECTOR
    fi

    if [[ $CONFIG_SIMULATION == "" ]]; then
        CONFIG_SIMULATION="MuonOnly"
        export CONFIG_SIMULATION
    fi

    if [[ $CONFIG_RECONSTRUCTION == "" ]]; then
        CONFIG_RECONSTRUCTION="MuonOnly"
        export CONFIG_RECONSTRUCTION
    fi

    if [[ $CONFIG_QA == "" ]]; then
        CONFIG_QA="MuonOnly"
        export CONFIG_QA
    fi

    if [[ $CONFIG_AOD == "" ]]; then
        CONFIG_AOD="MuonOnly"
        export CONFIG_AOD
    fi

elif [[ $CONFIG_MODE == *"Muon"* ]]; then

    if [[ $CONFIG_DETECTOR == "" ]]; then
        CONFIG_DETECTOR="Muon"
        export CONFIG_DETECTOR
    fi

    if [[ $CONFIG_SIMULATION == "" ]]; then
        CONFIG_SIMULATION="Muon"
        export CONFIG_SIMULATION
    fi

    if [[ $CONFIG_RECONSTRUCTION == "" ]]; then
        CONFIG_RECONSTRUCTION="Muon"
        export CONFIG_RECONSTRUCTION
    fi

    if [[ $CONFIG_QA == "" ]]; then
        CONFIG_QA="Muon"
        export CONFIG_QA
    fi

    if [[ $CONFIG_AOD == "" ]]; then
        CONFIG_AOD="Muon"
        export CONFIG_AOD
    fi

elif [[ $CONFIG_MODE == *"Run3"* ]]; then

    if [[ $CONFIG_DETECTOR == "" ]]; then
        CONFIG_DETECTOR="Run3"
        export CONFIG_DETECTOR
    fi

    if [[ $CONFIG_SIMULATION == "" ]]; then
        CONFIG_SIMULATION="Run3"
        export CONFIG_SIMULATION
    fi

    if [[ $CONFIG_RECONSTRUCTION == "" ]]; then
        CONFIG_RECONSTRUCTION="Run3"
        export CONFIG_RECONSTRUCTION
    fi

fi

### automatic settings from GRP info

aliroot -b -q $ALIDPG_ROOT/MC/ExportGRPinfo.C\($CONFIG_RUN\) 2>/dev/null | grep export > grpdump.sh && source grpdump.sh # && rm grpdump.sh

### background from .xml collection
if [[ $CONFIG_BACKGROUND == *.xml ]]; then
    aliroot -b -q $ALIDPG_ROOT/MC/ExportXMLbackground.C\(\"$CONFIG_BACKGROUND\"\) 2>/dev/null | grep export > xmldump.sh && source xmldump.sh # && rm xmldump.sh    
fi

### background from AliEn file, copy files locally
if [[ $CONFIG_BACKGROUND == alien://* ]]; then
    echo "Copying background files from AliEn to local cache..."
    mkdir -p BKG
    alien_cp ${CONFIG_BACKGROUND%galice.root}/*.zip BKG/.
    for I in BKG/*.zip; do
	unzip $I -d BKG && rm $I
    done
    export OVERRIDE_BKG_PATH_RECORD=$CONFIG_BACKGROUND
    export CONFIG_BACKGROUND="BKG/galice.root"
    echo "Content of background local cache:"
    ls BKG/*
    echo "================================="
fi

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
if [ "$CONFIG_TRIGGER" == "Custom.cfg" ] && [ ! -f Custom.cfg ]; then
    echo "Custom.cfg trigger configuration requested, file not present"
    exit 1
elif [[ $OVERRIDE_TRIGGER == "" ]]; then
    aliroot -b -q "$ALIDPG_ROOT/MC/CheckTOF.C($CONFIG_RUN)"
    checkTOF=$?
    if [ $checkTOF == 1 ]; then
	#echo "TOF was triggering but not in data acquisition --> Using custom trigger from AliDPG"
	echo "TOF was not in data acquisition --> Using custom trigger from AliDPG"
	cp $ALIDPG_ROOT/Utils/Custom.cfg .
	CONFIG_TRIGGER="Custom.cfg"
    fi
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
echo "PROCID........... $CONFIG_PROCID"
echo "============================================"
echo "Background....... $CONFIG_BACKGROUND"
echo "Override record.. $OVERRIDE_BKG_PATH_RECORD"
echo "No. Events....... $CONFIG_NBKG"
#echo "MC seed.......... $CONFIG_SEED (based on $CONFIG_SEED_BASED)"
echo "============================================"
echo "Detector......... $CONFIG_DETECTOR"
echo "GEANT4........... $CONFIG_GEANT4"
echo "FLUKA............ $CONFIG_FLUKA"
echo "PurifyKineOff.... $CONFIG_PURIFYKINEOFF"
echo "Fast-B........... $CONFIG_FASTB"
echo "VDT math......... $CONFIG_VDT"
echo "Material Budget.. $CONFIG_MATERIAL"
echo "TrackRefs to keep ${CONFIG_KEEPTRACKREFSFRACTION}%"
echo "Remove TrackRefs. ${CONFIG_REMOVETRACKREFS} (in this job)"
echo "Filter fraction.. ${CONFIG_SIGNALFILTERINGFRACTION}%"
echo "Filter signal.... ${CONFIG_SIGNALFILTERING} (in this job)"
echo "Simulation....... $CONFIG_SIMULATION"
echo "Reconstruction... $CONFIG_RECONSTRUCTION"
echo "System........... $CONFIG_SYSTEM"
echo "Trigger.......... $CONFIG_TRIGGER"
echo "OCDB............. $CONFIG_OCDB"
echo "OCDBCustom....... $CONFIG_OCDBCUSTOM"
echo "OCDBRun3......... $CONFIG_OCDBRUN3"
echo "HLT.............. $CONFIG_HLT"
echo "============================================"
#echo "B-field.......... $CONFIG_MAGNET"
#echo "Physicslist...... $CONFIG_PHYSICSLIST"
echo "PDG code......... $CONFIG_PDG"
echo "b-min............ $CONFIG_BMIN"
echo "b-max............ $CONFIG_BMAX"
echo "y-min............ $CONFIG_YMIN"
echo "y-max............ $CONFIG_YMAX"
echo "phi-min (in deg.) $CONFIG_PHIMIN"
echo "phi-max (in deg.) $CONFIG_PHIMAX"
echo "pT-min........... $CONFIG_PTMIN"
echo "pT-max........... $CONFIG_PTMAX"
echo "============================================"
echo "pT-hard bin...... $CONFIG_PTHARDBIN"
echo "pT-hard min...... $CONFIG_PTHARDMIN"
echo "pT-hard max...... $CONFIG_PTHARDMAX"
echo "pT-trigger bin... $CONFIG_PTTRIGBIN"
echo "pT-trigger min... $CONFIG_PTTRIGMIN"
echo "pT-trigger max... $CONFIG_PTTRIGMAX"
echo "quenching........ $CONFIG_QUENCHING"
echo "q-hat............ $CONFIG_QHAT"
echo "============================================"
echo "Debug mode....... $CONFIG_DEBUG"
echo "============================================"
echo

### sim.C

if [[ $CONFIG_GENERATOR == *"AMPT_v2"* ]]; then

    AMPTC=$ALIDPG_ROOT/MC/EXTRA/AMPTstandalone.C
    runcommand "AMPT" $AMPTC ampt.log 10

    if [ ! -f ampteventfifo ]; then
	echo "*! Could not find ampteventfifo, the AMPT simulation failed!"
	echo "Could not find ampteventfifo, the AMPT simulation failed!" >> validation_error.message
	exit 2
    fi
    
fi


if [[ $CONFIG_MODE == *"sim"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then

    if [[ $CONFIG_GENERATOR == "" ]]; then
	echo ">>>>> ERROR: generator is required for full production mode!"
	COMMAND_HELP
	exit 2
    fi

    if [[ $CONFIG_SYSTEM == "" ]]; then
	echo ">>>>> ERROR: system is required for full production mode!"
	COMMAND_HELP
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


# ESD tag creation removed on September 2017 after CB and PB discussions

    # Commenting out the CheckESD while we sort out how to configure
    # the PID response properly. In any case, the output of this check is
    # neither checked nor stored at present
#    CHECKESDC=$ALIDPG_ROOT/MC/CheckESD.C
#    if [ -f CheckESD.C ]; then
#	CHECKESDC=CheckESD.C
#    fi
#    runcommand "CHECK ESD" $CHECKESDC check.log 60 1
#
    
    # delete files not needed anymore
    if [[ $CONFIG_SIMULATION == "EmbedBkg" ]]; then
	rm -f *.RecPoints.root *.Digits.root
	ls *.Hits.root | grep -v T0.Hits.root | xargs rm
    else
	rm -f *.RecPoints.root *.Hits.root *.Digits.root *.SDigits.root
    fi

fi

### Embedded signal filtering (only upon request and in embedded mode and for given fraction)

if [[ $CONFIG_MODE == *"extractembedded"* ]] && [[ $CONFIG_SIMULATION == *"EmbedSig"* ]] && [ $CONFIG_SIGNALFILTERING == "on" ]; then

    if [[ $CONFIG_MODE == *"extractembeddedmixed"* ]]; then
	EXTRACTEMBEDDEDC=$ALIDPG_ROOT/MC/ExtractEmbeddedWrapper.C\(kTRUE\)
    else
	EXTRACTEMBEDDEDC=$ALIDPG_ROOT/MC/ExtractEmbeddedWrapper.C\(kFALSE\)
    fi
	
    runcommand "EXTRACTEMBEDDED" $EXTRACTEMBEDDEDC extractembedded.log 10
    
    if [ ! -f AliESDs_EMB.root ]; then
	echo "*! Could not find AliESDs_EMB.root, the embedded signal filtering failed!"
	echo "Could not find AliESDs_EMB.root, the embedded signal filtering failed!" >> validation_error.message
	exit 2
    fi

    if [ ! -f galice_EMB.root ]; then
	echo "*! Could not find galice_EMB.root, the embedded signal filtering failed!"
	echo "Could not find galice_EMB.root, the embedded signal filtering failed!" >> validation_error.message
	exit 2
    fi

    if [ "$CONFIG_DEBUG" = "on" ]; then
	mv "AliESDs.root" "AliESDs_ORIG.root"
	mv "galice.root" "galice_ORIG.root"
	mv "AliESDs_EMB.root" "AliESDs.root"
	mv "galice_EMB.root" "galice.root"
    else
	mv "AliESDs_EMB.root" "AliESDs.root"
	mv "galice_EMB.root" "galice.root"
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

### AODtrainRawAndMC.C

if [[ $CONFIG_MODE == *"aod"* ]] || [[ $CONFIG_MODE == *"full"* ]]; then

    echo "AliAOD.root" >> validation_extrafiles.list

    AODTRAINSIMC=$ALIDPG_ROOT/AOD/AODtrainRawAndMC.C\(0,kTRUE\)
    if [ -f AODtrainRawAndMC.C ]; then
	AODTRAINSIMC=AODtrainRawAndMC.C\(0,kTRUE\)
    fi

    rm -f outputs_valid &>/dev/null

    runcommand "AOD TRAIN" $AODTRAINSIMC aod.log 1000
    mv -f syswatch.log aodwatch.log

    for file in *.stat; do
	mv -f $file $file.aod
    done

    if [ -f $ALIDPG_ROOT/QA/QAtrainAOD.C ]; then

	echo "QAresults_AOD.root" >> validation_extrafiles.list

	QATRAINAOD=$ALIDPG_ROOT/QA/QAtrainAOD.C\(kTRUE\)
	if [ -f QAtrainAOD.C ]; then
	    QATRAINAOD=QAtrainAOD.C\(kTRUE\)
	fi

	runcommand "QAtrain AOD" $QATRAINAOD aodqa.log 2000

	for file in *.stat; do
	    mv -f $file $file.qa_aod
	done

    fi

fi

if [ $CONFIG_REMOVETRACKREFS == "on" ] && [ -f TrackRefs.root ] ; then
    echo "Removing TrackRefs.root"
    rm -f TrackRefs.root
fi


exit 0
