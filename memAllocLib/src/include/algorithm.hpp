#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_
#include <cstddef>		// For size_t.
#include <forward_list>
#include <new>			// For bad_alloc.
#include <climits>
#include <semaphore.h>
#include <vector>
#include <forward_list>
#include "error.hpp"


// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP      |                                                                       
extern pthread_mutex_t printLock; // TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP     


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

  namespace locking
  {/* This semaphore is shared between readers and writers it is initialised
      and passed to the Readers class in the function init(). It should only
      be directly accessed in other functions where it is being used for writer
      code. */
    extern sem_t rwMutex;
    extern pthread_mutex_t iterLock;
    extern std::vector<std::_Fwd_list_iterator<mmState::chunk *>> itersLocked;
    /* Since free calls mergeHoles() which calls holes.sort() all iteraters to
       holes may be invalidated. Therfore we need some way of letting other
       threads know that we are in free. (It is unfortunate to have such a
       costly section of code locked exclusively. However we think it would be a
       lot of work to come up with and implement a better solution.)*/
    extern pthread_mutex_t freeingLock;
    extern bool freeing;
  }
}


extern void * (* allocAlgo)(const size_t chunk_size);

void * _firstFit(const size_t chunk_size);
void * _bestFit(const size_t chunk_size);
void * _worstFit(const size_t chunk_size);
/* Exit's if chunk_size is zero. It doesn't make sense to return a brk value
since there may be holes. */
inline void checkZeroChunkSize(const size_t chunk_size);



inline bool tryFreeingLockPostAndUnlockThisAddress();
inline bool tryFreeingLockPost();



//inline bool tryLockThisAndNext(mmState::chunk * thisChunk);
// If we are allocating and there is only one hole.
inline void * handleOneHole(const size_t chunk_size);
/* Splits candidate into two holes acording to value of chunk_size. */
template <typename T> inline void * splitChunkFromHoles(const size_t chunk_size,
							T candidate);
/* Moves candidate from holes to inUse and returns base address of candidate (it
   is assumed that the size of the chunk candidate has already been checked.) */
template <typename T> inline void * useChunkFromHoles(T candidate);
/* Sets (*thisChunk)->locked to true if it is false. Thread safe. */
//template<typename T> bool tryLockThis(T thisChunk);
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
inline void lockFreeing();


#endif
