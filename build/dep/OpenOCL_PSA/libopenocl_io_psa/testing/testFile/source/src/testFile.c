#include <stdio.h>
#include <string.h>

#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/io/psa/File.h>


/*
 *	Series
 *	1)	Standard streams
 *	2)	File_exists()
 *	3)	File_create()/File_delete()
 *	4)	File_read()/File_write()
 *	5)	one-many, stream-File IO.
 */

int test01_01()
{
	char* buffer = "The quick brown fox jumped over the lazy hen.\n";
	unsigned int len = CharString_getLength( buffer );

	IFile* file = File_GetFileForStandardDescriptor( OPENOCL_IO_PSA_FILE_STDERR );
	File_write( file, (const byte*) buffer, len );
	free_File( file );
	
	return 1;
}

int test02_01()
{
	int result = 0;

	IPath* path = new_Path( "source/src/testFile.c" );
	IFile* file = new_File_path( path );

	if ( File_exists( file ) )
	{
		result = 1;
	}
	free_File( file );
	free_Path( path );
	
	return result;
}

int test03_01()
{
	int result = 0;

	IPath* ipath = new_Path( "/tmp/openocl_io_psa_testFile03_01" );
	IFile* ifile = new_File_path( ipath );
	
	if ( !File_exists( ifile ) )
	{
		File_create( ifile );
	}
	if ( File_exists( ifile ) )
	{
		if ( File_delete( ifile ) )
		{
			result = 1;
		}
	}
	if ( File_exists( ifile ) )
	{
		result = 0;
	}
	free_File( ifile );
	free_Path( ipath );
	
	return result;
}

int test04_01()
{
	const char* first = "<first>This is the first part written to the file</first>";
	const char* second = "<second>This is the second part written to the file</second>";
	char* expected = CharString_cat2( first, second );
	char buffer[1024];
	unsigned int r;

	int result = 0;

	IPath* ipath = new_Path( "/tmp/openocl_io_psa_testFile" );
	IFile* ifile = new_File_path( ipath );

	if ( File_exists( ifile ) )
	{
		File_delete( ifile );
	}

	if ( File_create( ifile ) )
	{
		File_open( ifile, "w" );
		File_write( ifile, first, strlen( first ) );
		File_write( ifile, second, strlen( second ) );
		File_close( ifile );

		if ( File_open( ifile, "r" ) )
		{
			if ( (r = File_read( ifile, buffer, 1023 )) )
			{
				buffer[r] = '\0';
				if ( 0 == CharString_compare( buffer, expected ) )
				{
					result = 1;
				}
			} else if ( File_isEOF( ifile ) ) {
				fprintf( stderr, "reached end of file\n" );
			}			
			File_close( ifile );
		}
	}

	if ( !File_delete( ifile ) )
	{
		fprintf( stderr, "could not remove file\n" ); 
		result = 0;
	}

	free_Path( ipath );
	free_File( ifile );
	free_CharString( expected );

	return result;
}	

int test05_01()
{
	const char* first = "<first>This is the first part written to the file</first>";
	const char* second = "<second>This is the second part written to the file</second>";
	char buffer[1024];
	unsigned int r;

	int result = 0;

	IPath* ipath = new_Path( "/tmp/openocl_io_psa_testFile" );
	IFile* ifile = new_File_path( ipath );

	if ( File_exists( ifile ) )
	{
		File_delete( ifile );
	}

	if ( File_create( ifile ) )
	{
		File_open( ifile, "w" );
		File_write( ifile, first, strlen( first ) );
		File_write( ifile, second, strlen( second ) );
		File_close( ifile );

		File_open( ifile, "r" );
		if ( (r = File_read( ifile, buffer, 1023 )) )
		{
			buffer[r] = '\0';

			if ( File_isEOF( ifile ) )
			{
				result = 1;
			}
		}
		File_close( ifile );
	}

	if ( !File_delete( ifile ) )
	{
		fprintf( stderr, "could not remove file\n" ); 
		result = 0;
	}

	free_Path( ipath );
	free_File( ifile );

	return result;
}	

int test06_01()
{
	const char* first = "<first>This is the first part written to the file</first>";
	const char* second = "<second>This is the second part written to the file</second>";
	char*       expected = CharString_cat2( first, second );
	char buffer[1024];
	unsigned int r;

	int status = 1;

	IFile* ifile = new_File( "/tmp/openocl_io_psa_testFile" );
	IFile* ifile_target = new_File( "/tmp/openocl_io_psa_testFile_tgt" );

	if ( File_exists( ifile ) )
	{
		File_delete( ifile );
	}

	if ( File_exists( ifile_target ) )
	{
		File_delete( ifile_target );
	}

	if ( File_create( ifile ) )
	{
		File_open( ifile, "w" );
		status &= File_write( ifile, first, strlen( first ) );
		status &= File_write( ifile, second, strlen( second ) );
		File_close( ifile );

		File_open( ifile, "r" );
		File_copyTo( ifile, File_getPath( ifile_target ), 0 );
		File_close( ifile );

		File_open( ifile_target, "r" );
		if ( (r = File_read( ifile_target, buffer, 1023 )) )
		{
			buffer[r] = '\0';
			status &= ( 0 == CharString_compare( buffer, expected ) );
			status &= File_isEOF( ifile_target );
		}
		File_close( ifile_target );
	}

	if ( !File_delete( ifile ) )
	{
		fprintf( stderr, "could not remove file\n" ); 
		status = 0;
	}

	if ( !File_delete( ifile_target ) )
	{
		fprintf( stderr, "could not remove file\n" ); 
		status = 0;
	}

	free_File( ifile );
	free_File( ifile_target );
	free_CharString( expected );

	return status;
}	

int test07_01()
{
	const char* first = "<first>This is the first part written to the file</first>";

	int status = 1;

	IFile* ifile = new_File( "/tmp/openocl_io_psa_testFile" );

	if ( File_exists( ifile ) )
	{
		File_delete( ifile );
	}

	if ( File_create( ifile ) )
	{
		File_open( ifile, "w" );
		File_write( ifile, first, CharString_getLength( first ) );
		File_close( ifile );

		status &= (0 != File_getLastAccess( ifile ));
		status &= (0 != File_getLastModification( ifile ));
		status &= (0 != File_getLastChange( ifile ));
	}

	if ( !File_delete( ifile ) )
	{
		fprintf( stderr, "could not remove file\n" ); 
		status = 0;
	}

	free_File( ifile );

	return status;
}	

int main( int argc, char** argv )
{
	Profile_TestFunction( "test01_01", test01_01 );
	Profile_TestFunction( "test02_01", test02_01 );
	Profile_TestFunction( "test03_01", test03_01 );
	Profile_TestFunction( "test04_01", test04_01 );
	Profile_TestFunction( "test05_01", test05_01 );
	Profile_TestFunction( "test06_01", test06_01 );
	Profile_TestFunction( "test07_01", test07_01 );
	
	return 0;
}

