/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/io/psa/Path.private.h"

#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Environment.h>
#include <openocl/base/psa/Stack.h>
#include <openocl/base/psa/System.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <direct.h>

static IStack* WorkingDirectories = NULL;

static bool FileExists( const char* location )
{
	bool exists = 0;
	FILE* f = fopen( location, "r" );
	if ( f )
	{
		fclose( f );
		exists = 1;
	}

	//IFile* file = new_File( location );
	//bool exists = File_exists( file );
	//free_File( file );
	return exists;
}

IPath* new_Path( const char* location )
{
	//	URL		://		file:///home/Johnny/documents		/HOSTNAME/home/
	//	UNC		\\		\\\C\home\Johnny\documents
	//	WINDOWS	:		C:\home\Johnny\documents			C:Johnny\documents
	//	POSIX			/home/Johnny/documents				Johnny\documents

	if ( CharString_contains( location, "://" ) )
	{
		return new_Path_URL( location );
	}
	else if ( CharString_contains( location, "\\\\" ) || CharString_contains( location, "//" ) )
	{
		return new_Path_UNC( location );
	}
	else if ( CharString_contains( location, "$" ) )
	{
		return new_Path_WinPOSIX_dynamic( location );
	}
	else
	{
		return new_Path_WinPOSIX( location );
	}
}

IPath* new_Path_URL( const char* location )
{
	//	URL				file://hostname/C:/home/Johnny/documents		/HOSTNAME/home/

	IPath* self = (IPath*) CRuntime_calloc( 1, sizeof( IPath ) );						// 1

	unsigned int pci;
	unsigned int hsi;
	unsigned int vci;
	unsigned int psi;

	self->type = OPENOCL_IO_PSA_TYPE_NONE;

	//	Here we remove whitespace - removed spaces at start and end,
	//	and replaced newlines with null terminators.
	self->original = CharString_removeWhitespace( location );						// 2
	pci = CharString_indexOfNext( self->original, 0, ':' );

	if ( ('/' == self->original[pci+1]) && ('/' == self->original[pci+2]) )
	{
		//	Extract the protocol
		self->protocol = CharString_substring_start_length( self->original, 0, CharString_CountPositions( 0, pci-1 ) );				// 3
	} else {
		abort();
	}

	pci += 3;
	hsi = CharString_indexOfNext( self->original, pci, '/' );

	if ( pci < hsi )
	{
		self->hostname = CharString_substring_start_length( self->original, pci, CharString_CountPositions( pci, hsi - 1 ) );				// 4
	} else {
		self->hostname = System_Hostname();								// 4
	}
	hsi++;

	vci = CharString_indexOfNext( self->original, hsi, ':' );
	psi = CharString_indexOfNext( self->original, hsi, '/' );

	if ( vci < psi )
	{
		Path_setVolume( self, CharString_substring_start_length( self->original, hsi, CharString_CountPositions( hsi, vci - 1 ) ) );			// 5
		self->common   = Path_CondensePath( &self->original[psi] );					// 6
		self->absolute = CharString_cat3( self->volume, ":", self->common );				// 7
	} else {
		Path_setVolume( self, System_CurrentVolume() );							// 5
		self->common   = Path_CondensePath( &self->original[hsi-1] );					// 6
		if ( CharString_getLength( self->volume ) )
		{
			self->absolute = CharString_cat3( self->volume, ":", self->common );			// 7
		} else {
			self->absolute = new_CharString( self->common );					// 7
		}
	}

	return self;
}

