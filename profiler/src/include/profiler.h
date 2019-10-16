#ifndef PROFILER_H_
#define PROFILER_H_
#include <limits.h>


enum _error
  {
    ERROR_THREAD_CREATION = INT_MIN,
    ERROR_THREAD_INIT,
    ERROR_RANGE
  };


/* Usefull for function pointers passed to new threads with an argument with
   original type FuncArgs. */
#define args(X) ((struct FuncArgs * )X)


const int listStatFalse = 0, listStatTrue = 1;


struct FuncArgs
{
  int size;
  bool listStat;
  /* The following leads us to beleave that TIDs may not be sequential. We could
     not find any specific information on this issue upon a quick search but we
     feel that given the following they must not be:"A thread ID may be reused
     after a terminated thread has been joined, or a detached thread has
     terminated."
     -- http://man7.org/linux/man-pages/man3/pthread_self.3.html
  */
  size_t sequentialThreadNum; /* This veriable should NEVER be updated OR
				 accecced out side of initSequentialThreadNum(),
				 incSequentialThreadNum(). This point is
				 critically important ;) */
  pthread_mutex_t sequentialThreadNumMutex;
} funcArgs;


const int maxPolicyNum = 2;	/* There are only three policies */
/* Number of allocations to make for tests. (Maximum is less for veriable sized
   allocation tests because of memory constraints.) */
const int testSizes[] = {4*1, 1024*4, 1024*16, 1024*64, 1024*256, 1024*1024,
			 1024*4096};
const int fixedSizeAllocationUnit = 1; /* For tests with fixed alloction unit
					  size. */
/* Ranges for test with veriable allocation unit sizes. */
const int allocationUnitMin = 1, allocationUnitMax[] = {1024*1, 1024*4, 1024*16,
							1024*64, 1024*256};
#define saveStatOutputFrequency 9 /* How many times per allocation or
				     deallocation loop should printStats be
				     called? Will be called this many times
				     (plus 1) if allocation and
				     deallocation are in the same loop,
				     twice as many otherwise.) */
/* Maximum number of threads that will be created (this does not include the
   initial thread.) */
#define maxThreadCount 16
#define minThreadCount 1 /* We always spin off at least one thread... */
/* AllocSize, deallocSize, allocOrder and deallocOrder are all used by the
   functions randomFixedSizeAllocationsAndDeallocations() and
   randomAllocationsAndDeallocations() */
const int allocSize = 2, deallocSize = 3;
int allocOrder[maxThreadCount][1024*4096 * 2];
int deallocOrder[maxThreadCount][1024*4096 * 3];
double allocationTime[maxThreadCount];
double deallocationTime[maxThreadCount];


struct Stats
{
/* * deallocSize because randomFixedSizeAllocationsAndDeallocations() and
   randomAllocationsAndDeallocations() both use
   (testSizes[args(args)->size] * deallocSize) in their loop control. 
   + 1 because of %. Where 9 is deallocSize. We do not use deallocSize directly
   because we cannot without declaring it using #define and we cannot use
   #define because of naming conflicts with function arguments. We leave it as
   an exercise to the reader to resolve this issue ;). 
   Also note that when saveStat() is called more then once in a function the
   stats for the second call should be found at
   (saveStatOutputFrequency + 1) * callNum, where callNum starts at 0. (or
   alternatively initialBaseIndexForStats can be used for the first index)*/
  double chunksInInUseList[maxThreadCount]
  [saveStatOutputFrequency * 3 +1];
  double chunksInHolesList[maxThreadCount]
  [saveStatOutputFrequency * 3 +1];
  double avgInUseSz[maxThreadCount]
  [saveStatOutputFrequency * 3 +1];
  double avgHoleSz[maxThreadCount]
  [saveStatOutputFrequency * 3 +1];
} stats;


/* Used when saveStats is only called once in a function OR on the initial call
   to saveStats in a function*/
const int initialBaseIndexForStats = 0;
/* Used by all tests */
/* Store pointers returned from actual allocation. */
void * allocs[maxThreadCount][1024*4096 * 2];


void printArgumentErrorMsg(const int testNum);
/* Should only ever be called once (should be before calls to other
   "sequentialThreadNum" functions.) We sacrifce some generallity here as the
   other "sequentialThreadNum" functions would work with more then one data
   structure as they don't use a static veriable like we do here. We do this
   because we are constrained in our options with the language (we can't assigne
   initial values to members of a struct that are not const (or maybe static) as
   far as we know.) That is why we choose to use a static veriable in this
   function. */
