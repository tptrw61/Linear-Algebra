/*
Author: Timothy Williams
*/
#include "../inc/Matrix.hpp"
#include <stdexcept>
#include <string>

using namespace std;
bool Matrix::printOn = 0;
bool Matrix::rowOpsOn = 0;
FILE *Matrix::printStream = stdout;



Matrix::Matrix(int rows, int columns) {
	if (rows <= 0 || columns <= 0)
		throw invalid_argument("Arg error: dimentions must be greater than 0");
	this->m = rows;
	this->n = columns;
	this->ma = MatrixArray(this->M);
	for (int i = 0; i < this->M; i++)
		this->ma[i] = VectorArray(this->N);
}

Matrix::Matrix(initializer_list<initializer_list<Rational>> entries) {
	char throwException = 0;
	if (entries.size() == 0)
		throwException = 1;
	int n = throwException ? 0 : entries.begin()[0].size();
	if (!throwException && n == 0)
		throwException = 1;
	for (int i = 1; i < (int)entries.size() && !throwException; i++) {
		if (entries.begin()[i].size() == 0) {
			throwException |= 1;
		} else if ((int)entries.begin()[i].size() != n) {
			throwException |= 2;
		}
	}
	if (throwException & 1)
		throw invalid_argument("Arg error: cannot have size 0 entries");
	if (throwException & 2)
		throw invalid_argument("Arg error: cannot have differently sized entries");
	this->m = entries.size();
	this->n = n;
	this->ma = MatrixArray(this->M);
	for (int i = 0; i < this->M; i++)
		this->ma[i] = VectorArray(this->N);
	for (int i = 0; i < this->M; i++) {
		for (int j = 0; j < this->N; j++) {
			this->ma[i][j] = entries.begin()[i].begin()[j];
		}
	}
}

Matrix::Matrix(MatrixArray entries) {
	char throwException = 0;
	if (entries.size() == 0)
		throwException = 1;
	int n = throwException ? 0 : entries[0].size();
	if (!throwException && n == 0)
		throwException = 1;
	for (int i = 1; i < (int)entries.size() && !throwException; i++) {
		if (entries[i].size() == 0) {
			throwException |= 1;
		} else if ((int)entries[i].size() != n) {
			throwException |= 2;
		}
	}
	if (throwException & 1)
		throw invalid_argument("Arg error: cannot have size 0 entries");
	if (throwException & 2)
		throw invalid_argument("Arg error: cannot have differently sized entries");
	this->m = entries.size();
	this->n = n;
	this->ma = entries;
}

Matrix::Matrix(const Matrix& other) {
	this->m = other.M;
	this->n = other.N;
	this->ma = other.ma;
}

Matrix::Matrix(Matrix&& other) {
	this->m = other.M;
	this->n = other.N;
	this->ma.swap(other.ma);
}

Rational& Matrix::operator()(int row, int column) {
	if (row > this->M || row <= 0)
		throw invalid_argument("Dim error: arg 'row' out of range");
	if (column > this->N || column <= 0)
		throw invalid_argument("Dim error: arg 'column' out of range");
	return this->ma[row-1][column-1];
}

const Rational& Matrix::operator()(int row, int column) const {
	if (row > this->M || row <= 0)
		throw invalid_argument("Dim error: arg 'row' out of range");
	if (column > this->N || column <= 0)
		throw invalid_argument("Dim error: arg 'column' out of range");
	return this->ma[row-1][column-1];
}

Matrix& Matrix::operator=(const Matrix& rhs) {
	this->m = rhs.M;
	this->n = rhs.N;
	this->ma = rhs.ma.clone();
	return *this;
}

Matrix& Matrix::operator=(Matrix&& rhs) {
	this->m = rhs.M;
	this->n = rhs.N;
	this->ma.swap(rhs.ma);
	return *this;
}

