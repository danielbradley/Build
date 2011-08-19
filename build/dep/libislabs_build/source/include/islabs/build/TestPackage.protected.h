/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_TESTPACKAGE_PROTECTED_H
#define ISLABS_BUILD_TESTPACKAGE_PROTECTED_H

#include "islabs/build.h"
#include "islabs/build.private.h"
#include "islabs/build/Package.protected.h"
#include "islabs/build/ProviderContext.h"
#include <openocl/base/psa/Index.h>

TestPackage* new_TestPackage( BuildManager* bm, const IDirectory* source );
void     free_TestPackage( TestPackage* self );
bool     TestPackage_buildTo( const TestPackage* self, const IDirectory* target, const BuildParameters* parameters, int depth );

// private functions
void TestPackage_init( TestPackage* self, BuildManager* bm, const IDirectory* source );
void TestPackage_final( TestPackage* self );
//bool TestPackage_buildPackage( const TestPackage* self, const IDirectory* target, const BuildParameters* parameters );
bool TestPackage_runPackage( const IDirectory* target );

struct _TestPackage
{
	Package super;

	// virtual
	void (*free)( TestPackage* );
	bool (*buildTo)( const TestPackage* self, const IDirectory*, const BuildParameters*, int );
};

#endif
