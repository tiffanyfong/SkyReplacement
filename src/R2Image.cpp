// Source file for image class

#define _USE_MATH_DEFINES

// Include files 

#include "R2/R2.h"
#include "R2Pixel.h"
#include "R2Image.h"
#include "svd.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


////////////////////////////////////////////////////////////////////////
// Constructors/Destructors
////////////////////////////////////////////////////////////////////////


R2Image::
R2Image(void)
	: pixels(NULL),
	npixels(0),
	width(0),
	height(0),
	skyFeatures(std::vector<int>()),
	h(std::vector<double>(9)),
	translationVector(std::vector<int>(2))
{

}



R2Image::
R2Image(const char *filename)
	: pixels(NULL),
	npixels(0),
	width(0),
	height(0),
	skyFeatures(std::vector<int>()),
	h(std::vector<double>(9)),
	translationVector(std::vector<int>(2))
{
	// Read image
	Read(filename);
}



R2Image::
R2Image(int width, int height)
	: pixels(NULL),
	npixels(width * height),
	width(width),
	height(height),
	skyFeatures(std::vector<int>()),
	h(std::vector<double>(9)),
	translationVector(std::vector<int>(2))
{
	// Allocate pixels
	pixels = new R2Pixel[npixels];
	assert(pixels);
}



R2Image::
R2Image(int width, int height, const R2Pixel *p)
	: pixels(NULL),
	npixels(width * height),
	width(width),
	height(height),
	skyFeatures(std::vector<int>()),
	h(std::vector<double>(9)),
	translationVector(std::vector<int>(2))
{
	// Allocate pixels
	pixels = new R2Pixel[npixels];
	assert(pixels);

	// Copy pixels 
	for (int i = 0; i < npixels; i++)
		pixels[i] = p[i];
}



R2Image::
R2Image(const R2Image& image)
	: pixels(NULL),
	npixels(image.npixels),
	width(image.width),
	height(image.height),
	skyFeatures(image.skyFeatures),
	h(image.h),
	translationVector(image.translationVector)
{


	// Allocate pixels
	pixels = new R2Pixel[npixels];
	assert(pixels);

	// Copy pixels 
	for (int i = 0; i < npixels; i++)
		pixels[i] = image.pixels[i];
}



R2Image::
~R2Image(void)
{
	// Free image pixels
	if (pixels) delete[] pixels;
}



R2Image& R2Image::
operator=(const R2Image& image)
{
	// Delete previous pixels
	if (pixels) { delete[] pixels; pixels = NULL; }

	// Reset width and height
	npixels = image.npixels;
	width = image.width;
	height = image.height;
	skyFeatures = image.skyFeatures;
	h = image.h;
	translationVector = image.translationVector;

	// Allocate new pixels
	pixels = new R2Pixel[npixels];
	assert(pixels);

	// Copy pixels 
	for (int i = 0; i < npixels; i++)
		pixels[i] = image.pixels[i];

	// Return image
	return *this;
}


void R2Image::
svdTest(void)
{
	// fit a 2D conic to five points
	R2Point p1(1.2, 3.5);
	R2Point p2(2.1, 2.2);
	R2Point p3(0.2, 1.6);
	R2Point p4(0.0, 0.5);
	R2Point p5(-0.2, 4.2);

	// build the 5x6 matrix of equations
	double** linEquations = dmatrix(1, 5, 1, 6);

	linEquations[1][1] = p1[0] * p1[0];
	linEquations[1][2] = p1[0] * p1[1];
	linEquations[1][3] = p1[1] * p1[1];
	linEquations[1][4] = p1[0];
	linEquations[1][5] = p1[1];
	linEquations[1][6] = 1.0;

	linEquations[2][1] = p2[0] * p2[0];
	linEquations[2][2] = p2[0] * p2[1];
	linEquations[2][3] = p2[1] * p2[1];
	linEquations[2][4] = p2[0];
	linEquations[2][5] = p2[1];
	linEquations[2][6] = 1.0;

	linEquations[3][1] = p3[0] * p3[0];
	linEquations[3][2] = p3[0] * p3[1];
	linEquations[3][3] = p3[1] * p3[1];
	linEquations[3][4] = p3[0];
	linEquations[3][5] = p3[1];
	linEquations[3][6] = 1.0;

	linEquations[4][1] = p4[0] * p4[0];
	linEquations[4][2] = p4[0] * p4[1];
	linEquations[4][3] = p4[1] * p4[1];
	linEquations[4][4] = p4[0];
	linEquations[4][5] = p4[1];
	linEquations[4][6] = 1.0;

	linEquations[5][1] = p5[0] * p5[0];
	linEquations[5][2] = p5[0] * p5[1];
	linEquations[5][3] = p5[1] * p5[1];
	linEquations[5][4] = p5[0];
	linEquations[5][5] = p5[1];
	linEquations[5][6] = 1.0;

	printf("\n Fitting a conic to five points:\n");
	printf("Point #1: %f,%f\n", p1[0], p1[1]);
	printf("Point #2: %f,%f\n", p2[0], p2[1]);
	printf("Point #3: %f,%f\n", p3[0], p3[1]);
	printf("Point #4: %f,%f\n", p4[0], p4[1]);
	printf("Point #5: %f,%f\n", p5[0], p5[1]);

	// compute the SVD
	double singularValues[7]; // 1..6
	double** nullspaceMatrix = dmatrix(1, 6, 1, 6);
	svdcmp(linEquations, 5, 6, singularValues, nullspaceMatrix);

	// get the result
	printf("\n Singular values: %f, %f, %f, %f, %f, %f\n", singularValues[1], singularValues[2], singularValues[3], singularValues[4], singularValues[5], singularValues[6]);

	// find the smallest singular value:
	int smallestIndex = 1;
	for (int i = 2; i<7; i++) if (singularValues[i]<singularValues[smallestIndex]) smallestIndex = i;

	// solution is the nullspace of the matrix, which is the column in V corresponding to the smallest singular value (which should be 0)
	printf("Conic coefficients: %f, %f, %f, %f, %f, %f\n", nullspaceMatrix[1][smallestIndex], nullspaceMatrix[2][smallestIndex], nullspaceMatrix[3][smallestIndex], nullspaceMatrix[4][smallestIndex], nullspaceMatrix[5][smallestIndex], nullspaceMatrix[6][smallestIndex]);

	// make sure the solution is correct:
	printf("Equation #1 result: %f\n", p1[0] * p1[0] * nullspaceMatrix[1][smallestIndex] +
		p1[0] * p1[1] * nullspaceMatrix[2][smallestIndex] +
		p1[1] * p1[1] * nullspaceMatrix[3][smallestIndex] +
		p1[0] * nullspaceMatrix[4][smallestIndex] +
		p1[1] * nullspaceMatrix[5][smallestIndex] +
		nullspaceMatrix[6][smallestIndex]);

	printf("Equation #2 result: %f\n", p2[0] * p2[0] * nullspaceMatrix[1][smallestIndex] +
		p2[0] * p2[1] * nullspaceMatrix[2][smallestIndex] +
		p2[1] * p2[1] * nullspaceMatrix[3][smallestIndex] +
		p2[0] * nullspaceMatrix[4][smallestIndex] +
		p2[1] * nullspaceMatrix[5][smallestIndex] +
		nullspaceMatrix[6][smallestIndex]);

	printf("Equation #3 result: %f\n", p3[0] * p3[0] * nullspaceMatrix[1][smallestIndex] +
		p3[0] * p3[1] * nullspaceMatrix[2][smallestIndex] +
		p3[1] * p3[1] * nullspaceMatrix[3][smallestIndex] +
		p3[0] * nullspaceMatrix[4][smallestIndex] +
		p3[1] * nullspaceMatrix[5][smallestIndex] +
		nullspaceMatrix[6][smallestIndex]);

	printf("Equation #4 result: %f\n", p4[0] * p4[0] * nullspaceMatrix[1][smallestIndex] +
		p4[0] * p4[1] * nullspaceMatrix[2][smallestIndex] +
		p4[1] * p4[1] * nullspaceMatrix[3][smallestIndex] +
		p4[0] * nullspaceMatrix[4][smallestIndex] +
		p4[1] * nullspaceMatrix[5][smallestIndex] +
		nullspaceMatrix[6][smallestIndex]);

	printf("Equation #5 result: %f\n", p5[0] * p5[0] * nullspaceMatrix[1][smallestIndex] +
		p5[0] * p5[1] * nullspaceMatrix[2][smallestIndex] +
		p5[1] * p5[1] * nullspaceMatrix[3][smallestIndex] +
		p5[0] * nullspaceMatrix[4][smallestIndex] +
		p5[1] * nullspaceMatrix[5][smallestIndex] +
		nullspaceMatrix[6][smallestIndex]);

	R2Point test_point(0.34, -2.8);

	printf("A point off the conic: %f\n", test_point[0] * test_point[0] * nullspaceMatrix[1][smallestIndex] +
		test_point[0] * test_point[1] * nullspaceMatrix[2][smallestIndex] +
		test_point[1] * test_point[1] * nullspaceMatrix[3][smallestIndex] +
		test_point[0] * nullspaceMatrix[4][smallestIndex] +
		test_point[1] * nullspaceMatrix[5][smallestIndex] +
		nullspaceMatrix[6][smallestIndex]);

	return;
}



////////////////////////////////////////////////////////////////////////
// Image processing functions
// YOU IMPLEMENT THE FUNCTIONS IN THIS SECTION
////////////////////////////////////////////////////////////////////////

// Per-pixel Operations ////////////////////////////////////////////////


// for getFeaturePositions (detecting features)
// descending sort <r+g+b value, position> by the pixel's r+g+b value.
struct sortWhiteDescending {
	bool operator()(const std::pair<double, int> &left, const std::pair<double, int> &right) {
		return left.first > right.first;
	}
};


void R2Image::
Brighten(double factor)
{
	// Brighten the image by multiplying each pixel component by the factor.
	// This is implemented for you as an example of how to access and set pixels
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel(i, j) *= factor;
			Pixel(i, j).Clamp();
		}
	}
}

