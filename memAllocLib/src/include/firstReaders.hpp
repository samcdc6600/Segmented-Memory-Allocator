#ifndef FIRST_READERS_HPP_
#define FIRST_READERS_HPP_
#include <semaphore.h>
#include "error.hpp"



// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
// TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP
extern pthread_mutex_t printLock; // TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP TMP 

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
  size_t readCount = 0;
  const int pshared {0};
  const int mInitVal {1};

  //  pthread_mutex_t tmpMutex;
  
public:
  Readers()
  {
    if(sem_init(&mutex, pshared, mInitVal))
      error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
		      "sem_init() failed in call to Readers() on Readers "
		      "object!\n");
      
  }

  
  Readers(sem_t * rwMutex)
  {
    rwSet = true;
    this->rwMutex = rwMutex;
    if(sem_init(&mutex, pshared, mInitVal))
      error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
		      "sem_init() failed in call to Readers(sem_t * ) on "
		      "Readers object!\n");
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

  // TMP========================================================================
  bool inCritical {false};
  // TMP========================================================================
  
  void enterCritical()
  {
    /*    pthread_mutex_lock(&printLock);
    std::cout<<"in enterCritical() "<<pthread_self()<<'\n';
    pthread_mutex_unlock(&printLock);*/
    rwSetCheck("Error (in firstReaders.hpp): enterCritical() called on object "
	       "of type Reader but rwMutex not set!\n");
    if(sem_wait(&mutex))
      error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
		      "sem_wait(mutex) in enterCritical() failed!\n");

    
    //    pthread_mutex_lock(&tmpMutex);
    //    std::cout<<"\tEntering critical section for reader. read count = "<<readCount<<std::endl;
    //    pthread_mutex_unlock(&tmpMutex);

    
    ++readCount;
    if(readCount == 1)
      if(sem_wait(rwMutex))
	error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
			"sem_wait(rwMutex) in enterCritical() failed!\n");
    if(sem_post(&mutex))
      error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
		      "sem_Post(mutex) in enterCritical() failed!\n");


    // TMP======================================================================
    inCritical = true;
    // TMP======================================================================

    /*a    pthread_mutex_lock(&printLock);
    std::cout<<"exiting enterCritical() "<<pthread_self()<<'\n';
    pthread_mutex_unlock(&printLock);*/
  }
  

  void exitCritical()
  {
    /*    pthread_mutex_lock(&printLock);
    std::cout<<"in exitCritical() "<<pthread_self()<<'\n';
    pthread_mutex_unlock(&printLock);*/
    
    rwSetCheck("Error (in firstReaders.hpp): exitCritical() called on object of"
	       "type Reader but rwMutex not set!\n");
    if(sem_wait(&mutex))
      error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
		      "sem_wait(mutex) in exitCritical() failed!\n");


    //    pthread_mutex_lock(&tmpMutex);
    //    std::cout<<"\tExiting critical section for reader. read count = "<<readCount<<std::endl;
    //    pthread_mutex_unlock(&tmpMutex);


        // TMP======================================================================
    inCritical = false;
    // TMP======================================================================
    
    --readCount;
    if(readCount == 0)
      if(sem_post(rwMutex))
	error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
			"sem_post(rwMutex) in exitCritical() failed!\n");
    if(sem_post(&mutex))
      error::genError(error::INIT, "Error (in firstReaders.hpp): call to "
		      "sem_post(&mutex) in exitCritical() failed!\n");

    /*        pthread_mutex_lock(&printLock);
    std::cout<<"exiting exitCritical() "<<pthread_self()<<'\n';
    pthread_mutex_unlock(&printLock);*/
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
