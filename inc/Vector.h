#ifndef MATRIX_VECTOR_H
#define MATRIX_VECTOR_H

#include <stdexcept>
#include "Matrix.h"

class Vector : public Matrix {
	
	Matrix M;
	bool col;
	int len;
	
public:
	
	const int& size = len;
	
	Vector();
	Vector(int length,  bool column = true);
	Vector(VectorArray vals , bool column = true);
	Vector(const Matrix& matrix, bool column = true, int pos = 1);
	Vector(const Vector& rhs);
	Vector(Vector&& rhs);
	
	Vector& operator=(const Vector& rhs);
	Vector& operator=(Vector&& rhs);
	Vector& operator=(VectorArray rhs);
	
	Rational& operator()(int pos);
	const Rational& operator()(int pos) const;
	
	Vector operator+(const Vector& rhs) const;
	Vector& operator+=(const Vector& rhs);
	
	Rational operator*(const Vector& rhs) const; //this is the dot product
	
	Vector operator*(const Rational& rhs) const;
	Vector& operator*=(const Rational& rhs);
	
	Vector operator*(const Matrix& rhs) const;
	Vector& operator*=(const Matrix& rhs);
	
	operator Matrix() const;
	
	bool isColumn() const;
	bool isRow() const;
	
	Vector toColumn() const;
	Vector toRow() const;
	
	Vector& makeColumn();
	Vector& makeRow();
	
	Rational dot(Vector v) const;
	Rational inner(Vector v) const;
	Matrix outer(Vector v)const;
	
};

Vector operator*(const Rational& lhs, const Vector& rhs);

Vector operator*(const Matrix& lhs, const Vector& rhs);
