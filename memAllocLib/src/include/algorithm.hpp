#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_
#include <cstddef>		// For size_t.
#include <forward_list>
#include <new>			// For bad_alloc.
#include <climits>


namespace error
{
  enum _error
    {
      RANGE = INT_MIN,		// Negative error values.
      ALLOC,
      POD,
      FREE,
      SBRK = -1
    };
}


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

void * _firstFit(const size_t chunk_size);
void * _bestFit(const size_t chunk_size);
void * _worstFit(const size_t chunk_size);
inline void * getNewChunkFromSystem(const size_t chunk_size);
void free(const void * chunk);
/* Merges adjacent holes. Should be called whenever a new holes is inserted into holes. E.g., at the end of free. */
inline void mergeHoles();
inline bool holeComp(mmState::chunk * a, mmState::chunk * b);
// Returns true if chunk a is adjacent to chunk b. Returns false otherwise.
inline bool holeAbuttedAgainstHole(mmState::chunk * a, mmState::chunk * b);
void * _checkPOD(const size_t chunk); // Calls checkPODProper() and then calls _firstFit().
inline void checkPODProper();	// Does the actual POD checking. Exits if the struct "chunk" is not a POD.


#endif
