
// Computer Vision for Digital Post-Production
// Lecturer: Gergely Vass - vassg@vassg.hu
//
// Skeleton Code for programming assigments
// 
// Code originally from Thomas Funkhouser
// main.c
// original by Wagner Correa, 1999
// modified by Robert Osada, 2000
// modified by Renato Werneck, 2003
// modified by Jason Lawrence, 2004
// modified by Jason Lawrence, 2005
// modified by Forrester Cole, 2006
// modified by Tom Funkhouser, 2007
// modified by Chris DeCoro, 2007
//



// Include files
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "R2/R2.h"
#include "R2Pixel.h"
#include "R2Image.h"



// Program arguments

static char options[] =
"  -help\n"
"  -svdTest\n"
"  -sobelX\n"
"  -sobelY\n"
"  -log\n"
"  -harris <real:sigma>\n"
"  -feature <real:sigma> <int:numFeatures>\n"
"  -featureTrack <file:other_image>\n"
"  -ransac <file:other_image>\n"
"  -dltransac <file:other_image>\n"
"  -saturation <real:factor>\n"
"  -brightness <real:factor>\n"
"  -blur <real:sigma>\n"
"  -sharpen \n"
"  -sharpenHighPass <real:sigma> <real:contrast> \n"
"  -bilateral <real:sigma>\n"
"  -median \n"
"  -fisheye \n"
"  -matchTranslation <file:other_image>\n"
"  -matchHomography <file:other_image>\n"
"  -skyReplace <int:numFrames>\n";

static void 
ShowUsage(void)
{
  // Print usage message and exit
  fprintf(stderr, "Usage: imgpro input_image output_image [  -option [arg ...] ...]\n");
  fprintf(stderr, options);
  exit(EXIT_FAILURE);
}



static void 
CheckOption(char *option, int argc, int minargc)
{
  // Check if there are enough remaining arguments for option
  if (argc < minargc)  {
    fprintf(stderr, "Too few arguments for %s\n", option);
    ShowUsage();
    exit(-1);
  }
}



static int 
ReadCorrespondences(char *filename, R2Segment *&source_segments, R2Segment *&target_segments, int& nsegments)
{
  // Open file
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Unable to open correspondences file %s\n", filename);
    exit(-1);
  }

  // Read number of segments
  if (fscanf(fp, "%d", &nsegments) != 1) {
    fprintf(stderr, "Unable to read correspondences file %s\n", filename);
    exit(-1);
  }

  // Allocate arrays for segments
  source_segments = new R2Segment [ nsegments ];
  target_segments = new R2Segment [ nsegments ];
  if (!source_segments || !target_segments) {
    fprintf(stderr, "Unable to allocate correspondence segments for %s\n", filename);
    exit(-1);
  }

  // Read segments
  for (int i = 0; i <  nsegments; i++) {

    // Read source segment
    double sx1, sy1, sx2, sy2;
    if (fscanf(fp, "%lf%lf%lf%lf", &sx1, &sy1, &sx2, &sy2) != 4) { 
      fprintf(stderr, "Error reading correspondence %d out of %d\n", i, nsegments);
      exit(-1);
    }

    // Read target segment
    double tx1, ty1, tx2, ty2;
    if (fscanf(fp, "%lf%lf%lf%lf", &tx1, &ty1, &tx2, &ty2) != 4) { 
      fprintf(stderr, "Error reading correspondence %d out of %d\n", i, nsegments);
      exit(-1);
    }

    // Add segments to list
    source_segments[i] = R2Segment(sx1, sy1, sx2, sy2);
    target_segments[i] = R2Segment(tx1, ty1, tx2, ty2);
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}


