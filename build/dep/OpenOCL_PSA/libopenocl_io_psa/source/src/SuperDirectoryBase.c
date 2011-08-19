/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/io/psa/Directory.h"
#include "openocl/io/psa/Path.h"
#include "openocl/io/psa/SuperDirectory.private.h"

ISuperDirectory* new_SuperDirectory()
{
	ISuperDirectory* self = CRuntime_calloc( 1, sizeof( ISuperDirectory ) );
	SuperDirectory_refresh( self );
	return self;
}

void free_SuperDirectory( ISuperDirectory* self )
{
	if ( self->volumes ) free_List( self->volumes );
	CRuntime_free( self );
}

IDirectory*
SuperDirectory_subdirectory( const ISuperDirectory* self, const char* name )
{
	IDirectory* subdir = NULL;
	IPath* path = new_Path( name );
	if ( OPENOCL_IO_PSA_TYPE_FS_DIRECTORY == Path_getType( path ) )
	{
		subdir = new_Directory_path( path );
	}
	free_Path( path );
	return subdir;
}

const IList*
SuperDirectory_getVolumes( const ISuperDirectory* self )
{
	return self->volumes;
}

bool
SuperDirectory_containsVolume( const ISuperDirectory* self, const char* volumeName )
{
	return 1;
}



