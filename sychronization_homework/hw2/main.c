//main.c
//Created by: Ozan Onur Tek - 210201012

#include "header.h"

//In this solution there is no sequences between threads. Thread Type C and B are not destroyed before checking final version of output file.
   
int main(void)
{
	//Initializing parameters
    int error;
	THREAD_PARAMETERS* lpParameter;
	//Memory allocation for thread parameters
	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* NUMBER_OF_THREADS);
	//Execute initializeMutex function
 	if(initializeMutex() == 1){
		printf("Error initializing mutex, aborting processes...");
 		return 1;
 	}
   	//Creating threads
	int i = 0;
    while(i < 6)
    {
		switch(i){
				case 0:
					lpParameter[i].TYPE_OF_THREAD = 'B';
					lpParameter[i].threadID = i;
					error = pthread_create(&(tid[i]), NULL, &cleanPrimesFromResourceType1, &lpParameter[i]);//Thread type B
					if(error!= 0){
		        		printf("\nThread can't be created :[%s]", strerror(error));	
		        		return 1;
			}
				break;
				case 1:
					lpParameter[i].TYPE_OF_THREAD = 'C';
					lpParameter[i].threadID = i;
					error = pthread_create(&(tid[i]), NULL, &cleanNegativesFromResourceType1, &lpParameter[i]);//Thread type C
					if(error!= 0){
		        		printf("\nThread can't be created :[%s]", strerror(error));	
		        		return 1;	
					}
				break;
				default:
					lpParameter[i].TYPE_OF_THREAD = 'A';
					lpParameter[i].threadID = i;
		        	error = pthread_create(&(tid[i]), NULL, &readFromResource1AndWriteToResource2, &lpParameter[i]);
		        	if (error != 0){
		          	  printf("\ncan't create thread :[%s]", strerror(error));
		        		return 1;
		        	}
			}
		
        i++;
    }
	//Joining threads
	i = 0;
    while(i < 6){
	    pthread_join(tid[i], NULL);
		i++;
    }
	
	//Deallocating threads
    pthread_mutex_destroy(&RESOURCE_TYPE_1_1);
  	pthread_mutex_destroy(&RESOURCE_TYPE_1_2);
  	pthread_mutex_destroy(&RESOURCE_TYPE_2);
	
	//Checking final file if there is negative or prime number inside it( Self-controlled hw :D )
	checkFinalCondition();
	
	//Delete all content of resource files.
	fclose(fopen(RESOURCE_TYPE_1_1_FILE_NAME, "w"));
	fclose(fopen(RESOURCE_TYPE_1_2_FILE_NAME, "w"));
	
	printf("\nfinalNumbers.txt is generated under same directory!\n");
    return 0;
}
//Pragma Mark-: Initialize Mutex

int initializeMutex(){
    if (pthread_mutex_init(&RESOURCE_TYPE_1_1, NULL) != 0)
    {
        printf("\n Can not initialize mutex for resource type 1 file 1");
        return 1;
    }
	
    if (pthread_mutex_init(&RESOURCE_TYPE_1_2, NULL) != 0)
    {
        printf("\n Can not initialize mutex for resource type 1 file 2");
        return 1;
    }
	
    if (pthread_mutex_init(&RESOURCE_TYPE_2, NULL) != 0)
    {
        printf("\n Can not initialize mutex for resource type 1 file 2");
        return 1;
    }
	return 0;
}
//Pragma Mark-: Initialize Threads With Paramaters
//Pragma Mark-: Helpers

//Just test purpose that final file does not contains any negative or prime number.
void checkFinalCondition(){
	FILE* tempFile = fopen(RESOURCE_TYPE_2_FILE_NAME, "r");
	char line [10];
	short primeChecker = 0;
	short negativeChecker = 0;
	if( tempFile != NULL )
	{
		while( fgets(line, sizeof(line), tempFile) != NULL )
		{
			int number;
			sscanf (line, "%d", &number);
			if(isPrime(number) == 1){
				printf("Prime number detected in final file: %d \n", number);
				primeChecker = 1;
			}
			if(number < 0){
				printf("Negative number detected in final file: %d \n", number);
				negativeChecker = 1;
			}
			
		}
	}
	if(negativeChecker == 0){
		printf("No negative number detected in final file.\n");
	}
	if(primeChecker == 0){
		printf("No prime number detected: detected in final file. \n");
	}
	
	fclose(tempFile);
}

//Helper method that check if given number is prime
int isPrime(int number)
{
     if (number % 2 == 0 && number > 2){
     	return 0;
     } 
     if (number <= 1) {
     	return 0;
     }
     for(int counter = 3; counter < number / 2; counter+= 2)
     {
         if (number % counter == 0){
             return 0;
         }
     }
     return 1;
}

