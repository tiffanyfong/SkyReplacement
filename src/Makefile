#
# Unix/Linux makefile for assignment #1
#



# 
# List of source files
#

IMGPRO_SRCS=imgpro.cpp R2Image.cpp R2Pixel.cpp svd.cpp
IMGPRO_OBJS=$(IMGPRO_SRCS:.cpp=.o)



#
# Compile and link options
#

CC=g++
CPPFLAGS=-Wall -I. -Ijpeg/linux-src -g -DUSE_JPEG -std=c++11
LDFLAGS=-g



#
# Libraries
#

LIBS=R2/libR2.a jpeg/libjpeg.a



#
# Compile command
#

%.o: %.cpp
	    $(CC) $(CPPFLAGS) -c $< -o $@

#
# GNU Make: targets that don't build files
#

.PHONY: all clean distclean

#
# Make targets
#

all: $(LIBS) imgpro

imgpro: $(LIBS) $(IMGPRO_OBJS) 
	    $(CC) -o imgpro $(CPPFLAGS) $(LDFLAGS) $(IMGPRO_OBJS) $(LIBS) -lm

R2/libR2.a: 
	    cd R2; make

jpeg/libjpeg.a: 
	    cd jpeg; make

clean:
	    ${RM} -f */*.a */*/*.a *.o */*.o */*/*.o imgpro imgpro.exe $(LIBS)

distclean:  clean
	    ${RM} -f *~ 

