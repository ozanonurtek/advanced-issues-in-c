//header.h
//Created by: Ozan Onur Tek - 210201012

#ifndef HEADER_H
#define HEADER_H

//Necessity includes
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

//Thread number, A=4 + B=1 + C=1 = 6 
#define NUMBER_OF_THREADS 6
 
//File names
//Resources
#define RESOURCE_TYPE_1_1_FILE_NAME "numbers1.txt"
#define RESOURCE_TYPE_1_2_FILE_NAME "numbers2.txt"
#define RESOURCE_TYPE_2_FILE_NAME "finalNumbers.txt"
//Temp files to manage cut and pastes.
#define CLEANED_RESOURCE_TYPE_1_1_FILE_NAME "tmp_numbers1.txt"
#define CLEANED_RESOURCE_TYPE_1_2_FILE_NAME "tmp_numbers2.txt"
#define CLEANED_RESOURCE_TYPE_2_FILE_NAME "tmp_finalNumbers.txt"

pthread_t tid[NUMBER_OF_THREADS];

pthread_mutex_t RESOURCE_TYPE_1_1;
pthread_mutex_t RESOURCE_TYPE_1_2; 
pthread_mutex_t RESOURCE_TYPE_2;

//Initiale values for checkers. Using them as booleans.
short primesCleaned_File_TYPE_1_1 = 0;
short primesCleaned_File_TYPE_1_2 = 0;

short negativesCleaned_File_TYPE_1_1 = 0;
short negativesCleaned_File_TYPE_1_2 = 0;

short copyPasteFinished_File_TYPE_1_1 = 0;
short copyPasteFinished_File_TYPE_1_2 = 0;

//Thread parameters struct
typedef struct
{
	char TYPE_OF_THREAD;
	int threadID;
	
}THREAD_PARAMETERS;

//Function signatures
/**
*Function checkFinalCondition()
*Checks if final condition is provided or not, inform user about it.
*
*/
void checkFinalCondition();
/**
*Function isPrime(int number)
*Helper function that checks given number prime or not
*@params: num -> integer number will be checked
*@returns: If given number is prime returns 1, else returns 0 
*/
int isPrime(int num);
/**
*Function initializeMutex()
*Helper function that initialize all mutex
*@returns: if there is no error during initialization returns 0, otherwise returns 1
*/
int initializeMutex();
/**
*Function readFromResource1AndWriteToResource2(void *parameters)
*Runner function for type A threads. Executing cut and paste job.
*@returns: NULL
*@params: reference to parameters of thread.
*/
void *readFromResource1AndWriteToResource2(void *parameters);
/**
*Function cleanNegativesFromResourceType1(void *parameters)
*Runner function for type C threads. Cleaning negative numbers in resource files
*@returns: NULL
*@params: reference to parameters of thread.
*/
void *cleanNegativesFromResourceType1(void *parameters);
/**
*Function cleanNegativesFromResourceType1(void *parameters)
*Runner function for type B threads. Cleaning prime numbers in resource files
*@returns: NULL
*@params: reference to parameters of thread.
*/
void *cleanPrimesFromResourceType1(void *parameters);
#endif