//Pragma Mark-:Thread runners

void *cleanPrimesFromResourceType1(void *parameters){
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	short checkingDone = 0;
	while(primesCleaned_File_TYPE_1_1 == 0 || primesCleaned_File_TYPE_1_2 == 0 || copyPasteFinished_File_TYPE_1_1 == 0 || copyPasteFinished_File_TYPE_1_2 == 0 || checkingDone == 0){
		//Check if resource is locked
		if(pthread_mutex_lock(&RESOURCE_TYPE_1_1) == 0 && primesCleaned_File_TYPE_1_1 == 0){
			//Inform
		    printf("Thread-> Type: %c Id: %d, cleaning primes from file: numbers1.txt\n", param->TYPE_OF_THREAD, param->threadID);
			//Do cleaning job
			//Open file
			FILE* inFile = fopen(RESOURCE_TYPE_1_1_FILE_NAME, "r");
			FILE* outFile = fopen(CLEANED_RESOURCE_TYPE_1_1_FILE_NAME, "a");
			char line [10];
			if( inFile != NULL )
			{
				while( fgets(line, sizeof(line), inFile) != NULL )
				{
					int number;
					sscanf (line, "%d", &number);
						if(isPrime(number) == 0){
					        fprintf(outFile, "%s", line);
						}
	
				}	
			}
			//Close file operation before unlocking the mutex.
			fclose(inFile);
			fclose(outFile);
			int ret = remove(RESOURCE_TYPE_1_1_FILE_NAME);
			if(ret == 0){
				rename(CLEANED_RESOURCE_TYPE_1_1_FILE_NAME, RESOURCE_TYPE_1_1_FILE_NAME);
			}
		    printf("Thread-> Type: %c Id: %d, cleaning primes operation is finished for file: numbers1.txt\n", param->TYPE_OF_THREAD, param->threadID);
			// Unlock resource so there will be no hold and wait situation and starvation.
			pthread_mutex_unlock(&RESOURCE_TYPE_1_1);
			// Boolean like checker
			primesCleaned_File_TYPE_1_1 = 1;
		}
		//Check that mutex is locked by another thread.
		if(pthread_mutex_lock(&RESOURCE_TYPE_1_2) == 0 && primesCleaned_File_TYPE_1_2 == 0){
		    printf("Thread-> Type: %c Id: %d, cleaning primes from file: numbers2.txt\n", param->TYPE_OF_THREAD, param->threadID);
				//Do cleaning job
				FILE* inFile2 = fopen(RESOURCE_TYPE_1_2_FILE_NAME, "r");
				FILE* outFile2 = fopen(CLEANED_RESOURCE_TYPE_1_2_FILE_NAME, "a");
				char line [10];
				if( inFile2 != NULL )
				{
				
					while( fgets(line, sizeof(line), inFile2) != NULL )
					{
						int number;
						sscanf (line, "%d", &number);
				    
							if(isPrime(number) == 0){
						        fprintf(outFile2, "%s", line);
							}
				    
					}
				}
			    printf("Thread-> Type: %c Id: %d, cleaning primes operation is finished for file: numbers2.txt\n", param->TYPE_OF_THREAD, param->threadID);
				//Close file operation before unlocking the mutex.
				fclose(inFile2);
				fclose(outFile2);
				//Renaming file after purification.
				int ret = remove(RESOURCE_TYPE_1_2_FILE_NAME);
				if(ret == 0){
					rename(CLEANED_RESOURCE_TYPE_1_2_FILE_NAME, RESOURCE_TYPE_1_2_FILE_NAME);
				}
				// Unlock resource so there will be no hold and wait situation and starvation.
				pthread_mutex_unlock(&RESOURCE_TYPE_1_2);
				// Boolean like checkers
				primesCleaned_File_TYPE_1_2 = 1;
		}
		//In this if statement since we don't know how many A type thread captured prime number before we clean it
		//We must check for final version of output file if prime number exist or not.
		if(copyPasteFinished_File_TYPE_1_1 == 1 && copyPasteFinished_File_TYPE_1_2 == 1){
			if(pthread_mutex_lock(&RESOURCE_TYPE_2) == 0){
				checkingDone =1;
				FILE* inFile = fopen(RESOURCE_TYPE_2_FILE_NAME, "r");
				FILE* outFile = fopen(CLEANED_RESOURCE_TYPE_2_FILE_NAME, "a");		
				char line [10];
				if( inFile != NULL )
				{
					while( fgets(line, sizeof(line), inFile) != NULL )
					{
						int number;
						sscanf (line, "%d", &number);
						if(isPrime(number) == 0){
					        fprintf(outFile, "%s", line);
						}
					}	
				}
				fclose(inFile);
				fclose(outFile);
				//Deleting numbers: Copying necessary numbers to temp and renaming temp as old file name
				int ret = remove(RESOURCE_TYPE_2_FILE_NAME);
				if(ret == 0){
					rename(CLEANED_RESOURCE_TYPE_2_FILE_NAME, RESOURCE_TYPE_2_FILE_NAME);
				}
				pthread_mutex_unlock(&RESOURCE_TYPE_2);	
			}
	
		}
		
	}
	return NULL;
}

