#include <iostream>
#include <sstream>
#include <unistd.h>		// For brk and sbrk.
#include <type_traits>
#include "include/mm.h"
#include "include/algorithm.hpp"


namespace mmState
{
  std::forward_list<chunk *> inUse {};
  std::forward_list<chunk *> holes {};
}


constexpr size_t chunkAccountingSize {sizeof(mmState::chunk)};
void * (* allocAlgo)(const size_t chunk_size) {_checkPOD}; // First things first we must make sure "chunk" is a POD!


void * _firstFit(const size_t chunk_size)
{
  using namespace mmState;

  if(chunk_size == 0)
    {
      return sbrk(chunk_size);
    }
  else
    {
      for(auto candidate {holes.before_begin()}; std::next(candidate) != holes.cend(); ++candidate)
	{ /* We will need to add new accounting info when we split the chunk so it must have space for it. */
	  if(((*std::next(candidate))->size) >= (chunk_size + chunkAccountingSize))
	    {			// We have found a chunk but it is too big. There is more work to be done :'(.	      
	      auto ret = (*std::next(candidate))->base; // Base address of chunk to be returned.
	      // CALCULATE FOR NEW HOLES CHUNK---------------------------------------------------------------
	      {						// New chunk.
		// Base address of new chunk to be put in holes.
		auto newBase ((char *)(ret) + chunk_size + chunkAccountingSize);
		// Set new chunk's base address accounting info to the base address of new chunk.
		((chunk *)((char *)(ret) + chunk_size))->base = newBase;
		// Set new chunk's size accounting info to the the size of the new chunk.
		((chunk *)((char *)(ret) + chunk_size))->size =
		  (*std::next(candidate))->size - (chunkAccountingSize + chunk_size);
	      }

	      // UPDATE HOLES AND INUSE--------------------------------------------------------------------
	      // Set new size of chunk to be returned.
	      (*std::next(candidate))->size = chunk_size;
	      // Push new hole onto inUse list.
	      inUse.push_front(*std::next(candidate));
	      // Remove old hole from holes list.

	      // TEST CODE-=============================================================================
	      // TEST CODE-=============================================================================
	      /*	      std::cout<<"AFTER--------SPLIT:\n"
		       <<"\t new inUse.size = "<<(*std::next(candidate))->size
		       <<"\n\t new inUse.base = "<<(*std::next(candidate))->base
		       <<"\n\t new holes.size = "<<((chunk *)((char*)ret + chunk_size))->size
		       <<"\n\t new holes.base = "<<((chunk *)((char*)ret + chunk_size))->base<<'\n';
	      std::cout<<"All list's\n";
	      for(auto a: inUse)
		{
		  std::cout<<"\tinUse\n"
			   <<"a.size = "<<a->size<<"\na.base = "
			   <<a->base<<'\n';
		}
	      for(auto a: holes)
		{
		  std::cout<<"\tholes\n"
			   <<"a.size = "<<a->size<<"\na.base = "
			   <<a->base<<'\n';
		}
		std::cout<<'\n';*/
	      // TEST CODE-=============================================================================
	      // TEST CODE-=============================================================================
	      std::cout<<"split\n";
	      
	      holes.erase_after(candidate);
	      // Add new hole to holes list.
	      holes.push_front((chunk *)((char*)ret + chunk_size));

	      
	      return ret;
	    }
	  else
	    {			// We dont split the chunk if it is equal in size so we don't need any extra space.
	      if(((*std::next(candidate))->size) == chunk_size)
		{			// The chunk is exactly the right size :).
		  auto ret = (*std::next(candidate))->base;
		  inUse.push_front(*std::next(candidate));
		  holes.erase_after(candidate);

		  std::cout<<"returning (*std::next(candidate))->base\n"<<ret<<'\n';
		  return ret;
		}
	    }
	}
    }
  // Holes was empty or we didn't find a large enough chunk
  // Get new chunk (plus memory for accounting.)
  address virtualChunk {address(sbrk(chunk_size + chunkAccountingSize))};

  std::cout<<"virtualChunk = "<<virtualChunk<<", real chunk = "<<(void *)((char *)virtualChunk + chunkAccountingSize)<<'\n';
  if(virtualChunk == (address)(error::SBRK))
    {				// The allocation wasn't successfull :'(.
      std::cerr<<"Error in _firstFit: ";
      throw std::bad_alloc();
    }
  // Store base address of virtual chunk.
  ((chunk *)(virtualChunk))->base = ((char *)virtualChunk + chunkAccountingSize);
  ((chunk *)(virtualChunk))->size = chunk_size;			  // Store length of virtual chunk.
  inUse.push_front((chunk *)(virtualChunk));			  // Put new chunk accounting info on the inUse list.
  
  return ((chunk *)(virtualChunk))->base;
}


void * _bestFit(const size_t chunk_size)
{
  using namespace mmState;
  if(holes.empty())
    {				// This is our first allocation (no need to walk the list.)
    }
  
  std::cout<<"In _bestFit()\n";
  return (void *)(0);		// tmp
}


void * _worstFit(const size_t chunk_size)
{
  using namespace mmState;
  if(holes.empty())
    {				// This is our first allocation (no need to walk the list.)
    }
  
  std::cout<<"In _worstFit()\n";
  return (void *)(0);		// tmp
}


