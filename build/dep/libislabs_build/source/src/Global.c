/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/Global.h"
#include "islabs/build/ProviderRegistry.h"

#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/CharStringList.h>
#include <openocl/base/psa/Environment.h>
#include <openocl/io/psa/Path.h>
#include <openocl/io/psa/Directory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char* find_provider_dir( const char* exe_dir, char ifs );
static char* determine_os_type();
static char* determine_os_type_using_dodgy_kludges();
static char* to_truncated_alpha_string( const char* string );

Global*
Global_new( const Arguments* args )
{
	Global* global;
	int inVS;
	if ( getenv( "VSINSTALLDIR" ) )
	{
		inVS = 1;
	} else {
		inVS = 0;
	}
	{
		global = (Global*) CRuntime_calloc( 1, sizeof( Global ) );
		
	#ifndef EMBED_PROVIDERS
		{
			const char* command  = Arguments_getCommand( args );
			IEnvironment* environment  = new_Environment( command );
			{
				const char* exe_dir = Environment_getDirectoryContainingExecutable( environment );
				char        ifs     = Environment_GetFileSeparator();
				global->providerDirectory = find_provider_dir( exe_dir, ifs );
				//fprintf( stdout, "Providerdir: %s\n", global->providerDirectory );
			}
			free_Environment( environment );
		}
	#endif
	
		if ( NULL == (global->OSTYPE = determine_os_type()) )
		{
			global->OSTYPE = new_CharString( "unknown" );
		}

		if ( NULL == ( global->PLATFORM = CharString_copy( (char*) getenv( "PLATFORM" ) ) ) )
		{
			if ( 0 == CharString_compare( global->OSTYPE, "windows" ) )
			{
				global->PLATFORM = new_CharString( "win32" );
			}
			else if ( 0 == CharString_compare( global->OSTYPE, "linux-gnu" ) )
			{
				global->PLATFORM = new_CharString_format_args( "%s:%s", global->OSTYPE, "posix:x11" );
			}
			else if ( CharString_startsWith( global->OSTYPE, "darwin" ) )
			{
				global->PLATFORM = new_CharString_format_args( "%s:%s", global->OSTYPE, "posix:x11:carbon" );
			}
			else if ( CharString_startsWith( global->OSTYPE, "solaris" ) )
			{
				global->PLATFORM = new_CharString_format_args( "%s:%s", global->OSTYPE, "posix:x11" );
			}
		}
	
		if ( NULL == ( global->HOSTTYPE = CharString_copy( (char*) getenv( "HOSTTYPE" ) ) ) )
		{
			global->HOSTTYPE = CharString_copy( "i386" );
		}
	
		if ( NULL == ( global->CC = CharString_copy( (char*) getenv( "CC" ) ) ) )
		{
			if ( inVS )
			{
				global->CC = CharString_copy( "cl" );
			} else {
				global->CC = CharString_copy( "gcc" );
			}
		}
	
		if ( NULL == ( global->C_PLUS_PLUS = CharString_copy( (char*) getenv( "C_PLUS_PLUS" ) ) ) )
		{
			if ( inVS )
			{
				global->C_PLUS_PLUS = CharString_copy( "cl" );
			} else {
				global->C_PLUS_PLUS = CharString_copy( "g++" );
			}
		}
	
		if ( NULL == ( global->JAVAC = CharString_copy( (char*) getenv( "JAVAC" ) ) ) )
		{
			if ( inVS )
			{
				global->JAVAC = CharString_copy( "javac" );
			} else {
				global->JAVAC = CharString_copy( "javac" );
			}
		}
	
		if ( NULL == ( global->LD = CharString_copy( (char*) getenv( "LD" ) ) ) )
		{
			if ( inVS )
			{
				global->LD = CharString_copy( "link" );
			}
			else {
				global->LD = CharString_copy( "g++" );
			}
		}
	
		if ( NULL == ( global->AR = CharString_copy( (char*) getenv( "AR" ) ) ) )
		{
			if ( inVS )
			{
				global->AR = CharString_copy( "lib" );
			} else {
				global->AR = CharString_copy( "ar" );
			}
		}
	
		if ( NULL == ( global->JAR = CharString_copy( (char*) getenv( "JAR" ) ) ) )
		{
			if ( inVS )
			{
				global->JAR = CharString_copy( "jar" );
			} else {
				global->JAR = CharString_copy( "jar" );
			}
		}
	
		global->registry = new_ProviderRegistry( global->providerDirectory );
	
		global->CPPFLAGS = new_List();
		global->CFLAGS = new_List();
		global->LFLAGS = new_List();
		global->LDFLAGS = new_List();
		global->LIBFLAGS = new_List();
		global->JFLAGS = new_List();
	
		global->DLLList = new_List();
		global->builtPackages = new_List();
	
		{
		char* cppflags = CharString_copy( getenv( "CPPFLAGS" ) );
		char* cflags   = CharString_copy( getenv( "CFLAGS" ) );
		char* lflags   = CharString_copy( getenv( "LFLAGS" ) );
		char* ldflags  = CharString_copy( getenv( "LDFLAGS" ) );
		char* libflags = CharString_copy( getenv( "LIBFLAGS" ) );
		char* jflags   = CharString_copy( getenv( "JFLAGS" ) );
	
		const char* token;
		if ( cppflags )
		{
			token = (const char*) strtok( cppflags, " " );
			while ( NULL != token )
			{
				List_copyItem( global->CPPFLAGS, token );
				token = (const char*) strtok( NULL, " " );
			}
			free_CharString( cppflags );
		} 
		List_addList( global->CPPFLAGS, Arguments_getDefines( args ) );
	
		if ( cflags )
		{
			token = (const char*) strtok( cflags, " " );
			while ( NULL != token )
			{
				List_copyItem( global->CFLAGS, token );
				token = (const char*) strtok( NULL, " " );
			}
			free_CharString( cflags );
		}
	
		if ( lflags )
		{
			token = (const char*) strtok( lflags, " " );
			while ( NULL != token )
			{
				List_copyItem( global->LFLAGS, token );
				token = (const char*) strtok( NULL, " " );
			}
			free_CharString( lflags );
		}
	
		if ( ldflags )
		{
			token = (const char*) strtok( ldflags, " " );
			while ( NULL != token )
			{
				List_copyItem( global->LDFLAGS, token );
				token = (const char*) strtok( NULL, " " );
			}
			free_CharString( ldflags );
		}
	
		if ( libflags )
		{
			token = (const char*) strtok( libflags, " " );
			while ( NULL != token )
			{
				List_copyItem( global->LIBFLAGS, token );
				token = (const char*) strtok( NULL, " " );
			}
			free_CharString( libflags );
		}
	
		if ( jflags )
		{
			token = (const char*) strtok( jflags, " " );
			while ( NULL != token )
			{
				List_copyItem( global->JFLAGS, token );
				token = (const char*) strtok( NULL, " " );
			}
			free_CharString( jflags );
		}
		}
	
		global->verbosity = Arguments_getVerbosity( args );
		global->indentation = -1;
		global->copyLibs = Arguments_isCopyLibs( args );
		global->recursive = Arguments_isRecursive( args );
		global->no = Arguments_isNo( args );
		global->buildStatic = Arguments_isBuildStatic( args );
		global->debug = Arguments_isDebug( args );
		global->debugLib = Arguments_isDebugLibc( args );
		global->release = Arguments_isRelease( args );
		global->monolithic = Arguments_isMonolithic( args );
		global->withProfiling = Arguments_isWithProfiling( args );
		
		if ( Arguments_isLog( args ) )
		{
			global->logFile = fopen( "build.log", "a+" );
		}
	}
	return global;
}

