
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/io/psa/SuperDirectory.h>

#include <stdio.h>

int test01_01()
{
	bool status = 1;
	ISuperDirectory* super = new_SuperDirectory();
	free_SuperDirectory( super );
	return status;
}


int test01_10()
{
	bool status = 1;
	ISuperDirectory* super = new_SuperDirectory();

	const IList* volumes = SuperDirectory_getVolumes( super );
	int max = List_count( volumes );
	int i;
	for ( i=0; i < max; i++ )
	{
		fprintf( stderr, "%s\n", volumes->items[i] );
	}

	free_SuperDirectory( super );
	return status;
}

int main( int argc, char** argv )
{

	Profile_TestFunction( "test01_01", test01_01 );
	Profile_TestFunction( "test01_10", test01_10 );

	return 0;
}
