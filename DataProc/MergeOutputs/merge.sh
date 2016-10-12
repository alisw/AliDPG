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

TO_MERGE="$ALIEN_JDL_LPMCOLLECTIONENTITY";

if [ -z "$TO_MERGE" ]; then
  TO_MERGE="FilterEvents_Trees.root"
fi

time aliroot -b -q -x merge.C\(\"$TO_MERGE\"\)

echo "FILESTOCHECK=\"outputs_valid $TO_MERGE\"" | tr ',' ' ' > validation_merge.rc

exitcode=$?

echo "======== merge.C(\"$TO_MERGE\") finished with exit code: $exitcode ========"
echo "############## memory after: ##############"
free -m

if [ $exitcode -ne 0 ]; then
    echo "merge.C exited with code $exitcode" > validation_error.message
fi

exit $exitcode
