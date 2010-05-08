/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/BuildParameters.h"
#include "islabs/build/Command.private.h"
#include "islabs/build/ProviderContext.h"
#include "islabs/build/TestPackage.protected.h"
#include "islabs/build/providers/ToolSet.h"
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Index.h>
#include <openocl/base/psa/Stack.h>
#include <openocl/io/psa/File.h>
#include <openocl/io/psa/Link.h>

TestPackage* new_TestPackage( BuildManager* bm, const IDirectory* location )
{
	TestPackage* self = (TestPackage*) CRuntime_calloc( 1, sizeof( TestPackage ) );
	TestPackage_init( self, bm, location );
	return self;
}

void free_TestPackage( TestPackage* self )
{
	TestPackage_final( self );
}

void TestPackage_init( TestPackage* self, BuildManager* bm, const IDirectory* location )
{
	Buildable_init( &self->super.super, bm, location );
	Buildable_setType( &self->super.super, TEST_PACKAGE );

	self->super.super.free    = (void(*)(Buildable*)) free_TestPackage;
	self->super.super.buildTo = (bool(*)(const Buildable*,const IDirectory*,const BuildParameters*,int)) TestPackage_buildTo;
	self->super.free           = (void(*)(Package*)) free_TestPackage;
	self->super.buildTo        = (bool(*)(const Package*,const IDirectory*,const BuildParameters*,int)) TestPackage_buildTo;
	self->free                 = free_TestPackage;
	self->buildTo              = TestPackage_buildTo;
	
	self->super.includePrivateHeaders = 1;
}

void TestPackage_final( TestPackage* self )
{
       Package_final( &self->super );
       CRuntime_free( self );
}

bool TestPackage_buildTo( const TestPackage* self, const IDirectory* target, const BuildParameters* parameters, int depth )
{
	bool status = 1;
	if ( BuildParameters_isBuildTests( parameters ) && (!Buildable_isBuilt( (const Buildable*) self ) || BuildParameters_isFull( parameters )) )
	{
		status &= Package_buildPackage( &self->super, target, parameters );
		Buildable_setBuilt( (Buildable*) self, 1 );
	}
	if ( !Buildable_isTested( (const Buildable*) self ) && BuildParameters_isRunTests( parameters ) )
	{
		TestPackage_runPackage( target );
		Buildable_setTested( (Buildable*) self, 1 );
	}
	return status;
}

bool TestPackage_runPackage( const IDirectory* target )
{
	bool status = 1;

	const IDirectory* target_bin = Directory_getCachedSubdirectory( target, "bin" );
	if ( target_bin && Path_ChangeCurrentDirectory( Path_getCondensed( Directory_getRealPath( target ) ) ) )
	{
		const ISet* files = Directory_getCachedFiles( target_bin );
		unsigned int count = Set_count( files );
		unsigned int i;
		for ( i=0; i < count; i++ )
		{
			const IFile* test = (const IFile*) Set_get( files, i );
			const IPath* test_path = File_getPath( test );
			const char*  test_location = Path_getCondensed( test_path );
			const char*  test_basename = Path_getBasename( test_path );

			if ( CharString_startsWith( test_basename, "test" ) )
			{
				Command* command;
				IList* arguments = new_List();
				List_copyItem( arguments, test_location );
				
				command = new_Command( test_location, (const char**) arguments->items );
				Command_setStandardOut( command, "/dev/null" );
				Command_setStandardError( command, "/dev/null" );
			
				if ( Command_run( command ) && Command_waitFor( command ) )
				{
					status &= Command_getResult( command );
					if ( status )
					{
						fprintf( stdout, "%s: passed\n", test_location );
					} else {
						fprintf( stderr, "%s: !!FAILED!!\n", test_location );
					}
				}
			}
		}
		Path_RevertCurrentDirectory();		
	}
	
	return status;
}
