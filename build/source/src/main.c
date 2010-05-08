/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

//#include "islabs/build/build.h"

#include <islabs/build/Arguments.h>
#include <islabs/build/BuildManager.h>
#include <islabs/build/BuildParameters.h>
#include <islabs/build/Global.h>
#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/CharString.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/Path.h>

#include <fcntl.h>
//#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void               printUsage();
int                   dobuild( Arguments* arguments );
int        build_using_legacy( Arguments* arguments, Global* global, const char* package_dir, const char* target_dir );
int build_using_build_manager( Arguments* arguments, Global* global, const char* package_dir, const char* target_dir );

char* retrievePackageDirectory( const Arguments* args );
char* retrieveTargetDirectory( const Arguments* args, const char* default_dir );


/**
 *  Usage:
 *              build <package dir> [--destdir <destination>] [-march=<arch>] --cppflags --cflags -ldflags
 *
 */
int main( int argc, char** argv )
{
	int mem_start = CRuntime_GetAllocationCount();
	int mem_stop;

	int status = 1;

	Arguments* arguments = Arguments_new( argc, argv );
	if ( Arguments_isHelp( arguments ) )
	{
		printUsage();
		exit(0);
	}
	else if ( Arguments_isInvalid( arguments ) )
	{
		printUsage();
		exit(0);
	}

	if ( Arguments_isVersion( arguments ) )
	{
		fprintf( stdout, "build %s\n", BuildManager_GetVersion() );
		exit(0);
	}

	status = dobuild( arguments );

	Arguments_delete( arguments );
	if ( !status )
	{
		fprintf( stderr, "build: Exiting due to error.\n" );
	}

	mem_stop = CRuntime_GetAllocationCount();

#ifdef DEBUG_MEMORY	
	fprintf( stderr, "build: Memory leaked: %i\n", mem_stop - mem_start ); 
#endif

	return !status;
}

int dobuild( Arguments* arguments )
{
	int status = 1;

	char* package_dir = retrievePackageDirectory( arguments );
	char* target_dir  = retrieveTargetDirectory( arguments, package_dir );

	Global* global = Global_new( arguments );
	if ( global )
	{
		if ( 1 < global->verbosity )
		{
			Global_print( global );
		}
		if ( global->logFile )
		{
			fprintf( stdout, "%s/build.log\n", target_dir );
		}

		if ( Arguments_isLegacy( arguments ) )
		{
			status = build_using_legacy( arguments, global, package_dir, target_dir );
		}
		else
		{
			status = build_using_build_manager( arguments, global, package_dir, target_dir );
		}

	}
	Global_delete( global );
	
	CRuntime_free( target_dir );
	CRuntime_free( package_dir );

	return status;
}

int build_using_legacy( Arguments* arguments, Global* global, const char* package_dir, const char* target_dir )
{
	int status = 1;
/*
	IDirectory* package_directory = new_Directory( package_dir );
	if ( Directory_refresh( package_directory ) )
	{
		if ( Directory_containsDirectory( package_directory, "source" ) )
		{
			//printf( "build( global, %s, %s\n", package_dir, target_dir );
		        status = build( global, package_dir, target_dir, Arguments_isAll( arguments ) );
		} else {
			const IList* list = Directory_getFileList( package_directory );
			unsigned int count = List_count( list );
			char* current_package_dir;
			char* current_target_dir;
			unsigned int i;
			for ( i=0; i < count; i++ )
			{
				if ( !CharString_startsWith( list->items[i], "." ) )
				{
					current_package_dir = CharString_cat3( package_dir, "/", list->items[i] );
					current_target_dir  = CharString_cat3( target_dir,  "/", list->items[i] );
					status |= build( global, current_package_dir, current_target_dir, Arguments_isAll( arguments ) );
					free( current_package_dir );
					free( current_target_dir );
				}
			}
		}
	}
	free_Directory( package_directory );
*/
	return status;
}

