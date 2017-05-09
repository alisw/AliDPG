#!/bin/bash

# Script to run:
#    1. reconstruction
#    2. calibration 
#
# Files assumed to be in working directory:
# recCPass0.C          - reconstruction macro
# runCalibTrain.C     - calibration/filtering macro
# Arguments (run locally):
#    1  - raw data file name
#    2  - number of events to be processed
#    3  - run number 

# example:
# runCPass0.sh raw.root  50  104892

##########################################################################
# Function to extract some information from rec.log
parseRecLog() {
    perl -e '

my $total = 0;
my $total_tpc = 0.0;
my $count_tpc = 0;
my $total_no_tpc = 0.0;
my $count_no_tpc = 0;
my $tpc_line = 0;

open FILE, "<", $ARGV[0] or die("Couldnt open file!");

while (my $row = <FILE>) {
  chomp $row;

  # grep logic
  $row =~ /TriggerCluster.+TPC/ and $tpc_line=1;

  if ($row =~ /End\sEvent.+CPU\s([\d|\.]+)\s/){
    if($tpc_line){ 
      $total_tpc += $1;
      $count_tpc += 1;
      $tpc_line = 0;
    }
    else{
      $total_no_tpc += $1;
      $count_no_tpc += 1;
    }
  }
}

close FILE;
$total = $count_tpc + $count_no_tpc;
print "found $total events in total \n";
print "found $count_tpc events with TPC \n";
print "found $count_no_tpc events without TPC \n";
$count_tpc and print "AVG TPC: ".($total_tpc/$count_tpc). "\n";
$count_no_tpc and print "AVG NO TPC (using $count_no_tpc events without TPC): ".($total_no_tpc/$count_no_tpc)."\n";


' $1 >&2
}
##########################################################################

#ALIEN setting
# $1 = raw input filename
tmpName=$(basename "$1")
runNumF="${tmpName:2:9}"
runNum=`echo "$runNumF" |  sed 's/^0*//'`
##runNum=`echo $1 | cut -d "/" -f 6 | sed 's/^0*//'`

# Exporting variable to define that we are in CPass0 to be used in reconstruction
export CPass='0'

# Set memory limits to a value lower than the hard site limits to at least get the logs of failing jobs
ulimit -S -v 3500000

# run TPC clusterization in separate process before reconstruction
export preclusterizeTPC='0'
# enabling it for 2015 PbPb
if [ "$runNum" -ge 244917 ] && [ "$runNum" -le 246994 ]; then
 preclusterizeTPC='1'
fi

