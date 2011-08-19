/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>

#include "openocl/io/psa/File.private.h"

#include "openocl/io/psa/Directory.h"
#include <openocl/base/psa/CharString.h>

//#include <unistd.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

/*
IFile* new_File( const IPath* location )
{
	IFile* self = calloc( 1, sizeof( IFile ) );
	self->path = Path_copy( location );
	self->closeOnFree = 1;
	return self;
}
*/

/*
void free_File( IFile* self )
{
	if ( self->path )
	{
		free_Path( self->path );
	}
	if ( self->stream && self->closeOnFree )
	{
		File_close( self );
	}
	free( self );
}
*/
/*
int File_open( IFile* self, const char* access )
{
	int status = 0;

	self->stream = fopen( Path_getNativeFormat( self->path ), access );
	if ( self->stream )
	{
		status = 1;
	}
	return status;
}
*/
/*
int File_close( IFile* self )
{
	int status;
	if ( 0 == fclose( self->stream ) )
	{
		status = 1;
	}
	return status;
}
*/
/*
char* File_readCharacters( IFile* self )
{
	//
	//	At most read one less than buffer size so that buffer is
	//	null terminated.
	//
	void* buffer = calloc( 1024, sizeof( char ) );
	const unsigned char* buf = (unsigned char*) buffer;

	//
	//	fread( buffer, 1023, 1, self->stream );
	//
	File_read( self, buffer, 1023 );

	//
	//	If file is not ASCII, i.e., negative char values, return 0.
	//
	{
		unsigned int i;
		for ( i=0; i < 1024; i++ )
		{
			if ('\0' == buf[i])
			{
				break;
			}
			else if ( 127 < buf[i] )
			{
				free( buffer );
				buffer = NULL;
				break;
			}
		}
	}
	return buffer;
}
*/
/*
unsigned int File_read( IFile* self, byte* buffer, unsigned int bufferSize )
{
	unsigned int read = fread( buffer, 1, bufferSize, self->stream );
	if ( feof( self->stream ) )
	{
		self->eof = 1;
	}
	return read;
}
*/
/*
unsigned int File_write( IFile* self, const byte* buffer, unsigned int bufferSize )
{
	return fwrite( buffer, bufferSize, 1, self->stream );
}
*/
/*
const IPath* File_getPath( const IFile* self )
{
	return self->path;
}
*/
int File_create( const IFile* self )
{
	int status;
	if ( -1 != (status = _creat( Path_getNativeFormat( self->path ), _S_IREAD | _S_IWRITE )) )
	{
		_close( status );
		status = 1;
	} else {
		status = 0;
	}
	return status;	
}

int File_delete( const IFile* self )
{
	int status = 0;
	if ( DeleteFile( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
	return status;
}

int File_remove( const IFile* self )
{
	int status = 0;
	if ( DeleteFile( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
	return status;
}

bool File_copyTo( const IFile* self, const IPath* target, bool force )
{
	int status = 0;

	BOOL bFailIfExists;
	if ( force ) {
		bFailIfExists = FALSE;
	} else {
		bFailIfExists = TRUE;
	}

	{
	IDirectory* d = new_Directory_path( target );
	if ( Directory_exists( d ) )
	{
		char* path = CharString_cat3( Path_getCommon( target ), "/", Path_getBasename( self->path ) );
		IPath* full_target = new_Path( path );
		
		if ( CopyFile( Path_getNativeFormat( self->path ), Path_getNativeFormat( full_target ), bFailIfExists ) )
		{
			status = 1;
		}
		free_Path( full_target );
		free( path );

	} else {
		if ( CopyFile( Path_getNativeFormat( self->path ), Path_getNativeFormat( target ), bFailIfExists ) )
		{
			status = 1;
		}
	}
	}
	return status;
}

/*
bool File_exists( const IFile* self )
{
	bool exists = 0;
	FILE* f = fopen( Path_getNativeFormat( self->path ), "r" );
	if ( f )
	{
		fclose( f );
		exists = 1;
	}
	return exists;
}
*/

/*
bool File_isOpen( const IFile* self )
{
	if ( self->stream )
	{
		return 1;
	} else {
		return 0;
	}
}
*/
/*
bool File_isEOF( const IFile* self )
{
	return self->eof;
}
*/
/*
IFile* File_GetFileForStandardDescriptor( flags stream )
{
	IFile* self = calloc( 1, sizeof( IFile ) );
	self->path = new_Path( NameForDescriptor( stream ) );
	self->closeOnFree = 1;
	
	switch( stream )
	{
	case OPENOCL_IO_PSA_FILE_STDIN:
		self->stream = fdopen( fileno( stdin ), "r" );
		break;
	case OPENOCL_IO_PSA_FILE_STDOUT:
		self->stream = fdopen( fileno( stdout ), "w" );
		break;
	case OPENOCL_IO_PSA_FILE_STDERR:
		self->stream = fdopen( fileno( stderr ), "w" );
		break;
	default:
		free_File( self );
		self = NULL;
	}
	
	return self; 
}
*/
/*
IFile* File_GetFileForStandardStream( void* stream )
{
	int fd1 = fileno( (FILE*) stream );

	IFile* self = calloc( 1, sizeof( IFile ) );
	self->path = new_Path( NameForDescriptor( fd1 ) );
	self->closeOnFree = 1;

	switch( fd1 )
	{
	case OPENOCL_IO_PSA_FILE_STDIN:
		self->stream = fdopen( fileno( stdin ), "r" );
		break;
	case OPENOCL_IO_PSA_FILE_STDOUT:
		self->stream = fdopen( fileno( stdout ), "w" );
		break;
	case OPENOCL_IO_PSA_FILE_STDERR:
		self->stream = fdopen( fileno( stderr ), "w" );
		break;
	default:
		self->stream = stream;
		break;
	}
	
	return self; 
}
*/
/*
IFile* File_GetFileForStream( void* stream )
{
	IFile* self = calloc( 1, sizeof( IFile ) );
	self->path = new_Path( "UNKNOWN" );
	self->stream = stream;
	self->closeOnFree = 0;
		
	return self; 
}	
*/
/*
const char* NameForDescriptor( int fd )
{
	const char* name;
	switch ( fd )
	{
	case OPENOCL_IO_PSA_FILE_STDIN:
		name = "STDIN";
		break;
	case OPENOCL_IO_PSA_FILE_STDOUT:
		name = "STDOUT";
		break;
	case OPENOCL_IO_PSA_FILE_STDERR:
		name = "STDERR";
		break;
	default:
		name = "UNKNOWN";
	}
	return name;
}
*/
