/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/List.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//  Internal functions

static int List_expand( IList* list );

IList* new_List()
{
	IList* list = (IList*) CRuntime_calloc( 1, sizeof( IList ) );
	list->count = 0;
	list->size = 50;

	list->items = (char**) CRuntime_calloc( 50, sizeof( char* ) );
	return list;
}

IList* new_List_items( const char** items )
{
	IList* list = new_List();
	while ( NULL != *items )
	{
		List_copyItem( list, *items );
		items++;
	}
	return list;
}

void
free_List( IList* list )
{
	if ( list )
	{ 
		unsigned int count = List_count( list );
		unsigned int i;

		for ( i=0; i < count; i++ )
		{
			if ( NULL != list->items[i] )
			{
				CRuntime_free( list->items[i] );
			}
		}

		CRuntime_free( list->items );
		CRuntime_free( list );
	}
}

void
List_addItem( IList* list, char* anItem )
{
	if ( list->count == list->size - 1 )
	{
		List_expand( list );
	}

	list->items[ list->count ] = anItem;
	list->count++;
}	

int
List_addItemIfNew( IList* list, char* anItem )
{
	int isNew = 0;
	
	if ( List_contains( list, anItem ) )
	{
		CRuntime_free( anItem );
	} else {
		if ( list->count == list->size - 1 )
		{
			List_expand( list );
		}

		list->items[ list->count ] = anItem;
		list->count++;
		isNew = 1;
	}
	return isNew;
}	

void
List_copyItem( IList* list, const char* anItem )
{
	if ( list->count == list->size - 1 )
	{
		List_expand( list );
	}

	{
		char* tmp = (char*) CRuntime_calloc( strlen( anItem ) + 1, sizeof( char ) );
		strcpy( tmp, anItem );
		list->items[ list->count ] = tmp;
		list->count++;
	}
}

void
List_copyItem2( IList* list, const char* anItem, const char* suffixOfItem )
{
	if ( list->count == list->size - 1 )
	{   
		List_expand( list );
	}

	{
		char* tmp = (char*) CRuntime_calloc( strlen( anItem ) + strlen( suffixOfItem ) + 1, sizeof( char ) );
		strcpy( tmp, anItem );
		strcat( tmp, suffixOfItem );
		list->items[ list->count ] = tmp;
		list->count++;
	}
}           

void
List_copyItem3( IList* list, const char* anItem, const char* suffixOfItem, const char* anotherSuffix )
{       
	if ( list->count == list->size - 1 )
	{   
		List_expand( list );
	}

	{
		char* tmp = (char*) CRuntime_calloc( strlen( anItem ) + strlen( suffixOfItem ) + strlen( anotherSuffix ) + 1, sizeof( char ) );
		strcpy( tmp, anItem );
		strcat( tmp, suffixOfItem );
		strcat( tmp, anotherSuffix );
		list->items[ list->count ] = tmp;
		list->count++;
	}
}

int
List_contains( const IList* list, const char* anItem )
{
	int result = 0;

	unsigned int i;
	for ( i=0; i < list->count; i++ )
	{
		if ( 0 == strcmp( list->items[i], anItem ) )
		{
			result = 1;
			break;
		}
	}
	return result;
}

unsigned int
List_count( const IList* list )
{
	return list->count;
}

void
List_addList( IList* list, const IList* aList )
{
	unsigned int count = List_count( aList );
	unsigned int i;

	for ( i=0; i < count; i++ )
	{
		List_copyItem( list, aList->items[i] );
	}
}

int
List_expand( IList* list )
{
	int status = 1;

	unsigned int new_size = list->size * 2;
	char** new_items = (char**) CRuntime_realloc( list->items, new_size * sizeof(char*) );
	if ( NULL != new_items )
	{
		list->items = new_items;
		list->size = new_size;

		{
			unsigned int i;
			for ( i = list->count; i < list->size; i++ )
			{
				list->items[i] = NULL;
			}
		}
		status = 0;
	}	
	return status;
}

void
List_print( IList* list )
{
	unsigned int i = 0;

	while ( NULL != list->items[i] )
	{
		printf( "\t\t%s\n", list->items[i] );
		i++;
	}
}

IList* List_quotedList( const IList* self )
{
	IList* list = new_List();
	unsigned int count = List_count( self );
	unsigned int i;
	for ( i=0; i < count; i++ )
	{
		List_addItem( list, CharString_cat3( "\"", self->items[i], "\"" ) );
	}
	return list;
}

