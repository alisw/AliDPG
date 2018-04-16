#!/bin/bash

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
echo "========================================="
echo "############## PATH : ##############"
echo $PATH
echo "############## LD_LIBRARY_PATH : ##############"
echo $LD_LIBRARY_PATH
echo "############## ROOTSYS : ##############"
echo $ROOTSYS
echo "############## which root : ##############"
which root
echo "############## ALICE_ROOT : ##############"
echo $ALICE_ROOT
echo "############## which aliroot : ##############"
which aliroot
echo "############## system limits : ##############"
ulimit -a
echo "############## memory : ##############"
free -m
echo "========================================="

MACROPATH=""

for testPath in AODtrainsim.C AODtrain.C $ALIDPG_ROOT/AOD/AODtrainsim.C $ALIDPG_ROOT/AOD/AODtrain.C; do
    if [ -f "$testPath" ]; then
        MACROPATH="$testPath"
        break
    fi
done

if [ -z "$MACROPATH" ]; then
    echo "* No AOD macro was found"
    exit 1
fi

time aliroot -b -q -x $MACROPATH\(1\)
    
exitcode=$?
    
echo "======== $MACROPATH finished with exit code: $exitcode ========"

if [ $exitcode -ne 0 ]; then
    echo "$MACROPATH exited with code $exitcode" > validation_error.message
fi

exit $exitcode