Matrix Matrix::operator+(const Matrix& rhs) const {
	Matrix retval = this->clone();
	retval += rhs;
	return retval;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
	if (this->N != rhs.M)
		throw domain_error("Dim error: dim mismatch");
	Matrix retval(this->M, rhs.N);
	for (int i = 0; i < retval.M; i++) {
		for (int j = 0; j < retval.N; j++) {
			Rational sum;
			for (int k = 0; k < rhs.M; k++) {
				sum += this->ma[i][k] * rhs.ma[k][j];
			}
			retval.ma[i][j] = sum;
		}
	}
	return retval;
}

Matrix Matrix::operator*(const Rational& rhs) const {
	Matrix retval(*this);
	retval *= rhs;
	return retval;
}

Matrix operator*(const Rational& lhs, const Matrix& rhs) {
	return rhs * lhs;
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
	if (this->M != rhs.M || this->N != rhs.N)
		throw domain_error("Dim error: dim mismatch");
	for (int i = 0; i < this->M; i++)
		for (int j = 0; j < this->N; j++)
			this->ma[i][j] += rhs.ma[i][j];
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& rhs) {
	*this = *this * rhs;
	return *this;
}

Matrix& Matrix::operator*=(const Rational& rhs) {
	for (int i = 0; i < this->M; i++)
		for (int j = 0; j < this->N; j++)
			this->ma[i][j] *= rhs;
	return *this;
}

Matrix pow(const Matrix& B, int e) {
	if (e == 1)
		return B;
	if (B.M != B.N)
		throw domain_error("Dim error: matrix not square");
	if (e == 0) {
		if (B.invertable())
			return Matrix::identity(B.M, B.N);
		else
			throw domain_error("Inv error: matrix not invertable");
	}
	if (e < 0) {
		return pow(B.inverse(), -e);
	}
	
	Matrix retval(B);
	for (int i = 1; i < e; i++) //bc at i=0 the operation is: I*=B
		retval *= B;
	return retval;
}

void Matrix::swap(Matrix& other) {
	this->ma.swap(other.ma);
	
	int temp = this->m;
	this->m = other.m;
	other.m = temp;
	
	temp = this->n;
	this->n = other.n;
	other.n = temp;
}

Matrix Matrix::clone() const {
	return *this;
}

Matrix& Matrix::rowSwap(int row1, int row2, int print) {
	if (print == -1)
		print = Matrix::rowOpsOn || Matrix::printOn;
	if (row1 > this->M || row1 <= 0)
		throw invalid_argument("Dim error: arg 'row1' out of range");
	if (row2 > this->M || row2 <= 0)
		throw invalid_argument("Dim error: arg 'row2' out of range");
	if (row1 == row2)
		return *this;
	Rational temp;
	for (int i = 0; i < this->N; i++) {
		temp = this->ma[row1-1][i];
		this->ma[row1-1][i] = this->ma[row2-1][i];
		this->ma[row2-1][i] = temp;
	}
	if (print) fprintf(printStream, "R%d<->R%d\n", row1, row2);
	return *this;
}

Matrix& Matrix::rowScale(int row, const Rational& scalar, int print) {
	if (print == -1)
		print = Matrix::rowOpsOn || Matrix::printOn;
	if (row > this->M || row <= 0)
		throw invalid_argument("Dim error: arg 'row' out of range");
	for (int i = 0; i < this->N; i++)
		this->ma[row-1][i] *= scalar;
	if (scalar.isOne())
		return *this;
	if (print) fprintf(printStream, "R%d->%sR%d\n", row, scalar.isNegOne() ? "-" : scalar.toString().c_str(), row);
	return *this;
}

Matrix& Matrix::rowAdd(int rowStored, int rowScaled, const Rational& scalar, int print) {
	if (print == -1)
		print = Matrix::rowOpsOn || Matrix::printOn;
	if (rowStored > this->M || rowStored <= 0)
		throw invalid_argument("Dim error: arg 'rowStored' out of range");
	if (rowScaled > this->M || rowScaled <= 0)
		throw invalid_argument("Dim error: arg 'rowScaled' out of range");
	for (int i = 0; i < this->N; i++)
		this->ma[rowStored-1][i] += scalar * this->ma[rowScaled-1][i];
	if (print) fprintf(printStream, "R%d->R%d%s%sR%d\n", rowStored, rowStored, sign(scalar) == -1 ? "-" : "+", !abs(scalar).isOne() ? abs(scalar).toString().c_str() : "", rowScaled);
	return *this;
}

