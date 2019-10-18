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
  /* Number of test for which we print alloc and dealloc numbers */
  const int allocAndDeallocTest = 4;

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
	/* Print info about test to be run. */
	printTestName(atoi(argv[argExplicitConcurrencyN]),
		      atoi(argv[argTestNum]));
	printTestSize(atoi(argv[argTestNum]), atoi(argv[argTestSize]),
		      testSizes, fixedSizeAllocationUnit);

	pthread_t tids[maxThreadCount]; /* Store TIds here for join. */
	initSequentialThreadNum(&funcArgs);
	/* Perform initial initialisation before using the memory manager. */
	initMM();
	
	for(int iter = 0; iter < atoi(argv[argExplicitConcurrencyN]); ++iter)
	  {			/* Spin off new threads */
	    funcArgs.size = atoi(argv[argTestSize]);
	    funcArgs.listStat = listStat;
	    
	    int pthreadRet;
	    pthread_mutex_t messageLock;

	    
	    //	    test[atoi(argv[argTestNum])](&funcArgs);
	    pthreadRet = pthread_create(&tids[iter], NULL,
					test[atoi(argv[argTestNum])],
					&funcArgs);
	    if(pthreadRet)
	      {
		pthread_mutex_lock(&messageLock);
		fprintf(stderr, "Error:\tFailed to create thread: %i, "
			"aboring!\n", pthreadRet);
		pthread_mutex_unlock(&messageLock);
		exit(ERROR_THREAD_CREATION);
	      }
	  }
	waitForThreads(atoi(argv[argExplicitConcurrencyN]), tids);
	printResults(atoi(argv[argTestNum]), allocAndDeallocTest,
		     atoi(argv[argExplicitConcurrencyN]), allocationTime,
		     deallocationTime, atoi(argv[argListStat]), &stats);
      }
      else
	{
	  printf("Error:\tmalformed arguments, policy and or testNum and or\n\t"
		 "sizeNum and or explicitConcurrencyNumber and or listStat\n\t"
		 "not in range.\n");
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
  printf("Error:\tmalformed arguments, the format is:\n\t\"command policy "
	 "testNum sizeNum explicitConcurrencyNumber listStat\"\n\tWhere command"
	 " is the name of the program, policy is the memory allocation policy\n"
	 "\t(the range is [0, %i]), testNum is the number of the test (the "
	 "range is [0,%i]),\n\tsizeNum is the number of allocations to perform "
	 "in the test (the range is [0,%lu]),\n\texplicitConcurrencyNumber is "
	 "the number of threads the test will be run in (the\n\trange is [%i, "
	 "%i]) and listStat is a value of '1' or '0', where '1' will cause\n\t"
	 "the program to output extra information about the \"inUse\" and \""
	 "holes\" lists at\n\tthe cost of the accuracy of the other timing "
	 "measurments. A value of '0' will\n\tcause the program to run as usual"
	 " without outputting extra info about the lists\n\tand without the "
	 "loss in accuracy.\n",
	 maxPolicyNum, testNum -1, (sizeof(testSizes) / sizeof(int)) -1,
	 minThreadCount, maxThreadCount);
}


void initSequentialThreadNum(struct FuncArgs * args)
{
  static bool initialized = false;
  if(initialized == true)
    {
      fprintf(stderr, "Error: initSequentialThreadNum() called more then once, "
	      "exiting!\n");
      exit(ERROR_THREAD_INIT);
    }
  else
    {
      initialized = true;
      args->sequentialThreadNum = -1; /* We index from 0. */
    }
}


void printTestName(const int threadNum, const int testNum)
{
  const char tests[][60] = {"sequentialFixedSizeAllocationAndDeallocation()",
			    "sequentialAllocationAndDeallocation()",
			    "sequentialFixedSizeAllocationAndReverseDeallocation()",
			    "sequentialAllocationAndReverseDeallocation()",
			    "interleavedFixedSizeAllocationAndDeallocation()",
			    "interleavedAllocationAndDeallocation()",
			    "randomFixedSizeAllocationsAndDeallocations()",
			    "randomAllocationsAndDeallocations()"};
  printf("Starting battery of %i test\\s of type \"%s\"\n",
	 threadNum,
    tests[testNum]);
}


void printTestSize(const int testNum, int testSize, const int testSizes[],
		   const int fixedSizeAllocationUnit)
{

  if(testNum % 2 == 1)
    {
      if(capTestSize(&testSize))
	printf("\nSize to large for this test...\tCapping test size!");
      printf("\nTest size\t\t= %i\n", testSizes[testSize]);
      printf("Allocation unit\t\t= %i\n\n",
	     allocationUnitMax[setAllocationUnitMaxIndex(testSize)]);
    }
  else
    {
      printf("\nTest size\t\t= %i\n", testSizes[testSize]);
      printf("Allocation unit\t\t= %i\n\n", fixedSizeAllocationUnit);
    }
}


