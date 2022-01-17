<<COMMENT
  Builds a test shared library to be used with generate_images.py.
  
  This program compiles the simple matrix LED library with a special TEST_EXTERNAL flag, which opens up
  a function in the library that returns the internal matrix buffer. The test application is also 
  compiled
COMMENT
g++ -c -fPIC -D TEST_EXTERNAL -I . -I ../../src ../../src/simple_matrix.cpp -o simple_matrix.o
g++ -c -fPIC -D TEST_EXTERNAL -I . -I ../../src test.cpp -o test.o
g++ -shared -Wl,-soname,libtest.so -L . -o libtest.so simple_matrix.o test.o 
# python generate_svg.py 
