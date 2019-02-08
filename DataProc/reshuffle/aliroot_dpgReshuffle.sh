#!/bin/bash

export PRODUCTION_METADATA="$ALIEN_JDL_LPMMETADATA"

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

EXECUTABLE=$ALIDPG_ROOT/DataProc/reshuffle/runreshuffle.sh
if [ -f runreshuffle.sh ]; then
    chmod +x runreshuffle.sh
    EXECUTABLE=./runreshuffle.sh
fi
echo "Calling '$EXECUTABLE $*'"
$EXECUTABLE $*
error=$?

if [ $error -ne 0 ]; then
    echo "*! Command '$EXECUTABLE' exited with error code $error"
    exit $error
fi

exit 0
