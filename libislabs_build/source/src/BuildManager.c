/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/Buildable.h"
#include "islabs/build/BuildManager.h"
#include "islabs/build/version.h"
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Environment.h>
#include <openocl/base/psa/Index.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/Path.h>

static void EnsureDefaultBuildPath( const char* topdir );

struct _BuildManager
{
	const Global*        global;
	IDirectory*          cachedTopDirectory;
	IDirectory*          topDirectory;
	const Buildable*     top_buildable;
	IIndex*              buildables;
	IIndex*              suites;
};

BuildManager* new_BuildManager( const char* topDirectory )
{
	BuildManager* self = CRuntime_calloc( 1, sizeof( BuildManager ) );

	EnsureDefaultBuildPath( topDirectory );

	//fprintf( stderr, "BuildManager::BuildManager(): %s\n", topDirectory );
	self->cachedTopDirectory = new_Directory( topDirectory );
	Directory_setCaching( self->cachedTopDirectory, 1 );
	self->topDirectory = new_Directory( topDirectory );
	self->buildables = new_Index();
	self->suites     = new_Index();
	return self;
}

void free_BuildManager( BuildManager* self )
{
	free_Index( self->buildables );
	free_Index( self->suites );
	free_Directory( self->topDirectory );
	CRuntime_free( self );
}

int BuildManager_refresh( BuildManager* self )
{
	int status = 1;
	const Buildable* b = Buildable_getInstance( self, self->cachedTopDirectory );
	self->top_buildable = b;

	if ( NULL == self->top_buildable )
	{
		fprintf( stderr, "build: Could not find source package directory!\n" );
		status = 0;
	}

	return status;
}

int BuildManager_buildTo( BuildManager* self, const BuildParameters* parameters, const char* target, int depth )
{
	int status = 1;

#ifdef DEBUG_MEMORY
	fprintf( stderr, "BuildManager_buildTo >>  %lli\n", CRuntime_GetAllocationCount() );
	{
#endif
	IDirectory* target_dir = new_Directory( target );
	if ( !Directory_exists( target_dir ) )
	{
		Directory_createAll( target_dir );
	}

	if ( self->top_buildable && ( TEST_PACKAGE == Buildable_getType( self->top_buildable ) ) && !BuildParameters_isRunTestsOnly( parameters ) )
	{
		BuildParameters_setBuildTests( ((BuildParameters*)parameters), 1 );
	}

	Directory_setCaching( target_dir, 1 );
	//fprintf( stderr, "BuildManager_buildTo >>> %lli\n", CRuntime_GetAllocationCount() );
	status &= Buildable_buildTo( (Buildable*) self->top_buildable, target_dir, parameters, depth );
	//fprintf( stderr, "BuildManager_buildTo <<< %lli\n", CRuntime_GetAllocationCount() );
	
	free_Directory( target_dir );

#ifdef DEBUG_MEMORY
	}
	fprintf( stderr, "BuildManager_buildTo <<  %lli\n", CRuntime_GetAllocationCount() );
#endif
	
	return status;
}

IIndex* BuildManager_getBuildablesIndex( BuildManager* self )
{
	return self->buildables;
}

IIndex* BuildManager_getSuitesIndex( BuildManager* self )
{
	return self->suites;
}

int BuildManager_buildTestsTo( const BuildManager* self, const BuildParameters* parameters, const char* target )
{
	int status = 1;
	return status;
}

int BuildManager_runTestsFrom( const BuildManager* self, const BuildParameters* parameters, const char* target )
{
	int status = 1;
	return status;
}

void BuildManager_printTree( const BuildManager* self )
{
	if ( self->top_buildable ) Buildable_printTreeTo( self->top_buildable, stdout, 0 );
}

const char*
BuildManager_GetVersion()
{
	return LIBISLABS_BUILD_VERSION;
}

void EnsureDefaultBuildPath( const char* topdir )
{
	const char* existing_build_path = Environment_GetEnvironmentValue( "BUILD_PATH" );
	char sep = Environment_GetPathSeparator();
	{
		IPath* path                  = new_Path( topdir );
		IPath* parent_path           = new_Path( Path_getDirname( path ) );
		IPath* grandparent_path      = new_Path( Path_getDirname( parent_path ) );
		IPath* greatgrandparent_path = new_Path( Path_getDirname( grandparent_path ) );
		{
			const char* directory        = Path_getNativeFormat( path );
			const char* parent           = Path_getNativeFormat( parent_path );
			const char* grandparent      = Path_getNativeFormat( grandparent_path );
			const char* greatgrandparent = Path_getNativeFormat( greatgrandparent_path );

			char* build_path = new_CharString_format_args( "%s%c%s%c%s%c%s", existing_build_path, sep, parent, sep, grandparent, sep, greatgrandparent );
			Environment_SetEnvironmentValue( "BUILD_PATH", build_path, 1 );
			free_CharString( build_path );
		}
		free_Path( path );
		free_Path( parent_path );
		free_Path( grandparent_path );
		free_Path( greatgrandparent_path );
	}
}

