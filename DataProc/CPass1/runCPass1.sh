#!/bin/bash

# Script to run:
#    1. reconstruction
#    2. calibration 
#
# Files assumed to be in working directory:
# recCPass1.C          - reconstruction macro
# runCalibTrain.C     - calibration/filtering macro
# Arguments (run locally):
#    1  - raw data file name
#    2  - number of events to be processed
#    3  - run number 

# example:
# runCPass1.sh raw.root  50  104892

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

defAlias="kCalibBarrel"
#default alias for CPass1 2015 PbPb is kCalibBarrelMB
if [ "$runNum" -ge 244917 ] && [ "$runNum" -le 246994 ]; then 
defAlias="kCalibBarrelMB"
fi	

#optionallly skip Outer pass, set to true for 2015 PbPb
export outerUsesFullITS=1
if [ "$runNum" -ge 244917 ] && [ "$runNum" -le 246994 ]; then
export skipOuter='1'
outerUsesFullITS=0
fi

# Exporting variable to define that we are in CPass1 to be used in reconstruction
export CPass='1'

export PRODUCTION_METADATA="$ALIEN_JDL_LPMMETADATA"

# Set memory limits to a value lower than the hard site limits to at least get the logs of failing jobs
ulimit -S -v 3500000

# run TPC clusterization in separate process before reconstruction
export preclusterizeTPC='0'
# enabling it for 2015 PbPb
if [ "$runNum" -ge 244917 ] && [ "$runNum" -le 246994 ]; then
 preclusterizeTPC='1'
fi