int 
main(int argc, char **argv)
{
  // Look for help
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-help")) {
      ShowUsage();
    }
	if (!strcmp(argv[i], "-svdTest")) {
      R2Image *image = new R2Image();
	  image->svdTest();
	  return 0;
    }
  }

  // Read input and output image filenames
  if (argc < 3)  ShowUsage();
  argv++, argc--; // First argument is program name
  char *input_image_name = *argv; argv++, argc--; 
  char *output_image_name = *argv; argv++, argc--; 

  // Allocate image
  R2Image *image = new R2Image();
  if (!image) {
    fprintf(stderr, "Unable to allocate image\n");
    exit(-1);
  }

  // Read input image
  if (!image->Read(input_image_name)) {
    fprintf(stderr, "Unable to read image from %s\n", input_image_name);
    exit(-1);
  }

  // Initialize sampling method
  int sampling_method = R2_IMAGE_POINT_SAMPLING;

  // Parse arguments and perform operations 
  while (argc > 0) {
    if (!strcmp(*argv, "-brightness")) {
      CheckOption(*argv, argc, 2);
      double factor = atof(argv[1]);
      argv += 2, argc -=2;
      image->Brighten(factor);
    }
	  else if (!strcmp(*argv, "-sobelX")) {
      argv++, argc--;
      image->SobelX();
    }
	  else if (!strcmp(*argv, "-sobelY")) {
      argv++, argc--;
      image->SobelY();
    }
	  else if (!strcmp(*argv, "-log")) {
      argv++, argc--;
      image->LoG();
    }
    else if (!strcmp(*argv, "-saturation")) {
      CheckOption(*argv, argc, 2);
      double factor = atof(argv[1]);
      argv += 2, argc -= 2;
      image->ChangeSaturation(factor);
    }
	  else if (!strcmp(*argv, "-harris")) {
      CheckOption(*argv, argc, 2);
      double sigma = atof(argv[1]);
      argv += 2, argc -= 2;
      image->Harris(sigma);
    }
    else if (!strcmp(*argv, "-feature")) {
      CheckOption(*argv, argc, 2);
      double sigma = atof(argv[1]);
      CheckOption(*argv, argc, 3);
      int numFeatures = (int)atoi(argv[2]);
      argv += 3, argc -= 3;
      image->DetectFeatures(sigma, numFeatures);
    }
    else if (!strcmp(*argv, "-featureTrack")) {
      CheckOption(*argv, argc, 2);
      R2Image *other_image = new R2Image(argv[1]);
      argv += 2, argc -= 2;
      image->TrackFeatures(other_image);
      delete other_image;
    }
    else if (!strcmp(*argv, "-ransac")) {
      CheckOption(*argv, argc, 2);
      R2Image *other_image = new R2Image(argv[1]);
      argv += 2, argc -= 2;
      image->RANSAC(other_image);
      delete other_image;
    }
    else if (!strcmp(*argv, "-dltransac")) {
      CheckOption(*argv, argc, 2);
      R2Image *other_image = new R2Image(argv[1]);
      argv += 2, argc -= 2;
      image->DLTRANSAC(other_image);
      delete other_image;
    }
    else if (!strcmp(*argv, "-blur")) {
      CheckOption(*argv, argc, 2);
      double sigma = atof(argv[1]);
      argv += 2, argc -= 2;
      image->Blur(sigma);
    }
    else if (!strcmp(*argv, "-sharpen")) {
      argv++, argc--;
      image->Sharpen();
    }
    else if (!strcmp(*argv, "-sharpenHighPass")) {
      CheckOption(*argv, argc, 2);
      double sigma = atof(argv[1]);
      CheckOption(*argv, argc, 3);
      double contrast = atof(argv[2]);
      argv += 3, argc -= 3;
      image->SharpenHighPass(sigma, contrast);
    }
    else if (!strcmp(*argv, "-bilateral")) {
      CheckOption(*argv, argc, 2);
      double sigma = atof(argv[1]);
      argv += 2, argc -= 2;
      image->Bilateral(sigma);
    }
    else if (!strcmp(*argv, "-median")) {
      argv++, argc--;
      image->Median();
    }
    else if (!strcmp(*argv, "-fisheye")) {
      argv++, argc--;
      image->Fisheye();
    }
    else if (!strcmp(*argv, "-matchTranslation")) {
      CheckOption(*argv, argc, 2);
      R2Image *other_image = new R2Image(argv[1]);
      argv += 2, argc -= 2;
      image->blendOtherImageTranslated(other_image);
      delete other_image;
    }
    else if (!strcmp(*argv, "-matchHomography")) {
      CheckOption(*argv, argc, 2);
      R2Image *other_image = new R2Image(argv[1]);
      argv += 2, argc -= 2;
      image->blendOtherImageHomography(other_image);
      delete other_image;
    }
    else if (!strcmp(*argv, "-skyBlack")) {
      argv++, argc--;
      image->MakeSkyBlack();
    }
    else if (!strcmp(*argv, "-skyReplace")) {
      CheckOption(*argv, argc, 2);
      const int numFrames = atoi(argv[1]);
      argv += 2, argc -= 2;
      printf("NUMBER OF FRAMES: %d\n", numFrames);

      std::string inputPath = "/Users/tmf/Desktop/skeleton/testSequence/test"; //"/Users/tmf/Desktop/skeleton/THING/test";
      std::string outputPath = "/Users/tmf/Desktop/skeleton/testSequence/OUTPUTtest"; //"/Users/tmf/Desktop/skeleton/THING/OUTPUTtest";
      std::string extension = ".jpg";

      // DRAW SHIT HERE
      R2Image * outputOrigImage = new R2Image(*image);
      // R2Image * tempImage;


      std::string number = "0000002"; // padding = 7 digits
      const int height = image->Height();
      const double sigma = 2.0;
      const int numFeatures = 150;
      const int sqRadius = 4;
      int xa, xb, ya, yb;

      // image = first frame
      std::vector<int> featuresA = image->getFeaturePositions(sigma, numFeatures, sqRadius);
      // std::vector<int> featuresA = {995987,824627,743595,975604,875383,1388169,828212,1509147,827975,1032845,828224,1147327,1505890,1511158,998283,850545,744816,1390194,618140,746506,826773,977627,617985,829998,1433024,828586,746659,892542,742893,1312434,742903,1164471,888265,1389268,881749,1123759,1112766,829118,1267911,829965,848546,878534,847417,1308248,1147192,1122816,743807,98829,828741,696218,1311505,742869,829977,902226,852692,621506,1140344,1231187,860266,1005981,741953,619202,133396,1286290,1120914,871046,1432778,1156006,1482525,848736,828724,828199,1465929,1099806,745489,854755,1156154,887044,1247391,742880,1120709,1233918,1154377,744666,828188,1269559,876503,830040,688897,1133481,840604,872037,957244,1269697,727781,217613,1432637,745588,883918,840723,1969385,93835,1451466,89109,2046396,1970401,2046483,566056,1434232,827189,938884,2046414,1971352,2044150,1434143,1235377,1184241,217436,1451422,621311,830813,2066783,902258,829986,1970449,827233,867705,679172,880730,871923,842971,640802,621567,2045281,657168,1166762,924937,744803,1468109,1452590,877410,829956,110083,850578,1245607,1970417,895877,1484309,848357,933473};
      image->SetSkyFeatures(featuresA);

      // imageA = second frame
      R2Image *imageB = new R2Image((inputPath + number + extension).c_str());

      std::vector<int> featuresB = image->findAFeaturesOnB(imageB, image->SkyFeatures(), sqRadius);
      // std::vector<int> featuresB = {995987,824627,743595,975604,875383,1388169,828212,1509147,827975,1032845,828224,1147327,1505890,1511158,998283,850545,744816,1390194,618140,746506,826773,977627,617985,829998,1433024,828586,746659,892542,742893,1312434,742903,1164471,888265,1389268,881749,1123759,1112766,829118,1267911,829965,848546,878534,847417,1308248,1147192,1122816,743807,98829,828741,696218,1311505,742869,829977,902227,852692,621506,1140344,1231187,860266,1005981,741953,619202,133396,1286290,1120914,871046,1432778,1156006,1482525,848736,828724,828199,1465929,1099806,745489,854756,1156154,887044,1247391,742880,1120709,1233918,1154377,744666,828188,1269559,876503,830040,688897,1133481,840604,872037,957244,1269697,727781,217613,1432637,745588,883918,840723,1969385,93835,1451466,89109,2046396,1970401,2046483,566056,1434232,827189,938884,2046414,1971352,2044150,1434143,1235377,1184241,217436,1451422,621311,830813,2066783,902258,829986,1970449,827233,867705,679172,880730,871923,842971,640802,621567,2045281,657168,1166762,924937,744803,1468109,1452590,877410,829956,110083,850578,1245607,1970417,895877,1484309,848357,933473};
      imageB->SetSkyFeatures(featuresB);

      printf("Tracked features from frame1 to frame2\n");

      // Calculate H using DLTRANSAC between frame(1) and frame(2). reject bad tracks
      double H[3][3]; // TODO DONT OVERWRITE. 3D array or R2Image field?


      image->SkyDLTRANSAC(imageB, H);

      for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
          std::cout<<H[i][k]<<" ";
        }
        std::cout<<"\n";
      }

      // FOR THE SECOND IMAGE
      // draw features in input image
      for (int i = 0; i < image->SkyFeatures().size(); i++) {
        xa = image->SkyFeatures().at(i) / height;
        ya = image->SkyFeatures().at(i) % height;
        xb = featuresB.at(i) / height;
        yb = featuresB.at(i) % height;

        // Add motion vectors (RED+BLUE)
        outputOrigImage->line(xa,xa,ya,ya,1,0,1);
        imageB->line(xb,xb,yb,yb,1,0,1);
      }
      if (!imageB->Write((outputPath + number + extension).c_str())) {
        fprintf(stderr, "Unable to read image from %s\n", (outputPath + number + extension).c_str());
        exit(-1);
      }

      // TODO deleting images deletes the feature positions as well (may need to calc H)
      R2Image *imageA;

      // iterate through frames
      // imageA = frame(i-1)
      // imageB = frame(i)

      for (int i = 3; i <= numFrames; i++) {
        // SETUP
        // copy imageB into imageA
        imageA = imageB;

        // Calculate new imageB
        number = "0000000" + std::to_string(i);
        number = number.substr(number.length()-7);

        // const char * filenameB = (inputPath + number + extension).c_str();
        imageB = new R2Image((inputPath + number + extension).c_str());


        // Track features from frame(i-1) to frame(i)
        featuresB.clear();
        featuresB = image->findAFeaturesOnB(imageB, imageA->SkyFeatures(), sqRadius);
        imageB->SetSkyFeatures(featuresB);

        // CLEAR THE H
        for (int iii = 0; iii < 3; iii++) {
          for (int kk = 0; kk < 3; kk++) {
            H[iii][kk] = 0;
          }
        }

        // Calculate H between frame(i-1) and frame(i)
        image->SkyDLTRANSAC(imageB, H);

        for (int iii = 0; iii < 3; iii++) {
          for (int kk = 0; kk < 3; kk++) {
            std::cout<<H[iii][kk]<<" ";
          }
          std::cout<<"\n";
        }


        // TODO replace sky with image (video later?)
          // apply the H to the sky. 

        // draw features in input image
        for (int j = 0; j < featuresB.size(); j++) {
          // xa = imageA->SkyFeatures().at(j) / height;
          // ya = imageA->SkyFeatures().at(j) % height;
          xb = featuresB.at(j) / height;
          yb = featuresB.at(j) % height;

          // Add motion vectors (RED+BLUE)
          imageB->line(xb,xb,yb,yb,1,0,1);
        }

        if (!imageB->Write((outputPath + number + extension).c_str())) {
          fprintf(stderr, "Unable to read image from %s\n", (outputPath + number + extension).c_str());
          exit(-1);
        }

        printf("Tracked features from frame%d to frame%d\n", i-1, i);
        delete imageA;
        // delete imageB;
      }
      delete image;
      delete imageB;
      image = outputOrigImage;

      // delete tempImage;


      
    }
    else {
      // Unrecognized program argument
      fprintf(stderr, "image: invalid option: %s\n", *argv);
      ShowUsage();
    }
  }

  // Write output image
  if (!image->Write(output_image_name)) {
    fprintf(stderr, "Unable to read image from %s\n", output_image_name);
    exit(-1);
  }

  delete image;

  // Return success
  return EXIT_SUCCESS;
}




