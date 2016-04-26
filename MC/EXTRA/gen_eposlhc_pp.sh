#!/bin/bash
set -e

if [[ $# -lt 4 ]]; then
    echo "usage: gen_eposlhc_pp.sh [outfilename] [nevents] [projectile_energy] [target_energy]
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

# prepare environment
source /cvmfs/alice.cern.ch/etc/login.sh
eval $(alienv printenv GCC-Toolchain/v4.9.3-7)
eval $(alienv printenv CRMC::v1.5.4-2)

# force path to requested output
sed -e s#__CRMC_BASEDIR__#"$CRMC_ROOT"# ${ALICE_PHYSICS}/PWG/MCLEGO/CRMC/crmc_template.param > crmc.param

# run CRMC
crmc -o hepmc -p $3 -P-$4 -n $2 -m 0 -f $1
