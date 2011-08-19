/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Environment.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* new_CharString( const char* charString )
{
	char* self = NULL;
	
	if ( charString )
	{
		unsigned int length_required = CharString_getLength( charString ) + 1;
		const char* src = charString;
		char* tmp;
		self = CRuntime_malloc( length_required * sizeof( char ) );	
		tmp = self;
		
		while ( '\0' != *src )
		{
			*tmp = *src;
			tmp++;
			src++;
		}
		*tmp = *src;
	}
	
	return self;
}

char* new_CharString_delimiter( const char** tokens, const char delimiter )
{
	char delim[2] = {delimiter,0};
	//delim[0] = delimiter;

	char* str = CharString_copy( "" );
	char* tmp = CharString_copy( "" );

	unsigned int i=0;
	while ( tokens[i] )
	{
		free_CharString( tmp );
		tmp = str;
		str = CharString_cat2( tmp, tokens[i] );
		free_CharString( tmp );
		tmp = str;
		str = CharString_cat2( tmp, delim );
		i++;
	}
	str[CharString_getLength( str ) - 1] = '\0';
	free_CharString( tmp );

	return str;
}

char* new_CharString_format_args( const char* format, ... )
{
	va_list ap;
	va_start( ap, format );
	return new_CharString_format_valist( format, ap ); 
}

void free_CharString( char* self )
{
	CRuntime_free( self );
}

void CharString_replace( char* str, char old, char with )
{
	while ( 0 != *str )
	{
		if ( old == *str ) *str = with;
		str++;
	}
}

/*
 *  Const allocating functions
 */

char* CharString_between( const char* aString, const char* prefix, const char* suffix )
{
	char* ret = NULL;

	char* copy = CharString_copy( aString );
	unsigned int prefix_len = CharString_getLength( prefix );
	const char* start = strstr( copy, prefix );

	if ( start )
	{
		start = start + prefix_len;
		
		//str = (char*) CRuntime_calloc( strlen( start ), sizeof( char ) );
		//str = strcpy( str, start );

		if ( ('\0' != *start) && (0 != strcmp( "\0", suffix )) && (0 != strcmp( "", suffix ) ) )
		{
			char* end = strstr( copy, suffix );
			if ( end )
			{
				*end = '\0';
				ret = CharString_copy( start );
			}
		}
	}
	CRuntime_free( copy );

	return ret;
}

char* CharString_cat2( const char* prefix, const char* suffix )
{
	char* str = CRuntime_calloc( strlen( prefix ) + strlen( suffix ) + 1, sizeof( char ) );
	strcpy( str, prefix );
	strcat( str, suffix );
	return str;
}

char* CharString_cat3( const char* part1, const char* part2, const char* part3 )
{
	char* str = CRuntime_calloc( strlen( part1 ) + strlen( part2 ) + strlen( part3 ) + 1, sizeof( char ) );
	sprintf( str, "%s%s%s", part1, part2, part3 );
	return str;
}

char* CharString_copy( const char* aString )
{
	return new_CharString( aString );
}

char* CharString_removeWhitespace( const char* aString )
{
	unsigned int len;
	char* tmp = CharString_copy( aString );
	char* buffer = tmp;
	char* ret;

	/*
	 *	Remove white space at front of path
	 *	by incrementing pointer.
	 */
	while ( (' ' == *buffer) || ('\t' == *buffer) ) buffer++;
			
	/*
	 *	Replace all newline characters with null
	 *	terminators.
	 */
	CharString_replace( buffer, '\n', '\0' );

	/*
	 *	Replace all trailing white space characters
	 *	with null terminators.
	 */
	len = strlen( buffer ) - 1;
	while ( (' ' == buffer[len]) || ('\t' == buffer[len] ) )
	{
		buffer[len] = '\0';
		len--;
	}

	ret = CharString_copy( buffer );
	free_CharString( tmp );
	return ret;
}

char* CharString_substring( const char* self, int start, int end )
{
	char* string = NULL;
	int max = CharString_getLength( self );
	
	if (  start > end )
	{
		int tmp = start;
		start = end;
		end = tmp;
	}

	if (start < 0) start = 0;
	if (max <= end) end = max - 1;

	{
		unsigned int size = (end - start) + 2;
		string = CRuntime_calloc( size, sizeof( char ) );
		{
			int posn = 0;
			unsigned int i;
			for ( i=start; i <= end; i++ )
			{
				string[posn++] = self[i];
			}
		}
	}
	return string;
}

char* CharString_substring_start_length( const char* self, int start, int length )
{
	char* string = CRuntime_calloc( length + 1, sizeof( char ) );
	int max = CharString_getLength( self );
	
	if (start < 0)  start  = 0;
	if (length < 0) length = 0;

	if ( max < start+length )
	{
		length = max - start;
	}

	{
		int i;
		for ( i=0; i < length; i++ )
		{
			string[i] = self[start+i];
		}
	}
	return string;
}

char* CharString_token( const char* self, unsigned int start, char delimiter )
{
	unsigned int index = CharString_indexOfNext( self, start, delimiter );
	return CharString_substring_start_length( self, start, index - start );
}

