#include <stdio.h>
#include <stdlib.h>		/* srand, rand */
#include <time.h>
#include "include/mm.h"


const int maxPolicyNum = 2;	/* There are only three policies */
/* Number of allocations to make for tests. (Maximum is less for veriable sized allocation tests because of memory
   constraints.) */
const int testSizes[] = {1024*1, 1024*4, 1024*16, 1024*64, 1024*256, 1024*1024, 1024*4096};
const int fixedSizeAllocationUnit = 1; /* For tests with fixed alloction unit size. */
/* We don't want to allow test sizes of more then this index because we only have so much time and memory. */
const int veriableAllocationUnitTestSizesSaturationIndex = 4;
/* Ranges for test with veriable allocation unit sizes. */
const int allocationUnitMin = 1, allocationUnitMax[] = {1024*1, 1024*4, 1024*16, 1024*64, 1024*256};


void printArgumentErrorMsg(const int testNum);
/* All functions with moniker component "FixedSize" use a constant allocation unit "allocationUnit" */
void sequentialFixedSizeAllocationAndDeallocation(const int size);
/* We think that with a large hols list the order of deallocations will make a big difference to performance since we
   sort holes in mergeHoles() (to make it easier to compare addresses.) */
void sequentialFixedSizeAllocationAndReverseDeallocation(const int size);
void interleavedFixedSizeAllocationAndDeallocation(const int size);
void randomFixedSizeAllocationsAndDeallocations(const int size);
/* All functions without moniker component "FixedSize" use random allocation units */
void sequentialAllocationAndDeallocation(const int size);
void sequentialAllocationAndReverseDeallocation(const int size);
void interleavedAllocationAndDeallocation(const int size);
void randomAllocationsAndDeallocations(const int size);
/* We want our maximum allocation unit to be the inverse of our test size. This function shouldn't be called from
   any functions with the moniker component "FixedSize" */
int setAllocationUnitMaxIndex(const int size);


int main(const int argc, const char **argv)
{
  srand(time(NULL));
  const int argcArgs = 4;		/* We must be passed exactly this many arguments. */
  const int argPolicy = 1, argTestNum = 2, argTestSize = 3;
  void (*test[])(const int size) = {sequentialFixedSizeAllocationAndDeallocation,
				    sequentialFixedSizeAllocationAndReverseDeallocation,
				    interleavedFixedSizeAllocationAndDeallocation, randomFixedSizeAllocationsAndDeallocations,
				    sequentialAllocationAndDeallocation, sequentialAllocationAndReverseDeallocation,
				    interleavedAllocationAndDeallocation, randomAllocationsAndDeallocations};
  const int testNum = (sizeof(test) / sizeof(void (*)(const int)));

  if(argc == argcArgs)
    {
      if((atoi(argv[argPolicy]) >= 0 && atoi(argv[argPolicy]) <= maxPolicyNum) &&
	 (atoi(argv[argTestNum]) >= 0  && atoi(argv[argTestNum]) < testNum) &&
	 (atoi(argv[argTestSize]) >= 0 && (unsigned)atoi(argv[argTestSize]) < (sizeof(testSizes) / sizeof(int))))
	{
	  switch(atoi(argv[argPolicy]))
	    {
	    case 0:
	      printf("Using first fit.\n");
	      setAllocationAlgorithm(firstFit);
	      break;
	    case 1:
	      printf("Using best fit.\n");
	      setAllocationAlgorithm(bestFit);
	      break;
	    case 2:
	      printf("Using worst fit.\n");
	      setAllocationAlgorithm(worstFit);
	      break;
	    }
	  test[atoi(argv[argTestNum])](atoi(argv[argTestSize]));
	}
      else
	{
	  printf("Error malformed arguments, testNum or sizeNum not in range.\n");
	  printArgumentErrorMsg(testNum);
	}
    }
  else
    {
      printArgumentErrorMsg(testNum);
    }
}


