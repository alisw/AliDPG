#! /bin/bash
set -e

if [[ $# -lt 1 ]]; then
    echo "usage: gen_eposlhc_PbPb.sh [outfilename] [nevents] [projectile_id] [projectile_energy] [target_id] [target_energy]"
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

# prepare environment
source /cvmfs/alice.cern.ch/etc/login.sh
eval $(alienv printenv Herwig/v7.0.4-alice1-3)
export LHAPDF_DATA_PATH=$LHAPDF_ROOT/share/LHAPDF

$ALIDPG_ROOT/MC/EXTRA/generate_matchbox_from_template.py $ALIDPG_ROOT/MC/CustomGenerators/PWGJE/LHC-MB.tmpl
Herwig read --repo=$HERWIG_ROOT/share/Herwig/HerwigDefaults.rpo LHC-MB.in

cat output.hepmc >> $1
