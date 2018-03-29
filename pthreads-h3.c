/* PROGRAM NOTES

  The three command line parameters
    - n: Primes up to this #
    - p: # of threads
    - c: size of each chunk

  - Master thread dictates work to slave threads. Each slave thread waits for
  work does the work and comes back for more until there is no more work.

  - Master should only loop through array that it manages from k=2 to sqrt(n).

  - Master and slave threads should communicate via conditional variables.

  *** Important need to know this so that we can create Classical Producer-Consumer Problem ***
  - Keep a ready queue with the list of threads from (0 to p-1) that are ready
  for more work.

  - The program should also keep a list of data structures, one for each
  slave thread.


  //                Notes on Producer-Consumer Implementation
  //--------------------------------------------------------------------------->
  - Start from slide 24 at this URL
  - https://people.cs.pitt.edu/~melhem/courses/xx45p/pthread.pdf


  //                Notes on Producer-Consumer Implementation
  //--------------------------------------------------------------------------->
  - Start from slide 15 at this URL
  - https://people.cs.pitt.edu/~melhem/courses/xx45p/pthread.pdf


  //                     Notes on Condition Variables
  //--------------------------------------------------------------------------->
  - they allow threads to synchronize based upon the actual data value;
  - With condition variables, the thread sleeps and waits on the condition variable;
  - A condition variable is always used in conjunction with a mutex.

  - Start from slide 17 at this URL
  - https://people.cs.pitt.edu/~melhem/courses/xx45p/pthread.pdf


  //                     Explanation of pthread_create
  //--------------------------------------------------------------------------->
  int pthread_create (     thread_id,          thread_attributes,             point_to_function, argument_for_function)
  int pthread_create (pthread_t *tid, const pthread_attr_t *attr, void*(*start_rountine)(void*),             void *arg)

  - returns the ID of the new thread via the tid argument
  - the attr parameter is used to set thread attributes, NULL as default
  - the start-routine is the C routine that the thread will start executing once it
  is created
  - a single argument may be passed to start_routine via arg. it must be passed by
  reference as a pointer cast of type void. You can use a data structure if you
  want to pass multiple arguments
  //--------------------------------------------------------------------------->

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

// Work should hold a range (min and max indicies to the shared array) and a number k
// It should filter out the multiples of k within the given range (by marking them off)
struct work {
  int workMin;
  int workMax;
  int kthValue;
};
struct threadStruct {
  pthread_t id;
  int threadIndex;
  struct work blockNum;
};

pthread_mutex_t mutex;
pthread_cond_t cond_empty, cond_full;

int globalPrimeArray*;

int main(int argc, char *argv[]) {
  ///* PROGRAM DON'T DELETE

  //                            Initialize Program
  //--------------------------------------------------------------------------->
  if (validateCommandLine(argc) == FAIL){
    printf("%s\n", "Enter the correct number of arguments");
  }
  int nthPrime = atoi(argv[1]);
  int numOfThreads = atoi(argv[2]);
  int chunkSize = atoi(argv[3]);
  //--------------------------------------------------------------------------->

  pthread_mutex_init(&mutex, NULL);

  //                      Create array up to Nth Prime
  //--------------------------------------------------------------------------->
  // - Initialize the array from 2 -> nThPrimeNumber
  // - Maybe have each index equal to 1 and as we loop set them to 0 indicating
  // false for not prime number
  int elements = nthPrime - 1;

  int arrayPrime[nthPrime - 1];
  int startNum = 2;
  for (size_t i = 0; i < nthPrime; i++) {
    arrayPrime[i]= startNum;
    startNum++;
  }
  //--------------------------------------------------------------------------->

  //                  Display the array that was just created
  //--------------------------------------------------------------------------->
  int arrayLength = sizeof(arrayPrime)/sizeof(arrayPrime[0]);
  int numOfWorks =(int) ceil((double)arrayLength/(double)chunkSize);
  //                  [  INSERT 1  ] PRINT OUT INFO OF PROGRAM
  printf("\n[Primes up to this #: %d] [# of threads: %d] [Size of each chunk: %d]\n", nthPrime, numOfThreads, chunkSize);
  printf("[This is the size of the array: %d]\n", arrayLength);
  printf("[The number of work Structs to create: %d]\n", numOfWorks);
  for (size_t i = 0; i < arrayLength ; i++) {
    printf("%d ", arrayPrime[i]);
  }
  printf("%s\n\n", "");
  //--------------------------------------------------------------------------->

  //         Create the work struct that should then be put into queue
  //--------------------------------------------------------------------------->
  struct work* group;
  group = (struct work*)malloc(numOfWorks*sizeof(struct work));

  int minValue, maxValue;
  int remainingNumbers = arrayLength;

  // Initialize the work struct so we can add each work group item into the queue.
  for(size_t i = 0; i < numOfWorks; i++){
    if(i == 0){ // Initializing the first work group
      minValue = 0;
      maxValue = chunkSize - 1;  // This is to fix the offset of arrays
      group[i].workMin = minValue;
      group[i].workMax = maxValue;
      // Since you already gave chunkSize subtract it from remainingNumbers
      remainingNumbers -= chunkSize;
    } else if (i == (numOfWorks - 1)) { // Initializing the last work group
      minValue = maxValue + 1;
      maxValue = maxValue + remainingNumbers;
      group[i].workMin = minValue;
      group[i].workMax = maxValue;
    } else { // Initializing any work group in between
      minValue = maxValue + 1;
      maxValue = maxValue + chunkSize;
      group[i].workMin = minValue;
      group[i].workMax = maxValue;
      remainingNumbers -= chunkSize;
    }
  }
  //        [  INSERT 2  ] TO PRINT OUT THE MIN MAX OF EACH WORK GROUP
  //--------------------------------------------------------------------------->

  //            Create Pointer to point to threadStruct
  //--------------------------------------------------------------------------->
  // - Pointer that will be used to index through all the thread structs I create.
  struct threadStruct* threadPTR;

  // - Allocating the number of threads passed by the command line * the size of each threadStruct type.
  // - Then casting it to type of threadStruct.
  threadPTR = (struct threadStruct*)malloc(numOfThreads*sizeof(struct threadStruct));

  // CREATE THE THREADS AND INDEX TO THE NEXT POINTER LOCATION
  for (size_t i = 0; i < numOfThreads; i++) {
    threadPTR[i].threadIndex = i;
    threadPTR[i].blockNum.workMin = group[i].workMin;
    threadPTR[i].blockNum.workMax = group[i].workMax;
    pthread_create(&threadPTR[i].id, NULL, helloworld, &threadPTR[i]);
  }

  // JOIN THE THREADS THAT WHERE CREATED
  for (size_t i = 0; i < (numOfThreads); i++) {
    pthread_join(threadPTR[i].id, NULL);
  }

  pthread_mutex_destroy(&mutex);

  return 0;
  // */

} // END OF MAIN

