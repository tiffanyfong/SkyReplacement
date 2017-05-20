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

# src/imgpro skyVid/test0000001.jpg skyVid/OUTPUTtest0000001.jpg \
# -skyReplace skyVid/newSky.jpg 77

# src/imgpro ait3/test0000001.jpg ait3/OUTPUTtest0000001.jpg \
# -skyReplace ait3/newSkySCALED.jpg 30

src/imgpro aitSunny/test0000001.jpg aitSunny/OUTPUTtest0000001.jpg \
-skyReplace grumpyCatSky.jpg 142

# src/imgpro testSequence/test0000001.jpg testSequence/OUTPUTtest0000001.jpg \
# -skyReplace 147

# src/imgpro skyVid/test0000001.jpg skyVid/OUTPUTtest0000001.jpg \
# -skyReplace 77

# src/imgpro skyVid/test0000001.jpg skyVid/redOUTPUT0000001.jpg \
# -skyBlack 77

# src/imgpro THING/test0000001.jpg THING/hella.jpg \
# -skyReplace 78

# src/imgpro testSequence/sky/copenhagen.jpg testSequence/sky/copenhagenRED.jpg \
# 	-skyBlack

# src/imgpro testSequence/sky/boat.jpg testSequence/sky/boatRED.jpg \
# 	-skyBlack

# src/imgpro testSequence/sky/bikas.jpg testSequence/sky/bikasRED.jpg \
# 	-skyBlack
