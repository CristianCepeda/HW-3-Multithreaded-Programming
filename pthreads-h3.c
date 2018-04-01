// /*==========================================================================
// |   Source code:  [pthreads-h3.c]
// |        Author:  [Cristian C Cepeda]
// |    Student ID:  [4959207]
// |    Assignment:  Program #[3] [pthreads-h3.c]
// |
// |        Course:  [COP 4520 (Introduction to Parallel Computing)]
// |       Section:  [U01]
// |    Instructor:  Jason Liu
// |      Due Date:  [01 April 2018], at the beginning of class
// |
// |	I hereby certify that this collective work is my own
// |	and none of it is the work of any other person or entity.
// |
// |	__Cristian C Cepeda____________________________________ [Signature]
// |
// |        Language:  [Program Language: C]
// |
// |     Compile/Run:
// |
// |                  COMPILE
// |                  Once you are in the correct directory where all the
// |                  files are found enter
// |
// |                  make
// |
// |                  - This will compile all the necessary files needed to
// |                    run the program. Incase you just want to compile this
// |                    file then you can run this command to compile
// |
// |                      - gcc pthreads-h3.c -o pthreads-h3
// |
// |                  RUN
// |                  - So Enter this to Run a test version of it
// |
// |                  ./pthreads-h3 20 3 2
// |
// |                  - Where 20 equals the number up to which you want to find
// |                  your prime numbers.
// |                  - Where 3 equals to the number of threads you want to create
// |                  so that the program runs.
// |                  - Where 2 is the value each thread should loop through the
// |                  array and determine if its a prime number.
// |
// |  +------------------------------------------------------------------------
// |
// |  Description:  [ The main goal of this program is to receive three
// |                  arguments from the command line. These three arguments
// |                  determine the expected output. The first argument is used
// |                  to create an array so that we can loop through it and we
// |                  can determine if its a prime number or not. The second
// |                  argument is used to determine how many threads should
// |                  work on the array of numbers. And finally the third
// |                  argument determines the chunk size that each thread
// |                  should look at while they are looping through the array.
// |                  To test the program you will first have to clone it and
// |                  place it in a directory. Once your inside the directory
// |                  you can run the command line "make" and it should compile
// |                  the necessary files. You then will have the option to
// |                  change the command line arguments but to give you an
// |                  example you can type this. And it should display all the
// |                  prime numbers up to 20.
// |
// |                  - ./pthreads-h3 20 3 2
// |
// |                 I would play around by changing the values from the
// |                 command line so that you can see different outputs now. ]
// |
// |        Input:  [ The input is done through the command line. An example
// |                  looks like this:
// |
// |                  -  make
// |                  -  ./pthreads-h3 20 3 2
// |                        - Where 20 equals the number up to which you want
// |                        to find your prime numbers.
// |                        - Where 3 equals to the number of threads you want
// |                        to create so that the program runs.
// |                        - Where 2 is the value each thread should loop
// |                        through the array and determine if its a prime
// |                        number.
// |
// |                 - The restriction on the program can be seen in the
// |                 validation methods. The command line will have to satisfy
// |                 those test so that the program can work. ]
// |
// |       Output:  [The order of the outputs for the program go in this order.
// |
// |                 - A list of all the prime numbers from 2 up to "N". "N"
// |                 being the first argument you passed in the command line
// |                 prompt.
// |                 - And if the value "N" is a Prime number.
// |                 - The time it took the program to run. ]
// |
// |
// |      Process:  [ The program’s steps are as follows:
// |                1. The program first validates the command line arguments.
// |                There should be a total of three arguments so that it can
// |                run and some of these values have some restrictions.
// |                2. Once the program has validated the command line
// |                arguments it creates the array to hold all the numbers that
// |                we will be checking to determine if they are primes or not.
// |                3. The next step is that the Master thread creates the
// |                total number of work packets that will need to be consumed
// |                by the threads. These work packets will be used by the
// |                threads so that they can update their min, max, and kth
// |                Value that they are searching for through the array.
// |                4. The next step is to call the pthread_create() function
// |                so that each thread can be created based on the number
// |                passed on by the command line. As the threads are created
// |                they enter the work function where its here that each
// |                thread has to work until they are done checking through the
// |                array.
// |                5. See the file program-notes.txt under version 2 to see
// |                the thought process on how to solve this problem. ]
// |
// |   Required Features Not Included:  [ Everything was completed ]
// |
// |   Known Bugs:  [There are no Bugs]
// |  *===================================================================== */

