/*
Kody Sanchez
CS3100 
Compute Pi
*/

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <unordered_map>
#include <vector>
#include "computeNthPi.h"
#include <functional>
#include <iomanip>

// Protypes

// compute the nth digit of pi.
void computePiDigit(int n, std::mutex &, std::unordered_map<int, int> &);

// Function creates a new queue. 
std::queue<int> createQueue();

// Function creates a new hash table.
std::unordered_map<int, int> createHashTable();

// Function intiliazes threads
void intitializeThreads(std::queue<int>&, std::unordered_map<int, int> &);

// Function prints pi. 
void printPi(std::unordered_map<int, int> piDigits);


// Main Function
int main()
{	
	// Create new FIFO queue
	std::queue<int> queue;
	queue = createQueue();

	// Create a hash table to store the digits of pi.
	std::unordered_map<int, int> hashTable;
	hashTable = createHashTable();

	// Fill the queue with 1000 tasks to compute pi
	intitializeThreads(std::ref(queue), std::ref(hashTable));

	return 0;
}

void computePiDigit(int n, std::mutex &mutex, std::unordered_map<int, int> &hashTable)
{
	std::cout << ".";
	std::cout.flush();

	std::unique_lock<std::mutex> lock(mutex);

	computeNthPi piDigit(n);
	int digit = piDigit.returnDigit();
	hashTable[n] = digit;
}

// Function creates a new queue. 
std::queue<int> createQueue()
{
	std::queue<int> queue;

	for (int i = 1; i <= 1000; i++)
	{
		queue.push(i);
	}

	return queue;
}

// Function creates a new hash table.
std::unordered_map<int, int> createHashTable()
{
	std::unordered_map<int, int> hashTable;

	return hashTable;
}

// Function intiliazes and runs threads
void intitializeThreads(std::queue<int> &queue, std::unordered_map<int, int> &hashTable)
{
	std::mutex mutex;
	auto hardware = std::thread();

	int numberOfThreads = hardware.hardware_concurrency();

	std::vector<std::thread> threads(numberOfThreads);

	unsigned int size = queue.size();
	int j = queue.front();
	while (!queue.empty())
	{
		for (int i = 0; i < numberOfThreads; i++)
		{		
			if (queue.empty())
				continue;

			// Start running a new thread
			threads[i] = std::thread(&computePiDigit, queue.front(), std::ref(mutex), std::ref(hashTable));

			// increment the digit we are working on. 
			queue.pop();

			threads[i].join();
		}

	}
	
	// print pi
	printPi(std::ref(hashTable));
}

// Function prints pi. 
void printPi(std::unordered_map<int, int> piDigits) 
{
	std::cout << std::endl;
	std::cout << "3.";
	for (unsigned int i = 1; i < piDigits.size(); i++)
	{
		std::cout << piDigits[i];
	}
}