IPath* new_Path_UNC( const char* location )
{
	//	UNC				\\\C\home\Johnny\documents

	IPath* self = (IPath*) CRuntime_calloc( 1, sizeof( IPath ) );
	int next_slash;

	self->type = OPENOCL_IO_PSA_TYPE_NONE;

	//	Here we remove whitespace - removed spaces at start and end,
	//	and replaced newlines with null terminators.
	self->original = CharString_removeWhitespace( location );
	self->common = new_CharString( self->original );
	CharString_replace( self->common, '\\', '/' );

	//	Determine the protocol
	self->protocol = new_CharString( "unc" );

	next_slash = CharString_indexOfNext( self->common, 2, '/' );

	if ( '/' != self->common[2] )
	{
		//	Has host name			"//XXX/c/somedir"
		self->hostname = CharString_substring_start_length( self->common, 2, CharString_CountPositions( 2, next_slash - 1 ) );
	} else {
		self->hostname = System_Hostname();
	}
	
	if ( '/' != self->common[next_slash + 1] )
	{
		//	Has volume id			"//XXX/c/somedir" or "///c/somedir"
		int start = next_slash + 1;
		next_slash = CharString_indexOfNext( self->common, start, '/' );
		self->volume = CharString_substring_start_length( self->common, start, CharString_CountPositions( start, next_slash - 1 ) );
	} else {
		Path_setVolume( self, System_CurrentVolume() );
	}

	{
		char* tmp = self->common;
		self->common = Path_CondensePath( &tmp[next_slash] );
		if( self->volume[0] )
		{
			self->absolute = CharString_cat3( self->volume, ":", self->common );
		} else {
			self->absolute = new_CharString( self->common );
		}
		free_CharString( tmp );
	}

	return self;
}

IPath*
new_Path_WinPOSIX_dynamic( const char* location )
{
	IPath* path = NULL;
	//	Extract out environment variable
	int first = CharString_indexOfNext( location, 0, '$' );
	int last  = CharString_indexOfNext( location, first, '/' );
	int max   = CharString_getLength( location ) - 1;
	//fprintf( stderr, "location: %s\n", location );

	if ( (first < last) && (last < max) )
	{
		char* prefix   = CharString_substring_start_length( location, 0, CharString_CountPositions( 0, first - 1 ) );
		char* variable = CharString_substring_start_length( location, first + 1, CharString_CountPositions( first + 1, last - 1 ) );
		char* postfix  = CharString_substring_start_length( location, last, CharString_CountPositions( last, max ) );
		{
			const char* value = Environment_GetEnvironmentValue( variable );
			int   max = CharString_getLength( value );
			
			if ( max )
			{
				char  separator = Path_GetPathSeparator();
				char* token = NULL;
				int   next  = 0;
				//fprintf( stderr, "%s=%s\n", variable, value );

				while ( (token = CharString_token( value, next, separator )) )
				{
					int breakout = 0;
					char* trial_location = NULL;
					if ( '/' == token[0] )
					{
						trial_location = new_CharString_format_args( "%s%s", token, postfix );
					}
					else if ( ('\0' != token[0]) && ('\0' != token[1]) && (':' == token[1]) && ('\\' == token[2]) )
					{
						trial_location = new_CharString_format_args( "%s%s", token, postfix );
					}
					else
					{
						trial_location = new_CharString_format_args( "%s%s%s", prefix, token, postfix );
					}

					{
						IPath* trial_path = new_Path( trial_location );
						if ( Path_exists( trial_path ) )
						{
							path = new_Path( trial_location );
							breakout = 1;
						}

						next = CharString_indexOfNext( value, next, separator ) + 1;
						if ( next >= max ) breakout = 1;
						free_Path( trial_path );
					}
					free_CharString( trial_location );
					free_CharString( token );

					if ( breakout ) break;
				}	
			}
		}
		free_CharString( prefix );
		free_CharString( variable );
		free_CharString( postfix );
	}
	return path;
}

IPath*
new_Path_WinPOSIX( const char* location )
{
	IPath* self = (IPath*) CRuntime_calloc( 1, sizeof( IPath ) );

	int vci;
	self->type = OPENOCL_IO_PSA_TYPE_NONE;

	self->original = CharString_removeWhitespace( location );
	self->protocol = new_CharString( "file" );
	self->hostname = System_Hostname();

	vci = CharString_indexOfNext( self->original, 0, ':' );

	if ( ':' == self->original[vci] )
	{
		self->volume = CharString_substring_start_length( self->original, 0, CharString_CountPositions( 0, vci-1 ) );
		self->common = Path_CondensePath( &self->original[vci+1] );
	}
    else
	{
		self->volume = System_CurrentVolume();
		self->common = Path_CondensePath( self->original );
	}

	CharString_replace( self->common, '\\', '/' );
	if ( '/' == self->common[0] )
	{
		if ( '\0' != self->volume[0] )
		{
			self->absolute = CharString_cat3( self->volume, ":", self->common );
		} else {
			self->absolute = new_CharString( self->common );
		}
	} else {
		char* tmp = System_CurrentDirectory();
		
		if ( '/' == tmp[CharString_getLength( tmp )-1] )
		{
			self->absolute = CharString_cat2( tmp, self->common );
		} else {
			self->absolute = CharString_cat3( tmp, "/", self->common );
		}		
		free_CharString( tmp );
	}

	//if ( CharString_contains( self->absolute, "/.." ) )
	//{
	//	self->type = OPENOCL_IO_PSA_TYPE_FS_DIRECTORY_SUPER;
	//}

	return self;
}