int getCappedTestSize()
{
  /* We don't want to allow test sizes of more then this index because we only
     have so much time and memory. */
  return 4;
}


bool capTestSize(int * testSize)
{
  if(*testSize > getCappedTestSize())
    {
      *testSize = getCappedTestSize();
      return true;
    }
  return false;
}


size_t incSequentialThreadNum(struct FuncArgs * args)
{
  size_t ret;
  pthread_mutex_lock(&args->sequentialThreadNumMutex);
  ret = ++args->sequentialThreadNum;
  pthread_mutex_unlock(&args->sequentialThreadNumMutex);
  return ret;
}


inline void saveStat(const int size, const bool listStat, const int iter,
		     const int indexOffset, double chunksInInUseListP[],
			double chunksInHolesListP[], double avgInUseSzP[],
			double avgHoleSzP[])
{
  if(listStat)
    { 
      if(iter % (testSizes[size] / saveStatOutputFrequency) == 0)
	{
	  printf("\t\tHey\n");
	  const int index = iter / (testSizes[size] /
				    saveStatOutputFrequency) + indexOffset;
	  getStats(&chunksInInUseListP[index], &chunksInHolesListP[index],
		   &avgInUseSzP[index], &avgHoleSzP[index]);
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


void waitForThreads(const size_t threadCount, const pthread_t tids[])
{
  for(size_t iter = 0; iter < threadCount; ++iter)
    {			/* Wait for threads to return. */
      int joinRet;
      int * pJoinRet = &joinRet;
      pthread_join(tids[iter], (void **)(&pJoinRet));
      printf("\t\t\tjoined thread!!!\n");
      /* Compile results here? */
    }
}


void printResults(const int testNum, const int allocAndDeallocTest,
		  const size_t threadCount, const double allocationTime[],
		  const double deallocationTime[], const int listStats,
		  const struct Stats * stats)
{
  for(size_t iter = 0; iter < threadCount; ++iter)
    {			/* Output results. */
      printf("\n\n\n==========================================================="
	     "=====================\nResults for thread\t%lu:\n================"
	     "================================================================",
	     iter);
      if(testNum >= allocAndDeallocTest)
	printTotalTime(allocationTime[iter]);
      else
	printAllocDeallocTotalTime(allocationTime[iter],
				   deallocationTime[iter]);

      if(listStats == listStatTrue)
	{
	  const int firstOutputSize = 2, secondOutputSize = 1,
	    thirdOutputSize = 3;
	  /* FirstOutputMagic is the only truely "magic" number we are using.
	     We use it because we are one short on output for these cases and we
	     do not have time to figour out exactly why sadly :'(.
	     NormalOutputMagic is just to compensate for off by 1's in the
	     arithmetic. */
	  const int firstOutputMagic = 2, normalOutputMagic = 1; 
	  int statPrintIndexLimit;
	  switch(testNum)
	    {			/* The first 4 tests call saveStat() twice. */
	    case 0:
	    case 1:
	    case 2:
	    case 3:
	      statPrintIndexLimit = firstOutputSize * saveStatOutputFrequency +
		firstOutputMagic;
	      break;
	      /* The 4th and 5th tests call saveStat() once. */
	    case 4:
	    case 5:
	      statPrintIndexLimit = secondOutputSize * saveStatOutputFrequency +
		normalOutputMagic;
	      break;
	      /* Finaly the 6th and 7th tests call saveStat once but at 3 times
		 the granularity (aka 3 times as much.) */
	    case 6:
	    case 7:
	      statPrintIndexLimit = thirdOutputSize * saveStatOutputFrequency +
		normalOutputMagic;
	      break;
	      /* We should never reach this point because the value of testNum
		 should have already been validated! */
	    default:
	      fprintf(stderr, "Error:\tin printResults() in profiler.c testNum "
		      "out of range (%i), aborting!\n", testNum);
	      exit(ERROR_RANGE);
	    }

	  printf("statPrintIndexLimit = %i\n", statPrintIndexLimit);
	  printListStats(stats->chunksInInUseList[iter],
			 stats->chunksInHolesList[iter],
			 stats->avgInUseSz[iter], stats->avgHoleSz[iter],
			 statPrintIndexLimit);
	}
    }
}


void printTotalTime(const double totalTime)
{
  printf("\n\tTotal time\t\t= %f\n", totalTime);
}


void printAllocDeallocTotalTime(const double allocTime,
				const double deallocTime)
{
  printf("\n\tAllocation time\t\t= %f\n\tDeallocation time\t= %f"
	 "\n\tTotal time\t\t= %f\n", allocTime, deallocTime,
	 allocTime + deallocTime);
}


void printListStats(const double chunksInInUseList[],
		    const double chunksInHolesList[],
		    const double avgInUseSz[], const double avgHoleSz[],
		    const int statPrintIndexLimit)
{
  for(int iter = 0; iter < statPrintIndexLimit; ++iter)
    {
      printf("--------------------------------\n"
	     "\tChunks in \"in use\" list:\t%f\n"
	     "\tChunks in \"holes\" list:\t\t%f\n"
	     "\t\tAverage size of chunks in \"in use\" list:\t%f\n"
	     "\t\tAverage size of chunks in holes list:\t\t%f\n\n",
	     chunksInInUseList[iter],
	     chunksInHolesList[iter],
	     avgInUseSz[iter],
	     avgHoleSz[iter]);
    }
}


void * sequentialFixedSizeAllocationAndDeallocation(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n allocations.
      allocs[threadIndex][iter] = alloc(fixedSizeAllocationUnit);
      
      /* Some dummy data to make sure the compiler is not doing anything tricky
	 idk (although I highly doubt it.) */
      *(char *)(allocs[threadIndex][iter]) = iter;

      saveStat(args(args)->size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  printf("All allocation's passed?\n");
  
  clock_t end = clock();
  const double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  allocationTime[threadIndex] = timeAlloc; /* Save time for this thread */
  begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n deallocations.
      printf("\t\tIn dealloc for loop\n");
      dealloc(allocs[threadIndex][iter]);
      saveStat(args(args)->size, args(args)->listStat, iter,
	       saveStatOutputFrequency +1,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  end = clock();
  const double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  deallocationTime[threadIndex] = timeDealloc;

  return NULL;
}


void * sequentialAllocationAndDeallocation(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));  
  int size = args(args)->size;
  capTestSize(&size);
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[threadIndex][iter] = alloc(abs(rand() %
			       (allocationUnitMax[allocationUnitMaxIndex])) +
			   allocationUnitMin);
      *(char *)(allocs[threadIndex][iter]) = iter;
      saveStat(size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
  allocationTime[threadIndex] = timeAlloc;
  begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n deallocations.
      dealloc(allocs[threadIndex][iter]);
      saveStat(size, args(args)->listStat, iter,
	       saveStatOutputFrequency +1,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  deallocationTime[threadIndex] = timeDealloc;

  return NULL;
}


void * sequentialFixedSizeAllocationAndReverseDeallocation(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n allocations.
      allocs[threadIndex][iter] = alloc(fixedSizeAllocationUnit);
      *(char *)(allocs[threadIndex][iter]) = iter;
      saveStat(args(args)->size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
    allocationTime[threadIndex] = timeAlloc;
  begin = clock();
  
  for(int iter = (testSizes[args(args)->size] -1), statIter = 0; iter >= 0;
      --iter, ++statIter)
    {			// Make n deallocations.
      dealloc(allocs[threadIndex][iter]);
      saveStat(args(args)->size, args(args)->listStat, statIter,
	       saveStatOutputFrequency +1,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
  deallocationTime[threadIndex] = timeDealloc;

  return NULL;
}


void * sequentialAllocationAndReverseDeallocation(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));
  int size = args(args)->size;
  capTestSize(&size);
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[threadIndex][iter] = alloc(abs(rand() %
			       (allocationUnitMax[allocationUnitMaxIndex])) +
			   allocationUnitMin);
      *(char *)(allocs[threadIndex][iter]) = iter;
      saveStat(size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }
  
  clock_t end = clock();
  double timeAlloc = (double)(end - begin) / CLOCKS_PER_SEC;
    allocationTime[threadIndex] = timeAlloc;
  begin = clock();
  
  for(int iter = (testSizes[size] -1), statIter = 0; iter >= 0 ;
      --iter, ++statIter)
    {			// Make n deallocations.
      dealloc(allocs[threadIndex][iter]);
      saveStat(size, args(args)->listStat, statIter,
	       saveStatOutputFrequency +1,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  end = clock();
  double timeDealloc = (double)(end - begin) / CLOCKS_PER_SEC;
    deallocationTime[threadIndex] = timeDealloc;

  return NULL;
}


void * interleavedFixedSizeAllocationAndDeallocation(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));
  clock_t begin = clock();
  
  for(int iter = 0; iter < testSizes[args(args)->size]; ++iter)
    {			// Make n allocations.
      allocs[threadIndex][iter] = alloc(fixedSizeAllocationUnit);
      *(char *)(allocs[threadIndex][iter]) = iter;
      dealloc(allocs[threadIndex][iter]);
      saveStat(args(args)->size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  allocationTime[threadIndex] = time;

  return NULL;
}


void * interleavedAllocationAndDeallocation(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));
  int size = args(args)->size;
  capTestSize(&size);
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  clock_t begin = clock();

  for(int iter = 0; iter < testSizes[size]; ++iter)
    {			// Make n allocations.
      allocs[threadIndex][iter] = alloc(abs(rand() %
			       (allocationUnitMax[allocationUnitMaxIndex])) +
			   allocationUnitMin);

      *(char *)(allocs[threadIndex][iter]) = iter;
      dealloc(allocs[threadIndex][iter]);
      saveStat(size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  allocationTime[threadIndex] = time;

  return NULL;
}


void * randomFixedSizeAllocationsAndDeallocations(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));
  /* Fill ararys with this value initially. */
  const int fillVar = -1;
  /* AllocOrder indices that are in use equal this. */
  const int allocOrderInUseVar = 1;

	 printf("Calculating random allocation and deallocation sequence...\n");
  
  // Fill arrays with known value
  fillArray(allocOrder[threadIndex], testSizes[args(args)->size] * allocSize, fillVar);
  fillArray(deallocOrder[threadIndex], testSizes[args(args)->size] * deallocSize, fillVar);

  // Setup allocation order
  populateAllocOrder(allocOrder[threadIndex], testSizes[args(args)->size], allocOrderInUseVar);
  // Setup deallocation order.
  populateDeallocOrder(allocOrder[threadIndex], deallocOrder[threadIndex], testSizes[args(args)->size], allocSize,
		       deallocSize);

  clock_t begin = clock();

  for(int iter = 0; iter < (testSizes[args(args)->size] * deallocSize); ++iter)
    {
      if(iter < (testSizes[args(args)->size] * allocSize))
	{			// Perform allocation.
	  if(allocOrder[threadIndex][iter] == allocOrderInUseVar)
	    {
	      allocs[threadIndex][iter] = alloc(fixedSizeAllocationUnit);
	    }
	}
      if(deallocOrder[threadIndex][iter] != fillVar)
	{
	  dealloc(allocs[threadIndex][deallocOrder[threadIndex][iter]]);
	}
      saveStat(args(args)->size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  allocationTime[threadIndex] = time;

  return NULL;
}


void * randomAllocationsAndDeallocations(void * args)
{
  const size_t threadIndex = incSequentialThreadNum(args(args));
  int size = args(args)->size;
  capTestSize(&size);
  /* Fill ararys with this value initially. */
  const int fillVar = -1;
  /* AllocOrder indices that are in use equal this. */
  const int allocOrderInUseVar = 1;
  int allocationUnitMaxIndex = setAllocationUnitMaxIndex(size);
  
  printf("Calculating random allocation and deallocation sequence...\n");

  // Fill arrays with known value
  fillArray(allocOrder[threadIndex], testSizes[size] * allocSize, fillVar);
  fillArray(deallocOrder[threadIndex], testSizes[size] * deallocSize, fillVar);

  // Setup allocation order
  populateAllocOrder(allocOrder[threadIndex], testSizes[size], allocOrderInUseVar);
  // Setup deallocation order.
  populateDeallocOrder(allocOrder[threadIndex], deallocOrder[threadIndex], testSizes[size], allocSize,
		       deallocSize);

  clock_t begin = clock();


  for(int iter = 0; iter < (testSizes[size] * deallocSize); ++iter)
    {
      if(iter < (testSizes[size] * allocSize))
	{			// Perform allocation.
	  if(allocOrder[threadIndex][iter] == allocOrderInUseVar)
	    {
	      allocs[threadIndex][iter] = alloc(abs(rand() %
				       (allocationUnitMax[allocationUnitMaxIndex])) +
				       allocationUnitMin);
	    }
	}
      if(deallocOrder[threadIndex][iter] != fillVar)
	{
	  dealloc(allocs[threadIndex][deallocOrder[threadIndex][iter]]);
	}
      saveStat(size, args(args)->listStat, iter,
	       initialBaseIndexForStats,
	       stats.chunksInInUseList[threadIndex],
	       stats.chunksInHolesList[threadIndex],
	       stats.avgInUseSz[threadIndex], stats.avgHoleSz[threadIndex]);
    }

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;
  allocationTime[threadIndex] = time;

  return NULL;
}
