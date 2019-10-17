#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_
#include <cstddef>		// For size_t.
#include <forward_list>
#include <new>			// For bad_alloc.
#include <climits>
#include <semaphore.h>
#include <vector>
#include <forward_list>


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
    //    bool locked;
  };
  
  extern std::forward_list<chunk *> inUse;
  extern std::forward_list<chunk *> holes;

  namespace locking
  {/* This semaphore is shared between readers and writers it is initialised
      and passed to the Readers class in the function init(). It should only
      be directly accessed in other functions where it is being used for writer
      code. */
    extern sem_t rwMutex;
    extern pthread_mutex_t chunkLock;
    extern std::vector<address> chunksLocked;
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
/*inline bool tryFreeingLockPostAndUnlockTheseAddresses(const mmState::address
						      addressLocked1,
						      const mmState::address
						      addressLocked2);*/
inline bool tryFreeingLockPostAndUnlockThisAddress(const mmState::address
						      addressLocked);
inline bool tryFreeingLockPost();
/* Executes "chunksLocked.push_back((*thisChunk)->base)" and
   "chunksLocked.push_back((*std::next(thisChunk))->base)" if neither of them
   are locked. */
template <typename T>
inline bool tryLockThisAndNextAddress(T thisChunk, mmState::address &lockedRet1,
				      mmState::address &lockedRet2);
/* If thisChunk is not locked adds thisChunk->base to chunksLocked (if thisChunk
   is != end, else adds nullptr to chunksLocked (to indicate that we are
   locking the empty list that thisChunk would be a member of if it was not null
   and the list was not empty :) )). */
template <typename T1, typename T2>
inline bool tryLockThisAddress(T1 thisChunk, const T2 end,
			       mmState::address & lockedRet);
inline void unlockThisAddress(const mmState::address lockedAddress);


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


template <typename T>
inline bool tryLockThisAndNextAddress(T thisChunk, mmState::address &lockedRet1,
				      mmState::address &lockedRet2)
{
  bool ret {true};                     // Indicate success / failure.

  pthread_mutex_lock(&mmState::locking::chunkLock);

  for(auto lockedChunk: mmState::locking::chunksLocked)
    {
      if(lockedChunk == (*thisChunk)->base ||
	 lockedChunk == (*std::next(thisChunk))->base)
	{			// The chunks we want are not both unlocked.
	  ret = false;
	  break;
	}
    }
  if(ret)
    {				// Chunks are both unlocked. Lock them.
      mmState::locking::chunksLocked.push_back((*thisChunk)->base);
      mmState::locking::chunksLocked.push_back((*std::next(thisChunk))->base);
    }
  
  pthread_mutex_unlock(&mmState::locking::chunkLock);

  lockedRet1 = (*thisChunk)->base;
  lockedRet2 = (*std::next(thisChunk))->base;
  return ret;
}


template <typename T1, typename T2>
inline bool tryLockThisAddress(T1 thisChunk, const T2 end,
			       mmState::address & lockedRet)
{
  bool ret {true};
  mmState::address lockAddress {nullptr};

  pthread_mutex_lock(&mmState::locking::chunkLock);

  if(thisChunk != end)
    {				/* ThisChunk points to a valid chunk */
      lockAddress = (*thisChunk)->base;
    }
 else
   {
     for(auto lockedChunk: mmState::locking::chunksLocked)
       {
	 if(lockedChunk == lockAddress)
	   {			// The chunk we want is not unlocked.
	     ret = false;
	     break;
	   }
       }
   }
  
  if(ret)
    {				// Chunk was unlocked. Lock it.
      mmState::locking::chunksLocked.push_back(lockAddress);
    }

  pthread_mutex_unlock(&mmState::locking::chunkLock);

  pthread_mutex_lock(&printLock);
  std::cout<<"lockAddress = "<<lockAddress<<" "<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);
  // Return address that was locked (so we can unlock it later.)
  lockedRet = lockAddress;
  return ret;
}


#endif
