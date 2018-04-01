// /*==========================================================================
// |   Source code:  [primeFinder.h]
// |        Author:  [Cristian C Cepeda]
// |    Student ID:  [4959207]
// |    Assignment:  Program #[3] [primeFinder.h]
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
// |
// |    This is the header file.
// |
// *==========================================================================*/

// BOTH FILES
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

//                                                       [pthreads-h3.c]
//--------------------------------------------------------------------->
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

//                                                           [CONSTANTS]
//--------------------------------------------------------------------->
#define FAIL 0
#define PASS 1
#define NOT_DONE 0
#define DONE 1
enum ASCII{ZERO_ASCII = 48, NINE_ASCII = 57};

//                                            PROTOTYPES [pthreads-h3.c]
//--------------------------------------------------------------------->
int validateCommandLine(int argc, char *argv[]);
int checkForOnlyNumbers(int argc, char * argv[]);
int secondParameter(char * argv[]);
int thirdParameter(char * argv[]);

void createWork(struct work **returnWorkPackets, int ARRAY_LENGTH, int NUMBER_OF_WORK_PACKETS, int chunkSize);
void getWork(struct work **returnWorkPacket);
void* workMethod(void* p);

// GLOBAL VARIABLES
pthread_mutex_t mutex;

// Needed
int *globalPrimeArray;
struct work* workSegment;
int UP_TO_THIS_NUMBER; // Used in workMethod()
int kthValueON; // Used in getWork()
int packetON; // Used in getWork()
int packetsLEFT; // Used in getWork()
int resetPACKETS; // Used in getWork()
