// Include files for R2 package
#ifndef R2_INCLUDED
#define R2_INCLUDED


#ifdef _WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif


// Standard dependency include files 

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>



// Class declarations

class R2Point;
class R2Vector;
class R2Line;
class R2Segment;



// Geometry include files

#include "R2Point.h"
#include "R2Vector.h"
#include "R2Line.h"
#include "R2Segment.h"



// Utility include files

#include "R2Distance.h"



#endif
