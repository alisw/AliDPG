#!/bin/bash

function initDefaultEventsIniFile
{
    echo "FreezeOutModel = Lhyquid2DBI"       >  events.ini
    echo "NumberOfEvents = 50000"             >> events.ini
    echo "EventFileType = root"               >> events.ini
    echo "MultiplicityDistribution = Poisson" >> events.ini
    echo "IntegrateSamples = 5000000"         >> events.ini
    echo "Randomize = 1"                      >> events.ini
    echo "ShareDir = share/"                  >> events.ini
    echo "FreezeOutDir = fomodel/"            >> events.ini
    echo "MacroDir = macro/"                  >> events.ini
    echo "EventDir = events/"                 >> events.ini
    echo "LogFile = therminator.log"          >> events.ini
}

function setFreezeoutXml
{
    # if it's a GRID path
    if [[ $1 == alien:* ]]
    then
        # Downloading the GRID file to fomodel/
        alien_cp $1 fomodel/
    else
        # If it's not a GRID path, just copy the file to fomodel
        cp $1 fomodel/
    fi
    BASENAME=$(basename $1)
    export THERM2_PARAMS_FreezeFile="$BASENAME"

} 

# Updates the key=value pairs according to environment variables
function updateConfigFile
{
    awk 'BEGIN {FS=" *= *"} 
        /^ *\w+/{
        if ("THERM2_PARAMS_"$1 in ENVIRON)
            printf "%s = %s\n",$1,ENVIRON["THERM2_PARAMS_"$1];
        else
            print $0;
        }
        /^ *# *EventSubDir/ {print "EventSubDir=./"}' $1 > $1.temp
    mv $1.temp $1
}

# Applies the whole configuration to Therminator2
# Must be executed in the same directory as fomodel/ and events.ini
function applyCustomConfig
{
    # Maps from selected model to .ini file in fomodel/
    declare -A freezeoutMap=(
    ["KrakowSFO"]="krakow.ini"
    ["BlastWave"]="blastwave.ini"
    ["BWAVT"]="bwa.ini"
    ["BWAVTDelay"]="bwa.ini"
    ["BWAVLinear"]="bwa.ini"
    ["BWAVLinearDelay"]="bwa.ini"
    ["BWAVLinearFormation"]="bwa.ini"
    ["Lhyquid3D"]="lhyquid3d.ini"
    ["Lhyquid2DBI"]="lhyquid2dbi.ini"
    )

    # Default model
    model="Lhyquid2DBI"

    # Non-default model change
    if ! [ -z ${THERM2_PARAMS_FreezeOutModel+x} ]
    then
        model=$THERM2_PARAMS_FreezeOutModel
    fi

    # Easier to handle
    export THERM2_PARAMS_EventSubDir="./"
    # The parser works on text files
    export THERM2_PARAMS_EventFileType="text"

    # Check if the selected model is available
    if ! [ ${freezeoutMap[$model]+_} ]
    then
        echo "Invalid model name, quitting..."
        exit
    fi

    # Set the xml path (for the hydro models)
    if ! [ -z ${THERM2_PARAMS_XML_PATH+x} ]
    then
        setFreezeoutXml $THERM2_PARAMS_XML_PATH
    fi

    # Update configuration files with new values
    updateConfigFile "events.ini"
    updateConfigFile "fomodel/"${freezeoutMap[$model]}
}

if [[ $# -lt 1 ]]; then
    echo "You must specify at least where to write output to as first parameter."
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

# prepare environment
eval $(/cvmfs/alice-nightlies.cern.ch/bin/alienv printenv Therminator2::v2.0.3-alice1_PWGMMTEST-2)

if [ -d events/ ];
then
    rm -rf events/
fi
mkdir events

cp -r $THERMINATOR2_ROOT/fomodel .
cp -r $THERMINATOR2_ROOT/share .
cp -r $THERMINATOR2_ROOT/macro .

mkfifo events/event.txt

initDefaultEventsIniFile

# update the key=value pairs in the configuration files based on environment variables
# setting the THERM2_PARAMS_<variable_name> sets the corresponding <variable_name> in configuration file
# also downloads and sets the proper xml file (if hydro model is selected and THERM2_PARAMS_XML_PATH is set)
applyCustomConfig

therm2_parser events/event.txt $1 &
therm2_events events.ini 0
