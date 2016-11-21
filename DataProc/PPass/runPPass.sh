#!/bin/bash

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

echo ALICE_ROOT = $ALICE_ROOT
echo AliROOT = $AliROOT
cp $ALICE_ROOT/.rootrc ~/.rootrc
cp $ALICE_ROOT/.rootrc $HOME
#cat $HOME/.rootrc
export GRID_TOKEN=OK
export XRD_TRANSACTIONTIMEOUT=300

export PRODUCTION_METADATA="$ALIEN_JDL_LPMMETADATA"

echo "* PATH: $PATH"
echo "* LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo "* rec.C:"
cat rec.C
echo


ls -l

# $1 = raw input filename
CHUNKNAME="$1"
shift

runstripped=0
if [ -z "$LPMRunNumber" ]; then
  tmpName=$(basename "$CHUNKNAME")
  runnum="${tmpName:2:9}"
  runstripped=`echo $runnum | sed 's/^0*//'`
else
  runstripped=$LPMRunNumber
fi

echo runstripped is $runstripped
#runnum=`echo $CHUNKNAME | cut -d "/" -f 6`

# run TPC clusterization in separate process before reconstruction
export preclusterizeTPC='0'
if [ "$runstripped" -ge 244917 ] && [ "$runstripped" -le 246994 ]; then
 preclusterizeTPC='1'
fi


if [ "$1" == "OCDB" ]; then
    echo "* Generating OCDB.root only"
    export OCDB_SNAPSHOT_CREATE="kTRUE"
    export OCDB_SNAPSHOT_FILENAME="OCDB.root"

    shift
fi

RECO_ARGS=""

if [ "$1" == "SPLIT" ]; then
    RECO_ARGS=",$2,$3"
    shift 3
fi

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

echo ""
echo " Printing ALL env variables"
printenv
echo ""

timeUsed=0

# Extraction of TPC clusters
if [ "$preclusterizeTPC" = "1" ] && [ "$OCDB_SNAPSHOT_CREATE" != "kTRUE" ] && [ -f raw2clust.C ]; then
    echo ""
    echo "running the following raw2clust.C macro:"
    cat raw2clust.C
    echo ""
    echo "" >&2
    echo "raw2clust.C" >&2
    echo executing aliroot -l -b -q -x "raw2clust.C(\"$CHUNKNAME\"$RECO_ARGS)"
    timeStart=`date +%s`
    time aliroot -l -b -q -x "raw2clust.C(\"$CHUNKNAME\"$RECO_ARGS)" &> clust.log
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


echo "* Running AliRoot to reconstruct '$CHUNKNAME', extra arguments are '$RECO_ARGS' and run number is $runnum ..."
echo ""
echo "running the following rec.C macro:"
cat rec.C
echo ""
echo executing aliroot -l -b -q -x "rec.C(\"$CHUNKNAME\"$RECO_ARGS)"
echo "" >&2
echo "rec.C" >&2
timeStart=`date +%s`
time aliroot -l -b -q -x "rec.C(\"$CHUNKNAME\"$RECO_ARGS)" &> rec.log

exitcode=$?
timeEnd=`date +%s`
timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
delta=$(( $timeEnd-$timeStart ))
echo "rec:  delta = $delta, timeUsed so far = $timeUsed"
echo "rec:  delta = $delta, timeUsed so far = $timeUsed" >&2
parseRecLog rec.log

echo "Exit code: $exitcode"

if [ $exitcode -ne 0 ]; then
    echo "rec.C exited with code $exitcode" > validation_error.message
    exit 10
fi

if [ "$OCDB_SNAPSHOT_CREATE" == "kTRUE" ]; then
    echo "* Reconstruction ran in fake mode to create OCDB.root, exiting quickly now"
    touch OCDB.generating.job

    if [ -f OCDB.root ]; then
        echo "*  OCDB.root was indeed produced"
    else
        echo "!!! Error: OCDB.root was NOT generated !!!"
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


echo "* Running AliRoot to generate Tags..."
echo ""
echo "running the following tag.C macro:"
cat tag.C
echo ""
echo "" >&2
echo "tag.C" >&2
echo executing aliroot -l -b -q -x tag.C\(\)
timeStart=`date +%s`
time aliroot -l -b -q -x tag.C\(\) &> tag.log

exitcode=$?
timeEnd=`date +%s`
timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
delta=$(( $timeEnd-$timeStart ))
echo "tag:  delta = $delta, timeUsed so far = $timeUsed"
echo "tag:  delta = $delta, timeUsed so far = $timeUsed" >&2
echo "Exit code: $exitcode"

if [ $exitcode -ne 0 ]; then
    echo "tag.C exited with code $exitcode" > validation_error.message
    exit 50
fi

