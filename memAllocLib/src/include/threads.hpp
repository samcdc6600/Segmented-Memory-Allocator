#include <cstddef>
#include <iostream>

class Threads
{
private:
  size_t threadPoolSize {1};
  
public:
  Threads(const size_t tPS)
  {
    threadPoolSize = tPS;
  }

  size_t getThreadPoolSize()
  {
    return threadPoolSize;
  }
};
