/*
*Kody Sanchez
*A01514541
*Assignment 2
*/

#include <iostream>
#include<string>
#include<cmath>
#include <iomanip>
#include <exception>
#include "Fib.h"
#include "e.h"
#include "pi.h"

// Prototypes
void help();

int main(int argc, char* args[])
{
	try
	{
		Fib fib;
		e e;
		pi pi;

		if (argc < 2)
		{
			help();
			return 0;
		}
		
		std::string functionToBeCalled = args[1];
		int numberOfIterations = std::stoi(args[2]);

		if (functionToBeCalled == "" || numberOfIterations == 0)
		{
			help();
		}
		else if (functionToBeCalled == "-fib")
		{
			if (numberOfIterations < 0 || numberOfIterations > 40)
			{
				help();
				return 0;
			}
			
			std::cout << fib.computeNthFib(numberOfIterations) << std::endl;
		}
		else if (functionToBeCalled == "-e")
		{
			if (numberOfIterations < 0 || numberOfIterations > 30)
			{
				help();
				return 0;

			}

			std::cout << std::setprecision(numberOfIterations + 1) << e.computeNthe(numberOfIterations) << std::endl;
		}
		else if (functionToBeCalled == "-pi")
		{
			if (numberOfIterations < 0 || numberOfIterations > 10)
			{
				help();
				return 0;
			}

			std::cout << std::setprecision(numberOfIterations + 1) << pi.computeNthPi(numberOfIterations) << std::endl;
		}
		else
		{
			help();
		}
	}
	catch (int n)
	{
		help();
		throw n;
	}
	return 0;
}

void help()
{
	std::cout << "--- Assign 1 Help ---\n"
		<< " -fib [n] : Compute the fibonacci of [n]. [n] may be 0 - 40\n"
		<< " -e [n] : compute the value of 'e' using [n] iterations. [n] may be 0 - 30\n"
		<< " -pi [n] : Compute Pi to [n] digits. [n] may be 0 - 10\n" 
		<< " Press any key to continue..."
		<< std::endl;

	std::cin.get();

}
