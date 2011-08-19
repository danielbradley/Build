
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/base/psa/System.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/Path.h>

#include <stdio.h>

int test01_01()
{
	bool status = 0;
	IPath* current_directory = Path_GetCurrentDirectory();
	free_Path( current_directory );
	status = 1;
	return status;
}

int test01_02()
{
	bool status = 1;
	IPath* path = new_Path( "http://openocl.org/documentation/api/namespaces" );
	free_Path( path );

	return status;
}

int test01_03()
{
	IPath* path = new_Path( "/.." );
	free_Path( path );
	
	return 1;
}


/*
 *	Test parsing of URLs
 */

int test01_10()
{
	bool status = 1;
	IPath* path = new_Path( "http://openocl.org/documentation/api/namespaces" );

	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );
	const char* native   = Path_getNativeFormat( path );
	char* volume         = System_CurrentVolume();
	char* expected;
	char fs = Path_GetFileSeparator();

	if ( volume[0] )
	{
		expected = new_CharString_format_args( "%s:%s", volume, "/documentation/api/namespaces" );
	} else {
		expected = new_CharString( "/documentation/api/namespaces" );
	}

	status &= !CharString_compare( common, "/documentation/api/namespaces" );
	status &= !CharString_compare( absolute, expected );
	
	CharString_replace( expected, '/', fs );
	CharString_replace( expected, '\\', fs );
	
	status &= !CharString_compare( native, expected );

	free_Path( path );
	free_CharString( volume );
	free_CharString( expected );

	return status;
}

int test01_11()
{
	bool status = 1;
	IPath* path = new_Path( "http://openocl.org/D:/documentation/api/namespaces" );

	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );
	const char* native   = Path_getNativeFormat( path );
	char* expected       = new_CharString_format_args( "D:/documentation/api/namespaces" );
	char* expected2      = new_CharString_format_args( "/documentation/api/namespaces" );
	char fs = Path_GetFileSeparator();

	status &= !CharString_compare( common, "/documentation/api/namespaces" );
	status &= !CharString_compare( absolute, expected );
	
	CharString_replace( expected, '/', fs );
	CharString_replace( expected, '\\', fs );
	
	status &= !CharString_compare( native, expected2 );

	free_Path( path );
	free_CharString( expected );
	free_CharString( expected2 );

	return status;
}

/*
 *	UNC style paths
 */

int test01_20()
{
	bool status = 1;
	IPath* path = new_Path( "\\\\HOSTNAME\\D\\Johnny\\documents" );

	const char* hostname = Path_getHostname( path );
	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );

	status &= !CharString_compare( hostname, "HOSTNAME" );
	status &= !CharString_compare( common, "/Johnny/documents" );
	status &= !CharString_compare( absolute, "D:/Johnny/documents" );

	free_Path( path );

	return status;
}

int test01_21()
{
	bool status = 1;
	IPath* current = Path_GetCurrentDirectory();
	IPath* path = new_Path( "\\\\\\D\\Johnny\\documents" );

	const char* hostname = Path_getHostname( path );
	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );

	status &= !CharString_compare( hostname, Path_getHostname( current ) );
	status &= !CharString_compare( common, "/Johnny/documents" );
	status &= !CharString_compare( absolute, "D:/Johnny/documents" );

	free_Path( path );
	free_Path( current );

	return status;
}

int test01_22()
{
	bool status = 1;
	IPath* current = Path_GetCurrentDirectory();
	IPath* path = new_Path( "\\\\\\\\Johnny\\documents" );
	char* volume = System_CurrentVolume();

	const char* hostname = Path_getHostname( path );
	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );

	status &= !CharString_compare( hostname, Path_getHostname( current ) );
	status &= !CharString_compare( common, "/Johnny/documents" );
	
	if ( volume[0] )
	{
		status &= !CharString_compare( absolute, "C:/Johnny/documents" );
	} else {
		status &= !CharString_compare( absolute, "/Johnny/documents" );
	}
	free_Path( path );
	free_Path( current );

	free_CharString( volume );
	
	return status;
}