#include "primeFinder.h"

int main(int argc, char *argv[]) {
  clock_t begin = clock();
  // /* PROGRAM DON'T DELETE
  pthread_mutex_init(&mutex, NULL);

  //                                                                     STEP 1
  //--------------------------------------------------------------------------->
  if (validateCommandLine(argc,argv) == FAIL){
    pthread_mutex_destroy(&mutex);
    return 0;
  }

  //                                                                     STEP 2
  //--------------------------------------------------------------------------->
  int nthPrime = atoi(argv[1]);
  int NUMBER_OF_THREADS = atoi(argv[2]);
  int chunkSize = atoi(argv[3]);

  //                                                                     STEP 3
  //--------------------------------------------------------------------------->
  int ARRAY_LENGTH = nthPrime + 1;
  globalPrimeArray = malloc(ARRAY_LENGTH * sizeof(*globalPrimeArray));
  int value = 0;
  for (size_t arrayIndex = 0; arrayIndex < ARRAY_LENGTH; arrayIndex++) {
    globalPrimeArray[arrayIndex] = value;
    value +=1;
  }

  // 0 and 1 are not considered prime numbers so then set them to 0
  globalPrimeArray[0] = 0;
  globalPrimeArray[1] = 0;
  /************************** [  INSERT 1  ] *********************************/

  //                                                                     STEP 4
  //--------------------------------------------------------------------------->
  // Create the work packets
  int NUMBER_OF_WORK_PACKETS = (int)ceil((double)ARRAY_LENGTH/(double)chunkSize);
  workSegment = NULL;
  createWork(&workSegment, ARRAY_LENGTH, NUMBER_OF_WORK_PACKETS, chunkSize);
  /************************** [  INSERT 2  ] *********************************/

  //                                                                   STEP 4-5
  //--------------------------------------------------------------------------->
  // Variables that will be used by each thread
  UP_TO_THIS_NUMBER = (int)sqrt((double)(nthPrime));
  kthValueON = 2; // First Prime is 2
  packetON = 0;
  packetsLEFT = NUMBER_OF_WORK_PACKETS; // will be meant to decrease every time the method getWork is called.
  resetPACKETS = NUMBER_OF_WORK_PACKETS;

  //                                                                     STEP 5
  //--------------------------------------------------------------------------->
  struct threadStruct* threadPTR = (struct threadStruct*)malloc(NUMBER_OF_THREADS*sizeof(struct threadStruct));
  for (size_t i = 0; i < NUMBER_OF_THREADS; i++) {
    threadPTR[i].threadIndex = i;
    pthread_create(&threadPTR[i].id, NULL, workMethod, &threadPTR[i]);
  }
  // JOIN THE THREADS THAT WHERE CREATED
  for (size_t i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(threadPTR[i].id, NULL);
  }
  pthread_mutex_destroy(&mutex);
  clock_t end = clock();

  //                                                                 FINAL STEP
  //--------------------------------------------------------------------------->

  // Displaying only the prime numbers globalPrimeArray
  printf("\nTHESE ARE THE PRIME NUMBERS FROM 0 -> %d\n", nthPrime);
  for (size_t arrayIndex = 0; arrayIndex < ARRAY_LENGTH ; arrayIndex++) {
    if (globalPrimeArray[arrayIndex] == 0){
      // DO NOTHING
    } else {
      printf("%d ", globalPrimeArray[arrayIndex]);
    }
  }

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nTIME THE PROGRAM TOOK: %f\n", time_spent);
  if(globalPrimeArray[nthPrime] == 0){
    printf("nth Number: %d%s\n", nthPrime, " [NOT PRIME NUMBER]");
  } else {
    printf("nth Number: %d%s\n", nthPrime , " [PRIME NUMBER]");
  }
  printf("%s\n", "");
  return 0;
  // */

} // END OF MAIN

