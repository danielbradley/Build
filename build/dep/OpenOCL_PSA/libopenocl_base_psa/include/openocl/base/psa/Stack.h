/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef _OPENOCL_BASE_PSA_STACK_H_
#define _OPENOCL_BASE_PSA_STACK_H_

#include "openocl/base/psa/export.h"

#define DEFAULT_STACK_SIZE 128

typedef struct _IStack IStack;

EXPORT	IStack*		new_Stack();
EXPORT	void		free_Stack( IStack* self );

EXPORT	void		Stack_addItem( IStack* self, char* token );
EXPORT	void		Stack_copyItem( IStack* self, const char* token );
EXPORT	char*		Stack_pop( IStack* self );

EXPORT	void		Stack_expand( IStack* self );

EXPORT	const char** Stack_getAsCharStringList( const IStack* self );

#endif
