#!/bin/bash

set -e

if [[ $# -lt 1 ]]; then
    echo "You must specify at least where to write output to as first parameter."
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

# prepare environment
source /cvmfs/alice.cern.ch/etc/login.sh
eval $(alienv printenv Therminator2::v2.0.3-1)

if [ -d events/ ];
then
    rm -rf events/
fi
mkdir events

# copy necessary files
cp $ALIDPG_ROOT/MC/EXTRA/therm2_apply_custom_config.py .
cp $THERMINATOR2_ROOT/events.ini .
cp -r $THERMINATOR2_ROOT/fomodel .
cp -r $THERMINATOR2_ROOT/share .
cp -r $THERMINATOR2_ROOT/macro .

mkfifo events/event.txt

# update the key=value pairs in the configuration files based on environment variables
# setting the THERM2_PARAMS_<variable_name> sets the corresponding <variable_name> in configuration file
# also downloads and sets the proper xml file (if hydro model is selected and THERM2_PARAMS_XML_PATH is set)
python therm2_apply_custom_config.py

# start the generator and the parser
therm2_parser events/event.txt $1 &
therm2_events events.ini 0
 

