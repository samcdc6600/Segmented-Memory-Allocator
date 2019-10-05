#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_
#include <cstddef>		// For size_t.
#include <forward_list>
#include <new>			// For bad_alloc.


namespace mmState
{
  typedef void * address;
  struct chunk
  {
    address base;
    size_t size;
  };
  extern std::forward_list<chunk *> inUse;
  extern std::forward_list<chunk *> holes;
}


extern void * (* allocAlgo)(const size_t chunk_size);

/* Spins off a number of threads with _firstFitPropper(), giving appropritate
   iterators for it's start and end values. */
void * _firstFit(const size_t chunk_size);
void * _bestFit(const size_t chunk_size);
void * _worstFit(const size_t chunk_size);
/* Exit's if chunk_size is zero. It doesn't make sense to return a brk value
since there may be holes. */
inline void checkZeroChunkSize(const size_t chunk_size);
// If we are allocating and there is only one hole.
inline void * handleOneHole(const size_t chunk_size);
/* Splits candidate into  */
template <typename T> inline void * splitChunkFromHoles(const size_t chunk_size,
							T candidate);
/* Moves candidate from holes to inUse and returns base address of candidate (it
   is assumed that the size of the chunk candidate has already been checked.) */
template <typename T> inline void * useChunkFromHoles(T candidate);
/* Allocates a chunk of chunk_size using sbrk() and put's it on the inUse list
and then returns base address. */
inline void * getNewChunkFromSystem(const size_t chunk_size);
void free(const void * chunk);
/* Merges adjacent holes. Should be called whenever a new holes is inserted into
   holes. E.g., at the end of free. Most importantly it should be called twice
   (because it will only ever merge two holes in a single call. */
inline void mergeHoles();
inline bool holeComp(mmState::chunk * a, mmState::chunk * b);
// Returns true if chunk a is adjacent to chunk b. Returns false otherwise.
inline bool holeAbuttedAgainstHole(mmState::chunk * a, mmState::chunk * b);


/* Performs first fit in the range (start, end). May be stopped part way through
   if another thread finds a fit first. */
template <typename T> inline void _firstFitProper(const size_t chunk_size,
						  T start)
{
  /*  using namespace mmState;

      checkZeroChunkSize(chunk_size);
  
      for(auto candidate {holes.before_begin()};
      std::next(candidate) != holes.cend(); ++candidate)
      {*/ /* We will need to add new accounting info when we split the chunk so it
	     must have space for it. */
  /*      if(((*std::next(candidate))->size) >= (chunk_size + chunkAccountingSize))
	  {	*/		/* We have found a chunk but it is too big.
				   There is more work to be done :'(. */
  /*	  return splitChunkFromHoles(chunk_size, candidate);
	  }
	  else
	  {	*/		/* We dont split the chunk if it is equal in
				   size so we don't need any extra space. */
  /*	  if(((*std::next(candidate))->size) == chunk_size)
	  {			// The chunk is exactly the right size :).
	  return useChunkFromHoles(candidate);
	  }
	  }
	  }

	  // Holes was empty or we didn't find a large enough chunk
	  return getNewChunkFromSystem(chunk_size);*/
}


#endif
