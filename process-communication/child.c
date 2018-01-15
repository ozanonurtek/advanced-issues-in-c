#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int main()
{
	char message[100];
	int i;

	while(1)
	{
		for(i=0;(message[i]=getchar()) != 0; i++)
		{}

		printf("message from parent: %s\n", message);
	}

	system("pause");
	return 0;
}