void R2Image::
SobelX(void)
{
	// Apply the Sobel oprator to the image in X direction

	const double kernel[3][3] = {
		{ -1, -2, -1 },
		{ 0, 0, 0 },
		{ 1, 2, 1 }
	};

	// copy image
	R2Image origImage = *this;

	// make boundaries 0?
	for (int x = 0; x < width; x++) {
		Pixel(x, 0) = R2null_pixel;
		Pixel(x, height - 1) = R2null_pixel;
	}

	for (int y = 0; y < height; y++) {
		Pixel(0, y) = R2null_pixel;
		Pixel(0, width - 1) = R2null_pixel;
	}

	// For every pixel (except border)
	for (int x = 1; x < width - 1; x++) {
		for (int y = 1; y < height - 1; y++) {
			// convolution using the original image
			R2Pixel p = R2null_pixel;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					p += (origImage.Pixel(x + j, y + i) * kernel[1 - j][1 - i]);
				}
			}

			// insert new pixel
			//     p.Clamp();   // NO CLAMPING FOR HARRIS
			Pixel(x, y) = p;
		}
	}
}

void R2Image::
SobelY(void)
{
	// Apply the Sobel oprator to the image in Y direction
	const double kernel[3][3] = {
		{ 1, 0, -1 },
		{ 2, 0, -2 },
		{ 1, 0, -1 }
	};

	// copy image
	R2Image origImage = *this;

	// make boundaries 0?
	for (int x = 0; x < width; x++) {
		Pixel(x, 0) = R2null_pixel;
		Pixel(x, height - 1) = R2null_pixel;
	}

	for (int y = 0; y < height; y++) {
		Pixel(0, y) = R2null_pixel;
		Pixel(0, width - 1) = R2null_pixel;
	}

	// For every pixel (except border)
	for (int x = 1; x < width - 1; x++) {
		for (int y = 1; y < height - 1; y++) {
			// convolution using the original image
			R2Pixel p = R2null_pixel;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					p += (origImage.Pixel(x + j, y + i) * kernel[1 - j][1 - i]);
				}
			}

			// insert new pixel 
			// p.Clamp();   // NO CLAMPING FOR HARRIS
			Pixel(x, y) = p;
		}
	}
}

void R2Image::
LoG(void)
{
	// Apply the LoG oprator to the image

	// FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
	fprintf(stderr, "LoG() not implemented\n");
}



void R2Image::
ChangeSaturation(double factor)
{
	// Changes the saturation of an image
	// Find a formula that changes the saturation without affecting the image brightness

	// FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
	fprintf(stderr, "ChangeSaturation(%g) not implemented\n", factor);
}


// Linear filtering ////////////////////////////////////////////////
void R2Image::
Blur(double sigma)
{
	// Gaussian blur of the image. Separable solution

	// COMPUTE KERNEL //////////////////////////////////////

	// Initialize kernel
	const int length = (int)(6 * sigma + 1);
	const int mid = length / 2;

	//double kernel[length];
	double *kernel = (double*)malloc(sizeof(double) * length);
	double sum = 0;

	// Compute Gaussian equation
	const double coef = 1 / (sqrt(2 * M_PI) * sigma);
	const double expCoef = -0.5 / (sigma * sigma);

	for (int x = 0; x < length; x++) {
		double a = x - mid;
		kernel[x] =
			coef * exp(expCoef * a * a);

		sum += kernel[x];
	}

	// Ensure kernel sum = 1
	for (int x = 0; x < length; x++) {
		kernel[x] /= sum;
	}

	R2Image tempImage = *this;

	// TWO-PASS FILTER //////////////////////////////////////

	// blur horizontally on original to create tempImage
	// For every pixel (excluding border)
	for (int x = mid; x < width - mid; x++) {
		for (int y = mid; y < height - mid; y++) {
			R2Pixel p = R2null_pixel;

			for (int i = -mid; i <= mid; i++) {
				p += (Pixel(x, y + i) * kernel[mid - i]);
			}

			// p.Clamp();   // NO CLAMPING FOR HARRIS
			tempImage.Pixel(x, y) = p;
		}
	}

	// blur vertically on tempImage to create final image
	// For every pixel (excluding border)
	for (int x = mid; x < width - mid; x++) {
		for (int y = mid; y < height - mid; y++) {
			R2Pixel p = R2null_pixel;

			for (int i = -mid; i <= mid; i++) {
				p += (tempImage.Pixel(x + i, y) * kernel[mid - i]);
			}

			// p.Clamp();   // NO CLAMPING FOR HARRIS
			Pixel(x, y) = p;
		}
	}
	free(kernel);
}



void R2Image::
Harris(double sigma)
{
	// Harris corner detector. Make use of the previously developed filters, such as the Gaussian blur filter
	// Output should be 50% grey at flat regions, white at corners and black/dark near edges

	// compute Ix and Iy with Sobel (NO CLAMP)

	R2Image Ix(*this);
	Ix.SobelX();
	R2Image Iy(*this);
	Iy.SobelY();

	// compute Ix^2, Iy^2 and Ix*Iy
	R2Image Ix_sq(Ix);
	R2Image Iy_sq(Iy);
	R2Image Ix_Iy(Ix);


	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Ix_sq.Pixel(x, y) *= Ix.Pixel(x, y);
			Iy_sq.Pixel(x, y) *= Iy.Pixel(x, y);
			Ix_Iy.Pixel(x, y) *= Iy.Pixel(x, y);
		}
	}


	// gaussian blur to all 3 (small kernel... determined by sigma?)
	// NO CLAMP

	Ix_sq.Blur(sigma);
	Iy_sq.Blur(sigma);
	Ix_Iy.Blur(sigma);


	// compute harris value for each pixel
	// det(A) - alpha*trace(A)
	const double alpha = 0.04;
	R2Pixel det = R2null_pixel;
	R2Pixel trace = R2null_pixel;

	const R2Pixel normalizer = R2Pixel(0.5, 0.5, 0.5, 1);
	// +0.5 makes edges (which were 0) visible. The Harris value of edges is negative


	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			det = Ix_sq.Pixel(x, y)*Iy_sq.Pixel(x, y) - Ix_Iy.Pixel(x, y)*Ix_Iy.Pixel(x, y);
			trace = Ix_sq.Pixel(x, y) + Iy_sq.Pixel(x, y);
			Pixel(x, y) = det - alpha * trace * trace + normalizer;
			Pixel(x, y).Clamp(); // TODO clamp here?
		}
	}
}



void R2Image::
DetectFeatures(double sigma, int numFeatures)
{
	const int sqRadius = 5;
	// changes the image to show features (in red)
	std::vector<int> featurePositions = getFeaturePositions(sigma, numFeatures, sqRadius);
	int x, y;
	for (int pos : featurePositions) {
		x = pos / height;
		y = pos % height;

		makeSquare(x, y, 1, 0, 0, sqRadius);
	}
}



void R2Image::
TrackFeatures(R2Image * imageB)
{
	// Tracks 150 features (sigma=2.0) from imageA (this) to imageB
	// Features shown with motion vectors

	R2Image outputImage = *imageB;
	const int numFeatures = 150;
	const int sqRadius = 5;
	const double sigma = 2.0;
	int xa, xb, ya, yb;

	// Detect 150 features on imageA
	std::vector<int> featuresA = getFeaturePositions(sigma, numFeatures, sqRadius);
	// std::vector<int> featuresA = {300378,333995,162119,162189,1177349,36364,1202224,1176873,101746,36392,1222998,101725,1091825,36322,21280,161761,161743,611879,101677,102546,101666,102573,101645,36598,331135,21109,336861,161437,1091633,549574,36889,1050362,161181,609679,783799,20973,100754,1092624,339236,327317,327250,1050488,1201887,1201875,1092966,553146,1178804,1222560,1222540,1222500,323872,21918,104477,545759,1031599,1050645,1090877,1175441,1075494,167978,105063,320575,1014369,1208037,105464,38548,169788,317728,169881,1174520,542019,542005,39187,158321,1038979,1174181,20048,39231,170818,315764,314873,349421,171829,172680,540732,1095567,1201306,173651,1180750,156473,1180773,312939,873825,156319,96116,40305,310947,872854,176496,309141,1052375,22988,534900,307168,1169592,178536,41467,1097708,362863,747077,180347,1098571,302383,602979,181378,868272,92020,91742,18218,300392,370532,112224,529452,182488,112305,182903,91183,91101,91085,1219593,91049,528361,1183641,183353,298587,1165865,113234,184209,23873,184809,296612,296598,296571,686690,987525,295627,1164882,187050,44013,1070467};
	// std::vector<int> featuresA = {420535,185714,420524,1001617,1001606,420641,185730,420486,1109776,185741,420673,714047,714015,420722,1064770,420368,1109826,420806,714004,420297,185632,420900,1190672,185618,713984,420226,713973,420215,1001390,185599,420986,185823,1190613,420997,421031,1001815,185587,421042,1001873,421056,421067,420124,185576,713962,185845,185553,714281,185977,185518,1198411,1109678,185507,185496,714339,1064854,1064909,186023,185467,185451,1198389,186045,185425,186056,714703,1222357,1222337,186215,714753,422201,999810,714911,1002525,418957,186524,998837,1109249,185122,1065094,1109975,423564,1065116,998151,424403,186838,1221348,1065183,417915,1065228,1003335,424851,711231,187147,425049,1003504,997000,425346,996839,417298,187258,996175,1004403,1208217,427294,1110479,184354,416145,1004767,427818,427835,709882,427853,709232,428332,188229,428497,415466,184009,188421,188577,991878,1144494,1005751,991064,991042,991024,430771,430782,430793,431079,413933,431223,431255,431366,431388,431453,431597,431621,183191,432440,183154,189696,413017,183115,706420,988914,190086,1181050,433475,190280,988225};

	// Find those features from imageA on imageB
	std::vector<int> featuresB = findAFeaturesOnB(imageB, featuresA, sqRadius);
	// std::vector<int> featuresB = {316747,352280,174663,145832,1136033,47961,1152267,1189305,54684,51829,1193195,318605,1045719,45039,245834,141662,139724,613776,117108,110296,117096,113203,115156,265155,351341,219731,354186,114461,1026332,529436,9123,987945,176605,554023,759796,30651,278287,1031162,347926,359937,342656,1000546,1219117,1218145,1067013,508050,1192196,1192754,1190814,1187895,250950,29676,113187,527541,1014290,1018936,1046690,1218476,1032267,51801,79209,339822,1001862,1162878,116093,224714,181371,83350,126661,1142798,533401,532427,45983,175665,989999,1105993,33567,49868,174663,334051,261982,131335,145833,186182,501397,1039862,1155188,188114,1197022,356036,1198965,249496,825820,165983,45308,60413,328275,825806,187118,249671,998595,52736,488846,202554,1121558,161233,17561,1065995,124618,700041,192888,1055343,107500,551160,257182,842341,102651,76451,32451,317721,141888,127653,513155,129751,37349,164722,296440,105573,103636,1182108,100720,499586,1200873,187114,239119,1133184,283962,198670,34511,165667,201520,214026,315821,649261,979820,262984,1114906,197671,53689,1002290};
	// std::vector<int> featuresB = {327366,96497,610695,772017,880821,460107,288626,217075,1022531,80112,345750,840649,709141,375587,993609,200366,1156925,394002,708170,423140,394974,204781,977622,122249,703350,412510,702379,338710,1158933,239211,391192,346175,1125197,193331,389315,968106,196939,280927,972963,357779,375108,287589,245074,701409,40871,126809,932364,157985,114469,1143557,1105708,114457,116393,632654,1139753,1006229,154372,33762,262167,1141615,186998,295044,188929,487292,1160777,1159797,75630,685883,427924,957461,700438,914020,360230,118354,955528,1064966,187995,1023698,1044568,350561,1024679,981720,370824,164753,1156888,1030506,409242,1034390,933141,391215,714994,137063,385795,948674,963293,333136,757773,302087,16392,973025,977527,1170641,338926,1091153,88393,360565,976816,438382,406669,668530,582489,705318,474416,117195,557950,313876,216685,426490,201903,833525,1075241,980681,1069739,970038,935235,260033,423065,424036,368761,338051,379367,357192,583995,549459,666633,409482,409506,185103,394004,208259,153127,353470,328975,728403,1076457,279485,1151205,302971,175945,940117};

	// for (int i : featuresA) {
	//   std::cout << i << ",";
	// }

	for (int i = 0; i < numFeatures; i++) {
		xa = featuresA.at(i) / height;
		ya = featuresA.at(i) % height;
		xb = featuresB.at(i) / height;
		yb = featuresB.at(i) % height;

		// Add motion vectors
		outputImage.line(xa, xb, ya, yb, 1, 0, 0);
	}

	*this = outputImage;
}


