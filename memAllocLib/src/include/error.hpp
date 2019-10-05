namespace error
{
  enum _error
    {
      RANGE = INT_MIN,		// Negative error values.
      ALLOC,
      POD,
      FREE,
      SBRK = -1
    };
}