void free_Path( IPath* self )
{
	if ( self->original )  free_CharString( self->original );
	if ( self->common )    free_CharString( self->common );
	if ( self->condensed ) free_CharString( self->condensed );
	if ( self->protocol )  free_CharString( self->protocol );
	if ( self->hostname )  free_CharString( self->hostname );
	if ( self->volume )    free_CharString( self->volume );
	if ( self->dirname )   free_CharString( self->dirname );
	if ( self->basename )  free_CharString( self->basename );
	if ( self->extension ) free_CharString( self->extension );
	if ( self->absolute )  free_CharString( self->absolute );
	if ( self->unc )       free_CharString( self->unc );
	if ( self->url )       free_CharString( self->url );
	if ( self->native )    free_CharString( self->native );
	CRuntime_free( self );
}

IPath* Path_copy( const IPath* self )
{
	return new_Path( self->common );
}

//-----------------------------------------------------------------------------
//	const functions
//-----------------------------------------------------------------------------

	//-------------------------------------------------------
	//	The following functions return members that are
	//	always set during initialization.
	//-------------------------------------------------------

const char* Path_getProtocol( const IPath* self )
{
	return self->protocol;
}

const char* Path_getHostname( const IPath* self )
{
	return self->hostname;
}

const char* Path_getVolume( const IPath* self )
{
	return self->volume;
}

const char* Path_getCommon( const IPath* self )
{
	return self->common;
}

const char* Path_getAbsolute( const IPath* self )
{
	return self->absolute;
}

	//-------------------------------------------------------


const char* Path_getCondensed( const IPath* self )
{
	if ( NULL == self->condensed )
	{
		IPath* writeable = (IPath*) self;
		writeable->condensed = Path_CondensePath( self->common );
	}
	return self->condensed;
}

const char* Path_getBasename( const IPath* self )
{
	if ( NULL == self->basename )
	{
		IPath* writeable = (IPath*) self;
		writeable->basename = CharString_basename_ifs( self->common, '/' );
	}
	return self->basename;
}

const char* Path_getDirname( const IPath* self )
{
	if ( NULL == self->dirname )
	{
		IPath* writeable = (IPath*) self;
		writeable->dirname = CharString_dirname_ifs( self->common, '/' );
	}
    return self->dirname;
}

const char* Path_getExtension( const IPath* self )
{
	if ( NULL == self->extension )
	{
		IPath* writeable = (IPath*) self;
		
		char* tmp = NULL;
		unsigned int length = CharString_getLength( self->common );
		unsigned int i;
		for ( i = length - 1; i >= 0; i-- )
		{
			if ( '.' == self->common[i] )
			{
				tmp = &(self->common[i+1]);
				break;
			}
		}
		writeable->extension = CharString_copy( tmp );
	}
	return self->extension;
}

const char* Path_getUNC( const IPath* self )
{
	if ( NULL == self->unc )
	{
		IPath* rw = (IPath*) self;
		const char* absolute = Path_getAbsolute( self );
		unsigned int i = CharString_indexOfNext( absolute, 0, '/' );
		rw->unc = new_CharString_format_args( "\\\\%s\\%s%s", self->hostname, self->volume, &absolute[i] );
		CharString_replace( self->unc, '/', '\\' );
	}
	return self->unc;
}

bool Path_hasExtension( const IPath* self, const char* extension )
{
	int status = 0;

	char* copy = CharString_copy( self->common );
	unsigned int len = strlen( copy );
	for ( len = len - 1; len >= 0; len-- )
	{
		if ( '/' == copy[len] ) {
			copy[len] = '\0';
		} else {
			break;
		}
	}
	
	if ( CharString_endsWith( copy, extension ) )
	{
	 	status = 1;
	}
	free_CharString( copy );
	return status;
}