void printArgumentErrorMsg(const int testNum)
{
  printf("Error malformed arguments, the format is: command policy testNum sizeNum\nWhere "
	 "command is the name of the program, policy is the memory allocation policy\n"
	 "(the range is [0, %i]), testNum is the number of the test (the range is [0,%i])\n"
	 "and sizeNum is the number of allocations to perform in the test (the range is [0,%lu].)\n",
	 maxPolicyNum, testNum -1, (sizeof(testSizes) / sizeof(int)) -1);
}


/* Causes seg faults when allocated declared in a func and alloc() used after it. Maybe something to do with stack
   limit's. But it seems odd because we only get the seg faults when alloc is called in the same func. */
void * allocs[4096*16*16*16*16];
void sequentialFixedSizeAllocationAndDeallocation(const int size)
{
  printf("In sequentialFixedSizeAllocationAndDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
	 , testSizes[size], fixedSizeAllocationUnit);
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(fixedSizeAllocationUnit);
      /* Some dummy data to make sure the compiler is not doing anything tricky idk (although I highly doubt it.) */
      *(char *)(allocs[iter]) = iter;
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tAllocation time = %f\n", timeAlloc);
  begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n", timeAlloc + timeDealloc);
}


void sequentialFixedSizeAllocationAndReverseDeallocation(const int size)
{
  const int fixedSizeAllocationUnit = 1;
  printf("In sequentialFixedSizeAllocationAndReverseDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
	 , testSizes[size], fixedSizeAllocationUnit);
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(1);
      *(char *)(allocs[iter]) = iter;
    }

  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tAllocation time = %f\n", timeAlloc);
  begin = clock();
  
  for(int iter = (testSizes[size] -1); iter >= 0; --iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n", timeAlloc + timeDealloc);
}


void interleavedFixedSizeAllocationAndDeallocation(const int size)
{
  const int fixedSizeAllocationUnit = 1;
  printf("In interleavedFixedSizeAllocationAndDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
	 , testSizes[size], fixedSizeAllocationUnit);
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(1);
      *(char *)(allocs[iter]) = iter;
      dealloc(allocs[iter]);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tTotal time = %f\n", time);
}


void randomFixedSizeAllocationsAndDeallocations(const int size)
{
  int allocOrder[10 * 2];
  int deallocOrder[10 * 2];

  for(int iter = 0; iter < (10 * 2); ++iter)
    {				/* Mark elements with known value. */
      allocOrder[iter] = -1;
      deallocOrder[iter] = -1;
    }

  /* Time steps are discrete and relate directly to indexes into allocOrder and deallocOrder. */
  {				/* Populate allocOrder with random allocations. */
    int i = 0, allocs = 0; /* How many future allocations have we assigned so far. */
    while(allocs < 10)
      {
	if((rand() % 3) == 0)
	  {
	    if(allocOrder[i] == -1)
	      {
		allocOrder[i] = 1;
		++allocs;
	      }
	  }
	
	++i;
	if(i == (10 * 2))
	  i = 0;		/* Cycle back around to the first index. */
      }
  }
  {				/* Populate deallocOrder with random deallocations (with the restriction that each
				   dealloction must follow it's corresponding allocation in allocOrder and no two
				   deallocations can refere to the same allocation. */
    int i = 0, deallocs = 0;
    while(deallocs < 10)
      {
      asdf:
	if((rand() % 3) == 0)
	  {
	    if(deallocOrder[i] == -1)
	      {
		  while(true)
		    {
		      int index = abs(rand()) % (10 * 2);
		      if(index > i || allocOrder[index] == -1)
			goto asdf;

		      bool alreadyAllocated = false;
		      for(int iter = 0; iter < (10 * 2) && !alreadyAllocated; ++iter)
			{
			  if(deallocOrder[iter] == index)
			    alreadyAllocated = true;
			}
		      if(alreadyAllocated)
			continue;

		      deallocOrder[i] = index;
		      break;
		    }
		++deallocs;
	      }
	  }

	++i;
	if(i == (10 * 2))
	  i = 0;		/* Cycle back around to the first index. */
      }
  }


    for(int iter = 0; iter < (10 * 2); ++iter)
    {				/* Mark elements with known value. */
      printf("alloc = %i, dealloc = %i\n", allocOrder[iter], deallocOrder[iter]);
    }
    printf("\n");
    for(int iter = 0; iter < (10 * 2); ++iter)
    {				/* Mark elements with known value. */
      if(deallocOrder[iter] != -1)
	printf("alloc (indexed by dealloc) = %i\n", allocOrder[deallocOrder[iter]]);
    }

  
  
  /* 
     const int maxPolicyNum = 2;	// There are only three policies
     // Number of allocations to make for tests. (Maximum is less for veriable sized allocation tests because of memory
     //   constraints.)
     const int testSizes[] = {1024*1, 1024*4, 1024*16, 1024*64, 1024*256, 1024*1024, 1024*4096};
     const int fixedSizeAllocationUnit = 1; // For tests with fixed alloction unit size.
     // We don't want to allow test sizes of more then this index because we only have so much time and memory.
     const int veriableAllocationUnitTestSizesSaturationIndex = 4;
     // Ranges for test with veriable allocation unit sizes.
     const int allocationUnitMin = 1, allocationUnitMax[] = {1024*1, 1024*4, 1024*16, 1024*64, 1024*256};
  */
}