void free(const void * chunk)
{
  using namespace mmState;

  
  // TEST--------------------------------------------------------------------------
  // TEST--------------------------------------------------------------------------
  std::cout<<"FF:before free\n";
  for(auto a: inUse)
    {
      std::cout<<"\tinUse\n"
	       <<"a.size = "<<a->size<<"\na.base = "
	       <<a->base<<'\n';
    }
  for(auto a: holes)
    {
    std::cout<<"\tholes\n"
	       <<"a.size = "<<a->size<<"\na.base = "
	       <<a->base<<'\n';
    }
    std::cout<<'\n';
// TEST--------------------------------------------------------------------------
  // TEST--------------------------------------------------------------------------

    
  for(auto candidate {inUse.before_begin()}; std::next(candidate) != inUse.cend(); ++candidate)
    {
      if((*std::next(candidate))->base == chunk)
	{
	  holes.push_front(*std::next(candidate));
	  inUse.erase_after(candidate);

	  
	  // TEST--------------------------------------------------------------------------
	  // TEST--------------------------------------------------------------------------
	  std::cout<<"FF:after free\n";
	  for(auto a: inUse)
	    {
	      std::cout<<"\tinUse\n"
		       <<"a.size = "<<a->size<<"\na.base = "
		       <<a->base<<'\n';
	    }
	  for(auto a: holes)
	    {
	      std::cout<<"\tholes\n"
		       <<"a.size = "<<a->size<<"\na.base = "
		       <<a->base<<'\n';
		       }
	  std::cout<<'\n';
	  // TEST--------------------------------------------------------------------------
	  // TEST--------------------------------------------------------------------------

	  mergeHoles();

	  // TEST--------------------------------------------------------------------------
	  // TEST--------------------------------------------------------------------------
	  std::cout<<"FM:after free and merge\n";
	  for(auto a: inUse)
	    {
	      std::cout<<"\tinUse\n"
		       <<"a.size = "<<a->size<<"\na.base = "
		       <<a->base<<'\n';
	    }
	  for(auto a: holes)
	    {
	      std::cout<<"\tholes\n"
		       <<"a.size = "<<a->size<<"\na.base = "
		       <<a->base<<'\n';
	    }
	  std::cout<<"\n-----------------------------------------------------------------------\n";
	  // TEST--------------------------------------------------------------------------
	  // TEST--------------------------------------------------------------------------
	  
	  return;
	}
    }

  // We do this here and not in dealloc for perfomance reasons (we would have to have a separate test in dealloc.)
  std::cerr<<"Fatal error: invalid address ("<<chunk<<") passed to free() (via dealloc.)\n";
  exit(error::FREE);
}


void mergeHoles()
{
  using namespace mmState;
  // We must make sure that the holes is sorted (it may be more efficient to do this differently.)
  holes.sort(holeComp);
  
  if(std::next(holes.begin()) == holes.cend())
    return;			// There is only one hole in the list.
  else
    {
      /*      if(std::next(std::next(holes.begin())) == holes.cend())
	{		      // There are only two holes in the list.
	  if(holeAbuttedAgainstHole(*holes.begin(), *std::next(holes.begin())))
	    {			// Resize lower hole.
	      (*holes.begin())->size += ((*std::next(holes.begin()))->size + chunkAccountingSize);
	      // Remove higher hole from holes list :).
	      holes.erase_after(holes.begin());
		return;
	    }
	}
      else
      {		 // There are three or more holes in the list.*/
	  for(auto candidate {holes.begin()}; std::next(candidate) != holes.cend(); )
	    {
	      if(holeAbuttedAgainstHole(*candidate, *std::next(candidate)))
		{			// Resize lower hole.
		  (*candidate)->size += ((*std::next(candidate))->size + chunkAccountingSize);
		  // Remove higher hole from holes list :).
		  std::cout<<"Hello :)\n";
		  candidate = holes.erase_after(candidate);
		  if(candidate == holes.cend()) // Avoid seg fault with std::next().
		      break;
		  /* Erase_after() returns "An iterator pointing to the element that follows the last element erased
		     by the function call", therefore we must skip ++candidate */
		  continue;
		}
	      ++candidate;
	    }
	  //	}
    }
}



bool holeComp(mmState::chunk * a, mmState::chunk * b)
{				// Sort from low to high.
  return a->base < b->base ? true : false;
}


bool holeAbuttedAgainstHole(mmState::chunk * a, mmState::chunk * b)
{
  return (((char *)(a->base) + a->size) == ((char *)(b->base) - chunkAccountingSize)) ? true : false;
}



/* We put these definitions that are below this point so that they are the only ones that may see podChecked since it
   is global and only they need see it. */
// Have we made sure that "chunk" is a POD (and therefore it is safe to not use new and delete with it?)
static bool podChecked {false};	// Only visible after this point
void * _checkPOD(const size_t chunk)
{
  checkPODProper();
  allocAlgo = _firstFit;
  return allocAlgo(chunk);
}

void checkPODProper()
{
  if(!std::is_pod<mmState::chunk>::value)
    {
      std::cerr<<"Fatal error: Struct \"check\" was found not to be a POD\n";
      exit(error::POD);
    }
}


bool setAllocationAlgorithm(const allocationAlgorithm algo)
{
  std::cout<<"We should not be here !\n";
  if(podChecked)
    {
      switch(algo)
	{
	case firstFit:
	  allocAlgo = _firstFit;
	  std::cout<<"Set to first fit...\n";
	  break;
	case bestFit:
	  allocAlgo = _bestFit;
	  std::cout<<"Set to best fit...\n";
	  break;
	case worstFit:
	  allocAlgo = _worstFit;
	  std::cout<<"Set to worst fit...\n";
	  break;
	default:
	  std::cout<<"setAllocationAlgorithm called but value of algo out of range!\n";
	  return false;		// algo out of range :'(.
	}
    }
  else
    {
      checkPODProper();
      return setAllocationAlgorithm(algo);
    }
  return true;
}
