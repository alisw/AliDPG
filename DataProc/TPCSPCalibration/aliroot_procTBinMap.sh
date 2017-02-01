#!/bin/bash

export PRODUCTION_METADATA="$ALIEN_JDL_LPMMETADATA"

###

#if [ "$1" = "OCDB" ]; then
#    echo "Setting env for generating OCDB.root"
#
#    export OCDB_SNAPSHOT_CREATE="kTRUE"
#    export OCDB_SNAPSHOT_FILENAME="OCDB.root"
#
#    touch OCDB.generating.job
#
#    shift
#fi

# unset ALIDPG_ROOT in case you find the tgz, so that it can be set again later
if [ -f "alidpg.tgz" ]; then
    export ALIDPG_ROOT=
fi

    ### check environment

if [ "$ALIDPG_ROOT" = "" ]; then

    if [ -f "alidpg.tgz" ]; then

	echo "Using AliDPG from tarball"
	tar zxvf alidpg.tgz
	export ALIDPG_ROOT=AliDPG
	
    else
	
	echo "*! ERROR: ALIDPG_ROOT is not set!"
	echo "ERROR: ALIDPG_ROOT is not set!" > validation_error.message
	exit
	
    fi
fi


### executable

EXECUTABLE=$ALIDPG_ROOT/DataProc/TPCSPCalibration/procTBinMap.sh
if [ -f procTBinMap.sh ]; then
    chmod +x procTBinMap.sh
    EXECUTABLE=./procTBinMap.sh
fi
echo "Calling '$EXECUTABLE $*'"
$EXECUTABLE $*
error=$?

if [ $error -ne 0 ]; then
    echo "*! Command '$EXECUTABLE $*' exited with error code $error"
#    echo "Command '$EXECUTABLE $*' exited with error code $error" > validation_error.message
    exit $error
fi

exit 0