/*---------------------------- checkForTwoArguments --------------------------*
| They entered more than 3 argument is the command line or less then 3
| argument. We have 4 because we have to count the name of the program.
*-----------------------------------------------------------------------------*/
int validateCommandLine(int argc){
  if (argc != 4) {
    return FAIL; // FAILED THE TEST
  }
  return PASS; // PASSED THE TEST
}

/*------------------------------- helloworld ----------------------------------*
| - They entered more than 3 argument is the command line or less then 3
| argument. We have 4 because we have to count the name of the program.
|
| - To improve performance move all unnecessary code outside the critical section;
| the code inside a critical section is doomed to be sequential.
*-----------------------------------------------------------------------------*/
void* helloworld(void* p){
  pthread_mutex_lock(&mutex);
  struct threadStruct* individualStruct = (struct threadStruct*)p;
  printf("This is this threads ID #: %d\n", (individualStruct->threadIndex));
  printf("Thread # min: %d\n", (individualStruct->blockNum.workMin));
  printf("Thread # max: %d\n", (individualStruct->blockNum.workMax));
  pthread_mutex_unlock(&mutex);
  return 0;
}

/* THINGS YOU CAN INSERT TO SEE TRACE OF PRINT OUTS

  ******************************  [  INSERT 1  ]  ******************************
  printf("\n[Primes up to this #: %d] [# of threads: %d] [Size of each chunk: %d]\n", nthPrime, numOfThreads, chunkSize);
  printf("[This is the size of the array: %d]\n", arrayLength);
  printf("[The number of work Structs to create: %d]\n", numOfWorks);
  for (size_t i = 0; i < arrayLength ; i++) {
    printf("%d ", arrayPrime[i]);
  }
  printf("%s\n\n", "");

  ******************************  [  INSERT 2  ]  ******************************
  for (size_t i = 0; i < numOfWorks; i++) {
    printf("[For Work Group: %zu] [Min: %d] [Max: %d]\n", i, group[i].workMin, group[i].workMax);
  }

*/

/*
| 2  3  4  5  6  | 7  8  9  10 11 | 12 13 14 15 16 | 17 18 19 20 21 |
| 22 23 24 25 26 | 27 28 29 30 31 | 32 33 34 35 36 | 37 38 39 40 41 |
| 42 43 44 45 46 | 47 48 49 50    |
*/
