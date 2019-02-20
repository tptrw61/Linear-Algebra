#pragma once
#include "Matrix.hpp"

//
bool isZeroMatrix(const Matrix& A);
bool isZeroVector(const VectorArray& v);

//left is default as it is more useful
Matrix pinv(const Matrix& A, bool left = 1);

//for full rank (linearly independent cols)
//pinv(A) * A = I
//(A' * A)^-1 * A'
Matrix pinv_left(const Matrix& A);

//for full rank of transpose (linearly independent rows)
//A * pinv(A) = I
//A' * (A * A')^-1
Matrix pinv_right(const Matrix& A);

//removes zero vector rows from rref(A)
Matrix reduceRowsToPivots(const Matrix& A);
//removes zero rows/columns from A (not rref(A))
Matrix removeZeroRows(const Matrix& A);
Matrix removeZeroColumns(const Matrix& A);

//removes all rows in 'pos' 
Matrix removeRows(const Matrix& A, Array<int> pos);
Matrix removeColumns(const Matrix& A, Array<int> pos);

//inserts 'ins' at 'pos'
Matrix insertRows(const Matrix& A, const Matrix& ins, int pos);
Matrix insertColumns(const Matrix& A, const Matrix& ins, int pos);

//------------------------- definitions -------------------------

bool isZeroMatrix(const Matrix& A) {
	for (int i = 1; i <= A.M; i++)
		if (!isZeroVector(A.getRow(i)))
			return 0;
	return 1;
}

bool isZeroVector(const VectorArray& v) {
	for (int i = 0; i < (int)v.size(); i++)
		if (v[i].isNotZero())
			return 0;
	return 1;
}

Matrix pinv(Matrix& A, bool left) {
	return left ? pinv_left(A) : pinv_right(A);
}

//(A' * A)^-1 * A'
Matrix pinv_left(const Matrix& A) {
	Matrix T = A.transpose();
	Matrix inv = (T * A).inverse(SHOW_NONE);
	return inv * T;
}

//A' * (A * A')^-1
Matrix pinv_right(const Matrix& A) {
	Matrix T = A.transpose();
	Matrix inv = (A * T).inverse(SHOW_NONE);
	return T * inv;
}

Matrix reduceRowsToPivots(const Matrix& A) {
	return removeZeroRows(A.rref(SHOW_NONE));
}

Matrix removeZeroRows(const Matrix& A) {
	Array<int> keeps(A.M);
	int n = 0;
	for (int r = 1; r <= A.M; r++) {
		if (!isZeroVector(A.getRow(r))) {
			keeps[r-1] = 1;
			n++
		} else {
			keeps[r-1] = 0;
		}
	}
	if (n == A.M)
		return A;
	if (n == 0)
		return Matrix::zero(1, A.N);
	Matrix M(1, A.N);
	int i, r;
	for (i = r = 1; r <= n; r++, i++) {
		while (!keeps[i-1]) i++;
		M.setRow(r, A.getRow(i));
	}
	return M;
}

Matrix removeZeroColumns(const Matrix& A) {
	return removeZeroRows(A.transpose()).transpose();
}

Matrix removeRows(const Matrix& A, Array<int> pos) {
	pos.sort();
	Array<int> posActual(pos.size());
	int n = 0;
	for (int i = 0; i < pos.size(); i++) {
		if (pos[i] < 1 || pos[i] > A.M)
			n = A(i, 1); //gets error
		if (!posActual.in(pos[i]))
			posActual[n++] = pos[i];
	}
	//n is amt of rows removed
	int rows = A.M - n;
	if (rows == 0)
		return Matrix::zero(1, A.N);
	if (rows == A.M)
		return A;
	Matrix M(rows, A.N);
	int i, j;
	for (i = j = 1; i <= rows; i++, j++) {
		while (posActual.in(j))
			j++;
		M.setRow(i, A.getRow(j));
	}
	return M;
}

Matrix removeColumns(const Matrix& A, Array<int> pos) {
	return removeRows(A.transpose(), pos).transpose();
}

Matrix insertRows(const Matrix& A, const Matrix& ins, int pos) {
	Matrix M(A.M + ins.M, A.N);
	int i = 1, j = 1, k;
	while (i < pos) {
		M.setRow(i, A.getRow(i));
		i++;
	}
	k = i;
	while (j <= ins.M) {
		M.setRow(i, ins.getRow(j));
		i++;
		j++;
	}
	while (k <= A.M) {
		M.setRow(i, A.getRow(k));
		i++;
		k++;
	}
	return M;
}

Matrix insertColumns(const Matrix& A, const Matrix& ins, int pos) {
	//fix this implementation
	return insertRows(A.transpose(), ins.transpose(), pos).transpose();
}