void R2Image::
RANSAC(R2Image * imageB)
{
	// Tracks 150 features (sigma=2.0) from imageA (this) to imageB
	// Features shown with motion vectors
	// RANSAC (translation) = eliminate false input data

	R2Image outputImage = *imageB;
	const int sqRadius = 5;
	const int numFeatures = 150;
	const double sigma = 2.0;

	// Detect 150 features on imageA
	std::vector<int> featuresA = getFeaturePositions(sigma, numFeatures, sqRadius);

	// Find A's features on imageB
	std::vector<int> featuresB = findAFeaturesOnB(imageB, featuresA, sqRadius);

	const int minInliers = numFeatures / 2;
	const int numTrials = 10;
	const int distThreshold = 5; // pixels

								 // Set up random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, numFeatures - 1);

	int randIndex, count;
	int bestNumInliers = 0;

	int dist[numFeatures][2];
	short inliers[numFeatures];
	short temp[numFeatures];

	// initialize distance metric for all features
	for (int i = 0; i < numFeatures; i++) {
		dist[i][0] = (featuresB.at(i) / height) - (featuresA.at(i) / height);
		dist[i][1] = (featuresB.at(i) % height) - (featuresA.at(i) % height);
	}

	// Count for the most inliers >= minInliers
	// inlier array stores whether a feature vector is an inlier or not
	for (int trial = 0; trial < numTrials; trial++) {
		count = 0;
		randIndex = dis(gen);

		int xDistRand = dist[randIndex][0];
		int yDistRand = dist[randIndex][1];

		for (int i = 0; i < numFeatures; i++) {
			temp[i] = abs(xDistRand - dist[i][0]) <= distThreshold &&
				abs(yDistRand - dist[i][1]) <= distThreshold;

			if (temp[i] == 1)
				count++;
		}


		if (count > bestNumInliers) {
			bestNumInliers = count;
			for (int i = 0; i < numFeatures; i++) {
				inliers[i] = temp[i];
			}
		}
		else if (count < minInliers) {
			trial--; // repeat the process if below the min inlier threshold
		}
	}

	// Add motion vectors

	for (int i = 0; i < numFeatures; i++) {
		outputImage.line(
			featuresA.at(i) / height,
			featuresB.at(i) / height,
			featuresA.at(i) % height,
			featuresB.at(i) % height,
			!inliers[i], inliers[i], 0);   // GREEN = inlier, RED = outlier
	}

	*this = outputImage;
}

void R2Image::
DLTRANSAC(R2Image * imageB)
{
	// Tracks 150 features (sigma=2.0) from imageA (this) to imageB
	// Features shown with motion vectors
	// RANSAC (translation) = eliminate false input data
	// Based on DLT algorithm to find transformation matrix H

	R2Image outputImage = *imageB;
	const int sqRadius = 4;
	const int numFeatures = 150;
	const double sigma = 2.0;

	// Detect 150 features on imageA
	std::vector<int> featuresA = getFeaturePositions(sigma, numFeatures, sqRadius);

	// Find A's features on imageB
	std::vector<int> featuresB = findAFeaturesOnB(imageB, featuresA, sqRadius);

	const int minInliers = numFeatures * 1 / 3;
	const int numTrials = 700;
	const int distThreshold = 4; // pixels

								 // Set up random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, numFeatures - 1);
	std::vector<int> randIndexes;

	int r, count;
	int bestNumInliers = 0;
	bool inliers[numFeatures];
	bool temp[numFeatures];

	// #correspondences
	const int n = 4;
	double H[3][3];
	std::vector<R2Point> tracksA;
	std::vector<R2Point> tracksB;

	// loop to find best H
	for (int trial = 0; trial < numTrials; trial++) {
		tracksA.clear();
		tracksB.clear();
		randIndexes.clear();
		count = 0;

		// randomly pick 4 points
		for (int i = 0; i < n; i++) {
			do {
				r = dis(gen);
			} while (std::find(randIndexes.begin(), randIndexes.end(), r) != randIndexes.end());
			randIndexes.push_back(r);
			tracksA.push_back(R2Point(featuresA.at(r) / height, featuresA.at(r) % height));
			tracksB.push_back(R2Point(featuresB.at(r) / height, featuresB.at(r) % height));
		}

		HomoEstimate(H, tracksA, tracksB, n);

		// count inliers based on H
		for (int i = 0; i < numFeatures; i++) {
			int fA_x = featuresA.at(i) / height;
			int fA_y = featuresA.at(i) % height;
			int fB = featuresB.at(i);

			// matrix multiplication with H
			double HfA_x = H[0][0] * fA_x + H[0][1] * fA_y + H[0][2]; // H[0][2]*1
			double HfA_y = H[1][0] * fA_x + H[1][1] * fA_y + H[1][2];

			// todo: keep the shear factor to normalize z=1?
			double HfA_z = H[2][0] * fA_x + H[2][1] * fA_y + H[2][2];
			HfA_x /= HfA_z;
			HfA_y /= HfA_z;

			temp[i] = fabs(HfA_x - fB / height) <= distThreshold &&
				fabs(HfA_y - fB%height) <= distThreshold;// ? 1 : 0;

			if (temp[i])// == 1)
				count++;
		}

		// update bestH
		if (count < minInliers) {
			trial--; // repeat the process if below the minInlier threshold
		}
		else if (count > bestNumInliers) {
			bestNumInliers = count;

			for (int i = 0; i < numFeatures; i++)
				inliers[i] = temp[i];
		}
	}

	// Add motion vectors
	for (int i = 0; i < numFeatures; i++) {
		outputImage.line(
			featuresA.at(i) / height,
			featuresB.at(i) / height,
			featuresA.at(i) % height,
			featuresB.at(i) % height,
			inliers[i] ? 0 : 1, // RED = outlier
			inliers[i] ? 1 : 0, // GREEN = inlier
			0);
	}

	*this = outputImage;
}



void R2Image::
Sharpen()
{
	// Sharpen an image using a linear filter. Use a kernel of your choosing.

	const double kernel[3][3] = {
		{ 0, -1, 0 },
		{ -1, 5, -1 },
		{ 0, -1, 0 }
	};

	// copy image
	R2Image origImage = *this;

	// For every pixel (except border, which is unchanged)
	for (int x = 1; x < width - 1; x++) {
		for (int y = 1; y < height - 1; y++) {
			// convolution using the original image
			R2Pixel p = R2null_pixel;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					p += (origImage.Pixel(x + j, y + i) * kernel[1 - j][1 - i]);
				}
			}

			// insert new pixel
			p.Clamp();
			Pixel(x, y) = p;
		}
	}
}

void R2Image::
SharpenHighPass(double sigma, double contrast)
{
	// Sharpen with a high pass filter

	// const double kernel[3][3] = {
	//   {0, -1, 0},
	//   {-1, 5, -1},
	//   {0, -1, 0}
	// };

	// copy image
	R2Image origImage = *this;

	// get low frequency component - OK
	// (gaussian blur with sigma)
	R2Image lowImage = *this;
	lowImage.Blur(sigma);


	// get high frequency component
	R2Image highImage = *this;

	// lol idk
	// highImage.Sharpen();

	// high = original - low
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			highImage.Pixel(x, y) = this->Pixel(x, y) - lowImage.Pixel(x, y);
			highImage.Pixel(x, y).Clamp();
		}
	}


	// increase contrast of high frequency component
	highImage.Brighten(contrast);

	// recombine images
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Pixel(x, y) = lowImage.Pixel(x, y) + highImage.Pixel(x, y);
			Pixel(x, y).Clamp();
		}
	}




	// // DEBUG - return the image
	// for (int x = 0; x < width; x++) {
	//   for (int y = 0; y < height; y++) {
	//     Pixel(x,y) = highImage.Pixel(x,y);
	//   }
	// }
}

void R2Image::
Bilateral(double sigma)
{
	// COMPUTE KERNEL //////////////////////////////////////

	// Initialize kernel
	const int length = (int)(6 * sigma + 1);
	const int mid = length / 2;

	//double kernel[length];
	double *kernel = (double*)malloc(sizeof(double) * length);
	double sum = 0;

	// Compute Gaussian equation
	const double coef = 1 / (sqrt(2 * M_PI) * sigma);
	const double expCoef = -0.5 / (sigma * sigma);

	for (int x = 0; x < length; x++) {
		double a = x - mid;
		kernel[x] =
			coef * exp(expCoef * a * a);

		sum += kernel[x];
	}

	// Ensure kernel sum = 1
	for (int x = 0; x < length; x++) {
		kernel[x] /= sum;
	}




	// BILATERAL SHIT LOLOLOL

	free(kernel);
}

