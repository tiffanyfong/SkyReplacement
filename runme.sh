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

# src/imgpro input/grainy.jpg output/grainy_median.jpg \
#     -median

# src/imgpro input/viennaduck.jpg output/viennaduck_shp1.jpg \
# 	 -sharpenHighPass 2.0 1.0

# src/imgpro input/viennaduck.jpg output/viennaduck_shp2.jpg \
# 	 -sharpenHighPass 2.0 2.0

# src/imgpro input/viennaduck.jpg output/viennaduck_shp3.jpg \
# 	 -sharpenHighPass 6.0 2.0

 # src/imgpro input/trackA.jpg output/trackA_harris.jpg \
 # 	 -harris 2.0

  # src/imgpro input/desk_A.jpg output/desk_featureTrack.jpg \
 	#  -featureTrack input/desk_B.jpg

 # src/imgpro input/test_C_bridge01.jpg output/test_C_featureTrack.jpg \
 # 	 -featureTrack input/test_C_bridge02.jpg

 # src/imgpro input/test_D_face01.jpg output/test_D_featureTrack.jpg \
 # 	 -featureTrack input/test_D_face02.jpg

 # src/imgpro input/test_E_sitting01.jpg output/test_E_featureTrack.jpg \
 # 	 -featureTrack input/test_E_sitting02.jpg

 # src/imgpro input/testB.jpg output/testB_feature.jpg \
 # 	 -feature 2.0 150

  # src/imgpro input/globos_de_colores.jpg output/globos_de_colores_fisheye.jpg \
 	#  -fisheye

 # src/imgpro input/test_C_bridge01.jpg output/test_C_blend.jpg \
 # 	 -matchHomography input/test_C_bridge02.jpg

 # src/imgpro input/test_D_face01.jpg output/test_D_ransac.jpg \
 # 	 -ransac input/test_D_face02.jpg

 # src/imgpro input/test_E_sitting01.jpg output/test_E_RANSAC.jpg \
 # 	 -ransac input/test_E_sitting02.jpg

 # src/imgpro input/TESTA1.jpg output/TESTA_featureTrack.jpg \
 # 	 -featureTrack input/TESTA2.jpg

 	 #  src/imgpro input/TESTB1.jpg output/TESTB_featureTrack.jpg \
 	 # -featureTrack input/TESTB2.jpg

 # src/imgpro input/TESTA1.jpg output/TESTA_blendBilin.jpg \
 # 	 -matchHomography input/TESTA2.jpg

 #  #  src/imgpro input/TESTB1.jpg output/TESTB_harris.jpg \
 # 	#  -harris 2.0

 #  # src/imgpro input/TESTB1.jpg output/TESTB_feature.jpg \
 # 	#  -feature 2.0 150

 # src/imgpro input/TESTB1.jpg output/TESTB_blendBilin.jpg \
 # 	 -matchHomography input/TESTB2.jpg

 # # src/imgpro input/test_D_face01.jpg output/test_D_dltransac.jpg \
 # # 	 -dltransac input/test_D_face02.jpg

 # src/imgpro input/myimage1.jpg output/myimage_blendBilin.jpg \
 # 	-matchHomography input/myimage2.jpg

 # src/imgpro input/myimage1.jpg output/myimage_dltransac.jpg \
 # 	-dltransac input/myimage2.jpg


src/imgpro testSequence/test0000001.jpg testSequence/OUTPUT.jpg \
-skyReplace 147
