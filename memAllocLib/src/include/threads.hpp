#ifndef THREADS_HPP_
#define THREADS_HPP_
#include <cstddef>
#include <vector>
#include <iostream>
#include "error.hpp"
#include "mmDefs.hpp"


template <typename T, typename TArgs> class Threads
{				// Where TI should be an iterator of T.
private:
  size_t threadPoolSize;
  std::vector<pthread_t> tids;
  
public:
  Threads(const size_t tPS) : threadPoolSize (tPS),
			      tids (tPS, 0)
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

  // Takes a function pointer to an algorithm and a struct containing 
  void startThreads(void * (* _firstFitProper)(void * args),
		    std::vector<TArgs> args)
  {
    for(auto t: tids)
      {
	int pthreadRet {};
	pthreadRet = pthread_create(&t, nullptr, _firstFitProper, nullptr);
	
      }
  }
};


#endif
