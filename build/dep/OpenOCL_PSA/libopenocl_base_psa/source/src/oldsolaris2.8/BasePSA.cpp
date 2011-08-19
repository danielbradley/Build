#include <openocl/base/BasePSA.h>

#include <stdio.h>
#include <signal.h>
//#include <unistd.h>
//#include <pthread.h>
//#include <sys/time.h>
//#include <winbase.h>
//#include <winsock.h>

using namespace openocl::base;

const int BasePSA::SIGNAL_ALARM = 0;
const bool BasePSA::DEBUG = false;

//-----------------------------------------------------------------------------
//  Time Operations
//-----------------------------------------------------------------------------

int BasePSA::GetTimeAsInt()
{
  //struct timeval tv;
  //gettimeofday( &tv, null );
  //return tv.tv_sec;
  return -1;
}

//-----------------------------------------------------------------------------
//  Sleep Operations
//-----------------------------------------------------------------------------

unsigned int BasePSA::SleepFor( unsigned int seconds )
{
  return 0; //Sleep( seconds );
}

//-----------------------------------------------------------------------------
//  Signals
//-----------------------------------------------------------------------------

void (*BasePSA::SetSignal(int signum, void (*handler)(int)))(int)
{
  return signal( signum, handler );
}

void (*BasePSA::ResetSignal(int signum) )(int)
{
  return signal( signum, SIG_DFL );
}

int BasePSA::SetAlarm( unsigned int seconds )
{
  return -1; //alarm( seconds);
}

int BasePSA::ResetAlarm()
{
  return -1; //alarm( 0 );
}
