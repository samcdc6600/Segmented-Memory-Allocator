#include <stdio.h>
#include <stdlib.h>		/* srand, rand */
#include <time.h>
#include <pthread.h>
#include "include/mm.h"
#include "include/profiler.h"


int main(const int argc, const char **argv)
{
  srand(time(NULL));
  const int argcArgs = 6;		/* We must be passed exactly this many
					   arguments. */
  const int argPolicy = 1, argTestNum = 2, argTestSize = 3,
    argExplicitConcurrencyN = 4, argListStat = 5;
  const int listStatFalse = 0, listStatTrue = 1;
  /* Maximum number of threads that will be created (this does not include the
     initial thread.) */
  const size_t maxThreadCount = 4096, minThreadCount = 1; /* We always spin off
							     at least one
							     thread... */
  void * (*test[])(void *) = {
    sequentialFixedSizeAllocationAndDeallocation,
    sequentialAllocationAndDeallocation,
    sequentialFixedSizeAllocationAndReverseDeallocation,
    sequentialAllocationAndReverseDeallocation,
    interleavedFixedSizeAllocationAndDeallocation,
    interleavedAllocationAndDeallocation,
    randomFixedSizeAllocationsAndDeallocations,
    randomAllocationsAndDeallocations};
  const int testNum = (sizeof(test) / sizeof(void (*)(const int)));

  if(argc == argcArgs)
    {
      if((atoi(argv[argPolicy]) >= 0 && atoi(argv[argPolicy])
	  <= maxPolicyNum) &&
	 (atoi(argv[argTestNum]) >= 0  && atoi(argv[argTestNum]) < testNum) &&
	 (atoi(argv[argTestSize]) >= 0 && (unsigned)atoi(argv[argTestSize]) <
	  (sizeof(testSizes) / sizeof(int))) &&
	 (atoi(argv[argExplicitConcurrencyN]) >= (long)minThreadCount &&
	  atoi(argv[argExplicitConcurrencyN]) <= (long)maxThreadCount) &&
	 (atoi(argv[argListStat]) == listStatFalse ||
	  atoi(argv[argListStat]) == listStatTrue))
	{
	  bool listStat = false;
	  if(atoi(argv[argListStat]) == listStatTrue)
	    listStat = true;
	  initMM();		/* Initialize MM data structures. */
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

	pthread_t tids[maxThreadCount]; /* Store TIds here for join. */
	
	for(int iter = 0; iter < atoi(argv[argExplicitConcurrencyN]); ++iter)
	  {
	    funcArgs.size = atoi(argv[argTestSize]);
	    funcArgs.listStat = listStat;
	    
	    int pthreadRet;
	    pthread_mutex_t messageLock;
	    
	    pthreadRet = pthread_create(&tids[iter], NULL,
					test[atoi(argv[argTestNum])],
					&funcArgs);
	    if(pthreadRet)
	      {
		pthread_mutex_lock(&messageLock);
		fprintf(stderr, "Error:\tFailed to create thread: %i, "
			"aboring!\n", pthreadRet);
		pthread_mutex_unlock(&messageLock);
		
	      }
	  }
	for(int iter = 0; iter < atoi(argv[argExplicitConcurrencyN]); ++iter)
	  {
	    int joinRet;
	    int * pJoinRet = &joinRet;
	    pthread_join(tids[iter], (void **)(&pJoinRet));
	    /* Compile results here? */
	  }
      }
      else
	{
	  printf("Error:\tmalformed arguments, policy and or testNum and or\n\t"
		 "sizeNum and or explicitConcurrencyNumber and or listStat\n\t"
		 "not in range.\n");
	  printArgumentErrorMsg(testNum, minThreadCount, maxThreadCount);
	}
    }
  else
    {
      printArgumentErrorMsg(testNum, minThreadCount, maxThreadCount);
    }
}


