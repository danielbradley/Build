#include <openocl/base/psa/List.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/Path.h>

#include <stdio.h>

/*
 *	Usage:
 *
 *	search <pattern>
 *
 *	e.g. search *.a
 */
int main( int argc, char** argv )
{
	if ( 2 > argc )
	{
		fprintf( stdout, "Usage: search <pattern>\n" );
		exit(0);
	} else {
		IDirectory* directory = new_Directory_path( Path_GetCurrentDirectory() );
		IList* dir_list = Directory_search_depthFirst_followLinks( directory, argv[1], 1 );
		List_print( dir_list );
	}
	return 0;
}
