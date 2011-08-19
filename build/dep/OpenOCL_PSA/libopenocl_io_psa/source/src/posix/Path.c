/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Stack.h"
#include "openocl/io/psa/Path.private.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

const char* Path_getNativeFormat( const IPath* self )
{
	if ( NULL == self->native )
	{
		IPath* writeable = (IPath*) self;
		writeable->native = new_CharString( Path_getAbsolute( self ) );
		CharString_replace( writeable->native, '\\', '/' );
	}
	return self->native;
}

char* Path_retrieveLinkTarget( const IPath* self )
{
	const char* absolute = Path_getAbsolute( self );
	char* target = NULL;
	if ( OPENOCL_IO_PSA_TYPE_FS_SYMLINK == Path_getType( self ) )
	{
		int read = 0;
		int size = 64;
		char* tmp = calloc( sizeof( char ), size );

		while ( -1 == (read = readlink( absolute, tmp, size - 1 )) )
		{
			size *= 2;
			free( tmp );
			tmp = calloc( sizeof( char ), size );
		}
		
		tmp[read] = '\0';
		
		target = new_CharString( tmp );
		free( tmp );
	} else {
		target = new_CharString( Path_getAbsolute( self ) );
	}
	return target;
}

bool Path_isLink( const IPath* self )
{
	bool link = 0;
	struct stat info;
	if ( (0 == lstat( Path_getAbsolute( self ), &info )) && ( S_IFLNK == (info.st_mode & S_IFMT) ) )
	{
		link = 1;
	}
	return link;
}

IPath* Path_GetCurrentDirectory()
{
	IPath* path;
	const char* protocol = "file";
	char* hostname    = (char*) CRuntime_calloc( 1024, sizeof( char ) );
	char* current_dir = (char*) CRuntime_calloc( 1024, sizeof( char ) );
	char* url;
	
	if ( gethostname( hostname, 1024 ) )
	{
		hostname[0] = '\0';
	}
	current_dir = (char*) getcwd( current_dir, 1024 );

	url = new_CharString_format_args( "%s://%s%s", protocol, hostname, current_dir );
	
	path = new_Path( url );

	free_CharString( url );
	
	CRuntime_free( current_dir );
	CRuntime_free( hostname );

	return path;
}

bool Path_SetCurrentDirectory( const char* location )
{
	return !chdir( location );
}


/*
void Path_ClearDirectoryHistory()
{
	if ( WorkingDirectories )
	{
		free_Stack( WorkingDirectories );
		WorkingDirectories = NULL;
	}
}
*/
char* Path_ConvertToNative( const char* path )
{
	char* native = CharString_copy( path );
	CharString_replace( native, '\\', '/' );
	return native;
}

IList* Path_ConvertListToNative( const IList* list )
{
	IList* native_list = new_List();

	char* native;
	unsigned int count = List_count( list );
	unsigned int i;
	for ( i=0; i < count; i++ )
	{
		native = Path_ConvertToNative( list->items[i] );
		List_addItem( native_list, native );
	}
	return native_list;
}

int
Path_getType( const IPath* self )
{
	//	Warning, if a symbolic link will return the type of
	//	the target file. Use Path_isLink to determine if this
	//	is a symlink.

	IPath* rw;
	struct stat buffer;
	const char* location;

	switch ( self->type )
	{
	case OPENOCL_IO_PSA_TYPE_NONE:
	case OPENOCL_IO_PSA_TYPE_UNKNOWN:
		rw = (IPath*) self;
		location = Path_getNativeFormat( self );
		if ( 0 == stat( location, &buffer ) )
		{
			dev_t m = buffer.st_mode & S_IFMT;

			switch( m )
			{
			case S_IFREG:
				rw->type = OPENOCL_IO_PSA_TYPE_FS_FILE;
				break;
			case S_IFDIR:
				rw->type = OPENOCL_IO_PSA_TYPE_FS_DIRECTORY;
				break;
			case S_IFLNK:
				rw->type = OPENOCL_IO_PSA_TYPE_FS_SYMLINK;
				break;
			case S_IFCHR:
				rw->type = OPENOCL_IO_PSA_TYPE_FS_DEVICE_CHARACTER;
				break;
			case S_IFBLK:
				rw->type = OPENOCL_IO_PSA_TYPE_FS_DEVICE_BLOCK;
				break;
			case S_IFIFO:
				rw->type = OPENOCL_IO_PSA_TYPE_IO_FIFO;
				break;
			case S_IFSOCK:
				rw->type = OPENOCL_IO_PSA_TYPE_IO_SOCKET;
				break;
			default:
				rw->type = OPENOCL_IO_PSA_TYPE_UNKNOWN;
				break;
			}
		} else {
			switch ( errno )
			{
			case EACCES:
				rw->type = OPENOCL_IO_PSA_TYPE_UNKNOWN;
			}
		}
		break;
	}
	return self->type;
}

//char
//Path_GetFileSeparator()
//{
//	return '/';
//}