char* CharString_basename( const char* path )
{
	return CharString_basename_ifs( path, Environment_GetFileSeparator() );
}

char* CharString_basename_ifs( const char* path, char fs )
{
	//fprintf( stdout, "String_basename( %s )\n", path );

    char* bname  = NULL;
	char  ifs[2] = {fs,0};

	{
		char* tmp = CharString_copy( path );
		const char* last = NULL;

		const char* token = (const char*) strtok( tmp, ifs );
		while ( NULL != token )
		{
		//fprintf( stdout, "\t%s\n", token );
			last = token;
			token = (const char*) strtok( NULL, ifs );
		}
		if ( last )
		{
			bname = CharString_copy( last );
		}
		else if ( '/' == path[0] )
		{
			bname = CharString_copy( "/" );
		}
		else
		{
			bname = CharString_copy( "" );
		}
		CRuntime_free( tmp );
	}
	return bname;
}

char* CharString_dirname( const char* path )
{
	return CharString_dirname_ifs( path, Environment_GetFileSeparator() );
}

char* CharString_dirname_ifs( const char* path, char ifs )
{
	char*        tmp    = CharString_copy( path );
	int          i      = strlen(tmp) - 1;

	//
	//	First remove any trailing /'s
	//
	while ( (0 < i) && (ifs == tmp[i]) )
	{
		tmp[i] = '\0';
		i--;
	}
	
	//
	//	Next remove everything up until the / that is reached
	//
	while ( (0 <= i) && (ifs != tmp[i]) )
	{
		tmp[i] = '\0';
		i--;
	}
	
	//
	//	Now if length > 1 or tmp[0] != / remove the last slash.
	//
	if ( (ifs == tmp[i]) && (0 < i) )
	{
		tmp[i] = '\0';
	}
	
	return tmp;
}

/*
 *  Boolean functions
 */

int CharString_contains( const char* self, const char* token )
{
	if ( strstr( self, token ) )
	{
		return 1;
	} else {
		return 0;
	}
}

int CharString_startsWith( const char* this, const char* token ) 
{
        if ( this == strstr( this, token ) )
        {
                return 1;
        } else {
                return 0;
        }
}

int CharString_endsWith( const char* this, const char* token )
{
	int itDoes = 0;

	unsigned int this_len = strlen( this );
	unsigned int token_len = strlen( token );

	if ( token_len <= this_len )
	{
		//	0123456789X1234		012	this_len = 15	token_len = 3
		//	libsomething.so		.so	
		//	123456789X12345		123

		unsigned int stop = this_len - token_len;
		unsigned int this_posn = this_len;
		unsigned int token_posn = token_len;
		
		itDoes = 1;
		while ( this_posn >= stop )
		{
			if ( this[ this_posn ] != token[ token_posn ] )
			{
				itDoes = 0;
				break;
			}
			this_posn--;
			token_posn--;
		}
	}
	return itDoes;
}

int CharString_matches( const char* self, const char* pattern )
{
	int matches = 1;
	int loop = 1;

	const char* string = self;
	const char* current = pattern;
	
	while ( loop && *string )
	{
		switch ( *current )
		{
		case '*':
			{
				matches = 0;
				while ( '*' == *current ) {
					current++;
				}
			
				while ( *string )
				{
					matches |= CharString_matches( string, current );
					string++;
					
					if ( matches )
					{
						while ( *string ) string++; 
						while ( *current ) current++;
						loop = 0;
						break;
					}
				}
			}
			
			break;
		case '?':
			string++;
			current++;
			break;
		case '\0':
			if ( '\0' != *string )
			{
				matches = 0;
			}
			loop = 0;
			break;
		default:
			if ( *string != *current )
			{
				matches = 0;
				loop = 0;
			} else {
				string++;
				current++;
			}
		}
	}
	
	if ( *current && ( *current != '?' ) && ('*' != *current) )
	{
		matches = 0;
	}
	
	return matches;
}

int CharString_compare( const char* self, const char* other )
{
	int difference = 0;
	unsigned int i = 0;
	while ( ('\0' != self[i]) && ('\0' != other[i]) )
	{
		if ( (difference = self[i] - other[i]) )
		{
			break;
		}
		i++;
	}
	if ( !difference )
	{
		if ( !(difference = self[i]) )
		{
			difference -= other[i];
		}
	}
	return difference;
}

unsigned int CharString_getLength( const char* self )
{
	const char* tmp = self;
	unsigned int count = 0;
	while( '\0' != *tmp )
	{
		tmp++;
		count++;
	}
	return count;
}

unsigned int CharString_indexOfNext( const char* self, unsigned int start, char delimiter )
{
	unsigned int i = start;
	while ( ('\0' != self[i]) && (delimiter != self[i]) )
	{
		i++;
	}
	return i;
}

//
//	Help function to help use new CharString_substring_start_length operation instead of
//	CharString_substring
//
//	CharString_substring( start, end ) -->
//  CharString_substring_start_length( start, CharString_CountPositions( start, end ) )
//
int CharString_CountPositions( int start, int end )
{
	int nr;
	if ( start > end )
	{
		nr = 0;
	} else {
		nr = end - start + 1;
	}
	return nr;
}


