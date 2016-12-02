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
void detectAnomily(int, int, int, std::vector<int>&, std::vector<int>&, std::unordered_map<int, std::vector<int>>&, std::unordered_map<int, bool>, std::mutex&);
void printResults(std::unordered_map<int, std::vector<int>>);
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
    std::unordered_map<int, std::vector<int>> results(numberOfTests);

    // Generate 100 sequences with random numbers from 1-250
    std::vector<std::vector<int>> sequences(numberOfSequences);
    for (std::vector<int>& e : sequences)
    {
        e = generateSequence(beginPage, endPage, sequenceSize);
    }

    // Set up threads
    initiateThreads(maxNumberOfFrames, endPage, sequences, results);


    // Each thread will run an instance of the paging test, reporting back paging faults.
    // Print the results
    printResults(results);

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

void initiateThreads(int numberOfFrames, int numberOfPages, std::vector<std::vector<int>> sequences, std::unordered_map<int, std::vector<int>> &results) {
    std::mutex mutex;
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
                workerThread = std::thread(&detectAnomily, ++run, ++memorySize, i+1, std::ref(sequences[i]), std::ref(stats), std::ref(results), hashTable, std::ref(mutex));
            }

            for (auto &workerThread : threads)
            {
                workerThread.join();
            }
        }
    }
}

void detectAnomily(int runNumber, int numberOfFrames, int sequenceNumber, std::vector<int> &sequence, std::vector<int> &stats, std::unordered_map<int, std::vector<int>> &results, std::unordered_map<int, bool> hashTable, std::mutex& mutex) {

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
            hashTable[ queue.front() - 1] = false;
            queue.pop_front();
            queue.push_back(request);
            hashTable[request - 1] = true;
        }
    }

    stats[0] = sequenceNumber;
    stats[1] = numberOfFrames;
    stats[2] = pageFault;
    results[runNumber] = stats;
}

void printResults(std::unordered_map<int, std::vector<int>> results) {
    int numberOfAnomilies = 0;
    std::cout << "Length of reference string: 1000" << std::endl << "Frames of physical memory: 100" << std::endl << std::endl;

    for (int i = 1; i <=results.size() - 1; i++)
    {
        if (results[i][0] != results[i + 1][0])
            continue;

        auto result1 = results[i];
        auto result2 = results[i + 1];

        if (result1[2] < result2[2])
        {
            std::cout << "Anomily discovered!" << std::endl << "	Sequence number: " << results[i][0]
            << std::endl << "	Page faults: " << results[i][2] << " @ Frame size: " << results[i][1]
            << std::endl << "	Page faults: " << results[i+1][2] << " @ Frame size: " << results[i+1][1] << std::endl;
            numberOfAnomilies++;
        }

    }

    std::cout << std::endl << "Anomily detected : " << numberOfAnomilies << " times." <<  std::endl;
}


std::unordered_map<int, bool> makeHashTable(int size) {
    std::unordered_map<int, bool> hashtable(size);
    for (int i = 0; i<size; i++)
    {
        hashtable[i] = false;
    }

    return hashtable;
}
