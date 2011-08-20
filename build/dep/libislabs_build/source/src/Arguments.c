/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/Arguments.private.h"

#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/List.h>

#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>
#include <string.h>

Arguments*
Arguments_new( unsigned int argc, char** argv )
{
	Arguments* self = (Arguments*) calloc( 1, sizeof( Arguments ) );
	self->command = new_CharString( argv[0] );
	self->defines = new_List();
	self->verbosity = 0;
	self->debug = 1;		// Debug by default

	{
	unsigned int i;
	for ( i=1; i < argc; i++ )
	{
		if ( '-' == argv[i][0] )
		{
			unsigned int hash = simple_hash( argv[i] );

			switch( hash )
			{
			case DEF:
				i++;
				if ( argv[i] )
				{
					char* definition = new_CharString_format_args( "-D%s", argv[i] );
					List_addItem( self->defines, definition );
				}
				break;
			case V:
				self->verbosity++;
				break;
			case WC:
				break;
			case WL:
				break;
			case NEAR:
				self->near = 1;
				break;
			case N:
			case NO:
				self->no = 1;
				break;
			case ALL:
				self->all   = 1;
				break;
			case LOG:
				self->log = 1;
				break;
			case NEW:
				self->new = 1;
				break;
//			case D:
//			case DEEP:
//				self->deep = 1;
//				if ( self->recursive ) self->invalid = 1;
//				break;
			case HASH:
				i++;
				hash = simple_hash( argv[i] );
				fprintf( stdout, "%s = %u\n", argv[i], hash ); 
				exit( 0 );
				break;
			case HELP:
				self->help = 1;
				break;
			case FAST:
				self->fast = 1;
				break;
			case TREE:
				self->tree = 1;
				break;
			case FULL:
				self->full = 1;
				break;
			case CLEAN:
				self->clean = 1;
				break;
			case G:
			case DEBUG:
				self->debug = 1;
				break;
			case M:
			case MARCH:
				break;
			case F:
			case FORCE:
				self->force = 1;
				break;
			case DEPTH:
				i++;
				self->depth = atoi( argv[i] );
			case WORLD:
				break;
			case STATIC:
				self->buildStatic = 1;
				break;
//			case LEGACY:
//				self->legacy = 1;
//				break;
			case SHARED:
				self->buildShared = 1;
				break;
//			case T:
//			case TARGET:
//				i++;
//				self->target = CharString_copy( argv[i] );
//				break;
			case S:
			case STATIC2:
				self->buildStatic = 1;
				break;
			case RELEASE:
				self->release = 1;
				break;
			case DYNAMIC:
				self->dynamic = 1;
				break;
			case VERSION:
				self->version = 1;
				break;
			case COPY_LIBS:
				self->copyLibs = 1;
				break;
			case RUN_TESTS:
				self->runTests = 1;
				break;
			case DEBUG_LIBC:
				self->debugLibc = 1;
				break;
//			case R:
//			case RECURSIVE:
//				self->recursive = 1;
//				if ( self->deep ) self->invalid = 1;
//				break;
			case MONOLITHIC:
				self->monolithic = 1;
				break;
			case BUILD_TESTS:
				self->buildTests = 1;
				break;
			case RUN_TESTS_ONLY:
				self->runTestsOnly = 1;
				break;
			case WITH_PROFILING:
				self->withProfiling = 1;
				break;
			case BUILD_TESTS_ONLY:
				self->buildTestsOnly = 1;
				break;
			default:
				if ( '-' == argv[i][0] )
				{
					fprintf( stdout, "invalid argument: %s\n\n", argv[i] );
					self->invalid = 1;
				}
			}
		}
		else if ( NULL == self->package_dir )
		{
			self->package_dir = CharString_copy( argv[i] );
		} else {
			self->invalid = 1;
		}
	}
	}

	return self;
}

void
Arguments_delete( Arguments* self )
{
	if ( self->command ) free_CharString( self->command ); 
	if ( self->march ) free( self->march );
	if ( self->target ) free( self->target );
	if ( self->package_dir ) free( self->package_dir );
	if ( self->defines ) free_List( self->defines );
	free( self );
}

const IList*
Arguments_getDefines( const Arguments* self )
{
	return self->defines;
}

const char*
Arguments_getCommand( const Arguments* self )
{
	return self->command;
}

const char*
Arguments_getPackageDir( const Arguments* self )
{
	return self->package_dir;
}

int
Arguments_getDepth( const Arguments* self )
{
	return self->depth;
}

int
Arguments_isTree( const Arguments* self )
{
	return self->tree;
}

const char*
Arguments_getTargetDir( const Arguments* self )
{
	return self->target;
}

unsigned int
Arguments_getVerbosity( const Arguments* self )
{
	return self->verbosity;
}

int
Arguments_isFull( const Arguments* self )
{
	return self->full;
}

int
Arguments_isAll( const Arguments* self )
{
	return self->all;
}

int
Arguments_isDebug( const Arguments* self )
{
	return self->debug;
}

int
Arguments_isDeep( const Arguments* self )
{
	return self->deep;
}

int
Arguments_isLog( const Arguments* self )
{
	return self->log;
}

int
Arguments_isNew( const Arguments* self )
{
	return self->new;
}

int
Arguments_isHelp( const Arguments* self )
{
	return self->help;
}

int
Arguments_isInvalid( const Arguments* self )
{
	return self->invalid;
}

int
Arguments_isNear( const Arguments* self )
{
	return self->near;
}

int
Arguments_isNo( const Arguments* self )
{
	return self->no;
}

int
Arguments_isForce( const Arguments* self )
{
	return self->force;
}

int
Arguments_isLegacy( const Arguments* self )
{
	return self->legacy;
}

int
Arguments_isBuildStatic( const Arguments* self )
{
	return self->buildStatic;
}

int
Arguments_isRelease( const Arguments* self )
{
	return self->release;
}

int
Arguments_isDynamic( const Arguments* self )
{
	return self->dynamic;
}

int
Arguments_isVersion( const Arguments* self )
{
	return self->version;
}

int
Arguments_isCopyLibs( const Arguments* self )
{
	return self->copyLibs;
}

int
Arguments_isRunTests( const Arguments* self )
{
	return self->runTests;
}

int
Arguments_isDebugLibc( const Arguments* self )
{
	return self->debugLibc;
}

int
Arguments_isRecursive( const Arguments* self )
{
	return self->recursive;
}

int
Arguments_isMonolithic( const Arguments* self )
{
	return self->monolithic;
}

int
Arguments_isBuildTests( const Arguments* self )
{
	return self->buildTests;
}

int
Arguments_isRunTestsOnly( const Arguments* self )
{
	return self->runTestsOnly;
}

int
Arguments_isWithProfiling( const Arguments* self )
{
	return self->withProfiling;
}

int
Arguments_isBuildTestsOnly( const Arguments* self )
{
	return self->buildTestsOnly;
}

void
Arguments_discover( unsigned int argc, char** argv )
{
	unsigned int i;
	for ( i=1; i < argc; i++ )
	{
		unsigned int hash = simple_hash( argv[i] );
		printf( "hash( %s ) == %i\n", argv[i], hash );
	}
}

unsigned int simple_hash( const char* string )
{
	unsigned int hash = 0;

	unsigned int len = strlen( string );
	unsigned int i=0;
	while ( i < len )
	{
		if ( '=' == string[i] ) break;
		if ( ',' == string[i] ) break;

		hash += (int) string[i];
		i++;
	}

	return hash;
}
