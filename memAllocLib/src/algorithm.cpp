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
  std::list<chunk *> inUse {};
  std::list<chunk *> holes {};
  
  namespace locking
  {/* This semaphore is shared between readers and writers it is initialised
      and passed to the allocsReaders object in the function initMM(). It should
      only be directly accessed in the alloc functions where it is being used
      for writer code. */
    sem_t rwSem;
    /* This semaphore is shared between readers and writers and like the last
       variable it is initialised and passed to an object (freeVsAllocsReaders)
       in intMM(). Should only be used in free(). */
    sem_t freeVsAllocsRwSem;
    // This is used to make rw ops on chunk.locked atomic.
    pthread_mutex_t chunkLock;
  }

  /* allocReaders should be used only within the allocaion
     algorithms and not free(). freeVsAllocsReaders is used along with 
     freeVsAllocsRwSem to implement readers writer locking on the allocation
     algorithms and free(), where free() is the reader. */
  Readers allocsReaders {}, freeVsAllocsReaders {}; // IDK :)
}


// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
pthread_mutex_t printLock {}; // TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP


static_assert(std::is_pod<mmState::chunk>::value, "Fatal error: Struct \"chunk"
	      "\" was found not to be a POD\n");


constexpr size_t chunkAccountingSize {sizeof(mmState::chunk)};
// First things first we must make sure "chunk" is a POD!
void * (* allocAlgo)(const size_t chunk_size) {_firstFit};


void * _firstFit(const size_t chunk_size)
{
  //  std::cout<<"In first fit\n";
  using namespace mmState;
  /* Note that there are multiple exit points for this critical section where
     readers.exitCritical() is called.  */
  freeVsAllocsReaders.enterCritical();

  //   TRY_AGAIN:  
  allocsReaders.enterCritical();
  checkZeroChunkSize(chunk_size);
  
  for(auto candidate {holes.begin()};
      candidate != holes.cend(); ++candidate)
    { /* We will need to add new accounting info when we split the chunk so it
	 must have space for it. */
      if((*candidate)->size >= (chunk_size + chunkAccountingSize))
	{
	  if(!chunkLockTestAndSet(*candidate))
	    {
	      //std::cout<<"trying again in ChunkLockTestAndSet\n";
	      //	      allocsReaders.exitCritical();
	      //	      goto TRY_AGAIN;
	      continue;
	    }
	    
	  allocsReaders.exitCritical();
	  
	  sem_wait(&locking::rwSem); // Enter critical region for writer.
	  setChunkLockFalse(*candidate);
	  auto ret = splitChunkFromHoles(chunk_size, candidate);
	  sem_post(&locking::rwSem); // Exit critical.

	  freeVsAllocsReaders.exitCritical();

	  return ret;
	}
      else
	{			/* We dont split the chunk if it is equal in
				   size so we don't need any extra space. */
	  if(((*candidate)->size) == chunk_size)
	    {			// The chunk is exactly the right size :).
	      if(!chunkLockTestAndSet(*candidate))
		{
		  //		  std::cout<<"trying again in next one\n";
		  //		  allocsReaders.exitCritical();
		  //		  goto TRY_AGAIN;
		  continue;
		}
		
	      allocsReaders.exitCritical();
	      
	      sem_wait(&locking::rwSem); // Enter critical region for writer.
	      setChunkLockFalse(*candidate);
	      auto ret = useChunkFromHoles(candidate);
	      sem_post(&locking::rwSem); // Exit critical.

	      freeVsAllocsReaders.exitCritical();

	      return ret;
	    }
	}
    }
  // Holes was empty or we didn't find a large enough chunk
  allocsReaders.exitCritical();
  sem_wait(&locking::rwSem);
  auto ret = getNewChunkFromSystem(chunk_size);
  sem_post(&locking::rwSem);

  
  freeVsAllocsReaders.exitCritical();

  /*  pthread_mutex_lock(&printLock);
  std::cout<<"exiting _firstFit after genNewChunkFromSystem()\t"<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);*/

  return ret;
}


