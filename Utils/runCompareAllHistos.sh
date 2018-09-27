#!/bin/bash -e
exec > >(tee "compare_all_histos.txt") 2>&1
cd "$(dirname "$0")"
echo "Current QAresults: $1"
echo "Comparing to: $COMPARE_QARESULTS"
if [[ ! $1 || ! $COMPARE_QARESULTS ]]; then
  echo "WARNING: Cannot run CompareAllHistos.C"
  echo "WARNING: Usage (has one argument and an environment var to set):"
  echo "WARNING:   export COMPARE_QARESULTS=/path/to/oldQaResults.root"
  echo "WARNING:   runCompareAllHistos /path/to/ourQaResults.root"
  exit 0
fi
set -x
root -b -l -q "CompareAllHistos.C(\"$1\", \"$COMPARE_QARESULTS\", \"\");"
