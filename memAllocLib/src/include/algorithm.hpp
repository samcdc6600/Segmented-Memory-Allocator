#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_
#include <cstddef>		// For size_t.
#include <forward_list>
#include <new>			// For bad_alloc.
#include <climits>


namespace mmState
{
  typedef void * address;
  struct chunk
  {
    address base;
    size_t size;
    /* This variable is used to signify that this chunk is being used
       exclusively by a writer in the memory manager. While we think it is
       somewhat wastfull to store an extra byte for each chunk for this purpose
       we choose to do so because it seems to be the simplest way to implement
       it. */
    bool locked;
  };
  extern std::forward_list<chunk *> inUse;
  extern std::forward_list<chunk *> holes;
}


extern void * (* allocAlgo)(const size_t chunk_size);

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


#endif
