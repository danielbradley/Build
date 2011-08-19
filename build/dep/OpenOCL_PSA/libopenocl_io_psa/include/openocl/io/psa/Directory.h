/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_DIRECTORY_H
#define OPENOCL_IO_PSA_DIRECTORY_H

#include "openocl/io/psa/File.h"
#include "openocl/io/psa/Path.h"
#include <openocl/base/psa.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Set.h>
#include <openocl/base/psa/Time.h>

#ifdef __cplusplus
	using namespace openocl::base::psa;

	namespace openocl {
		namespace io {
			namespace psa {
	extern "C" {
#endif

typedef struct _Directory IDirectory;

//  lifecycle functions
EXPORT	IDirectory*  new_Directory( const char* location );
EXPORT	IDirectory*  new_Directory_path( const IPath* location );
EXPORT	IDirectory*  Directory_subdirectory( const IDirectory* self, const char* name );
EXPORT	void         free_Directory( IDirectory* self );

//  filesystem actions
EXPORT	void         Directory_setCaching( IDirectory* self, bool cache );    
EXPORT	bool         Directory_refresh( IDirectory* self );

EXPORT	bool         Directory_createAll( const IDirectory* self );
EXPORT	bool         Directory_create( const IDirectory* self );
EXPORT	bool         Directory_delete( const IDirectory* self );

//  filesystem queries
EXPORT	const ITime* Directory_getLastAccess( const IDirectory* self );
EXPORT	const ITime* Directory_getLastModification( const IDirectory* self );
EXPORT	const ITime* Directory_getLastChange( const IDirectory* self );

EXPORT	bool         Directory_isDirectory( const IDirectory* self );
EXPORT	bool         Directory_isLink( const IDirectory* self );
EXPORT	bool         Directory_exists( const IDirectory* self );
EXPORT	bool         Directory_containsDirectory( const IDirectory* self, const char* dirname );

//  const functions
EXPORT	const ISet*          Directory_getCachedSubdirectories( const IDirectory* self );
EXPORT	const ISet*          Directory_getCachedFiles( const IDirectory* self );
EXPORT	const IDirectory*    Directory_getCachedSubdirectory( const IDirectory* self, const char* name );
EXPORT	const IDirectory*    Directory_getCachedSubdirectory_new( const IDirectory* self, const char* name );
EXPORT	const IFile*         Directory_getCachedFile( const IDirectory* self, const char* name );

EXPORT	const char** Directory_getContents( const IDirectory* self );
EXPORT	const IList* Directory_getFileList( const IDirectory* self );
EXPORT	const IList* Directory_getFileList_alphabetical( const IDirectory* self );
EXPORT	const IPath* Directory_getPath( const IDirectory* self );
EXPORT	const IPath* Directory_getRealPath( const IDirectory* self );

//  filesystem search functions
EXPORT	IList*       Directory_filenamesStartingWith( const IDirectory* self, const char* filter );
EXPORT	IList*       Directory_filteredFileList( const IDirectory* self, const char* filter );
EXPORT	IList*       Directory_search( IDirectory* self, const char* name, bool absolute );
EXPORT	IList*       Directory_search_followLinks( IDirectory* self, const char* name, bool absolute );
EXPORT	IList*       Directory_search_depthFirst_followLinks( IDirectory* self, const char* name, bool absolute );
EXPORT	IList*       Directory_search_shallow_followLinks( IDirectory* self, const char* name, bool absolute );

EXPORT	IList*       Directory_searchCached_followLinks( const IDirectory* self, const char* name, bool absolute );
EXPORT	IList*       Directory_searchCached_shallow_followLinks( const IDirectory* self, const char* name, bool absolute );
EXPORT	IList*       Directory_searchCached_depthFirst_followLinks( const IDirectory* self, const char* name, bool absolute );
EXPORT	IList*       Directory_searchCached_depthFirst_filter_followLinks( const IDirectory* self, const char* filter, bool absolute );

#ifdef __cplusplus
}
};};};
#endif

#endif
