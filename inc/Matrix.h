#ifndef MATRIX_H
#define MATRIX_H

/*
Author: Timothy Williams
Created On : 09-09-2018
Modified On: 11-7-2018
*/

#include <stdio.h>
#include <initializer_list>
#include "Array.h"
#include "Rational.h"

#define SHOW_DEFAULT -1
#define SHOW_NONE 0
#define SHOW_MATRIX 1
#define SHOW_ROW_OPS 2

using VectorArray = Array<Rational>;
using MatrixArray = Array<VectorArray>;

class Matrix {
protected:
	
	MatrixArray ma;
	
	int m; //rows
	int n; //columns
	
public:
	
	static bool printOn; // = 0; //prints all steps when row reducing
	static bool rowOpsOn; // = 0; //prints all row operations when row reducing
	static FILE *printStream;
	
	/*
	 * parameters for .inverse(), .triangular(), .ref(), .rref()
	 * 
	 * Default values:
	 * printOn = 0;
	 * rowOpsOn = 0;
	 * 
	 * SHOW_DEFAULT   : Use user's entered values for printOn and rowOpsOn
	 * SHOW_NONE      : Print nothing
	 * SHOW_MATRIX    : Print matrix steps and row operations
	 * SHOW_ROW_OPS   : Print only row operations
	 * 
	 * displays matrix and row ops OR row ops OR neither without
	 *     changing static variables printOn and rowOpsOn
	 * printStream is the stream/file that disp() prints to, default is stdout
	 */
	
	//MxN matrix (M by N)
	const int& M = m;
	const int& N = n;
	
	Matrix(int rows, int columns);
	Matrix(std::initializer_list<std::initializer_list<Rational>> entries);
	Matrix(MatrixArray entries);
	Matrix(const Matrix& other);
	Matrix(Matrix&& other);
	Matrix() : Matrix(1, 1) {  }; //default
	~Matrix() {  };
	
	Rational& operator()(int row, int column);
	const Rational& operator()(int row, int column) const;
	
	Matrix& operator=(const Matrix& source);
	Matrix& operator=(Matrix&& other);
	
	Matrix operator+(const Matrix& rhs) const;
	Matrix& operator+=(const Matrix& rhs);
	
	Matrix operator*(const Matrix& rhs) const;
	Matrix operator*(const Rational& rhs) const;
	
	Matrix& operator*=(const Matrix& rhs);
	Matrix& operator*=(const Rational& rhs);
	
	
	void swap(Matrix& other); 
	Matrix clone() const; 
	
	Matrix& rowSwap(int row1, const int row2, int print = -1);
	Matrix& rowScale(int row, const Rational& scalar, int print = -1);
	Matrix& rowAdd(int rowStored, int rowScaled, const Rational& scalar, int print = -1); //rowStored -> rowStored + (scalar * rowScaled) 
	
	static Matrix zero(int rows, int columns);
	static Matrix identity(int rows, int columns);
	static Matrix fill(int rows, int columns, const Rational& r);
	
	Rational det() const;
	
	bool invertable() const;
	
	Matrix transpose() const;
	Matrix triangular(int print = SHOW_DEFAULT) const;
	Matrix ref(int print = SHOW_DEFAULT) const;
	Matrix rref(int print = SHOW_DEFAULT) const;
	Matrix augment(const Matrix& aug) const;
	Matrix inverse(int print = SHOW_DEFAULT) const;
	
	
	//TODO figure this out
	// array<double> characteristic();
	// array<double> eigen_values();
	// array<Vector> eigen_vectors();
	
	void setRow(int row, std::initializer_list<Rational> entries);
	void setRow(int row, VectorArray entries);
	void setColumn(int column, std::initializer_list<Rational> entries);
	void setColumn(int column, VectorArray entries);
	
	VectorArray getRow(int row) const;
	VectorArray getColumn(int column) const;
	
	//accept Array<int> instead of initializer_list<int>
	// Matrix subMatrix(std::initializer_list<int> rows, std::initializer_list<int> columns, bool useRanges = 1) const;
	
	std::string toString() const;
	std::string toString(const std::string name) const;
	void disp() const;
	void disp(const std::string name) const;
	
};

Matrix operator*(const Rational lhs, const Matrix& rhs);


Matrix pow(const Matrix& B, int e);





#endif