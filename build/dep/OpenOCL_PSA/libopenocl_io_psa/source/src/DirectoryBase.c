/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/io/psa/Directory.private.h"

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/CharStringList.h"
#include "openocl/base/psa/List.h"
#include "openocl/base/psa/Sort.h"
#include "openocl/base/psa/Stack.h"
#include "openocl/io/psa/File.h"
#include "openocl/io/psa/Link.h"
#include "openocl/io/psa/Path.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

IDirectory* new_Directory( const char* location )
{
	IDirectory* self = CRuntime_calloc( 1, sizeof( IDirectory ) );
	self->path = new_Path( location );
	self->contents = new_CharStringList_default();
	self->fileList = new_List();

	if ( Directory_exists( self ) || Directory_isLink( self ) )
	{
		Directory_refresh( self );
	}

	return self;
}

IDirectory* new_Directory_path( const IPath* location )
{
	IDirectory* self = CRuntime_calloc( 1, sizeof( IDirectory ) );
	self->path = Path_copy( location );
	self->contents = new_CharStringList_default();
	self->fileList = new_List();

	if ( Directory_exists( self ) || Directory_isLink( self ) )
	{
		Directory_refresh( self );
	}

	return self;
}

void free_Directory( IDirectory* self )
{
	if ( self->path )     free_Path( self->path );
	if ( self->contents ) free_CharStringList( self->contents );
	if ( self->fileList ) free_List( self->fileList );
	if ( self->alphabetical ) free_List( self->alphabetical );
	if ( self->link )     free_Link( self->link );

	if ( self->times.lastAccess )       free_Time( self->times.lastAccess );
	if ( self->times.lastModification ) free_Time( self->times.lastModification );
	if ( self->times.lastChange )       free_Time( self->times.lastChange );

	if ( self->cache ) Cache_destroy( self );

	CRuntime_free( self );
}

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
	free_CharString( path_and_name );
	return dir;
}

void Directory_setCaching( IDirectory* self, bool cache )
{
	Directory_setCaching_index( self, cache, NULL );
}

void Directory_setCaching_index( IDirectory* self, bool cache, IIndex const* anIndex )
{
	if ( self->cache )
	{
		Cache_destroy( self );
	}

	if ( cache )
	{
		Cache_init( self, anIndex );
	}
}

const ISet* Directory_getCachedSubdirectories( const IDirectory* self )
{
	ISet* ret = NULL;
	if ( self->cache )
	{
		ret = self->cache->directories;
	}
	return ret;
}

const ISet* Directory_getCachedFiles( const IDirectory* self )
{
	ISet* ret = NULL;
	if ( self->cache )
	{
		ret = self->cache->files;
	}
	return ret;
}

const IDirectory*
Directory_getCachedSubdirectory( const IDirectory* self, const char* name )
{
	const IDirectory* ret = NULL;
	if ( self->cache )
	{
		const ISet* cached_subdirectories = Directory_getCachedSubdirectories( self );
		unsigned int max = Set_count( cached_subdirectories );
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			const IDirectory* dir = (IDirectory*) Set_get( cached_subdirectories, i );
			const char* dir_name = Path_getBasename( Directory_getRealPath( dir ) );
			if ( 0 == CharString_compare( name, dir_name ) )
			{
				ret = dir;
				break;
			}
		}
	}
	return ret;
}

const IDirectory*
Directory_getCachedSubdirectory_new( const IDirectory* self, const char* name )
{
	const IDirectory* ret = NULL;
	if ( self->cache )
	{
		const ISet* cached_subdirectories = Directory_getCachedSubdirectories( self );
		unsigned int max = Set_count( cached_subdirectories );
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			const IDirectory* dir = (IDirectory*) Set_get( cached_subdirectories, i );
			const char* dir_name = Path_getBasename( Directory_getRealPath( dir ) );
			if ( 0 == CharString_compare( name, dir_name ) )
			{
				ret = dir;
				break;
			}
		}
		if ( !ret )
		{
			IDirectory* child = Directory_subdirectory( self, name );
			const char* child_location = Path_getCondensed( Directory_getRealPath( child ) );
			if ( Index_add( (IIndex*) self->cache->sharedIndex, child_location, child ) ) abort();
			Set_add( self->cache->directories, child );
			Directory_setCaching_index( child, 1, self->cache->sharedIndex );
			ret = child;
		}
	}
	return ret;
}

