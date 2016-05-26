#!/bin/bash

### check environment

if [ "$ALIDPG_ROOT" = "" ]; then

    if [ -f "alidpg.tgz" ]; then

	echo "Using AliDPG from tarball"
	tar zxvf alidpg.tgz
	export ALIDPG_ROOT=AliDPG
	
    else
	
	echo "*! ERROR: ALIDPG_ROOT is not set!"
	echo "ERROR: ALIDPG_ROOT is not set!" >> validation_error.message
	exit
	
    fi
fi

### dgpsim.sh

DPGSIMSH=$ALIDPG_ROOT/MC/dpgsim.sh
if [ -f dpgsim.sh ]; then
    chmod +x dpgsim.sh
    DPGSIMSH=./dpgsim.sh
fi
echo "Calling '$DPGSIMSH $*'"
$DPGSIMSH $*
error=$?

if [ $error -ne 0 ]; then
    echo "*! Command '$DPGSIMSH $*' exited with error code $error"
    echo "Command '$DPGSIMSH $*' exited with error code $error" >> validation_error.message
    exit $error
fi

###

exit 0