void
Global_delete( Global* self )
{
	free_CharString( self->PLATFORM );
	free_CharString( self->OSTYPE );
	free_CharString( self->HOSTTYPE );

	free_CharString( self->CC );
	free_CharString( self->C_PLUS_PLUS );
	free_CharString( self->JAVAC );
	free_CharString( self->LD );
	free_CharString( self->AR );
	free_CharString( self->JAR );

	free_CharString( self->providerDirectory );

	free_List( self->CPPFLAGS );
	free_List( self->CFLAGS );
	free_List( self->LFLAGS );
	free_List( self->LDFLAGS );
	free_List( self->LIBFLAGS );
	free_List( self->JFLAGS );
	free_List( self->DLLList );
	free_List( self->builtPackages );
	
	free_ProviderRegistry( self->registry );
	
	if ( self->logFile ) fclose( self->logFile );

	CRuntime_free( self );
}

void
Global_increment( const Global* self )
{
	((Global*)self)->indentation++;
}

void
Global_decrement( const Global* self )
{
	((Global*)self)->indentation--;
}

void
Global_print( const Global* self )
{
	printf( "\t<global>\n" );
	printf( "\t\tOSTYPE=%s\n", self->OSTYPE );
	printf( "\t\tPLATFORM=%s\n", self->PLATFORM );
	printf( "\t\tBUILD_PATH=%s\n", Environment_GetEnvironmentValue( "BUILD_PATH" ) );
	printf( "\t\tCC=%s\n", self->CC );
	printf( "\t\tC_PLUS_PLUS=%s\n", self->C_PLUS_PLUS );
	printf( "\t\tLD=%s\n", self->LD );
	printf( "\t\tAR=%s\n", self->AR );

	printf( "\t\tCPPFLAGS:\n" );
	List_print( self->CPPFLAGS );

	printf( "\t\tCFLAGS:\n" );
	List_print( self->CFLAGS );

	printf( "\t\tLDFLAGS:\n" );
	List_print( self->LDFLAGS );

	printf( "\t\tJFLAGS:\n" );
	List_print( self->JFLAGS );

	printf( "\t</global>\n" );
}