Matrix Matrix::zero(int rows, int columns) {
	if (rows <= 0)
		throw invalid_argument("Arg error: arg 'row' must be positive");
	if (columns <= 0)
		throw invalid_argument("Arg error: arg 'column' must be positive");
	Matrix Z(rows, columns);
	return Z;
}

Matrix Matrix::identity(int rows, int columns) {
	if (rows <= 0)
		throw invalid_argument("Arg error: arg 'row' must be positive");
	if (columns <= 0)
		throw invalid_argument("Arg error: arg 'column' must be positive");
	Matrix I(rows, columns);
	for (int i = 0; i < rows && i < columns; i++)
		I.ma[i][i] = 1;
	return I;
}

Matrix Matrix::fill(int rows, int columns, const Rational& r) {
	if (rows <= 0)
		throw invalid_argument("Arg error: arg 'row' must be positive");
	if (columns <= 0)
		throw invalid_argument("Arg error: arg 'column' must be positive");
	Matrix F(rows, columns);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			F.ma[i][j] = r;
	return F;
}

Rational Matrix::det() const {
	if (this->M != this->N)
		throw domain_error("Dim error: matrix not square");
	//special cases
	if (this->M == 1)
		return this->ma[0][0];
	if (this->M == 2)
		return (this->ma[0][0] * this->ma[1][1]) - (this->ma[0][1] * this->ma[1][0]);
	if (this->M == 3) {
		Rational d;
		for (int i = 0; i < 3; i++) {
			Rational term = this->ma[0][i % 3];
			term *= this->ma[1][(i+1) % 3];
			term *= this->ma[2][(i+2) % 3];
			d += term;
			term = this->ma[2][i % 3];
			term *= this->ma[1][(i+1) % 3];
			term *= this->ma[0][(i+2) % 3];
			d -= term;
		}
		return d;
	}
	//normal row reduction algorithm for finding determinant
	Matrix D = this->clone(); //T for triangular not transpose
	int sign = 1;
	int row, col;
	for (row = 0, col = 0; row < D.M && col < D.N; row++, col++) {
		//check if current pivot is zero
		if (D.ma[row][col].isZero()) {
			bool swapped = 0;
			for (int i = row + 1; i < D.M; i++) {
				if (D.ma[i][col].isNotZero()) {
					D.rowSwap(row+1, i+1, 0);
					sign = -sign;
					swapped = 1;
					break;
				}
			}
			//if the column was zero filled pivot and below, move on to next column in current pivot row
			if (!swapped) {
				return 0;
			}
		}
		//row reduce in pivot column
		for (int i = row + 1; i < D.M; i++) {
			if (D.ma[i][col].isZero())
				continue;
			Rational reduceMult = D.ma[row][col].inverse() * -D.ma[i][col]; //pivotVal^-1 * -otherVal
			D.rowAdd(i+1, row+1, reduceMult, 0);
		}
	}
	Rational retval = 1;
	for (int i = 0; i < D.M; i++)
		retval *= D.ma[i][i];
	if (sign < 0)
		retval = -retval;
	return retval;
}

bool Matrix::invertable() const {
	if (this->M != this->N)
		return 0;
	if (this->M == 1)
		return !this->ma[0][0].isZero();
	return this->det().isZero()  ? 0 : 1;
}

Matrix Matrix::transpose() const {
	Matrix retval(this->N, this->M);
	for (int i = 0; i < this->M; i++)
		for (int j = 0; j < this->N; j++)
			retval.ma[j][i] = this->ma[i][j];
	return retval;
}

