#include <iostream>
#include "include/mm.hpp"
#include "include/algorithm.hpp"


extern "C"
{				// These function names are to be the interface to this library from a C program.
  void * alloc(const size_t chunk_size)
  {
    mmState::address ret;
    try
      {
	ret = allocAlgo(chunk_size);
      }
    catch(std::bad_alloc e)
      {
	std::cerr<<e.what()<<'\n';
	exit(error::ALLOC);
      }
    return ret;
  }


  void dealloc(const void * chunk)
  {
    free(chunk);
  }
}