void
Global_tab( const Global* self, FILE* stream )
{
	//unsigned int i;
	//for ( i=0; i < self->indentation; i++ )
	//{
	//	fprintf( stream, "\t" );
	//}
}

void
Global_clearline( const Global* self, FILE* stream, unsigned int nr )
{
	unsigned int i;
	fprintf( stream, "\r" );
	for ( i=0; i < nr; i++ )
	{
		fprintf( stdout, " " );
	}
	fprintf( stream, "\r" );
}

//
//	Returns provider dir, or null if neither option exists.
//
static char* find_provider_dir( const char* exe_dir, char ifs )
{
	//	$exe_dir/../providers/lib
	char* ret = NULL;
	char* option_one = new_CharString_format_args( "%s%c%s%c%s%c%s", exe_dir, ifs, "..", ifs, "providers", ifs, "lib" );

	//	$exe_dir/../libexec/build/providers
	char* option_two = new_CharString_format_args( "%s%c%s%c%s%c%s%c%s", exe_dir, ifs, "..", ifs, "libexec", ifs, "build", ifs, "providers" );

	IDirectory* dir_one = new_Directory( option_one );
	IDirectory* dir_two = new_Directory( option_two );
	if ( Directory_exists( dir_one ) )
	{
		ret = option_one;
		free_CharString( option_two );
	}
	else if ( Directory_exists( dir_two ) )
	{
		ret = option_two;
		free_CharString( option_one );
	} else {
		free_CharString( option_one );
		free_CharString( option_two );
	}
	free_Directory( dir_one );
	free_Directory( dir_two );

	return ret;
}

static char* determine_os_type()
{
	char* ostype = NULL;
	const char* OS     = getenv( "OS" );
	const char* OSTYPE = getenv( "OSTYPE" );

	if ( OS && (0 == strcmp( OS, "Windows_NT" )) )
	{
		ostype = CharString_copy( "windows" );
	}
	else if ( OSTYPE )
	{
		ostype = to_truncated_alpha_string( OSTYPE );
	}
	else
	{
		ostype = determine_os_type_using_dodgy_kludges();
	}
	return ostype;
}

static char* determine_os_type_using_dodgy_kludges()
{
	char* ostype = NULL;
	const char* HOME = getenv( "HOME" );
	
	if ( HOME && (0 == strncmp( HOME, "/Users", 6 )) )
	{
		ostype = CharString_copy( "darwin" );
	}
	else
	{
		ostype = CharString_copy( "unknown" );
	}

	return ostype;
}

static char* to_truncated_alpha_string( const char* string )
{
	char* truncated = CharString_copy( string );
	int   len       = strlen( string );
	int   i;
	for ( i=0; i < len; i++ )
	{
		if ( isdigit( truncated[i] ) )
		{
			truncated[i] = '\0';
			break;
		}
	}
	return truncated;
}