const IFile*
Directory_getCachedFile( const IDirectory* self, const char* name )
{
	const IFile* ret = NULL;
	if ( self->cache )
	{
		const ISet* cached_files = Directory_getCachedFiles( self );
		unsigned int max = Set_count( cached_files );
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			const IFile* file = (IFile*) Set_get( cached_files, i );
			const char* file_name = Path_getBasename( File_getPath( file ) );
			if ( 0 == CharString_compare( name, file_name ) )
			{
				ret = file;
				break;
			}
		}
	}
	return ret;
}

const char** Directory_getContents( const IDirectory* self )
{
	return (const char**) self->contents;
}

const IList* Directory_getFileList( const IDirectory* self )
{
	return self->fileList;
}

const IList* Directory_getFileList_alphabetical( const IDirectory* self )
{
	if ( self->fileList && !self->alphabetical )
	{
		IList* alpha_list = new_List();
		ISort* sort = new_Sort();
		const char* filename;
		int i;
		int count = List_count( self->fileList );
		for ( i=0; i < count; i++ )
		{
			filename = self->fileList->items[i];
			Sort_insert( sort, filename, (void*) filename );
		}

		{
			IIterator* it = Sort_iterator_values( sort );

			List_copyItem( alpha_list, "." );
			List_copyItem( alpha_list, ".." );

			while ( (filename = Iterator_next( it )) )
			{
				if ( (filename[0] != '.') || ((filename[1] != '.') && (filename[1] != '\0')) )
				{
					List_copyItem( alpha_list, filename );
				}
			}
			free_Iterator( it );
		}
		free_Sort( sort );

		((IDirectory*)self)->alphabetical = alpha_list;
	}
	return self->alphabetical;
}

const IPath* Directory_getPath( const IDirectory* self )
{
	return self->path;
}

const IPath* Directory_getRealPath( const IDirectory* self )
{
	if ( self->link ) {
		return Link_getPath( self->link );
	} else {
		return self->path;
	}
}

bool Directory_createAll( const IDirectory* self )
{
	int status = 1;
	if ( !Directory_exists( self ) && !Directory_isLink( self ) )
	{
		IDirectory* parent = new_Directory( Path_getDirname( Directory_getRealPath( self ) ) );
		if ( !Directory_exists( parent ) && !Directory_isLink( parent ) )
		{
			status &= Directory_createAll( parent );
		}
		if ( status )
		{
			status &= Directory_create( self );
			//IDirectory* subdir = Directory_subdirectory( parent, Path_getBasename( Directory_getRealPath( self ) ) );
			//status &= Directory_create( subdir );
		}
	}
	return status;
}

/*
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
*/

/*
bool Directory_delete( const IDirectory* self )
{
	int status = 0;
	if ( RemoveDirectory( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
	return status;
}
*/

/*
bool Directory_isLink( const IDirectory* self )
{
	return 0;
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
*/

/*
 *  Taken from:
 *  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/base/file_management_reference.asp
 */
/*
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
*/

int Directory_isLink( const IDirectory* self )
{
	IDirectory* rw_self = (IDirectory*) self;
	int status = 0;
	
	if ( self->link )
	{
		status = 1;
	}
	else if ( !Directory_isDirectory( self ) )
	{
		rw_self->link = new_Link();

		if ( Link_load( self->link, Path_getCommon( self->path ) ) )
		{
			IDirectory* link_dir = new_Directory_path( Link_getPath( self->link ) );
			if ( Directory_exists( link_dir ) )
			{
				status = 1;
			} else {
				free_Link( rw_self->link );
				rw_self->link = NULL;
			}
			free_Directory( link_dir );
		} else {
			free_Link( rw_self->link );
			rw_self->link = NULL;
		}
	}
	return status;
}

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

