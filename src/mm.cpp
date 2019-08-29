#include <iostream>
#include "include/mm.hpp"
#include "include/algorithm.hpp"


/* This function is only for internal testing purposes. If the allocator were to be used seriously then this file
   would lack this "testAllocator()" function and a main() function and would be compiled as a library to be linked
   with another program. The program it would be linked against would also have to include mm.hpp. */
void testAllocator();
/*void * operator new(std::size_t size) throw(std::bad_alloc);
void * operator new[](std::size_t size) throw (std::bad_alloc);
void operator delete(void * ptr) throw();
void operator delete[] (void* ptr) throw();*/


int main()
{
  testAllocator();
}


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


//====================================================================================================================
//===================================================TEST FUNCS=======================================================
//====================================================================================================================


/*void * operator new(std::size_t size) throw(std::bad_alloc)
{
  void * ret {alloc(size)};
  if(ret == (void *)(-1))
    throw std::bad_alloc();
  return ret;
}


void * operator new[](std::size_t size) throw (std::bad_alloc)
{
  void * ret {alloc(size)};
  if(ret == (void *)(-1))
    throw std::bad_alloc();
  return ret;
}


void operator delete(void * ptr) throw()
{
}


void operator delete[] (void* ptr) throw()
{
}*/


void testAllocator()
{
  size_t allocSz {1024};
  int * nums1 {(int *)(alloc(allocSz * sizeof(int)))};
  int * nums2 {(int *)(alloc(allocSz * sizeof(int)))};

  int a {1}, b {1};

  for(size_t iter {}; iter < allocSz; ++iter)
    {
      nums1[iter] = 0;
      ++a, ++b;
      if(&nums1[iter] == (void *)0x88)
	std::cout<<"What!\n";
    }

  for(size_t iter {}; iter < allocSz; ++iter)
    {
      //      std::cout<<"nums1[iter]` = "<<nums1[iter]<<'\n';
    }



  for(size_t iter {}; iter < allocSz; ++iter)
    {
      nums2[iter] = int(a * b);
      ++a, ++b;
    }

  for(size_t iter {}; iter < allocSz; ++iter)
    {
      //      std::cout<<"nums2[iter]` = "<<nums2[iter]<<'\n';
      }

    dealloc(nums1);

    size_t allocSz2 {1020};
    std::cout<<"aloc256--------------------\n";
    
    int * nums3 {(int *)(alloc(allocSz2 * sizeof(int)))};

  
    for(size_t iter {}; iter < allocSz2; ++iter)
    {
      nums3[iter] = int(a * b);
      ++a, ++b;
    }

  for(size_t iter {}; iter < allocSz2; ++iter)
    {
      //      std::cout<<"nums3[iter]` = "<<nums3[iter]<<'\n';
      }



  //std::cout<<"aloc256--------------------\n"
  /*  int * nums4 {(int *)(alloc(allocSz2 * sizeof(int)))};

  
    for(size_t iter {}; iter < allocSz2; ++iter)
    {
      nums4[iter] = int(a * b);
      ++a, ++b;
    }

  for(size_t iter {}; iter < allocSz2; ++iter)
    {
      //      std::cout<<"nums3[iter]` = "<<nums3[iter]<<'\n';
      }*/

  std::cout<<"======================================hello\n";
  dealloc(nums3);
    std::cout<<"====================================hello\n";

    dealloc(nums2);

    //        std::cout<<"====================================hello\n";
    //	dealloc(nums4);
}
