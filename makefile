
#pseudo targets
default: lib

lib: build
	rm *.o

build: Matrix.o Rational.o
	ar rcs libMatrix.a Matrix.o Rational.o

clean:
	-rm libMatrix.a Matrix.o Rational.o

#real targets
Matrix.o: inc/Matrix.hpp src/Matrix.cpp inc/Rational.hpp Rational.o
	g++ -c -Wall src/Matrix.cpp

Rational.o: inc/Rational.hpp src/Rational.cpp
	g++ -c -Wall src/Rational.cpp