Matrix Matrix::triangular(int print) const {
	//vector row reduction
	if (this->M == 1)
		return *this;
	if (this->N == 1) {
		for (int i = 0; i < this->M; i++)
			if (this->ma[i][0].isNotZero()) {
				Matrix T = Matrix::zero(this->M, 1);
				T.ma[0][0] = this->ma[i][0];
				return T;
			}
		return Matrix::zero(this->M, 1);
	}
	//regular row reduction
	Matrix T = this->clone(); //T for triangular not transpose
	if (print == SHOW_DEFAULT ? Matrix::printOn : print == SHOW_MATRIX) T.disp();
	int row, col;
	for (row = 0, col = 0; row < T.M && col < T.N; row++, col++) {
		//check if current pivot is zero
		if (T.ma[row][col].isZero()) {
			bool swapped = 0;
			for (int i = row + 1; i < T.M; i++) {
				if (T.ma[i][col].isNotZero()) {
					T.rowSwap(row+1, i+1, print);
					if (print == SHOW_DEFAULT ? Matrix::printOn : print == SHOW_MATRIX) T.disp();
					swapped = 1;
					break;
				}
			}
			//if the column was zero filled pivot and below, move on to next column in current pivot row
			if (!swapped) {
				row--;
				continue;
			}
		}
		//row reduce in pivot column
		for (int i = row + 1; i < T.M; i++) {
			if (T.ma[i][col].isZero())
				continue;
			Rational reduceMult = T.ma[row][col].inverse() * -T.ma[i][col]; //pivotVal^-1 * -otherVal
			T.rowAdd(i+1, row+1, reduceMult, print);
			if (print == SHOW_DEFAULT ? Matrix::printOn : print == SHOW_MATRIX) T.disp();
		}
	}
	return T;
}

Matrix Matrix::ref(int print) const {
	Matrix R = this->triangular(print);
	//vector ref
	if (R.M == 1 || R.N == 1) {
		R.rowScale(1, R.ma[0][0].inverse(), print);
		return R;
	}
	//ref
	int row, col;
	for (row = 0, col = 0; row < R.M && col < R.N; row++, col++) {
		//get next pivot
		if (R.ma[row][col].isZero()) {
			row--;
			continue;
		}
		if (R.ma[row][col].isOne()) //skip pivots that are already 1
			continue;
		R.rowScale(row+1, R.ma[row][col].inverse(), print);
		if (print == SHOW_DEFAULT ? Matrix::printOn : print == SHOW_MATRIX) R.disp();
	}
	return R;
}

Matrix Matrix::rref(int print) const {
	Matrix R = this->ref(print);
	//vector rref
	if (R.M == 1 || R.N == 1)
		return R;
	//rref
	int row, col;
	for (row = R.M - 1; row > 0; row--) {
		bool rowZeroFilled = 1;
		//find pivot col
		for (int i = 0; i < R.N; i++) {
			if (R.ma[row][i].isNotZero()) {
				rowZeroFilled = 0;
				col = i;
				break;
			}
		}
		if (rowZeroFilled) //continue if the row has no pivot
			continue;
		for (int i = row - 1; i >= 0; i--) {
			Rational scalar = -R.ma[i][col];
			if (scalar.isZero())
				continue;
			R.rowAdd(i+1, row+1, scalar, print);
			if (print == SHOW_DEFAULT ? Matrix::printOn : print == SHOW_MATRIX) R.disp();
		}
	}
	return R;
}

Matrix Matrix::augment(const Matrix& aug) const {
	if (this->M != aug.M)
		throw domain_error("Dim error: dim mismatch");
	Matrix A(this->M, this->N + aug.N);
	for (int i = 0; i < A.M; i++) {
		int j;
		for (j = 0; j < this->N; j++)
			A.ma[i][j] = this->ma[i][j];
		for (int k = 0; j < A.N; j++, k++)
			A.ma[i][j] = aug.ma[i][k];
	}
	return A;
}

