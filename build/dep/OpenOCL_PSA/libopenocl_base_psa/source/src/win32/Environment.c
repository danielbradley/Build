/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <Windows.h>
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Environment.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

static const char* EMPTY_STRING = "";
static const int DEFAULT_SIZE = 255;
static char* ENVIRONMENT_VARIABLE = NULL;
bool Environment_private_CheckExistance( const char* path, const char* filename );

void
Environment_SetEnvironmentValue( const char* key, const char* value, int overwrite )
{
	if ( (0 == CharString_compare( "", Environment_GetEnvironmentValue( key ))) || overwrite )
	{
		SetEnvironmentVariable( key, value );
	}
}

const char* Environment_GetEnvironmentValue( const char* variable )
{
	const char* value = EMPTY_STRING;

	int size  = DEFAULT_SIZE;
	int size2 = 0;

	if ( NULL != ENVIRONMENT_VARIABLE ) CRuntime_free( ENVIRONMENT_VARIABLE );
	
	ENVIRONMENT_VARIABLE = (char*) CRuntime_calloc( size, sizeof( char ) );

	if ( size < (size2 = GetEnvironmentVariable( variable, ENVIRONMENT_VARIABLE, size )) )
	{
		CRuntime_free( ENVIRONMENT_VARIABLE );
		ENVIRONMENT_VARIABLE = (char*) CRuntime_calloc( size2, sizeof( char ) );
		size = GetEnvironmentVariable( variable, ENVIRONMENT_VARIABLE, size2 );
	}
	
	if ( size )
	{
		value = ENVIRONMENT_VARIABLE;
	}
	return value;
}

//----------------------------------------------------------------------------
//	private functions
//----------------------------------------------------------------------------

bool
Environment_private_CheckExistance( const char* path, const char* filename )
{
	bool result = 0;
	struct stat buf;
	char* full_path = CharString_cat3( path, "/", filename );

	if ( 0 == stat( full_path, &buf ) )
	{
		result = 1;
	}
	free_CharString( full_path );
	return result;
}

char
Environment_GetFileSeparator()
{
	return '\\';
}

char
Environment_GetPathSeparator()
{
	return ';';
}

bool
Environment_private_IsLink( const char* path )
{
	return 0;
}

char*
Environment_private_ReadLink( const char* path )
{
	return EMPTY_STRING;
}
