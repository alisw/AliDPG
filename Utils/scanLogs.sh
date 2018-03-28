#!/bin/bash

rm found.list
rm rec.log
rm rec.list
rm scanned.list
file=$1
while read -r line
do
#line=/alice/data/2017/LHC17c/000270581/pass1/
    alien_find $line \rec.log > rec.list
    cat rec.list | while read recFile
    do
	echo scanning $recFile
#	if [ $recFile != "/alice/data/2017/LHC17c/000270581/pass1/17000270581020.104/rec.log" ] ; then
#	    echo this file will be ignored
#	    continue
#	else
#	    echo this is the file I need
#	fi
	alien_cp alien://$recFile rec.log
	#	cat rec.log
	echo $recFile >> scanned.list
	result="$(grep "E-AliHLTComponentHandler::LoadLibrary: can not load library libAliHLTTPC.so (gSystem)" rec.log)"
	#echo result is $result
	if [ $(echo "${result}" | wc -w) -ge 1 ]; then
	 #   echo match found in $recFile
	    echo $recFile >> found.list
#	else
	 #   echo match not found in $recFile
	fi
	rm rec.log
    done
    rm rec.list
done < "$file"
