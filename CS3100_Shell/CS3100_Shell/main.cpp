#include <iostream>
#include <chrono>
#include <queue>
#include <string>
#include <exception>
#include <vector>

std::queue<std::string> constructQueue(std::string);
void clearQueue(std::queue<std::string>&);

int main() {

	//  queue contains the commands to be executed. 
	std::queue<std::string> inputQueue;

	// This vector stores a history of all commands used. 
	std::vector<std::queue<std::string>> historyQueue;

	// String of commands inputed by the user. 
	std::string inputString;

	// Starts a clock of how long the process has been running.
	std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
	
	// Keep executing until exit() is called. 
	while (true)
	{
		// output the input sign
		std::cout << "[cmd]: ";
		std::getline(std::cin, inputString);

		// Construct a queue of the commands the user submitted. 
		inputQueue = constructQueue(inputString);
		
		while (!inputQueue.empty())
		{
			std::string workingString = inputQueue.front();
			historyQueue.push_back(inputQueue);
			inputQueue.pop();

			if (workingString == "exit")
			{
				clearQueue(inputQueue);
				return 0;
			}
			else if (workingString == "ptime")
			{
				std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> time = end - start;
				std::cout << "Time spent executing processes: " << time.count() << " seconds" << std::endl;
				clearQueue(inputQueue);
			}
			else if (workingString == "history")
			{
				for each (auto var in historyQueue)
				{
					int size = var.size();
					for (int i = 0; i < size; ++i)
					{
						std::cout << var.front() << " ";
						var.pop();
					}
				}
				clearQueue(inputQueue);
			}
			else if (workingString.find('^') != std::string::npos)
			{
				std::string index = "";
				int findIndex = 0;
				for (int i = 1; i < workingString.size(); ++i)
				{
					index += workingString[i];
				}

				findIndex = std::stoi(index);

				inputQueue = historyQueue[findIndex - 1];
			}
			else
			{
				/* Attempt to execute a command. If we fail, display Invaild Input */


				clearQueue(inputQueue);
				std::cout << "Invalid Input" << std::endl;
			}
		}
	}
	

	return 0;
}

std::queue<std::string> constructQueue(std::string input)
{
	std::queue<std::string> returnedQueue;
	std::string stringToPush;
	bool isEndOfFile = false;

	for (int i = 0; i < input.size(); ++i)
	{
		if (input[i] == ' ')
		{
			returnedQueue.push(stringToPush);
			stringToPush.clear();
		}
		else
			stringToPush += input[i];
	}
	
	returnedQueue.push(stringToPush);

	return returnedQueue;
}

void clearQueue(std::queue<std::string>& queue)
{
	while (!queue.empty())
		queue.pop();
}