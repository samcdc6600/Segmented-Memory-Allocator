/* This header (and only this header) should be included by any program using this memeory manager.
   ONLY things that are needed by another program using this mm should be defined here (POLA.) */
#ifndef MM_HPP_
#define MM_HPP_


enum allocationAlgorithm
  {
    firstFit,
    bestFit,
    worstFit
  };



/* When called with an element from allocationAlgorithm sets the function pointer allocAlgo (decleared in
   algorithm.hpp) to the corresponding function and returns true. If the argument is not valid return's false. */
bool setAllocationAlgorithm(const allocationAlgorithm algo);
/* Checks for a chunk (big enough to meet the allocation request) in holes (based on the currently set allocation
   strategy. The default being first fit.) If a chunk large enough is found and it is larger than what is requested
   the chunk will be split up (with the new chunk matching the requested chunk size being returned (and put on the
   inUse list.) The other chunk will be put back into holes and possibly be merged with other chunks if need be.)
   If no chunk of adequate size is found in holes or if holes is empty (the starting condition) then the operating
   system is asked for more memory via use of the brk() or sbrk() sys-call wrappers */
void * alloc(const size_t chunk_size);
/* Searches for chunk in inUse. If found remove from inUse and add to holes (merging with other chunks if need be.)
   If chunk is not found in inUse aborts program! */
void dealloc(const void * chunk);


#endif
