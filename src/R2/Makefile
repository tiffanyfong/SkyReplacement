#
# Linux makefile for assignment #1
#

# works on Ubuntu 9.10:

AR=ar -r

# hack for the building of the library of Mac OS X 10.5.6 installed in Friend 016/017
AR=libtool -o


#
# List of files
#

CCSRCS=R2Distance.cpp \
  R2Segment.cpp R2Line.cpp R2Vector.cpp R2Point.cpp 
OBJS=$(CCSRCS:.cpp=.o)
INCS=$(HSRCS) $(CCSRCS:.cpp=.h)



#
# Options
#

CC=g++
CXX=g++
CPPFLAGS=-Wall -g -I. -DUSE_JPEG 
LDFLAGS=-g



#
# Output library name
#

LIB=./libR2.a



#
# Make targets
#

$(LIB): $(OBJS)
	    - rm -f $(LIB)
	    ${AR} $(LIB) $(OBJS) $(USER_OBJS)

clean:
	    -  rm -f *~ *.o *.obj *.pdb $(LIB)

