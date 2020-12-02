#!/bin/bash
set -ev

if [[ $# -lt 1 ]]; then
    echo "You must specify at least where to write output to as first parameter."
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

echo "aligenmc configuration:" 
echo "==========================================="
echo "Generator:              $ALIGENMC_GENERATOR"
echo "Number of events:       $CONFIG_NEVENTS"
echo "CMS energy:             $CONFIG_ENERGY"
echo "Random seed:            $CONFIG_SEED"
echo "Process                 $CONFIG_PROCESS"
echo "Pt-hard min:            $CONFIG_PTHARDMIN"
echo "Pt-hard max:            $CONFIG_PTHARDMAX"
echo "Packages:               $ALIGENMC_PACKAGES" 
echo "Extra options:          $ALIGENMC_OPTIONS"

aligenmc ${ALIGENMC_PACKAGES:+-p $ALIGENMC_PACKAGES} ${ALIGENMC_GENERATOR:+-g $ALIGENMC_GENERATOR} ${CONFIG_NEVENTS:+-N $CONFIG_NEVENTS} ${CONFIG_ENERGY:+-E $CONFIG_ENERGY} ${CONFIG_SEED:+-S $CONFIG_SEED} ${CONFIG_PROCESS:+-t $CONFIG_PROCESS} ${CONFIG_PTHARDMIN:+-k $CONFIG_PTHARDMIN} ${CONFIG_PTHARDMAX:+-K $CONFIG_PTHARDMAX} ${ALIGENMC_OPTIONS:+$ALIGENMC_OPTIONS} -o $1
