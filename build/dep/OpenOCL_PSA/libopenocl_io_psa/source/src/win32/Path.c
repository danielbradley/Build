/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#include "openocl/io/psa/Path.private.h"

#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Stack.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
IPath* new_Path( const char* location )
{
	IPath* self = (IPath*) calloc( 1, sizeof( IPath ) );
	self->common = CharString_removeWhitespace( location );
	return self;
}
*/
/*
void free_Path( IPath* self )
{
	if ( self->common ) free( self->common );
	if ( self->dirname ) free( self->dirname );
	if ( self->basename ) free( self->basename );
	if ( self->extension ) free( self->extension );
	if ( self->native ) free( self->native );
}
*/
/*
IPath* Path_copy( const IPath* self )
{
	return new_Path( self->common );
}
*/
/*
const char* Path_getCommon( const IPath* self )
{
	return self->common;
}
*/
/*
const char* Path_getCondensed( const IPath* self )
{
	if ( NULL == self->condensed )
	{
		IPath* writeable = (IPath*) self;
		writeable->condensed = Path_CondensePath( self->common );
	}
	return self->condensed;
}
*/

/*
const char* Path_getExtension( const IPath* self )
{
	if ( NULL == self->extension )
	{
		IPath* writeable = (IPath*) self;
		
		char* tmp = NULL;
		unsigned int length = CharString_getLength( self->common );
		unsigned int i;
		for ( i = length - 1; i >= 0; i-- )
		{
			if ( '.' == self->common[i] )
			{
				tmp = &(self->common[i+1]);
				break;
			}
		}
		writeable->extension = CharString_copy( tmp );
	}
	return self->extension;
}
*/

const char* Path_getNativeFormat( const IPath* self )
{
	if ( NULL == self->native )
	{
		IPath* writeable = (IPath*) self;
		writeable->native = new_CharString( Path_getAbsolute( self ) );
		CharString_replace( writeable->native, '/', '\\' );
	}
	return self->native;
}
/*
bool Path_hasExtension( const IPath* self, const char* extension )
{
	int status = 0;

	char* copy = CharString_copy( self->common );
	unsigned int len = strlen( copy );
	for ( len = len - 1; len >= 0; len-- )
	{
		if ( '/' == copy[len] ) {
			copy[len] = '\0';
		} else {
			break;
		}
	}
	
	if ( CharString_endsWith( copy, extension ) )
	{
	 	status = 1;
	}
	free( copy );
	return status;
}
*/
/*
bool Path_isAbsolute( const IPath* self )
{
	int absolute = 0;
	if ( '/' == *self->common )
	{
		absolute = 1;
	} else {
		const char* name = self->common;

		while ( '\0' != *name )
		{
			if ( (':' == *name) && ('/' == *(++name)) )
			{
				absolute = 1;
				break;
			}
			name++;
		}
	}
	return absolute;
}
*/

char* Path_retrieveLinkTarget( const IPath* self )
{
	char* target = new_CharString( Path_getAbsolute( self ) );
	return target;
}

bool Path_isLink( const IPath* self )
{
	bool link = 0;
	return link;
}

IPath* Path_GetCurrentDirectory()
{
	IPath* path;
	char* hostname = (char*) CRuntime_calloc( MAX_COMPUTERNAME_LENGTH + 1026, sizeof( char ) );
	char* current_dir = (char*) CRuntime_calloc( 1024, sizeof( char ) );
	char* volume = NULL;
	char* unc_name = NULL;

	DWORD index = MAX_COMPUTERNAME_LENGTH + 1;
	hostname[0] = '\\';
	hostname[1] = '\\';
	GetComputerName( &hostname[2], &index );
	index = CharString_getLength( hostname );
	hostname[index] = '\\';
    index++;

	current_dir = (char*) _getcwd( current_dir, 1024 );
	index = CharString_indexOfNext( current_dir, 0, ':' );
	volume = CharString_substring_start_length( current_dir, 0, CharString_CountPositions( 0, index - 1 ) );

	unc_name = CharString_cat3( hostname, volume, &current_dir[index+1] );

	path = new_Path( unc_name );

	CRuntime_free( hostname );
	CRuntime_free( current_dir );
	free_CharString( volume );
	free_CharString( unc_name );

	return path;
}

bool Path_SetCurrentDirectory( const char* location )
{
	return !_chdir( location );
}

/*
char* Path_CondensePath( const char* path )
{
	char* canonical_path = NULL;

	IStack* stack = new_Stack();
	
	int now  = 0;
	int last = -1;
	
	while ( '\0' != path[now] )
	{
		switch ( path[now] )
		{
		case '/':
			if ( (now != last) && (now != (last + 1) ) )
			{
				char* token = CharString_substring( path, last + 1, now - 1 );
				if ( 0 == CharString_compare( token, "." ) )
				{
					free( token );
				}
				else if ( 0 == CharString_compare( token, ".." ) )
				{
					free( token );
					free( Stack_pop( stack ) );
				}
				else
				{
					Stack_addItem( stack, token );
				}
			}
			last = now;
			// Intentional fall through
		default:
			now++;
		}
	}

	if ( (now != last) && (now != (last + 1) ) )
	{
		char* token = CharString_substring( path, last + 1, now - 1 );
		if ( 0 == CharString_compare( token, "." ) )
		{
			free( token );
		}
		else if ( 0 == CharString_compare( token, ".." ) )
		{
			free( token );
			free( Stack_pop( stack ) );
		}
		else
		{
			Stack_addItem( stack, token );
		}
	}

	{
	const char** cs_list = Stack_getAsCharStringList( stack );
	char* tmp = new_CharString_delimiter( cs_list, '/' );

	if ( '/' == path[0] )
	{
		canonical_path = CharString_cat2( "/", tmp );
		free( tmp );
	} else {
		canonical_path = tmp;
	}
	}
	free_Stack( stack );

	return canonical_path;
}
*/
char* Path_ConvertToNative( const char* path )
{
	char* native = CharString_copy( path );
	CharString_replace( native, '/', '\\' );
	return native;
}

IList* Path_ConvertListToNative( const IList* list )
{
	IList* native_list = new_List();

	char* quoted_native;
	char* tmp;
	unsigned int count = List_count( list );
	unsigned int i;
	for ( i=0; i < count; i++ )
	{
		quoted_native = Path_ConvertToNative( list->items[i] );
		if ( '\"' != quoted_native[0] )
		{
			tmp = quoted_native;
			quoted_native = CharString_cat3( "\"", tmp, "\"" );
			free( tmp );
		}
		List_addItem( native_list, quoted_native );
	}
	return native_list;
}

int
Path_getType( const IPath* self )
{
	struct _stat buffer;
	const char* location;
	dev_t m;

	switch ( self->type )
	{
	case OPENOCL_IO_PSA_TYPE_NONE:
	case OPENOCL_IO_PSA_TYPE_UNKNOWN:
		location = Path_getNativeFormat( self );
		if ( 0 == _stat( location, &buffer ) )
		{
			m = buffer.st_mode;

			switch(	m &	S_IFMT )
			{
			case S_IFDIR:
				((IPath*)self)->type = OPENOCL_IO_PSA_TYPE_FS_DIRECTORY;
				break;
			case S_IFREG:
				((IPath*)self)->type = OPENOCL_IO_PSA_TYPE_FS_FILE;
				break;
			default:
				((IPath*)self)->type = OPENOCL_IO_PSA_TYPE_UNKNOWN;
				break;
			}
		}
	}
	return self->type;
}

//char
//Path_GetFileSeparator()
//{
//	return '\\';
//}

