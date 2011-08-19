/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/io/psa/Directory.private.h"

#include "openocl/base/psa/CharStringList.h"
#include "openocl/io/psa/Path.h"

#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
IDirectory* new_Directory( const char* location )
{
	IDirectory* self = CRuntime_calloc( 1, sizeof( IDirectory ) );
	self->path = new_Path( location );
	self->contents = new_CharStringList_default();

	if ( Directory_exists( self ) )
	{
		Directory_refresh( self );
	}

	return self;
}
*/
/*
IDirectory* new_Directory_path( const IPath* location )
{
	IDirectory* self = CRuntime_calloc( 1, sizeof( IDirectory ) );
	self->path = Path_copy( location );
	self->contents = new_CharStringList_default();

	if ( Directory_exists( self ) )
	{
		Directory_refresh( self );
	}

	return self;
}
*/
/*
void CRuntime_free_Directory( IDirectory* self )
{
	if ( self->path ) CRuntime_free_Path( self->path );
	if ( self->contents ) CRuntime_free_CharStringList( self->contents );
	free( self );
}
*/

int Directory_refresh( IDirectory* self )
{
	int status = 0;
	struct dirent* entry;
	DIR* dir;
	
	free_CharStringList( self->contents );
	free_List( self->fileList );
	self->contents = new_CharStringList_default();
	self->fileList = new_List();

	if ( Directory_isLink( self ) )
	{
		dir = opendir( Path_getNativeFormat( Link_getPath( self->link ) ) );
	} else {
		dir = opendir( Path_getNativeFormat( self->path ) );
	}
	if ( dir )
	{
		while ( (entry = readdir( dir )) )
		{
			CharStringList_copyItem( self->contents, entry->d_name );
			List_copyItem( self->fileList, entry->d_name );
		}
		closedir( dir );
		status = 1;
	}
	if ( self->alphabetical )
	{
		free_List( self->alphabetical );
		self->alphabetical = NULL;
	}
	return status;
}

/*
const char** Directory_getContents( const IDirectory* self )
{
	return (const char**) self->contents;
}
*/

int Directory_create( const IDirectory* self )
{
	int status = 0;
	mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
	if ( 0 == mkdir( Path_getNativeFormat( self->path ), mode ) )
	{
		status = 1;
	}
	return status;
}

int Directory_delete( const IDirectory* self )
{
	int status = 0;
	if ( 0 == rmdir( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
	return status;
}

bool Directory_isDirectory( const IDirectory* self )
{
	int status = 0;
	DIR* dir = opendir( Path_getNativeFormat( self->path ) );
	if ( dir )
	{
		closedir( dir );
		status = 1;
	}
	return status;
}	

bool Directory_exists( const IDirectory* self )
{
	int status = 0;
	DIR* dir = opendir( Path_getNativeFormat( self->path ) );
	if ( dir )
	{
		closedir( dir );
		status = 1;
	}
	return status;
}
