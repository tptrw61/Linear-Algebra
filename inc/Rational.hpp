#ifndef RATIONAL_H
#define RATIONAL_H

/*
Author: Timothy Williams
Created On : 10-02-2018
Modified On: 10-31-2018
*/

#include <string>
#include <iostream>


/*
;                 x - 1
sqrt(x) = 1 + -------------
;              1 + sqrt(x)
*/

/*
//Apply the destructor attribute to myCleanupFun() so that it is executed after main() 
void myCleanupFun (void) __attribute__ ((destructor));
//                       ^~~~~~~~~~~~^ ^~~~~~~~~~~~~^ 

//'constructor' instead of 'destructor' makes it run before main;
//they are automatically called
*/

class Rational;

int sign(long x);
int sign(Rational r);
Rational abs(Rational r);

long gcf(long a, long b); //lcd
long lcm(long a, long b);

Rational pow(Rational x, int y);



class Rational {
	
	// struct oarr {
		// int* ints;
		// const int length;
		// const int n;
		// oarr(int len) : length(len), n(len - 2) { ints = new int[len + 3]; };
		// int& operator[](int index) { return ints[index + 2]; };
		// ~oarr() { delete[] ints; };
	// };
	
	long N;
	long D;
	
public:
	
	Rational() { N = 0; D = 1; };
	Rational(long n, long d);
	Rational(long x) { N = x; D = 1; };
	// Rational(double number);
	//Rational(const std::string& expression); //"N/D" format only
	//Rational(const int*& contFrac, int arrayLength);
	Rational(const Rational& other) : Rational(other.N, other.D) {  };
	~Rational() {  };
	
	
	//inverse
	Rational inverse() const { return Rational(D, N); };
	
	//assignment
	Rational& operator=(const Rational& rhs);
	Rational& operator=(long rhs) { *this = Rational(rhs); return *this; };
	//Rational& operator=(double rhs) { *this = Rational(rhs); };
	
	//casts
	operator long() const { return N / D; };
	operator int() const { return N / D; };
	operator double() const { return (double)N / (double)D; };
	operator bool() const { return N/D || N; };
	
	//unary
	bool operator!() const { return !(bool)(*this); };
	Rational operator-() const { return Rational(-N, D); };
	Rational operator++() { N += D; return Rational(N, D); }; //prefix
	Rational operator++(int) { N += D; return Rational(N-D, D); }; //postfix (int is just there to tell the compiler that its postfix)
	Rational operator--() { N -= D; return Rational(N, D); };
	Rational operator--(int) { N -= D; return Rational(N+D, D); };
	
	//binary
	Rational operator+(const Rational& rhs) const;
	Rational operator-(const Rational& rhs) const { return *this + (-rhs); };
	Rational operator*(const Rational& rhs) const;
	Rational operator/(const Rational& rhs) const { return *this * rhs.inverse(); };
	Rational operator%(const Rational& rhs) const { Rational f = *this / rhs; return f.N % f.D; };
	
	Rational operator+(long rhs) const { return *this + Rational(rhs); };
	Rational operator-(long rhs) const { return *this - Rational(rhs); };
	Rational operator*(long rhs) const { return *this * Rational(rhs); };
	Rational operator/(long rhs) const { return *this / Rational(rhs); };
	Rational operator%(long rhs) const { return *this % Rational(rhs); };
	
	//binary-assign
	Rational& operator+=(const Rational& rhs) { *this = *this + rhs; return *this; };
	Rational& operator-=(const Rational& rhs) { *this = *this - rhs; return *this; };
	Rational& operator*=(const Rational& rhs) { *this = *this * rhs; return *this; };
	Rational& operator/=(const Rational& rhs) { *this = *this / rhs; return *this; };
	Rational& operator%=(const Rational& rhs) { *this = *this % rhs; return *this; };
	
	Rational& operator+=(long rhs) { *this = *this + rhs; return *this; };
	Rational& operator-=(long rhs) { *this = *this - rhs; return *this; };
	Rational& operator*=(long rhs) { *this = *this * rhs; return *this; };
	Rational& operator/=(long rhs) { *this = *this / rhs; return *this; };
	Rational& operator%=(long rhs) { *this = *this % rhs; return *this; };
	
	//comparison
	bool operator==(const Rational& rhs) const { return (*this - rhs); };
	bool operator!=(const Rational& rhs) const { return !(*this == rhs); };
	bool operator< (const Rational& rhs) const { return ((*this - rhs).numerator() < 0); };
	bool operator<=(const Rational& rhs) const{ return ((*this < rhs) || (*this == rhs)); };
	bool operator> (const Rational& rhs) const { return !(*this <= rhs); };
	bool operator>=(const Rational& rhs) const { return !(*this < rhs); };
	
	
	bool operator==(long rhs) const { return this->D == 1 ? this->N == rhs : 0; };
	bool operator!=(long rhs) const { return this->D == 1 ? this->N != rhs : 1; };
	bool operator< (long rhs) const { return this->D == 1 ? this->N <  rhs : *this <  Rational(rhs); };
	bool operator<=(long rhs) const { return this->D == 1 ? this->N <= rhs : *this <= Rational(rhs); };
	bool operator> (long rhs) const { return this->D == 1 ? this->N >  rhs : *this >  Rational(rhs); };
	bool operator>=(long rhs) const { return this->D == 1 ? this->N >= rhs : *this >= Rational(rhs); };
	
	// bool operator==(double rhs) { return *this == Rational(rhs); };
	// bool operator!=(double rhs) { return *this != Rational(rhs); };
	// bool operator< (double rhs) { return *this <  Rational(rhs); };
	// bool operator<=(double rhs) { return *this <= Rational(rhs); };
	// bool operator> (double rhs) { return *this >  Rational(rhs); };
	// bool operator>=(double rhs) { return *this >= Rational(rhs); };
	
	bool isDefined() const { return D; };
	bool isZero() const { return isDefined() && (N == 0); };
	bool isNotZero() const { return isDefined() && (N != 0); };
	bool isOne() const { return this->D == 1 && this->N == 1; };
	bool isNegOne() const { return this->D == 1 && this->N == -1; };
	bool isPositive() const { return isDefined() && (N > 0); };
	bool isNegative() const { return isDefined() && (N < 0); };
	
	//stream
	//std::ostream& operator<<(std::ostream& os, const Rational& rhs) { return os << to_string(rhs); };
	
	//remainder on mixed number
	long remainder() const { return sign(*this) >= 0 ? N % D : (N % D) - D; }; //signed
	long modulus() const { return N % D; };
	
	//rounding
	friend double floor(const Rational& r);// { return (N - (N % D)) / D; };
	friend double ceil(const Rational& r);// { return (N + D - (N % D)) / D; };
	
	//simplify
	void simplify();
	
	//value
	double valueDouble() const { return (double)(*this); };
	long valueLong() const { return (long)(*this); };
	int valueInt() const { return (int)(*this); };
	
	//to_string
	//friend std::string to_string(const Rational& r);
	std::string toString() const { 
		return this->D == 1 ? std::to_string(this->N) : std::to_string(this->N) + "/" + std::to_string(this->D);
	};
	void disp() const { puts(toString().c_str()); };
	
	//getters
	long numerator() const { return N; };
	long denominator() const { return D; };
	//setters
	void numerator(long n) { N = n; this->simplify(); };
	void denominator(long d);
	
	//special constants and stuff
	
	//find 
	//static Rational PI() { return Rational(833719, 265381); }; 
	//static Rational PHI() { return Rational(832040, 514229); };
	//add E
	
	
	
	//sqrt rational approx
	//fraction from continued fraction
	
};


#endif
