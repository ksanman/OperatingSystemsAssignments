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
void initiateThreads(int, int, std::vector<std::vector<int>>, std::unordered_map<int, std::vector<int>>&);
void detectAnomily(int, int, int, std::vector<int>&, std::vector<int>&, std::unordered_map<int, std::vector<int>>&, std::unordered_map<int, bool>, int& ,std::mutex&);
void printResults(std::vector<int>, int, int);
std::unordered_map<int, bool> makeHashTable(int);

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
	numberOfAnomolties = initiateThreads(maxNumberOfFrames, endPage, sequences, results);


	// Each thread will run an instance of the paging test, reporting back paging faults.
	// Print the results
//	printResults(results);

    std::cout << std::endl << "Anomily detected : " << numberOfAnomolties << " times." << std::endl;
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

int initiateThreads(int numberOfFrames, int numberOfPages, std::vector<std::vector<int>> sequences, std::unordered_map<int, std::vector<int>> &results) {
	std::mutex mutex;
	int numberOfAnomilties;
	std::vector<int> stats(3); // index 1 is sequence number, index 2 is the frame number, index 3 is the number of page faults

	std::unordered_map<int, bool> hashTable = makeHashTable(numberOfPages);
	int memorySize;
	auto hardware = std::thread();

	int numberOfThreads = hardware.hardware_concurrency();
	int run = 0;

	std::vector<std::thread> threads(numberOfThreads);
	for (int i = 0; i < sequences.size(); i++) {
		memorySize = 0;
		while (memorySize < numberOfFrames) {
			for (auto &workerThread : threads) {
				workerThread = std::thread(&detectAnomily, ++run, ++memorySize, i + 1, std::ref(sequences[i]), std::ref(stats), std::ref(results), hashTable, std::ref(numberOfAnomolties), std::ref(mutex));
			}

			for (auto &workerThread : threads)
			{
				workerThread.join();
			}
		}
	}
}

void detectAnomily(int runNumber, int numberOfFrames, int sequenceNumber, std::vector<int> &sequence, std::vector<int> &stats, std::unordered_map<int, std::vector<int>> &results, std::unordered_map<int, bool> hashTable, int& numberOfAnomolties, std::mutex& mutex) {

	std::unique_lock<std::mutex> lock(mutex);
	std::deque<int> queue(numberOfFrames);
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

	stats[0] = sequenceNumber;
	stats[1] = numberOfFrames;
	stats[2] = pageFault;
	results[runNumber] = stats;
	

	if (runNumber == 1)
		return;

	auto result1 = results[runNumber - 1];

	if (sequenceNumber > results[runNumber - 1][0])
		return;

	if (pageFault > results[runNumber - 1][2])
	{
		//printResults(results[runNumber - 1], pageFault, numberOfFrames);
		std::cout << "Anomily discovered!" << std::endl << "	Sequence number: " << sequenceNumber
			<< std::endl << "	Page faults: " << result1[2] << " @ Frame size: " << result1[1]
			<< std::endl << "	Page faults: " << pageFault << " @ Frame size: " << numberOfFrames << std::endl;
		++numberOfAnomolties;
	}

}

void printResults(std::vector<int> result1, int pageFault, int frameSize) {

			std::cout << "Anomily discovered!" << std::endl << "	Sequence number: " << result1[0]
				<< std::endl << "	Page faults: " << result1[2] << " @ Frame size: " << result1[1]
				<< std::endl << "	Page faults: " << pageFault<< " @ Frame size: " << frameSize << std::endl;
			//numberOfAnomilies++;
	

	//}

	//std::cout << std::endl << "Anomily detected : " << numberOfAnomilies << " times." << std::endl;
}


std::unordered_map<int, bool> makeHashTable(int size) {
	std::unordered_map<int, bool> hashtable(size);
	for (int i = 0; i<size; i++)
	{
		hashtable[i] = false;
	}

	return hashtable;
}