/*--------------------------- validateCommandLine ---------------------------*
|  Purpose:  If the user enters more that 3 arguments in the command line then
|            its invalid and the test fails. We have 4 because we have to count
|            the name of the program.
*-----------------------------------------------------------------------------*/
int validateCommandLine(int argc, char *argv[]){
  if (argc != 4) {
    printf("%s\n", "Enter the correct number of arguments");
    return FAIL; // FAILED THE TEST
  } else if (checkForOnlyNumbers(argc,argv) == FAIL){
    return FAIL; // FAILED THE TEST
  } else if(secondParameter(argv) == FAIL){
    return FAIL;
  } else if(thirdParameter(argv) == FAIL){
    return FAIL;
  }
  return PASS; // PASSED THE TEST
}
/*-------------------------- checkForOnlyNumbers ----------------------------*
|  Purpose:  This function is meant to check and validate that the users didn't
|            input anything other than numbers. So a minus sign to indicate a
|            negative number would be wrong since negative numbers are not
|            accepted. As well as a period to indicate that its a fractional
|            number or decimal number.
|*-----------------------------------------------------------------------------*/
int checkForOnlyNumbers(int argc, char * argv[]) {
  // ********* CHECK IF THERE IS ANYTHING INPUTED BESIDES NUMBERS *************
  size_t row, column;
  for(row = 1; row < argc; row++){
    for(column = 0; argv[row][column] != '\0'; column++){
      int valueOfArgv = argv[row][column];
      if (valueOfArgv < ZERO_ASCII || valueOfArgv > NINE_ASCII ) { //57 is 9 and 48 is 0 in ascii
        printf("\n%s\n", "***************************************************");
        printf("%s\n%s\n", "There was an incorrect input value. Only positive",
        "whole values are accepted.");
        printf("\n");
        printf("%s\n\n", "***************************************************");
        return FAIL; // didn't work is the number 0
      }
    }
  }
  return PASS; // if it worked then return 1
}
/*--------------------------- secondParameter -------------------------------*
|  Purpose: See if the second argument equals to zero. If it does then the
|           program wont work since this program is based on creating threads
|           and having the threads do the work of finding the prime numbers.
|*-----------------------------------------------------------------------------*/
int secondParameter(char * argv[]) {
  if (atoi(argv[2]) == 0){
    printf("\n%s\n\n", "This wont do anything. Change the second parameter so that it can be at least 1.");
    return FAIL;
  }
  return PASS;
}
/*----------------------------- thirdParameter ------------------------------*
|  Purpose: See if the third argument equals to zero. If it does then it fails
|           the test because the threads need a value so that they can know how
|           many numbers they need to check in each iteration.
|*-----------------------------------------------------------------------------*/
int thirdParameter(char * argv[]) {
  if (atoi(argv[3]) == 0){
    printf("\n%s\n\n", "This wont do anything. Change the third parameter so that it can be at least 1.");
    return FAIL;
  }
  return PASS;
}