void R2Image::
Median(void)
{
	// for kernel
	const int length = 5;
	const int border = length / 2;
	const int size = length * length;
	const int mIndex = size / 2;

	R2Image origImage = *this;
	R2Pixel currPixel;

	double reds[size];
	double greens[size];
	double blues[size];
	double alphas[size];
	int currIndex;

	// For every pixel (except border)
	for (int x = border; x < width - border; x++) {
		for (int y = border; y < height - border; y++) {
			// get surrounding pixel r,g,b values;

			for (int i = -border; i <= border; i++) {
				for (int j = -border; j <= border; j++) {
					currIndex = (i + 1)*length + (j + 1);

					currPixel = origImage.Pixel(x + i, y + j);

					reds[currIndex] = currPixel.Red();
					greens[currIndex] = currPixel.Green();
					blues[currIndex] = currPixel.Blue();
					alphas[currIndex] = currPixel.Alpha();
				}
			}

			// sort to find median
			std::sort(reds, reds + size);
			std::sort(greens, greens + size);
			std::sort(blues, blues + size);
			std::sort(alphas, alphas + size);

			// replace in output image
			Pixel(x, y) = R2Pixel(
				reds[mIndex],
				greens[mIndex],
				blues[mIndex],
				alphas[mIndex]
			);
		}
	}
}

// HOLY FUCK SHIT THIS DOESNT WORK LOL
void R2Image::
Fisheye(void)
{
	// Lens distortion

	double nx, nxnx, ny, r, nr;
	int centerX = width / 2;
	int centerY = height / 2;

	R2Image outputImage(width, height);



	//   Pseudo code:
	// for each pixel (x,y)
	//     normalize (x,y) to (nx, ny) to be in range [-1,1]
	//     calculate distance from (nx, ny) to center (0,0)
	//     convert (nx,ny) to polar coordinates
	//     calculate new distance from center on the sphere surface
	//         The new distance is r' = r + (1 - sqrt(1 -r^2)) / 2
	//     translate (nx, ny) back to screen coordinates (x',y')

	// For each pixel(x,y)
	for (int x = 0; x < width; x++) {
		nx = 1.0*(x - centerX) / centerX;
		nxnx = nx*nx;

		for (int y = 0; y < height; y++) {

			// normalize (x,y) to (nx, ny) to be in range [-1,1]
			ny = 1.0*(y - centerY) / centerY;

			// calculate distance from (nx, ny) to center (0,0)
			r = sqrt(nxnx + ny*ny);

			// ignore pixels outside the circle?

			if (r >= 0.0 && r <= 1.0) {
				// calculate new distance from center on the sphere surface
				// The new distance is nr = r + (1 - sqrt(1 -r^2)) / 2

				nr = r + (1 - sqrt(1 - r*r)) / 2;

				// discard radius > 1?
				if (nr <= 1.0) {
					double theta = atan2(ny, nx);
					// calculate new x,y position with new distance in same angle
					double nxn = nr * cos(theta);
					double nyn = nr * sin(theta);

					// translate polar
					// map from -1 ... 1 to image coordinates
					int x2 = (int)(((nxn + 1)*width) / 2.0);
					// map from -1 ... 1 to image coordinates
					int y2 = (int)(((nyn + 1)*height) / 2.0);

					outputImage.Pixel(x2, y2) = this->Pixel(x, y);
				}

			}
		}
	}

	*this = outputImage;
}


void R2Image::
blendOtherImageTranslated(R2Image * otherImage)
{
	// find at least 100 features on this image, and another 100 on the "otherImage". Based on these,
	// compute the matching translation (pixel precision is OK), and blend the translated "otherImage" 
	// into this image with a 50% opacity.
	fprintf(stderr, "fit other image using translation and blend imageB over imageA\n");
	return;
}

void R2Image::
blendOtherImageHomography(R2Image * imageB)
{
	// DLT algorithm to find transformation matrix H

	R2Image outputImage(width, height);
	const int sqRadius = 4;
	const int numFeatures = 150;
	const double sigma = 2.0;

	// Detect 150 features on imageA
	std::vector<int> featuresA = getFeaturePositions(sigma, numFeatures, sqRadius);

	// Find A's features on imageB
	std::vector<int> featuresB = findAFeaturesOnB(imageB, featuresA, sqRadius);

	const int minInliers = numFeatures * 1 / 3;
	const int numTrials = 700;
	const int distThreshold = 4; // pixels

								 // Set up random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, numFeatures - 1);
	std::vector<int> randIndexes;

	int r, count;
	int bestNumInliers = 0;
	bool inliers[numFeatures];
	bool temp[numFeatures];

	// #correspondences
	const int n = 4;
	double H[3][3];
	std::vector<R2Point> tracksA;
	std::vector<R2Point> tracksB;

	// loop to find best H
	for (int trial = 0; trial < numTrials; trial++) {
		tracksA.clear();
		tracksB.clear();
		randIndexes.clear();
		count = 0;

		// randomly pick 4 points
		for (int i = 0; i < n; i++) {
			do {
				r = dis(gen);
			} while (std::find(randIndexes.begin(), randIndexes.end(), r) != randIndexes.end());
			randIndexes.push_back(r);
			tracksA.push_back(R2Point(featuresA.at(r) / height, featuresA.at(r) % height));
			tracksB.push_back(R2Point(featuresB.at(r) / height, featuresB.at(r) % height));
		}

		HomoEstimate(H, tracksA, tracksB, n);

		// count inliers based on H
		for (int i = 0; i < numFeatures; i++) {
			int fA_x = featuresA.at(i) / height;
			int fA_y = featuresA.at(i) % height;
			int fB = featuresB.at(i);

			// matrix multiplication with H
			double HfA_x = H[0][0] * fA_x + H[0][1] * fA_y + H[0][2]; // H[0][2]*1
			double HfA_y = H[1][0] * fA_x + H[1][1] * fA_y + H[1][2];

			// todo: keep the shear factor to normalize z=1?
			double HfA_z = H[2][0] * fA_x + H[2][1] * fA_y + H[2][2];
			HfA_x /= HfA_z;
			HfA_y /= HfA_z;

			temp[i] = fabs(HfA_x - fB / height) <= distThreshold &&
				fabs(HfA_y - fB%height) <= distThreshold;// ? 1 : 0;

			if (temp[i])// == 1)
				count++;
		}

		// update bestH
		if (count < minInliers) {
			trial--; // repeat the process if below the minInlier threshold
		}
		else if (count > bestNumInliers) {
			bestNumInliers = count;

			for (int i = 0; i < numFeatures; i++)
				inliers[i] = temp[i];
		}
	}

	////// FROM THIS POINT ON
	////// WARPING IMAGEA TO FIT IMAGEB

	// improve H using all good tracks
	tracksA.clear();
	tracksB.clear();

	for (int i = 0; i < numFeatures; i++) {
		if (inliers[i]) {
			tracksA.push_back(R2Point(featuresA.at(i) / height, featuresA.at(i) % height));
			tracksB.push_back(R2Point(featuresB.at(i) / height, featuresB.at(i) % height));
		}
	}

	assert(tracksA.size() == bestNumInliers && tracksB.size() == bestNumInliers);
	HomoEstimate(H, tracksA, tracksB, bestNumInliers);

	// DEBUG: print H
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << H[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	// warp image A. H*A
	// TODO inverse warp, not forward warp

	// compute inverse of H
	double H1[3][3];
	double det = H[0][0] * (H[1][1] * H[2][2] - H[2][1] * H[1][2]) -
		H[0][1] * (H[1][0] * H[2][2] - H[1][2] * H[2][0]) +
		H[0][2] * (H[1][0] * H[2][1] - H[1][1] * H[2][0]);

	assert(det != 0);

	double invdet = 1.0 / det;

	H1[0][0] = (H[1][1] * H[2][2] - H[2][1] * H[1][2]) * invdet;
	H1[0][1] = (H[0][2] * H[2][1] - H[0][1] * H[2][2]) * invdet;
	H1[0][2] = (H[0][1] * H[1][2] - H[0][2] * H[1][1]) * invdet;
	H1[1][0] = (H[1][2] * H[2][0] - H[1][0] * H[2][2]) * invdet;
	H1[1][1] = (H[0][0] * H[2][2] - H[0][2] * H[2][0]) * invdet;
	H1[1][2] = (H[1][0] * H[0][2] - H[0][0] * H[1][2]) * invdet;
	H1[2][0] = (H[1][0] * H[2][1] - H[2][0] * H[1][1]) * invdet;
	H1[2][1] = (H[2][0] * H[0][1] - H[0][0] * H[2][1]) * invdet;
	H1[2][2] = (H[0][0] * H[1][1] - H[1][0] * H[0][1]) * invdet;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {

			// matrix multiplication with H
			double HA_x = H1[0][0] * x + H1[0][1] * y + H1[0][2]; // H[0][2]*1
			double HA_y = H1[1][0] * x + H1[1][1] * y + H1[1][2];
			double HA_z = H1[2][0] * x + H1[2][1] * y + H1[2][2];
			HA_x /= HA_z;
			HA_y /= HA_z;

			// bilinear interpolation
			// such that x2-x1=1 and y2-y1=1
			double x1 = (int)HA_x;
			double y1 = (int)HA_y;
			double x2 = x1 + 1;
			double y2 = y1 + 1;

			if (validPixel(x1, y1) && validPixel(x2, y2)) {
				R2Pixel fxy1 = (x2 - HA_x)*Pixel(x1, y1) + (HA_x - x1)*Pixel(x2, y1);
				R2Pixel fxy2 = (x2 - HA_x)*Pixel(x1, y2) + (HA_x - x1)*Pixel(x2, y2);

				R2Pixel bilinearPix = (y2 - HA_y)*fxy1 + (HA_y - y1)*fxy2;

				outputImage.Pixel(x, y) = bilinearPix;
			}
		}
	}

	// blend outputImage and imageB 50%
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			outputImage.Pixel(x, y) = outputImage.Pixel(x, y)*0.5 + imageB->Pixel(x, y)*0.5;
		}
	}

	*this = outputImage;
}

