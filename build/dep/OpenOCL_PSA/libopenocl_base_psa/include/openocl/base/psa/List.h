/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_LIST_H
#define OPENOCL_BASE_PSA_LIST_H

#include "openocl/base/psa/export.h"

typedef struct
{
	char** items;
	unsigned int count;
	unsigned int size;
} IList;

EXPORT	IList*		new_List();
EXPORT	IList*		new_List_items( const char** items );
EXPORT	void		free_List( IList* list );

EXPORT	void		List_addItem( IList* list, char* anItem );
EXPORT	int		List_addItemIfNew( IList* list, char* anItem );
EXPORT	void		List_copyItem( IList* list, const char* anItem );
EXPORT	void		List_copyItem2( IList* list, const char* anItem, const char* suffixOfItem );
EXPORT	void		List_copyItem3( IList* list, const char* anItem, const char* suffixOfItem, const char* anotherSuffix );

EXPORT	int			List_contains( const IList* list, const char* anItem ); 
EXPORT	unsigned int	List_count( const IList* list );
EXPORT	void		List_addList( IList* list, const IList* aList );
EXPORT	void		List_print( IList* list );
EXPORT	IList*		List_quotedList( const IList* self );

#endif

