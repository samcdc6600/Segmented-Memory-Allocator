#include <cstddef>
#include <vector>
#include <iostream>

template <typename T> class Threads
{				// Where TI should be an iterator of T.
private:
  size_t threadPoolSize;
  std::vector<size_t> inUseSizes {threadPoolSize, 0};
  std::vector<T> inUse {threadPoolSize};
  std::vector<size_t> holesSizes {threadPoolSize, 0};
  std::vector<T> holes {threadPoolSize};
  
public:
  Threads(const size_t tPS) : threadPoolSize (tPS)
  {
    if(tPS < 1)
      {
	std::cerr<<"Error (in Threads() in threads.hpp): tPS < 1.\n";
	//	exit();
      }
  }

  size_t getThreadPoolSize()
  {
    //    return threadPoolSize;
  }
};
