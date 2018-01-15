#include <Windows.h>
#include <stdio.h>

#define MAX_THREADS 20
#define MAX_READERS 2
#define MAX_WRITERS MAX_THREADS-MAX_READERS

void read(int id)
{
	printf("READER %3d: Reading...\n", id);
	Sleep(500);
}

void write(int id)
{
	printf("\tWRITER %3d:  Writing...\n", id);
	Sleep(1000);
}

　
DWORD WINAPI reader(int id);
DWORD WINAPI writer(int id);
//filesem -> controll access to data
//count the number of readers -> countSem 
HANDLE fileSem, countSem;

int readerCount = 0;

void main()
{

	HANDLE threads[MAX_THREADS];

	//allocate memory for every parameters needed

	/*
	SECURITY_ATTRIBUTES sa;
	SecureZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	*/

	//Mutex
	countSem = CreateSemaphore(NULL, 1, 1, "countSemaphore");
	if (!countSem){
		printf_s("Error creating semaphore %d", GetLastError());
		exit(0);
	}
	//Create semaphores
	fileSem = CreateSemaphore(NULL, 1, 1, "fileSemaphore");
	if (!fileSem){
		printf_s("Error creating semaphore %s", GetLastError());
		system("pause");
		exit(0);
	}

	//--------------------------------------
	//for each thread
	
	for (int i = 0; i < MAX_READERS; i++)
	{
		//initialize parameters
		threads[i] = CreateThread(NULL, 0, reader, i, 0, NULL);
		if (!threads[i]){
			printf_s("Error code: %d", GetLastError());
		}

		//check errors in creation
		if (threads[i] == INVALID_HANDLE_VALUE)
		{
			printf("error when creating thread\n");
			system("pause");
			exit(0);
		}

　
	}

	for (int i = MAX_READERS; i < MAX_THREADS; i++)
	{
		//initialize parameters

		threads[i] = CreateThread(NULL, 0, writer, i, 0, NULL);
		if (!threads[i]){
			printf_s("Error code: %d", GetLastError());
		}

		//check errors in creation
		if (threads[i] == INVALID_HANDLE_VALUE)
		{
			printf("error when creating thread\n");
			system("pause");
			exit(0);
		}

　
	}
	//Create reader and writer threads

	//-------------------------------------

　
	WaitForMultipleObjects(MAX_THREADS, threads, TRUE, INFINITE);
	CloseHandle(fileSem);
	CloseHandle(countSem);

	system("pause");
	return 1;
}

　
DWORD WINAPI  reader(int id)
{
	while (1){
		// Solve the critical section problem for readers
		WaitForSingleObject(countSem, INFINITE);
		readerCount++;
		ReleaseSemaphore(countSem, 1, NULL);
		if (readerCount == 1){
			WaitForSingleObject(fileSem, INFINITE);
		}
		read(id);
		readerCount--;
		if (readerCount == 0){
			ReleaseSemaphore(fileSem, 1, NULL);
		}
		WaitForSingleObject(countSem, INFINITE);
		ReleaseSemaphore(countSem, 1, NULL);
	}

}

　
DWORD WINAPI writer(int id)
{
	while (1){
		WaitForSingleObject(fileSem, INFINITE);
		write(id);
		ReleaseSemaphore(fileSem, 1, NULL);
	}
	
}
