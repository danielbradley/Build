/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_SUITE_PROTECTED_H
#define ISLABS_BUILD_SUITE_PROTECTED_H

#include "islabs/build.h"
#include "islabs/build.private.h"
#include "islabs/build/Buildable.protected.h"
#include <openocl/base/psa/Index.h>

// public functions
Suite* new_Suite( BuildManager* bm, const IDirectory* dir );
void   free_Suite( Suite* self );
bool   Suite_buildTo( const Suite* self, const IDirectory* target, const BuildParameters* params, int depth );

// private functions
void Suite_init( Suite* self, BuildManager* bm, const IDirectory* source );
void Suite_final( Suite* self );

struct _Suite
{
	Buildable super;

	// virtual
	void (*free)( Suite* );
	bool (*buildTo)( const Suite* self, const IDirectory*, const BuildParameters*, int );

	// non-virtual
	bool (*buildSuite)( const Suite* self, const char*, BuildParameters* );
};

#endif
