/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_BUILDABLE_PROTECTED_H
#define ISLABS_BUILD_BUILDABLE_PROTECTED_H

#include "islabs/build.h"
#include "islabs/build.private.h"
#include "islabs/build/BuildParameters.h"
#include "islabs/build/BuildManager.h"
#include <openocl/base/psa/Index.h>
#include <openocl/io/psa/Directory.h>

/*
 *	Buildable
 *		Collection
 *		Suite
 *			Software Suite
 *			Test Suite
 *		Package
 *			TestPackage
 *			UserPackage
 */

#define UNKNOWN            0x0000
#define BUILDABLE          0x0001
#define COLLECTION         0x0011
#define SUITE              0x0021
#define SOFTWARE_SUITE     0x0121
#define TEST_SUITE         0x0221
#define PACKAGE            0x0041
#define USER_PACKAGE       0x0141
#define SOFTWARE_PACKAGE   0x1141
#define LIBRARY_PACKAGE    0x2141
#define JAVA_PACKAGE       0x4141
#define DOTNET_PACKAGE     0x8141
#define TEST_PACKAGE       0x0241

// Public functions
const Buildable* Buildable_getInstance( BuildManager* bm, const IDirectory* aDirectory );
//void           Buildable_setParent( Buildable* self, const Buildable* parent );
//void           Buildable_addSibling( Buildable* self, const Buildable* sibling );
void             Buildable_addChild( Buildable* self, const Buildable* child );
BuildManager*    Buildable_getBuildManager( const Buildable* self );
int              Buildable_getType( const Buildable* self );
const char*      Buildable_getTypeName( const Buildable* self );
const char*      Buildable_getName( const Buildable* self );
void             Buildable_printTreeTo( const Buildable* self, FILE* stream, unsigned int indent );

void             Buildable_setBuilt( Buildable* self, bool built );
void             Buildable_setTested( Buildable* self, bool tested );
void             Buildable_setUpToDate( Buildable* self, bool upToDate ); 
bool             Buildable_isBuilt( const Buildable* self );
bool             Buildable_isTested( const Buildable* self );
bool             Buildable_isUpToDate( const Buildable* self );

// Private functions

void             Buildable_init( Buildable* self, BuildManager* bm, const IDirectory* source );
void             Buildable_final( Buildable* self );
void             Buildable_setType( Buildable* self, int buildableType );

//  Virtual functions
bool             Buildable_buildTo( const Buildable* self, const IDirectory* target, const BuildParameters* parameters, int depth );

// private structure
struct _Buildable
{
	// public virtual functions
	void (*free)( Buildable* );
	bool (*buildTo)( const Buildable* self, const IDirectory*, const BuildParameters*, int depth );

	// private members
	BuildManager* bm;

	int type;
	char* name;
	char* suiteName;
		
	const IDirectory* directory;
	ISet* children;

	//const Buildable* parent;
	//const Buildable* child;
	//const Buildable* sibling;
	
	bool   upToDate;
	bool   built;
	bool   tested;
	
};

#endif