Matrix Matrix::inverse(int print) const {
	if (!this->invertable())
		throw domain_error("Inv error: matrix not invertable");
	Matrix invAug = this->augment(identity(this->M, this->N));
	invAug = invAug.rref(print);
	Matrix Inv(this->M, this->N);
	for (int i = 0; i < Inv.M; i++)
		for (int j = 0; j < Inv.N; j++)
			Inv.ma[i][j] = invAug.ma[i][j+Inv.N];
	if (print == SHOW_DEFAULT ? Matrix::printOn : print == SHOW_MATRIX) Inv.disp();
	return Inv;
}


void Matrix::setRow(int row, initializer_list<Rational> entries) {
	if (row > this->M || row <= 0)
		throw invalid_argument("Dim error: arg 'row' out of range");
	if ((int)entries.size() != this->N)
		throw invalid_argument("Dim error: args don't match dims");
	this->ma[row-1] = entries;
}

void Matrix::setRow(int row, VectorArray entries) {
	if (row > this->M || row <= 0)
		throw invalid_argument("Dim error: arg 'row' out of range");
	if ((int)entries.size() != this->N)
		throw invalid_argument("Dim error: args don't match dims");
	this->ma[row-1] = entries;
}

void Matrix::setColumn(int column, initializer_list<Rational> entries) {
	if (column > this->N || column <= 0)
		throw invalid_argument("Dim error: arg 'column' out of range");
	if ((int)entries.size() != this->M)
		throw invalid_argument("Dim error: args don't match dims");
	for (int i = 0; i < this->M; i++)
		this->ma[i][column-1] = entries.begin()[i];
}

void Matrix::setColumn(int column, VectorArray entries) {
	if (column > this->N || column <= 0)
		throw invalid_argument("Dim error: arg 'column' out of range");
	if ((int)entries.size() != this->M)
		throw invalid_argument("Dim error: args don't match dims");
	for (int i = 0; i < this->M; i++)
		this->ma[i][column-1] = entries[i];
}

VectorArray Matrix::getRow(int row) const {
	if (row > this->M || row <= 0)
		throw invalid_argument("Dim error: arg 'row' out of range");
	return this->ma[row-1];
}

VectorArray Matrix::getColumn(int column) const {
	if (column > this->N || column <= 0)
		throw invalid_argument("Dim error: arg 'column' out of range");
	VectorArray retval(this->M);
	for (int i = 0; i < (int)retval.length(); i++)
		retval[i] = this->ma[i][column-1];
	return retval;
}

// Matrix Matrix::subMatrix(initializer_list<int> rows, initializer_list<int> columns, bool useRanges) const {
	// Matrix R; //R for retval
	// if (useRanges) {
		
	// } else { //individual selected
		// int nRows, nColumns;
		// nRows = rows.size() == 0 ? this->M : rows.size();
		// nColumns = columns.size() == 0 ? this->N : columns.size();
		// R = Matrix(nRows, nColumns);
		// Array<int> rowsArr = rows;
		// Array<int> colArr = columns;
		// for (int i = 0; i < nRows; i++) {
			
			// for (int j = 0; j < this->N; j++) {
				// if (colsArr.in(j))
			// }
		// }
	// }
// }

string Matrix::toString() const {
	string** strs = new string*[this->M];
	for (int i = 0; i < this->M; i++)
		strs[i] = new string[this->N];
	int longest = 0;
	for (int i = 0; i < this->M; i++) {
		for (int j = 0; j < this->N; j++) {
			strs[i][j] = this->ma[i][j].toString();
			int len = strs[i][j].length();
			longest = len > longest ? len : longest;
		}
	}
	string spaces;
	spaces.assign(longest + 3, ' ');
	string retval = "";
	for (int i = 0; i < this->M; i++) {
		for (int j = 0; j < this->N; j++) {
			retval += spaces.substr(0, spaces.length() - strs[i][j].length()) + strs[i][j];
		}
		retval += "\n";
	}
	retval += "\n";
	return retval;
}
string Matrix::toString(const string name) const {
	return name + " =\n" + this->toString();
}

void Matrix::disp() const {
	fputs(this->toString().c_str(), printStream);
}
void Matrix::disp(const string name) const {
	fputs(this->toString(name).c_str(), printStream);
}