void initSequentialThreadNum(struct FuncArgs * args);
/* Prints the name of the test to be run. */
void printTestName(const int threadNum, const int testNum);
void printTestSize(const int testNum, int testSize, const int testSizes[],
		     const int fixedSizeAllocationUnit);
/* Sets testSize to capped value */
int getCappedTestSize();
/* Sets testSize to getCappedTestSize() and returns true if
   testSize > getCappedTestSize(). Otherwise returns false. */
bool capTestSize(int * testSize);
/* Should be called at the start of each test (as each test runs in it's own
   thread. Returns the thread num for that test.) */
size_t incSequentialThreadNum(struct FuncArgs * args);
/* Calls getStats() in intervals "iter % (testSizes[size] /
   saveStatOutputFrequency)" IndexOffset is used as an offset index into
   checkInInuseListP, chunksInHolesListP, avgInUseSzP and avgHoleSzP. This is
   needed for functions that call saveStat more then onces (Save stat would
   overrite what it already stored if used properly (with the exception of not
   using the offset properly.)) The base index into the afformentioned arguments
   is calculated as follows: (iter / (testSizes[size] / saveStatOutputFrequency) */
void saveStat(const int size, const bool listStat, const int iter,
		     const int indexOffset, double chunksInInUseListP[],
		     double chunksInHolesListP[], double avgInUseSzP[],
		     double avgHoleSzP[]);
/* Fills arr with fill. */
void fillArray(int * arr, const int size, const int fill);
/* Fills alloc Order with size number of 1's at random indecies. AllocOrder
   should be of size size * 2 */
void populateAllocOrder(int * allocOrder, const int size,
			const int allocOrderInUseVar);
/* Fills deallocOrder with indexes into allocOrder that equal 1. Where each new
   entry in deallocOrder must be unique and the entry must be a lower number
   then the index of deallocOrder that it is being inserted into. */
void  populateDeallocOrder(int * allocOrder, int * deallocOrder, const int size,
			   const int allocSize, const int deallocSize);
/* We want our maximum allocation unit to be the inverse of our test size. This
   function shouldn't be called from any functions with the moniker component
   "FixedSize" */
int setAllocationUnitMaxIndex(const int size);
/* Call pthread_join on all threads started in startThreads. */
void waitForThreads(const size_t threadCount, const pthread_t tids[]);
/* Prints results stored in 2nd and 3rd argument (indexed by first argument.) */
void printResults(const int testNum, const int testSize,
		  const int allocAndDeallocTest, const size_t threadCount,
		  const double allocationTime[],
		  const double deallocationTime[], const int listStats,
		  const struct Stats * stats);
void printTotalTime(const double totalTime);
void printAllocDeallocTotalTime(const double allocTime,
				  const double deallocTime);
void printListStats(const double chunksInInUseList[],
		    const double chunksInHolesList[],
		    const double avgInUseSz[], const double avgHoleSz[],
		    const int statPrintIndexLimit);
/* Tests. =================================================================== */
/* ========================================================================== */
/* All functions with the moniker component "FixedSize" use a constant
   allocation unit "allocationUnit". All functions without moniker component
   "FixedSize" use random allocation units. All "FixedSize" tests should be at
   even indexes in the function pointer array in main and everywhere else they
   are mentioned or used. This is because the functions used to print
   information about the tests rely on knowing their indexs. This is may not be
   an ideal secheme, however we have expaneded this program a lot and are under
   time constraints. */
/* Sequential allocations and deallocations. ================================ */
void * sequentialFixedSizeAllocationAndDeallocation(void * args);
void * sequentialAllocationAndDeallocation(void * args);
/* Sequential allocations and reverse deallocations.========================= */
/* We think that with a large hols list the order of deallocations will make a
   big difference to performance since we sort holes in mergeHoles() (to make it
   easier to compare addresses.) */
void * sequentialFixedSizeAllocationAndReverseDeallocation(void * args);
void * sequentialAllocationAndReverseDeallocation(void * args);
/* Interleaved allocations and deallocations. =============================== */
void * interleavedFixedSizeAllocationAndDeallocation(void * args);
void * interleavedAllocationAndDeallocation(void * args);
/* Random allocations and deallocations. ==================================== */
void * randomFixedSizeAllocationsAndDeallocations(void * args);
void * randomAllocationsAndDeallocations(void * args);


#endif
