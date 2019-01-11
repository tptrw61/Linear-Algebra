/*
Author: Timothy Williams
*/
#include "../inc/Rational.h"
#include <stdexcept>

using namespace std;


inline void catchUndef(Rational* r) {
	if (!r->isDefined())
		fprintf(stderr, "Warning: Denominator of Rational at %p is 0\n         Value is undefined", r);
}


Rational::Rational(long n, long d) {
	int s = sign(d);
	this->N = n * s;
	this->D = d * s;
	catchUndef(this);
	this->simplify();
}

Rational& Rational::operator=(const Rational& rhs) {
	this->N = rhs.N;
	this->D = rhs.D;
	catchUndef(this);
	return *this;
}

Rational Rational::operator+(const Rational& rhs) const {
	if (this->D == 1 && rhs.D == 1)
		return Rational(this->N + rhs.N);
	1/this->D; 1/rhs.D; //error checking for divide by zero
	long newD = lcm(this->D, rhs.D);
	long leftNMult = newD / this->D;
	long rightNMult = newD / rhs.D;
	Rational retval;
	retval.D = newD;
	retval.N = (this->N * leftNMult) + (rhs.N * rightNMult);
	retval.simplify();
	return retval;
}
Rational Rational::operator*(const Rational& rhs) const {
	if (this->D == 1 && rhs.D == 1)
		return Rational(this->N * rhs.N);
	1/this->D; 1/rhs.D; //error checking for divide by zero
	Rational retval;
	if (this->N == 0 || rhs.N == 0)
		return retval;
	Rational tl_br(this->N, rhs.D);
	Rational bl_tr(rhs.N, this->D);
	retval.N = tl_br.N * bl_tr.N;
	retval.D = tl_br.D * bl_tr.D;
	return retval;
}

void Rational::simplify() {
	if (this->N == 0) {
		this->D = 1;
		return;
	}
	if (abs(this->N) <= 1 || this->D <= 1)
		return;
	long factor = gcf(abs(this->N), this->D);
	this->N /= factor;
	this->D /= factor;
}


void Rational::denominator(long d) {
	int s = sign(d);
	this->N *= s;
	this->D = d * s;
	this->simplify();
	catchUndef(this);
}



double floor(const Rational& r) {
	return (r.N - (r.N % r.D)) / r.D;
}
double ceil(const Rational& r) {
	return (r.N + r.D - (r.N % r.D)) / r.D;
}

//Euler's gcf algorithm
long gcf(long a, long b) {
	if (a <= 0 || b <= 0)
		return -1;
	long n, d, q, r;
	n = a; d = b;
	while (true) {
		r = n % d;
		q = n / d;
		//printf("%d=(%d)%d+(%d)\n", n, q, d, r);
		if (r == 0)
			return d;
		n = d;
		d = r;
	}
}


long lcm(long a, long b) {
	if (a <= 0 || b <= 0)
		return -1;
	long commonFactor = gcf(a, b);
	long uniqueFactorA = a / commonFactor;
	return uniqueFactorA * b; //b contains commonFactor and its own unique factors
}


int sign(long x) { return (x > 0) - (x < 0); };
int sign(Rational r) { return (r.numerator() > 0) - (r.numerator() < 0); };
Rational abs(Rational r) { return r.isNegative() ? -r : r; };

Rational pow(Rational x, int y) {
	if (y < 0)
		return pow(x.inverse(), -y);
	(long)x;
	if (x.isZero() && y == 0)
		return pow(0,0);
	if (y == 0)
		return 1;
	return Rational((long)pow(x.numerator(), y), (long)pow(x.denominator(), y));
}
