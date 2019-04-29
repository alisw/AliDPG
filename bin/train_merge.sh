#!/bin/bash

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
echo "========================================="
echo "* PATH: $PATH"
echo "* LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo "* ROOTSYS: $ROOTSYS"
echo "* which root : `which root`"
echo "* ALICE_ROOT : $ALICE_ROOT"
echo "* which aliroot : `which aliroot`"
echo "* Arguments: $*"
echo "* system limits:"
ulimit -a
echo "* memory:"
free -m
echo "* df -h . : `df -h .`" 
echo "========================================="
    
runNum=${ALIEN_JDL_LPMRUNNUMBER}
echo " * Run number is $runNum * "
echo "Printing all environment variables"
printenv

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

if [ ! -f QAtrain_duo.C ]; then 
    cp $ALIDPG_ROOT/QA/QAtrain_duo.C .
fi

if [ -f "QAtrain_duo.C" ]; then
    echo "QAtrain_duo macro found"
    if grep -q -E -e "/cpass1|cpass1/" *.xml || grep -q -E -e "/cpass1|cpass1/" "$1"; then
        echo "* Running QA duo on _barrel" >&2
        time aliroot -b -q -x QAtrain_duo.C\(\"_barrel\",$runNum,\"$1\",$2\) >stdout.barrel.log 2>stderr.barrel.log

        exitcode=$?

        echo "* QAtrain_duo.C(_barrel) finished with exit code: $exitcode"

        if [ $exitcode -ne 0 ]; then
            echo "QAtrain_duo.C / barrel exited with code $exitcode" > validation_error.message
            exit 1
        fi

        for file in *.stat; do
            mv "$file" "$file.barrel"
        done

        echo "* Running QA duo on _outer" >&2
        time aliroot -b -q -x QAtrain_duo.C\(\"_outer\",$runNum,\"$1\",$2\) >stdout.outer.log 2>stderr.outer.log

        exitcode=$?

        echo "* QAtrain_duo.C(_outer) finished with exit code: $exitcode"

        if [ $exitcode -ne 0 ]; then
            echo "QAtrain_duo.C / outer exited with code $exitcode" > validation_error.message
            exit 2
        fi

        for file in *.stat; do
            mv "$file" "$file.outer"
        done

        exit 0
    else
        echo "* Running QA duo on <empty>" >&2
        time aliroot -b -q -x QAtrain_duo.C\(\"\",$runNum,\"$1\",$2\)

        exitcode=$?

        echo "* QAtrain_duo.C() finished with exit code: $exitcode"
        
        if [ $exitcode -ne 0 ]; then
             echo "QAtrain_duo.C / empty exited with code $exitcode" > validation_error.message
             exit $exitcode
        fi

        echo "* Running QA duo on _AOD" >&2
        time aliroot -b -q -x QAtrain_duo.C\(\"_AOD\",$runNum,\"$1\",$2\) >stdout.AOD.log 2>stderr.AOD.log

        exitcode=$?

        echo "* QAtrain_duo.C(_AOD) finished with exit code: $exitcode"

        if [ $exitcode -ne 0 ]; then
            echo "QAtrain_duo.C / AOD exited with code $exitcode" > validation_error.message
            exit $exitcode
        fi

        exit 0
    fi
else 
    echo "No QAtrain_duo.C macro found"
fi

export ARG="QAtrain.C(0,\"$1\",$2)"
time aliroot -b -q -x $ARG

exitcode=$?

echo "* QAtrain.C finished with exit code: $exitcode"

if [ $exitcode -ne 0 ]; then
    echo "$ARG exited with code $exitcode" > validation_error.message
fi

exit $exitcode
