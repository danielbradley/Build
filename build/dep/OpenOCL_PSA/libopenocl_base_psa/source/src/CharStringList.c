/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/CharStringList.h"

#include <stdlib.h>

static const unsigned int DEFAULT_LIST_SIZE = 512;

char** new_CharStringList_default()
{
	char** self = CRuntime_calloc( DEFAULT_LIST_SIZE + 1, sizeof( char* ) );
	return self;
}

char** new_CharStringList( const char** list )
{
	unsigned int required_list_length = CharStringList_getLength( list ) + 1;
	char** self = CRuntime_calloc( required_list_length, sizeof( char* ) );
	char** tmp = self;

	const char** tmp_list = list;
	while( NULL != *tmp_list )
	{
		*tmp = new_CharString( *tmp_list );
		tmp++;
		tmp_list++;
	}
	tmp = NULL;
	return self;
}

void free_CharStringList( char** self )
{
	char** tmp = self;
	while ( NULL != *tmp )
	{
		free_CharString( *tmp );
		tmp++;
	}
	CRuntime_free( self );
}

int CharStringList_copyItem( char** self, const char* item )
{
	int status = 0;
	unsigned int i;
	for ( i=0; i < DEFAULT_LIST_SIZE; i++ )
	{
		if ( NULL == self[i] )
		{
			self[i] = CharString_copy( item );
			status = 1;
			break;
		}
	}
	return status;
}

unsigned int CharStringList_getLength( const char** self )
{
	unsigned int count = 0;
	const char** tmp = self;
	while ( NULL != *tmp )
	{
		tmp++;
		count++;
	}
	return count;
}

char* CharStringList_toCharString( const char** self )
{
	char* str = CharString_copy( "" );
	char* tmp = CharString_copy( "" );

	unsigned int i=0;
	while ( self[i] )
	{
		CRuntime_free( tmp );
		tmp = str;
		str = CharString_cat2( tmp, self[i] );
		CRuntime_free( tmp );
		tmp = str;
		str = CharString_cat2( tmp, " " );
		i++;
	}
	CRuntime_free( tmp );
	return str;
}
