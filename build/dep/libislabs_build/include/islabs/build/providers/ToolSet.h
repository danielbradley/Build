/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef PROVIDERS_TOOLSET_H
#define PROVIDERS_TOOLSET_H

typedef struct _ToolSet ToolSet;

int ToolSet_compile( const ToolSet* self,
                     const void*    buildParameters,
		     const void*    providerContext,
		     const void*    targetIDirectory );

int ToolSet_link( const ToolSet* self,
                  const void*    buildParameters,
		  const void*    providerContext,
		  const void*    targetIDirectory );

struct _ToolSet
{
	int (*compile)( const void*, const void*, const void* );
	int (*link)( const void*, const void*, const void* );
};

#endif
