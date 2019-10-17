#include <iostream>
#include <sstream>
#include <unistd.h>		// For brk and sbrk.
#include <type_traits>
#include "include/mm.hpp"
#include "include/algorithm.hpp"
#include "include/firstReaders.hpp"
#include "include/error.hpp"


namespace mmState
{
  std::forward_list<chunk *> inUse {};
  std::forward_list<chunk *> holes {};
  
  namespace locking
  {/* This semaphore is shared between readers and writers it is initialised
      and passed to the Readers class in the function init(). It should only
      be directly accessed in other functions where it is being used for writer
      code. */
    sem_t rwMutex;
    pthread_mutex_t chunkLock;
    std::vector<address> chunksLocked {};
    pthread_mutex_t freeingLock;
  }
  
  Readers readers {};
}


static_assert(std::is_pod<mmState::chunk>::value, "Fatal error: Struct \"chunk"
	      "\" was found not to be a POD\n");


constexpr size_t chunkAccountingSize {sizeof(mmState::chunk)};
// First things first we must make sure "chunk" is a POD!
void * (* allocAlgo)(const size_t chunk_size) {_firstFit};


void * _firstFit(const size_t chunk_size)
{
  using namespace mmState;

  /* Note that there are multiple exit points for this critical section where
     readers.exitCritical() is called.  */
  std::cout<<"in _firstFit() "<<pthread_self()<<std::endl;

 TRY_AGAIN:
  readers.enterCritical();
  
  checkZeroChunkSize(chunk_size);
  
  for(auto candidate {holes.before_begin()};
      std::next(candidate) != holes.cend(); ++candidate)
    { /* We will need to add new accounting info when we split the chunk so it
	 must have space for it. */

      if(((*std::next(candidate))->size) >= (chunk_size + chunkAccountingSize))
	{
	  //mmState::address addressLocked1, addressLocked2;
	  mmState::address addressLocked;
	  if(!tryLockThisAddress(std::next(candidate), holes.cend(),
				 addressLocked))
	    {		  // This chunk is already locked.
	      ++candidate;
	      continue;
	    }
	  /*	  if(!tryLockThisAndNextAddress(candidate, addressLocked1,
					addressLocked2))
	    {		  // This chunk is already locked.
	      ++candidate;
	      continue;
	      }*/

	  std::cout<<"\tin first if\t"<<pthread_self()<<std::endl;
	  readers.exitCritical();
	  sem_wait(&locking::rwMutex); // Enter critical region for writer.
	  /*	  if(!tryFreeingLockPostAndUnlockTheseAddresses(addressLocked1,
		  addressLocked2))*/
	  if(!tryFreeingLockPostAndUnlockThisAddress(addressLocked))
	      goto TRY_AGAIN;
	  
	  auto ret = splitChunkFromHoles(chunk_size, candidate);
	  
	  /*unlockThisAddress(addressLocked1);
	    unlockThisAddress(addressLocked2);*/
	  unlockThisAddress(addressLocked);
	  sem_post(&locking::rwMutex); // Exit critical.
	  return ret;
	}
      else
	{			/* We dont split the chunk if it is equal in
				   size so we don't need any extra space. */
	  if(((*std::next(candidate))->size) == chunk_size)
	    {			// The chunk is exactly the right size :).
	      std::cout<<"\tin second if\t"<<pthread_self()<<std::endl;;
	      //mmState::address addressLocked1, addressLocked2;
	      mmState::address addressLocked;
	      if(!tryLockThisAddress(std::next(candidate), holes.cend(),
				     addressLocked))
		{		  // This chunk is already locked.
		  ++candidate;
		  continue;
		}
	      /*	      if(!tryLockThisAndNextAddress(candidate, addressLocked1,
					    addressLocked2))
		{		  // This chunk is already locked.
		  ++candidate;
		  continue;
		  }*/

	      readers.exitCritical();
	      sem_wait(&locking::rwMutex); // Enter critical region for writer.
	      /*if(!tryFreeingLockPostAndUnlockTheseAddresses(addressLocked1,
		addressLocked2))*/
	      if(!tryFreeingLockPostAndUnlockThisAddress(addressLocked))
		 goto TRY_AGAIN;

	      auto ret = useChunkFromHoles(candidate);
	      
	      /*unlockThisAddress(addressLocked1);
		unlockThisAddress(addressLocked2);*/
	      unlockThisAddress(addressLocked);
	      sem_post(&locking::rwMutex); // Exit critical.
	      return ret;
	    }
	}
    }
  
  // Holes was empty or we didn't find a large enough chunk
  readers.exitCritical();
  sem_wait(&locking::rwMutex);

  if(!tryFreeingLockPost())
    goto TRY_AGAIN;
  /*  if(pthread_mutex_trylock(&locking::freeingLock) != 0)
    {
      sem_post(&locking::rwMutex);
      goto TRY_AGAIN;
      }*/
  
  auto ret = getNewChunkFromSystem(chunk_size);
  sem_post(&locking::rwMutex);
  return ret;
}


