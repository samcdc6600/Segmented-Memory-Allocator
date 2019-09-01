#include <stdio.h>
#include <stdlib.h>		/* srand, rand */
#include <time.h>
#include "include/mm.h"


const int testSizes[] = {4096, 4096*16, 4096*16*16, 4096*16*16*16, 4096*16*16*16*16};


/*  */
void genRandomAllocations();


int main()
{
  //  srand(time(NULL));


  setAllocationAlgorithm(worstFit);
  size_t allocSz = 20;
  int a = 1, b = 1;
  
  int * nums1;

  nums1 = ((int *)(alloc(allocSz * sizeof(int))));

  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      nums1[iter] = a * b;
      ++a, ++b;
    }
  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      printf("nums1[iter] = %i\n", nums1[iter]);
    }

   dealloc(nums1);

  allocSz = 10;

    int * nums2;
  nums2 = ((int *)(alloc(allocSz * sizeof(int))));
  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      nums2[iter] = a * b;
      ++a, ++b;
    }
  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      printf("nums2[iter] = %i\n", nums2[iter]);
    }

  allocSz = 8;

  int * nums3;
  nums3 = ((int *)(alloc(allocSz * sizeof(int))));
  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      nums3[iter] = a * b;
      ++a, ++b;
    }
  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      printf("nums3[iter] = %i\n", nums3[iter]);
    }
  

  dealloc(nums2);


  int * nums4;
  nums4 = ((int *)(alloc(allocSz * sizeof(int))));
  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      nums4[iter] = a * b;
      ++a, ++b;
    }
  for(size_t iter = 0; iter < allocSz; ++iter)
    {
      printf("nums4[iter] = %i\n", nums4[iter]);
    }

  //  dealloc(nums1);
    dealloc(nums4);
    dealloc(nums3);
  //  dealloc(nums3);
  
    /*  printf("hello world\n");
  printf("%i\n", 4096*16*16*16*16);
  genRandomAllocations();*/
}


void genRandomAllocations()
{				/* I.e. we allocate an absolute maximum of 256 MB (testSize * maxAllocSize.) */
  /*  int testSize = 6;//65536;
  int maxAllocSize = 4096;
  
  int sizes[testSize];
  int allocOrder[testSize * 2];
  int deallocOrder[testSize * 2];
  

  for(int iter = 0; iter < testSize; ++iter)
    {				/* -1 means that we have not given that index a value. Since all values for allocOrder are (rand() % X) they are
				   never negative and since all values for deallocOrder are indexes into allocOrder they are also never negative. 
      allocOrder[iter] = -1;
      deallocOrder[iter] = -1;
    }
  
  /*  for(int iter = 0; iter < testSize; ++iter)
    {				/* Generate randome allocation sizes 
      sizes[0] = rand() % maxAllocSize;
      }

  int allocs = 0;
  for(int iter = 0; iter < (testSize * 2); )
    {				/* Generate random sequence of allocatins and deallocations (with the restriction that we can't deallocate something
				   untill it has been allocated of course.) 
      allocOrder[iter] = rand() % maxAllocSize;
      while(true)
	{
	  int dealloc = rand() % (testSize * 2);

	  for(int inerIter = 0; inerIter < (testSize * 2); ++inerIter)
	    {
	      if(deallocOrder[inerIter] == dealloc)
		{
		  ++iter;
		  continue;
		}
	    }
	  deallocOrder[iter] = dealloc;
	  break;
	}
      ++iter;
      ++allocs;
      if(allocs == testSize)
	break;
    }

  for(int iter = 0; iter < (testSize * 2); ++iter)
    {
      printf("allocOrder = %i\t\t deallocOrder = %i\n", allocOrder[iter], deallocOrder[iter]);
      }*/


  int testSize = 5;//65536;
  int maxAllocSize = 4096;
  
  int sizes[testSize];
  int allocOrder[testSize * 2];
  int deallocOrder[testSize * 2];


  for(int iter = 0; iter < testSize; ++iter)
    {				/* -1 means that we have not given that index a value. Since
				   all values for allocOrder are (rand() % X) they are never
				   negative and since all values for deallocOrder are indexes
				   into allocOrder they are also never negative. */
      allocOrder[iter] = -1;
      deallocOrder[iter] = -1;
    }

  /* How many indexs in allocOrder have we assigned random numbers to? */
  int allocOrderAllocs = 0;
  int allocOrderIndex = 0;
  while(allocOrderAllocs != testSize)
    {
      if(allocOrderIndex == (testSize *2))
	allocOrderIndex = 0;	/* Don't want to use an out of bounds index */
      if(rand() % 2)		/* Randomly select this index. */
	{
	  if(allocOrder[allocOrderIndex] == -1)
	  { /* If we have not already made an assignment to this index. */
	    allocOrder[allocOrderIndex] = rand() % maxAllocSize;
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
    }
}
