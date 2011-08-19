/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Environment.h"
#include "openocl/base/psa/System.h"

//#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
//#include <unistd.h>
#include <sys/stat.h>

struct _IEnvironment
{
	char* executableName;
	char* executableLocation;
	char* directoryContainingExecutable;
	char* path;
};

extern bool  Environment_private_CheckExistance( const char* path, const char* filename );
extern bool  Environment_private_IsLink( const char* path );
extern char* Environment_private_ReadLink( const char* path );

IEnvironment* new_Environment( const char* argv_0 )
{
	IEnvironment* self = NULL;
	char* arg0 = new_CharString( argv_0 );

	char ifs[] = { Environment_GetFileSeparator(), '\0' };

	if ( 0 < CharString_getLength( argv_0 ) )
	{
		self                 = CRuntime_calloc( 1, sizeof( IEnvironment ) );
		self->path           = new_CharString( Environment_GetEnvironmentValue( "PATH" ) );
		self->executableName = CharString_basename( arg0 );
		{
			char* directory = Environment_DirectoryContainingExecutable( arg0, self->path );
			char* executable_location = CharString_cat3( directory, ifs, self->executableName );

			self->executableLocation = Environment_FollowLinks( executable_location, directory );
			self->directoryContainingExecutable = Environment_DirectoryContainingExecutable( self->executableLocation, self->path );

			free_CharString( directory );
			free_CharString( executable_location );
		}
				
	} else {
		abort();
	}

	free_CharString( arg0 );

	return self;
}

void free_Environment( IEnvironment* self )
{
	if ( self->directoryContainingExecutable ) free_CharString( self->directoryContainingExecutable );
	if ( self->executableLocation )            free_CharString( self->executableLocation );
	if ( self->executableName )                free_CharString( self->executableName );
	if ( self->path )                          free_CharString( self->path );

	CRuntime_free( self );
}

const char*
Environment_getExecutableName( const IEnvironment* self )
{
	return self->executableName;
}

const char*
Environment_getExecutableLocation( const IEnvironment* self )
{
	return self->executableLocation;
}

const char*
Environment_getDirectoryContainingExecutable( const IEnvironment* self )
{
	return self->directoryContainingExecutable;
}

const char*
Environment_getPath( const IEnvironment* self )
{
	return self->path;
}

char*
Environment_DirectoryContainingExecutable( const char* executable, const char* PATH )
{
	char* directory = NULL;
	//IPath* path = new_Path( executable );

	char* dirname = CharString_dirname( executable );
	char* basename = CharString_basename( executable );

	int length = CharString_getLength( dirname );
	if ( 0 == length )
	{
		//	If dirname has a length of zero that means the executable
		//	must have been found via the path.
	
		directory = Environment_SearchPathFor( PATH, basename );
	}
	else if ( '/' == dirname[0] )
	{
		//	If the first character is a '/' the command was run on POSIX
		//	using an absolute path.

		directory = new_CharString( dirname );
	}
	else if ( (3 < length) && (':' == dirname[1]) && ('\\' == dirname[2]) )
	{
		//	If the second letter is a ':' and the third a '/' then the command
		//	was run on Windows using an absolute path.

		directory = new_CharString( dirname );
	}
	else
	{
		//	Else the executable must have been run relative to a directory.
	
		char* current = System_CurrentDirectory();
		directory = CharString_cat3( current, "/", dirname );
		free_CharString( current );
	}
	
	free_CharString( dirname );
	free_CharString( basename );
	//free_Path( path );
	return directory;
}

char*
Environment_SearchPathFor( const char* PATH, const char* file )
{
	char* dir    = NULL;
	int   last   = 0;
	int   length = CharString_getLength( PATH );
	
	int i;
	for ( i=0; i <= length; i++ )
	{
		switch ( PATH[i] )
		{
		case ':':
		case '\0':
			if ( (last+1) < i )
			{
				char* p = CharString_substring_start_length( PATH, last, i-last );
				if ( Environment_private_CheckExistance( p, file ) )
				{
					dir = p;
					i = length; // exit loop
				} else {
					free_CharString( p );
					last = i+1;
				}
			}
			break;
		}
	}
	return dir;
}

char*
Environment_FollowLinks( const char* location, const char* directory )
{
	char* loc = new_CharString( location );
	
	while ( Environment_private_IsLink( loc ) )
	{
		char* link = Environment_private_ReadLink( loc );
		free_CharString( loc );
		
		if ( '/' == link[0] ) {
			loc = new_CharString( link );
		} else {
			loc = CharString_cat3( directory, "/", link );
		}
		free_CharString( link );
	}
	return loc;
}

