/*
Created by: Ozan Onur TEK-210201012
*/

#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Named pipe, different pipe for each day.
char pipeName[] = "\\\\.\\Pipe\\DAY X";
//PLEASE CHANGE THE FILE PATH WITH YOUR Market.txt
#define FILE_NAME "c:\\users\\asus\\source\\repos\\IPC-namedPipe\\Market.txt"
//THREAD COUNT
#define THREAD_COUNT 4
char dayInfo[5];
char sendMessage[256];

DWORD WINAPI threadExecution(LPVOID parameters);
void readInfoFromParrent(HANDLE *namedPipe, DWORD  *cbBytes);
void connectToNamedPipe(HANDLE *namedPipe);
void sendMessageToParrent(HANDLE *namedPipe, DWORD  *cbBytes);
void createThreads();

typedef struct
{
	char *trackWhat;
	char *dayInfo;
	int countOfTrack;
}THREAD_PARAMETERS;

int main(int argc, char* argv[])
{
	HANDLE hPipe;
	DWORD cbBytes;
	int dayC = atoi(argv[1]);
	pipeName[13] = dayC + '0';
	printf_s("\nConnected over named pipe: %s\n", pipeName);
	connectToNamedPipe(&hPipe);
	readInfoFromParrent(&hPipe, &cbBytes);
	createThreads();
	sendMessageToParrent(&hPipe, &cbBytes);

	system("pause");

	return 1;
}
void connectToNamedPipe(HANDLE *namedPipe) {
	*namedPipe = CreateFile(
		pipeName,   
		GENERIC_READ |  
		GENERIC_WRITE,
		0,              
		NULL,        
		OPEN_EXISTING,  
		0,               
		NULL);         

	if (INVALID_HANDLE_VALUE == *namedPipe)
	{
		printf("\nError occurred while connecting"
			" to the server: %d", GetLastError());
		exit(0);
	}
	else
	{
		printf("\nConnection succeed!\n");
	}
}
void readInfoFromParrent(HANDLE *namedPipe, DWORD  *cbBytes) {

	//Reading message from parrent!
	BOOL bResult = ReadFile(
		*namedPipe,                // handle to pipe 
		dayInfo,             // buffer to receive data 
		sizeof(dayInfo),     // size of buffer 
		&cbBytes,             // number of bytes read 
		NULL);                // not overlapped I/O 

	if ((!bResult) || (0 == cbBytes))
	{
		printf("\nError occurred while reading %d", GetLastError());
		CloseHandle(*namedPipe);
		exit(0);
	}
	else
	{
		//Read succeed
		printf("My message is: %s\n", dayInfo);
	}
}
void sendMessageToParrent(HANDLE *namedPipe, DWORD  *cbBytes) {
	
	
	//Send the message to parrent
	BOOL bResult = WriteFile(
		*namedPipe,               
		sendMessage,             
		strlen(sendMessage) + 1,  
		cbBytes,          
		NULL);              

	if (!bResult)
	{
		printf_s("\nError occured, code= %d", GetLastError());
		CloseHandle(*namedPipe);
		exit(0);
	}
	else
	{
		printf_s("\n Message sent to parrent! \n");
	}

}
void createThreads() {
	HANDLE* handles;
	THREAD_PARAMETERS* threadParameters;
	int* id;

	//Allocate memory
	handles = malloc(sizeof(HANDLE)* THREAD_COUNT);
	threadParameters = malloc(sizeof(THREAD_PARAMETERS)* THREAD_COUNT);
	id = malloc(sizeof(int)* THREAD_COUNT);

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		threadParameters[i].countOfTrack = 0;

		switch (i) {
		case 0:
			threadParameters[i].trackWhat = "MILK";
			break;
		case 1:
			threadParameters[i].trackWhat = "BISCUIT";
			break;
		case 2:
			threadParameters[i].trackWhat = "CHIPS";
			break;
		case 3:
			threadParameters[i].trackWhat = "COKE";
			break;
	}
		handles[i] = CreateThread(NULL, 0, threadExecution, &threadParameters[i], 0, &id[i]);

		//Check for errors.
		if (handles[i] == INVALID_HANDLE_VALUE)
		{
			printf_s("Error in thread creation\n");
			system("pause");
			exit(0);
		}

	}

	WaitForMultipleObjects(THREAD_COUNT, handles, TRUE, INFINITE);
	//Formatting string for easy data manipulation in parent process...
	sprintf_s(sendMessage, sizeof(sendMessage), "%s-%s=%d-%s=%d-%s=%d-%s=%d \n", dayInfo, threadParameters[0].trackWhat, threadParameters[0].countOfTrack, threadParameters[1].trackWhat, threadParameters[1].countOfTrack, threadParameters[2].trackWhat, threadParameters[2].countOfTrack, threadParameters[3].trackWhat, threadParameters[3].countOfTrack);


}
//Thread execution, please read the note inside the method.
DWORD WINAPI threadExecution(LPVOID parameters)
{
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	FILE *file;
	errno_t errorCode = fopen_s(&file, FILE_NAME, "r");
	int count = 0;
	if (errorCode == 0) {
		char line[300];

		while (fgets(line, sizeof(line), file)) {
			if (strstr(line, dayInfo) != NULL) {
				count++;
			}
			if (count != 0) {
				if (strstr(line, param->trackWhat) != NULL) {
					//Dear instructor:
					//If delactosed milk not kind of milk and diet coke is not coke, please comment out these lines of code.
					/*
					if (param->trackWhat == "MILK") {
						if (!((strstr(line, "DELACTOSED") != NULL) || (strstr(line, "CHOCOLATE") != NULL)))
							param->countOfTrack++;
					}
					else if (param->trackWhat == "COKE") {
						if (!(strstr(line, "DIET") != NULL))
							param->countOfTrack++;
					}
					else {
						param->countOfTrack++;
					}
					*/
					//And dont forget the delete following line:
					param->countOfTrack++;

				}
				if (count == 2)
					break;
			}
		
		}
		
	}
	else {
		printf_s("Error %d", errno);
	}
	return 1;
}
