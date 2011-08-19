/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/io/psa/Directory.h"
#include "openocl/io/psa/File.h"
#include "openocl/io/psa/Link.h"
#include "openocl/io/psa/Path.h"

#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/List.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int Link_private_is7bit( const char* buffer );

struct _ILink
{
	IPath* target;
	char*  original;
	IList* list;
	bool   allowDynamic;
};

ILink* new_Link()
{
	ILink* self = (ILink*) CRuntime_calloc( 1, sizeof( ILink ) );
	self->target = NULL;
	self->original = NULL;
	self->list = new_List();
	self->allowDynamic = 0;
	
	return self;
}

void free_Link( ILink* self )
{
	if ( self->target )   free_Path( self->target );
	if ( self->original ) free_CharString( self->original );
	if ( self->list )     free_List( self->list );
	CRuntime_free( self );
}

void Link_setTarget( ILink* self, const char* aTarget )
{
	self->target = new_Path( aTarget );
}

int Link_load( ILink* self, const char* location )
{
	int status = 0;
	
	if ( self->target )
	{
		free_Path( self->target );
		self->target = NULL;
	}

	if ( CharString_endsWith( location, ".link" ) )
	{
		//fprintf( stdout, " |\n" );
		//fprintf( stdout, " |  Loading link from: %s\n", location );

		IPath* p = new_Path( location );
		IFile* f = new_File_path( p );
		
		if ( File_open( f, "r" ) )
		{
			char* buffer = File_readCharacters( f );
			if ( buffer )
			{
				//	Test buffer to make sure it only contains ASCII 7-bit characters.
				//	Test to make sure it doesn't contain a $ if dynamic not allowed.
				bool seven_bit = Link_private_is7bit( buffer );
			
				if ( seven_bit )
				{
					bool loop = 1;
					unsigned int i=0;
					unsigned int size=0;
					const char* start = &buffer[i];
					unsigned int nr_paths;
					unsigned int j;
					while ( loop )
					{
						switch ( buffer[i] )
						{
						case '\0':
							loop = 0;
						case '\n':
							buffer[i] = '\0';
							if ( size )
							{
								List_addItem( self->list, CharString_removeWhitespace( start ) );
							}
							start = &buffer[i+1];
							size = 0;
							break;
						default:
							size++;
						}
						i++;
					}
					
					//  Iterate through list until we find a valid link
					
					nr_paths = List_count( self->list );
					for ( j=0; j < nr_paths; j++ )
					{
						IFile* file = NULL;
						IDirectory* dir = NULL;

						if ( self->target ) { free_Path( self->target ); self->target = NULL; }
						if ( self->original ) { free_CharString( self->original ); self->original = NULL; };
						self->target = new_Path( self->list->items[j] );
						if ( self->target )
						{
							self->original = CharString_copy( self->list->items[j] );

							if ( !Path_isAbsolute( self->target ) )
							{
								char* base = CharString_dirname( location );
								char* tmp = CharString_cat3( base, "/", self->list->items[j] );
								free_Path( self->target );
								self->target = new_Path( tmp );
								free_CharString( base );
								free_CharString( tmp );
							}

							file = new_File_path( self->target );
							dir = new_Directory_path( self->target );
							if ( File_exists( file ) || Directory_exists( dir ) || (j+1 == nr_paths) )
							{
								free_File( file );
								free_Directory( dir );
								status = 1;
								break;
							}
							else
							{
								free_File( file );
								free_Directory( dir );
							}
						}
					}
				}
				free_CharString( buffer );
			}
			File_close( f );
		}
		
		free_File( f );
		free_Path( p );
	}
	return status;
}

void
Link_setAllowDynamic( ILink* self, bool value )
{
	self->allowDynamic = value;
}

const IPath* Link_getPath( const ILink* self )
{
	return self->target;
}

const char* Link_getTarget( const ILink* self )
{
	const char* ret = NULL;
	if ( self->target )
	{
		ret = Path_getCommon( self->target );
	}
	return ret;
}

const char* Link_getCanonical( const ILink* self )
{
	const char* ret = NULL;
	if ( self->target )
	{
		ret = Path_getCondensed( self->target );
	}
	return ret;
}

const char* Link_getOriginal( const ILink* self )
{
	return self->original;
}

int Link_save( const ILink* self, const char* location )
{
	return 0;
}

int Link_private_is7bit( const char* buffer )
{
	int seven_bit = 1;
	const char* c = buffer;
	while ( *c )
	{
		if ( (*c & 0x80) ) //|| (!self->allowDynamic && (*c == '$') ) )
		{
			fprintf( stderr, "Link: encounted %i (%c)\n", *c, *c );
			break;
		}
		c++;
	}
	if ( *c ) { seven_bit = 0; }// If c has a value the character is 8-bit, fail.
	return seven_bit;
}

