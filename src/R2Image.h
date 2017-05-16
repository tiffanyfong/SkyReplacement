// Include file for image class
#ifndef R2_IMAGE_INCLUDED
#define R2_IMAGE_INCLUDED

#include <vector>



// Constant definitions

typedef enum {
  R2_IMAGE_RED_CHANNEL,
  R2_IMAGE_GREEN_CHANNEL,
  R2_IMAGE_BLUE_CHANNEL,
  R2_IMAGE_ALPHA_CHANNEL,
  R2_IMAGE_NUM_CHANNELS
} R2ImageChannel;

typedef enum {
  R2_IMAGE_POINT_SAMPLING,
  R2_IMAGE_BILINEAR_SAMPLING,
  R2_IMAGE_GAUSSIAN_SAMPLING,
  R2_IMAGE_NUM_SAMPLING_METHODS
} R2ImageSamplingMethod;

typedef enum {
  R2_IMAGE_OVER_COMPOSITION,
  R2_IMAGE_IN_COMPOSITION,
  R2_IMAGE_OUT_COMPOSITION,
  R2_IMAGE_ATOP_COMPOSITION,
  R2_IMAGE_XOR_COMPOSITION,
} R2ImageCompositeOperation;



// Class definition

class R2Image {
 public:
  // Constructors/destructor
  R2Image(void);
  R2Image(const char *filename);
  R2Image(int width, int height);
  R2Image(int width, int height, const R2Pixel *pixels);
  R2Image(const R2Image& image);
  ~R2Image(void);

  // Image properties
  int NPixels(void) const;
  int Width(void) const;
  int Height(void) const;
  std::vector<int> SkyFeatures(void) const;
  std::vector<double> H(void) const;
  std::vector<int> TranslationVector(void) const;

  // Pixel access/update
  R2Pixel& Pixel(int x, int y);
  R2Pixel *Pixels(void);
  R2Pixel *Pixels(int row);
  R2Pixel *operator[](int row);
  const R2Pixel *operator[](int row) const;
  void SetPixel(int x, int y,  const R2Pixel& pixel);
  void SetSkyFeatures(const std::vector<int> sf);
  void SetH(const std::vector<double> hmatrix);
  void SetTranslationVector(const std::vector<int> tv);

  // Image processing
  R2Image& operator=(const R2Image& image);

  // Per-pixel operations
  void Brighten(double factor);
  void ChangeSaturation(double factor);

  // show how SVD works
  void svdTest();

  // Linear filtering operations
  void SobelX();
  void SobelY();
  void LoG();
  void Blur(double sigma);
  void Harris(double sigma);
  void DetectFeatures(double sigma, int numFeatures);
  void TrackFeatures(R2Image * imageB);
  void RANSAC(R2Image * imageB);
  void DLTRANSAC(R2Image * imageB);
  void Sharpen(void);
  void SharpenHighPass(double sigma, double contrast);
  void Bilateral(double sigma);
  void Median();
  void Fisheye();

  // further operations
  void blendOtherImageTranslated(R2Image * otherImage);
  void blendOtherImageHomography(R2Image * imageB);



  // SKY REPLACEMENT
  // void SkyFirstFrameProcess
  void SkyFrameProcess(int i, R2Image * imageA, R2Image * imageB);
  void SkyRANSAC(R2Image * imageB);
  void MakeSkyBlack(R2Image * newSky, const std::vector<int> featuresA);
  void MakeSkyBlackTranslation(R2Image * newSky);
  void SkyDLTRANSAC(R2Image * imageB, double H[3][3]);

  // helper functions
  bool validPixel(const int x, const int y);
  void makeSquare(const int x, const int y, const double r, const double g, const double b, const int sqRadius);
  std::vector<int> getFeaturePositions(const double sigma, const int numFeatures, const int sqRadius);
  std::vector<int> findAFeaturesOnB(R2Image * imageB, const std::vector<int> featuresA, const int sqRadius);
  void line(int x0, int x1, int y0, int y1, float r, float g, float b);
  // todo this is unrelated to the image
  void HomoEstimate(double H[3][3], const std::vector<R2Point> orig, const std::vector<R2Point> modified, const int n);
  void ImprovedH(double H[3][3], const std::vector<int> featuresA, const std::vector<int> featuresB, const int height);


  // File reading/writing
  int Read(const char *filename);
  int ReadBMP(const char *filename);
  int ReadPPM(const char *filename);
  int ReadJPEG(const char *filename);
  int Write(const char *filename) const;
  int WriteBMP(const char *filename) const;
  int WritePPM(const char *filename, int ascii = 0) const;
  int WriteJPEG(const char *filename) const;

 private:
  // Utility functions
  void Resize(int width, int height);
  R2Pixel Sample(double u, double v,  int sampling_method);

 private:
  R2Pixel *pixels;
  int npixels;
  int width;
  int height;
  std::vector<int> skyFeatures;
  std::vector<double> h; // 9 vector
  std::vector<int> translationVector;

};



// Inline functions

inline int R2Image::
NPixels(void) const
{
  // Return total number of pixels
  return npixels;
}



inline int R2Image::
Width(void) const
{
  // Return width
  return width;
}



inline int R2Image::
Height(void) const
{
  // Return height
  return height;
}


inline std::vector<int> R2Image::
SkyFeatures(void) const
{
  return skyFeatures;
}

inline std::vector<double> R2Image::
H(void) const
{
  return h;
}



inline R2Pixel& R2Image::
Pixel(int x, int y)
{
  // Return pixel value at (x,y)
  // (pixels start at lower-left and go in row-major order)
  return pixels[x*height + y];
}



inline R2Pixel *R2Image::
Pixels(void)
{
  // Return pointer to pixels for whole image 
  // (pixels start at lower-left and go in row-major order)
  return pixels;
}



inline R2Pixel *R2Image::
Pixels(int x)
{
  // Return pixels pointer for row at x
  // (pixels start at lower-left and go in row-major order)
  return &pixels[x*height];
}



inline R2Pixel *R2Image::
operator[](int x) 
{
  // Return pixels pointer for row at x
  return Pixels(x);
}



inline const R2Pixel *R2Image::
operator[](int x) const
{
  // Return pixels pointer for row at x
  // (pixels start at lower-left and go in row-major order)
  return &pixels[x*height];
}



inline void R2Image::
SetPixel(int x, int y, const R2Pixel& pixel)
{
  // Set pixel
  pixels[x*height + y] = pixel;
}

inline void R2Image::
SetSkyFeatures(const std::vector<int> sf)
{
  skyFeatures = sf;
}

inline void R2Image::
SetH(const std::vector<double> hmatrix)
{
  h = hmatrix;
}

inline void R2Image::
SetTranslationVector(const std::vector<int> tv)
{
  translationVector = tv;
}

#endif
