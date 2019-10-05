/* This header should be included by any program using this memeory manager.
   ONLY things that are needed by another program using this MM should be
   defined here (POLA.) Additionally only C11 compatible syntax should be used
   in this head file (with the exception of the statment  "extern "C"" and the
   associated braces (Each of the three lines that contain the aformentioned
   statment and associated braces has a comment that can be used by a script to
   identify and remove them (thus rendering the file appropriate to be included
   in C source files.)))*/
#ifndef MM_HPP_
#define MM_HPP_


#include <stddef.h>
#include <stdbool.h>		// We need this when used with C.

enum allocationAlgorithm
  {
    firstFit,
    bestFit,
    worstFit
  };


extern "C"			// ([-{REMOVE--ME}-])
{	// ([-{REMOVE--ME}-])
  /* Changes the threadPoolSize (1 by default.) This function must be called
     before any calls to alloc() or dealloc() if it is to be called at all. */
  void setThreadPoolSize(const size_t tPS);
  /* When called with an element from allocationAlgorithm sets the function
     pointer allocAlgo (decleared in algorithm.hpp) to the corresponding
     function and returns true. If the argument is not valid return's false. */
  bool setAllocationAlgorithm(const enum allocationAlgorithm algo);
  // This function is for testing purposes only.
  void printStats();
  /* Checks for a chunk (big enough to meet the allocation request) in holes
     (based on the currently set allocation strategy. The default being first
     fit.) If a chunk large enough is found and it is larger than what is
     requested the chunk will be split up (with the new chunk matching the
     requested chunk size being returned (and put on the inUse list.) The other
     chunk will be put back into holes and possibly be merged with other chunks
     if need be.) If no chunk of adequate size is found in holes or if holes is
     empty (the starting condition) then the operating system is asked for more
     memory via use of the brk() or sbrk() sys-call wrappers */
  void * alloc(const size_t chunk_size);
  /* Searches for chunk in inUse. If found remove from inUse and add to holes
     (merging with other chunks if need be.) If chunk is not found in inUse
     aborts program! */
  void dealloc(const void * chunk);
}	// ([-{REMOVE--ME}-])


#endif
