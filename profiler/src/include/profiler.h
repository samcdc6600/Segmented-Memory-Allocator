#ifndef PROFILER_H_
#define PROFILER_H_


struct FuncArgs
{
  int size;
  bool listStat;
} funcArgs;


const int maxPolicyNum = 2;	/* There are only three policies */
/* Number of allocations to make for tests. (Maximum is less for veriable sized
   allocation tests because of memory constraints.) */
const int testSizes[] = {1024*1, 1024*4, 1024*16, 1024*64, 1024*256, 1024*1024,
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
/* Used by all tests */
/* Store pointers returned from actual allocation. */
void * allocs[1024*4096 * 2];
const int listStatOutputFrequency = 9; /* How many times per allocation or
					  deallocation loop should printStats be
					  called? Will be called this many times
					  (plus 1) if allocation and
					  deallocation are in the same loop,
					  twice as many otherwise.) */


void printArgumentErrorMsg(const int testNum, const size_t minThreadCount,
			   const size_t maxThreadCount);
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
void sequentialFixedSizeAllocationAndDeallocation(const int size,
						  const bool listStat);
void sequentialAllocationAndDeallocation(int size, const bool listStat);
/* Sequential allocations and reverse deallocations.========================= */
/* We think that with a large hols list the order of deallocations will make a
   big difference to performance since we sort holes in mergeHoles() (to make it
   easier to compare addresses.) */
void sequentialFixedSizeAllocationAndReverseDeallocation(const int size,
							 const bool listStat);
void sequentialAllocationAndReverseDeallocation(int size, const bool listStat);
/* Interleaved allocations and deallocations. =============================== */
void interleavedFixedSizeAllocationAndDeallocation(const int size,
						   const bool listStat);
void interleavedAllocationAndDeallocation(int size, const bool listStat);
/* Random allocations and deallocations. ==================================== */
void randomFixedSizeAllocationsAndDeallocations(const int size,
						const bool listStat);
void randomAllocationsAndDeallocations(int size, const bool listStat);


#endif
