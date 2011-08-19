#include <openapi/base/BasePSA.h>

#include <stdio.h>
#include <signal.h>
//#include <unistd.h>
//#include <pthread.h>
//#include <sys/time.h>
//#include <winbase.h>
//#include <winsock.h>

using namespace openapi::base;

const int BasePSA::SIGNAL_ALARM = 0;
const bool BasePSA::DEBUG = false;

int BasePSA::Print(char *s1)
{
  // WARNING:STDIO used
  return printf(s1);
}

int BasePSA::Print(char c)
{
  return printf("%c", c);
}

//int BasePSA::print(String *s1)
//{
//  // WARNING:STDIO used
//  return printf(s1->getChars());
//}

int BasePSA::Print( int number )
{
  return printf( "%i", number );
}

int BasePSA::Print( unsigned int number )
{
  return printf( "%u", number );
}

int BasePSA::Println(char *s1)
{
  // WARNING:STDIO used
  return printf(s1) + printf("\n");
}

//int BasePSA::println(String *s1)
//{
//  // WARNING:STDIO used
//  return printf(s1->getChars()) + printf("\n");
//}

int BasePSA::Println( int number )
{
  return printf( "%i\n", number );
}

int BasePSA::Println( unsigned int number )
{
  return printf( "%u\n", number );
}

void BasePSA::Flush()
{
  fflush( null );
}


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
