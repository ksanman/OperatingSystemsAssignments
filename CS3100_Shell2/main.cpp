
#include <iostream>
#include <chrono>
#include <queue>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <iomanip>
#include <signal.h>

std::deque<std::string> constructQueue(std::string);
std::deque<std::deque<std::string>> parseCommandString(std::deque<std::string>);
int indexLocation(std::deque<std::string>, std::string);
extern "C" void ctrlcHandler(int SignalID);


const int PIPE_COUNT = 2;
const int PIPE_READ_END = 0;
const int PIPE_WRITE_END = 1;
const int STDIN = 0;
const int STDOUT = 1;
int main() {

	//  queue contains the commands to be executed.
	std::deque<std::string> inputQueue;

	// This vector stores a history of all commands used.
	std::deque<std::deque<std::string>> historyQueue;

	// String of commands inputed by the user.
	std::string inputString;
	std::chrono::time_point<std::chrono::high_resolution_clock> time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> totalProcessTime = time - time;

	signal(SIGINT, ctrlcHandler);

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
			inputQueue.pop_front();



			if(std::find(inputQueue.begin(), inputQueue.end(), "|") != inputQueue.end())
			{
				inputQueue.push_front(commandString);

				int pids[PIPE_COUNT];
				pipe(pids);

				int savedStdout = dup(STDOUT);
				int savedStdin = dup(STDIN);
				std::deque<std::deque<std::string>> parsedQueue = parseCommandString(inputQueue);
				std::deque<std::string> pipeBefore = parsedQueue[0];
				std::deque<std::string> pipeAfter = parsedQueue[1];

				pid_t pid = fork();
				if(pid == 0)
				{
					close(pids[PIPE_READ_END]);
					dup2(pids[PIPE_WRITE_END], STDOUT);

					// Set the size of the argv array size. If there is nothing in the queue, it is one. If there are objects in the queue, it is the queue size plus one.
					auto argvSize = (pipeBefore.size() > 0) ? pipeBefore.size() + 1 : 1;

					// Instantiate the argv char array.
					char** argv = new char*[argvSize];

//					// Set the first position of the argv char array to the commandString command.
//					argv[0] = new char[commandString.size()];
//
//					strcpy(argv[0], commandString.c_str());

					//location = indexLocation(inputQueue, "|");

					// for each element in the queue, if the exist, add it to the argv char array.
					for (int i = 0; i < pipeBefore.size(); ++i)
					{
						argv[i] = new char[pipeBefore[i].size()];
						strcpy(argv[i], pipeBefore[i].c_str());

					}

					// Set the last position to null.
					argv[argvSize] = NULL;

					// Call execvp
					execvp(argv[0], argv);
				}

				pid_t pid2 = fork();
				if(pid2 == 0)
				{
					close(pids[PIPE_WRITE_END]);

					dup2(pids[PIPE_READ_END], STDIN);



					// Set the size of the argv array size. If there is nothing in the queue, it is one. If there are objects in the queue, it is the queue size plus one.
					auto argvSize = (pipeAfter.size() > 0) ? pipeAfter.size() + 1 : 1;

					// Instantiate the argv char array.
					char** argv = new char*[argvSize];

//					// Set the first position of the argv char array to the commandString command.
//					argv[0] = new char[commandString.size()];
//
//					strcpy(argv[0], commandString.c_str());

					//location = indexLocation(inputQueue, "|");

					// for each element in the queue, if the exist, add it to the argv char array.
					for (int i = 0; i < pipeAfter.size(); ++i)
					{
						argv[i] = new char[pipeAfter[i].size()];
						strcpy(argv[i], pipeAfter[i].c_str());
					}

					// Set the last position to null.
					argv[argvSize] = NULL;

					// Call execvp
					execvp(argv[0], argv);

				}

				int status;
				// Starts a clock of how long the process has been running.
				std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
				waitpid(pid, &status, 0);
				std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> time = end - start;

				totalProcessTime += time;

				close(pids[PIPE_WRITE_END]);
				close(pids[PIPE_READ_END]);

				// Starts a clock of how long the process has been running.
				std::chrono::time_point<std::chrono::high_resolution_clock> start2 = std::chrono::high_resolution_clock::now();
				waitpid(pid2, &status, 0);
				std::chrono::time_point<std::chrono::high_resolution_clock> end2 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> time2 = end2 - start2;

				totalProcessTime += time2;

				dup2(savedStdout, STDOUT);
				dup2(savedStdin, STDIN);

				inputQueue.clear();
			}
			else if (commandString == "cd")
			{
				std::string newDir = inputQueue.front();
				int rc = chdir(newDir.c_str());

				if(rc < 0)
				{
					std::cout<< "Invalid Directory" << std::endl;

				}

				inputQueue.clear();
			}
			else if (commandString == "exit")
			{
				inputQueue.clear();
				return 0;
			}
			else if (commandString == "ptime")
			{
				std::cout << std::setprecision(4)<<std::fixed<< "Time spent executing processes: " << totalProcessTime.count() << " seconds" << std::endl;
				inputQueue.clear();
			}
			else if (commandString == "history")
			{
				int count = 1;
				for (auto &var : historyQueue)
				{
					std::cout << count << ": ";
					int size = var.size();
					for (int i = 0; i < size; ++i)
					{
						std::cout <<  var[i] << " ";
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
						tmpQueue.pop_front();
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
					inputQueue.pop_front();
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
					// Starts a clock of how long the process has been running.
					std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
					waitpid(pid, &status, 0);
					std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> time = end - start;
					totalProcessTime += time;
				}
				else{
					// Call execvp
					execvp(argv[0], argv);

					std::cout << "Invaild command" << std::endl;
					exit(-1);
				}

				// Clear the queue
				inputQueue.clear();

			}
		}
	}


	return 0;
}

std::deque<std::string> constructQueue(std::string input)
{
	std::deque<std::string> returnedQueue;
	std::string stringToPush;

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

int indexLocation(std::deque<std::string> container, std::string target)
{
	for(int i = 0; i < container.size(); i++)
	{
		if(container[i] == target)
			return i;
	}

	return -1;
}

std::deque<std::deque<std::string>> parseCommandString(std::deque<std::string> queue)
{
	std::deque<std::deque<std::string>> returnQueue;
	std::deque<std::string> queue1;
	std::deque<std::string> queue2;
	int location = 0;

	for(int i = 0; i< queue.size(); i++)
	{
		if(queue[i] == "|") {
			location = i;
			break;
		}

		queue1.push_back(queue[i]);
	}

	for(int i = location + 1; i < queue.size(); i++)
	{
		queue2.push_back(queue[i]);
	}

	returnQueue.push_back(queue1);
	returnQueue.push_back(queue2);
	return returnQueue;



}

extern "C" void ctrlcHandler(int SignalID)
{
	/* do nothing */
}