void R2Image::
SkyRANSAC(R2Image * imageB)
{
	const std::vector<int> featuresA = this->SkyFeatures();
	std::vector<int> featuresB = imageB->SkyFeatures();

	const int numFeatures = featuresA.size();
	printf("PLZ BE >= 4   %d\n", numFeatures);

	const int minInliers = 4;
	const int numTrials = 800;
	const int distThreshold = 4; // pixels

	// Set up random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, numFeatures - 1);
	std::vector<int> randIndexes;

	int bestNumInliers = 0;
	//bool inliers[numFeatures];
	bool *inliers = (bool*)malloc(sizeof(bool) * numFeatures);
	//bool temp[numFeatures];
	bool *temp = (bool*)malloc(sizeof(bool) * numFeatures);
	int randIndex, count;
	//int dist[numFeatures][2];
	int **dist = (int**)malloc(sizeof(int*) * numFeatures);
	for (int i = 0; i < numFeatures; i++) {
		dist[i] = new int[2];
	}
	double xSum = 0.0;
	double ySum = 0.0;

	// initialize distance metric for all features
	for (int i = 0; i < numFeatures; i++) {
		dist[i][0] = (featuresB.at(i) / height) - (featuresA.at(i) / height);
		dist[i][1] = (featuresB.at(i) % height) - (featuresA.at(i) % height);
	}

	for (int trial = 0; trial < numTrials; trial++) {
		count = 0;
		randIndex = dis(gen);

		//some random feature's distance
		int xDistRand = dist[randIndex][0];
		int yDistRand = dist[randIndex][1];

		//find difference between the selected random feature and all the others
		//if its close enough, then copy a 1 into temp and up count
		for (int i = 0; i < numFeatures; i++) {
			temp[i] = abs(xDistRand - dist[i][0]) <= distThreshold &&
				abs(yDistRand - dist[i][1]) <= distThreshold;

			if (temp[i] == 1) {
				count++;
			}
		}

		//if the count is large enough
		if (count > bestNumInliers) {
			//set that count as the best number of inliers
			bestNumInliers = count;
			//copy all the true-false values into the inlier list
			for (int i = 0; i < numFeatures; i++) {
				inliers[i] = temp[i];
			}
		}
		else if (count < minInliers) {
			trial--; // repeat the process if below the min inlier threshold
		}
	}

	// Reject outliers
	std::vector<int> newFeaturesB;
	for (int i = 0; i < numFeatures; i++) {
		if (inliers[i] == 1) { // inlier
			xSum += dist[i][0];
			ySum += dist[i][1];
			newFeaturesB.push_back(featuresB.at(i));
		}
	}

	// Calculate average translation vector
	int avgX = (int)(xSum / numFeatures);
	int avgY = (int)(ySum / numFeatures);

	imageB->SetTranslationVector({avgX,avgY});
	imageB->SetSkyFeatures(newFeaturesB);

	free(inliers);
	free(temp);
	for (int i = 0; i < numFeatures; i++) {
		delete [] dist[i];
	}
	free(dist);
}


void R2Image::
SkyDLTRANSAC(R2Image * imageB, double H[3][3])
{
	// Tracks imageA's features (this) to imageB
	// Exterminates outliers based on RANSAC 
	// NEW: outliers are represented as -1 in the features vector --> the features vector will always be the same size
	// Also returns the resulting H matrix

	const std::vector<int> featuresA = this->SkyFeatures();
	std::vector<int> featuresB = imageB->SkyFeatures();

	const int numFeatures = featuresA.size();
	printf("PLZ BE >= 4   %d\n", numFeatures);

	const int minInliers = 4;
	const int numTrials = 800;
	const int distThreshold = 4; // pixels

								 // Set up random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, numFeatures - 1);
	std::vector<int> randIndexes;

	int r, count;
	int bestNumInliers = 0;
	//bool inliers[numFeatures];
	bool *inliers = (bool*)malloc(sizeof(bool) * numFeatures);
	//bool temp[numFeatures];
	bool *temp = (bool*)malloc(sizeof(bool) * numFeatures);

	// #correspondences
	const int n = 4;
	std::vector<R2Point> tracksA;
	std::vector<R2Point> tracksB;

	// loop to find best H
	for (int trial = 0; trial < numTrials; trial++) {
		tracksA.clear();
		tracksB.clear();
		randIndexes.clear();
		count = 0;

		// randomly pick 4 points
		for (int i = 0; i < n; i++) {
			do {
				r = dis(gen);
			} while (std::find(randIndexes.begin(), randIndexes.end(), r) != randIndexes.end()
				&& featuresA.at(r) != -1);
			randIndexes.push_back(r);
			tracksA.push_back(R2Point(featuresA.at(r) / height, featuresA.at(r) % height));
			tracksB.push_back(R2Point(featuresB.at(r) / height, featuresB.at(r) % height));
		}

		HomoEstimate(H, tracksA, tracksB, n);

		// count inliers based on H
		for (int i = 0; i < numFeatures; i++) {
			if (featuresA.at(i) != -1) {
				int fA_x = featuresA.at(i) / height;
				int fA_y = featuresA.at(i) % height;
				int fB = featuresB.at(i);

				// matrix multiplication with H
				double HfA_x = H[0][0] * fA_x + H[0][1] * fA_y + H[0][2]; // H[0][2]*1
				double HfA_y = H[1][0] * fA_x + H[1][1] * fA_y + H[1][2];

				// todo: keep the shear factor to normalize z=1?
				double HfA_z = H[2][0] * fA_x + H[2][1] * fA_y + H[2][2];
				HfA_x /= HfA_z;
				HfA_y /= HfA_z;

				if (fabs(HfA_x - fB / height) <= distThreshold &&
					fabs(HfA_y - fB%height) <= distThreshold) {
					temp[i] = 1;
					count++;
				}
				else {
					temp[i] = 0;
				}
			}
			else {
				temp[i] = 0;
			}
		}

		// update bestH
		if (count < minInliers) {
			trial--; // repeat the process if below the minInlier threshold
		}
		if (count > bestNumInliers) {
			bestNumInliers = count;

			for (int i = 0; i < numFeatures; i++)
				inliers[i] = temp[i];
		}
	}

	// DELETE outliers
	for (int i = 0; i < numFeatures; i++) {
		if (inliers[i] == 0) { // outlier
			featuresB.at(i) = -1;
		}
	}

	imageB->SetSkyFeatures(featuresB);
}






/// HELPER FUNCTIONS //////

bool R2Image::
validPixel(const int x, const int y)
{
	return (x >= 0 && x < width && y >= 0 && y < height);
}


void R2Image::
makeSquare(const int x, const int y, const double r, const double g, const double b, const int sqRadius)
{
	// makes a square with given radius, centered on the given point
	for (int i = -sqRadius; i <= sqRadius; i++) {
		for (int j = -sqRadius; j <= sqRadius; j++) {
			Pixel(x + i, y + j).Reset(r, g, b, 1);
		}
	}
}




// returns a vector of central feature pixel locations
std::vector<int> R2Image::
getFeaturePositions(const double sigma, const int numFeatures, const int sqRadius)
{
	R2Image tempThis = *this;
	tempThis.Harris(sigma);

	// Find features with high corner score
	// ensure features are separated

	// higher r+g+b value -> whiter -> higher corner score

	// <r+g+b value, location> sorted descending
	std::vector< std::pair<double, int> > values;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			values.push_back(std::pair<double, int>(
				tempThis.Pixel(x, y).Red() +
				tempThis.Pixel(x, y).Green() +
				tempThis.Pixel(x, y).Blue(),
				x*height + y)
			);
		}
	}

	std::sort(values.begin(), values.end(), sortWhiteDescending());


	// get the whitest features
	// collects central feature pixel locations in vector
	std::vector<int> pixelLoc;

	int featureCount = 0;

	for (std::pair<double, int> pr : values) {
		int x = pr.second / height;
		int y = pr.second % height;

		// make sure pixel is far from other feature
		if (!tempThis.Pixel(x - sqRadius, y - sqRadius).IsRed() &&
			!tempThis.Pixel(x - sqRadius, y + sqRadius).IsRed() &&
			!tempThis.Pixel(x + sqRadius, y - sqRadius).IsRed() &&
			!tempThis.Pixel(x + sqRadius, y + sqRadius).IsRed()) {

			pixelLoc.push_back(pr.second);
			featureCount++;

			tempThis.makeSquare(x, y, 1, 0, 0, sqRadius);
		}

		if (featureCount >= numFeatures) {
			break;
		}
	}

	return pixelLoc;
}

std::vector<int> R2Image::
findAFeaturesOnB(R2Image * imageB, const std::vector<int> featuresA, const int sqRadius)
{
	// FOR SKYREPLACEMENT, ASSUME SMALL MOTION
	// const int searchW = width/5-sqRadius;
	// const int searchH = height/5-sqRadius;
	const int searchW = 50;
	const int searchH = 50;
	std::vector<int> featuresB;


	/////// FIND ALL 150 FEATURES ON IMAGEB ///////
	R2Pixel currPixelA, currPixelB;
	int xa, ya, xb, yb;
	double ssd, ssdBest;

	// For each feature in imageA, run a local search
	for (int pos : featuresA) {
		xa = pos / height;
		ya = pos % height;
		ssdBest = INT_MAX;

		// Search loop centered at original location
		for (int i = -searchW; i <= searchW; i++) {
			for (int j = -searchH; j <= searchH; j++) {

				// check if the pixels of the search area is in bounds
				if (validPixel(xa + i - sqRadius, ya + j - sqRadius) &&
					validPixel(xa + i + sqRadius, ya + j + sqRadius)) {

					// Calculates SSD
					// SSD = sum of squared differences across the feature descriptor squares
					// (feature descriptor square is the size of my red/green squares)
					ssd = 0;
					for (int k = -sqRadius; k <= sqRadius; k++) {
						for (int l = -sqRadius; l <= sqRadius; l++) {
							currPixelA = this->Pixel(xa + k, ya + l);
							currPixelB = imageB->Pixel(xa + i + k, ya + j + l);

							ssd += (currPixelB.Red() - currPixelA.Red())*(currPixelB.Red() - currPixelA.Red()) +
								(currPixelB.Green() - currPixelA.Green())*(currPixelB.Green() - currPixelA.Green()) +
								(currPixelB.Blue() - currPixelA.Blue())*(currPixelB.Blue() - currPixelA.Blue());
							// ignore alpha?
						}
					}

					// Choose the best ssd
					if (ssd < ssdBest) {
						ssdBest = ssd;
						xb = xa + i;
						yb = ya + j;
					}
				}
			}
		}

		// (xb,yb) is the position with the min SSD (likely feature match)
		featuresB.push_back(xb*height + yb);


	}

	return featuresB;
}

