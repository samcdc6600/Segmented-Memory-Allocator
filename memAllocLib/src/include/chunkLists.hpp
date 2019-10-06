#ifndef CHUNKLISTS_HPP_
#define CHUNKLISTS_HPP_


template <typename T> class ChunkLists
{
private:
  std::vector<size_t> inUseSizes;
  std::vector<T> inUse;
  std::vector<size_t> holesSizes;
  std::vector<T> holes;
public:
  ChunkLists(const size_t tPS) :
    inUseSizes (tPS, 0), inUse (tPS), holesSizes (tPS, 0), holes (tPS)
  {
  }
};


#endif