const ITime*
Directory_getLastAccess( const IDirectory* self )
{
	if ( !self->times.lastAccess )
	{
		Directory_retrieveTimes( (IDirectory*) self );
	}
	return self->times.lastAccess;
}

const ITime*
Directory_getLastModification( const IDirectory* self )
{
	if ( !self->times.lastModification )
	{
		Directory_retrieveTimes( (IDirectory*) self );
	}
	return self->times.lastModification;
}

const ITime*
Directory_getLastChange( const IDirectory* self )
{
	if ( !self->times.lastChange )
	{
		Directory_retrieveTimes( (IDirectory*) self );
	}
	return self->times.lastChange;
}

IList* Directory_filenamesStartingWith( const IDirectory* self, const char* pattern )
{
	IList* list = new_List();

	unsigned int count = List_count( self->fileList );
	unsigned int i = 0;
	for ( i=0; i < count; i++ )
	{
		if ( CharString_startsWith( self->fileList->items[i], pattern ) )
		{
			List_copyItem( list, self->fileList->items[i] );
		}
	}
	return list;
}

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

void
Directory_retrieveTimes( IDirectory* self )
{
	struct stat buf;
	stat( Path_getCondensed( Directory_getRealPath( self ) ), &buf );
	
	self->times.lastAccess = new_Time( buf.st_atime );
	self->times.lastModification = new_Time( buf.st_mtime );
	self->times.lastChange = new_Time( buf.st_ctime );
}

//-------------------------------------------------------------------------------
//	Uncached search functions
//-------------------------------------------------------------------------------

/*
 *	Directory search methods
 *
 *  For each file in this directory:
 *      a) If its name equals "name" add it to the list.
 *      b) If it is a directory recursively search that directory.
 */
IList* Directory_search( IDirectory* self, const char* name, bool absolute )
{
        IList* list = new_List();
        char* prefix = ".";

        Directory_search_private( self, name, prefix, list, absolute );

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

IList* Directory_search_depthFirst_followLinks( IDirectory* self, const char* name, int absolute )
{
	IList* list = new_List();
	IList* searched = new_List();
	char* prefix = ".";

	Directory_search_depthFirst_followLinks_private( self, name, prefix, list, absolute, searched );

	free_List( searched );
	return list;
}

IList* Directory_search_shallow_followLinks( IDirectory* self, const char* name, int absolute )
{
	IList* list = new_List();
	IList* searched = new_List();
	char* prefix = ".";

	Directory_search_shallow_breadth_followLinks_private( self, name, prefix, list, absolute, searched );

	free_List( searched );
	return list;
}

IList*
Directory_search_private(
	IDirectory*       self,
	const char*       name,
	const char*       prefix,
	IList*            list,
	bool              absolute )
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

				if ( CharString_matches( file, name ) )
				{
					char* item_loc;
					if ( absolute )
					{
						item_loc = CharString_cat3( Path_getCondensed( Directory_getRealPath( self ) ), "/", file );
					} else {
						item_loc = CharString_copy( next_prefix );
					}
					List_addItem( list, item_loc );
				}
				next_dir = Directory_subdirectory( self, file );
				if ( !Directory_isLink( next_dir ) )
				{
					Directory_search_private( next_dir, name, next_prefix, list, absolute );
				}
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

IList* Directory_search_shallow_breadth_followLinks_private(
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

		bool found = 0;
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
						found = 1;
					}
					else if ( Directory_isLink( child_dir ) )
					{
						const char* path = Path_getCondensed( Directory_getPath( child_dir ) );
						const char* real_path = Path_getCondensed( Directory_getRealPath( child_dir ) );
						fprintf( stderr, "invalid link: %s --> %s\n", path, real_path );
						abort();
					}
					free_Directory( child_dir );
				}
			}
		}

		if ( !found )
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

