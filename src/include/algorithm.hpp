#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_
#include <cstddef>		// For size_t.
#include <forward_list>
#include <new>			// For bad_alloc
#include <climits>


namespace error
{
  enum _error
    {
      RANGE = INT_MIN,		// Negative error values
      ALLOC,
      POD,
      FREE,
      SBRK = -1
    };
}


namespace mmState
{
  typedef void * address;
  //  namespace chunk
  //  {
    /* This namespace is used to logically group "base" and "size" I would have put them in a struct but I was not
      sure whether it would be POD and as it is unsafe to not use new and delete with something that is not POD i
      have decided to simply use these types without the use of a struct (alghout I think it probably would have
      been fine.) */
  /*    typedef void * base;
	typedef size_t size;*/
  //  }
  struct chunk
  {
    address base;
    size_t size;
  };
  extern std::forward_list<chunk *> inUse;
  extern std::forward_list<chunk *> holes;
}


extern void * (* allocAlgo)(const size_t chunk_size);

void * _firstFit(const size_t chunk_size);
void * _bestFit(const size_t chunk_size);
void * _worstFit(const size_t chunk_size);
void free(const void * chunk);
/* Merges adjacent holes. Should be called whenever a new holes is inserted into holes. E.g., at the end of free. */
void mergeHoles();
bool holeComp(mmState::chunk * a, mmState::chunk * b);
// Returns true if chunk a is adjacent to chunk b. Returns false otherwise.
bool holeAbuttedAgainstHole(mmState::chunk * a, mmState::chunk * b);
void * _checkPOD(const size_t chunk); // Calls checkPODProper() and then calls _firstFit().
void checkPODProper();	// Does the actual POD checking. Exits if the struct "chunk" is not a POD.


#endif