/*

NOTES ON SKY DETECTION

1. if green and red are similar and blue > (green and red), then it is "blue"


2. "brightness" = whiteness of pixels. Sky is likely to be brighter than other things...


make the sky black!

FOR WHOLE VIDEO

*/

// input image = imageB with featuresB
void R2Image::
MakeSkyBlack(R2Image *newSky, const std::vector<int> featuresA) {
	double whitenessMin = 1.2;
	double whitenessMax = 1.4;
	double minBlue = 0.6;
	R2Image* warpedSky = new R2Image(*newSky);
	std::vector<double> h = H();

	// warp sky image - inverse warping
	double H1[3][3];
	double det = h.at(0) * (h.at(4) * h.at(8) - h.at(7) * h.at(5)) -
		h.at(1) * (h.at(3) * h.at(8) - h.at(5) * h.at(6)) +
		h.at(2) * (h.at(3) * h.at(7) - h.at(4) * h.at(6));


	// double det = H[0][0] * (H[1][1] * H[2][2] - H[2][1] * H[1][2]) -
	//              H[0][1] * (H[1][0] * H[2][2] - H[1][2] * H[2][0]) +
	//              H[0][2] * (H[1][0] * H[2][1] - H[1][1] * H[2][0]);
	if (det == 0) {
		printf("Oops determinant = 0\n");
		return;
	}

	double invdet = 1.0 / det;

	// H1[0][0] = (H[1][1] * H[2][2] - H[2][1] * H[1][2]) * invdet;
	// H1[0][1] = (H[0][2] * H[2][1] - H[0][1] * H[2][2]) * invdet;
	// H1[0][2] = (H[0][1] * H[1][2] - H[0][2] * H[1][1]) * invdet;
	// H1[1][0] = (H[1][2] * H[2][0] - H[1][0] * H[2][2]) * invdet;
	// H1[1][1] = (H[0][0] * H[2][2] - H[0][2] * H[2][0]) * invdet;
	// H1[1][2] = (H[1][0] * H[0][2] - H[0][0] * H[1][2]) * invdet;
	// H1[2][0] = (H[1][0] * H[2][1] - H[2][0] * H[1][1]) * invdet;
	// H1[2][1] = (H[2][0] * H[0][1] - H[0][0] * H[2][1]) * invdet;
	// H1[2][2] = (H[0][0] * H[1][1] - H[1][0] * H[0][1]) * invdet;


	H1[0][0] = (h.at(4) * h.at(8) - h.at(7) * h.at(5)) * invdet;
	H1[0][1] = (h.at(2) * h.at(7) - h.at(1) * h.at(8)) * invdet;
	H1[0][2] = (h.at(1) * h.at(5) - h.at(2) * h.at(4)) * invdet;
	H1[1][0] = (h.at(5) * h.at(6) - h.at(3) * h.at(8)) * invdet;
	H1[1][1] = (h.at(0) * h.at(8) - h.at(2) * h.at(6)) * invdet;
	H1[1][2] = (h.at(3) * h.at(2) - h.at(0) * h.at(5)) * invdet;
	H1[2][0] = (h.at(3) * h.at(7) - h.at(6) * h.at(4)) * invdet;
	H1[2][1] = (h.at(6) * h.at(1) - h.at(0) * h.at(7)) * invdet;
	H1[2][2] = (h.at(0) * h.at(4) - h.at(3) * h.at(1)) * invdet;


	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {

			// matrix multiplication with H
			double HA_x = H1[0][0] * x + H1[0][1] * y + H1[0][2]; // H[0][2]*1
			double HA_y = H1[1][0] * x + H1[1][1] * y + H1[1][2];
			double HA_z = H1[2][0] * x + H1[2][1] * y + H1[2][2];
			HA_x /= HA_z;
			HA_y /= HA_z;

			// bilinear interpolation
			// such that x2-x1=1 and y2-y1=1
			double x1 = (int)HA_x;
			double y1 = (int)HA_y;
			double x2 = x1 + 1;
			double y2 = y1 + 1;

			if (newSky->validPixel(x1, y1) && newSky->validPixel(x2, y2)) {
				R2Pixel fxy1 = (x2 - HA_x)*newSky->Pixel(x1, y1) + (HA_x - x1)*newSky->Pixel(x2, y1);
				R2Pixel fxy2 = (x2 - HA_x)*newSky->Pixel(x1, y2) + (HA_x - x1)*newSky->Pixel(x2, y2);

				R2Pixel bilinearPix = (y2 - HA_y)*fxy1 + (HA_y - y1)*fxy2;

				warpedSky->Pixel(x, y) = bilinearPix;
			}
		}
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			R2Pixel pix = Pixel(x, y);
			double red = pix.Red();
			double green = pix.Green();
			double blue = pix.Blue();

			// too low - reject
			// high  - accept
			// middle - linear function 

			// if BLUEEEE
			if (fabs(red - green) < 0.4
				&& blue > red
				&& blue > green
				&& blue > minBlue) {

				// CONSIDER BRIGHTNESS
				double whiteness = red + green + blue;
				if (whiteness >= whitenessMin && whiteness <= whitenessMax) {
					double skyWeight = (whiteness - whitenessMin) / (whitenessMax - whitenessMin);
					Pixel(x, y) = warpedSky->Pixel(x, y)*skyWeight + Pixel(x, y)*(1.0 - skyWeight);
				}
				else if (whiteness > whitenessMax) {
					Pixel(x, y) = warpedSky->Pixel(x, y);
				}
			}
		}
	}

	// replace newSky with warpedSky
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			newSky->SetPixel(x, y, warpedSky->Pixel(x, y));
		}
	}

	delete warpedSky;
}



// input image = imageB with featuresB
void R2Image::
MakeSkyBlackTranslation(R2Image *newSky) {
	double whitenessMin = 1.2;
	double whitenessMax = 1.4;
	double minBlue = 0.6;
	R2Image* warpedSky = new R2Image(*newSky);

	assert(this->translationVector.size() == 2);
	int dx = translationVector.at(0);
	int dy = translationVector.at(1);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (newSky->validPixel(x-dx, y-dy)) {
				warpedSky->Pixel(x, y) = newSky->Pixel(x-dx, y-dy);
			}
		}
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			R2Pixel pix = Pixel(x, y);
			double red = pix.Red();
			double green = pix.Green();
			double blue = pix.Blue();

			// too low - reject
			// high  - accept
			// middle - linear function 

			// if BLUEEEE
			if (fabs(red - green) < 0.4
				&& blue > red
				&& blue > green
				&& blue > minBlue) {

				// CONSIDER BRIGHTNESS
				double whiteness = red + green + blue;
				if (whiteness >= whitenessMin && whiteness <= whitenessMax) {
					double skyWeight = (whiteness - whitenessMin) / (whitenessMax - whitenessMin);
					Pixel(x, y) = warpedSky->Pixel(x, y)*skyWeight + Pixel(x, y)*(1.0 - skyWeight);
				}
				else if (whiteness > whitenessMax) {
					Pixel(x, y) = warpedSky->Pixel(x, y);
				}
			}
		}
	}

	// replace newSky with warpedSky
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			newSky->SetPixel(x, y, warpedSky->Pixel(x, y));
		}
	}

	delete warpedSky;
}


void R2Image::
line(int x0, int x1, int y0, int y1, float r, float g, float b)
{
	if (x0>x1)
	{
		int x = y1;
		y1 = y0;
		y0 = x;

		x = x1;
		x1 = x0;
		x0 = x;
	}
	int deltax = x1 - x0;
	int deltay = y1 - y0;
	float error = 0;
	float deltaerr = 0.0;
	if (deltax != 0) deltaerr = fabs(float(float(deltay) / deltax));    // Assume deltax != 0 (line is not vertical),
																		// note that this division needs to be done in a way that preserves the fractional part
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		Pixel(x, y).Reset(r, g, b, 1.0);
		error = error + deltaerr;
		if (error >= 0.5)
		{
			if (deltay>0) y = y + 1;
			else y = y - 1;

			error = error - 1.0;
		}
	}
	if (x0>3 && x0<width - 3 && y0>3 && y0<height - 3)
	{
		for (int x = x0 - 3; x <= x0 + 3; x++)
		{
			for (int y = y0 - 3; y <= y0 + 3; y++)
			{
				Pixel(x, y).Reset(r, g, b, 1.0);
			}
		}
	}
}


// Computes the H homography matrix from n>=4 point correspondences
void R2Image::
HomoEstimate(double H[3][3], const std::vector<R2Point> orig, const std::vector<R2Point> modified, const int n) {
	const double smallSingular = 0.0000000001; // 1e-10
	const double w = 1; // up to scale? works for 2D
	const double wprime = 1;
	double x, y, xprime, yprime;

	// assemble n matrices into a single 2n*9 matrix
	double** linEquations = dmatrix(1, 2 * n, 1, 9);

	// for each correspondence, compute the first 2 rows of A and put in single matrix
	for (int i = 0; i < n; i++) {
		x = orig.at(i).X();
		y = orig.at(i).Y();
		xprime = modified.at(i).X();
		yprime = modified.at(i).Y();

		linEquations[2 * i + 1][1] = 0;
		linEquations[2 * i + 1][2] = 0;
		linEquations[2 * i + 1][3] = 0;
		linEquations[2 * i + 1][4] = -wprime*x;
		linEquations[2 * i + 1][5] = -wprime*y;
		linEquations[2 * i + 1][6] = -wprime*w;
		linEquations[2 * i + 1][7] = yprime*x;
		linEquations[2 * i + 1][8] = yprime*y;
		linEquations[2 * i + 1][9] = yprime*w;

		linEquations[2 * i + 2][1] = wprime*x;
		linEquations[2 * i + 2][2] = wprime*y;
		linEquations[2 * i + 2][3] = wprime*w;
		linEquations[2 * i + 2][4] = 0;
		linEquations[2 * i + 2][5] = 0;
		linEquations[2 * i + 2][6] = 0;
		linEquations[2 * i + 2][7] = -xprime*x;
		linEquations[2 * i + 2][8] = -xprime*y;
		linEquations[2 * i + 2][9] = -xprime*w;
	}


	// SVD
	double singularValues[10]; //1..9
	double** nullspaceMatrix = dmatrix(1, 9, 1, 9);
	svdcmp(linEquations, 2 * n, 9, singularValues, nullspaceMatrix);

	// find smallest singular value
	int smallestIndex = 1;
	for (int i = 2; i<10; i++) {
		if (singularValues[i]<singularValues[smallestIndex])
			smallestIndex = i;
	}

	// todo what if its 0?
	const double scale = nullspaceMatrix[9][smallestIndex];
	if (scale == 0) {
		std::cout << "ERROR: scale = 0. cannot divide by 0\n";
	}

	// solution = nullspace = column in V corresponding to the smallest singular value (which should be 0)
	for (int i = 0; i < 9; i++) {
		// set "supersmall" singular values to 0
		// scale H so that last singular value = 1
		H[i / 3][i % 3] = fabs(nullspaceMatrix[i + 1][smallestIndex]) < smallSingular
			? 0
			: (nullspaceMatrix[i + 1][smallestIndex] / scale);
	}
}

