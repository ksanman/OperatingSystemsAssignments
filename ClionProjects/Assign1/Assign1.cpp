#include <iostream>
#include<cstring>
#include<cmath>

// Prototypes
int fib(int n);
double e (int n);
double pi (int n);
double factorial (int n);
void help();
int sizeOf(char* array)
{

}

int main(int argc, char* args[])
{

    if(argc < 1)
    {
        help();
        return 0;
    }

    std::string functionToBeCalled = args[1];
    int numberOfIterations = std::stoi(args[2]);

    if(functionToBeCalled == "" || numberOfIterations == 0)
    {
        help();
    }
    else if(functionToBeCalled == "-fib")
    {
        if(numberOfIterations < 0 || numberOfIterations > 40)
        {
            help();
            return 0;
        }

        std::cout << fib(numberOfIterations) << std::endl;
    }
    else if(functionToBeCalled == "-e")
    {
        if(numberOfIterations < 0 || numberOfIterations > 30)
        {
            help();
            return 0;

        }

        std::cout << e(numberOfIterations) << std::endl;
    }
    else if(functionToBeCalled == "-pi")
    {
        if(numberOfIterations < 0 || numberOfIterations > 10)
        {
            help();
            return 0;
        }

        std::cout << pi(numberOfIterations) << std::endl;
    }
    else
    {
        help();
    }

    return 0;
}

// Function computes the fibonacci of n.
int fib(int n)
{
    // If n equals 0, return 0.
    if(n == 0)
        return 0;

    // If n equals 1, return 1.
    if(n ==1)
        return 1;

    // Return the function fib(n-1) + fib(n-2)
    return fib(n-1) + fib(n-2);
}


// Function computes the vale of 'e' using n iterations
double e (int n)
{
    double result = 2.0;
    double factorial = 1;

    for(int i = 2; i<n;++i)
    {
        factorial *= i;
        result += 1/factorial;
    }

    return result;
}

// Function computes Pi to n digits.
double pi (int n)
{
    double pi = 0.0;

    // Using Chudnovskys; algorithm, find pi.
    for(auto i = 0.0; i<n;++i)
    {
        pi += (std::pow(-1.0, i) * factorial(6.0 * i) * (13591409.0 + (54140134.0 * i))) / (factorial(3.0 * i) * pow(factorial(i), 3.0)*pow(640320.0, 3.0 * i + 3.0/2.0));
    }

    // Times pi by 2, then return the inverse.
    pi *= 12;
    double result = 1/pi;

    return result;
}

// Function computes the factorial of n.
double factorial(int n)
{
    double result = 1;

    for(double i = 2; i<n;++i)
    {
        result *= i;
    }

    return result;
}

void help()
{
    int input;
    std::cout<< "--- Assign 1 Help ---\n"
        << " -fib [n] : Compute the fibonacci of [n]. [n] may be 0 - 40\n"
        << " -e [n] : compute the value of 'e' using [n] iterations. [n] may be 0 - 30\n"
        << " -pi [n] : Compute Pu to [n] digits. [n] may be 0 - 10"
        << std::endl;

    std::cin.get();

}