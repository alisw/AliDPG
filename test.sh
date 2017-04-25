#!/bin/bash -e
cd "$(dirname "$0")"
type shellcheck &> /dev/null
for SH in $(find . -name '*.sh'); do
  printf "Testing shell syntax of $SH: "
  [[ $(shellcheck -f gcc "$SH" | grep -c ': error: ') -gt 0 ]] && { echo ERROR; shellcheck -f gcc "$SH"; }
  echo OK
done