int build_using_build_manager( Arguments* arguments, Global* global, const char* package_dir, const char* target_dir )
{
	int status = 1;

	BuildManager* build_manager = new_BuildManager( package_dir );
	if ( build_manager )
	{
		BuildParameters* build_parameters = new_BuildParameters( global );
		
		BuildParameters_setAll( build_parameters, Arguments_isAll( arguments ) );
		BuildParameters_setBuildTests( build_parameters, Arguments_isBuildTests( arguments ) );
		BuildParameters_setForce( build_parameters, Arguments_isForce( arguments ) );
		BuildParameters_setLog( build_parameters, Arguments_isLog( arguments ) );
		BuildParameters_setNear( build_parameters, Arguments_isNear( arguments ) );
		BuildParameters_setNo( build_parameters, Arguments_isNo( arguments ) );
		BuildParameters_setRelease( build_parameters, Arguments_isRelease( arguments ) );
		BuildParameters_setRunTests( build_parameters, Arguments_isRunTests( arguments ) );
		BuildParameters_setRunTestsOnly( build_parameters, Arguments_isRunTestsOnly( arguments ) );

		if ( (0 != CharString_compare( package_dir, target_dir )) && Arguments_isDeep( arguments ) )
		{ 
			BuildParameters_setDeep( build_parameters, 1 );
			BuildParameters_setFull( build_parameters, Arguments_isFull( arguments ) );
		} else {
			BuildParameters_setRecursive( build_parameters, 1 );
		}

		if ( !Arguments_isDynamic( arguments ) )
		{
			BuildParameters_setMonolithic( build_parameters, 1 );
		}

		if ( !(Arguments_isBuildTestsOnly( arguments ) || Arguments_isRunTestsOnly( arguments )) )
		{
			BuildParameters_setBuildPackages( build_parameters, 1 );
		} else {
			BuildParameters_setBuildTests( build_parameters, Arguments_isBuildTestsOnly( arguments ) );
			BuildParameters_setRunTests( build_parameters, Arguments_isRunTestsOnly( arguments ) );
		}

		if ( (status = BuildManager_refresh( build_manager )) )
		{
			if ( Arguments_isTree( arguments ) )
			{
				BuildManager_printTree( build_manager );
			} else {
				int depth = 0;
				if ( BuildParameters_isAll( build_parameters ) || BuildParameters_isNear( build_parameters ) )
				{
					depth = 0x7FFFFFFF;
				} else {
					depth = Arguments_getDepth( arguments );
				}
				status = BuildManager_buildTo( build_manager, build_parameters, target_dir, depth );
			}
		
			//if ( Arguments_isBuildTests( arguments ) || Arguments_isBuildTestsOnly( arguments ) )
			//{
			//	BuildManager_buildTestsTo( build_manager, build_parameters, target_dir );
			//}
		
			//if ( Arguments_isRunTests( arguments ) || Arguments_isRunTestsOnly( arguments ) )
			//{
			//	BuildManager_runTestsFrom( build_manager, build_parameters, target_dir );
			//}
		}
		free_BuildParameters( build_parameters );
	}
	free_BuildManager( build_manager );
	return status;
}

