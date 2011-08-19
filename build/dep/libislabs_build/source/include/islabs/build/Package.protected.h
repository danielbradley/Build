/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_PACKAGE_PROTECTED_H
#define ISLABS_BUILD_PACKAGE_PROTECTED_H

#include "islabs/build.h"
#include "islabs/build.private.h"
#include "islabs/build/Buildable.protected.h"
///ProviderContext.h"
#include <openocl/base/psa/Index.h>
#include <openocl/io/psa/Directory.h>

Package*
new_Package( BuildManager* bm, const IDirectory* source );
void     free_Package( Package* self );
bool     Package_buildTo( const Package* self, const IDirectory* target, const BuildParameters* parameters, int depth );

// private functions
void Package_init( Package* self, BuildManager* bm, const IDirectory* source );
void Package_final( Package* self );
bool Package_buildChildren( const Package* self, const IDirectory* target, const BuildParameters* parameters, int depth );
bool Package_buildPackage( const Package* self, const IDirectory* target, const BuildParameters* parameters );

// private class side functions
IList* Package_ReorderAndReduceLibList( const IList* libs );
void Package_DetermineDependencies( ProviderContext* context, const IList* dep_list );
void Package_DetermineLibraries( ProviderContext* context, const char* lib_dir, const char* relative_lib_dir );
void Package_AddPrivateHeaderDirs( IList* includeDirsList );
bool Package_isUpToDate( Package* self, const IDirectory* location );

struct _Package
{
	Buildable super;

	// virtual
	void (*free)( Package* );
	bool (*buildTo)( const Package* self, const IDirectory*, const BuildParameters*, int );
	
	// members
	const Buildable* testSuite;
	bool             includePrivateHeaders;

	unsigned long long timeOfLastBuild;
	unsigned long long timeOfLastPublicHeaderModified;
	unsigned long long timeOfLastPrivateHeaderModified;
	unsigned long long timeOfOldestObjectFileModified;

	bool forceBuild;	//	Set to true when private headers have changed
				//	necessitating build of only this package.
};

#endif