// this = imageB with featuresB
void R2Image::
ImprovedH(double H[3][3], const std::vector<int> featuresA, const std::vector<int> featuresB, const int height)
{
	std::vector<R2Point> tracksA;
	std::vector<R2Point> tracksB;
	const int size = featuresB.size();
	assert(featuresA.size() == size);

	for (int i = 0; i < size; i++) {
		if (featuresA.at(i) != -1 && featuresB.at(i) != -1) {
			tracksA.push_back(R2Point(featuresA.at(i) / height, featuresA.at(i) % height));
			tracksB.push_back(R2Point(featuresB.at(i) / height, featuresB.at(i) % height));
		}
	}

	HomoEstimate(H, tracksA, tracksB, tracksA.size());
}



////////////////////////////////////////////////////////////////////////
// I/O Functions
////////////////////////////////////////////////////////////////////////

int R2Image::
Read(const char *filename)
{
	// Initialize everything
	if (pixels) { delete[] pixels; pixels = NULL; }
	npixels = width = height = 0;

	// Parse input filename extension
	char *input_extension;
	if (!(input_extension = (char*)strrchr(filename, '.'))) {
		printf("%s\n", filename);
		fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
		return 0;
	}

	// Read file of appropriate type
	if (!strncmp(input_extension, ".bmp", 4)) return ReadBMP(filename);
	else if (!strncmp(input_extension, ".ppm", 4)) return ReadPPM(filename);
	else if (!strncmp(input_extension, ".jpg", 4)) return ReadJPEG(filename);
	else if (!strncmp(input_extension, ".jpeg", 5)) return ReadJPEG(filename);

	// Should never get here
	fprintf(stderr, "Unrecognized image file extension");
	return 0;
}



int R2Image::
Write(const char *filename) const
{
	// Parse input filename extension
	char *input_extension;
	if (!(input_extension = (char*)strrchr(filename, '.'))) {
		fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
		return 0;
	}

	// Write file of appropriate type
	if (!strncmp(input_extension, ".bmp", 4)) return WriteBMP(filename);
	else if (!strncmp(input_extension, ".ppm", 4)) return WritePPM(filename, 1);
	else if (!strncmp(input_extension, ".jpg", 5)) return WriteJPEG(filename);
	else if (!strncmp(input_extension, ".jpeg", 5)) return WriteJPEG(filename);

	// Should never get here
	fprintf(stderr, "Unrecognized image file extension");
	return 0;
}



////////////////////////////////////////////////////////////////////////
// BMP I/O
////////////////////////////////////////////////////////////////////////

#if (RN_OS == RN_LINUX) && !WIN32

typedef struct tagBITMAPFILEHEADER {
	unsigned short int bfType;
	unsigned int bfSize;
	unsigned short int bfReserved1;
	unsigned short int bfReserved2;
	unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short int biPlanes;
	unsigned short int biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBTRIPLE {
	unsigned char rgbtBlue;
	unsigned char rgbtGreen;
	unsigned char rgbtRed;
} RGBTRIPLE;

typedef struct tagRGBQUAD {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;

#endif

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

#define BMP_BF_TYPE 0x4D42 /* word BM */
#define BMP_BF_OFF_BITS 54 /* 14 for file header + 40 for info header (not sizeof(), but packed size) */
#define BMP_BI_SIZE 40 /* packed size of info header */


static unsigned short int WordReadLE(FILE *fp)
{
	// Read a unsigned short int from a file in little endian format 
	unsigned short int lsb, msb;
	lsb = getc(fp);
	msb = getc(fp);
	return (msb << 8) | lsb;
}



static void WordWriteLE(unsigned short int x, FILE *fp)
{
	// Write a unsigned short int to a file in little endian format
	unsigned char lsb = (unsigned char)(x & 0x00FF); putc(lsb, fp);
	unsigned char msb = (unsigned char)(x >> 8); putc(msb, fp);
}



static unsigned int DWordReadLE(FILE *fp)
{
	// Read a unsigned int word from a file in little endian format 
	unsigned int b1 = getc(fp);
	unsigned int b2 = getc(fp);
	unsigned int b3 = getc(fp);
	unsigned int b4 = getc(fp);
	return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void DWordWriteLE(unsigned int x, FILE *fp)
{
	// Write a unsigned int to a file in little endian format 
	unsigned char b1 = (x & 0x000000FF); putc(b1, fp);
	unsigned char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
	unsigned char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
	unsigned char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



static int LongReadLE(FILE *fp)
{
	// Read a int word from a file in little endian format 
	int b1 = getc(fp);
	int b2 = getc(fp);
	int b3 = getc(fp);
	int b4 = getc(fp);
	return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void LongWriteLE(int x, FILE *fp)
{
	// Write a int to a file in little endian format 
	char b1 = (x & 0x000000FF); putc(b1, fp);
	char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
	char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
	char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



int R2Image::
ReadBMP(const char *filename)
{
	// Open file
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Unable to open image file: %s\n", filename);
		return 0;
	}

	/* Read file header */
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = WordReadLE(fp);
	bmfh.bfSize = DWordReadLE(fp);
	bmfh.bfReserved1 = WordReadLE(fp);
	bmfh.bfReserved2 = WordReadLE(fp);
	bmfh.bfOffBits = DWordReadLE(fp);

	/* Check file header */
	assert(bmfh.bfType == BMP_BF_TYPE);
	/* ignore bmfh.bfSize */
	/* ignore bmfh.bfReserved1 */
	/* ignore bmfh.bfReserved2 */
	assert(bmfh.bfOffBits == BMP_BF_OFF_BITS);

	/* Read info header */
	BITMAPINFOHEADER bmih;
	bmih.biSize = DWordReadLE(fp);
	bmih.biWidth = LongReadLE(fp);
	bmih.biHeight = LongReadLE(fp);
	bmih.biPlanes = WordReadLE(fp);
	bmih.biBitCount = WordReadLE(fp);
	bmih.biCompression = DWordReadLE(fp);
	bmih.biSizeImage = DWordReadLE(fp);
	bmih.biXPelsPerMeter = LongReadLE(fp);
	bmih.biYPelsPerMeter = LongReadLE(fp);
	bmih.biClrUsed = DWordReadLE(fp);
	bmih.biClrImportant = DWordReadLE(fp);

	// Check info header 
	assert(bmih.biSize == BMP_BI_SIZE);
	assert(bmih.biWidth > 0);
	assert(bmih.biHeight > 0);
	assert(bmih.biPlanes == 1);
	assert(bmih.biBitCount == 24);  /* RGB */
	assert(bmih.biCompression == BI_RGB);   /* RGB */
	int lineLength = bmih.biWidth * 3;  /* RGB */
	if ((lineLength % 4) != 0) lineLength = (lineLength / 4 + 1) * 4;
	assert(bmih.biSizeImage == (unsigned int)lineLength * (unsigned int)bmih.biHeight);

	// Assign width, height, and number of pixels
	width = bmih.biWidth;
	height = bmih.biHeight;
	npixels = width * height;

	// Allocate unsigned char buffer for reading pixels
	int rowsize = 3 * width;
	if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
	int nbytes = bmih.biSizeImage;
	unsigned char *buffer = new unsigned char[nbytes];
	if (!buffer) {
		fprintf(stderr, "Unable to allocate temporary memory for BMP file");
		fclose(fp);
		return 0;
	}

	// Read buffer 
	fseek(fp, (long)bmfh.bfOffBits, SEEK_SET);
	if (fread(buffer, 1, bmih.biSizeImage, fp) != bmih.biSizeImage) {
		fprintf(stderr, "Error while reading BMP file %s", filename);
		return 0;
	}

	// Close file
	fclose(fp);

	// Allocate pixels for image
	pixels = new R2Pixel[width * height];
	if (!pixels) {
		fprintf(stderr, "Unable to allocate memory for BMP file");
		fclose(fp);
		return 0;
	}

	// Assign pixels
	for (int j = 0; j < height; j++) {
		unsigned char *p = &buffer[j * rowsize];
		for (int i = 0; i < width; i++) {
			double b = (double) *(p++) / 255;
			double g = (double) *(p++) / 255;
			double r = (double) *(p++) / 255;
			R2Pixel pixel(r, g, b, 1);
			SetPixel(i, j, pixel);
		}
	}

	// Free unsigned char buffer for reading pixels
	delete[] buffer;

	// Return success
	return 1;
}



int R2Image::
WriteBMP(const char *filename) const
{
	// Open file
	FILE *fp = fopen(filename, "wb");
	if (!fp) {
		fprintf(stderr, "Unable to open image file: %s\n", filename);
		return 0;
	}

	// Compute number of bytes in row
	int rowsize = 3 * width;
	if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;

	// Write file header 
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = BMP_BF_TYPE;
	bmfh.bfSize = BMP_BF_OFF_BITS + rowsize * height;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = BMP_BF_OFF_BITS;
	WordWriteLE(bmfh.bfType, fp);
	DWordWriteLE(bmfh.bfSize, fp);
	WordWriteLE(bmfh.bfReserved1, fp);
	WordWriteLE(bmfh.bfReserved2, fp);
	DWordWriteLE(bmfh.bfOffBits, fp);

	// Write info header 
	BITMAPINFOHEADER bmih;
	bmih.biSize = BMP_BI_SIZE;
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;       /* RGB */
	bmih.biCompression = BI_RGB;    /* RGB */
	bmih.biSizeImage = rowsize * (unsigned int)bmih.biHeight;  /* RGB */
	bmih.biXPelsPerMeter = 2925;
	bmih.biYPelsPerMeter = 2925;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	DWordWriteLE(bmih.biSize, fp);
	LongWriteLE(bmih.biWidth, fp);
	LongWriteLE(bmih.biHeight, fp);
	WordWriteLE(bmih.biPlanes, fp);
	WordWriteLE(bmih.biBitCount, fp);
	DWordWriteLE(bmih.biCompression, fp);
	DWordWriteLE(bmih.biSizeImage, fp);
	LongWriteLE(bmih.biXPelsPerMeter, fp);
	LongWriteLE(bmih.biYPelsPerMeter, fp);
	DWordWriteLE(bmih.biClrUsed, fp);
	DWordWriteLE(bmih.biClrImportant, fp);

	// Write image, swapping blue and red in each pixel
	int pad = rowsize - width * 3;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			const R2Pixel& pixel = (*this)[i][j];
			double r = 255.0 * pixel.Red();
			double g = 255.0 * pixel.Green();
			double b = 255.0 * pixel.Blue();
			if (r >= 255) r = 255;
			if (g >= 255) g = 255;
			if (b >= 255) b = 255;
			fputc((unsigned char)b, fp);
			fputc((unsigned char)g, fp);
			fputc((unsigned char)r, fp);
		}

		// Pad row
		for (int i = 0; i < pad; i++) fputc(0, fp);
	}

	// Close file
	fclose(fp);

	// Return success
	return 1;
}



////////////////////////////////////////////////////////////////////////
// PPM I/O
////////////////////////////////////////////////////////////////////////

int R2Image::
ReadPPM(const char *filename)
{
	// Open file
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Unable to open image file: %s\n", filename);
		return 0;
	}

	// Read PPM file magic identifier
	char buffer[128];
	if (!fgets(buffer, 128, fp)) {
		fprintf(stderr, "Unable to read magic id in PPM file");
		fclose(fp);
		return 0;
	}

	// skip comments
	int c = getc(fp);
	while (c == '#') {
		while (c != '\n') c = getc(fp);
		c = getc(fp);
	}
	ungetc(c, fp);

	// Read width and height
	if (fscanf(fp, "%d%d", &width, &height) != 2) {
		fprintf(stderr, "Unable to read width and height in PPM file");
		fclose(fp);
		return 0;
	}

	// Read max value
	double max_value;
	if (fscanf(fp, "%lf", &max_value) != 1) {
		fprintf(stderr, "Unable to read max_value in PPM file");
		fclose(fp);
		return 0;
	}

	// Allocate image pixels
	pixels = new R2Pixel[width * height];
	if (!pixels) {
		fprintf(stderr, "Unable to allocate memory for PPM file");
		fclose(fp);
		return 0;
	}

	// Check if raw or ascii file
	if (!strcmp(buffer, "P6\n")) {
		// Read up to one character of whitespace (\n) after max_value
		int c = getc(fp);
		if (!isspace(c)) putc(c, fp);

		// Read raw image data 
		// First ppm pixel is top-left, so read in opposite scan-line order
		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				double r = (double)getc(fp) / max_value;
				double g = (double)getc(fp) / max_value;
				double b = (double)getc(fp) / max_value;
				R2Pixel pixel(r, g, b, 1);
				SetPixel(i, j, pixel);
			}
		}
	}
	else {
		// Read asci image data 
		// First ppm pixel is top-left, so read in opposite scan-line order
		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				// Read pixel values
				int red, green, blue;
				if (fscanf(fp, "%d%d%d", &red, &green, &blue) != 3) {
					fprintf(stderr, "Unable to read data at (%d,%d) in PPM file", i, j);
					fclose(fp);
					return 0;
				}

				// Assign pixel values
				double r = (double)red / max_value;
				double g = (double)green / max_value;
				double b = (double)blue / max_value;
				R2Pixel pixel(r, g, b, 1);
				SetPixel(i, j, pixel);
			}
		}
	}

	// Close file
	fclose(fp);

	// Return success
	return 1;
}



