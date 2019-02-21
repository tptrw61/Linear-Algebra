LIB_DIR=../lib/
INC_DIR=../include/Matrix/

.PHONY: clean move_headers move_libraries

DEFAULT: libMatrix.a

#empty targets
build: libMatrix.a
	rm *.o

#phony targets
clean:
	-rm libMatrix.a libMatrix.so Matrix.o Rational.o

move_headers: inc/Array.hpp inc/Rational.hpp inc/Matrix.hpp inc/ExtraOps.hpp
	cp inc/Array.hpp $(INC_DIR)
	cp inc/Rational.hpp $(INC_DIR)
	cp inc/Matrix.hpp $(INC_DIR)
	cp inc/ExtraOps.hpp $(INC_DIR)

move_libraries: libMatrix.a #libMatrix.so
	cp libMatrix.a $(LIB_DIR)
	#cp libMatrix.so $(LIB_DIR)

#real targets
libMatrix.a: Rational.o Matrix.o
	ar rcs libMatrix.a Matrix.o Rational.o

libMatrix.so: Rational.o Matrix.o
	g++ -shared -o libMatrix.so Rational.o Matrix.o

Matrix.o: inc/Matrix.hpp src/Matrix.cpp
	g++ -c -Wall -o Matrix.o src/Matrix.cpp

Rational.o: inc/Rational.hpp src/Rational.cpp
	g++ -c -Wall -o Rational.o src/Rational.cpp

