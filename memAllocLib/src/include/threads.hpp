#include <cstddef>
#include <vector>
#include <iostream>

template <typename T, typename TI> class Threads
{				// Where TI should be an iterator of T.
private:
  bool setTPSTwice {false};
  size_t threadPoolSize {1};
  std::vector<size_t> inUseSizes {threadPoolSize, 0};
  std::vector<TI> inUse;
  std::vector<size_t> holesSizes {threadPoolSize, 0};
  std::vector<TI> holes;
  
public:
  Threads(const size_t tPS, const T inUse, const T holes) :
    inUse {threadPoolSize, inUse.before_begin()}
  {
    if(tPS > 0)
      {
	threadPoolSize = tPS;
	//	this->inUse.resize(threadPoolSize);
	//	this->inUse[0] = inUse.cbefore_begin();
      }
  }

  void setPoolSize(const size_t tPS)
  {
    /*    if(!setTPSTwice)
      {
	setTPSTwice = true;
	threadPoolSize = tPS;
      }
    else
      {
	std::cerr<<"Error (in threads.hpp in setPoolSize()): setPoolSize() "
	  "called more then once (note it is also an error to call "
	  "setPoolSize() after any calls to alloc() or dealloc() have occured."
	  " No warning will be issued for this error!)";
	  }*/
  }

  size_t getThreadPoolSize()
  {
    //    return threadPoolSize;
  }
};