/*
 *	Windows style paths
 */
int test01_30()
{
	bool status = 1;
	IPath* current = Path_GetCurrentDirectory();
	IPath* path = new_Path( "\\Documents and Settings\\Johnny\\documents" );
	char* volume = System_CurrentVolume();

	const char* hostname = Path_getHostname( path );
	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );

	status &= !CharString_compare( hostname, Path_getHostname( current ) );
	status &= !CharString_compare( common, "/Documents and Settings/Johnny/documents" );
	
	if ( volume[0] )
	{
		status &= !CharString_compare( absolute, "C:/Documents and Settings/Johnny/documents" );
	} else {
		status &= !CharString_compare( absolute, "/Documents and Settings/Johnny/documents" );
	}

	free_Path( path );
	free_Path( current );
	free_CharString( volume );

	return status;
}

int test01_31()
{
	bool status = 1;
	IPath* current = Path_GetCurrentDirectory();
	IPath* path = new_Path( "D:\\Documents and Settings\\Johnny\\documents" );

	const char* hostname = Path_getHostname( path );
	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );

	status &= !CharString_compare( hostname, Path_getHostname( current ) );
	status &= !CharString_compare( common, "/Documents and Settings/Johnny/documents" );
	status &= !CharString_compare( absolute, "D:/Documents and Settings/Johnny/documents" );

	free_Path( path );
	free_Path( current );

	return status;
}

int test01_32()
{
	bool status = 1;
	IPath* current = Path_GetCurrentDirectory();
	IPath* path = new_Path( "Johnny\\documents" );
	char fs = Path_GetFileSeparator();

	const char* hostname = Path_getHostname( path );
	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );
	//const char* native   = Path_getNativeFormat( path );

	char* absolute_expected = CharString_cat3( Path_getAbsolute( current ), "/", "Johnny/documents" );

	status &= !CharString_compare( hostname, Path_getHostname( current ) );
	status &= !CharString_compare( common, "Johnny/documents" );
	status &= !CharString_compare( absolute, absolute_expected );
	CharString_replace( absolute_expected, '/', fs );
	CharString_replace( absolute_expected, '\\', fs );

	free_CharString( absolute_expected );
	free_Path( path );
	free_Path( current );

	return status;
}

/*
 *	POSIX Paths
 */

int test01_40()
{
	bool status = 1;
	IPath* current = Path_GetCurrentDirectory();
	IPath* path = new_Path( "Johnny/documents" );
	char fs = Path_GetFileSeparator();

	const char* hostname = Path_getHostname( path );
	const char* common   = Path_getCommon( path );
	const char* absolute = Path_getAbsolute( path );
	//const char* native   = Path_getNativeFormat( path );

	char* absolute_expected = CharString_cat3( Path_getAbsolute( current ), "/", "Johnny/documents" );

	status &= !CharString_compare( hostname, Path_getHostname( current ) );
	status &= !CharString_compare( common, "Johnny/documents" );
	status &= !CharString_compare( absolute, absolute_expected );
	CharString_replace( absolute_expected, '/', fs );
	CharString_replace( absolute_expected, '\\', fs );

	free_CharString( absolute_expected );
	free_Path( path );
	free_Path( current );

	return status;
}

/*
 *	Tests Path_relativeTo
 */
int test02_01()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";
	const char* source = "/local/checkouts/build-0.5";
	const char* expected = "../openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );
	
	return status;
}

int test02_02()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib/";
	const char* source = "/local/checkouts/build-0.5/";
	const char* expected = "../openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test02_03()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib/";
	const char* source = "/local/checkouts/build-0.5";
	const char* expected = "../openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test02_04()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";
	const char* source = "/local/checkouts/build-0.5/";
	const char* expected = "../openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test02_05()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";
	const char* source = "/local/checkouts";
	const char* expected = "openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test02_06()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib/";
	const char* source = "/local/checkouts/";
	const char* expected = "openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test02_07()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib/";
	const char* source = "/local/checkouts";
	const char* expected = "openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test02_08()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";
	const char* source = "/local/checkouts/";
	const char* expected = "openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test03_01()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";
	const char* source = "/";
	const char* expected = "local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test03_02()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib/";
	const char* source = "/";
	const char* expected = "local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

