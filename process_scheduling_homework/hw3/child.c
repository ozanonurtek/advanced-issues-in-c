/*
Created by: Ozan Onur TEK-210201012
*/

#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Function signatures
void connectToNamedPipe(HANDLE *namedPipe);
void sendMessageToParrent(HANDLE *namedPipe, DWORD  *cbBytes, char sendMessage[]);
//Named pipe, different pipe for each process.
char pipeName[] = "\\\\.\\Pipe\\PROCESS X";
//Messages
char startMessage[] = "PX STARTED";
char endMessage[] = "PX ENDED";

int main(int argc, char* argv[])
{
	HANDLE namedPipe;
	DWORD cbBytes;
	int processC = atoi(argv[1]);
	//Setting pipe name
	pipeName[17] = processC + '0';
	//Getting burst time
	int burstTime = atoi(argv[2]);
	//Connect to named pipe
	connectToNamedPipe(&namedPipe);
	printf_s("\nConnected over named pipe: %s\n", pipeName);
	//Sending start message
	startMessage[1] = processC + '0';
	sendMessageToParrent(&namedPipe, &cbBytes, startMessage);
	//Sleep according to burst time
	Sleep(burstTime);
	//Send end message.
	endMessage[1] = processC + '0';
	sendMessageToParrent(&namedPipe, &cbBytes, endMessage);
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
		printf_s("Can't connect to parent process.");
		system("pause");
		system("exit");
	}
	else
	{
		printf("\nConnection succeed!\n");
	}
}
void sendMessageToParrent(HANDLE *namedPipe, DWORD  *cbBytes, char sendMessage[]) {


	//Send the message to parrent
	BOOL bResult = WriteFile(
		*namedPipe,
		sendMessage,
		strlen(sendMessage) + 1,
		cbBytes,
		NULL);
	//Error handling
	if (!bResult)
	{
		printf_s("\nError occured while sending message to parrent, code= %d", GetLastError());
		CloseHandle(*namedPipe);
		system("pause");
		system("exit");
	}
	else
	{
		printf_s("\n Message sent to parrent! \n");
	}

}