/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_BUILDABLE_H
#define ISLABS_BUILD_BUILDABLE_H

#include "islabs/build.h"
#include "islabs/build/BuildParameters.h"
#include "islabs/build/BuildManager.h"
#include <openocl/base/psa/Index.h>
#include <openocl/io/psa/Directory.h>

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

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
#define TEST_PACKAGE       0x0241

/*
 *	Collection
 *		Software Suite
 *			User Package
 *				Test Suite
 *					Test Package
 */

const Buildable* Buildable_getInstance( BuildManager* bm, const IDirectory* aDirectory );

void             Buildable_setParent( Buildable* self, const Buildable* parent );
void             Buildable_addSibling( Buildable* self, const Buildable* sibling );
void             Buildable_addChild( Buildable* self, const Buildable* child );

BuildManager*    Buildable_getBuildManager( const Buildable* self );
int              Buildable_getType( const Buildable* self );
const char*      Buildable_getTypeName( const Buildable* self );
const char*      Buildable_getName( const Buildable* self );
const char*      Buildable_getSuiteName( const Buildable* self );
void             Buildable_printTreeTo( const Buildable* self, FILE* stream, unsigned int indent );

//  Virtual functions
bool          Buildable_buildTo( const Buildable* self, const IDirectory* target, const BuildParameters* parameters, int depth );

//	Static functions
int Buildable_GetNumberOfSuitesEncountered();

// public structure
//struct _Buildable
//{
//	// public virtual functions
//	void (*free)( Buildable* );
//	bool (*buildTo)( const Buildable* self, const IDirectory*, const BuildParameters* );
//};

#ifdef __cplusplus
};};};
#endif

#endif