int test03_03()
{
	bool status = 0;
	const char* target = "/local/checkouts/openocl-1.0/libopenocl_base-1.0/lib";
	const char* source = "";
	const char* expected = "";

	IPath* target_path = new_Path( target );
	char* relative = Path_relativeTo( target_path, source );
	
	if ( 0 == CharString_compare( relative, expected ) )
	{
		status = 1;
	}
	
	free_CharString( relative );
	free_Path( target_path );

	return status;
}

/*
 *	Tests Path_getCondensed
 */
int test04_01()
{
	bool status = 0;
	const char* target = "/local/checkouts/../openocl-1.0/libopenocl_base-1.0/../lib";
	const char* expected = "/local/openocl-1.0/lib";

	IPath* target_path = new_Path( target );
	const char* condensed = Path_getCondensed( target_path );
	
	if ( 0 == CharString_compare( condensed, expected ) )
	{
		status = 1;
	}
	
	free_Path( target_path );

	return status;
}

/*
 *	Series 5: Tests Path_GetCurrentDirectory, Path_ChangeCurrentDirectory and Path_RevertCurrentDirectory
 */
int test05_01()
{
	bool status = 1;
	
	IPath* current_path = Path_GetCurrentDirectory();
	free_Path( current_path );
	
	return status;
}

/*
int test05_02()
{
	bool status = 1;
	
	IPath* current_path = Path_GetCurrentDirectory();
	const char* parent_location = Path_getDirname( current_path );
	
	if ( (status &= Path_ChangeCurrentDirectory( parent_location ) ) )
	{
		IPath* next_current_path = Path_GetCurrentDirectory();
		free_Path( next_current_path );
		Path_RevertCurrentDirectory();
	}
	
	free_Path( current_path );
	Path_ClearDirectoryHistory();
	
	return status;
}

int test05_03()
{
	bool status = 1;
	
	IPath* current_path     = Path_GetCurrentDirectory();
	IDirectory* current_dir = new_Directory_path( current_path );
	IDirectory* data_dir    = Directory_subdirectory( current_dir, "testdata" );
	
	if ( data_dir && Directory_exists( data_dir ) )
	{
		if ( Path_ChangeCurrentDirectory( Path_getCondensed( Directory_getRealPath( data_dir ) ) ) )
		{
			char* file_location = CharString_cat2( Path_getCondensed( Directory_getRealPath( data_dir ) ), "/testfile.txt" );
			IFile* file = new_File( file_location );

			status &= File_exists( file );
			Path_RevertCurrentDirectory();

			free_File( file );
			free_CharString( file_location );
		}
	} else {
		status = 0;
	}	

	free_Directory( data_dir );
	free_Directory( current_dir );
	free_Path( current_path );
	Path_ClearDirectoryHistory();
	
	return status;
}
*/

int test06_01()
{
	IPath* path = new_Path( "" );
	
	if ( OPENOCL_IO_PSA_TYPE_FS_DIRECTORY_SUPER != Path_getType( path ) )
	{
		fprintf( stderr, "volume:   %s\n", Path_getVolume( path ) );
		fprintf( stderr, "common:   %s\n", Path_getCommon( path ) );
		fprintf( stderr, "absolute: %s\n", Path_getAbsolute( path ) );
		fprintf( stderr, "dirname:  %s\n", Path_getDirname( path ) );
		fprintf( stderr, "basename: %s\n", Path_getBasename( path ) );
	}
	free_Path( path );
	
	return 1;
}

