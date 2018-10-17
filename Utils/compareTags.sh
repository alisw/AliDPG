#!/bin/bash -e

set -x

LATEST_RELCANDIDATE_PATH=$1
REFERE_RELCANDIDATE_PATH=$2

for filename in QAresults_barrel.root QAresults.root QAresults_AOD.root 
do
    
    if [ "$filename" = "QAresults_AOD.root" ]
    then
	directory=pass1
	for folder in PIDqa MultSelection CheckVertexAOD CheckAODTracks PWG3_D2H_QA
	do 
	    echo running for $directory, $filename, $folder
	    root -b -l -q "CompareAllHistos.C(\"$1/$directory/$filename\", \"$2/$directory/$filename\", \"$folder\")" >& AOD_$folder.out
	done
    elif [ "$filename" = "QAresults.root" ]
    then 
	directory=pass1
	for folder in TPC TOF ITS TRD CaloQA_default CaloQA_trigEMC
	do
	    echo running for $directory, $filename, $folder
	    root -b -l -q "CompareAllHistos.C(\"$1/$directory/$filename\", \"$2/$directory/$filename\", \"$folder\")" >& pass1_$folder.out
	done
    else #filename" = "QAresults_barrel.root" 
	directory=cpass1_pass1
	for folder in TPC TOF ITS TRD CaloQA_default CaloQA_trigEMC
	do
	    echo running for $directory, $filename, $folder
	    root -b -l -q "CompareAllHistos.C(\"$1/$directory/$filename\", \"$2/$directory/$filename\", \"$folder\")" >& cpass1_$folder.out
	done
    fi
done

	
