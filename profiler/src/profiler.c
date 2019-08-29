#include <stdio.h>
#include <stdbool.h>
#include "include/mm.h"

int main()
{
  size_t allocSz (1024);
  int a (1), b (1);
  
  int * nums1 ((int *)alloc(allocSz * sizeof(int)));

  for(size_t iter (0); iter < allocSz; ++iter)
    {
      nums1[iter] = a * b;
      ++a, ++b
    }
    for(size_t iter (0); iter < allocSz; ++iter)
    {
      printf("nums1[iter] = %i\n", nums1[iter]);
    }
  
  printf("hello world\n");
}


/*void testAllocator()
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
    }                                     for(size_t iter {}; iter < allocSz2; ++iter)                                                                         
    {                                                                                                                  
      //      std::cout<<"nums3[iter]` = "<<nums3[iter]<<'\n';                                                         
      }*/

/*  std::cout<<"======================================hello\n";
  dealloc(nums3);
    std::cout<<"====================================hello\n";

    dealloc(nums2);

    //        std::cout<<"====================================hello\n";                                                
    //  dealloc(nums4);                                                                                                
}*/
