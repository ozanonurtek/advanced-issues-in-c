/*
Created by: Ozan Onur TEK-210201012
*/

#include <stdio.h>
#include <stdlib.h>
#include "windows.h"

//Name given to the pipe
char pipeName[] = "\\\\.\\Pipe\\DAY X";
#define SIZE_OF_MESSAGE 256 

#define NUMBER_OF_CHILD_PROCESSES  7

//Function signatures
void createProcess(STARTUPINFO *si, PROCESS_INFORMATION *pi, int i);
void createNamedPipe(HANDLE *namedPipe);
void createSecurityAttributes(STARTUPINFO *si, SECURITY_ATTRIBUTES *sa, PROCESS_INFORMATION *pi);
BOOL waitChildToConnect(HANDLE *namedPipe);
void readChildMessage(HANDLE *namedPipe, char *message, DWORD *bytesRead, int sizeOfMessage);
void sendMessageToChild(HANDLE *namedPipe, char *message, DWORD *bytesWritten, int sizeOfMessage);
void totalDay(char message[], char dayInfo[]);
void totalWeek();

//Global variables to track counts of items.
int totalCoke = 0, totalBiscuit = 0, totalChips = 0, totalMilk;

int main(int argc, char* argv[])
{
	//Creating startup information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	char dayInfo[] = "DAY X";
	char message[SIZE_OF_MESSAGE];

	HANDLE pipes[NUMBER_OF_CHILD_PROCESSES];
	//Creating security attributes
	for (int i = 1; i < NUMBER_OF_CHILD_PROCESSES + 1; i++) {

		HANDLE *pipe;
		DWORD bytesRead, bytesWritten;
		createSecurityAttributes(&si, &sa, &pi);
		createProcess(&si, &pi, i);
		pipeName[13] = i + '0';
		createNamedPipe(&pipe);
		
	if (!waitChildToConnect(&pipe)) {
		printf_s("Connection timed out.");
		system("pause");
		system("exit");
	}

	//Create generic day string
	dayInfo[4] = i + '0';

	//Send message to child about its day information
	sendMessageToChild(&pipe, dayInfo, &bytesWritten, sizeof(dayInfo) + 1);
	//Get messages from childs
	readChildMessage(&pipe, &message, &bytesRead, sizeof(message));
	printf_s("-----------------------------------------\n");
	printf_s("Message From Child: %s\n", message);
	printf_s("-----------------------------------------\n");
	totalDay(message, dayInfo);
	
	pipes[i - 1] = pipe;

}
	//Close pipes
	for (int i = 0; i < NUMBER_OF_CHILD_PROCESSES; i++)
	{
		if (!CloseHandle(pipes[i])) {
			printf_s("Error while closing handles");
		}
	}

	//Do total week calculations 
	totalWeek();
	system("pause");
	return 0; //Success
}

void createSecurityAttributes(STARTUPINFO *si, SECURITY_ATTRIBUTES *sa, PROCESS_INFORMATION *pi) {
	SecureZeroMemory(sa, sizeof(SECURITY_ATTRIBUTES));
	(*sa).bInheritHandle = TRUE;
	(*sa).lpSecurityDescriptor = NULL;
	(*sa).nLength = sizeof(SECURITY_ATTRIBUTES);
	SecureZeroMemory(si, sizeof(STARTUPINFO));
	SecureZeroMemory(pi, sizeof(PROCESS_INFORMATION));
	(*si).cb = sizeof(*si);
}

void createProcess(STARTUPINFO *si, PROCESS_INFORMATION *pi, int i)
{
	char childName[] = "child.exe X";
	childName[10] = i + '0';
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
void sendMessageToChild(HANDLE *namedPipe, char *message, DWORD *bytesWritten, int sizeOfMessage) {
	//Send message to child
	if (!WriteFile(
		*namedPipe,               
		message,           
		sizeOfMessage,   
		bytesWritten,           
		NULL)) {
		printf_s("Error");
		system("pause");
		system("exit");
	}

}

//Basic jobs about calculations 
void totalDay(char message[], char dayInfo[]) {
	int milk = message[11] - '0';
	int biscuit = message[21] - '0';
	int chips = message[29] - '0';
	int coke = message[36] - '0';
	printf_s("-----------------------------------------\n");

	printf_s("In %s :\n", dayInfo);
	printf_s("TOTAL COKE: %d\n", coke);
	printf_s("TOTAL MILK: %d\n", milk);
	printf_s("TOTAL CHIPS: %d\n", chips);
	printf_s("TOTAL BISCUIT %d\n", biscuit);
	printf_s("TOTAL IN THIS DAY: %d\n", coke + chips + biscuit + milk);
	totalCoke = totalCoke + coke;
	totalBiscuit = totalBiscuit + biscuit;
	totalChips = totalChips + chips;
	totalMilk = totalMilk + milk;

	printf_s("-----------------------------------------\n");
	int counts[4] = { biscuit, coke, chips, milk };
	char *names[4] = { "BISCUIT", "COKE", "CHIPS", "MILK" };
	int max = counts[3];
	int whereIsMax = 0;
	for (int i = 0; i < 3; i++)
	{
		if (counts[i] > max) {
			max = counts[i];
			whereIsMax = i;
		}
			
	}
	printf_s("The most sold item in %s is %s :\n", dayInfo, names[whereIsMax]);
	printf_s("-----------------------------------------\n");

}
void totalWeek() {
	printf_s("-----------------------------------------\n");

	printf_s("In this week total sell: \n");
	printf_s("TOTAL COKE: %d\n", totalCoke);
	printf_s("TOTAL MILK: %d\n", totalMilk);
	printf_s("TOTAL CHIPS: %d\n", totalChips);
	printf_s("TOTAL BISCUIT %d\n", totalBiscuit);
	printf_s("TOTAL: %d\n", totalBiscuit + totalChips + totalCoke + totalMilk);
	printf_s("-----------------------------------------\n");

	int counts[4] = { totalBiscuit,  totalCoke,  totalChips,  totalMilk };
	char *names[4] = { "BISCUIT", "COKE", "CHIPS", "MILK" };
	int max = counts[3];
	int whereIsMax = 0;
	for (int i = 0; i < 3; i++)
	{
		if (counts[i] > max) {
			max = counts[i];
			whereIsMax = i;
		}

	}
	printf_s("The most sold item in this week is %s :\n", names[whereIsMax]);
	printf_s("-----------------------------------------\n");
}