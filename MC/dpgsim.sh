#!/bin/bash

# set job and simulation variables as :
COMMAND_HELP="./simrun.sh  --run <runNumber> --generator <generatorConfig> --energy <energy> --detector <detectorConfig> --magnet <magnetConfig> --simulation <simulationConfig> --reconstruction <reconstructionConfig> --uid <uniqueID> --nevents <numberOfEvents> --mode <simrunMode>"

function runcommand(){
    echo -e "\n"
    echo -e "\n" >&2

    echo "* $1 : $2"
    echo "* $1 : $2" >&2
    
    time aliroot -b -q -x $2 >>$3 2>&1
    exitcode=$?
    
    expectedCode=${5-0}
    
    if [ "$exitcode" -ne "$expectedCode" ]; then
        echo "*! $2 failed with exitcode $exitcode, expecting $expectedCode"
        echo "*! $2 failed with exitcode $exitcode, expecting $expectedCode" >&2
        echo "$2 failed with exitcode $exitcode, expecting $expectedCode" > validation_error.message
        exit ${4-$exitcode}
    else
        echo "* $2 finished with the expected exit code ($expectedCode), moving on"
        echo "* $2 finished with the expected exit code ($expectedCode), moving on" >&2
    fi
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

# Define the pt hard bin arrays
pthardbin_loweredges=( 0 5 11 21 36 57 84 117 152 191 234 )
pthardbin_higheredges=( 5 11 21 36 57 84 117 152 191 234 -1)

CONFIG_NEVENTS=""
CONFIG_SEED=""
CONFIG_GENERATOR=""
CONFIG_MAGNET=""
CONFIG_ENERGY=""
CONFIG_DETECTOR=""
CONFIG_PHYSICSLIST=""
CONFIG_BMIN=""
CONFIG_BMAX=""
CONFIG_PTHARDBIN=""
CONFIG_PTHARDMIN=""
CONFIG_PTHARDMAX=""
CONFIG_QUENCHING=""
CONFIG_RUN=""
CONFIG_UID=""
CONFIG_SIMULATION=""
CONFIG_RECONSTRUCTION=""
CONFIG_MODE="simrec"

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
    elif [ "$option" = "--generator" ]; then
        CONFIG_GENERATOR="$1"
	export CONFIG_GENERATOR
        shift
    elif [ "$option" = "--magnet" ]; then
        CONFIG_MAGNET="$1"
	export CONFIG_MAGNET	
        shift
    elif [ "$option" = "--detector" ]; then
        CONFIG_DETECTOR="$1"
	export CONFIG_DETECTOR
        shift
    elif [ "$option" = "--energy" ]; then
        CONFIG_ENERGY="$1"
	export CONFIG_ENERGY
        shift
    elif [ "$option" = "--simulation" ]; then
        CONFIG_SIMULATION="$1"
	export CONFIG_SIMULATION
        shift
    elif [ "$option" = "--reconstruction" ]; then
        CONFIG_RECONSTRUCTION="$1"
	export CONFIG_RECONSTRUCTION
        shift
    elif [ "$option" = "--physicslist" ]; then
        CONFIG_PHYSICSLIST="$1"
	export CONFIG_PHYSICSLIST
        shift
    elif [ "$option" = "--bmin" ]; then
        CONFIG_BMIN="$1"
	export CONFIG_BMIN
        shift
    elif [ "$option" = "--bmax" ]; then
        CONFIG_BMAX="$1"
	export CONFIG_BMAX
        shift
    elif [ "$option" = "--pthardbin" ]; then
        CONFIG_PTHARDBIN="$1"
	export CONFIG_PTHARDBIN
        shift  
    elif [ "$option" = "--quench" ]; then
        CONFIG_QUENCHING="$1"
	export CONFIG_QUENCHING
        shift 
    elif [ "$option" = "--nevents" ]; then
        CONFIG_NEVENTS="$1"
	export CONFIG_NEVENTS
        shift 
    elif [ "$option" = "--sdd" ]; then
        RUNMODE="SDD"
	export RUNMODE
    fi
done

if [ "$CONFIG_RUN" = "" ]; then
    echo ">>>>> ERROR: run number is required!"
    echo $COMMAND_HELP
    exit
fi

if [ "$CONFIG_GENERATOR" = "" ]; then
    echo ">>>>> ERROR: generator is required!"
    echo $COMMAND_HELP
    exit
fi

DC_RUN=$CONFIG_RUN
DC_EVENT=$CONFIG_UID
export DC_RUN DC_EVENT

CONFIG_SEED=$((ALIEN_PROC_ID%1000000000))
if [ "$CONFIG_SEED" -eq 0 ]; then
    CONFIG_SEED=$(((CONFIG_RUN*100000+CONFIG_UID)%1000000000))
    echo "* MC Seed is $CONFIG_SEED (based on run / unique-id : $CONFIG_RUN / $CONFIG_UID)"
else
    echo "* MC Seed is $CONFIG_SEED (based on AliEn job ID)"
fi
export CONFIG_SEED

if [ "$CONFIG_SEED" -eq 0 ]; then
    echo "*!  WARNING! Seeding variable for MC is 0 !" >&2
fi

echo "* b min is $CONFIG_BMIN"
echo "* b max is $CONFIG_BMAX"
echo "* pt hard bin is $CONFIG_PTHARDBIN"

if [ ! -z "$CONFIG_PTHARDBIN" ]; then
    # Define environmental vars for pt binning
    CONFIG_PTHARDMIN=${pthardbin_loweredges[$CONFIG_PTHARDBIN]}
    CONFIG_PTHARDMAX=${pthardbin_higheredges[$CONFIG_PTHARDBIN]}
    export CONFIG_PTHARDMIN CONFIG_PTHARDMAX

    echo "* pt hard from $CONFIG_PTHARDMIN to $CONFIG_PTHARDMAX"
fi

mkdir input
mv galice.root ./input/galice.root
mv Kinematics.root ./input/Kinematics.root
ls input

#export CONFIG_SEED CONFIG_NEVENTS CONFIG_GENERATOR CONFIG_MAGNET CONFIG_ENERGY CONFIG_DETECTOR CONFIG_PHYSICSLIST CONFIG_BMIN CONFIG_BMAX CONFIG_PTHARDBIN CONFIG_PTHARDMIN CONFIG_PTHARDMAX CONFIG_RUN CONFIG_UID DC_RUN DC_EVENT

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

echo "SIMRUN:: Run $CONFIG_RUN | Unique-ID $CONFIG_UID | Generator $CONFIG_GENERATOR | Magnetic field $CONFIG_MAGNET | Energy $CONFIG_ENERGY | Physicslist $CONFIG_PHYSICSLIST | Number of Events $CONFIG_NEVENTS | Simulation $CONFIG_SIMULATION | Reconstruction $CONFIG_RECONSTRUCTION | Mode $CONFIG_MODE | "

if [ "$CONFIG_MODE" == "simrec" ] || [ "$CONFIG_MODE" = "sim" ]; then
    SIMSTART=`date "+%s"`
    runcommand "SIMULATION" "$ALIDPG_ROOT/MC/sim.C" sim.log 5
    mv syswatch.log simwatch.log
    SIMEND=`date "+%s"`
    echo "SIMTIME: $((SIMEND-SIMSTART))"
fi

runBenchmark

if [ "$CONFIG_MODE" == "simrec" ] || [ "$CONFIG_MODE" = "rec" ]; then
    RECSTART=`date "+%s"`
    runcommand "RECONSTRUCTION" "$ALIDPG_ROOT/MC/rec.C" rec.log 10
    mv syswatch.log recwatch.log
    RECEND=`date "+%s"`
    echo "RECTIME: $((RECEND-RECSTART))"
fi

runcommand "TAG" "tag.C" tag.log 50

runcommand "CHECK ESD" "CheckESD.C" check.log 60 1

rm -f *.RecPoints.root

if [ "$RUNMODE" = "SDD" ]; then
    if [ -f QAtrainsim_SDD.C ]; then
            runcommand "Running the QA train" "QAtrainsim_SDD.C(\"_wSDD\",$DC_RUN)" qa.log 100
    
            for file in *.stat; do
                mv $file ../$file.qa_wSDD
            done
    fi
    
    mv AliESDs.root AliESDs_wSDD.root
    mv AliESDfriends.root AliESDfriends_wSDD.root
    
    # Without SDD

    for logfile in rec.log qa.log tag.log check.log recwatch.log; do
            echo -e "\n\n* ------------ Without SDD ------------" >> $logfile
    done

    runcommand "RECONSTRUCTION without SDD" "recNoSDD.C" rec.log 11
    cat syswatch.log >> recwatch.log
    rm syswatch.log

    runcommand "TAG without SDD" "tag.C" tag.log 51

    runcommand "CHECK ESD without SDD" "CheckESD.C" check.log 61 1

    if [ -f QAtrainsim_SDD.C ]; then
            runcommand "Running the QA train without SDD" "QAtrainsim_SDD.C(\"\",$DC_RUN)" qa.log 101
    
            for file in *.stat; do
                mv $file ../$file.qa
            done
    fi
fi

rm -f *.RecPoints.root *.Hits.root *.Digits.root *.SDigits.root

exit 0
