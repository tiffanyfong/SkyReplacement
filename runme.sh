#!/bin/bash                                                                  
# 
# Create all output results 
#

# Useful shell settings:

# abort the script if a command fails
set -e

# abort the script if an unitialized shell variable is used
set -u

# make sure the code is up to date

pushd src
make
popd

# generate the result pictures

# src/imgpro testSequence/test0000001.jpg testSequence/output/OUTPUT.jpg \
# -skyReplace 147

# src/imgpro THING/test0000001.jpg THING/hella.jpg \
# -skyReplace 78

# src/imgpro testSequence/sky/copenhagen.jpg testSequence/sky/copenhagenRED.jpg \
# 	-skyBlack

# src/imgpro testSequence/sky/boat.jpg testSequence/sky/boatRED.jpg \
# 	-skyBlack

src/imgpro testSequence/sky/bikas.jpg testSequence/sky/bikasRED.jpg \
	-skyBlack
