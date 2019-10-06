#ifndef MM_DEFS_HPP_
#define MM_DEFS_HPP_


namespace mmDefs
{
  typedef void * address;
  struct chunk
  {
    address base;
    size_t size;
  };
  typedef std::forward_list<chunk *> chunkList;
  // This struct is used to pass multiple arguments into a new thread.
  struct threadFuncArg
  {
    const size_t chunk_size;
    mmDefs::chunkList start;
  };
}

#endif
