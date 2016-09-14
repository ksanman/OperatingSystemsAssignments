#include "e.h"



e::e()
{
}


e::~e()
{
}


// Function computes the vale of 'e' using n iterations
double e::computeNthe(int n)
{
	double result = 2.0;
	double factorial = 1;

	for (int i = 2; i<n;++i)
	{
		factorial *= i;
		result += 1 / factorial;
	}

	return result;
}
