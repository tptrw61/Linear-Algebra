
#pseudo targets
default: lib

lib: build
	rm *.o

build: Matrix.o Rational.o
	ar rcs libMatrix.a Matrix.o Rational.o

clean:
	-rm libMatrix.a Matrix.o Rational.o

#real targets
Matrix.o: inc/Matrix.h src/Matrix.cpp inc/Rational.h Rational.o
	g++ -c -o Matrix.o src/Matrix.cpp

Rational.o: inc/Rational.h src/Rational.cpp
	g++ -c -o Rational.o src/Rational.cpp

