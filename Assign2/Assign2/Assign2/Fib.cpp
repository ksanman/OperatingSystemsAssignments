#include "Fib.h"



Fib::Fib()
{
}


Fib::~Fib()
{
}

// Function computes the fibonacci of n.
int Fib::computeNthFib(int n)
{
	// If n equals 0, return 0.
	if (n == 0)
		return 0;

	// If n equals 1, return 1.
	if (n == 1)
		return 1;

	// Return the function fib(n-1) + fib(n-2)
	return computeNthFib(n - 1) + computeNthFib(n - 2);
}