void * _bestFit(const size_t chunk_size)
{
  using namespace mmState;

  checkZeroChunkSize(chunk_size);
  
  if(!holes.empty())
    {
      if(std::next(holes.begin()) == holes.cend())
	{			// There is only one hole in the holes list.
	  auto ret = handleOneHole(chunk_size);
	  if(ret != nullptr)
	    return ret;
	}
      else
	{
	  auto bestFit {holes.before_begin()};
	  bool foundBestFit {false};
	  for(auto candidate {holes.before_begin()};
	      std::next(candidate) != holes.cend(); ++candidate)
	    {
	      if(((*std::next(candidate))->size) == chunk_size)
		{		// Must be best fit.
		  return useChunkFromHoles(candidate);
		}
	      else
		{
		  if(((*std::next(candidate))->size) >=
		     (chunk_size + chunkAccountingSize))
		    {	// We have found a hole large enough.
		      if(foundBestFit)
			{
			  bestFit = (((*std::next(candidate))->size) <
				     (*bestFit)->size) ?
			    candidate: bestFit;
			}
		      else
			{
			  foundBestFit = true;
			  bestFit = candidate;
			}
		    }
		}
	    }
	  if(foundBestFit)
	    {		// A best fit (not exact size) was found.
	      return splitChunkFromHoles(chunk_size, bestFit);
	    }
	}
    }
  // Holes was empty or no hole of large enough size was found.
  return getNewChunkFromSystem(chunk_size);
}


void * _worstFit(const size_t chunk_size)
{
  using namespace mmState;

  checkZeroChunkSize(chunk_size);

  if(!holes.empty())
    {
      if(std::next(holes.begin()) == holes.cend())
	{			// There is only one hole in the holes list.
	  auto ret = handleOneHole(chunk_size);
	  if(ret != nullptr)
	    return ret;
	}
      else
	{
	  auto worstFit {holes.before_begin()};
	  /* When we don't find a worst fit (we use + chunkAccountingSize) we
	     may use equal. */
	  auto equal {holes.before_begin()};
	  bool foundWorstFit {false};
	  for(auto candidate {holes.before_begin()};
	      std::next(candidate) != holes.cend(); ++candidate)
	    {
	      if(((*std::next(candidate))->size) == chunk_size && equal ==
		 holes.before_begin())
		{		// Must be best fit.
		  equal = candidate;
		}
	      else
		{
		  if(((*std::next(candidate))->size) >=
		     (chunk_size + chunkAccountingSize))
		    {	// We have found a hole large enough.
		      if(foundWorstFit)
			{
			  worstFit = (((*std::next(candidate))->size) >
				      (*worstFit)->size) ?
			    candidate: worstFit;
			}
		      else
			{
			  foundWorstFit = true;
			  worstFit = candidate;
			}
		    }
		}
	    }
	  if(foundWorstFit)
	    {		// A best fit (not exact size) was found.
	      return splitChunkFromHoles(chunk_size, worstFit);
	    }
	  else
	    if(equal != holes.before_begin())
	      {
		return useChunkFromHoles(equal);
	      }
	}
    }

  // Holes was empty or no hole of large enough size was found.
  return getNewChunkFromSystem(chunk_size);
}


/*inline bool tryFreeingLockPostAndUnlockTheseAddresses(const mmState::address
						   addressLocked1,
						   const mmState::address
						   addressLocked2)
{
  if(pthread_mutex_trylock(&mmState::locking::freeingLock) != 0)
    {			// free() is in it's writer critical section.
      sem_post(&mmState::locking::rwMutex);
      unlockThisAddress(addressLocked1);
      unlockThisAddress(addressLocked2);
      return false;	/* Keep trying, free() will (should ;) ) leave
			   it's critical section evenutally. */
//    }
  /* We don't want to keep other threads not in free() from running. */
/*  pthread_mutex_unlock(&mmState::locking::freeingLock);
  return true;
}*/


inline bool tryFreeingLockPostAndUnlockThisAddress(const mmState::address
						      addressLocked)
{
  if(pthread_mutex_trylock(&mmState::locking::freeingLock) != 0)
    {			// free() is in it's writer critical section.
      sem_post(&mmState::locking::rwMutex);
      unlockThisAddress(addressLocked);
      return false;	/* Keep trying, free() will (should ;) ) leave
			   it's critical section evenutally. */
    }
  /* We don't want to keep other threads not in free() from running. */
  pthread_mutex_unlock(&mmState::locking::freeingLock);
  return true;
}


