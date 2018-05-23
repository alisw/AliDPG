#!/bin/bash
set -ev

if [[ $# -lt 1 ]]; then
    echo "You must specify at least where to write output to as first parameter."
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

aligenmc ${ALIGENMC_PACKAGES:+-p $ALIGENMC_PACKAGES} ${ALIGENMC_GENERATOR:+-g $ALIGENMC_GENERATOR} ${ALIGENMC_OPTIONS:+$ALIGENMC_OPTIONS} -o $1
