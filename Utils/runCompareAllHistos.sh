#!/bin/bash -e
exec > >(tee "compare_all_histos.txt") 2>&1
cd "$(dirname "$0")"
echo "Current QAresults: $1"
echo "Comparing to: $COMPARE_QARESULTS"
set -x
root -b -l -q "CompareAllHistos.C(\"$1\", \"$COMPARE_QARESULTS\", \"\");"
