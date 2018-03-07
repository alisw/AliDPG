#!/bin/bash

#ALIEN_JDL_PACKAGES='VO_ALICE@jemalloc::v3.6.0##VO_ALICE@AliPhysics::v5-09-20b-1##VO_ALICE@AliDPG::v5-09-XX-28'

echo PACKAGES = $ALIEN_JDL_PACKAGES
found=false

str=$ALIEN_JDL_PACKAGES
array=($(echo "$str" | sed 's/VO_ALICE@/ /g'))
for el in "${array[@]}"; do
    echo "checking element $el"
    strippedEl=($(echo "$el" | sed 's/\(.*\)##$/\1/'))
    echo "checking stripped element $strippedEl"
    if [[ $el =~ .*AliPhysics.* ]];
    then
	arrayAliPhysics=($(echo "$strippedEl" | sed 's/::/ /g'))
	version=${arrayAliPhysics[1]}
        echo "version = $version"
	arrayAliPhysicsVersion=($(echo "$version" | sed 's/-/ /g'))
	for elAliPhysicsVersion in "${arrayAliPhysicsVersion[@]}"; do
	    echo "elAliPhysicsVersion = $elAliPhysicsVersion"
	done
	mainVersion=${arrayAliPhysicsVersion[0]}
	mainRevision=${arrayAliPhysicsVersion[1]}
	mainTag=${arrayAliPhysicsVersion[2]:0:2}
	echo "main version = $mainVersion, main revision = $mainRevision, main tag = $mainTag"
	versionNumber=`echo $mainVersion | rev | cut -d'v' -f1 | rev`
	echo versionNumber=$versionNumber
	if [ $versionNumber -lt 5 ]; then echo "we cannot enable VDT, main version is too low ($versionNumber, it should be at least 5)";
	else
	    revisionNumber=$(echo $mainRevision | sed 's/^0*//')
	    echo revisionNumber=$revisionNumber
	    if [ $revisionNumber -lt 9 ]; then echo "we cannot enable VDT, main revision is too low ($revisionNumber, it should be at least 9)";
	    else
		tagNumber=$(echo $mainTag | sed 's/^0*//')
		echo tagNumber=$tagNumber
		if [ $tagNumber -lt 23 ]; then echo "we cannot enable VDT, main revision is too low ($tagNumber, it should be at least 23)";
		else
		    echo "Enabling VDT"
		    #
		    # Here we should enable VDT
		    #
		fi
	    fi
	fi
	found=true
	break
    else
	echo "This package does not contain AliPhysics, skipping it"
    fi
done
if [ "$found" != true ]; then
    echo "We have found no AliPhysics package, we cannot enable VDT"
fi