if [ $# -eq 1 ]; then
    # alien Setup
    nEvents=99999999
    ocdbPath="raw://"
    # use the value passed by LPM, or by default use the kCalibBarrel alias
    triggerAlias=${ALIEN_JDL_TRIGGERALIAS-?Trigger=$defAlias}
    #triggerAlias="?Trigger=kPhysicsAll"
fi

if [ $# -ge 4 ]; then
    # local setup
    nEvents=$2
    runNum=$3
    ocdbPath=$4
    triggerAlias="?Trigger=$defAlias"
fi

if [ $# -eq 5 ]; then
    # local setup in case we specify the trigger mask
    triggerAlias=$5
fi

# ===| TPC default values |===================================================
# can be overwritten by config file, or JDL below
# JDL will overwrite the config file
#
# ---| gain calibration |-----------------------------------------------------
# default in CPass1 is combined calibration + residual QA,
#    for number convention see AliTPCPreprocessorOffline::EGainCalibType
#
export TPC_CPass1_GainCalibType=3

# ===| TPC JDL overwrites |===================================================
#
export TPC_CPass1_GainCalibType=${ALIEN_JDL_TPC_CPASS1_GAINCALIBTYPE-$TPC_CPass1_GainCalibType}

echo "TPC_CPass1_GainCalibType=${TPC_CPass1_GainCalibType}" | tee -a calib.log

# CPass mode - 1 here
export CPASSMODE=1

export CPASSMODE=${ALIEN_JDL_LPMCPASSMODE-$CPASSMODE}

echo "CPASSMODE = ${CPASSMODE}" | tee -a calib.log

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
    # several chunks accessed remotely
    CHUNKNAME="collection://wn.xml"
fi

echo "* ************************"
echo "* runCPass1.sh $*"
echo "* Chunk name: $CHUNKNAME"
echo "* Run number: $runNum"
echo "* nEvents: $nEvents"
echo "* runNum: $runNum"
echo "* ocdbPath: $ocdbPath"
echo "* triggerAlias: $triggerAlias"
echo "* ************************"
echo "* Printing ALL env variables"
printenv
echo ""
timeUsed=0

mkdir Barrel OuterDet
if [ -n  "$skipOuter" ]; then
    echo "Outer pass will be skept"
fi

if [ -f Run0_999999999_v3_s0.root ]; then
    echo "* TPC correction file found"

    mkdir -p TPC/Calib/Correction
    mv Run0_999999999_v3_s0.root TPC/Calib/Correction/

    for DIR in Barrel OuterDet; do
        mkdir -p $DIR/TPC/Calib/Correction
        ln -s ../../../../TPC/Calib/Correction/Run0_999999999_v3_s0.root $DIR/TPC/Calib/Correction/Run0_999999999_v3_s0.root
    done
fi

echo "* PATH: $PATH"
echo "* LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo

if [ -f raw2clust.C ]; then
    echo "Use raw2clust.C macro passed as input"
else
    echo "Use raw2clust.C macro from AliDPG"
    cp $ALIDPG_ROOT/DataProc/Common/raw2clust.C .
fi

if [ -f recCPass1.C ]; then
    echo "Use recCPass1.C macro passed as input"
else
    echo "Use recCPass1.C macro from AliDPG"
    cp $ALIDPG_ROOT/DataProc/CPass1/recCPass1.C .
fi

if [ -f runCalibTrain.C ]; then
    echo "Use runCalibTrain.C macro passed as input"
else
    echo "Use runCalibTrain.C macro from AliDPG"
    cp $ALIDPG_ROOT/DataProc/CPass1/runCalibTrain.C .
fi

if [ -f QAtrain_duo.C ]; then
    echo "Use QAtrain_duo.C macro passed as input"
else
    echo "Use QAtrain_duo.C macro from AliDPG"
    cp $ALIDPG_ROOT/QA/QAtrain_duo.C .
fi

if [ -f mergeQAgroups.C ]; then
    echo "Use mergeQAgroups.C macro passed as input"
else
    echo "Use mergeQAgroups.C macro from AliDPG"
    cp $ALIDPG_ROOT/QA/mergeQAgroups.C .
fi

if [ -f recCPass1_OuterDet.C ]; then
    echo "Use recCPass1_OuterDet.C macro passed as input"
else
    echo "Use recCPass1_OuterDet.C macro from AliDPG"
    cp $ALIDPG_ROOT/DataProc/CPass1/recCPass1_OuterDet.C .
fi


if [ "$2" == "OCDB" ]; then
    export OCDB_SNAPSHOT_CREATE="kTRUE"
    export OCDB_SNAPSHOT_FILENAME="OCDB.root"
    touch OCDB.generating.job

    echo "* Running AliRoot to generate the OCDB based on $CHUNKNAME"

    echo "OCDB/recCPass1.C" >&2
    echo ""
    echo "running the following recCPass1.C macro:"
    cat recCPass1.C
    echo ""
    echo executing aliroot -l -b -q -x recCPass1.C\(\"$CHUNKNAME\"\)
    timeStart=`date +%s`
    time aliroot -l -b -q -x recCPass1.C\(\"$CHUNKNAME\"\) &> rec.log
    exitcode=$?
    timeEnd=`date +%s`
    timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
    delta=$(( $timeEnd-$timeStart ))
    echo "OCDBsnapshot:   delta = $delta, timeUsed so far = $timeUsed"
    echo "OCDBsnapshot:   delta = $delta, timeUsed so far = $timeUsed" >&2
    echo "Exit code: $exitcode"
    
    if [ $exitcode -ne 0 ]; then
        echo "recCPass1.C for OCDB snapshot exited with code $exitcode" > validation_error.message
        exit 10
    fi

    echo "* Reconstruction ran in fake mode to create OCDB.root, exiting quickly now"

    if [ -f OCDB.root ]; then
        echo "* OCDB.root was indeed produced"
    else
        echo "* Error: OCDB.root was NOT generated !!!"
        echo "OCDB.root was not generated" > validation_error.message
        exit 1
    fi

    exit 0
fi

for COMMON_FILE in wn.xml localOCDBaccessConfig.C AddTaskTPCCalib.C AddTaskTRDCalib.C OCDB.root QAtrain_duo.C mergeQAgroups.C; do
    if [ -f $COMMON_FILE ]; then
        ln -s ../$COMMON_FILE Barrel/$COMMON_FILE
        ln -s ../$COMMON_FILE OuterDet/$COMMON_FILE
    fi
done

for BARREL_FILE in recCPass1.C runCalibTrain.C; do
    ln -s ../$BARREL_FILE Barrel/$BARREL_FILE
done

if [ -f raw2clust.C ]; then ln -s ../raw2clust.C Barrel/raw2clust.C; fi

for OUTER_FILE in recCPass1_OuterDet.C; do
    ln -s ../$OUTER_FILE OuterDet/$OUTER_FILE
done

####################################   Barrel   #######################################

cd Barrel

echo ""
echo "* Running AliRoot to reconstruct barrel of $CHUNKNAME"
echo ""

# Extraction of TPC clusters - only if not in OCDB snapshot mode, and if requested, and if the rawToClust.C macro is there
if [ "$preclusterizeTPC" = "1" ] && [ "$2" != "OCDB" ] && [ -f raw2clust.C ]; then
    echo ""
    echo "running the following raw2clust.C macro:"
    cat raw2clust.C
    echo ""
    echo "" >&2
    echo "barrel: raw2clust.C" >&2
    echo executing aliroot -l -b -q -x "raw2clust.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")"
    timeStart=`date +%s`
    time aliroot -l -b -q -x "raw2clust.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")" &> ../clust.log
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

echo ""
echo "" >&2
echo "barrel: recCPass1.C" >&2
echo ""
echo "running the following recCPass1.C macro:"
cat recCPass1.C
echo ""
echo executing aliroot -l -b -q -x "recCPass1.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")"
timeStart=`date +%s`
time aliroot -l -b -q -x "recCPass1.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", \"$triggerAlias\")" &> ../rec.log
exitcode=$?
timeEnd=`date +%s`
timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
delta=$(( $timeEnd-$timeStart ))
echo "recCPass1:  delta = $delta, timeUsed so far = $timeUsed"
echo "recCPass1:  delta = $delta, timeUsed so far = $timeUsed" >&2
parseRecLog ../rec.log

echo "Exit code: $exitcode"

if [ $exitcode -ne 0 ]; then
    echo "recCPass1.C exited with code $exitcode" > ../validation_error.message
    exit 10
fi

if [ -n "$preclusterizeTPC" ] && [ -f TPC.RecPoints.root ]; then
# clean TPC recpoints
    echo "Removing preproduced TPC recpoints"
    rm TPC.RecPoints.root
fi

mv syswatch.log ../syswatch_rec_Barrel.log

echo ""
echo "* Running AliRoot to make calibration..."
echo "" 
echo "running the following runCalibTrain.C macro:"
cat runCalibTrain.C
echo ""

echo "" >&2
echo "barrel: runCalibTrain.C" >&2
echo executing aliroot -l -b -q -x "runCalibTrain.C($runNum,\"AliESDs.root\",\"$ocdbPath\")"
time aliroot -l -b -q -x "runCalibTrain.C($runNum,\"AliESDs.root\",\"$ocdbPath\")" &>> ../calib.log
exitcode=$?
timeEnd=`date +%s`
timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
delta=$(( $timeEnd-$timeStart ))
echo "calibTrain: delta = $delta, timeUsed so far = $timeUsed"
echo "calibTrain: delta = $delta, timeUsed so far = $timeUsed" >&2
echo "Exit code: $exitcode"

if [ $exitcode -ne 0 ]; then
    echo "runCalibTrain.C exited with code $exitcode" > ../validation_error.message
    exit 40
fi

mv syswatch.log ../syswatch_calib.log

if [ -f ResidualHistos.root ]; then
    mv ResidualHistos.root ../ResidualTrees.root
fi

echo "" 
echo "*  Running filtering task for barrel *"
filtMacro=$ALICE_PHYSICS/PWGPP/macros/runFilteringTask.C
if [ -f $filtMacro ]; then
    echo "" 
    echo "running the following runFilteringTask.C macro:"
    cat $filtMacro
    echo ""
    echo AliESDs.root > esd.list
    echo "" >&2
    echo "barrel: runFilteringTask" >&2
    timeStart=`date +%s`
    time aliroot -l -b -q "${filtMacro}(\"esd.list\",10000,1000,\"${ocdbPath}\")" &> ../filtering.log
    timeEnd=`date +%s`
    timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
    delta=$(( $timeEnd-$timeStart ))
    echo "filtering:  delta = $delta, timeUsed so far = $timeUsed"
    echo "filtering:  delta = $delta, timeUsed so far = $timeUsed" >&2
else
    echo "no ${filtMacro} ..."
fi

#
if [ -f QAtrain_duo.C ]; then
    echo "" 
    echo "* Running the QA train (barrel) ..."
    echo "running the following QAtrain_duo.C macro:"
    cat QAtrain_duo.C
    echo ""

#    echo executing aliroot -b -q "QAtrain_duo.C(\"_barrel\",$runNum,\"$ocdbPath\")"
#    time aliroot -b -q "QAtrain_duo.C(\"_barrel\",$runNum,\"$ocdbPath\")" &> ../qa_barrel.log

    for grp in 0 1 2 3 4 
    do
	export QAGROUP=$grp
	echo "" >&2
	echo "barrel: QAtrain_duo.C / grp$grp" >&2
	echo running QA for tasks group $QAGROUP
	echo executing aliroot -b -q -x "QAtrain_duo.C(\"_barrel_grp$grp\",$runNum,0,0,\"$ocdbPath\")"
	timeStart=`date +%s`
	time aliroot -b -q -x "QAtrain_duo.C(\"_barrel_grp$grp\",$runNum,0,0,\"$ocdbPath\")" >> ../qa_barrel.log 2>&1
#
	exitcode=$?
	timeEnd=`date +%s`
	timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
	delta=$(( $timeEnd-$timeStart ))
	echo "QA_$grp:   delta = $delta, timeUsed so far = $timeUsed"	
	echo "QA_$grp:   delta = $delta, timeUsed so far = $timeUsed" >&2
	echo "Exit code: $exitcode"
	if [ $exitcode -ne 0 ]; then
            echo "QAtrain_duo.C / barrel group $grp exited with code $exitcode" > ../validation_error.message
        # put the partial results in the main folder so they are registered, for QA debugging purposes
            mv AliESDs.root ../AliESDs_Barrel.root
            mv AliESDfriends.root ../AliESDfriends_Barrel.root
            exit 100
	fi
#
    done
#
    if [ -f mergeQAgroups.C ]; then
	echo ""
	echo "running the following mergeQAgroups.C macro:"
	cat mergeQAgroups.C
	echo ""
	lstQAbarrel="lstQAbarrel.txt"
	ls QAresults_barrel_grp*.root > $lstQAbarrel
	echo "" >&2
	echo "barrel: mergeQAgroups.C" >&2
	echo executing aliroot -b -q -x "mergeQAgroups.C(\"$lstQAbarrel\",\"QAresults_barrel.root\")"
	timeStart=`date +%s`
	time aliroot -b -q -x "mergeQAgroups.C(\"$lstQAbarrel\",\"QAresults_barrel.root\")"  >> ../qa_barrel.log 2>&1
	timeEnd=`date +%s`
	timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
	delta=$(( $timeEnd-$timeStart ))
	echo "QAmerging: delta = $delta, timeUsed so far = $timeUsed"
	echo "QAmerging: delta = $delta, timeUsed so far = $timeUsed" >&2
    else
	echo "no mergeQAgroups.C macro in current directory, cannot merge..."
    fi
 #  

    for file in *.stat; do
        mv $file ../$file.qa_barrel
    done
fi

mv AliESDs.root ../AliESDs_Barrel.root
mv AliESDfriends.root ../AliESDfriends_Barrel.root

for file in FilterEvents_Trees.root AliESDfriends_v1.root QAresults_barrel.root EventStat_temp_barrel_grp*.root AODtpITS.root Run*.Event*_*.ESD.tag.root TOFcalibTree.root T0AnalysisTree.root CalibObjects.root; do
    if [ -f "$file" ]; then
        mv "$file" ../
    fi
done

# cleanup of barrel
cd ../
mv EventStat_temp_barrel_grp0.root EventStat_temp_barrel.root
rm EventStat_temp_*_grp*.root

echo "The time spent in aliroot sessions is $timeUsed"

if [ -n  "$skipOuter" ]; then
  exit 0
fi

####################################   Outer   #######################################

cd OuterDet

echo ""
echo "* Running AliRoot to reconstruct outer of $CHUNKNAME"

echo "" >&2
echo "outer: recCPass1_OuterDet.C" >&2
echo ""
echo "running the following recCPass1_OuterDet.C macro:"
cat recCPass1_OuterDet.C
echo ""
echo executing aliroot -l -b -q -x "recCPass1_OuterDet.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", $outerUsesFullITS)"
timeStart=`date +%s`
time aliroot -l -b -q -x "recCPass1_OuterDet.C(\"$CHUNKNAME\", $nEvents, \"$ocdbPath\", $outerUsesFullITS)" &> ../rec_Outer.log
exitcode=$?
timeEnd=`date +%s`
timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
delta=$(( $timeEnd-$timeStart ))
echo "recCPass1_outer: delta = $delta, timeUsed so far = $timeUsed"
echo "recCPass1_outer: delta = $delta, timeUsed so far = $timeUsed" >&2
echo "Exit code: $exitcode"

if [ $exitcode -ne 0 ]; then
    echo "recCPass1_OuterDet.C exited with code $exitcode" > ../validation_error.message
    exit 11
fi

mv syswatch.log ../syswatch_rec_Outer.log

if [ -f QAtrain_duo.C ]; then
    echo "* Running the QA train (outer) ..."
    echo ""
    echo "running the following QAtrain_duo.C macro:"
    cat QAtrain_duo.C
    echo ""

#    echo executing aliroot -b -q "QAtrain_duo.C(\"_outer\",$runNum,\"$ocdbPath\")"
#    time aliroot -b -q "QAtrain_duo.C(\"_outer\",$runNum,\"$ocdbPath\")" &> ../qa_outer.log

    for grp in 0 4 
    do
	export QAGROUP=$grp
	echo running QA for tasks group $QAGROUP
	echo "" >&2
	echo "outer: QAtrain_duo.C / grp$grp" >&2
	echo executing aliroot -b -q -x "QAtrain_duo.C(\"_outer_grp$grp\",$runNum,0,0,\"$ocdbPath\")"
	timeStart=`date +%s`
	time aliroot -b -q -x "QAtrain_duo.C(\"_outer_grp$grp\",$runNum,0,0,\"$ocdbPath\")" >> ../qa_outer.log 2>&1
#
	exitcode=$?
	timeEnd=`date +%s`
	timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
	delta=$(( $timeEnd-$timeStart ))
	echo "QA_$grp_outer:   delta = $delta, timeUsed so far = $timeUsed"
	echo "QA_$grp_outer:   delta = $delta, timeUsed so far = $timeUsed" >&2
	echo "Exit code: $exitcode"
	if [ $exitcode -ne 0 ]; then
            echo "QAtrain_duo.C  / outer group $grp exited with code $exitcode" > ../validation_error.message
        # put the partial results in the main folder so they are registered, for QA debugging purposes
            mv AliESDs.root ../AliESDs_Outer.root
            mv AliESDfriends.root ../AliESDfriends_Outer.root
            exit 101
	fi
    done
#
    lstQAouter="lstQAouter.txt"
    ls QAresults_outer_grp*.root > $lstQAouter
    echo "" >&2
    echo "outer: mergeQAgroups.C" >&2
    echo executing aliroot -b -q -x "mergeQAgroups.C(\"$lstQAouter\",\"QAresults_outer.root\")"
    echo ""
    echo "running the following mergeQAgroups.C macro:"
    cat mergeQAgroups.C
    echo ""
    timeStart=`date +%s`
    time aliroot -b -q -x "mergeQAgroups.C(\"$lstQAouter\",\"QAresults_outer.root\")"  >> ../qa_outer.log 2>&1
    timeEnd=`date +%s`
    delta=$(( $timeEnd-$timeStart ))
    timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
    echo "QA_merging_outer: delta = $delta, timeUsed so far = $timeUsed"
    echo "QA_merging_outer: delta = $delta, timeUsed so far = $timeUsed" >&2
    
 #   

    for file in *.stat ; do
        mv $file ../$file.qa_outer
    done
fi

mv AliESDs.root ../AliESDs_Outer.root
mv AliESDfriends.root ../AliESDfriends_Outer.root

for file in QAresults_outer.root EventStat_temp_outer_grp*.root; do
    if [ -f "$file" ]; then
        mv "$file" ../
    fi
done

# cleanup of outer
cd ..
mv EventStat_temp_outer_grp0.root EventStat_temp_outer.root
rm EventStat_temp_*_grp*.root

echo "The time spent in aliroot sessions is $timeUsed"

exit 0
