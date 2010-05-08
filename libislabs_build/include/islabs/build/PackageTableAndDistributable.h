/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_PACKAGETABLE_AND_DISTRIBUTABLE_H
#define ISLABS_BUILD_PACKAGETABLE_AND_DISTRIBUTABLE_H

#include "islabs/build.h"
#include "islabs/build/BuildParameters.h"
#include "islabs/build/Global.h"

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

/*
 *	The Package Table maps between package locations
 *	and actual PACKAGE type Distributable objects instances.
 *
 *	"/local/source/package-1.0" --> Distributable*
 *
 *	The Package Table provides a way to find a previously
 *	create PACKAGE type Distributable* object  - this happens
 *	when a package is encountered that is a dependency of more
 *	than one package.
 */
 
PackageTable* new_PackageTable();
void free_PackageTable( PackageTable* self );

void PackageTable_add( PackageTable* self, Distributable* dep );
void PackageTable_clear( PackageTable* self );
int  PackageTable_contains( const PackageTable* self, const Distributable* dep );

const Distributable* PackageTable_findEquivalent( const PackageTable* self, const Distributable* dep ); 


Distributable* new_Distributable( Distributable* parent, const char* directory_loc );
void free_Distributable( Distributable* self );

void Distributable_setType( Distributable* self, int aType );

int    Distributable_addNewDependenciesTo( Distributable* self, PackageTable* table );
int          Unknown_addNewDependenciesTo( Distributable* self, PackageTable* table );
int          Package_addNewDependenciesTo( Distributable* self, PackageTable* table );
int     Dependencies_addNewDependenciesTo( Distributable* self, PackageTable* table );
int        TestSuite_addNewDependenciesTo( Distributable* self, PackageTable* table );
int      TestPackage_addNewDependenciesTo( Distributable* self, PackageTable* table );
int TestDependencies_addNewDependenciesTo( Distributable* self, PackageTable* table );

int  Distributable_buildTo( const Distributable* self, const BuildParameters* parameters, const char* targetDir, int depth );
int  Distributable_buildTestsTo( const Distributable* self, const BuildParameters* parameters, const char* targetDir );
int  Distributable_runTestsFrom( const Distributable* self, const BuildParameters* parameters, const char* targetDir );
void Distributable_printTree( const Distributable* self, unsigned int indent, int* lines );

const char* Distributable_getLocation( const Distributable* self );

#ifdef __cplusplus
};};};
#endif

#endif
