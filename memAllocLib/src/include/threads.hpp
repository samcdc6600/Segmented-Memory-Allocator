#include <cstddef>
#include <vector>
#include <iostream>
#include "error.hpp"

template <typename T> class Threads
{				// Where TI should be an iterator of T.
private:
  size_t threadPoolSize;
  std::vector<pthread_t> tids;
  
public:
  Threads(const size_t tPS) : threadPoolSize (tPS),
			      tids (tPS, 0),
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
