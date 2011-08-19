#include <openocl/provider/psa/Provider.h>

#include <stdio.h>
#include <dlfcn.h>


int main( int argc, char** argv )
{
	Provider* provider = Provider_Load( "/local/checkouts/openocl-1.0/providers/libislabs_demo_provider-1.0/lib/DemoProvider.dylib" );
	if ( provider )
	{
		Provider_print( provider, stdout );
	} else {
		fprintf( stderr, "%s\n", dlerror() );
	}
	
//	PrintProvider* pprovider = (PrintProvider*) Provider_getObject( provider );
//	pprovider->println( "Hello World" );

	return 0;
}
