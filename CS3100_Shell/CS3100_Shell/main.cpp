
#include <iostream>
#include <chrono>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <exception>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <iomanip>

std::vector<std::string> constructQueue(std::string);

int main() {

	//  queue contains the commands to be executed. 
	std::vector<std::string> inputQueue;

	// This vector stores a history of all commands used. 
	std::vector<std::vector<std::string>> historyQueue;

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
			std::string commandString = inputQueue.front();
			historyQueue.push_back(inputQueue);
			inputQueue.pop_back();

			if (commandString == "exit")
			{
				inputQueue.clear();
				return 0;
			}
			else if (commandString == "ptime")
			{
				std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> time = end - start;
				std::cout << std::setprecision(4)<< "Time spent executing processes: " << time.count() << " seconds" << std::endl;
				inputQueue.clear();
			}
			else if (commandString == "history")
			{
				int count = 1;
				for (auto &var : historyQueue)
				{
					int size = var.size();
					for (int i = 0; i < size; ++i)
					{
						std::cout << count  << ":  " <<  var[i] << " ";
					}
					std::cout << std::endl;
					count++;
			
				}
				inputQueue.clear();
			}
			else if (commandString.find('^') != std::string::npos)
			{
				std::string index = "";
				std::string numbers;
				int findIndex = 0;
				for (int i = 1; i <=inputQueue.size(); ++i)
				{
					numbers = inputQueue[i];
					index += numbers;

				}
				if(index < "0")
				{
					std::cout << "Invalid input" << std::endl;
					inputQueue.clear();
					continue;
				}

				findIndex = std::stoi(index);

				if(findIndex >= historyQueue.size())
				{
					std::cout << "History index out of range" << std::endl;
					inputQueue.clear();
				}
				else
				{
					inputQueue.clear();
					auto tmpQueue = historyQueue[findIndex - 1];

					for(int i = 0; i < tmpQueue.size(); ++i)
					{
						inputQueue.push_back(tmpQueue.front());
						tmpQueue.pop_back();
					}
				}
			}
			else
			{
				/* Attempt to execute a command. If we fail, display Invaild Input */

				// Set the size of the argv array size. If there is nothing in the queue, it is one. If there are objects in the queue, it is the queue size plus one. 
				auto argvSize = (inputQueue.size() > 0) ? inputQueue.size() + 1 : 1;

				// Instantiate the argv char array.
				char** argv = new char*[argvSize + 1];

				// Set the first position of the argv char array to the commandString command.
				argv[0] = new char[commandString.size()];

				strcpy(argv[0], commandString.c_str());



				// for each element in the queue, if the exist, add it to the argv char array. 
				for (int i = 1; i < argvSize; ++i)
				{
					argv[i] = new char[inputQueue.front().size()];
					strcpy(argv[i], inputQueue.front().c_str());
					inputQueue.pop_back();
				}

				// Set the last position to null. 
				argv[argvSize] = NULL;

				// Fork the process
				int pid = fork();
				if(pid == -1)
				{
					/* error forking */
					// If the fork fails, log to the console and return fail.
					std::cout << "Fork failed." << std::endl;
				}
				else if(pid > 0)
				{
					int status;
					waitpid(pid, &status, 0);
				}
				else{
					// Call execvp
					execvp(argv[0], argv);
					std::cout << "Invaild command" << std::endl;
					exit(-1);
					// The command failed. 
					std::cout<< "Invalid or unrecognized command" << std::endl;

				}
				// Clear the queue
				inputQueue.clear();

			}
		}
	}
	

	return 0;
}

std::vector<std::string> constructQueue(std::string input)
{
	std::vector<std::string> returnedQueue;
	std::string stringToPush;
	bool isEndOfFile = false;

	for (int i = 0; i < input.size(); ++i)
	{
		if (input[i] == ' ')
		{
			returnedQueue.push_back(stringToPush);
			stringToPush.clear();
		}
		else
			stringToPush += input[i];
	}
	
	returnedQueue.push_back(stringToPush);

	return returnedQueue;
}

