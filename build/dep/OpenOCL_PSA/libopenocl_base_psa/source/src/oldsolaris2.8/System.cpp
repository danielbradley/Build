#include <openapi/base/System.h>

#include <openapi/base/ConditionVariable.h>
#include <openapi/base/Runnable.h>
#include <openapi/base/String.h>
#include <openapi/base/Mutex.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

using namespace openapi::base;

int System::print(char *s1)
{
  // WARNING:STDIO used
  return printf(s1);
}

int System::print(char c)
{
  return printf("%c", c);
}

int System::print(String *s1)
{
  // WARNING:STDIO used
  return printf(s1->getChars());
}

int System::print( int number )
{
  return printf( "%i", number );
}

int System::println(char *s1)
{
  // WARNING:STDIO used
  return printf(s1) + printf("\n");
}

int System::println(String *s1)
{
  // WARNING:STDIO used
  return printf(s1->getChars()) + printf("\n");
}

int System::println( int number )
{
  return printf( "%i", number ) + printf("\n");
}

void System::flush()
{
  fflush( null );
}


//-----------------------------------------------------------------------------
//  Time Operations
//-----------------------------------------------------------------------------

int System::getTimeAsInt()
{
  struct timeval tv;
  gettimeofday( &tv, null );
  return tv.tv_sec;
}

//-----------------------------------------------------------------------------
//  Mutex Operations
//-----------------------------------------------------------------------------

int System::initializeMutex( Mutex *mutex )
{
  pthread_mutex_t *mutex_structure = new pthread_mutex_t;
  pthread_mutexattr_t *mutexattr = null;

  int result = pthread_mutex_init( mutex_structure, mutexattr );
  mutex->setMutexStructure ( mutex_structure );
  return result;
}

int System::lockMutex( Mutex *mutex )
{
  pthread_mutex_t *mut = (pthread_mutex_t*) mutex->getMutexStructure();
  return pthread_mutex_lock( mut );
}

int System::unlockMutex( Mutex *mutex )
{
  pthread_mutex_t *mut = (pthread_mutex_t*) mutex->getMutexStructure();
  return pthread_mutex_unlock( mut );
}

//-----------------------------------------------------------------------------
//  Condition Variable Operations
//-----------------------------------------------------------------------------

int System::initializeConditionVariable( ConditionVariable *variable )
{
  pthread_cond_t *cptr = new pthread_cond_t;
  pthread_condattr_t *cond_attr = null;               

  int result = pthread_cond_init( cptr, cond_attr );
  variable->setCondVarStructure( cptr );
  return result;
}

int System::waitConditionVariable ( ConditionVariable *variable,
                                    Mutex *mutex )
{
  pthread_cond_t *var = (pthread_cond_t*) variable->getCondVarStructure();
  pthread_mutex_t *mut = (pthread_mutex_t*) mutex->getMutexStructure();
  return pthread_cond_wait( var, mut);
}

int System::timedwaitConditionVariable( ConditionVariable *variable,
                                        Mutex *mutex, int seconds )
{ 
  struct timeval tv;
  struct timespec ts;

  gettimeofday( &tv, NULL );
  ts.tv_sec = tv.tv_sec + seconds;
  ts.tv_nsec = tv.tv_usec * 1000;

  pthread_cond_t *var = (pthread_cond_t*) variable->getCondVarStructure();
  pthread_mutex_t *mut = (pthread_mutex_t*) mutex->getMutexStructure();
  return pthread_cond_timedwait( var, mut, &ts);
}

int System::signalConditionVariable( ConditionVariable *variable )
{
  pthread_cond_t *var = (pthread_cond_t*) variable->getCondVarStructure();
  return pthread_cond_signal( var );
}

int System::broadcastConditionVariable( ConditionVariable *variable )
{
  pthread_cond_t *var = (pthread_cond_t*) variable->getCondVarStructure();
  return pthread_cond_broadcast( var );
}

//-----------------------------------------------------------------------------
//  Sleep Operations
//-----------------------------------------------------------------------------

unsigned int System::sleepFor( unsigned int seconds )
{
  return sleep( seconds );
}

//-----------------------------------------------------------------------------
//  Signals
//-----------------------------------------------------------------------------

void (*System::setSignal(int signum, void (*handler)(int)))(int)
{
  return signal( signum, handler );
}

void (*System::resetSignal(int signum) )(int)
{
  return signal( signum, SIG_DFL );
}

int System::setAlarm( unsigned int seconds )
{
  return alarm( seconds);
}

int System::resetAlarm()
{
  return alarm( 0 );
}

//-----------------------------------------------------------------------------
//  Processes
//-----------------------------------------------------------------------------

int System::forkProcesses()
{
  if ( DEBUG ) System::println( "System::fork()" );

  int pid = fork();
  if ( pid == 0 )
  {
    System::println("I am the child");
  } else
  {
    System::println("I am the parent");
  }
  return fork();
}

void System::exitProcess( int status )
{
  exit( status );
}

//-----------------------------------------------------------------------------
//  Thread Operations
//-----------------------------------------------------------------------------

int System::createThread( Runnable *runnable_object )
{
  /*
   *  POSIX Threads primitive
   *
   *  pthread_create( pthread_t              *tid,
   *                  const pthread_attr_t   *attr,
   *                  void                   *(*func)(void*),
   *                  void                   *arg
   *  );
   *
   *  Returns 0 if OK, positive Exxx value on error
   */
   pthread_t tid;
   pthread_attr_t *attr = null;

   int result = pthread_create( &tid,
                                attr,
                                System::runThread,
                                runnable_object
   );
   //runnable_object->setThreadID( tid );
   return result;
}

void *System::runThread( void *runnable_object )
{
  Runnable *runner = (Runnable*) runnable_object;
  return runner->run();
}