IList*
Directory_search_depthFirst_followLinks_private(
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
		IStack* stack = new_Stack();
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

				next_dir = Directory_subdirectory( self, file );
				Directory_search_depthFirst_followLinks_private( next_dir, name, next_prefix, list, absolute, searched );

				if ( CharString_matches( file, name ) )
				{
					Stack_addItem( stack, next_prefix );
				} else {
					free_CharString( next_prefix );
				}
				free_Directory( next_dir );
			}
		}

		while ( (next_prefix = Stack_pop( stack )) )
		{
			List_addItem( list, next_prefix );
		}
		free_Stack( stack );
	}
	return list;
}

//-------------------------------------------------------------------------------
//	Cached directory search functions
//-------------------------------------------------------------------------------

IList* Directory_searchCached_followLinks( const IDirectory* self, const char* name, int absolute )
{
	IList* list = new_List();
	IList* searched = new_List();
	char* prefix = ".";

	if ( self->cache )
	{
		Directory_searchCached_breadth_followLinks_private( self, name, prefix, list, absolute, searched );
	} else {
		abort();
	}
	free_List( searched );
	return list;
}

IList* Directory_searchCached_depthFirst_followLinks( const IDirectory* self, const char* name, int absolute )
{
	IList* list = new_List();
	IList* searched = new_List();
	char* prefix = ".";

	if ( self->cache )
	{
		Directory_searchCached_depthFirst_followLinks_private( self, name, prefix, list, absolute, searched );
	} else {
		abort();
	}
	free_List( searched );
	return list;
}

IList* Directory_searchCached_depthFirst_filter_followLinks( const IDirectory* self, const char* filter, int absolute )
{
	IList* list = new_List();
	IList* searched = new_List();
	char* prefix = ".";

	if ( self->cache )
	{
		Directory_searchCached_depthFirst_filter_followLinks_private( self, filter, prefix, list, absolute, searched );
	} else {
		abort();
	}
	free_List( searched );
	return list;
}

IList* Directory_searchCached_shallow_followLinks( const IDirectory* self, const char* name, int absolute )
{
	IList* list = new_List();
	IList* searched = new_List();
	char* prefix = ".";

	if ( self->cache )
	{
		Directory_searchCached_shallow_breadth_followLinks_private( self, name, prefix, list, absolute, searched );
	} else {
		abort();
	}
	free_List( searched );
	return list;
}

IList* Directory_searchCached_breadth_followLinks_private(
	const IDirectory* self,
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
	if ( List_addItemIfNew( searched, condensed_path ) )
	{
		const ISet* subdirs = Directory_getCachedSubdirectories( self );
		
		unsigned int count = Set_count( subdirs );
		unsigned int i;
		for ( i=0; i < count; i++ )
		{
			const IDirectory* subdir = (const IDirectory*) Set_get( subdirs, i );
			if ( Directory_containsDirectory( subdir, name ) )
			{
				char* item_loc;
				if ( absolute )
				{
					item_loc = CharString_cat3( Path_getCondensed( Directory_getRealPath( subdir ) ), "/", name );
				} else {
					char* tmp = CharString_cat3( prefix, "/", Path_getBasename( Directory_getRealPath( subdir ) ) );
					item_loc = CharString_cat3( tmp, "/", name );
					free( tmp );
				}
				List_addItemIfNew( list, item_loc );
			}
		}
		
		for ( i=0; i < count; i++ )
		{
			const IDirectory* next_search_dir = (const IDirectory*) Set_get( subdirs, i );
			const char* file = Path_getBasename( Directory_getRealPath( next_search_dir ) );
			char* next_prefix;
			if ( absolute )
			{
				next_prefix = CharString_cat3( Path_getCondensed( Directory_getRealPath( self ) ), "/", file );
			} else {
				next_prefix = CharString_cat3( prefix, "/", file );
			}
			Directory_searchCached_shallow_breadth_followLinks_private( next_search_dir, name, next_prefix, list, absolute, searched );
			free( next_prefix );
		}
	}
	return list;
}

