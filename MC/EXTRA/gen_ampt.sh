#!/bin/bash
set -e

if [[ $# -lt 13 ]]; then
    echo "Not enough arguments passed, see AliDPG/MC/GeneratorConfig.C"
    exit -1
fi

echo "running in `pwd`, writing HepMC to $1"

# prepare environment
source /cvmfs/alice.cern.ch/etc/login.sh
eval $(alienv printenv AMPT::v1.26t7-v2.26t7-1)

# run generator

# generate random seed
nrandom=`date '+%d%H%M%S'`
echo $nrandom > nseed_runtime

if [ -d ana/ ];
then
    rm -rf ana/
fi
mkdir ana

echo "${10}            ! EFRM (sqrt(S_NN) in GeV if FRAME is CMS)" > input.ampt
echo "CMS             ! FRAME" >> input.ampt
echo "$6               ! PROJ" >> input.ampt
echo "$7               ! TARG" >> input.ampt
echo "$2             ! IAP (projectile A number)" >> input.ampt
echo "$3              ! IZP (projectile Z number)" >> input.ampt
echo "$4             ! IAT (target A number)" >> input.ampt
echo "$5              ! IZT (target Z number)" >> input.ampt
echo "${11}         ! NEVNT (total number of events)" >> input.ampt
echo "$8              ! BMIN (mininum impact parameter in fm) " >> input.ampt
echo "$9            ! BMAX (maximum impact parameter in fm, also see below)" >> input.ampt
echo "${12}               ! ISOFT (D=1): select Default AMPT or String Melting(see below)" >> input.ampt
echo "${13}             ! NTMAX: number of timesteps (D=150), see below" >> input.ampt
echo "0.2             ! DT: timestep in fm (hadron cascade time= DT*NTMAX) (D=0.2)" >> input.ampt
echo "0.3             ! PARJ(41): parameter a in Lund symmetric splitting function" >> input.ampt
echo "0.15            ! PARJ(42): parameter b in Lund symmetric splitting function" >> input.ampt
echo "1               ! (D=1,yes;0,no) flag for popcorn mechanism(netbaryon stopping)" >> input.ampt
echo "1.0             ! PARJ(5) to control BMBbar vs BBbar in popcorn (D=1.0)" >> input.ampt
echo "1               ! shadowing flag (Default=1,yes; 0,no)" >> input.ampt
echo "0               ! quenching flag (D=0,no; 1,yes)" >> input.ampt
echo "2.0             ! quenching parameter -dE/dx (GeV/fm) in case quenching flag=1" >> input.ampt
echo "2.0             ! p0 cutoff in HIJING for minijet productions (D=2.0)" >> input.ampt
echo "2.265d0         ! parton screening mass in fm^(-1) (D=2.265d0)" >> input.ampt
echo "0               ! IZPC: (D=0 forward-angle parton scatterings; 100,isotropic)" >> input.ampt
echo "0.33d0          ! alpha in parton cascade (D=0.33d0), see parton screening mass" >> input.ampt
echo "1d6             ! dpcoal in GeV" >> input.ampt
echo "1d6             ! drcoal in fm" >> input.ampt
echo "11              ! ihjsed: take HIJING seed from below (D=0)or at runtime(11)" >> input.ampt
echo "13150909        ! random seed for HIJING" >> input.ampt
echo "8               ! random seed for parton cascade" >> input.ampt
echo "1               ! flag for K0s weak decays (D=0,no; 1,yes)" >> input.ampt
echo "1               ! flag for phi decays at end of hadron cascade (D=1,yes; 0,no)" >> input.ampt
echo "1               ! flag for pi0 decays at end of hadron cascade (D=0,no; 1,yes)" >> input.ampt
echo "0               ! optional OSCAR output (D=0,no; 1,yes; 2&3,more parton info)" >> input.ampt
echo "0               ! flag for perturbative deuteron calculation (D=0,no; 1or2,yes)" >> input.ampt
echo "1               ! integer factor for perturbative deuterons(>=1 & <=10000)" >> input.ampt
echo "1               ! choice of cross section assumptions for deuteron reactions" >> input.ampt
echo "-7.             ! Pt in GeV: generate events with >=1 minijet above this value" >> input.ampt
echo "1000            ! maxmiss (D=1000): maximum # of tries to repeat a HIJING event" >> input.ampt
echo "3               ! flag to turn off initial and final state radiation (D=3)" >> input.ampt
echo "1               ! flag to turn off Kt kick (D=1)" >> input.ampt
echo "0               ! flag to turn on quark pair embedding (D=0,no; 1,yes)" >> input.ampt
echo "7., 0.          ! Initial Px and Py values (GeV) of the embedded quark (u or d)" >> input.ampt
echo "0., 0.          ! Initial x & y values (fm) of the embedded back-to-back q/qbar" >> input.ampt
echo "1, 5., 0.       ! nsembd(D=0), psembd (in GeV),tmaxembd (in radian)." >> input.ampt
echo "0               ! Flag to enable users to modify shadowing (D=0,no; 1,yes)" >> input.ampt
echo "1.d0            ! Factor used to modify nuclear shadowing" >> input.ampt
echo "1               ! Flag for random orientation of reaction plane (D=0,no; 1,yes)" >> input.ampt

cp input.ampt ana/

mkfifo ana/ampt.dat

ampt < nseed_runtime &
parser ana/ampt.dat $1

rm -rf ana/
