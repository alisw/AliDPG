#!/bin/bash
rm listSteps

stepsize=0.5
originZrange=0.1

for step in {0..1000}
do
let rmin=$step*1
let rmax=1+$rmin
let imin=$step*1
let imax=1+$rmin
aliroot -b -q -l doStep.C\(10,${imin},${imax},${originZrange},${stepsize}\)
aliroot -b -q -l showLego.C >oo
cat oo|grep "mean budget" >${imin}_${imax}
cat oo |grep "mean density" >d${imin}_${imax}
echo d${imin}_${imax} >>listSteps
done

cat listSteps|awk '{print "cat",$1}'|bash|awk '{print $4}' >valuesToBePlotted

aliroot -b -q -l draw.C\(${stepsize}\)