IList* Directory_searchCached_depthFirst_followLinks_private(
	const IDirectory* self,
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
	if ( List_addItemIfNew( searched, condensed_path ) )
	{
		const ISet* subdirs = Directory_getCachedSubdirectories( self );
		
		unsigned int count = Set_count( subdirs );
		unsigned int i;
		for ( i=0; i < count; i++ )
		{
			const IDirectory* subdir = (const IDirectory*) Set_get( subdirs, i );
			const IPath* subdir_realpath = Directory_getRealPath( subdir );
			const char* file = Path_getBasename( subdir_realpath );
			if ( file )
			{
				char* next_prefix;

				if ( absolute )
				{
					next_prefix = CharString_cat3( Path_getCondensed( Directory_getRealPath( self ) ), "/", file );
				} else {
					next_prefix = CharString_cat3( prefix, "/", file );
				}
				Directory_searchCached_depthFirst_followLinks_private( subdir, name, next_prefix, list, absolute, searched );
				free( next_prefix );
				
				if ( CharString_matches( file, name ) )
				{
					char* item_loc;
					if ( absolute )
					{
						item_loc = CharString_cat3( condensed_path, "/", file );
					} else {
						item_loc = CharString_cat3( prefix, "/", file );
					}
					List_addItemIfNew( list, item_loc );
				}
			}
		}
	}
	return list;
}

IList* Directory_searchCached_depthFirst_filter_followLinks_private(
	const IDirectory* self,
	const char*      filter,
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

	if ( List_addItemIfNew( searched, condensed_path ) )
	{
		const ISet* subdirs = Directory_getCachedSubdirectories( self );
		const ISet* files   = Directory_getCachedFiles( self );
		
		unsigned int count = Set_count( subdirs );
		unsigned int i;
		for ( i=0; i < count; i++ )
		{
			const IDirectory* subdir = (const IDirectory*) Set_get( subdirs, i );
			const char* file = Path_getBasename( Directory_getRealPath( subdir ) );
			char* next_prefix;

			if ( absolute )
			{
				next_prefix = CharString_cat3( Path_getCondensed( Directory_getRealPath( self ) ), "/", file );
			} else {
				next_prefix = CharString_cat3( prefix, "/", file );
			}
			Directory_searchCached_depthFirst_filter_followLinks_private( subdir, filter, next_prefix, list, absolute, searched );
			free( next_prefix );

			if ( CharString_endsWith( file, filter ) )
			{
				char* item_loc;
				if ( absolute )
				{
					item_loc = CharString_cat3( condensed_path, "/", file );
				} else {
					item_loc = CharString_cat3( prefix, "/", file );
				}
				List_addItemIfNew( list, item_loc );
			}
		}
		
		count = Set_count( files );
		for ( i=0; i < count; i++ )
		{
			const IFile* file = (const IFile*) Set_get( files, i );
			if ( Path_hasExtension( File_getPath( file ), filter ) )
			{
				char* item_loc;
				const char* file_name = Path_getBasename( File_getPath( file ) );
				if ( absolute )
				{
					item_loc = CharString_cat3( condensed_path, "/", file_name );
				} else {
					item_loc = CharString_cat3( prefix, "/", file_name );
				}
				List_addItemIfNew( list, item_loc );
			}
		}
	}
	return list;
}

