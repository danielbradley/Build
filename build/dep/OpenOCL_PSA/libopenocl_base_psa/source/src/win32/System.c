/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#include <direct.h>
#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/System.h"

char*
System_CurrentDirectory()
{
	int   size = 128;
	char* tmp = (char*) CRuntime_calloc( size, sizeof( char ) );

	while ( !_getcwd( tmp, size ) )
	{
		CRuntime_free( tmp );
		size *= 2;
		tmp = CRuntime_calloc( size, sizeof( char ) );
	}
	return tmp;
}

char*
System_CurrentVolume()
{
	char* current_directory = System_CurrentDirectory();
	char* volume = CharString_token( current_directory, 0, ':' );
	free_CharString( current_directory );
	return volume;
}

char*
System_Hostname()
{
	DWORD max = MAX_COMPUTERNAME_LENGTH + 1;
	char* hostname = (char*) CRuntime_calloc( max, sizeof( char ) );
	GetComputerName( hostname, &max );
	return hostname;
}
