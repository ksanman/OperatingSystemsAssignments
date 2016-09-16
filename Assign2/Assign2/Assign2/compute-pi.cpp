#include "compute-pi.h"

pi::pi()
{
}


pi::~pi()
{
}

// Function computes Pi to n digits.
double pi::computeNthPi(int n)
{
	double pi = 0.0;

	// Using Chudnovskys; algorithm, find pi.
	for (double i = 0.0; i<n;++i)
	{
		pi += (std::pow(-1.0, i) * factorial(6.0 * i) * (13591409.0 + (54140134.0 * i))) / (factorial(3.0 * i) * std::pow(factorial(i), 3.0) * std::pow(640320.0, 3.0 * i + 3.0 / 2.0));
	}

	// Times pi by 2, then return the inverse.
	pi *= 12;
	double result = 1 / pi;

	return result;
}

// Function computes the factorial of n.
double pi::factorial(double n)
{
	double result = 1;

	for (double i = 2; i<n;++i)
	{
		result *= i;
	}

	return result;
}

