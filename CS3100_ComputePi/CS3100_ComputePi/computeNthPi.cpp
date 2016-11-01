//#include "computeNthPi.h"
//
//
//computeNthPi::computeNthPi(int n)
//{
//	double digitToCompute = (double)n;
//	computeNthDigit(digitToCompute);
//}
//
//computeNthPi::~computeNthPi()
//{
//}
//
//void computeNthPi::computeNthDigit(double n)
//{
//	piDigit = 12 * (((std::pow(-1.0, n) * factorial(6.0 * n) * (13591409.0 + (54140134.0 * n))) / (factorial(3.0 * n) * std::pow(factorial(n), 3.0)* std::pow(640320.0, (3.0 * n) + (3.0 / 2.0)))));
//}
//
//// Function computes the factorial of n.
//double computeNthPi::factorial(int n)
//{
//	double result = 1;
//
//	for (double i = 2; i<n;++i)
//	{
//		result *= i;
//	}
//
//	return result;
//}