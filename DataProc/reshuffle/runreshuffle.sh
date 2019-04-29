#!/bin/bash

ln -s $ALIDPG_ROOT/DataProc/reshuffle/ReshuffleRawTree.C ReshuffleRawTree.C
aliroot -b -q ReshuffleRawTree.C+\(\"$1\"\) | tee ReshuffleRawTree.log