/*----------------------------- createWork ----------------------------------*
|  Purpose: This method when called should return a pointer to a work struct.
|           This pointer can be index and the other work packets can be found.
|           You can find out the size of the work struct by finding the value
|           thats stored inside the NUMBER_OF_WORK_PACKETS.
|*-----------------------------------------------------------------------------*/
void createWork(struct work **returnWorkPackets, int ARRAY_LENGTH, int NUMBER_OF_WORK_PACKETS, int chunkSize) {
  struct work* tempWorkArray = (struct work*)malloc(NUMBER_OF_WORK_PACKETS*sizeof(struct work));

  int minValueChecking, maxValueChecking, numbersLeftToCheck;
  numbersLeftToCheck = ARRAY_LENGTH;

  // Initialize the work struct so we can add each work workSegment item into the queue.
  for(size_t packetON = 0; packetON < NUMBER_OF_WORK_PACKETS; packetON++){
    if(packetON == 0){ // Initializing the first workSegment
      minValueChecking = 0;
      maxValueChecking = chunkSize - 1;  // This is to fix the offset of arrays
      tempWorkArray[packetON].workMin = minValueChecking;
      tempWorkArray[packetON].workMax = maxValueChecking;
      // Since you already gave chunkSize subtract it from remainingNumbers
      numbersLeftToCheck -= chunkSize;
    } else if (packetON == (NUMBER_OF_WORK_PACKETS - 1)) { // Initializing the last workSegment
      minValueChecking = maxValueChecking + 1;
      maxValueChecking = maxValueChecking + numbersLeftToCheck;
      tempWorkArray[packetON].workMin = minValueChecking;
      tempWorkArray[packetON].workMax = maxValueChecking;
    } else { // Initializing any workSegment in between
      minValueChecking = maxValueChecking + 1;
      maxValueChecking = maxValueChecking + chunkSize;
      tempWorkArray[packetON].workMin = minValueChecking;
      tempWorkArray[packetON].workMax = maxValueChecking;
      numbersLeftToCheck -= chunkSize;
    }
  }

  *returnWorkPackets = tempWorkArray;
}
/*------------------------------- getWork -----------------------------------*
|  Purpose: This method is important because it allows each thread to call it
|           and receive the next work packet from the array of all work packets.
|           This is important because then each thread can update the min, max,
|           and kthValue that they are checking. So its critical that they
|           update the global variables correctly. The method takes in a work
|           struct and it and assigns the next work packet to it so that it
|           can then be passed back to each thread.
*-----------------------------------------------------------------------------*/
void getWork(struct work **returnWorkPacket) {
  int ONE_WORK_PACKET = 1;
  struct work* tempWorkArray = (struct work*)malloc(ONE_WORK_PACKET*sizeof(struct work));

  if(packetsLEFT == 0){
    kthValueON += 1;
    packetON = 0;  // RESET VALUES
    packetsLEFT = resetPACKETS; // RESET VALUES

    tempWorkArray->workMin = workSegment[packetON].workMin;
    tempWorkArray->workMax = workSegment[packetON].workMax;
    tempWorkArray->kthValue = kthValueON;
    packetON += 1;
    packetsLEFT -= 1;
  } else {
    tempWorkArray->workMin = workSegment[packetON].workMin;
    tempWorkArray->workMax = workSegment[packetON].workMax;
    tempWorkArray->kthValue = kthValueON;
    packetON += 1;
    packetsLEFT -= 1;
  }

  *returnWorkPacket = tempWorkArray;
}
/*----------------------------- workMethod ----------------------------------*
|  Purpose: This is the main Method that each thread calls. Notice how in here
|           we create a work struct called individualWorkPacket. This will be
|           used to go into the getWork() and get just one packet from the
|           array with all the work Packets. Once you have a single work Packet
|           then you use the information to loop through the array and change
|           the value of prime to not prime.
*-----------------------------------------------------------------------------*/
void* workMethod(void* p){
  struct threadStruct* individualStruct = (struct threadStruct*)p;
  struct work* individualWorkPacket;

  // int kthValueON = determineKthValue(int arrayIndex, int arrLength);
  while (kthValueON <= UP_TO_THIS_NUMBER) {
    pthread_mutex_lock(&mutex);
    getWork(&individualWorkPacket);
    individualStruct->blockNum.workMin = individualWorkPacket->workMin;
    individualStruct->blockNum.workMax = individualWorkPacket->workMax;
    individualStruct->blockNum.kthValue = individualWorkPacket->kthValue;

    /************************** [  INSERT 3  ] *******************************/

    int workMin = individualStruct->blockNum.workMin;
    int workMax = individualStruct->blockNum.workMax;
    int kthValue = individualStruct->blockNum.kthValue;
    pthread_mutex_unlock(&mutex);

    for (size_t valueChecking = workMin; valueChecking <= workMax; valueChecking++) {
      if (globalPrimeArray[valueChecking] != 0) { // Then the value checking appears to be prime
        if (globalPrimeArray[valueChecking] == kthValue){
          // Do NOTHING
        } else if (globalPrimeArray[valueChecking]%kthValue == 0) {
          globalPrimeArray[valueChecking] = 0;
        }
      }
    } // END OF FOR LOOP

  } // END OF WHILE LOOP

  return 0;
}

/* THINGS YOU CAN INSERT TO SEE TRACE OF PRINT OUTS

  ******************************  [  INSERT 1  ]  ******************************
  // Displaying the globalPrimeArray
  for (size_t arrayIndex = 0; arrayIndex < ARRAY_LENGTH ; arrayIndex++) {
    printf("%d ", globalPrimeArray[arrayIndex]);
  }
  printf("%s\n\n", "");

  ******************************  [  INSERT 2  ]  ******************************
  // PRINT STATEMENTS
  printf("NUMBER_OF_WORK_PACKETS: %d\n", NUMBER_OF_WORK_PACKETS);
  for (size_t packetON = 0; packetON < NUMBER_OF_WORK_PACKETS; packetON++) {
    printf("[For Work Group: %zu] [Min: %d] [Max: %d] [kth: %d]\n", packetON, workSegment[packetON].workMin, workSegment[packetON].workMax, workSegment[packetON].kthValue);
  }

  ************************** [  INSERT 3  ] *******************************
  printf("[ID#: %d] [Min: %d] [Max: %d] [kth: %d]\n", individualStruct->threadIndex, individualStruct->blockNum.workMin, individualStruct->blockNum.workMax, individualStruct->blockNum.kthValue);


*/