void printUsage()
{
	fprintf( stdout, "Usage: build [ --new OR --legacy ] [options] [source package directory]\n" );
	fprintf( stdout, "Options:\n" );
	//						          "                                                         "
	fprintf( stdout, "  %-23s%s\n", "    --all",              "Compile all dependencies (SEE --deep and --recursive).   " );
	fprintf( stdout, "  %-23s%s\n", "    --build-tests",      "Also build test packages located within packages'        " );
	fprintf( stdout, "  %-23s%s\n", "",                       "\"testing\" directories.                                " );
	fprintf( stdout, "  %-23s%s\n", "    --build-tests-only", "Only build test packages - do nothing else               " );
	fprintf( stdout, "  %-23s%s\n", "",                       "(see --build-tests).                                     " );
	fprintf( stdout, "  %-23s%s\n", "    --copy-libs",        "For program packages, copy all dependency libraries to   " );
	fprintf( stdout, "  %-23s%s\n", "",                       "bin directory (always on DOS/Windows platform).          " );
	fprintf( stdout, "  %-23s%s\n", "-D <definition>",        "Defines a C preprocessor definition i.e. gcc -D<define>  " );
	fprintf( stdout, "  %-23s%s\n", "",                       "However, use -D <definition> NOT -D<definition>.         " );
	fprintf( stdout, "  %-23s%s\n", "-d, --deep",             "Deep compile strictly within target directory structure  " );
	fprintf( stdout, "  %-23s%s\n", "",                       "(INCOMPATIBLE with --recursive).                         " );
	fprintf( stdout, "  %-23s%s\n", "    --depth <arg>",      "Compile dependencies only to specified depth.            " );
	fprintf( stdout, "  %-23s%s\n", "    --debug",            "Define DEBUG during preprocessing (CPPFLAGS+=-DDEBUG).   " );
	fprintf( stdout, "  %-23s%s\n", "    --debug-libc",       "Link to debug version of libc (MS Visual Studio only).   " );
	fprintf( stdout, "  %-23s%s\n", "    --dynamic",          "Dynamically link to all dependency libraries             " );
	fprintf( stdout, "  %-23s%s\n", "",                       "(INCOMPATIBLE with --monolithic).                        " );
	fprintf( stdout, "  %-23s%s\n", "-g",                     "Produce object files including debugging information.    " );
	fprintf( stdout, "  %-23s%s\n", "    --help",             "Print this command summary.                              " );
	fprintf( stdout, "  %-23s%s\n", "    --legacy",           "Build packages using old code path.                      " );
	fprintf( stdout, "  %-23s%s\n", "    --log",              "Log all executed commands to file \"./build.log\".       " );
	fprintf( stdout, "  %-23s%s\n", "    --monolithic",       "Add object files of non-system libraries to executable   " );
	fprintf( stdout, "  %-23s%s\n", "",                       "(DEFAULT) (INCOMPATIBLE with --dynamic).                 " );
	fprintf( stdout, "  %-23s%s\n", "-n",                     "No, means no! Do everything but execute external tools.  " );
	fprintf( stdout, "  %-23s%s\n", "    --near",             "Only build this package and the closest suite." );
	fprintf( stdout, "  %-23s%s\n", "-r, --recursive",        "Recursively follow dependency link files ie. compile into" );
	fprintf( stdout, "  %-23s%s\n", "",                       "location of dependency package (INCOMPATIBLE with --deep)." );
	fprintf( stdout, "  %-23s%s\n", "    --release",          "Build release binaries - stripped, no debugging etc.     " );
	fprintf( stdout, "  %-23s%s\n", "    --run-tests",        "Run compiled tests (see --build-tests).                  " );
	fprintf( stdout, "  %-23s%s\n", "    --run-tests-only",   "Only run tests - do nothing else (see --run-tests).      " );
	fprintf( stdout, "  %-23s%s\n", "-t, --target <arg>",     "Specify build target directory.                          " );
	fprintf( stdout, "  %-23s%s\n", "-v",                     "Increase verbosity of output, more means more.           " );
	fprintf( stdout, "\n");
	//fprintf( stdout, "Example: build -r --depth 1 --copy-libs --target /tmp/myprogram-1.0 ~/myprogram-1.0\n" );
	//fprintf( stdout, "\n");
	
}

char*
retrievePackageDirectory( const Arguments* args )
{
	const char* tmp_dir;
	char* pkg_dir;

	if ( NULL != ( tmp_dir = Arguments_getPackageDir( args ) ) )
	{
		IPath* p = new_Path( tmp_dir );
		pkg_dir = CharString_copy( Path_getCommon( p ) );
		free_Path( p );
	}
	else
	{
		IPath* current_path = Path_GetCurrentDirectory();
		pkg_dir = CharString_copy( Path_getCommon( current_path ) );
		free_Path( current_path );
	}
	return pkg_dir;
}

char*
retrieveTargetDirectory( const Arguments* args, const char* default_dir )
{
	char* target = NULL;
	const char* tmp;

	if ( NULL != (tmp = Arguments_getTargetDir( args ) ) )
	{
		IPath* target_path = new_Path( tmp );
		target = CharString_copy( Path_getCommon( target_path ) );
		free_Path( target_path );
	} else {
		target = CharString_copy( default_dir );
	}

	return target;
}

