#ifndef FIRST_READERS_HPP_
#define FIRST_READERS_HPP_
#include <semaphore.h>
#include "error.hpp"

/* Because rwMutext is common to both readers and writers it is not a member of
   either associated class. Rather The Readers class and the Writers class each
   get passed a mutex that they both use as the "rw-mutext". Therfore it is up
   to the person using the Readers and Writers classes to initialize the rwMutex
   and to also call the corresponding sem_destroy() on it at the appropriate
   point. */
class Readers
{
private:
  bool rwSet {false};
  sem_t * rwMutex;
  sem_t mutex;
  size_t read_count = 0;
  const int pshared {0};
  const int mInitVal {1};
  
public:
  Readers()
  {
    sem_init(&mutex, pshared, mInitVal);
  }
  
  Readers(sem_t * rwMutex)
  {
    rwSet = true;
    this->rwMutex = rwMutex;
    sem_init(&mutex, pshared, mInitVal);
  }

  void setRwMutex(sem_t * rwMutex)
  {
    if(!rwSet)
      {
	rwSet = true;
	this->rwMutex = rwMutex;
      }
    else
      {
	std::cerr<<"Error (in firstReaders.hpp): setRwMutex called more then "
	  "once or after Readers(sem_t * rwMutex) constructor on Readers "
	  "object!\n";
      }
  }

  ~Readers()
  {
    sem_destroy(&mutex);
  }

  int pRwM()
  {
    rwSetCheck("Error (in firstReaders.hpp): pRwM() called on object of type "
	       "Reader but rwMutex not set!\n");
    return sem_wait(rwMutex);
  }

  int pM()
  {
    rwSetCheck("Error (in firstReader.hpp): pM() called on object of type "
	       "Reader but rwMutex not set!\n");
    return sem_wait(&mutex);
  }

  int vRwM()
  {
    rwSetCheck("Error (in firstReader.hpp): vRwM() called on object of type "
	       "Reader but rwMutex not set!\n");
    return sem_post(rwMutex);
  }

  int vM()
  {
    rwSetCheck("Error (in firstReader.hpp): vM() called on object of type "
	       "Reader but rwMutex not set!\n");
    return sem_post(&mutex);
  }

private:
  void rwSetCheck(const char * e) const
  {
    if(!rwSet)
      {
	std::cerr<<e;
	exit(error::INIT);
      }
  }
};


#endif