bool Path_isAbsolute( const IPath* self )
{
	int absolute = 0;
	if ( '/' == *self->common )
	{
		absolute = 1;
	} else {
		const char* name = self->common;

		while ( '\0' != *name )
		{
			if ( (':' == *name) && ('/' == *(++name)) )
			{
				absolute = 1;
				break;
			}
			name++;
		}
	}
	return absolute;
}

bool
Path_exists( const IPath* self )
{
	return (bool) Path_getType( self );
}

//--------------------------------------------------------------
//	Allocating const functions
//--------------------------------------------------------------

char* Path_relativeTo( const IPath* self, const char* location )
{
	char* relative = new_CharString( "" );

	if ( location && CharString_getLength( location ) )
	{
		unsigned int backoff = 0;
		IPath* other_path = new_Path( location );
		const char* self_abs = Path_getAbsolute( self );
		const char* other_abs = Path_getAbsolute( other_path ); 

		const char* s = self_abs;
		const char* o = other_abs;

		while ( (*s == *o) && ( '\0' != *s ) )
		{
			s++;
			o++;
		}
	
		while ( ('/' == *s) && ('\0' != *s) ) {
			s++;
		}
	
		while ( ('/' == *o) && ('\0' != *o) ) {
			o++;
		}
	
		{
			bool loop = 1;
			bool text = 0;

			while ( '\0' != *o )
			{
				switch( *o )
				{
				case '\0':
					loop = 0;
					if ( text ) backoff++;
					break;
				case '/':
					text = 0;
					break;
				default:
					if ( 0 == text )
					{
						backoff++;
						text = 1;
					}
				}
				o++;
			}
		}
	
		{
			char* tmp;
			unsigned int i;
			for ( i=0; i < backoff; i++ )
			{
				tmp = CharString_cat2( relative, "../" );
				free_CharString( relative );
				relative = tmp;
			}			

			tmp = CharString_cat2( relative, s );
			free_CharString( relative );
			relative = tmp;
		}

		free_Path( other_path );
	}

	return relative;
}

/*
const char* Path_getNativeFormat( const IPath* self )
{
	if ( NULL == self->native )
	{
		IPath* writeable = (IPath*) self;
		writeable->native = new_CharString( self->common );
		CharString_replace( writeable->native, '/', '\\' );
	}
	return self->native;
}
*/


/*
 *  Moved to PSA dir 7 Feb 05.
 *
 *
int
Path_getType( const IPath* self )
{
	int type = OPENOCL_IO_PSA_TYPE_NONE;

	IDirectory* dir = new_Directory_path( self );
	if ( Directory_isDirectory( dir ) )
	{
		type = OPENOCL_IO_PSA_TYPE_FS_DIRECTORY;
	} else {
		IFile* file = new_File_path( self );
		if ( File_exists( file ) )
		{
			type = OPENOCL_IO_PSA_TYPE_FS_FILE;
		}
		free_File( file );
	}
	free_Directory( dir );
	return type;
}
*/


/*
IPath* Path_GetCurrentDirectory()
{
	IPath* path;
	char* current_dir = (char*) CRuntime_calloc( 1024, sizeof( char ) );
	current_dir = (char*) _getcwd( current_dir, 1024 );
	path = new_Path( current_dir );
	free( current_dir );
	return path;
}
*/

