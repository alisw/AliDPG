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

### dgpsim.sh

DPGSIMSH=$ALIDPG_ROOT/DataProc/CPass0/runCPass0.sh
if [ -f runCPass0.sh ]; then
    chmod +x runCPass0.sh
    DPGSIMSH=./runCPass0.sh
fi
echo "Calling '$DPGSIMSH $*'"
$DPGSIMSH $*
error=$?

if [ $error -ne 0 ]; then
    echo "*! Command '$DPGSIMSH $*' exited with error code $error"
#    echo "Command '$DPGSIMSH $*' exited with error code $error" > validation_error.message
    exit $error
fi

exit 0
