#include <stdio.h>
#include <stdlib.h>		/* srand, rand */
#include <time.h>
#include "include/mm.h"

/* Number of allocations to make for tests. (Maximum is less for veriable sized allocation tests because of memory
   constraints.) */
const int testSizes[] = {1024*1, 1024*4, 1024*16, 1024*64, 1024*256, 1024*1024, 1024*4096};
const int fixedSizeAllocationUnit = 1; /* For tests with fixed alloction unit size. */
/* Ranges for test with veriable allocation unit sizes. */
const int allocationUnitMin = 1, allocationUnitMax[] = {1024*1, 1024*4};
const int testNum = 2;


void printArgumentErrorMsg();
/* All functions with moniker component "FixedSize" use a constant allocation unit "allocationUnit" */
void sequentialFixedSizeAllocationAndDeallocation(const int size);
/* We think that with a large hols list the order of deallocations will make a big difference to performance since we
   sort holes in mergeHoles() (to make it easier to compare addresses.) */
void sequentialFixedSizeAllocationAndReverseDeallocation(const int size);
void interleavedFixedSizeAllocationAndDeallocation(const int size);
void randomFixedSizeAllocations(const int size);
/* All functions without moniker component "FixedSize" use random allocation units */


int main(const int argc, const char **argv)
{
  //  srand(time(NULL));
  void (*test[])(const int size) = {sequentialFixedSizeAllocationAndDeallocation,
				    sequentialFixedSizeAllocationAndReverseDeallocation,
				    interleavedFixedSizeAllocationAndDeallocation, randomFixedSizeAllocations};

  if(argc == 3)
    {
      if((atoi(argv[1]) >= 0  && atoi(argv[1]) < testNum) &&
	 (atoi(argv[2]) >= 0 && (unsigned)atoi(argv[2]) < (sizeof(testSizes) / sizeof(int))))
	{
	  test[atoi(argv[1])](atoi(argv[2]));
	  /*	  switch(atoi(argv[1]))
	    {
	    case 0:
	      sequentialAllocationAndDeallocation(atoi(argv[2]));
	      break;
	    case 1:
	      sequentialAllocationAndReverseDeallocation(atoi(argv[2]));
	      break;
	      }*/
	}
      else
	{
	  printf("Error malformed arguments, testNum or sizeNum not in range.\n");
	  printArgumentErrorMsg();
	}
    }
  else
    {
      printArgumentErrorMsg();
    }
}


void printArgumentErrorMsg()
{
  printf("Error malformed arguments, the format is: command testNum sizeNum\nWhere "
	 "command is the name of the program, testNum is the number of the test\n"
	 "(the range is [0,%i]) and sizeNum is the number of allocations to perform\n"
	 "in the test (the range is [0,%lu].)\n", testNum, (sizeof(testSizes) / sizeof(int)));
}


/* Causes seg faults when allocated declared in a func and alloc() used after it. Maybe something to do with stack
   limit's. But it seems odd because we only get the seg faults when alloc is called in the same func. */
void * allocs[4096*16*16*16*16];
void sequentialFixedSizeAllocationAndDeallocation(const int size)
{
  printf("In sequentialAllocationAndDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
	 , testSizes[size], fixedSizeAllocationUnit);
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(fixedSizeAllocationUnit);
      /* Some dummy data to make sure the compiler is not doing anything tricky idk. */
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
  printf("In sequentialAllocationAndReverseDeallocation():\nTest size = %i,\nAllocation unit = %i.\nStats:\n"
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
}


void randomFixedSizeAllocations(const int size)
{				/* I.e. we allocate an absolute maximum of 256 MB (testSize * maxAllocSize.) */

  /*  int testSize = 5;//65536;
  int maxAllocSize = 4096;
  
  int sizes[testSize];
  int allocOrder[testSize * 2];
  int deallocOrder[testSize * 2];


  for(int iter = 0; iter < testSize; ++iter)*/
  /*    {*/				/* -1 means that we have not given that index a value. Since
				   all values for allocOrder are (rand() % X) they are never
				   negative and since all values for deallocOrder are indexes
				   into allocOrder they are also never negative. */
      /*      allocOrder[iter] = -1;
      deallocOrder[iter] = -1;
      }*/

  /* How many indexs in allocOrder have we assigned random numbers to? */
      /*  int allocOrderAllocs = 0;
  int allocOrderIndex = 0;
  while(allocOrderAllocs != testSize)
    {
      if(allocOrderIndex == (testSize *2))
	allocOrderIndex = 0;	/* Don't want to use an out of bounds index */
      //      if(rand() % 2)		/* Randomly select this index. */
	/*	{
	  if(allocOrder[allocOrderIndex] == -1)
	  { *//* If we have not already made an assignment to this index. */
	    /*	    allocOrder[allocOrderIndex] = rand() % maxAllocSize;
	    ++allocOrderAllocs;
	    ++allocOrderIndex;
	    continue;
	  }
	}
      ++allocOrderIndex;
    }

  for(int iter = 0; iter < (testSize*2); ++iter)
    {
      printf("iter = %i\n", allocOrder[iter]);
    }*/
}
