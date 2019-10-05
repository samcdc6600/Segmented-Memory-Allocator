#include <cstddef>
#include <vector>
#include <iostream>
#include "error.hpp"

template <typename T> class Threads
{				// Where TI should be an iterator of T.
private:
  size_t threadPoolSize;
  std::vector<size_t> inUseSizes;
  std::vector<T> inUse;
  std::vector<size_t> holesSizes;
  std::vector<T> holes;
  
public:
  Threads(const size_t tPS) : threadPoolSize (tPS),
			      inUseSizes (threadPoolSize, 0),
			      inUse (threadPoolSize),
			      holesSizes (threadPoolSize, 0),
			      holes (threadPoolSize)
  {
    if(tPS < 1)
      {
	std::cerr<<"Error (in Threads() in threads.hpp): tPS < 1.\n";
	exit(error::RANGE);
      }
  }

  size_t getThreadPoolSize()
  {
    //    return threadPoolSize;
  }
};
