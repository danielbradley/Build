/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/Suite.protected.h"

Suite* new_Suite( BuildManager* bm, const IDirectory* source )
{
	Suite* self = (Suite*) CRuntime_calloc( 1, sizeof( Suite ) );
	Suite_init( self, bm, source );
	return self;
}

void free_Suite( Suite* self )
{
	Suite_final( self );
	CRuntime_free( self );
}

void
Suite_init( Suite* self, BuildManager* bm, const IDirectory* source )
{
	Buildable_init( &self->super, bm, source );
	Buildable_setType( &self->super, SUITE );
	
	self->super.free = (void(*)(Buildable*)) free_Suite;
	self->super.buildTo = (bool(*)(const Buildable*,const IDirectory*,const BuildParameters*,int)) Suite_buildTo;
	self->free = free_Suite;
	self->buildTo = Suite_buildTo;

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
				const Buildable* package = Buildable_getInstance( self->super.bm, package_dir );
				if ( package )
				{
					Buildable_addChild( (Buildable*) self, package );
				}
			}
		}
	}
}

void
Suite_final( Suite* self )
{
	Buildable_final( &self->super );
}

bool Suite_buildTo( const Suite* self, const IDirectory* target, const BuildParameters* parameters, int depth )
{
	bool status = 1;
	unsigned int i;
	unsigned int count = Set_count( self->super.children );
	for ( i=0; i < count; i++ )
	{
		Buildable* b = (Buildable*) Set_get( self->super.children, i );
		const char* basename = Path_getBasename( Directory_getRealPath( b->directory ) );
		const IDirectory* next_target = Directory_getCachedSubdirectory( target, basename );
		if ( !next_target )
		{
			next_target = Directory_getCachedSubdirectory_new( target, basename );
			Directory_createAll( next_target );
		}
		
		status &= Buildable_buildTo( b, next_target, parameters, depth );
	}
	return status;
}
