#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Profile.h>

#include <stdio.h>

static int test01_01()
{
	int status = 1;
	int mem = CRuntime_GetAllocationCount();
	
	IList* list = new_List();
	fprintf( stdout, "Allocations: %lli\n", CRuntime_GetAllocationCount() );
	free_List( list );
	fprintf( stdout, "Allocations: %lli\n", CRuntime_GetAllocationCount() );
	if ( CRuntime_GetAllocationCount() != mem )
	{
		status = 0;
	}
	return status;
}

int main()
{
	Profile_TestFunction( "01:01:00", test01_01 );
	return 0;
}	
	