int R2Image::
WritePPM(const char *filename, int ascii) const
{
	// Check type
	if (ascii) {
		// Open file
		FILE *fp = fopen(filename, "w");
		if (!fp) {
			fprintf(stderr, "Unable to open image file: %s\n", filename);
			return 0;
		}

		// Print PPM image file 
		// First ppm pixel is top-left, so write in opposite scan-line order
		fprintf(fp, "P3\n");
		fprintf(fp, "%d %d\n", width, height);
		fprintf(fp, "255\n");
		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				const R2Pixel& p = (*this)[i][j];
				int r = (int)(255 * p.Red());
				int g = (int)(255 * p.Green());
				int b = (int)(255 * p.Blue());
				fprintf(fp, "%-3d %-3d %-3d  ", r, g, b);
				if (((i + 1) % 4) == 0) fprintf(fp, "\n");
			}
			if ((width % 4) != 0) fprintf(fp, "\n");
		}
		fprintf(fp, "\n");

		// Close file
		fclose(fp);
	}
	else {
		// Open file
		FILE *fp = fopen(filename, "wb");
		if (!fp) {
			fprintf(stderr, "Unable to open image file: %s\n", filename);
			return 0;
		}

		// Print PPM image file 
		// First ppm pixel is top-left, so write in opposite scan-line order
		fprintf(fp, "P6\n");
		fprintf(fp, "%d %d\n", width, height);
		fprintf(fp, "255\n");
		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				const R2Pixel& p = (*this)[i][j];
				int r = (int)(255 * p.Red());
				int g = (int)(255 * p.Green());
				int b = (int)(255 * p.Blue());
				fprintf(fp, "%c%c%c", r, g, b);
			}
		}

		// Close file
		fclose(fp);
	}

	// Return success
	return 1;
}



////////////////////////////////////////////////////////////////////////
// JPEG I/O
////////////////////////////////////////////////////////////////////////


// #define USE_JPEG
#ifdef USE_JPEG
extern "C" {
#   define XMD_H // Otherwise, a conflict with INT32
#   undef FAR // Otherwise, a conflict with windows.h
#   include "jpeg/jpeglib.h"
};
#endif



int R2Image::
ReadJPEG(const char *filename)
{
#ifdef USE_JPEG
	// Open file
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Unable to open image file: %s\n", filename);
		return 0;
	}

	// Initialize decompression info
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fp);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	// Remember image attributes
	width = cinfo.output_width;
	height = cinfo.output_height;
	npixels = width * height;
	int ncomponents = cinfo.output_components;

	// Allocate pixels for image
	pixels = new R2Pixel[npixels];
	if (!pixels) {
		fprintf(stderr, "Unable to allocate memory for BMP file");
		fclose(fp);
		return 0;
	}

	// Allocate unsigned char buffer for reading image
	int rowsize = ncomponents * width;
	if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
	int nbytes = rowsize * height;
	unsigned char *buffer = new unsigned char[nbytes];
	if (!buffer) {
		fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
		fclose(fp);
		return 0;
	}

	// Read scan lines 
	// First jpeg pixel is top-left, so read pixels in opposite scan-line order
	while (cinfo.output_scanline < cinfo.output_height) {
		int scanline = cinfo.output_height - cinfo.output_scanline - 1;
		unsigned char *row_pointer = &buffer[scanline * rowsize];
		jpeg_read_scanlines(&cinfo, &row_pointer, 1);
	}

	// Free everything
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	// Close file
	fclose(fp);

	// Assign pixels
	for (int j = 0; j < height; j++) {
		unsigned char *p = &buffer[j * rowsize];
		for (int i = 0; i < width; i++) {
			double r, g, b, a;
			if (ncomponents == 1) {
				r = g = b = (double) *(p++) / 255;
				a = 1;
			}
			else if (ncomponents == 1) {
				r = g = b = (double) *(p++) / 255;
				a = 1;
				p++;
			}
			else if (ncomponents == 3) {
				r = (double) *(p++) / 255;
				g = (double) *(p++) / 255;
				b = (double) *(p++) / 255;
				a = 1;
			}
			else if (ncomponents == 4) {
				r = (double) *(p++) / 255;
				g = (double) *(p++) / 255;
				b = (double) *(p++) / 255;
				a = (double) *(p++) / 255;
			}
			else {
				fprintf(stderr, "Unrecognized number of components in jpeg image: %d\n", ncomponents);
				return 0;
			}
			R2Pixel pixel(r, g, b, a);
			SetPixel(i, j, pixel);
		}
	}

	// Free unsigned char buffer for reading pixels
	delete[] buffer;

	// Return success
	return 1;
#else
	fprintf(stderr, "JPEG not supported");
	return 0;
#endif
}




int R2Image::
WriteJPEG(const char *filename) const
{
#ifdef USE_JPEG
	// Open file
	FILE *fp = fopen(filename, "wb");
	if (!fp) {
		fprintf(stderr, "Unable to open image file: %s", filename);
		return 0;
	}

	// Initialize compression info
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, fp);
	cinfo.image_width = width; 	/* image width and height, in pixels */
	cinfo.image_height = height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	cinfo.dct_method = JDCT_ISLOW;
	jpeg_set_defaults(&cinfo);
	cinfo.optimize_coding = TRUE;
	jpeg_set_quality(&cinfo, 95, TRUE);
	jpeg_start_compress(&cinfo, TRUE);

	// Allocate unsigned char buffer for reading image
	int rowsize = 3 * width;
	if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
	int nbytes = rowsize * height;
	unsigned char *buffer = new unsigned char[nbytes];
	if (!buffer) {
		fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
		fclose(fp);
		return 0;
	}

	// Fill buffer with pixels
	for (int j = 0; j < height; j++) {
		unsigned char *p = &buffer[j * rowsize];
		for (int i = 0; i < width; i++) {
			const R2Pixel& pixel = (*this)[i][j];
			int r = (int)(255 * pixel.Red());
			int g = (int)(255 * pixel.Green());
			int b = (int)(255 * pixel.Blue());
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			*(p++) = r;
			*(p++) = g;
			*(p++) = b;
		}
	}



	// Output scan lines
	// First jpeg pixel is top-left, so write in opposite scan-line order
	while (cinfo.next_scanline < cinfo.image_height) {
		int scanline = cinfo.image_height - cinfo.next_scanline - 1;
		unsigned char *row_pointer = &buffer[scanline * rowsize];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	// Free everything
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	// Close file
	fclose(fp);

	// Free unsigned char buffer for reading pixels
	delete[] buffer;

	// Return number of bytes written
	return 1;
#else
	fprintf(stderr, "JPEG not supported");
	return 0;
#endif
}






