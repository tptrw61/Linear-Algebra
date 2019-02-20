#pragma once
#include "../Matrix.h"

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
	return removeZeroRows(A.transpose());
}

Matrix removeRows(Matrix& A, Array<int> pos) {
	pos.sort();
	Array<int> posActual(pos.size());
	int n = 0;
	for (int i = 0; i < pos.size(); i++)
		if (!posActual.in(pos[i]))
			posActual[n++] = pos[i];
	//n is amt of rows removed
}