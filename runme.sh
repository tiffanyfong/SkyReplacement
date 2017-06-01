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

src/imgpro origFrames/vid10000001.jpg OUTPUTframes/vid10000001.jpg \
-skyReplace grumpyCatSky.jpg 20