void printArgumentErrorMsg(const int testNum, const size_t minThreadCount,
			   const size_t maxThreadCount)
{
  printf("Error:\tmalformed arguments, the format is:\n\t\"command policy "
	 "testNum sizeNum explicitConcurrencyNumber listStat\"\n\tWhere command"
	 " is the name of the program, policy is the memory allocation policy\n"
	 "\t(the range is [0, %i]), testNum is the number of the test (the "
	 "range is [0,%i]),\n\tsizeNum is the number of allocations to perform "
	 "in the test (the range is [0,%lu]),\n\texplicitConcurrencyNumber is "
	 "the number of threads the test will be run in (the\n\trange is [%lu, "
	 "%lu]) and listStat is a value of '1' or '0', where '1' will cause\n\t"
	 "the program to output extra information about the \"inUse\" and \""
	 "holes\" lists at\n\tthe cost of the accuracy of the other timing "
	 "measurments. A value of '0' will\n\tcause the program to run as usual"
	 " without outputting extra info about the lists\n\tand without the "
	 "loss in accuracy.\n",
	 maxPolicyNum, testNum -1, (sizeof(testSizes) / sizeof(int)) -1,
	 minThreadCount, maxThreadCount);
}


inline void listStatOut(const int size, const bool listStat, const int iter)
{
  if(listStat)
    {
      if(iter % (testSizes[size] / listStatOutputFrequency) == 0)
	{
	  printStats();
	}
    }
}


void fillArray(int * arr, const int size, const int fill)
{
  for(int iter = 0; iter < size; ++iter)
    {
      arr[iter] = fill;
    }
}


/* We think that this function is not actually nessecary after all and we could
   do away with allocOrder. However we have already written it and choose to
   continue to use it (for now.) */
void populateAllocOrder(int * allocOrder, const int size,
			const int allocOrderInUseVar)
{
  /* Time steps are discrete and relate directly to indexes into allocOrder and
     deallocOrder. Populate allocOrder with random allocations. */
  /* How many future allocations have we assigned so far. */
  int i = 0, allocs = 0;
  while(allocs < size)
    {
      if((rand() % 3) == 0)
	{
	  if(allocOrder[i] == -1)
	    {
	      allocOrder[i] = allocOrderInUseVar;
	      ++allocs;
	    }
	}
	
      ++i;
      if(i == (size * 2))
	i = 0;		/* Cycle back around to the first index. */
    }
}


void  populateDeallocOrder(int * allocOrder, int * deallocOrder, const int size,
			   const int allocSize, const int deallocSize)
{
  /* Populate deallocOrder with random deallocations (with the restriction that
     each dealloction must follow it's corresponding allocation in allocOrder
     and no two deallocations can refere to the same allocation. */
  int i = 0, deallocs = 0;
  while(deallocs < size)
    {
    again:
      if((rand() % 3) == 0)
	{
	  if(deallocOrder[i] == -1)
	    {
	      while(true)
		{
		  int index = abs(rand()) % (size * allocSize);
		  if(index > i || allocOrder[index] == -1)
		    {
		      goto again;
		    }

		  bool alreadyAllocated = false;
		  for(int iter = 0; iter < (size * deallocSize) &&
			!alreadyAllocated; ++iter)
		    {
		      if(deallocOrder[iter] == index)
			alreadyAllocated = true;
		    }
		  if(alreadyAllocated)
		    {
		      continue;
		    }

		  deallocOrder[i] = index;
		  break;
		}
	      ++deallocs;
	    }
	}
      ++i;
      if(i == (size * deallocSize))
	i = 0;		/* Cycle back around to the first index. */
    }
}


/* This very much need's to be improved!!! */
int setAllocationUnitMaxIndex(const int size)
{
  switch(size)
    {				/* We are seting allocationUnitMaxIndex to the
				   number furthest away in the possible range
				   (if you conside the range to be circular. We
				   need to figoure out a better way to do this
				   :'( .) */
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
      printf("Error in setAllocationUnitMaxIndex(), no case for size (%i) in "
	     "switch.\n", size);
      exit(-1);
    }
}


void * sequentialFixedSizeAllocationAndDeallocation(void * args)
{
  printf("In sequentialFixedSizeAllocationAndDeallocation():\nTest size = %i,\n"
	 "Allocation unit = %i.\nStats:\n", testSizes[args(args)->size],
	 fixedSizeAllocationUnit);
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(fixedSizeAllocationUnit);
      /* Some dummy data to make sure the compiler is not doing anything tricky
	 idk (although I highly doubt it.) */
      *(char *)(allocs[iter]) = iter;
      listStatOut(args(args)->size, args(args)->listStat, iter);
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tAllocation time = %f\n\n", timeAlloc);
  begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
      listStatOut(args(args)->size, args(args)->listStat, iter);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n\n", timeAlloc + timeDealloc);

  return NULL;
}


