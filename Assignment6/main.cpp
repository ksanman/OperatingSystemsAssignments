#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <random>
#include <vector>
#include <unordered_map>
#include <pthread.h>

// Prototypes
std::vector<int> generateSequence(int, int, int);
void initiateThreads(int, std::vector<std::vector<int>>, std::unordered_map<int, std::vector<int>>&);
void detectAnomily(int, int, int, std::vector<int>&, std::vector<int>&, std::unordered_map<int, std::vector<int>>&, std::mutex&);
void printResults(std::unordered_map<int, std::vector<int>>);

int main() {
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
    initiateThreads(maxNumberOfFrames, sequences, results);


    // Each thread will run an instance of the paging test, reporting back paging faults.
    // Print the results
    printResults(results);
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

void initiateThreads(int numberOfFrames, std::vector<std::vector<int>> sequences, std::unordered_map<int, std::vector<int>> &results) {
    std::mutex mutex;
    std::vector<int> stats(3); // index 1 is sequence number, index 2 is the frame number, index 3 is the number of page faults
    int memorySize;
    auto hardware = std::thread();

    int numberOfThreads = hardware.hardware_concurrency();
    int run = 0;

    std::vector<std::thread> threads(numberOfThreads);
    for (int i = 0; i < sequences.size(); i++) {
        memorySize = 0;
        while (memorySize < numberOfFrames) {
            for (auto &workerThread : threads) {
                workerThread = std::thread(&detectAnomily, ++run, ++memorySize, i+1, std::ref(sequences[i]), std::ref(stats), std::ref(results), std::ref(mutex));
            }

            for (auto &workerThread : threads)
            {
                workerThread.join();
            }
        }
    }
}

void detectAnomily(int runNumber, int numberOfFrames, int sequenceNumber, std::vector<int> &sequence, std::vector<int> &stats, std::unordered_map<int, std::vector<int>> &results, std::mutex& mutex) {

    std::unique_lock<std::mutex> lock(mutex);

    std::deque<int> queue(numberOfFrames);
    int pageFault = 0;

    for (auto &request : sequence)
    {
        if (std::find(queue.begin(), queue.end(), request) != queue.end())
        {
            continue;
        }
        else if (queue.size() < numberOfFrames)
        {
            queue.push_back(request);
        }
        else
        {
            pageFault++;
            queue.pop_front();
            queue.push_back(request);
        }
    }

    stats[0] = sequenceNumber;
    stats[1] = numberOfFrames;
    stats[2] = pageFault;
    results[runNumber] = stats;
}

void printResults(std::unordered_map<int, std::vector<int>> results) {
    std::unordered_map<int, std::vector<int>> anomilies(results.size());
    int numberOfAnomilies = 0;

    std::cout << "Length of reference string: 1000" << std::endl << "Frames of physical memory: 100" << std::endl << std::endl;

    for (int i = 1; i <=results.size() - 1; i++)
    {
        auto result1 = results[i];
        auto result2 = results[i + 1];

        if ((result1[2] < result2[2]) && (result1[0] == result2[0]))
        {
            std::cout << "Anomily discovered!" << std::endl << "	Sequence number: " << results[i].at(0)
            << std::endl << "	Page faults: " << results[i].at(2) << " @ Frame size: " << results[i].at(1)
            << std::endl << "	Page faults: " << results[i + 1].at(2) << " @ Frame size: " << results[i + 1].at(1) << std::endl;
            numberOfAnomilies++;
        }
    }

    std::cout << std::endl << "Anomily detected " << numberOfAnomilies << " times." << std::endl;
}