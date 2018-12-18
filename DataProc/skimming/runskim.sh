#!/bin/bash

ln -s -f rootrc .rootrc
aliroot -b -q includeMacro.C skimRaw.C+g | tee skim.log





