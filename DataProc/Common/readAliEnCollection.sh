#!/bin/bash
XML=$(mktemp /tmp/alien_collection_XXXXX.xml)
TXT=$(mktemp /tmp/alien_collection_XXXXX.txt)
cat > $XML
root -l -b -q "$(dirname "$0")/"readAliEnCollection.C"(\"$XML\", \"$TXT\")" &> /dev/null
cat $TXT
rm -f $XML $TXT &> /dev/null
