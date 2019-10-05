#include <iostream>
#include <sstream>
#include <unistd.h>		// For brk and sbrk.
#include <type_traits>
#include <iterator>
#include "include/mm.hpp"
#include "include/algorithm.hpp"
#include "include/threads.hpp"
#include "include/error.hpp"


namespace mmState
{
  //  std::forward_list<chunk *> inUse {};
  //  std::forward_list<chunk *> holes {};
  typedef std::forward_list<chunk *> chunkList;
  Threads<chunkList > * threadPool;
  
}


static_assert(std::is_pod<mmState::chunk>::value, "Fatal error: Struct \"chunk"
	      "\" was found not to be a POD\n");


constexpr size_t chunkAccountingSize {sizeof(mmState::chunk)};
// First things first we must make sure "chunk" is a POD!
void * (* allocAlgo)(const size_t chunk_size) {_firstFit};


void * _firstFit(const size_t chunk_size)
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

void * _bestFit(const size_t chunk_size)
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
  return getNewChunkFromSystem(chunk_size);*/
}


void * _worstFit(const size_t chunk_size)
{
  using namespace mmState;
  /*
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
      std::cerr<<"Fatal error: can't allocate 0 bytes.\n";
      exit(error::ALLOC);
    }
}


inline void * handleOneHole(const size_t chunk_size)
{
  /*  using namespace mmState;
  if(((*holes.begin())->size) >= (chunk_size + chunkAccountingSize))
  {*/		/* We have found a chunk but it is too big. There is more work
		   to be done. */
  /*      return splitChunkFromHoles(chunk_size, holes.before_begin());
    }
  if(((*holes.begin())->size) == chunk_size)
    {		// The chunk is exactly the right size :).
      return useChunkFromHoles(holes.before_begin());
    }

    return nullptr;*/
}


template <typename T> inline void * splitChunkFromHoles(const size_t chunk_size,
							T candidate)
{
  /*  using namespace mmState;
  // Base address of chunk to be returned.
  auto ret = (*std::next(candidate))->base;
  // CALCULATE FOR NEW HOLES CHUNK----------------------------------------------
  {						// New chunk.
    // Base address of new chunk to be put in holes.
    auto newBase ((char *)(ret) + chunk_size + chunkAccountingSize);*/
    /* Set new chunk's base address accounting info to the base address of new
       chunk. *//*
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
  
  return ret;*/
}


template <typename T> inline void * useChunkFromHoles(T candidate)
{
  using namespace mmState;
  /*  auto ret = (*std::next(candidate))->base;
  inUse.push_front(*std::next(candidate));
  holes.erase_after(candidate);
  
  return ret;*/
}


inline void * getNewChunkFromSystem(const size_t chunk_size)
{
  using namespace mmState;
    // Get new chunk (plus memory for accounting.)
  /*  address virtualChunk {address(sbrk(chunk_size + chunkAccountingSize))};
  
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
  */
  //  return ((chunk *)(virtualChunk))->base;
}


void free(const void * chunk)
{
  using namespace mmState;
  /*    
  for(auto candidate {inUse.before_begin()};
      std::next(candidate) != inUse.cend(); ++candidate)
    {
      if((*std::next(candidate))->base == chunk)
	{
	  holes.push_front(*std::next(candidate));
	  inUse.erase_after(candidate);
	  mergeHoles();
	  mergeHoles();
	  
	  return;
	}
	}*/

  /* We do this here and not in dealloc for perfomance reasons (we would have to
  have a separate test in dealloc.) */
  std::cerr<<"Fatal error: invalid address ("<<chunk<<") passed to free() (via "
    "dealloc.)\n";
  exit(error::FREE);
}


inline void mergeHoles()
{
  using namespace mmState;
  /* We must make sure that holes is sorted (it may be more efficient to do this
     differently.) */
  /*  holes.sort(holeComp);
  
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
	      candidate = holes.erase_after(candidate);*/
	      /* A merge should only be possible after inserting a new node, so
		 no more then two merges should even need to be done (i.e.
		 mergeHoles() should be called twice.) */
  /*	      break;
	    }
	  ++candidate;
	}
	}*/
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


void setThreadPoolSize(const size_t tPS)
{
  mmState::threadPool = new Threads<mmState::chunkList> {tPS};
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


void printStats()
{
  using namespace mmState;

  int inUseSz {}, holesSz {};
  double avgHoleSz {}, avgInUseSz {};

  for(auto hole {holes.begin()}; hole != holes.cend(); ++hole)
    {
      ++holesSz;
      avgHoleSz += (*hole)->size;
    }
  avgHoleSz /= holesSz;
  for(auto chunkUsing {inUse.begin()}; chunkUsing != inUse.cend(); ++chunkUsing)
    {
      ++inUseSz;
      avgInUseSz += (*chunkUsing)->size;
    }
  avgInUseSz /= inUseSz;

  std::cout<<"--------------------------------\n\tChunks in \"in use\" list: "
	   <<inUseSz<<"\n\tChunks in \"holes\" list: "<<holesSz
	   <<"\n\t\tAverage size of chunks in \"in use\" list: "<<avgInUseSz
	   <<"\n\t\tAverage size of chunks in holes list: "<<avgHoleSz<<'\n';
}
