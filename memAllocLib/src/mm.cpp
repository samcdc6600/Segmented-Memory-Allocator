#include <iostream>
#include "include/mm.hpp"
#include "include/algorithm.hpp"




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
