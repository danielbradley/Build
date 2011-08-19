#include <stdio.h>
#include <string.h>

#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/base/psa/Set.h>
#include <openocl/base/psa/Time.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/Path.h>


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
	IDirectory* dir = new_Directory( "directory.testdir" );
	free_Directory( dir );

	return 1;
}

int test02_01()
{
	int status = 1;
	IDirectory* dir = new_Directory( "directory.testdir" );
	if ( Directory_exists( dir ) )
	{
		status = 0;
	}
	free_Directory( dir );

	return status;
}

int test03_01()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();
	//IDirectory* current_dir = new_Directory_path( current_path ); 
	
	//if ( !Directory_exists( current_dir ) )
	//{
	//	status = 0;
	//}
	//free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

int test03_02()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();
	const char* location = Path_getCondensed( current_path );
	IDirectory* current_dir = new_Directory( location ); 
	
	if ( !Directory_exists( current_dir ) )
	{
		status = 0;
	}
	
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

int test03_03()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path );
	IDirectory* dir = Directory_subdirectory( current_dir, "directory.testdir" );
	
	if ( !Directory_exists( current_dir ) )
	{
		status = 0;
	}

	free_Directory( dir );
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

int test03_04()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path ); 
	IDirectory* dir = Directory_subdirectory( current_dir, "directory.testdir" );
	
	if ( Directory_exists( dir ) )
	{
		status = 0;
	} else {
		status &= Directory_create( dir );
		status &= Directory_delete( dir );
	}

	if ( Directory_exists( dir ) )
	{
		status = 0;
	}

	free_Directory( dir );
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

int test03_05()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path ); 
	IDirectory* dir = Directory_subdirectory( current_dir, "directory.testdir" );
	
	if ( Directory_exists( dir ) )
	{
		status = 0;
	} else {
		status &= Directory_create( dir );
		{
			Directory_getLastAccess( dir );
			Directory_getLastModification( dir );
			Directory_getLastChange( dir );
		}
		status &= Directory_delete( dir );
	}
	free_Directory( dir );
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

/*
 *	Tests Directory_isLink()
 */
int test04_01()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();

	char* link_location = CharString_cat3( Path_getCommon( current_path ), "/", "test.openocl_io_psa.link" );
	const char* target  = Path_getCommon( current_path );
	IFile* link = new_File( link_location );

	if ( File_exists( link ) )
	{
		status &= File_delete( link );
	}

	if ( (status &= File_create( link )) )
	{
		if ( File_open( link, "w" ) )
		{
			status &= File_write( link, target, strlen( target ) );
			File_close( link );
		}
	}

	{
		IDirectory* dir = new_Directory( link_location );
		status &= Directory_isLink( dir );
		free_Directory( dir );
	}
	
	if ( File_exists( link ) )
	{
		File_delete( link );
	}
	
	free_Path( current_path );
	free_File( link );
	free_CharString( link_location );
	
	return status;
}

int test04_02()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();
	char* lib_location  = CharString_cat2( Path_getCommon( current_path ), "/../../lib" );

	{
		IDirectory* dir = new_Directory( lib_location );
		Directory_refresh( dir );
		const IList* files = Directory_getFileList( dir );
		int i;
		for ( i=0; i < List_count( files ); i++ )
		{
			fprintf( stdout, "%s\n", files->items[i] );
		
		}
		free_Directory( dir );
	}
	free_Path( current_path );
	free_CharString( lib_location );
	
	return status;
}

int test10_01()
{
	int status = 1;

	IPath* current_path = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path );
	Directory_setCaching( current_dir, 1 );
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

/*
 *  Tests below here are on directories with cacheing enabled.
 */

int test100_01()
{
	int status = 1;

	IPath* current_path = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path );
	Directory_setCaching( current_dir, 1 );
	{
		const ISet* subdirectories = Directory_getCachedSubdirectories( current_dir );
		unsigned int max = Set_count( subdirectories );
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			//const IDirectory* dir =
			Set_get( subdirectories, i );
		}
	}
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

int test104_02()
{
	int status = 1;
	IPath* current_path = Path_GetCurrentDirectory();
	char* lib_location  = CharString_cat2( Path_getCommon( current_path ), "/../../lib" );

	{
		IDirectory* dir = new_Directory( lib_location );
		Directory_setCaching( dir, 1 );
		Directory_refresh( dir );
		const ISet* subdirs = Directory_getCachedSubdirectories( dir );
		int i;
		for ( i=0; i < Set_count( subdirs ); i++ )
		{
			const IDirectory* d = (const IDirectory*) Set_get( subdirs, i );
			fprintf( stdout, "%s\n", Path_getCommon( Directory_getPath( d ) ) );
		}
		free_Directory( dir );
	}
	free_Path( current_path );
	free_CharString( lib_location );
	
	return status;
}


int test110_01()
{
	int status = 1;

	IPath* current_path = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path );
	Directory_setCaching( current_dir, 1 );
	{
		const ISet* subdirectories = Directory_getCachedSubdirectories( current_dir );
		unsigned int max = Set_count( subdirectories );
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			//const IDirectory* dir =
			Set_get( subdirectories, i );
		}
	}
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

int test120_01()
{
	int status = 1;

	IPath* current_path = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path );
	Directory_setCaching( current_dir, 1 );
	{
		const ISet* subdirectories = Directory_getCachedSubdirectories( current_dir );
		unsigned int max = Set_count( subdirectories );
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			const IDirectory* dir = Set_get( subdirectories, i );
			const IPath* path = Directory_getPath( dir );
			//const IDirectory* dir2 =
			Directory_getCachedSubdirectory( current_dir, Path_getBasename( path ) );
		}
	}
	free_Directory( current_dir );
	free_Path( current_path );

	return status;
}

int main( int argc, char** argv )
{
	Profile_TestFunction( "test01_01", test01_01 );
	Profile_TestFunction( "test02_01", test02_01 );
	Profile_TestFunction( "test03_01", test03_01 );
	Profile_TestFunction( "test03_02", test03_02 );
	Profile_TestFunction( "test03_03", test03_03 );
	Profile_TestFunction( "test03_04", test03_04 );
	Profile_TestFunction( "test03_05", test03_05 );
	
	Profile_TestFunction( "test04_01", test04_01 );
	Profile_TestFunction( "test04_02", test04_02 );

	Profile_TestFunction( "test100_01", test100_01 );
	Profile_TestFunction( "test104_02", test104_02 );
	Profile_TestFunction( "test110_01", test110_01 );
	return 0;
}

