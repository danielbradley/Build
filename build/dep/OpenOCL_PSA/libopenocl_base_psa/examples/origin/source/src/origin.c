#include <openocl/base/psa/System.h>
#include <stdio.h>

int main( int argc, char** argv )
{
	System_Initialize( argc, argv );

	IEnvironment* env = System_GetInitialEnvironment();
	fprintf( stdout, "%s\n", argv[0] );
	fprintf( stdout, "%s\n", Environment_getOrigin( env ) );
	free_Environment( env );

	return 0;
}
