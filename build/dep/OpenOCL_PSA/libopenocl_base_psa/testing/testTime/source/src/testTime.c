#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/base/psa/Time.h>

#include <stdio.h>

static bool test01_01()
{
	int status = 1;
	
	ITime* time = Time_Now();
	fprintf( stderr, "%s\n", Time_getTextual( time ) );
	free_Time( time );
	
	return status;
}

static bool test02_01()
{
	int status = 1;
	
	unsigned long long epoch = 0;
	
	ITime* time = new_Time( epoch );
	fprintf( stderr, "%s\n", Time_getTextual( time ) );
	free_Time( time );
	
	return status;
}

int main( int argc, char** argv )
{
	Profile_TestFunction( "01:01", test01_01 );
	Profile_TestFunction( "02:01", test02_01 );

	return 0;
}