inline bool tryFreeingLockPost()
{
  if(pthread_mutex_trylock(&mmState::locking::freeingLock) != 0)
    {				// free() is in it's writer critical section.
      sem_post(&mmState::locking::rwMutex);
      return false;		/* Keep trying, free() will (should ;) ) leave
				   it's critical section eveuntually. */
    }
  /* We don't want to keep other threads not in free() from running. */
  pthread_mutex_unlock(&mmState::locking::freeingLock);
  return true;
}


inline void checkZeroChunkSize(const size_t chunk_size)
{
  if(chunk_size == 0)
    {
      error::genError(error::ALLOC, "Fatal error: can't allocate 0 bytes.\n");
    }
}


inline void * handleOneHole(const size_t chunk_size)
{
  using namespace mmState;
  if(((*holes.begin())->size) >= (chunk_size + chunkAccountingSize))
    {		/* We have found a chunk but it is too big. There is more work
		   to be done. */
      return splitChunkFromHoles(chunk_size, holes.before_begin());
    }
  if(((*holes.begin())->size) == chunk_size)
    {		// The chunk is exactly the right size :).
      return useChunkFromHoles(holes.before_begin());
    }

  return nullptr;
}


template <typename T> inline void * splitChunkFromHoles(const size_t chunk_size,
							T candidate)
{
  using namespace mmState;

  std::cout<<"in splitChunkFromHoles()\n";
  
  // Base address of chunk to be returned.
  auto ret = (*std::next(candidate))->base;
  // CALCULATE FOR NEW HOLES CHUNK----------------------------------------------
  {						// New chunk.
    // Base address of new chunk to be put in holes.
    auto newBase ((char *)(ret) + chunk_size + chunkAccountingSize);

    /* Set new chunk's base address accounting info to the base address of new
       chunk. */
    ((chunk *)((char *)(ret) + chunk_size))->base = newBase;
    // Set new chunk's size accounting info to the the size of the new chunk.
    ((chunk *)((char *)(ret) + chunk_size))->size =
      (*std::next(candidate))->size - (chunkAccountingSize + chunk_size);
  }

  // UPDATE HOLES AND INUSE-----------------------------------------------------
  // Set new size of chunk to be returned.
  (*std::next(candidate))->size = chunk_size;
  // Push new hole onto inUse list.
  inUse.push_front(*std::next(candidate));
  // Remove old hole from holes list.
  holes.erase_after(candidate);
  // Add new hole to holes list.
  holes.push_front((chunk *)((char*)ret + chunk_size));
  
  return ret;
}


template <typename T> inline void * useChunkFromHoles(T candidate)
{
  using namespace mmState;

  std::cout<<"in useChunkFromHoles()\n";
  
  auto ret = (*std::next(candidate))->base;
  
  inUse.push_front(*std::next(candidate));
  holes.erase_after(candidate);
  
  return ret;
}


inline void * getNewChunkFromSystem(const size_t chunk_size)
{
  using namespace mmState;


  std::cout<<"in getNewChunkFromSystem()\n";

  // Get new chunk (plus memory for accounting.)
  address virtualChunk {address(sbrk(chunk_size + chunkAccountingSize))};
  
  if(virtualChunk == (address)(error::SBRK))
    {				// The allocation wasn't successfull :'(.
      std::cerr<<"Error in _firstFit: ";
      throw std::bad_alloc();
    }
  
  // Store base address of virtual chunk.
  ((chunk *)(virtualChunk))->base =
    ((char *)virtualChunk + chunkAccountingSize);
  // Store length of virtual chunk.
  ((chunk *)(virtualChunk))->size = chunk_size;
  // Put new chunk accounting info on the inUse list.
  inUse.push_front((chunk *)(virtualChunk));
  
  return ((chunk *)(virtualChunk))->base;
}


void free(const void * chunk)
{
  using namespace mmState;


  std::cout<<"in free("<<chunk<<")\n"<<pthread_self()<<'\n';

 TRY_AGAIN:
  readers.enterCritical();
    
  for(auto candidate {inUse.before_begin()};
      std::next(candidate) != inUse.cend(); ++candidate)
    {
      if((*std::next(candidate))->base == chunk)
	{
	  mmState::address addressLocked;
	  while(!tryLockThisAddress(std::next(candidate), inUse.cend(),
				    addressLocked)) {}

	  readers.exitCritical();
	  sem_wait(&locking::rwMutex);	// Enter critical region for writer.

	  // Try to signal that we are using all of holes.
	  if(pthread_mutex_trylock(&locking::freeingLock) != 0)
	    {
	      sem_post(&locking::rwMutex);
	      goto TRY_AGAIN;
	    }
	    
	  holes.push_front(*std::next(candidate));
	  inUse.erase_after(candidate);
	  mergeHoles();
	  mergeHoles();

	  pthread_mutex_unlock(&locking::freeingLock);
	  sem_post(&locking::rwMutex); // Exit critical region for writer.

	  std::cout<<"freed("<<*candidate<<") "<<pthread_self()<<'\n';
	  
	  return;
	}
    }  

  /* We do this here and not in dealloc for perfomance reasons (we would have to
     have a separate test in dealloc.) */
  error::genError(error::FREE, "Fatal error: invalid address (", chunk, ") "
		  "passed to free() (via dealloc.)\n");
}


