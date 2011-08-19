#include <openocl/base/psa/CharString.h>
#include <openocl/io/psa/Path.h>
#include <stdio.h>

void usage()
{
	fprintf( stdout, "Usage: dirname NAME\n" );
	fprintf( stdout, "   or: dirname OPTION\n" );
	fprintf( stdout, "Print NAME with its trailing /component remove; if NAME contains no /'s,\n" );
	fprintf( stdout, "output `.' (meaning the current directory).\n" );
	fprintf( stdout, "\n" );
	fprintf( stdout, "\t--help\tdisplay this help and exit\n" );
	fprintf( stdout, "\t--version\toutput version information and exit\n" );
	fprintf( stdout, "\n" );
	fprintf( stdout, "Report bugs to <examples@openocl.org>.\n" );
}

void version()
{
	fprintf( stdout, "dirname (OpenOCL examples)\n" );
	fprintf( stdout, "Written by Daniel Bradley\n" );
	fprintf( stdout, "\n" );
	fprintf( stdout, "Copyright (C) 2005 Daniel Robert Bradley.\n" );
	fprintf( stdout, "This is free software; see the source for copying conditions. There is NO\n" );
	fprintf( stdout, "warrenty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" );
}

int main( int argc, char** argv )
{
	if ( argc > 2 )
	{
		fprintf( stderr, "dirname: too many arguments\n" );
		fprintf( stderr, "Try `dirname --help' for more information\n" );
	} else if ( argc < 2 ) {
		fprintf( stderr, "dirname: too few arguments\n" );
		fprintf( stderr, "Try `dirname --help' for more information\n" );
	} else if ( 0 == CharString_compare( argv[1], "--help" ) ) {
		usage();
	} else if ( 0 == CharString_compare( argv[1], "--version" ) ) {
		version();
	} else {
		IPath* path = new_Path( argv[1] );
		fprintf( stdout, "%s\n", Path_getDirname( path ) );
		free_Path( path );
	}
	return 0;
}