int test06_02()
{
	IPath* path = new_Path( "C:/.." );
	if ( OPENOCL_IO_PSA_TYPE_FS_DIRECTORY_SUPER != Path_getType( path ) )
	{
		fprintf( stderr, "volume:   %s\n", Path_getVolume( path ) );
		fprintf( stderr, "common:   %s\n", Path_getCommon( path ) );
		fprintf( stderr, "absolute: %s\n", Path_getAbsolute( path ) );
		fprintf( stderr, "dirname:  %s\n", Path_getDirname( path ) );
		fprintf( stderr, "basename: %s\n", Path_getBasename( path ) );
	}
	free_Path( path );
	
	return 1;
}

int test06_03()
{
	Path_ChangeCurrentDirectory( "/" );
	Path_RevertCurrentDirectory();
	
	return 1;
}

int test06_04()
{
	fprintf( stderr, "%lli\n", CRuntime_GetAllocationCount() );
	Path_ChangeCurrentDirectory( "/" );

	/*
	IPath* path = new_Path( ".." );
	if ( OPENOCL_IO_PSA_TYPE_FS_DIRECTORY_SUPER != Path_getType( path ) )
	{
		fprintf( stderr, "volume:   %s\n", Path_getVolume( path ) );
		fprintf( stderr, "common:   %s\n", Path_getCommon( path ) );
		fprintf( stderr, "absolute: %s\n", Path_getAbsolute( path ) );
		fprintf( stderr, "dirname:  %s\n", Path_getDirname( path ) );
		fprintf( stderr, "basename: %s\n", Path_getBasename( path ) );
	}
	free_Path( path );
*/
	Path_RevertCurrentDirectory();
	
	return 1;
}

//
//	Test Path_getBasename

int test30_01()
{
	bool status = 0;
	const char* target = "/";
	const char* expected = "/";

	IPath* target_path = new_Path( target );
	const char* basename = Path_getBasename( target_path );
	
	if ( 0 == CharString_compare( basename, expected ) )
	{
		status = 1;
	}
	free_Path( target_path );

	return status;
}

int main( int argc, char** argv )
{
	Profile_TestFunction( "test01_01", test01_01 );
	Profile_TestFunction( "test01_02", test01_02 );
	Profile_TestFunction( "test01_03", test01_03 );

	Profile_TestFunction( "test01_10", test01_10 );
	Profile_TestFunction( "test01_11", test01_11 );

	Profile_TestFunction( "test01_20", test01_20 );
	Profile_TestFunction( "test01_21", test01_21 );
	Profile_TestFunction( "test01_22", test01_22 );

	Profile_TestFunction( "test01_30", test01_30 );
	Profile_TestFunction( "test01_31", test01_31 );
	Profile_TestFunction( "test01_32", test01_32 );

	Profile_TestFunction( "test01_40", test01_40 );

	Profile_TestFunction( "test02_01", test02_01 );
	Profile_TestFunction( "test02_02", test02_02 );
	Profile_TestFunction( "test02_03", test02_03 );
	Profile_TestFunction( "test02_04", test02_04 );
	Profile_TestFunction( "test02_05", test02_05 );
	Profile_TestFunction( "test02_06", test02_06 );
	Profile_TestFunction( "test02_07", test02_07 );
	Profile_TestFunction( "test02_08", test02_08 );

	Profile_TestFunction( "test03_01", test03_01 );
	Profile_TestFunction( "test03_02", test03_02 );
	Profile_TestFunction( "test03_03", test03_03 );

	Profile_TestFunction( "test04_01", test04_01 );

	Profile_TestFunction( "test05_01", test05_01 );
//	Profile_TestFunction( "test05_02", test05_02 );
//	Profile_TestFunction( "test05_03", test05_03 );

	Profile_TestFunction( "test06_01", test06_01 );
	Profile_TestFunction( "test06_02", test06_02 );
	Profile_TestFunction( "test06_03", test06_03 );
	Profile_TestFunction( "test06_04", test06_04 );

//	Profile_TestFunction( "test30_01", test30_01 );

	return 0;
}
