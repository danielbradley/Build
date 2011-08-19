#include <openocl/base/psa/Profile.h>
#include <openocl/threads/psa/Mutex.h>

static int test01_01()
{
	int status = 0;
	IMutex* mutex = new_Mutex();
	if ( Mutex_isValid( mutex ) )
	{
		if ( Mutex_lock( mutex ) && Mutex_unlock( mutex ) )
		{
			status = 1;
		}
	}
	free_Mutex( mutex );
	
	return status;
}

static int test01_02()
{
	int status = 0;
	IMutex* mutex = new_Mutex();
	if ( Mutex_isValid( mutex ) )
	{
		if ( Mutex_lock( mutex ) && Mutex_lock( mutex ) )
		{
			if ( Mutex_unlock( mutex ) && Mutex_unlock( mutex ) )
			{
				status = 1;
			}
		}
	}
	free_Mutex( mutex );
	
	return status;
}

int main( int argc, char** argv )
{
	int status = 1;
	status &= Profile_TestFunction( "test01:01", test01_01 );
	status &= Profile_SkipFunction( "test01:02", test01_02, "Double lock of mutex on Mac OS X causes lockup" );
	return !status;
}
