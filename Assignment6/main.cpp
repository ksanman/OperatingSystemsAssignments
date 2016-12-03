#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <random>
#include <vector>
#include <unordered_map>
#include <chrono>

// Prototypes
std::vector<int> generateSequence(int, int, int);
int initiateThreads(int, int, std::vector<std::vector<int>>);
void detectAnomily(int, int, std::vector<int>, std::unordered_map<int,std::vector<int>>&, std::mutex&);
void printResults(std::vector<int>, int, int);
std::unordered_map<int, bool> makeHashTable(int);
std::unordered_map <int, std::unordered_map<int, std::vector<int>>> instantiateResults();

int main() {
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();

	const int numberOfSequences = 100;
	const int beginPage = 1;
	const int endPage = 250;
	const int sequenceSize = 1000;
	const int maxNumberOfFrames = 100;
	const int numberOfTests = sequenceSize * maxNumberOfFrames;
	int numberOfAnomolties = 0;
	std::unordered_map<int, std::vector<int>> results(numberOfTests);

	std::cout << "Length of reference string: 1000" << std::endl << "Frames of physical memory: 100" << std::endl << std::endl;

	// Generate 100 sequences with random numbers from 1-250
	std::vector<std::vector<int>> sequences(numberOfSequences);
	for (std::vector<int>& e : sequences)
	{
		e = generateSequence(beginPage, endPage, sequenceSize);
	}

	// Set up threads
	numberOfAnomolties = initiateThreads(maxNumberOfFrames, endPage, sequences);

	// Print the number of anomilties found.
	std::cout << std::endl << "Anomily detected : " << numberOfAnomolties << " times." << std::endl;

	// Print the time.
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time = end - start;
	std::cout << "Time to complete: " << time.count() << std::endl;
}

std::vector<int> generateSequence(int begin, int end, int size) {

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randomNumbers(begin, end);

	std::vector<int> sequence(size);
	for (int& element : sequence)
	{
		element = randomNumbers(mt);
	}

	return sequence;
}

int initiateThreads(int numberOfFrames, int numberOfPages, std::vector<std::vector<int>> sequences) {
	std::mutex mutex;
	int numberOfAnomilties = 0;

	// int is the sequence number, each vector contains the runs and their results as a vector.
	std::unordered_map<int, std::unordered_map<int, std::vector<int>>> results = instantiateResults();
	std::vector<int> stats(3); // index 0 is the frame number, index 1 is the number of page faults

	std::unordered_map<int, bool> hashTable = makeHashTable(numberOfPages);
	int memorySize;
	auto hardware = std::thread();

	// Get the number of cores on the cpus
	int numberOfThreads = hardware.hardware_concurrency();

	std::vector<std::thread> threads(numberOfThreads);

	// Split each sequence into jobs for each frame.
	for (int i = 0; i < sequences.size(); i++) {
		memorySize = 0;
		while (memorySize < numberOfFrames) {
			for (auto &workerThread : threads) {
				workerThread = std::thread(&detectAnomily, ++memorySize, i + 1 , sequences[i], std::ref(results[i]), std::ref(mutex));
			}

			for (auto &workerThread : threads)
			{
				workerThread.join();
			}
		}

		// Print results for the sequence. 
		auto resultVector = results[i];
		for (int j = 1; j < resultVector.size(); j++)
		{
			auto result1 = resultVector[j];
			auto result2 = resultVector[j - 1];

			if (result1[1] > result2[1])
			{
				std::cout << "Anomily discovered!" << std::endl << "	Sequence number: " << i
					<< std::endl << "	Page faults: " << result1[1] << " @ Frame size: " << result1[0]
					<< std::endl << "	Page faults: " << result2[1] << " @ Frame size: " << result2[0] << std::endl;
				++numberOfAnomilties;
			}
		}

	}

	return numberOfAnomilties;
}

void detectAnomily(int numberOfFrames, int sequenceNumber, std::vector<int> sequence, std::unordered_map<int,std::vector<int>> &results, std::mutex& mutex) {

	std::vector<int> stats(2);
	std::deque<int> queue(numberOfFrames);
	std::unordered_map<int, bool> hashTable = makeHashTable(250);
	int pageFault = 0;

	for (auto &request : sequence)
	{
		if (hashTable[request - 1] == true)
		{
			continue;
		}
		else if (queue.size() < numberOfFrames)
		{
			queue.push_back(request);
			hashTable[request - 1] = true;
		}
		else
		{
			pageFault++;
			hashTable[queue.front() - 1] = false;
			queue.pop_front();
			queue.push_back(request);
			hashTable[request - 1] = true;
		}
	}

	stats[0] = numberOfFrames;
	stats[1] = pageFault;
	std::unique_lock<std::mutex> lock(mutex);

	results[numberOfFrames - 1] = stats;
}

std::unordered_map<int, bool> makeHashTable(int size) {
	std::unordered_map<int, bool> hashtable(size);
	for (int i = 0; i<size; i++)
	{
		hashtable[i] = false;
	}

	return hashtable;
}

std::unordered_map <int, std::unordered_map<int, std::vector<int>>> instantiateResults()
{
	std::unordered_map<int, std::vector<int>> runsVector(100);
	std::unordered_map<int, std::unordered_map<int, std::vector<int>>> resultMap(1000);

	for (int i = 0; i < 1000; i++)
	{
		resultMap[i] = runsVector;
	}

	return resultMap;

}
