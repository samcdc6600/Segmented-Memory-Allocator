#ifndef PROFILER_H_
#define PROFILER_H_
#include <limits.h>


enum _error
  {
    ERROR_THREAD_CREATION = INT_MIN,
    ERROR_THREAD_INIT
  };


/* Usefull for function pointers passed to new threads with an argument with
   original type FuncArgs. */
#define args(X) ((struct FuncArgs * )X)


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
				 incSequentialThreadNum() and
				 getSequentialThreadNum(). This point is
				 critically important ;) */
  pthread_mutex_t sequentialThreadNumMutex;
} funcArgs;


const int maxPolicyNum = 2;	/* There are only three policies */
/* Number of allocations to make for tests. (Maximum is less for veriable sized
   allocation tests because of memory constraints.) */
const int testSizes[] = {16*1, 1024*4, 1024*16, 1024*64, 1024*256, 1024*1024,
			 1024*4096};
const int fixedSizeAllocationUnit = 1; /* For tests with fixed alloction unit
					  size. */
/* We don't want to allow test sizes of more then this index because we only
   have so much time and memory. */
const int veriableAllocationUnitTestSizesSaturationIndex = 4;
/* Ranges for test with veriable allocation unit sizes. */
const int allocationUnitMin = 1, allocationUnitMax[] = {1024*1, 1024*4, 1024*16,
							1024*64, 1024*256};
/* AllocSize, deallocSize, allocOrder and deallocOrder are all used by the
   functions randomFixedSizeAllocationsAndDeallocations() and
   randomAllocationsAndDeallocations() */
const int allocSize = 2, deallocSize = 3;
int allocOrder[1024*4096 * 2];
int deallocOrder[1024*4096 * 3];
/* Maximum number of threads that will be created (this does not include the
   initial thread.) */
#define maxThreadCount 128
#define minThreadCount 1 /* We always spin off at least one thread... */
/* Used by all tests */
/* Store pointers returned from actual allocation. */
void * allocs[maxThreadCount][1024*4096 * 2];
const int listStatOutputFrequency = 9; /* How many times per allocation or
					  deallocation loop should printStats be
					  called? Will be called this many times
					  (plus 1) if allocation and
					  deallocation are in the same loop,
					  twice as many otherwise.) */


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
/* Should be called at the start of each test (as each test runs in it's own
   thread.) */
void incSequentialThreadNum(struct FuncArgs * args);
/* Should be called to get index into allocs. */
size_t getSequentialThreadNum(struct FuncArgs * args);
/* Calls printStats() in intervals "iter % (testSizes[size] /
   listStatOutputFrequency)" */
inline void listStatOut(const int size, const bool listStat, const int iter);
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
/* Tests. =================================================================== */
/* ========================================================================== */
/* All functions with the moniker component "FixedSize" use a constant
   allocation unit "allocationUnit". All functions without moniker component
   "FixedSize" use random allocation units */
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
