/*
Created by: Ozan Onur TEK-210201012
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "windows.h"

#define NUMBER_OF_CHILD_PROCESSES 5
#define TIMES 5
#define SIZE_OF_MESSAGE 256 

//Function signatures
int comparator(const void * firstProcess, const void * secondProcess);
void initializeBurstTimesForProcesses();
void createNamedPipe(HANDLE *namedPipe);
void createSecurityAttributes(STARTUPINFO *si, SECURITY_ATTRIBUTES *sa, PROCESS_INFORMATION *pi);
BOOL waitChildToConnect(HANDLE *namedPipe);
void createProcess(STARTUPINFO *si, PROCESS_INFORMATION *pi, int i, int burstTime);
void readChildMessage(HANDLE *namedPipe, char *message, DWORD *bytesRead, int sizeOfMessage);

//Process information is stored as struct for each process
struct process {
	int id;
	float burstTime;
};
//Gloabal variables
struct process * processes;
char pipeName[] = "\\\\.\\Pipe\\PROCESS X";

int main()
{
	//Declare necessity parameters for child process creation
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	//Initialize message which will come from child process.
	char message[SIZE_OF_MESSAGE];

	//We should implement it like this to see different random numbers
	srand(time((NULL)));
	int randomActualLength = 0;

	//Initialize burst times of processes
	initializeBurstTimesForProcesses();

	//It will execute 5 time each child process.
	for (int i = 0; i < TIMES; i++) {
		printf_s("--- %d. EXECUTION IN ORDER---\n", i + 1);
		for (int j = 0; j < NUMBER_OF_CHILD_PROCESSES; j++) {
			//Declare paramaters for named pipe connection.
				HANDLE *pipe;
				DWORD bytesRead, bytesWritten;
			//Creating security attributes and process
				createSecurityAttributes(&si, &sa, &pi);
				createProcess(&si, &pi, processes[j].id , processes[j].burstTime);
			//Setting pipe name
				pipeName[17] = processes[j].id + '0';
			//Creating named pipe
				createNamedPipe(&pipe);
			//Waiting child for connection
				if (!waitChildToConnect(&pipe)) {
					printf_s("Connection timed out.");
					system("pause");
					system("exit");
				}

				//Get messages from childs
				readChildMessage(&pipe, &message, &bytesRead, sizeof(message));
				printf_s("%s\n", message);
			
			//Since we get the message successfully, print the information.
			printf_s("Process id: %d, with burst time: %f ms, executing.\n", processes[j].id, processes[j].burstTime);

			//Read end message from child
			readChildMessage(&pipe, &message, &bytesRead, sizeof(message));
			printf_s("%s\n", message);
			
			//Generate random actual length where randomActualLength: [50 - 300] 
			randomActualLength =  (rand() % 251) + 50.0;
			//Calculating next burst time using current burst time and random actual length.
			//(ALPHA * currentBurstTime) + ((1-ALPHA) * randomActualLength))
			float nextBurstTime = (0.5 * processes[j].burstTime) + (0.5 * (float)randomActualLength);
			//Override process' burst time for next iteration
			processes[j].burstTime = nextBurstTime;
			
			printf_s("Random actual length generated as: %d\n", randomActualLength);
			printf_s("Next burst time calculated as : %f ms\n", processes[j].burstTime);
			printf_s("-------\n");
			//Closing pipes.
			if (!CloseHandle(pipe)) {
				printf_s("Error while closing handles");
				system("pause");
				system("exit");
			}
		}
		//Sort processes in each iteration.
		qsort((void *)processes, NUMBER_OF_CHILD_PROCESSES, sizeof(struct process), comparator);
	}

	system("Pause");
	return 1;
}

void initializeBurstTimesForProcesses() {
	int initialBurstTime[] = { 300, 220, 180, 45 ,255 };
	//Allocating memory for processes array
	processes = malloc(sizeof(struct process) * NUMBER_OF_CHILD_PROCESSES);
	for (int i = 0; i < NUMBER_OF_CHILD_PROCESSES; i++)
	{
		processes[i].id = i + 1;
		processes[i].burstTime = initialBurstTime[i];
	}
	//Sort processes for first iteration
	qsort((void *)processes, NUMBER_OF_CHILD_PROCESSES, sizeof(struct process), comparator);
}
//Reading message from child
void readChildMessage(HANDLE *namedPipe, char *message, DWORD *bytesRead, int sizeOfMessage) {
	if (!ReadFile(
		*namedPipe,
		message,
		sizeOfMessage,
		bytesRead,
		NULL
	) || (*bytesRead) == 0) {
		CloseHandle(*namedPipe);
		printf_s("Error");
		system("pause");
		system("exit");
	}

}
//Security attributes
void createSecurityAttributes(STARTUPINFO *si, SECURITY_ATTRIBUTES *sa, PROCESS_INFORMATION *pi) {
	SecureZeroMemory(sa, sizeof(SECURITY_ATTRIBUTES));
	(*sa).bInheritHandle = TRUE;
	(*sa).lpSecurityDescriptor = NULL;
	(*sa).nLength = sizeof(SECURITY_ATTRIBUTES);
	SecureZeroMemory(si, sizeof(STARTUPINFO));
	SecureZeroMemory(pi, sizeof(PROCESS_INFORMATION));
	(*si).cb = sizeof(*si);
}
void createProcess(STARTUPINFO *si, PROCESS_INFORMATION *pi, int i, int burstTime)
{
	//Burst time and process id are passed with argument to child process.
	char childName[25];
	sprintf_s(childName,sizeof(childName), "child.exe %d %d", i, burstTime);

	if (!CreateProcess(NULL,
		childName,
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		si,
		pi))
	{
		printf("unable to create process\n");
		system("pause");
		exit(0);
	}
}
void createNamedPipe(HANDLE *namedPipe) {
	//Creating named pipe.
	*namedPipe = CreateNamedPipe(
		pipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE |
		PIPE_READMODE_MESSAGE |
		PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		SIZE_OF_MESSAGE,
		SIZE_OF_MESSAGE,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	//Error handling
	if (INVALID_HANDLE_VALUE == *namedPipe)
	{
		printf_s("Pipe creating error, code = %d", GetLastError());
		system("pause");
		system("exit");
	}
}
BOOL waitChildToConnect(HANDLE *namedPipe) {
	//Wait for the child 
	BOOL isChildConnected = ConnectNamedPipe(*namedPipe, NULL);
	return isChildConnected;
}
//Comparator for qsort function
int comparator(const void * firstProcess, const void * secondProcess)
{
	int firstBurstTime = ((struct process *)firstProcess)->burstTime;
	int secondBurstTimer = ((struct process *)secondProcess)->burstTime;
	return (firstBurstTime - secondBurstTimer);
}

