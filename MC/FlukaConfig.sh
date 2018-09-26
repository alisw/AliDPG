#!/bin/bash

FLUKA_CONFIG_ERROR=
CORE_FLUKA_VMC="$ALICE_ROOT/TFluka/input/coreFlukaVmc.inp"
[[ -e $CORE_FLUKA_VMC ]] || CORE_FLUKA_VMC="$FLUVMC/input/coreFlukaVmc.inp"

LINK_FILES=( "$CORE_FLUKA_VMC"
             "$FLUPRO/neuxsc-ind_260.bin:neuxsc.bin"
             "$FLUPRO/random.dat:old.seed"
             "$FLUPRO/fluodt.dat"
             "$FLUPRO/xnloan.dat" )

for FILE in "${LINK_FILES[@]}"; do
  DEST=${FILE##*:}
  SRC=${FILE%:*}
  [[ $DEST != $FILE ]] || DEST=.
  [[ -e $SRC ]] || { echo "ERROR: Cannot link FLUKA input file $SRC to $DEST because it does not exist!"; FLUKA_CONFIG_ERROR=1; }
  ln -nfsv "$SRC" "$DEST"
done
