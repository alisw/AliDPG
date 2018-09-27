#!/bin/sh

# Link here some special Fluka files needed
ln -s $FLUPRO/neuxsc-ind_260.bin neuxsc.bin
ln -s $FLUVMC/input/coreFlukaVmc.inp coreFlukaVmc.inp
ln -s $FLUPRO/random.dat old.seed
ln -s $FLUPRO/fluodt.dat
ln -s $FLUPRO/xnloan.dat 