char* Path_CondensePath( const char* aPath )
{
	char* canonical_path = NULL;
	char* path = new_CharString( aPath );

	IStack* stack = new_Stack();
	int now  = 0;
	int last = -1;

	CharString_replace( path, '\\', '/' );
	
	while ( '\0' != path[now] )
	{
		switch ( path[now] )
		{
		case '/':
			if ( (now != last) && (now != (last + 1) ) )
			{
				char* token = CharString_substring_start_length( path, last + 1, CharString_CountPositions( last + 1, now - 1 ) );
				if ( 0 == CharString_compare( token, "." ) )
				{
					free_CharString( token );
				}
				else if ( 0 == CharString_compare( token, ".." ) )
				{
					char* popped = Stack_pop( stack );
					if ( popped )
					{
						free_CharString( popped );
						free_CharString( token );
					} else {
						Stack_addItem( stack, token );
					}
				}
				else
				{
					Stack_addItem( stack, token );
				}
			}
			last = now;
			// Intentional fall through
		default:
			now++;
		}
	}

	if ( (now != last) && (now != (last + 1) ) )
	{
		char* token = CharString_substring_start_length( path, last + 1, CharString_CountPositions( last + 1, now - 1 ) );
		if ( 0 == CharString_compare( token, "." ) )
		{
			free_CharString( token );
		}
		else if ( 0 == CharString_compare( token, ".." ) )
		{
			switch ( 0 )
			{
			case 0:
				//	This code path removes any extra .s of ..s from a
				//	root path, ie. /.. --> /
				free_CharString( token );
				free_CharString( Stack_pop( stack ) );
				break;
			case 1:
				{
					//	This code path keeps extra .s of ..s on a
					//	root path, ie. /.. --> /..
					char* popped = Stack_pop( stack );
					if ( popped )
					{
						free_CharString( popped );
						free_CharString( token );
					} else {
						Stack_addItem( stack, token );
					}
				}
				break;
			}
		}
		else
		{
			Stack_addItem( stack, token );
		}
	}

	{
		const char** cs_list = Stack_getAsCharStringList( stack );
		char* tmp = new_CharString_delimiter( cs_list, '/' );

		if ( '/' == path[0] )
		{
			canonical_path = CharString_cat2( "/", tmp );
			free_CharString( tmp );
		} else {
			canonical_path = tmp;
		}
	}
	{
		char* tmp = NULL;
		while ( (tmp = Stack_pop( stack )) ) free_CharString( tmp );
	}
	free_Stack( stack );
	free_CharString( path );

	return canonical_path;
}

/*
char* Path_ConvertToNative( const char* path )
{
	char* native = CharString_copy( path );
	CharString_replace( native, '/', '\\' );
	return native;
}
*/

/*
IList* Path_ConvertListToNative( const IList* list )
{
	IList* native_list = new_List();

	char* quoted_native;
	char* tmp;
	unsigned int count = List_count( list );
	unsigned int i;
	for ( i=0; i < count; i++ )
	{
		quoted_native = Path_ConvertToNative( list->items[i] );
		if ( '\"' != quoted_native[0] )
		{
			tmp = quoted_native;
			quoted_native = CharString_cat3( "\"", tmp, "\"" );
			free( tmp );
		}
		List_addItem( native_list, quoted_native );
	}
	return native_list;
}
*/

bool Path_ChangeCurrentDirectory( const char* location )
{
	bool status = 0;
	IPath* current_path = NULL;
	if ( NULL == WorkingDirectories )
	{
		//int before = CRuntime_GetAllocationCount();
		//int after;
		WorkingDirectories = new_Stack();
		//after = CRuntime_GetAllocationCount();

		//	WorkingDirectories is a singlton, which exists until
		//	the termination of the program. We decrement the number
		//	of allocated objects here so that it doesn't affect
		//	memory leak detection.

		//CRuntime_AlterAllocationCount( before - after );
	}
	current_path = Path_GetCurrentDirectory();
	if ( current_path )
	{
		const char* current_location = Path_getCondensed( current_path );
		Stack_copyItem( WorkingDirectories, current_location );
		if ( !(status = Path_SetCurrentDirectory( location )) )
		{
			free_CharString( Stack_pop( WorkingDirectories ) );
		}
		free_Path( current_path );
	}
	return status;
}

bool Path_RevertCurrentDirectory()
{
	bool status = 0;
	if ( WorkingDirectories )
	{
		char* previous_location = Stack_pop( WorkingDirectories );
		if ( previous_location )
		{
			status = Path_SetCurrentDirectory( previous_location );
			free_CharString( previous_location );
		}
	}
	return status;
}

char
Path_GetFileSeparator()
{
	return Environment_GetFileSeparator();
}

char
Path_GetPathSeparator()
{
	return Environment_GetPathSeparator();
}


//---------------------------------------------------------
//	Private functions
//---------------------------------------------------------

void
Path_setVolume( IPath* self, char* volume )
{
	int max = CharString_getLength( volume );
	int i;
	if ( self->volume )
	{
		free_CharString( self->volume );
	}
	for ( i=0; i < max; i++ )
	{
		volume[i] = toupper( volume[i] );
	}
	self->volume = volume;
}

