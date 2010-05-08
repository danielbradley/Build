/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/_Defines.h"
#include "islabs/build/BuildManager.h"
#include "islabs/build/Buildable.protected.h"
#include "islabs/build/Package.protected.h"
#include "islabs/build/Suite.protected.h"
#include "islabs/build/TestPackage.protected.h"
#include "islabs/build/TestSuite.protected.h"

#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Index.h>
#include <openocl/base/psa/Sequence.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/Path.h>
#include <stdlib.h>
#include <stdio.h>

//IIndex* buildables = 0;
IIndex* suites     = 0;

static int determineBuildableType( const IDirectory* aDirectory )
{
	int type;
	if ( Directory_containsDirectory( aDirectory, DIR_SOURCE ) )
	{
		const char* base = Path_getBasename( Directory_getRealPath( aDirectory ) );
		const IDirectory* sourcedir = Directory_getCachedSubdirectory( aDirectory, DIR_SOURCE );
		type = PACKAGE;

		if ( Directory_containsDirectory( sourcedir, SRC_DIR_JAVA ) )
		{
			type = JAVA_PACKAGE;
		}
		else if ( Directory_containsDirectory( sourcedir, SRC_DIR_CS ) )
		{
			type = DOTNET_PACKAGE;
		}
		else if ( CharString_startsWith( base, PREFIX_TEST ) )
		{
			type = TEST_PACKAGE;
		}
		else if ( CharString_startsWith( base, PREFIX_LIB ) )
		{
			type = LIBRARY_PACKAGE;
		} else {
			type = SOFTWARE_PACKAGE;
		}
	} else {
		const ISet* directories = Directory_getCachedSubdirectories( aDirectory );
		int max = Set_count( directories );
		int i;
		type = COLLECTION;
		for ( i=0; i < max; i++ )
		{
			const IDirectory* subdir = Set_get( directories, i );
			if ( Directory_containsDirectory( subdir, DIR_SOURCE ) )
			{
				type = SUITE;
				break;
			}
		}

		if ( SUITE == type )
		{
			const char* base = Path_getBasename( Directory_getRealPath( aDirectory ) );
			if ( 0 == CharString_compare( base, DIR_TESTING ) )
			{
				type = TEST_SUITE;
			} else {
				type = SOFTWARE_SUITE;
			}
		}
	}
	return type;
}

const Buildable*
Buildable_getInstance( BuildManager* bm, const IDirectory* aDirectory )
{
	Buildable* b = NULL;
	int type = 0;
	const IPath* path     = Directory_getRealPath( aDirectory );
	const char* key       = Path_getCondensed( path );
	char* type_name = NULL;

	//if ( !bm->buildables )
	//{
	//	fprintf( stderr, "Buildable_getInstance: Buildable not initialized, aborting!\n" );
	//	abort();
	//}
	IIndex* buildables = BuildManager_getBuildablesIndex( bm );
	if ( Index_containsKey( buildables, key ) )
	{
		b = (Buildable*) Index_get( buildables, key );
	} else {
		type = determineBuildableType( aDirectory );

		switch ( type )
		{
		case USER_PACKAGE:
			b = (Buildable*) new_Package( bm, aDirectory );
			Buildable_setType( b, USER_PACKAGE );
			type_name = LABEL_PACKAGE_USER;
			break;
		case LIBRARY_PACKAGE:
			b = (Buildable*) new_Package( bm, aDirectory );
			Buildable_setType( b, LIBRARY_PACKAGE );
			type_name = LABEL_PACKAGE_LIBRARY;
			break;
		case SOFTWARE_PACKAGE:
			b = (Buildable*) new_Package( bm, aDirectory );
			Buildable_setType( b, SOFTWARE_PACKAGE );
			type_name = LABEL_PACKAGE_SOFTWARE;
			break;
		case JAVA_PACKAGE:
			b = (Buildable*) new_Package( bm, aDirectory );
			Buildable_setType( b, JAVA_PACKAGE );
			type_name = LABEL_PACKAGE_JAVA;
			break;
		case DOTNET_PACKAGE:
			b = (Buildable*) new_Package( bm, aDirectory );
			Buildable_setType( b, DOTNET_PACKAGE );
			type_name = LABEL_PACKAGE_CS;
			break;
		case TEST_PACKAGE:
			b = (Buildable*) new_TestPackage( bm, aDirectory );
			Buildable_setType( b, TEST_PACKAGE );
			type_name = LABEL_PACKAGE_TEST;
			break;
		case SUITE:
			b = (Buildable*) new_Suite( bm, aDirectory );
			Buildable_setType( b, SUITE );
			type_name = LABEL_SUITE;
			break;
		case TEST_SUITE:
			b = (Buildable*) new_TestSuite( bm, aDirectory );
			Buildable_setType( b, TEST_SUITE );
			type_name = LABEL_SUITE_TEST;
			break;
		case SOFTWARE_SUITE:
			b = (Buildable*) new_Suite( bm, aDirectory );
			Buildable_setType( b, SOFTWARE_SUITE );
			type_name = LABEL_SUITE_SOFTWARE;
			break;
		case COLLECTION:
			b = NULL;
			type_name = UNKNOWN_COLLECTION;
			break;
		default:
			type_name = "Abort!!";
			abort();
		}
		if ( b ) {
			Index_add( buildables, key, (void*) b );
		}
	}

	return b;
}