inline void mergeHoles()
{
  using namespace mmState;

  std::cout<<"in mergeHoles\n";
  
  /* We must make sure that holes is sorted (it may be more efficient to do this
     differently.) */
  holes.sort(holeComp);
  
  if(std::next(holes.begin()) == holes.cend())
    return;			// There is only one hole in the list.
  else
    {
      for(auto candidate {holes.begin()};
	  std::next(candidate) != holes.cend(); )
	{
	  if(holeAbuttedAgainstHole(*candidate, *std::next(candidate)))
	    {			// Resize lower hole.
	      (*candidate)->size +=
		((*std::next(candidate))->size + chunkAccountingSize);
	      // Remove higher hole from holes list :).
	      candidate = holes.erase_after(candidate);
	      /* A merge should only be possible after inserting a new node, so
		 no more then two merges should even need to be done (i.e.
		 mergeHoles() should be called twice.) */
	      break;
	    }
	  ++candidate;
	}
    }
}



inline bool holeComp(mmState::chunk * a, mmState::chunk * b)
{				// Sort from low to high.
  return a->base < b->base ? true : false;
}


inline bool holeAbuttedAgainstHole(mmState::chunk * a, mmState::chunk * b)
{
  return (((char *)(a->base) + a->size) ==
	  ((char *)(b->base) - chunkAccountingSize)) ? true : false;
}


inline void unlockThisAddress(const mmState::address lockedAddress)
{
  pthread_mutex_lock(&mmState::locking::chunkLock);

  for(auto lockedChunkIter {mmState::locking::chunksLocked.begin()};
      lockedChunkIter != mmState::locking::chunksLocked.cend(); ++lockedChunkIter)
    {
      if(*lockedChunkIter == lockedAddress)
	{			// We have found a match. Remove it.
	  mmState::locking::chunksLocked.erase(lockedChunkIter);
	  pthread_mutex_unlock(&mmState::locking::chunkLock);
	  return;
	}
    }

  error::genError(error::ELEMENT_NOT_FOUND, "Error (in unlockThisAddress() "
		  "in algorithm.cpp): unlockThisAddress called with an invalid "
		  "address (", lockedAddress, ").\n");
}


void initMM()
{
  using namespace mmState;
  
  int pshared {}, mInitVal {1};
  if(sem_init(&locking::rwMutex, pshared, mInitVal) != 0)
  {
    error::genError(error::INIT, "Error (in intMM() in algorithm.cpp): "
		    "initialisation of rwMutex failed.\n");
  }
  if(pthread_mutex_init(&locking::chunkLock, NULL) != 0)
    {
      error::genError(error::INIT, "Error (in initMM() in algorithm.cpp): "
		      "initialistion of chunkLock failed.\n");
    }
  if(pthread_mutex_init(&locking::freeingLock, NULL) != 0)
    {
      error::genError(error::INIT, "Error (in initMM() in algorithm.cpp): "
		      "initialisation of freeingLock failed.\n");
    }

  readers.setRwMutex(&locking::rwMutex);
}


bool setAllocationAlgorithm(const allocationAlgorithm algo)
{
  switch(algo)
    {
    case firstFit:
      allocAlgo = _firstFit;
      break;
    case bestFit:
      allocAlgo = _bestFit;
      break;
    case worstFit:
      allocAlgo = _worstFit;
      break;
    default:
      return false;		// algo out of range :'(.
    }
      
  return true;
}


void getStats(double * chunksInInUseListP, double * chunksInHolesListP,
	      double * avgInUseSzP, double * avgHoleSzP)
{
  using namespace mmState;

  int inUseSz {}, holesSz {};
  double totalHoleSz {}, totalInUseSz {};

  for(auto hole {holes.begin()}; hole != holes.cend(); ++hole)
    {
      ++holesSz;
      totalHoleSz += (*hole)->size;
    }
  *chunksInHolesListP = holesSz; // Save current size of holes list.
  // Save current average size of holes on holes list.
  *avgHoleSzP = totalHoleSz /= holesSz;
  for(auto chunkUsing {inUse.begin()}; chunkUsing != inUse.cend(); ++chunkUsing)
    {
      ++inUseSz;
      totalInUseSz += (*chunkUsing)->size;
    }
  *chunksInInUseListP = inUseSz; // Save current size of inUse list.
  // Save current average size of chunks on inUse list.
  *avgInUseSzP = totalInUseSz /= inUseSz;
}
