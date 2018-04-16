#!/bin/bash
line=$1
run=`echo $line | cut -f6 -d '/'`
rm found_$run.list
rm rec_$run.log
rm rec_$run.list
rm scanned_$run.list
alien_find $line \rec.log > rec_$run.list

cat rec_$run.list | while read recFile
do
    echo scanning $recFile
    alien_cp alien://$recFile rec_$run.log
    echo $recFile >> scanned_$run.list
    result="$(grep "E-AliHLTComponentHandler::LoadLibrary: can not load library libAliHLTTPC.so (gSystem)" rec_$run.log)"
    #echo result is $result
    if [ $(echo "${result}" | wc -w) -ge 1 ]; then
	#   echo match found in $recFile
	echo $recFile >> found_$run.list
	#	else
	#   echo match not found in $recFile
    fi
    rm rec_$run.log
    done
rm rec_$run.list