if [ $# -lt 3 ]; then
    # alien Setup
    nEvents=99999999
    ocdbPath="raw://"
    
    # use the value passed by LPM, or by default use the kCalibBarrel alias
    triggerAlias=${ALIEN_JDL_TRIGGERALIAS-?Trigger=kCalibBarrel}
    #triggerAlias="?Trigger=kPhysicsAll"
fi

if [ $# -ge 4 ]; then
  # local setup
  nEvents=$2
  runNum=$3
  ocdbPath=$4
  triggerAlias="?Trigger=kCalibBarrel"
fi

if [ $# -eq 5 ]; then
    # local setup in case we provide the trigger mask
    # the trigger mask is first stripped of quotation characters
    triggerAlias=${5//\"/}
fi

# ===| TPC default values |===================================================
# can be overwritten by JDL below
# JDL will overwrite the config file
#
# ---| gain calibration |-----------------------------------------------------
# default in CPass0 is full calibration,
#    for number convention see AliTPCPreprocessorOffline::EGainCalibType
#
export TPC_CPass0_GainCalibType=1

# ===| TPC JDL overwrites |===================================================
#
export TPC_CPass0_GainCalibType=${ALIEN_JDL_TPC_CPASS0_GAINCALIBTYPE-$TPC_CPass0_GainCalibType}

echo "TPC_CPass0_GainCalibType=${TPC_CPass0_GainCalibType}" | tee -a calib.log

# CPass mode - 0 here
export CPASSMODE=0

export CPASSMODE=${ALIEN_JDL_LPMCPASSMODE-$CPASSMODE}

echo "CPASSMODE = ${CPASSMODE}" | tee -a calib.log

if [ -f Run0_999999999_v3_s0.root ]; then
    mkdir -p TPC/Calib/Correction
    mv Run0_999999999_v3_s0.root TPC/Calib/Correction/
fi

echo "File to be  processed $1"
echo "Number of events to be processed $nEvents"

echo "* PATH: $PATH"
echo "* LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo ""

if [ "$2" == "OCDB" ]; then
    echo "Generating OCDB.root only"
    export OCDB_SNAPSHOT_CREATE="kTRUE"
    export OCDB_SNAPSHOT_FILENAME="OCDB.root"
    touch OCDB.generating.job
fi

CHUNKNAME="$1"

if [ "${CHUNKNAME:0:1}" = "/" ]; then
    FILENAME=${CHUNKNAME##*/}

    if [ -f "$FILENAME" ]; then
        # locally downloaded chunk
        CHUNKNAME="`pwd`/$FILENAME"
    else
        # one chunk from alien (nodownload option to the collection)
        CHUNKNAME="alien://$CHUNKNAME"
    fi
fi

if [ -f "wn.xml" ]; then
    CHUNKNAME="collection://wn.xml"
fi

echo "* ************************"
echo "* Running AliRoot to reconstruct $*"
echo "* Chunk name: $CHUNKNAME"
echo "* Run number: $runNum"
echo "* nEvents: $nEvents"
echo "* ocdbPath: $ocdbPath"
echo "* triggerAlias: $triggerAlias"
echo "* ************************"
echo "* Printing ALL env variables"
printenv
echo ""

timeUsed=0

# Extraction of TPC clusters - only if not in OCDB snapshot mode, and if requested, and if the rawToClust.C macro is there
if [ -f raw2clust.C ]; then
    echo "Use raw2clust.C macro passed as input"
else
    echo "Use raw2clust.C macro from AliDPG"
    cp $ALIDPG_ROOT/DataProc/Common/raw2clust.C .
fi

if [ "$preclusterizeTPC" = "1" ] && [ "$2" != "OCDB" ] && [ -f raw2clust.C ]; then
    echo ""
    echo "running the following raw2clust.C macro:"
    cat raw2clust.C
    echo ""
    echo "" >&2
    echo "raw2clust.C" >&2
    echo executing aliroot -l -b -q -x "raw2clust.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")"
    timeStart=`date +%s`
    time aliroot -l -b -q -x "raw2clust.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")" &> clust.log
    exitcode=$?
    timeEnd=`date +%s`
    timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
    delta=$(( $timeEnd-$timeStart ))
    echo "raw2clust:   delta = $delta, timeUsed so far = $timeUsed"
    echo "raw2clust:   delta = $delta, timeUsed so far = $timeUsed" >&2
    echo "*! Exit code of raw2clust.C: $exitcode"
    if [ $exitcode -ne 0 ]; then
	echo "raw2clust.C exited with code $exitcode" > validation_error.message
	exit 10
    fi
# Cleanup after the extraction of TPC clusters
    rm -f galice.root AliESDs.root Run*.root Trigger.root QA.root
fi


if [ -f recCPass0.C ]; then
    echo "Use recCPass0.C macro passed as input"
else
    echo "Use recCPass0.C macro from AliDPG"
    cp $ALIDPG_ROOT/DataProc/CPass0/recCPass0.C .
fi

echo ""
echo "running the following recCPass0.C macro:"
cat recCPass0.C
echo ""
echo executing aliroot -l -b -q -x "recCPass0.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")"
echo "" >&2
echo "recCPass0.C" >&2
timeStart=`date +%s`
time aliroot -l -b -q -x "recCPass0.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")" &> rec.log
exitcode=$?
timeEnd=`date +%s`
timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
delta=$(( $timeEnd-$timeStart ))
echo "recCPass0:  delta = $delta, timeUsed so far = $timeUsed"
echo "recCPass0:  delta = $delta, timeUsed so far = $timeUsed" >&2
parseRecLog rec.log

echo "*! Exit code of recCPass0.C: $exitcode"

if [ $exitcode -ne 0 ]; then
    echo "recCPass0.C exited with code $exitcode" > validation_error.message
    exit 10
fi

mv syswatch.log syswatch_rec.log

if [ "$2" == "OCDB" ]; then
    echo "*! Reconstruction ran in fake mode to create OCDB.root, exiting quickly now"

    if [ -f OCDB.root ]; then
        echo "* OCDB.root was indeed produced"
    else
        echo "! Error: OCDB.root was NOT generated !!!"
        echo "OCDB.root was not generated" > validation_error.message
        exit 1
    fi
    
    exit 0
fi

if [ -n "$preclusterizeTPC" ] && [ -f TPC.RecPoints.root ]; then
# clean TPC recpoints
    echo "Removing preproduced TPC recpoints"
    rm TPC.RecPoints.root
fi

echo "* Running AliRoot to make calibration..."
if [ -f runCalibTrain.C ]; then
    echo "Use runCalibTrain.C macro passed as input"
else
    echo "Use runCalibTrain.C macro from AliDPG"
    cp $ALIDPG_ROOT/DataProc/CPass0/runCalibTrain.C .
fi

echo ""
echo "running the following runCalibTrain.C macro:"
cat runCalibTrain.C
echo ""
echo executing aliroot -l -b -q -x "runCalibTrain.C($runNum,\"AliESDs.root\",\"$ocdbPath\")"
echo "" >&2
echo "runCalibTrain.C" >&2
timeStart=`date +%s`
time aliroot -l -b -q -x "runCalibTrain.C($runNum,\"AliESDs.root\",\"$ocdbPath\")" &>> calib.log
exitcode=$?
timeEnd=`date +%s`
timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
delta=$(( $timeEnd-$timeStart ))
echo "calibTrain: delta = $delta, timeUsed so far = $timeUsed"
echo "calibTrain: delta = $delta, timeUsed so far = $timeUsed" >&2

echo "*! Exit code of runCalibTrain.C: $exitcode"

if [ -f ResidualHistos.root ]; then
    mv ResidualHistos.root ResidualTrees.root
fi

if [ $exitcode -ne 0 ]; then
    echo "runCalibTrain.C exited with code $exitcode" > validation_error.message
    exit 40
fi

echo "*  Running filtering task *"
filtMacro=$ALICE_PHYSICS/PWGPP/macros/runFilteringTask.C
if [ -f $filtMacro ]; then
    echo ""
    echo "running the following runFilteringTask.C macro:"
    cat $filtMacro
    echo ""
    echo AliESDs.root > esd.list
    echo executing aliroot -l -b -q "${filtMacro}(\"esd.list\",1000,100,\"${ocdbPath}\")"
    echo "" >&2
    echo "runFilteringTask.C" >&2
    timeStart=`date +%s`
    time aliroot -l -b -q "${filtMacro}(\"esd.list\",1000,100,\"${ocdbPath}\")" &> filtering.log
    timeEnd=`date +%s`
    timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
    delta=$(( $timeEnd-$timeStart ))
    echo "filtering:  delta = $delta, timeUsed so far = $timeUsed"
    echo "filtering:  delta = $delta, timeUsed so far = $timeUsed" >&2
else
    echo "no ${filtMacro} ..."
fi

echo "The time spent in aliroot sessions is $timeUsed"

if [ $exitcode -ne 0 ]; then
    echo "runCalibTrain.C exited with code $exitcode" > validation_error.message
    exit 40
fi

exit 0
