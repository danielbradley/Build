/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/PackageTableAndDistributable.h"
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Set.h>

struct _PackageTable
{
	IList* entries;
	ISet*  distributables;
};

PackageTable* new_PackageTable()
{
	PackageTable* self = CRuntime_calloc( 1, sizeof( PackageTable ) );
	self->entries = new_List();
	self->distributables = new_Set();
	return self;
}

void free_PackageTable( PackageTable* self )
{
	unsigned int max = Set_count( self->distributables );
	unsigned int i;
	for ( i=0; i < max; i++ )
	{
		free_Distributable( (Distributable*) Set_get( self->distributables, i ) );
	} 

	free_Set( self->distributables );
	free_List( self->entries );
	CRuntime_free( self );
}

void PackageTable_add( PackageTable* self, Distributable* dep )
{
	List_copyItem( self->entries, Distributable_getLocation( dep ) );
	Set_add( self->distributables, dep );
}

void PackageTable_clear( PackageTable* self )
{
}

const Distributable*
PackageTable_findEquivalent( const PackageTable* self, const Distributable* dep )
{
	const Distributable* ret = NULL;
	unsigned int max = Set_count( self->distributables );
	unsigned int i;
	for ( i=0; i < max; i++ )
	{
		const Distributable* obj = (const Distributable*) Set_get( self->distributables, i );
		if ( 0 == CharString_compare( Distributable_getLocation( dep ), Distributable_getLocation( obj ) ) )
		{
			ret = obj;
			break;
		}
	}
	return ret;
}

int PackageTable_contains( const PackageTable* self, const Distributable* dep )
{
	return List_contains( self->entries, Distributable_getLocation( dep ) );
}