void * _bestFit(const size_t chunk_size)
{
  using namespace mmState;

  freeVsAllocsReaders.enterCritical();
  TRY_AGAIN:
  allocsReaders.enterCritical();
  checkZeroChunkSize(chunk_size);
  std::cout<<"hello\n";
  if(!holes.empty())
    {
    if(std::next(holes.begin()) == holes.cend())
	{			// There is only one hole in the holes list.
	  	      std::cout<<"in fist if \n";
	  if(!chunkLockTestAndSet(*holes.begin()))
	    {
	      std::cout<<"in fist try again\n";
	      allocsReaders.exitCritical();
	      goto TRY_AGAIN;
	    }

	  allocsReaders.exitCritical();

	  std::cout<<"locking \n";
	  sem_wait(&locking::rwSem);
	  std::cout<<"<<<<<<<<<<<<<<<<<<<\n";
	  setChunkLockFalse(*holes.begin());
	  std::cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^6\n";
	  auto ret = handleOneHole(chunk_size);
	  std::cout<<"lkdsafjflk\n";
	  sem_post(&locking::rwSem);

	  freeVsAllocsReaders.exitCritical();
	  
	  if(ret != nullptr)
	  return ret;
	}
      else
	{
	  	  	      std::cout<<"in fist else \n";
	  auto bestFit {holes.begin()};
	  bool foundBestFit {false};
	  for(auto candidate {holes.begin()};
	      std::next(candidate) != holes.cend(); ++candidate)
    {
	      	  	  	      std::cout<<"in for else \n";
	      if(((*candidate)->size) == chunk_size)
		{		// Must be best fit.
		  if(!chunkLockTestAndSet(*candidate))
		    {
		      std::cout<<"in second try again\n";
		      // allocsReaders.exitCritical();
		      // goto TRY_AGAIN;
		      continue;
		    }

		  allocsReaders.exitCritical();

		  sem_wait(&locking::rwSem);
		  setChunkLockFalse(*candidate);
		  auto ret = useChunkFromHoles(candidate);
		  sem_post(&locking::rwSem);

		  freeVsAllocsReaders.exitCritical();
		  
		  return ret;
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
	      if(!chunkLockTestAndSet(*bestFit))
		    {
		      std::cout<<"in second try again\n";
		      allocsReaders.exitCritical();
		      goto TRY_AGAIN;
		    }
	      
	      allocsReaders.exitCritical();

	      sem_wait(&locking::rwSem);
	      setChunkLockFalse(*bestFit);
	      auto ret = splitChunkFromHoles(chunk_size, bestFit);
	      sem_post(&locking::rwSem);

	      freeVsAllocsReaders.exitCritical();
			  
	      return ret;
	    }
	}
    }
  std::cout<<"getting new chunk from system\n";
  freeVsAllocsReaders.exitCritical();
  // Holes was empty or no hole of large enough size was found.
  return getNewChunkFromSystem(chunk_size);
}


void * _worstFit(const size_t chunk_size)
{
  /*  using namespace mmState;

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
	auto worstFit {holes.before_begin()};*/
	  /* When we don't find a worst fit (we use + chunkAccountingSize) we
	     may use equal. */
  /*	  auto equal {holes.before_begin()};
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
  return getNewChunkFromSystem(chunk_size);*/
}


inline void checkZeroChunkSize(const size_t chunk_size)
{
  if(chunk_size == 0)
    {
      error::genError(error::ALLOC, "Fatal error: can't allocate 0 bytes.\n");
    }
}


/*inline bool checkChunkLock(const mmState::chunk * candidate)
{
  pthread_mutex_lock(&mmState::locking::chunkLock);
  bool ret {candidate->locked};
  return ret;
  pthread_mutex_unlock(&mmState::locking::chunkLock);
  }*/

inline bool chunkLockTestAndSet(mmState::chunk * candidate)
{
  /*      pthread_mutex_lock(&printLock);
  std::cout<<"exiting _firstFit \t"<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);*/
  pthread_mutex_lock(&mmState::locking::chunkLock);
  bool ret {false};
  if(!candidate->locked)	// Lock if not locked.
    {
      candidate->locked = true;	// :O
      ret = true;
    }
  pthread_mutex_unlock(&mmState::locking::chunkLock);
  return ret;
}

inline void setChunkLockFalse(mmState::chunk * victim)
{
  pthread_mutex_lock(&mmState::locking::chunkLock);
  victim->locked = false;
  pthread_mutex_unlock(&mmState::locking::chunkLock);
}

/*inline void setChunkLock(mmState::chunk * victim, const bool truth)
{
  pthread_mutex_lock(&mmState::locking::chunkLock);
  victim->locked = truth;	// :O
  pthread_mutex_unlock(&mmState::locking::chunkLock);
  }*/


inline void * handleOneHole(const size_t chunk_size)
{
  using namespace mmState;
  if(((*holes.begin())->size) >= (chunk_size + chunkAccountingSize))
    {		/* We have found a chunk but it is too big. There is more work
		   to be done. */
      return splitChunkFromHoles(chunk_size, holes.begin());
    }
  if(((*holes.begin())->size) == chunk_size)
    {		// The chunk is exactly the right size :).
      return useChunkFromHoles(holes.begin());
    }

  return nullptr;
}


template <typename T> inline void * splitChunkFromHoles(const size_t chunk_size,
							T candidate)
{
  using namespace mmState;

  /*  pthread_mutex_lock(&printLock);
  std::cout<<"in splitChunkFromHoles()\t"<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);*/
  
  // Base address of chunk to be returned.
  auto ret = (*candidate)->base;
  // CALCULATE FOR NEW HOLES CHUNK----------------------------------------------
  {						// New chunk.
    // Base address of new chunk to be put in holes.
    auto newBase ((char *)(ret) + chunk_size + chunkAccountingSize);

    /* Set new chunk's base address accounting info to the base address of new
       chunk. */
    ((chunk *)((char *)(ret) + chunk_size))->base = newBase;
    // Set new chunk's size accounting info to the the size of the new chunk.
    ((chunk *)((char *)(ret) + chunk_size))->size =
      (*candidate)->size - (chunkAccountingSize + chunk_size);
    // Set new chunk's locked var.
    ((chunk *)((char *)(ret) + chunk_size))->locked = false;
  }

  // UPDATE HOLES AND INUSE-----------------------------------------------------
  // Set new size of chunk to be returned.
  (*candidate)->size = chunk_size;
  // Push new hole onto inUse list.
  inUse.push_front(*candidate);
  // Remove old hole from holes list.
  holes.erase(candidate);
  // Add new hole to holes list.
  holes.push_front((chunk *)((char*)ret + chunk_size));

  /*  pthread_mutex_lock(&printLock);
  std::cout<<"exiting splitChunkFromHoles()\t"<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);*/
  
  return ret;
}


template <typename T> inline void * useChunkFromHoles(T candidate)
{
  using namespace mmState;
  
  auto ret = (*candidate)->base;
  
  inUse.push_front(*candidate);
  holes.erase(candidate);
  
  return ret;
}


inline void * getNewChunkFromSystem(const size_t chunk_size)
{
  using namespace mmState;


  /*  pthread_mutex_lock(&printLock);
  std::cout<<"in getNewChunkFromSystem()\t"<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);*/

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
  // Set lock var to false.
  ((chunk *)(virtualChunk))->locked = false;
  // Put new chunk accounting info on the inUse list.
  inUse.push_front((chunk *)(virtualChunk));
  
  return ((chunk *)(virtualChunk))->base;
}


void free(const void * chunk)
{
  using namespace mmState;

  constexpr long mergeRate {4096};

  sem_wait(&locking::freeVsAllocsRwSem);
    
  for(auto candidate {inUse.begin()};
      candidate != inUse.cend(); ++candidate)
    {
      if((*candidate)->base == chunk)
	{
	  holes.push_front(*candidate);
	  inUse.erase(candidate);

	  if(holes.size() > (mergeRate * inUse.size()))
	    { 
	      mergeHoles();
	      mergeHoles();
	    }

	  sem_post(&locking::freeVsAllocsRwSem);
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


  //  std::cout<<"hello \n";

  /*  pthread_mutex_lock(&printLock);
  std::cout<<"in mergeHoles\t"<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);*/
  
  /* We must make sure that holes is sorted (it may be more efficient to do this
     differently.) */
  holes.sort(holeComp);
  
  if(std::next(holes.begin()) == holes.cend())
    {
      /*      pthread_mutex_lock(&printLock);
      std::cout<<"returning from mergeHoles (only one hole was in list )\t"<<pthread_self()<<'\n';
      pthread_mutex_unlock(&printLock);*/
      return;			// There is only one hole in the list.
    }
  else
    {
      for(auto candidate {holes.begin()};
	  std::next(candidate) != holes.cend(); )
	{
	  if(holeAbuttedAgainstHole(*candidate, *std::next(candidate)))
	    {			// Resize lower hole.
	      (*candidate)->size +=
		((*std::next(candidate))->size + chunkAccountingSize);
	      /*	      if((*std::next(candidate))->locked)
		{
		  error::genError(-1, "candidate = locked\n");
		  }*/
	      // Remove higher hole from holes list :).
	      /*pthread_mutex_lock(&printLock);
	      std::cout<<"eraseing "<<*std::next(candidate)<<" in merge holes\t"<<pthread_self()<<'\n';
	      pthread_mutex_unlock(&printLock);*/
	      candidate = holes.erase(candidate);
	      /*pthread_mutex_lock(&printLock);
	      std::cout<<"erased previous "<<" in merge holes\t"<<pthread_self()<<'\n';
	      pthread_mutex_unlock(&printLock);*/
	      /* A merge should only be possible after inserting a new node, so
		 no more then two merges should even need to be done (i.e.
		 mergeHoles() should be called twice.) */
	      break;
	    }
	  ++candidate;
	}
    }

  /*  pthread_mutex_lock(&printLock);
  std::cout<<"exiting merge holes after erasure.\t"<<pthread_self()<<'\n';
  pthread_mutex_unlock(&printLock);*/
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


void initMM()
{
  using namespace mmState;
  
  int pshared {}, mInitVal {1};
  if(sem_init(&locking::rwSem, pshared, mInitVal) != 0)
  {
    error::genError(error::INIT, "Error (in intMM() in algorithm.cpp): "
		    "initialisation of rwSem failed.\n");
  }
  if(sem_init(&locking::freeVsAllocsRwSem, pshared, mInitVal) != 0)
    {
      error::genError(error::INIT, "Error (in initMM() in algorithm.cpp): "
		      "initialisation of freeVsAllocsRwSem failed.\n");
    }
  if(pthread_mutex_init(&locking::chunkLock, NULL))
    {
      error::genError(error::INIT, "Error (in initMM() in algorithm.cpp): "
		      "initialisation of chunkLock failed.\n");
    }
  // TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP
  // TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP
  // TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP
  // TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP
  // TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP
  pthread_mutex_init(&printLock, NULL); // TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP TMP  TMP

  allocsReaders.setRwMutex(&locking::rwSem);
  freeVsAllocsReaders.setRwMutex(&locking::freeVsAllocsRwSem);
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
