#!/bin/bash
<<COMMENT
  Builds a test shared library to be used with generate_images.py.
  
  This program compiles the simple matrix LED library with a special TEST_EXTERNAL flag, which opens up
  a function in the library that returns the internal matrix buffer. The test application is also 
  compiled
COMMENT

BUILD_FOLDER=build
DEFAULT_PROGRAM=all_gen.cpp
LIBRARY_SOURCE=../../src
IMAGES_FOLDER=gen_imgs

rm $IMAGES_FOLDER/*.png
mkdir -p $BUILD_FOLDER

# If we are given a second argument, which should be the C program name, then update it
if [[ -n "$2" ]] ; then
  DEFAULT_PROGRAM=$2
fi

# Compile the matrix library
g++ -c -fPIC -D TEST_EXTERNAL -I . -I $LIBRARY_SOURCE $LIBRARY_SOURCE/simple_matrix.cpp -o $BUILD_FOLDER/simple_matrix.o
# Compile the test application
g++ -c -fPIC -D TEST_EXTERNAL -I . -I $LIBRARY_SOURCE $DEFAULT_PROGRAM -o $BUILD_FOLDER/test.o
# Link the test application and library
g++ -shared -Wl,-soname,libtest.so -L . -o $BUILD_FOLDER/libtest.so $BUILD_FOLDER/simple_matrix.o $BUILD_FOLDER/test.o


python generate_svg.py $1