void sequentialAllocationAndDeallocation(int size)
{
  if(size > veriableAllocationUnitTestSizesSaturationIndex)
    {
      size = veriableAllocationUnitTestSizesSaturationIndex; /* We don't want to wast all of our time and memory. */
      printf("Capping allocation size at %i\n", size);
    }
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  printf("In sequentialAllocationAndDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
	 , testSizes[size], allocationUnitMax[allocationUnitMaxIndex]);
  
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(abs(rand() % (allocationUnitMax[allocationUnitMaxIndex])) + allocationUnitMin);
      *(char *)(allocs[iter]) = iter;
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tAllocation time = %f\n", timeAlloc);
  begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n", timeAlloc + timeDealloc);
}


void sequentialAllocationAndReverseDeallocation(int size)
{
  if(size > veriableAllocationUnitTestSizesSaturationIndex)
    {
      size = veriableAllocationUnitTestSizesSaturationIndex; /* We don't want to wast all of our time and memory. */
      printf("Capping allocation size at %i\n", size);
    }
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  printf("In sequentialAllocationAndReverseDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
	 , testSizes[size], allocationUnitMax[allocationUnitMaxIndex]);
    
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(abs(rand() % (allocationUnitMax[allocationUnitMaxIndex])) + allocationUnitMin);
      *(char *)(allocs[iter]) = iter;
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tAllocation time = %f\n", timeAlloc);
  begin = clock();
  
  for(int iter = (testSizes[size] -1); iter >= 0 ; --iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n", timeAlloc + timeDealloc);
}


void interleavedAllocationAndDeallocation(int size)
{
  if(size > veriableAllocationUnitTestSizesSaturationIndex)
    {
      size = veriableAllocationUnitTestSizesSaturationIndex; /* We don't want to wast all of our time and memory. */
      printf("Capping allocation size at %i\n", size);
    }
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  printf("In interleavedAllocationAndDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
	 , testSizes[size], allocationUnitMax[allocationUnitMaxIndex]);
    
  clock_t begin = clock();

  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(1);
      *(char *)(allocs[iter]) = iter;
      dealloc(allocs[iter]);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\tTotal time = %f\n", time);
}


void randomAllocationsAndDeallocations(int size)
{
  printf("Please implement me :'(\n");
}


/* This very much need's to be improved!!! */
int setAllocationUnitMaxIndex(const int size)
{
  switch(size)
    {				/* We are seting allocationUnitMaxIndex to the number furthest away in the possible
				   range (if you conside the range to be circular. We need to figoure out a better
				   way to do this :'( .) */
    case 0:
      return 4;
      break;
    case 1:
      return 3;
      break;
    case 2:
      return 2;
      break;
    case 3:
      return 1;
      break;
    case 4:
      return 0;
      break;
    default:
      printf("Error in setAllocationUnitMaxIndex(), no case for size (%i) in switch.\n", size);
      exit(-1);
    }
}