void * sequentialAllocationAndDeallocation(void * args)
{
  int size = args(args)->size;
  if(size > veriableAllocationUnitTestSizesSaturationIndex)
    { /* We don't want to wast all of our time and memory. */
      size = veriableAllocationUnitTestSizesSaturationIndex;
      printf("Capping allocation size at %i\n", size);
    }
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  printf("In sequentialAllocationAndDeallocation():\nTest size = %i,\n"
	 "Allocation unit = %i.\nStats:\n", testSizes[size],
	 allocationUnitMax[allocationUnitMaxIndex]);
  
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(abs(rand() %
			       (allocationUnitMax[allocationUnitMaxIndex])) +
			   allocationUnitMin);
      *(char *)(allocs[iter]) = iter;
      listStatOut(size, args(args)->listStat, iter);
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tAllocation time = %f\n\n", timeAlloc);
  begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
      listStatOut(size, args(args)->listStat, iter);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n\n", timeAlloc + timeDealloc);

  return NULL;
}


void * sequentialFixedSizeAllocationAndReverseDeallocation(void * args)
{
  printf("In sequentialFixedSizeAllocationAndReverseDeallocation():\nTest size "
	 "= %i,\nAllocation unit = %i.\nStats:\n", testSizes[args(args)->size],
	 fixedSizeAllocationUnit);
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(fixedSizeAllocationUnit);
      *(char *)(allocs[iter]) = iter;
      listStatOut(args(args)->size, args(args)->listStat, iter);
    }

  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tAllocation time = %f\n\n", timeAlloc);
  begin = clock();
  
  for(int iter = (testSizes[args(args)->size] -1); iter >= 0; --iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
      listStatOut(args(args)->size, args(args)->listStat, iter);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n\n", timeAlloc + timeDealloc);

  return NULL;
}


