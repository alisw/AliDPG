#!/bin/bash
set -ev

if [[ $# -lt 1 ]]; then
    echo "You must specify at least where to write output to as first parameter."
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

aligenmc -p AliGenerators/v20180515-1 -g jewel -o $1
