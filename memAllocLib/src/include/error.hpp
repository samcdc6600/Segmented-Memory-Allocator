#ifndef ERROR_HPP_
#define ERROR_HPP_
#include <climits>
#include <iostream>


namespace error
{
  enum _error
    {
      RANGE = INT_MIN,		// Negative error values.
      INIT,
      ALLOC,
      POD,
      FREE,
      ELEMENT_NOT_FOUND,
      GEN_ERROR_ZERO_ARGS,	// GenError was called with no arguments.
      SBRK = -1
    };
  
  template<typename T>
  void genError(const T & eNum)
  {
    exit(eNum);
  }
  
  template<typename T, typename... Args>
  void genError(const int eNum, const T& value, const Args&... args)
  {                             // A general error has occured.
    std::cerr<<value;
    genError(eNum, args...);
  }
}


#endif
