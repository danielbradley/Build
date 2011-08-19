/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Environment.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

static const char* EMPTY_STRING = "";

bool Environment_private_CheckExistance( const char* path, const char* filename );

void
Environment_SetEnvironmentValue( const char* key, const char* value, int overwrite )
{
	setenv( key, value, overwrite );
}

const char* Environment_GetEnvironmentValue( const char* variable )
{
	const char* value = getenv( variable );
	if ( !value )
	{
		value = EMPTY_STRING;
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
	return '/';
}

char
Environment_GetPathSeparator()
{
	return ':';
}

bool
Environment_private_IsLink( const char* path )
{
	bool link = 0;
	struct stat info;
	lstat( path, &info );
	{
		int type = info.st_mode & S_IFMT;
		if ( S_IFLNK == type )
		{
			link = 1;
		}
	}
	return link;
}

char*
Environment_private_ReadLink( const char* path )
{
	int read = 0;
	int size = 64;
	char* ret = NULL;
	char* tmp = calloc( size, sizeof(char) );
	while ( -1 == (read = readlink( path, tmp, size - 1 )) )
	{
		size *= 2;
		free( tmp );
		tmp = calloc( size, sizeof(char) );
	}
	tmp[read] = '\0';
	ret = new_CharString( tmp );
	free( tmp );

	return ret;
}
