/* It starts with three parameters
  - n: the max number we want to find out about prime numbers
  - c: the chunk size
  - print out whether n is a prime number

  Keep a ready queue with the list of threads from (0 to p-1) that are ready
  for more work.

  - The program should also keep a list of data structures, one for each
  slave thread.

int pthread_create (     thread_id,          thread_attributes,             point_to_function, argument_for_function)
int pthread_create (pthread_t *tid, const pthread_attr_t *attr, void*(*start_rountine)(void*),             void *arg)

- returns the ID of the new thread via the tid argument
- the attr parameter is used to set thread attributes, NULL as default
- the start-routine is the C routine that the thread will start executing once it
is created
- a single argument may be passed to start_routine via arg. it must be passed by
reference as a pointer cast of type void. You can use a data structure if you
want to pass multiple arguments
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// CONSTANTS
#define FAIL 0
#define PASS 1

int validateCommandLine(int argc);
void* helloworld(void* p);

struct threadStrcut {
  pthread_t id;
  int threadIdex;
  int min;
  int max;
};

struct work {
  int min;
  int max;
};

int main(int argc, char *argv[]) {
  ///* PROGRAM DONT DELETE

  /********************** Initialize Program **********************************/
  if (validateCommandLine(argc) == FAIL){
    printf("%s\n", "Enter the correct number of arguments");
  }
  int nthPrime = atoi(argv[1]);
  int numOfThreads = atoi(argv[2]);
  int chunckSize = atoi(argv[3]);
  /****************************************************************************/





  /******************** Create array up to Nth Prime **************************/
  // initialize the array from 2 -> nThPrimeNumber
  int arrayPrime[nthPrime - 1];
  int startNum = 2;
  for (size_t i = 0; i < nthPrime; i++) {
    arrayPrime[i]= startNum;
    startNum++;
  }
  /****************************************************************************/
  ///** DISPLAY THE ARRAY THAT WAS JUST CREATED
  //---------------------------------------------------------->
  int arrayLength = sizeof(arrayPrime)/sizeof(arrayPrime[0]);
  int numOfWorks =(int) ceil((double)arrayLength/(double)chunckSize);

  printf("%s\n", "");
  printf("[Primes up to this #: %d] [# of threads: %d] [Size of each chuck: %d]\n", nthPrime, numOfThreads, chunckSize);
  printf("[This is the size of the array: %d]\n", arrayLength);
  printf("[The number of work Structs to create: %d]\n", numOfWorks);

  for (size_t i = 0; i < arrayLength ; i++) {
    printf("%d ", arrayPrime[i]);
  }
  printf("%s\n", "");
  printf("%s\n", "");
  //---------------------------------------------------------->

  // */



  /** READ TO FIND OUT WHAT IM DOING
    - Pointer that will be used to index through all the thread structs I
    create.
  */
  struct threadStrcut* threadPTR;
  /** READ TO FIND OUT WHAT IM DOING
    - Allocating the number of threads passed by the command line * the size of
    each threadStrcut type.
    - I'm then casting it to type of threadStrcut.
  */
  threadPTR = (struct threadStrcut*)malloc(numOfThreads*sizeof(struct threadStrcut));

  //SET EACH THREADS MIN AND MAX
  int min = 0;
  int max = chunckSize - 1;

  // CREATE THE THREADS AND INDEX TO THE NEXT POINTER LOCATION
  for (size_t i = 0; i < (numOfThreads); i++) {
    threadPTR[i].threadIdex = i;
    threadPTR[i].min = min;
    threadPTR[i].max = max;
    pthread_create(&threadPTR[i].id, NULL, helloworld, &threadPTR[i]);
    min += chunckSize;
    max = min + (chunckSize - 1);
  }

  // JOIN THE THREADS THAT WHERE CREATED
  for (size_t i = 0; i < (numOfThreads); i++) {
    pthread_join(threadPTR[i].id, NULL);
  }
  return 0;
  // */

} //END OF MAIN

/*---------------------------- checkForTwoArguments --------------------------*
| They entered more than 3 argument is the command line or less then 3
| argument. We have 4 becuase we have to count the name of the program.
*-----------------------------------------------------------------------------*/
int validateCommandLine(int argc){
  if (argc != 4) {
    return FAIL; // FAILED THE TEST
  }
  return PASS; // PASSED THE TEST
}

/*------------------------------- helloworld ----------------------------------*
| They entered more than 3 argument is the command line or less then 3
| argument. We have 4 becuase we have to count the name of the program.
*-----------------------------------------------------------------------------*/
void* helloworld(void* p){
  struct threadStrcut* individualStruct = (struct threadStrcut*)p;
  printf("This is this threads ID #: %d\n", (individualStruct->threadIdex));
  printf("Thread # min: %d\n", (individualStruct->min));
  printf("Thread # max: %d\n", (individualStruct->max));
  return 0;
}

/*
| 2  3  4  5  6  | 7  8  9  10 11 | 12 13 14 15 16 | 17 18 19 20 21 |
| 22 23 24 25 26 | 27 28 29 30 31 | 32 33 34 35 36 | 37 38 39 40 41 |
| 42 43 44 45 46 | 47 48 49 50    |

nthPrime = 50
numOfThreads = 10
chunckSize = 5


while () {

}
int min = 0;
int max = chunckSize - 1;
*/

/* HOW TO ACCESS A STRUCT ELEMENTS
int main()
{
   // Create an array of structures
   struct Point arr[10];

   // Access array members
   arr[0].x = 10;
   arr[0].y = 20;

   printf("%d %d", arr[0].x, arr[0].y);
   return 0;
}
*/
/* HOW TO COMPILE A C PROGRAM
 gcc e_And_Sqrt_Calculation.c -o e_And_Sqrt_Calculation -lm
*/