for file in *.stat; do
    mv $file $file.rec
done

if [ -f QAtrain.C ]; then
    echo "* Running the QA train..."
    echo ""
    echo "running the following QAtrain.C macro:"
    cat QAtrain.C
    echo ""
    echo executing aliroot -b -q -x QAtrain.C\($runstripped\)
    echo "" >&2
    echo "QAtrain.C" >&2
    timeStart=`date +%s`
    time aliroot -b -q -x QAtrain.C\($runstripped\) &> qa.log
    echo "QAresults.root" >> validation_extrafiles.list

    exitcode=$?
    timeEnd=`date +%s`
    timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
    delta=$(( $timeEnd-$timeStart ))
    echo "QA:  delta = $delta, timeUsed so far = $timeUsed"
    echo "QA:  delta = $delta, timeUsed so far = $timeUsed" >&2
    echo "Exit code: $exitcode"
    
    if [ $exitcode -ne 0 ]; then
        echo "QAtrain.C exited with code $exitcode" > validation_error.message
        exit 100
    fi

    for file in *.stat; do
        mv $file $file.qa
    done
fi

if [ -f QAtrain_duo.C ]; then
    echo "QAresults.root" >> validation_extrafiles.list
    echo ""
    echo "running the following QAtrain_duo.C macro:"
    cat QAtrain_duo.C
    echo ""

    for grp in 0 1 2 3 4; do
        export QAGROUP=$grp
	echo "* Running QA for tasks group $QAGROUP"
	echo executing aliroot -b -q -x "QAtrain_duo.C(\"_grp$grp\",$runstripped)"
	echo "" >&2
	echo "QAtrain_duo.C / grp$grp" >&2
	timeStart=`date +%s`
	time aliroot -b -q -x "QAtrain_duo.C(\"_grp$grp\",$runstripped)" &>> qa.log

    	exitcode=$?
	timeEnd=`date +%s`
	timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
	delta=$(( $timeEnd-$timeStart ))
	echo "QA_$grp:   delta = $delta, timeUsed so far = $timeUsed"	
	echo "QA_$grp:   delta = $delta, timeUsed so far = $timeUsed" >&2
	echo "Exit code: $exitcode"
	if [ $exitcode -ne 0 ]; then
            echo "!!! QAtrain_duo.C / barrel group $grp exited with code $exitcode" > validation_error.message
            # put the partial results in the main folder so they are registered, for QA debugging purposes
            exit 100
    	fi
        
        for file in *.stat; do
            mv $file $file.qa_grp$grp
        done
    done
    
    if [ -f mergeQAgroups.C ]; then
	echo ""
	echo "running the following mergeQAgroups.C macro:"
	cat mergeQAgroups.C
	echo ""
        lstQA="lstQA.txt"
	ls QAresults_grp*.root > $lstQA
	echo executing aliroot -b -q -x "mergeQAgroups.C(\"$lstQA\",\"QAresults.root\")"
	echo "" >&2
        echo "mergeQAgroups.C" >&2
	timeStart=`date +%s`
	time aliroot -b -q -x "mergeQAgroups.C(\"$lstQA\",\"QAresults.root\")"  &> mergeQA.log
 	timeEnd=`date +%s`
	timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
	delta=$(( $timeEnd-$timeStart ))
	echo "QAmerging: delta = $delta, timeUsed so far = $timeUsed"
	echo "QAmerging: delta = $delta, timeUsed so far = $timeUsed" >&2
    else
	    echo "!!! No mergeQAgroups.C macro in current directory, cannot merge..."
    fi
fi

if [ -f AODtrain.C ]; then
    rm -f outputs_valid &>/dev/null
    echo "AliAOD.root" >> validation_extrafiles.list

    echo "* Running the FILTERING train..."
    echo ""
    echo "running the following AODtrain.C macro:"
    cat AODtrain.C
    echo ""
    echo executing aliroot -b -q  -x AODtrain.C\(\)
    echo "" >&2
    echo "AODtrain.C" >&2
    timeStart=`date +%s`
    time aliroot -b -q  -x AODtrain.C\(\) &> aod.log

    exitcode=$?
    timeEnd=`date +%s`
    timeUsed=$(( $timeUsed+$timeEnd-$timeStart ))
    delta=$(( $timeEnd-$timeStart ))
    echo "AOD: delta = $delta, timeUsed so far = $timeUsed"
    echo "AOD: delta = $delta, timeUsed so far = $timeUsed" >&2
    echo "Exit code: $exitcode"
    
    if [ $exitcode -ne 0 ]; then
        echo "AODtrain.C exited with code $exitcode" > validation_error.message
        exit 200
    fi

    for file in *.stat; do
        mv $file $file.aod
    done
fi

exit 0
