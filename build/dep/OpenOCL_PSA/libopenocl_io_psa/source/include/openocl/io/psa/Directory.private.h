/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_DIRECTORY_PRIVATE_H
#define OPENOCL_IO_PSA_DIRECTORY_PRIVATE_H

#include "openocl/io/psa/Directory.h"
#include "openocl/io/psa/Link.h"
#include <openocl/base/psa/Index.h>
#include <openocl/base/psa/Set.h>
#include <openocl/base/psa/Time.h>

IList*
Directory_search_private(
	IDirectory*       self,
	const char*       name,
	const char*       prefix,
	IList*            list,
	bool              absolute
);

IList*
Directory_search_depthFirst_followLinks_private(
	IDirectory*       self,
	const char*       name,
	const char*       prefix,
	IList*            list,
	int               absolute,
	IList*            searched
);

IList* Directory_search_breadth_followLinks_private(
	IDirectory*      self,
	const char*      name,
	const char*      prefix,
	IList*           list,
	int              absolute,
	IList*           searched
);

IList* Directory_search_shallow_breadth_followLinks_private(
	IDirectory*      self,
	const char*      name,
	const char*      prefix,
	IList*           list,
	int              absolute,
	IList*           searched
);

IList* Directory_searchCached_breadth_followLinks_private(
	const IDirectory* self,
	const char*       name,
	const char*       prefix,
	IList*            list,
	int               absolute,
	IList*            searched
);

IList* Directory_searchCached_shallow_breadth_followLinks_private(
	const IDirectory* self,
	const char*       name,
	const char*       prefix,
	IList*            list,
	int               absolute,
	IList*            searched
);

IList* Directory_searchCached_depthFirst_followLinks_private(
	const IDirectory* self,
	const char*       name,
	const char*       prefix,
	IList*            list,
	int               absolute,
	IList*            searched
);

IList* Directory_searchCached_depthFirst_filter_followLinks_private(
	const IDirectory*       self,
	const char*       filter,
	const char*       prefix,
	IList*            list,
	int               absolute,
	IList*            searched
);

void Directory_retrieveTimes( IDirectory* self );
void Directory_setCaching_index( IDirectory* self, bool cache, IIndex const* anIndex );

void Cache_init( IDirectory* self, IIndex const* anIndex );
void Cache_destroy( IDirectory* self );
void Cache_refresh( IDirectory* self );

struct _IAccessTimes
{
	ITime* lastAccess;
	ITime* lastModification;
	ITime* lastChange;
};

struct _ICache
{
	IIndex* master;
	IIndex const* sharedIndex;
	ISet* directories;
	ISet* files;
};

struct _Directory
{
	IPath* path;
	char** contents;
	IList* fileList;
	IList* alphabetical;
	ILink* link;
	struct _IAccessTimes times;
	struct _ICache*       cache;
};

#endif
