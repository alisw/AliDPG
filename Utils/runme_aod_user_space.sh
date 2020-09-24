#!/bin/bash

#  1) alienv enter VO_ALICE@AliDPG::prod-202009-03-1,VO_ALICE@AliPhysics::v5-09-53e-01-1
#
#  2) AliPhysics version needs to be set manually in main_AODtrainRawAndMC.C 
#       just copy it in the CWD from $ALIDPG_ROOT/MC/AOD folder, it will be picke-up automatically at runtime

#
# RAW/MC data re-filtering
#
isMC=1

#
# RAW environment
#
if [ $isMC == 0 ]; then
  export USER_AOD_ALIEN_DATA_DIR="/alice/data/2017/LHC17l"
  export USER_AOD_ALIEN_RUN_NUMBERS="278158, 277952"
  export USER_AOD_ALIEN_PASS="pass1"

  export ALIEN_JDL_LPMINTERACTIONTYPE=pp
  export ALIEN_JDL_LPMANCHORYEAR=$(echo $USER_AOD_ALIEN_DATA_DIR | awk -F'/' '{print $4}')
  export ALIEN_JDL_LPMPRODUCTIONTAG=$(echo $USER_AOD_ALIEN_DATA_DIR | awk -F"/" '{print $NF}')
  export ALIEN_JDL_LPMRUNNUMBER=$(echo $USER_AOD_ALIEN_RUN_NUMBERS | awk -F',' '{print $1}')
fi

#
# MC environment
#
if [ $isMC == 1 ]; then
  export USER_AOD_ALIEN_DATA_DIR="/alice/sim/2018/LHC18a8"
  export USER_AOD_ALIEN_RUN_NUMBERS="278158, 277952"
  export USER_AOD_ALIEN_PASS=""

  export ALIEN_JDL_LPMINTERACTIONTYPE=pp		# needs input specie
  export ALIEN_JDL_LPMANCHORYEAR=2017			# needs anchored period's year
  export ALIEN_JDL_LPMPRODUCTIONTAG=LHC17l		# needs anchored period
  export ALIEN_JDL_LPMRUNNUMBER=$(echo $USER_AOD_ALIEN_RUN_NUMBERS | awk -F',' '{print $1}')
fi


export ALIEN_JDL_RAWPRODTYPE=""
export CONFIG_OCDB="alien"
export CONFIG_SYSTEM=p-p				# needs update following AliDPG enums
export CONFIG_YEAR=$ALIEN_JDL_LPMANCHORYEAR
export CONFIG_PERIOD=$ALIEN_JDL_LPMPRODUCTIONTAG
export CONFIG_RUN=$ALIEN_JDL_LPMRUNNUMBER

#
# Disable extra cleanup, for HF
#
export ALIEN_JDL_SWITCHOFFEXTRACLEANUPINWEAKDECAYFINDER=1

aliroot -b -q $ALIDPG_ROOT/AOD/AODtrainRawAndMC.C\(0\,$isMC\,1\)

if [ $isMC == 0 ]; then
  echo "Running mode:               RAW ESD filtering"
else
  echo "Running mode:               MC ESD filtering"
fi
echo "ALIEN_JDL_LPMANCHORYEAR:    $ALIEN_JDL_LPMANCHORYEAR"
echo "ALIEN_JDL_LPMPRODUCTIONTAG: $ALIEN_JDL_LPMPRODUCTIONTAG"
echo "USER_AOD_ALIEN_DATA_DIR:    $USER_AOD_ALIEN_DATA_DIR"
echo "ALIEN_JDL_LPMRUNNUMBER:     $ALIEN_JDL_LPMRUNNUMBER"
