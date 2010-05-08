/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <openocl/base/psa/CharString.h>
#include "islabs/build/TestSuite.protected.h"


TestSuite* new_TestSuite( BuildManager* bm, const IDirectory* source )
{
	TestSuite* self = (TestSuite*) CRuntime_calloc( 1, sizeof( TestSuite ) );
	TestSuite_init( self, bm, source );
	return self;
}

void free_TestSuite( TestSuite* self )
{
	TestSuite_final( self );
	CRuntime_free( self );
}

void
TestSuite_init( TestSuite* self, BuildManager* bm, const IDirectory* source )
{
	Buildable_init( &self->super.super, bm, source );
	Buildable_setType( &self->super.super, SUITE );
	
	self->super.super.free = (void(*)(Buildable*)) free_TestSuite;
	self->super.super.buildTo = (bool(*)(const Buildable*,const IDirectory*,const BuildParameters*,int)) Suite_buildTo;
	self->super.free = (void(*)(Suite*)) free_TestSuite;
	self->super.buildTo = Suite_buildTo;
	self->free = free_TestSuite;
	self->buildTo = (bool(*)(const TestSuite*,const IDirectory*,const BuildParameters*,int)) Suite_buildTo;

	if ( source )
	{
		const ISet* packages = Directory_getCachedSubdirectories( source );
		if ( packages )	
		{
			unsigned int max = Set_count( packages );
			unsigned int i;
			for ( i=0; i < max; i++ )
			{
				const IDirectory* package_dir = (const IDirectory*) Set_get( packages, i );
				if ( CharString_startsWith( Path_getBasename( Directory_getRealPath( package_dir ) ), "test" ) )
				{
					const Buildable* package = Buildable_getInstance( self->super.super.bm, package_dir );
					if ( package )
					{
						Buildable_addChild( (Buildable*) self, package );
					}
				}
			}
		}
	}
}

void
TestSuite_final( TestSuite* self )
{
	Suite_final( &self->super );
}