IList* Directory_searchCached_shallow_breadth_followLinks_private(
	const IDirectory* self,
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
	if ( List_addItemIfNew( searched, condensed_path ) )
	{
		const ISet* subdirs = Directory_getCachedSubdirectories( self );
		
		bool found = 0;
		
		unsigned int count = Set_count( subdirs );
		unsigned int i;
		for ( i=0; i < count; i++ )
		{
			const IDirectory* subdir = (const IDirectory*) Set_get( subdirs, i );
			if ( Directory_containsDirectory( subdir, name ) )
			{
				char* item_loc;
				if ( absolute )
				{
					item_loc = CharString_cat3( Path_getCondensed( Directory_getRealPath( subdir ) ), "/", name );
				} else {
					char* tmp = CharString_cat3( prefix, "/", Path_getBasename( Directory_getRealPath( subdir ) ) );
					item_loc = CharString_cat3( tmp, "/", name );
					free( tmp );
				}
				List_addItemIfNew( list, item_loc );
				found = 1;
			}
		}
		
		if ( !found )
		{
			for ( i=0; i < count; i++ )
			{
				const IDirectory* next_search_dir = (const IDirectory*) Set_get( subdirs, i );
				const char* file = Path_getBasename( Directory_getRealPath( next_search_dir ) );
				char* next_prefix;
				if ( absolute )
				{
					next_prefix = CharString_cat3( Path_getCondensed( Directory_getRealPath( self ) ), "/", file );
				} else {
					next_prefix = CharString_cat3( prefix, "/", file );
				}
				Directory_searchCached_shallow_breadth_followLinks_private( next_search_dir, name, next_prefix, list, absolute, searched );
				free( next_prefix );
			}
		}
	}
	return list;
}

//-------------------------------------------------------------------------------
//	Cache functions
//-------------------------------------------------------------------------------

void Cache_init( IDirectory* self, IIndex const* sharedIndex )
{
	self->cache = CRuntime_calloc( 1, sizeof( struct _ICache ) );

	if ( !sharedIndex ) {
		self->cache->master = new_Index();
		self->cache->sharedIndex = self->cache->master;
	} else {
		self->cache->sharedIndex = sharedIndex;
	}
	self->cache->directories = new_Set();
	self->cache->files = new_Set();
	Cache_refresh( self );
}

void Cache_destroy( IDirectory* self )
{
	unsigned int maxFiles = Set_count( self->cache->files );
	unsigned int i;

	for ( i=0; i < maxFiles; i++ ) {
		free_File( (IFile*) Set_get( self->cache->files, i ) );
	}
	free_Set( self->cache->directories );
	free_Set( self->cache->files );

	if ( self->cache->master ) {
		Index_freeObjects( self->cache->master, (void(*)(void*)) free_Directory );
		free_Index( self->cache->master );
	}

	CRuntime_free( self->cache );
	self->cache = NULL;
}

void Cache_refresh( IDirectory* self )
{
	if ( Directory_refresh( self ) )
	{
		const IList* file_list = Directory_getFileList( self );
		unsigned int max = List_count( file_list );
		unsigned int i;
		
		for ( i=0; i < max; i++ )
		{
			if ( '.' != file_list->items[i][0] )
			{
				IDirectory* child = Directory_subdirectory( self, file_list->items[i] );
				Directory_isLink( child );
				{
					const char* child_location = Path_getCondensed( Directory_getRealPath( child ) );
				 
					if ( Directory_isDirectory( child )  )
					{
						if ( Index_containsKey( self->cache->sharedIndex, child_location ) )
						{
							const IDirectory* existing = Index_get( self->cache->sharedIndex, child_location );
							Set_add( self->cache->directories, existing );
							free_Directory( child );
						} else {
							if ( Index_add( (IIndex*) self->cache->sharedIndex, child_location, child ) ) abort();
							//fprintf( stdout, "Adding %s\n", child_location );
							Set_add( self->cache->directories, child );
							Directory_setCaching_index( child, 1, self->cache->sharedIndex );
						}
					}
					else if ( Directory_isLink( child ) )
					{
						if ( Index_containsKey( self->cache->sharedIndex, child_location ) )
						{
							const IDirectory* existing = Index_get( self->cache->sharedIndex, child_location );
							Set_add( self->cache->directories, existing );
						} else {
							//fprintf( stdout, "Adding %s\n", child_location );
							IDirectory* real_child = new_Directory( child_location );
							if ( Index_add( (IIndex*) self->cache->sharedIndex, child_location, real_child ) ) abort();
							Set_add( self->cache->directories, real_child );
							Directory_setCaching_index( real_child, 1, self->cache->sharedIndex );
						}
						free_Directory( child );
					}
					else
					{
						Set_add( self->cache->files, new_File( Path_getCommon( Directory_getPath( child ) ) ) );
						free_Directory( child );
					}
				}
			}
		}
	}
}
