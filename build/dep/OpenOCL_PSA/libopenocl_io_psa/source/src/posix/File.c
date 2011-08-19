/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/io/psa/File.private.h"
#include "openocl/io/psa/Directory.h"
#include "openocl/io/psa/Path.h"

#include <openocl/base/psa/CharString.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

/*
IFile* new_File( const IPath* location )
{
	IFile* self = CRuntime_calloc( 1, sizeof( IFile ) );
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

	if ( File_isOpen( self ) )
	{
		status = 1;
	} else {
		self->stream = fopen( Path_getNativeFormat( self->path ), access );
		if ( self->stream )
		{
			status = 1;
		}
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
		self->stream = NULL;
		self->eof = 0;
		status = 1;
	}
	return status;
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
	if ( -1 != (status = creat( Path_getNativeFormat( self->path ), S_IRWXU | S_IRWXG | S_IRWXO )) )
	{
		close( status );
		status = 1;
	} else {
		status = 0;
	}
	return status;	
}

int File_delete( const IFile* self )
{
	int status = 0;

	if ( 0 == unlink( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
		
	return status;
}

int File_remove( const IFile* self )
{
	int status = 0;

	if ( 0 == unlink( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
		
	return status;
}

int File_copyTo( const IFile* self, const IPath* targetPath, int force )
{
	bool status = 0;

	char* target = NULL;
	IFile* target_file = NULL;
	IDirectory* target_dir = new_Directory_path( targetPath );
	if ( Directory_exists( target_dir ) )
	{
		target = CharString_cat3( Path_getCommon( targetPath ), "/", Path_getBasename( self->path ) );
	} else {
		target = CharString_copy( Path_getCommon( targetPath ) );
	}

	target_file = new_File( target );
	if ( !File_exists( target_file ) || force )
	{
		if ( File_open( target_file, "w" ) )
		{
			byte buffer[256];
			int read;
		
			while ( (read = File_read( self, buffer, 256 )) )
			{
				File_write( target_file, buffer, read );
				status = 1;
			}
			if ( !status )
			{
				fprintf( stdout, "File.c::copyTo: could not copy data\n" );
			}
		}
		File_close( target_file );
	}		
	free_Directory( target_dir );
	free_File( target_file );
	free_CharString( target );	

	return status;
}

/*
int File_copyTo( const IFile* self, const IPath* target, int force )
{
	int status = 0;
	FILE* src = NULL;
	FILE* tgt = NULL;

	char buffer[256];
	unsigned int read;
	
	int flags = O_WRONLY | O_CREAT;
	if ( 0 == force ) flags |= O_EXCL;
	mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

	char* target_file = NULL;
	IPath* real_target = NULL;
	IDirectory* dir = new_Directory_path( target );
	if ( Directory_exists( dir ) )
	{
		target_file = CharString_cat3( Path_getCommon( target ), "/", Path_getBasename( self->path ) );
	} else {
		target_file = CharString_copy( Path_getCommon( target ) );
	}
	real_target = new_Path( target_file );
	
	if ( (src = fopen( Path_getNativeFormat( self->path ), "r" )) )
	{
		int fd = open( Path_getNativeFormat( real_target ), flags, mode );
	
		if ( (tgt = fdopen( fd, "w" )) )
		{
			while( (read = fread( (void*) buffer, 256, 1, src )) )
			{
				fwrite( (void*) buffer, 256, 1, tgt );
				status = 1;
			}
			if ( !status )
			{
				fprintf( stdout, "IOPSA.c: could not copy data\n" );
			}
			fclose( tgt );
		} else {
			fprintf( stdout, "IOPSA.c: could not open %s for writing\n", Path_getNativeFormat( real_target ) );
		}
		fclose( src );
	} else {
		fprintf( stdout, "IOPSA.c: could not open %s for reading\n", Path_getNativeFormat( self->path ) );
	}
	free_Path( real_target );
	free( target_file );

	return status;
}
*/
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
	else if ( EACCES == errno )
	{
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
	IFile* self = CRuntime_calloc( 1, sizeof( IFile ) );
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
	return File_GetFileForStandardDescriptor( fd1 );
}
*/

/*
IFile* File_GetFileForStream( void* stream )
{
	IFile* self = CRuntime_calloc( 1, sizeof( IFile ) );
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