void
Buildable_init( Buildable* self, BuildManager* bm, const IDirectory* source )
{
	IPath* parent_path = new_Path( Path_getDirname( Directory_getRealPath( source ) ) );

	self->bm         = bm;
	self->free       = NULL;
	self->buildTo    = NULL;
	self->directory  = source;
	self->children   = new_Set();
	self->name       = CharString_copy( Path_getBasename( Directory_getRealPath( source ) ) );
	self->suiteName  = CharString_copy( Path_getBasename( parent_path ) );

	free_Path( parent_path );
}

void
Buildable_final( Buildable* self )
{
	free_CharString( self->name );
	free_CharString( self->suiteName );
}

void
Buildable_setType( Buildable* self, int buildableType )
{
	self->type = buildableType;
}

//void
//Buildable_setParent( Buildable* self, const Buildable* parent )
//{
//	self->parent = parent;
//}

//void
//Buildable_addSibling( Buildable* self, const Buildable* sibling )
//{
//	if ( self->sibling )
//	{
//		((Buildable*)sibling)->sibling = self->sibling;
//	}
//	self->sibling = sibling;
//}

void
Buildable_addChild( Buildable* self, const Buildable* child )
{
	Set_add( self->children, child );
}

BuildManager*
Buildable_getBuildManager( const Buildable* self )
{
	return self->bm;
}

int
Buildable_getType( const Buildable* self )
{
	return self->type;
}

const char*
Buildable_getTypeName( const Buildable* self )
{
	const char* type_name;
	switch ( self->type )
	{
	case PACKAGE:
		type_name = LABEL_PACKAGE;
		break;
	case USER_PACKAGE:
		type_name = LABEL_PACKAGE_USER;
		break;
	case LIBRARY_PACKAGE:
		type_name = LABEL_PACKAGE_LIBRARY;
		break;
	case SOFTWARE_PACKAGE:
		type_name = LABEL_PACKAGE_SOFTWARE;
		break;
	case JAVA_PACKAGE:
		type_name = LABEL_PACKAGE_JAVA;
		break;
	case TEST_PACKAGE:
		type_name = LABEL_PACKAGE_TEST;
		break;
	case SUITE:
		type_name = LABEL_SUITE;
		break;
	case TEST_SUITE:
		type_name = LABEL_SUITE_TEST;
		break;
	case SOFTWARE_SUITE:
		type_name = LABEL_SUITE_SOFTWARE;
		break;
	case COLLECTION:
		type_name = UNKNOWN_COLLECTION;
		break;
	default:
		type_name = "Abort!!";
	}
	return type_name;
}

const char*
Buildable_getName( const Buildable* self )
{
	return self->name;
}

const char*
Buildable_getSuiteName( const Buildable* self )
{
	return self->suiteName;
}

void
Buildable_printTreeTo( const Buildable* self, FILE* stream, unsigned int indent )
{
	const char* up2date  = "up to date";
	const char* modified = "modified";
	const char* status;
	unsigned int i;
	unsigned int count = Set_count( self->children );
	for ( i=0; i < indent; i++ ) fprintf( stream, "\t" );

	if ( Buildable_isUpToDate( self ) )
	{
		status = up2date;
	} else {
		status = modified;
	}

	fprintf( stream, "%s (%s) (%s)\n", Buildable_getName( self ), Buildable_getTypeName( self ), status );

	for ( i=0; i < count; i++ )
	{
		Buildable_printTreeTo( Set_get( self->children, i ), stream, indent + 1 );
	}
}

void Buildable_setBuilt( Buildable* self, bool built )
{
	self->built = built;
}

void Buildable_setTested( Buildable* self, bool tested )
{
	self->tested = tested;
}

void Buildable_setUpToDate( Buildable* self, bool upToDate )
{
	self->upToDate = upToDate;
}

bool Buildable_isBuilt( const Buildable* self )
{
	return self->built;
}

bool Buildable_isTested( const Buildable* self )
{
	return self->tested;
}

bool Buildable_isUpToDate( const Buildable* self )
{
	return self->upToDate;
}

//
//	Virtual functions
//

bool Buildable_buildTo( const Buildable* self, const IDirectory* target, const BuildParameters* parameters, int depth )
{
	bool result = 0;

	bool build_on = 1;

	//fprintf( stdout, "target %s\n", Path_getAbsolute( Directory_getPath( target ) ) );

	//	Here we keep track of software suites.
	IIndex* suites = BuildManager_getSuitesIndex( self->bm );
	{
		const char* suite_name = Buildable_getSuiteName( self );
		const char* type_name  = Buildable_getTypeName( self );

		if ( BuildParameters_isNear( parameters ) )
		{
			if ( 0 == CharString_compare( type_name, LABEL_PACKAGE_LIBRARY ) )
			{
				if ( !Index_containsKey( suites, suite_name ) )
				{
					if ( 1 > Index_getSize( suites ) )
					{
//						fprintf( stderr, "Adding suite: %s\n", suite_name );
						Index_add( suites, suite_name, NULL );
					}
					else 
					{
						build_on = 0;
						result   = 1;
					}
				}
			}
		}
	}
#ifdef DEBUG_MEMORY
	fprintf( stderr, "Buildable_buildTo >>> %lli\n", CRuntime_GetAllocationCount() );
#endif
	if ( build_on ) result = self->buildTo( self, target, parameters, depth );

#ifdef DEBUG_MEMORY
	fprintf( stderr, "Buildable_buildTo <<< %lli\n", CRuntime_GetAllocationCount() );
#endif

	return result;
}

int Buildable_GetNumberOfSuitesEncountered()
{
	int number = Index_getSize( suites );
	fprintf( stderr, "Buildable_GetNumberOfSuitesEncountered() %i\n", number );
	return number;
}