void * sequentialAllocationAndReverseDeallocation(void * args)
{
  int size = args(args)->size;
  if(size > veriableAllocationUnitTestSizesSaturationIndex)
    { /* We don't want to wast all of our time and memory. */
      size = veriableAllocationUnitTestSizesSaturationIndex;
      printf("Capping allocation size at %i\n", size);
    }
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  printf("In sequentialAllocationAndReverseDeallocation():\nTest size = %i,\n"
	 "Allocation unit = %i.\nStats:\n", testSizes[size],
	 allocationUnitMax[allocationUnitMaxIndex]);
    
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(abs(rand() %
			       (allocationUnitMax[allocationUnitMaxIndex])) +
			   allocationUnitMin);
      *(char *)(allocs[iter]) = iter;
      listStatOut(size, args(args)->listStat, iter);
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tAllocation time = %f\n\n", timeAlloc);
  begin = clock();
  
  for(int iter = (testSizes[size] -1); iter >= 0 ; --iter)
    {			// Make n deallocations.
      dealloc(allocs[iter]);
      listStatOut(size, args(args)->listStat, iter);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tDeallocation time = %f\n", timeDealloc);
  printf("\tTotal time = %f\n\n", timeAlloc + timeDealloc);

  return NULL;
}


void * interleavedFixedSizeAllocationAndDeallocation(void * args)
{
  printf("In interleavedFixedSizeAllocationAndDeallocation():\nTest size = %i,"
	 "\nAllocation unit = %i.\nStats:\n", testSizes[args(args)->size],
	 fixedSizeAllocationUnit);
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(fixedSizeAllocationUnit);
      *(char *)(allocs[iter]) = iter;
      dealloc(allocs[iter]);
      listStatOut(args(args)->size, args(args)->listStat, iter);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tTotal time = %f\n\n", time);

  return NULL;
}


void * interleavedAllocationAndDeallocation(void * args)
{
  int size = args(args)->size;
  if(size > veriableAllocationUnitTestSizesSaturationIndex)
    { /* We don't want to wast all of our time and memory. */
      size = veriableAllocationUnitTestSizesSaturationIndex;
      printf("Capping allocation size at %i\n", size);
    }
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  printf("In interleavedAllocationAndDeallocation():\nTest size = %i,\n"
	 "Allocation unit = %i.\nStats:\n"
	 , testSizes[size], allocationUnitMax[allocationUnitMaxIndex]);
    
  clock_t begin = clock();

  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[iter] = alloc(abs(rand() %
			       (allocationUnitMax[allocationUnitMaxIndex])) +
			   allocationUnitMin);
      *(char *)(allocs[iter]) = iter;
      dealloc(allocs[iter]);
      listStatOut(size, args(args)->listStat, iter);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tTotal time = %f\n\n", time);

  return NULL;
}


void * randomFixedSizeAllocationsAndDeallocations(void * args)
{
  /* Fill ararys with this value initially. */
  const int fillVar = -1;
  /* AllocOrder indices that are in use equal this. */
  const int allocOrderInUseVar = 1;

  printf("In randomFixedSizeAllocationsAndDeallocations():\nTest size = %i,\n"
	 "Allocation unit = %i.\n", testSizes[args(args)->size], fixedSizeAllocationUnit);
  printf("Calculating random allocation and deallocation sequence...\n");
  
  // Fill arrays with known value
  fillArray(allocOrder, testSizes[args(args)->size] * allocSize, fillVar);
  fillArray(deallocOrder, testSizes[args(args)->size] * deallocSize, fillVar);

  // Setup allocation order
  populateAllocOrder(allocOrder, testSizes[args(args)->size], allocOrderInUseVar);
  // Setup deallocation order.
  populateDeallocOrder(allocOrder, deallocOrder, testSizes[args(args)->size], allocSize,
		       deallocSize);

  printf("Done! Running test.\nStats:\n");
  clock_t begin = clock();


  for(int iter = 0; iter < (testSizes[args(args)->size] * deallocSize); ++iter)
    {
      if(iter < (testSizes[args(args)->size] * allocSize))
	{			// Perform allocation.
	  if(allocOrder[iter] == allocOrderInUseVar)
	    {
	      allocs[iter] = alloc(fixedSizeAllocationUnit);
	      //	      printf("allocated %p\n", allocs[iter]);
	    }
	}
      if(deallocOrder[iter] != fillVar)
	{
	  dealloc(allocs[deallocOrder[iter]]);
	  //	  printf("deallocated %p\n", allocs[deallocOrder[iter]]);
	}
      listStatOut(args(args)->size, args(args)->listStat, iter);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tTotal time = %f\n\n", time);

  return NULL;
}


void * randomAllocationsAndDeallocations(void * args)
{
  int size = args(args)->size;
  /* Fill ararys with this value initially. */
  const int fillVar = -1;
  /* AllocOrder indices that are in use equal this. */
  const int allocOrderInUseVar = 1;


  if(size > veriableAllocationUnitTestSizesSaturationIndex)
    { /* We don't want to wast all of our time and memory. */
      size = veriableAllocationUnitTestSizesSaturationIndex;
      printf("Capping allocation size at %i\n", size);
    }
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  

  printf("In randomAllocationsAndDeallocations():\nTest size = %i,\nAllocation "
	 "unit = %i.\n", testSizes[size],
	 allocationUnitMax[allocationUnitMaxIndex]);
  printf("Calculating random allocation and deallocation sequence...\n");

  // Fill arrays with known value
  fillArray(allocOrder, testSizes[size] * allocSize, fillVar);
  fillArray(deallocOrder, testSizes[size] * deallocSize, fillVar);

  // Setup allocation order
  populateAllocOrder(allocOrder, testSizes[size], allocOrderInUseVar);
  // Setup deallocation order.
  populateDeallocOrder(allocOrder, deallocOrder, testSizes[size], allocSize,
		       deallocSize);

  printf("Done! Running test.\nStats:\n");
  clock_t begin = clock();


  for(int iter = 0; iter < (testSizes[size] * deallocSize); ++iter)
    {
      if(iter < (testSizes[size] * allocSize))
	{			// Perform allocation.
	  if(allocOrder[iter] == allocOrderInUseVar)
	    {
	      allocs[iter] = alloc(abs(rand() %
				       (allocationUnitMax[allocationUnitMaxIndex])) +
				   allocationUnitMin);
	    }
	}
      if(deallocOrder[iter] != fillVar)
	{
	  dealloc(allocs[deallocOrder[iter]]);
	}
      listStatOut(size, args(args)->listStat, iter);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\tTotal time = %f\n\n", time);

  return NULL;
}
