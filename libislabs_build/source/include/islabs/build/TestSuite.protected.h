/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_TESTSUITE_PROTECTED_H
#define ISLABS_BUILD_TESTSUITE_PROTECTED_H

#include "islabs/build.h"
#include "islabs/build.private.h"
#include "islabs/build/Suite.protected.h"
#include <openocl/base/psa/Index.h>

// public functions
TestSuite* new_TestSuite( BuildManager* bm, const IDirectory* dir );
void   free_TestSuite( TestSuite* self );
bool   TestSuite_buildTo( const TestSuite* self, const IDirectory* target, const BuildParameters* params, int depth );

// private functions
void TestSuite_init( TestSuite* self, BuildManager* bm, const IDirectory* source );
void TestSuite_final( TestSuite* self );

struct _TestSuite
{
	Suite super;

	// virtual
	void (*free)( TestSuite* );
	bool (*buildTo)( const TestSuite* self, const IDirectory*, const BuildParameters*, int );
};

#endif
