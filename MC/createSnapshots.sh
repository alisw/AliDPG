#!/bin/bash

Usage() {
    echo "Usage: createSnapshots.sh <runNumber>"
    exit 1
}

if [ $# -lt 1 ] ; then Usage ;fi

if [ "$ALIDPG_ROOT" = "" ]; then
    echo ">>>>> ERROR: ALIDPG_ROOT is not set!"
    exit
fi

runNumber=$1
export runNumber

OCDB_SNAPSHOT_CREATE="CreateSnapshot.C"
export OCDB_SNAPSHOT_CREATE

REC_OCDB_SNAPSHOT_FILENAME="OCDBrec.root"
export REC_OCDB_SNAPSHOT_FILENAME

SIM_OCDB_SNAPSHOT_FILENAME="OCDBsim.root"
export SIM_OCDB_SNAPSHOT_FILENAME

echo "Creating OCDB snapshot for simulation: $SIM_OCDB_SNAPSHOT_FILENAME"
time aliroot -b -q -x "ConfigOCDB.C(${runNumber},1)" >> simSnapshot.log 2>&1

echo "Creating OCDB snapshot for reconstruction: $REC_OCDB_SNAPSHOT_FILENAME"
time aliroot -b -q -x "ConfigOCDB.C(${runNumber},0)" >> recSnapshot.log 2>&1


# disable snapshot creation mode
unset OCDB_SNAPSHOT_CREATE