void *cleanNegativesFromResourceType1(void *parameters){
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	short checkingDone = 0;
	while(negativesCleaned_File_TYPE_1_1 == 0 || negativesCleaned_File_TYPE_1_2 == 0 || copyPasteFinished_File_TYPE_1_1 == 0 || copyPasteFinished_File_TYPE_1_2 == 0 || checkingDone == 0){
		//Check if resource is locked
		if(	pthread_mutex_lock(&RESOURCE_TYPE_1_2) == 0){
			//Inform
		    printf("Thread-> Type: %c Id: %d, cleaning negatives from file: numbers2.txt\n", param->TYPE_OF_THREAD, param->threadID);
			//Do cleaning job
			//Open file
			FILE* inFile2 = fopen(RESOURCE_TYPE_1_2_FILE_NAME, "r");
			FILE* outFile2 = fopen(CLEANED_RESOURCE_TYPE_1_2_FILE_NAME, "a");
			char line [10];
			if( inFile2 != NULL )
			{
			
				while( fgets(line, sizeof(line), inFile2) != NULL )
				{
					int number;
					sscanf (line, "%d", &number);
			    
					if( number >= 0){
					        fprintf(outFile2, "%s", line);
						}
			    
				}
			}
			//Close file operation before unlocking the mutex.
		    printf("Thread-> Type: %c Id: %d, cleaning negatives operation is finished for file: numbers2.txt\n", param->TYPE_OF_THREAD, param->threadID);
			fclose(inFile2);
			fclose(outFile2);
		
			int ret = remove(RESOURCE_TYPE_1_2_FILE_NAME);
			if(ret == 0){
				rename(CLEANED_RESOURCE_TYPE_1_2_FILE_NAME, RESOURCE_TYPE_1_2_FILE_NAME);
			}
			// Unlock resource so there will be no hold and wait situation and starvation.
			pthread_mutex_unlock(&RESOURCE_TYPE_1_2);
			negativesCleaned_File_TYPE_1_2 = 1;
		}
		if(	pthread_mutex_lock(&RESOURCE_TYPE_1_1) == 0){
		    printf("Thread-> Type: %c Id: %d, cleaning negatives from file: numbers2.txt\n", param->TYPE_OF_THREAD, param->threadID);
			FILE* inFile = fopen(RESOURCE_TYPE_1_1_FILE_NAME, "r");
			FILE* outFile = fopen(CLEANED_RESOURCE_TYPE_1_1_FILE_NAME, "a");
			char line [10]; 
			if( inFile != NULL )
			{
				while( fgets(line, sizeof(line), inFile) != NULL )
				{
		
					int number;
					sscanf (line, "%d", &number);
						if( number >= 0){
					        fprintf(outFile, "%s", line);
						}
			    
				}	
			}
			fclose(inFile);
			fclose(outFile);
			int ret = remove(RESOURCE_TYPE_1_1_FILE_NAME);
			if(ret == 0){
				rename(CLEANED_RESOURCE_TYPE_1_1_FILE_NAME, RESOURCE_TYPE_1_1_FILE_NAME);
			}
		    printf("Thread-> Type: %c Id: %d, cleaning negatives operation is finished for file: numbers1.txt\n", param->TYPE_OF_THREAD, param->threadID);
			pthread_mutex_unlock(&RESOURCE_TYPE_1_1);
			negativesCleaned_File_TYPE_1_1 = 1;
		}
		//In this if statement since we don't know how many A type thread captured negative number before we clean it
		//We must check for final version of output file if prime number exist or not.
		if(copyPasteFinished_File_TYPE_1_1 == 1 && copyPasteFinished_File_TYPE_1_2 == 1){
			if(pthread_mutex_lock(&RESOURCE_TYPE_2) == 0){
				checkingDone =1;
				FILE* inFile = fopen(RESOURCE_TYPE_2_FILE_NAME, "r");
				FILE* outFile = fopen(CLEANED_RESOURCE_TYPE_2_FILE_NAME, "a");		
				char line [10];
				if( inFile != NULL )
				{
					while( fgets(line, sizeof(line), inFile) != NULL )
					{
						int number;
						sscanf (line, "%d", &number);
							if( number >= 0){
						        fprintf(outFile, "%s", line);
							}
					}	
				}
				fclose(inFile);
				fclose(outFile);
				int ret = remove(RESOURCE_TYPE_2_FILE_NAME);
				if(ret == 0){
					rename(CLEANED_RESOURCE_TYPE_2_FILE_NAME, RESOURCE_TYPE_2_FILE_NAME);
				}
				pthread_mutex_unlock(&RESOURCE_TYPE_2);	
			}
	
		}
	}
	return NULL;

}
void *readFromResource1AndWriteToResource2(void *parameters){
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;

	while(copyPasteFinished_File_TYPE_1_1 == 0 || copyPasteFinished_File_TYPE_1_2 == 0){
		//Check for resource is locked or not
		if(pthread_mutex_lock(&RESOURCE_TYPE_1_1) == 0){
				if(pthread_mutex_lock(&RESOURCE_TYPE_2) == 0){
					FILE* inFile = fopen(RESOURCE_TYPE_1_1_FILE_NAME, "r");
					FILE* outFile = fopen(RESOURCE_TYPE_2_FILE_NAME, "a");
					FILE* tempFile = fopen(CLEANED_RESOURCE_TYPE_2_FILE_NAME, "w+");
					char line [10]; 
					int lineCount = 0;
					if( inFile != NULL )
					{
					    printf("Thread-> Type: %c Id: %d, randomly decides how many lines is going to cut and paste\n", param->TYPE_OF_THREAD, param->threadID);
						srand(time(NULL));
						int r = ( rand() % 10 ) + 1;
				
						while( fgets(line, sizeof(line), inFile) != NULL )
						{
						
							lineCount++;
							if(lineCount <= r){
								fprintf(outFile, "%s", line);
							}
							else{
								//Send to temp file
								fprintf(tempFile, "%s", line);
							}
					
						}
						//Checker for finished or not
						if(fgets(line, sizeof(line), inFile) == NULL){
							copyPasteFinished_File_TYPE_1_1 = 1;
						} 
				
					}
					//Close files
					fclose(inFile);
					fclose(outFile);
					fclose(tempFile);
					//Remove old resource file and rename temp as old resource file name.
					int ret = remove(RESOURCE_TYPE_2_FILE_NAME);
					if(ret == 0){
						rename(CLEANED_RESOURCE_TYPE_2_FILE_NAME, RESOURCE_TYPE_2_FILE_NAME);
					}
					//Unlock mutex
					pthread_mutex_unlock(&RESOURCE_TYPE_2);
				}
				pthread_mutex_unlock(&RESOURCE_TYPE_1_1);
			}
			if(pthread_mutex_lock(&RESOURCE_TYPE_1_2) == 0){
			
				if(pthread_mutex_lock(&RESOURCE_TYPE_2) == 0){
					//Open files
					FILE* inFile2 = fopen(RESOURCE_TYPE_1_1_FILE_NAME, "r");
					FILE* outFile2 = fopen(RESOURCE_TYPE_2_FILE_NAME, "a");
					FILE* tempFile2 = fopen(CLEANED_RESOURCE_TYPE_2_FILE_NAME, "w+");
					char line [10]; // maybe you have to user better value here
					int lineCount = 0;
					if( inFile2 != NULL )
					{
					    printf("Thread-> Type: %c Id: %d, randomly decides how many lines is going to cut and paste\n", param->TYPE_OF_THREAD, param->threadID);
						srand(time(NULL));
						int r = ( rand() % 10 ) + 1;
			
						while( fgets(line, sizeof(line), inFile2) != NULL )
						{
							lineCount++;
							if(lineCount <= r){
								fprintf(outFile2, "%s", line);
							}
							else{
								//Tempe aktar
								fprintf(tempFile2, "%s", line);
							}
				
						}
						if(fgets(line, sizeof(line), inFile2) == NULL){
							//Copy paste is finished
							copyPasteFinished_File_TYPE_1_2 = 1;
						} 
			
					}
					//Close files
					fclose(inFile2);
					fclose(outFile2);
					fclose(tempFile2);
					//Remove old resource file and rename temp as old resource file name.
					int ret = remove(RESOURCE_TYPE_2_FILE_NAME);
					if(ret == 0){
						rename(CLEANED_RESOURCE_TYPE_2_FILE_NAME, RESOURCE_TYPE_2_FILE_NAME);
					}
					//Unlock mutex
					pthread_mutex_unlock(&RESOURCE_TYPE_2);
				}
				pthread_mutex_unlock(&RESOURCE_TYPE_1_2);
			}
	}

	
	return NULL;
}
