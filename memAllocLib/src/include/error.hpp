#ifndef ERROR_HPP_
#define ERROR_HPP_
#include <climits>


namespace error
{
  enum _error
    {
      RANGE = INT_MIN,		// Negative error values.
      INIT,
      ALLOC,
      POD,
      FREE,
      SBRK = -1
    };
}


#endif
