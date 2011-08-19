/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_LINK_H
#define OPENOCL_IO_PSA_LINK_H

#include "openocl/io/psa/Path.h"

typedef struct _ILink ILink;

EXPORT	ILink*			new_Link();
EXPORT	void			free_Link( ILink* self );

EXPORT	void			Link_setTarget( ILink* self, const char* aTarget );
EXPORT	int				Link_load( ILink* self, const char* location );
EXPORT	void			Link_setAllowDynamic( ILink* self, bool value );

EXPORT	int				Link_save( const ILink* self, const char* location );
EXPORT	const IPath*	Link_getPath( const ILink* self );
EXPORT	const char*		Link_getTarget( const ILink* self );
EXPORT	const char*		Link_getCanonical( const ILink* self );
EXPORT	const char*		Link_getOriginal( const ILink* self );

#endif
