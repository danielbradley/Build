/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#undef byte
#include "openocl/io/psa/Directory.private.h"

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/CharStringList.h"
#include "openocl/base/psa/List.h"
#include "openocl/io/psa/Link.h"
#include "openocl/io/psa/Path.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
IDirectory* new_Directory( const char* location )
{
	IDirectory* self = calloc( 1, sizeof( IDirectory ) );
	self->path = new_Path( location );
	self->contents = new_CharStringList_default();
	self->fileList = new_List();
	self->link = NULL;

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
	IDirectory* self = calloc( 1, sizeof( IDirectory ) );
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
void free_Directory( IDirectory* self )
{
	if ( self->path ) free_Path( self->path );
	if ( self->contents ) free_CharStringList( self->contents );
	free( self );
}
*/
/*
IDirectory* Directory_subdirectory( const IDirectory* self, const char* name )
{
	IDirectory* dir = NULL;
	const char* path = NULL;
	char* path_and_name;

	if ( Directory_isLink( self ) )
	{
		path = Path_getCommon( Link_getPath( self->link ) );
	} else {
		path = Path_getCommon( self->path );
	}
	path_and_name = CharString_cat3( path, "/", name );
	dir = new_Directory( path_and_name );
	free( path_and_name );
	return dir;
}
*/
/*
int Directory_refresh( IDirectory* self )
{
	int status = 0;
	struct dirent* entry;

	DIR* dir = opendir( Path_getNativeFormat( self->path ) );
	if ( dir )
	{
		while ( (entry = readdir( dir )) )
		{
			CharStringList_copyItem( self->contents, entry->d_name );
		}
		closedir( dir );
		status = 1;
	}
	return status;
}
*/
bool Directory_refresh( IDirectory* self )
{
	int status = 0;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;
	char* dir_spec;

	free_CharStringList( self->contents );
	free_List( self->fileList );
	self->contents = new_CharStringList_default();
	self->fileList = new_List();

	if ( Directory_isLink( self ) )
	{
		dir_spec = CharString_cat2( Path_getNativeFormat( Link_getPath( self->link ) ), "\\*" );
	} else {
		dir_spec = CharString_cat2( Path_getNativeFormat( self->path ), "\\*" );
	}

	hFind = FindFirstFile( dir_spec, &FindFileData );

	if ( hFind != INVALID_HANDLE_VALUE )
	{
		CharStringList_copyItem( self->contents, FindFileData.cFileName );
		List_copyItem( self->fileList, FindFileData.cFileName );
		while( FindNextFile( hFind, &FindFileData ) )
		{
			CharStringList_copyItem( self->contents, FindFileData.cFileName );
			List_copyItem( self->fileList, FindFileData.cFileName );
		}

		dwError = GetLastError();
		if ( dwError == ERROR_NO_MORE_FILES )
		{
			FindClose( hFind );
			status = 1;
		}
	}
	free( dir_spec );
	return status;
}
/*
const char** Directory_getContents( const IDirectory* self )
{
	return (const char**) self->contents;
}
*/
/*
const IList* Directory_getFileList( const IDirectory* self )
{
	return self->fileList;
}
*/
/*
const IPath* Directory_getPath( const IDirectory* self )
{
	return self->path;
}
*/
/*
const IPath* Directory_getRealPath( const IDirectory* self )
{
	if ( self->link ) {
		return Link_getPath( self->link );
	} else {
		return self->path;
	}
}
*/
bool Directory_create( const IDirectory* self )
{
	int status = 0;
	const char* pathName = Path_getNativeFormat( self->path );
	if ( CreateDirectory( pathName, NULL ) )
	{
		status = 1;
	}
	return status;
}

bool Directory_delete( const IDirectory* self )
{
	int status = 0;
	if ( RemoveDirectory( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
	return status;
}

bool Directory_isDirectory( const IDirectory* self )
{
	int status = 0;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char* dir_spec = CharString_cat2( Path_getNativeFormat( self->path ), "\\*" );

	if ( INVALID_HANDLE_VALUE != (hFind = FindFirstFile( dir_spec, &FindFileData )) )
	{
		FindClose( hFind );
		status = 1;
	}
	free( dir_spec );
	return status;
}

/*
 *  Taken from:
 *  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/base/file_management_reference.asp
 */
int Directory_exists( const IDirectory* self )
{
	int status = 0;
	const char* path = Path_getNativeFormat( self->path );
	HANDLE hDir = CreateFile(
		path,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL );
	if ( INVALID_HANDLE_VALUE != hDir )
	{
		CloseHandle( hDir );
		status = 1;
	}
	return status;
}

/*
int Directory_isLink( const IDirectory* self )
{
	IDirectory* rw_self = (IDirectory*) self;
	int status = 0;
	
	if ( self->link )
	{
		status = 1;
	}
	else if ( !Directory_exists( self ) )
	{
		rw_self->link = new_Link();
		if ( Link_load( self->link, Path_getCommon( self->path ) ) )
		{
			IDirectory* link_dir = new_Directory_path( Link_getPath( self->link ) );
			if ( Directory_exists( link_dir ) )
			{
				status = 1;
			} else {
				free_Link( self->link );
				rw_self->link = NULL;
			}
			free_Directory( link_dir );
		} else {
			free_Link( self->link );
			rw_self->link = NULL;
		}
	}
	return status;
}
*/
/*
bool Directory_containsDirectory( const IDirectory* self, const char* dirname )
{
	int result = 0;
	char* dir_path = CharString_cat3( Path_getCommon( Directory_getRealPath( self ) ), "/", dirname );
	IDirectory* src_dir = new_Directory( dir_path );
	if ( Directory_exists( src_dir ) )
	{
		result = 1;
	}
	free_Directory( src_dir );
	free( dir_path );
	return result;
}
*/
/*
IList* Directory_filteredFileList( const IDirectory* self, const char* filter )
{
	IList* list = new_List();

	unsigned int count = List_count( self->fileList );
	unsigned int i = 0;
	for ( i=0; i < count; i++ )
	{
		if ( CharString_endsWith( self->fileList->items[i], filter ) )
		{
			List_copyItem( list, self->fileList->items[i] );
		}
	}
	return list;
}
*/

/*
 *	Directory search methods
 *
 *  For each file in this directory:
 *      a) If its name equals "name" add it to the list.
 *      b) If it is a directory recursively search that directory.
 */
/*
IList* Directory_search( IDirectory* self, const char* name )
{
        IList* list = new_List();
        char* prefix = ".";

        Directory_search_private( self, name, prefix, list );

        return list;
}

IList* Directory_search_followLinks( IDirectory* self, const char* name, int absolute )
{
	IList* list = new_List();
	IList* searched = new_List();
	char* prefix = ".";

	Directory_search_breadth_followLinks_private( self, name, prefix, list, absolute, searched );

	free_List( searched );
	return list;
}

IList*
Directory_search_private(
	IDirectory*       self,
	const char*       name,
	const char*       prefix,
	IList*            list )
{
	if ( Directory_refresh( self ) )
	{
		const IList* file_list = Directory_getFileList( self );
		unsigned int count = List_count( file_list );

		unsigned int i;
		const char* file;
		char* next_prefix;
		IDirectory* next_dir;
		for ( i=0; i < count; i++ )
		{
			file = file_list->items[i];

			if ( !CharString_startsWith( file, "." ) )
			{
				next_prefix = CharString_cat3( prefix, "/", file );

				if ( 0 == CharString_compare( file, name ) )
				{
					List_copyItem( list, next_prefix );
				}
				next_dir = Directory_subdirectory( self, file );
				if ( !Directory_isLink( next_dir ) )
				{
					Directory_search_private( next_dir, name, next_prefix, list );
				}
				free_Directory( next_dir );
				free( next_prefix );
			}
		}
	}
	return list;
}

IList*
Directory_search_followLinks_private(
	IDirectory*       self,
	const char*       name,
	const char*       prefix,
	IList*            list,
	int               absolute,
	IList*            searched )
{
	char* canonical_path = CharString_copy( Path_getCondensed( self->path ) );
	if ( Directory_refresh( self ) && List_addItemIfNew( searched, canonical_path ) )
	{
		const IList* file_list = Directory_getFileList( self );
		unsigned int count = List_count( file_list );

		unsigned int i;
		const char* file;
		char* next_prefix;
		IDirectory* next_dir;
		for ( i=0; i < count; i++ )
		{
			file = file_list->items[i];

			if ( !CharString_startsWith( file, "." ) )
			{
				if ( absolute )
				{
					next_prefix = CharString_cat3( Path_getCondensed( Directory_getRealPath( self ) ), "/", file );
				} else {
					next_prefix = CharString_cat3( prefix, "/", file );
				}

				if ( 0 == CharString_compare( file, name ) )
				{
					List_copyItem( list, next_prefix );
				}
				next_dir = Directory_subdirectory( self, file );
				Directory_search_followLinks_private( next_dir, name, next_prefix, list, absolute, searched );

				free_Directory( next_dir );
				free( next_prefix );
			}
		}
	}
	return list;
}

IList* Directory_search_breadth_followLinks_private(
	IDirectory* self,
	const char*      name,
	const char*      prefix,
	IList*           list,
	int              absolute,
	IList*           searched )
{
	char* condensed_path = CharString_copy( Path_getCondensed( self->path ) );

	//
	//	For each file, if it is a directory search it for a file named "name"
	//	using Directory_filteredFileList.
	//
	//	Then for each file, call Directory_search_bredth_followLinks_private.
	// 

	//
	//	For each file do a local search using Directory_filteredFileList
	//
	if ( Directory_refresh( self ) && List_addItemIfNew( searched, condensed_path ) )
	{
		const IList* file_list = Directory_getFileList( self );
		unsigned int count = List_count( file_list );
		const char* file;

		{
			IDirectory* child_dir;
			unsigned int i;
			for ( i=0; i < count; i++ )
			{
				file = file_list->items[i];
			
				if ( !CharString_startsWith( file, "." ) )
				{
					child_dir = Directory_subdirectory( self, file );
					if ( Directory_refresh( child_dir ) && Directory_containsDirectory( child_dir, name ) )
					{
						char* item_loc;

						if ( absolute )
						{
							item_loc = CharString_cat3( Path_getCondensed( Directory_getRealPath( child_dir ) ), "/", name );
						} else {
							char* tmp = CharString_cat3( prefix, "/", file );
							item_loc = CharString_cat3( tmp, "/", name );
							free( tmp );
						}
						List_addItemIfNew( list, item_loc );
					}
					free_Directory( child_dir );
				}
			}
		}

		{
			IDirectory* next_search_dir;
			char* next_prefix;
			unsigned int j;
			for ( j=0; j < count; j++ )
			{
				file = file_list->items[j];
				
				if ( !CharString_startsWith( file, "." ) )
				{
					if ( absolute )
					{
						next_prefix = CharString_cat3( Path_getCondensed( Directory_getRealPath( self ) ), "/", file );
					} else {
						next_prefix = CharString_cat3( prefix, "/", file );
					}
					next_search_dir = Directory_subdirectory( self, file );
					Directory_search_breadth_followLinks_private( next_search_dir, name, next_prefix, list, absolute, searched );

					free_Directory( next_search_dir );
					free( next_prefix );
				}
			}
		}
	}
	return list;
}
*/
