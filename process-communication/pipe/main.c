#include <Windows.h>
#include <stdio.h>



int main()
{
	char message[100];

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;

	DWORD bytesToWrite, bytesWritten; //A DWORD is a 32-bit unsigned integer

	HANDLE hWritePipe, hReadPipe;

	//creating security attributes
	SecureZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);

	//Create your pipe here


	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
		system("exit");
	
	printf("Pipe is created");
	

	SecureZeroMemory(&si, sizeof(STARTUPINFO));
	SecureZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(si);
	
	//Initialize your Pipe handles here

	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdInput = hReadPipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	if(!CreateProcess(NULL,
					  "child.exe",
					  NULL,
					  NULL,
					  TRUE,
					  CREATE_NEW_CONSOLE,
					  NULL,
					  NULL,
					  &si,
					  &pi))
	{
		printf("unable to create process\n");
		system("pause");
		exit(1);
	}
	

	while(1)
	{
		gets_s(message, 99);
		bytesToWrite = strlen(message);
		bytesToWrite++;
		WriteFile(hWritePipe, message, bytesToWrite, &bytesWritten, NULL);
		//Send your message here
		
		printf("numberof bytes: %d\n number of bytes succesfully sent: %d\n", bytesToWrite, bytesWritten);
	}


	system("pause");
	return 0;
}
