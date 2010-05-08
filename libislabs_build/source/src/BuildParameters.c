/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/BuildParameters.h"
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/CharStringList.h>

struct _BuildParameters
{
	const   Global* global;
	char*   ostype;
	char*   platform;
	char**  platformSubparts;

	bool all;
	bool buildPackages;
	bool buildTests;
	bool copyLibs;
	bool deep;
	bool force;
	bool full;
	bool log;
	bool monolithic;
	bool near;
	bool no;
	bool recursive;
	bool release;
	bool runTests;
	bool runTestsOnly;
	bool withProfiling;
};

BuildParameters* new_BuildParameters( const Global* global )
{
	BuildParameters* self = CRuntime_calloc( 1, sizeof( BuildParameters ) );
	self->global = global;
	self->ostype = CharString_copy( global->OSTYPE );

	if ( global->PLATFORM )
	{
		bool loop = 1;
		unsigned int start = 0;
		unsigned int end = 0;
		char* token;
		const char* platform = global->PLATFORM;
		
		self->platform = new_CharString( global->PLATFORM );
		self->platformSubparts = new_CharStringList_default(); 
		
		while ( loop )
		{
			switch ( platform[end] )
			{
			case '\0':
				loop = 0;
			case ':':
				if ( start < (end - 1) )
				{
					token = CharString_substring( platform, start, end - 1 );
					CharStringList_copyItem( self->platformSubparts, token );
					free_CharString( token );
				}
				start = end + 1;
			}
			end++;
		}
	}
	
	//  self->copyLibs = self->global->copyLibs;
	//  self->monolithic = self->global->monolithic;
	//  self->withProfiling = self->global->withProfiling;
	return self;
}

void free_BuildParameters( BuildParameters* self )
{
	free_CharString( self->ostype );
	free_CharString( self->platform );
	free_CharStringList( self->platformSubparts );
	CRuntime_free( self );
}

void BuildParameters_setAll( BuildParameters* self, bool all )
{
	self->all = all;
}

void BuildParameters_setBuildPackages( BuildParameters* self, bool buildPackages )
{
	self->buildPackages = buildPackages;
}

void BuildParameters_setBuildTests( BuildParameters* self, bool buildTests )
{
	self->buildTests = buildTests;
}

void BuildParameters_setDeep( BuildParameters* self, bool deep )
{
	self->deep = deep;
}

void BuildParameters_setForce( BuildParameters* self, bool force )
{
	self->force = force;
}

void BuildParameters_setFull( BuildParameters* self, bool full )
{
	self->full = full;
}

void BuildParameters_setLog( BuildParameters* self, bool log )
{
	self->log = log;
}

void BuildParameters_setMonolithic( BuildParameters* self, bool monolithic )
{
	self->monolithic = monolithic;
}

void BuildParameters_setNear( BuildParameters* self, bool near )
{
	self->near = near;
}

void BuildParameters_setNo( BuildParameters* self, bool no )
{
	self->no = no;
}

void BuildParameters_setRecursive( BuildParameters* self, bool recursive )
{
	self->recursive = recursive;
}

void BuildParameters_setRelease( BuildParameters* self, bool release )
{
	self->release = release;
}

void BuildParameters_setRunTests( BuildParameters* self, bool runTests )
{
	self->runTests = runTests;
}

void BuildParameters_setRunTestsOnly( BuildParameters* self, bool runTestsOnly )
{
	self->runTestsOnly = runTestsOnly;
}

const Global* BuildParameters_getGlobal( const BuildParameters* self )
{
	return self->global;
}

const char*   BuildParameters_getOSType( const BuildParameters* self )
{
	return self->ostype;
}

const char* BuildParameters_getPlatform( const BuildParameters* self )
{
	return self->platform;
}

const char** BuildParameters_getPlatformSubparts( const BuildParameters* self )
{
	return (const char**) self->platformSubparts;
}

bool BuildParameters_isAll( const BuildParameters* self )
{
	return self->all;
}

bool BuildParameters_isBuildPackages( const BuildParameters* self )
{
	return self->buildPackages;
}

bool BuildParameters_isBuildTests( const BuildParameters* self )
{
	return self->buildTests;
}

bool BuildParameters_isCopyLibs( const BuildParameters* self )
{
	return self->copyLibs;
}

bool BuildParameters_isDeep( const BuildParameters* self )
{
	return self->deep;
}

bool BuildParameters_isForce( const BuildParameters* self )
{
	return self->force;
}

bool BuildParameters_isFull( const BuildParameters* self )
{
	return self->full;
}

bool BuildParameters_isLog( const BuildParameters* self )
{
	return self->log;
}

bool BuildParameters_isMonolithic( const BuildParameters* self )
{
	return self->monolithic;
}

bool BuildParameters_isNear( const BuildParameters* self )
{
	return self->near;
}

bool BuildParameters_isNo( const BuildParameters* self )
{
	return self->no;
}

bool BuildParameters_isRecursive( const BuildParameters* self )
{
	return self->recursive;
}

bool BuildParameters_isRelease( const BuildParameters* self )
{
	return self->release;
}

bool BuildParameters_isRunTests( const BuildParameters* self )
{
	return self->runTests;
}

bool BuildParameters_isRunTestsOnly( const BuildParameters* self )
{
	return self->runTestsOnly;
}

bool BuildParameters_isWithProfiling( const BuildParameters* self )
{
	return self->withProfiling;
}

unsigned int BuildParameters_getVerbosity( const BuildParameters* self )
{
	return self->global->verbosity;
}

void
BuildParameters_print( const BuildParameters* self )
{
	Global_print( self->global );
}
