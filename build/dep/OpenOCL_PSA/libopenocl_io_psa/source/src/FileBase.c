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
#include <openocl/base/psa/CharString.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

IFile* new_File( const char* location )
{
	IFile* self = CRuntime_calloc( 1, sizeof( IFile ) );
	self->path = new_Path( location );
	self->closeOnFree = 1;
	return self;
}

IFile* new_File_path( const IPath* path )
{
	IFile* self = CRuntime_calloc( 1, sizeof( IFile ) );
	self->path = Path_copy( path );
	self->closeOnFree = 1;
	return self;
}

void free_File( IFile* self )
{
	if ( self->path ) free_Path( self->path );
	if ( self->stream && self->closeOnFree ) File_close( self );
	if ( self->times.lastAccess ) free_Time( self->times.lastAccess );
	if ( self->times.lastModification ) free_Time( self->times.lastModification );
	if ( self->times.lastChange ) free_Time( self->times.lastChange );
	CRuntime_free( self );
}

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

int File_flush( IFile* self )
{
	int status = 0;
	if ( 0 == fflush( self->stream ) )
	{
		status = 1;
	}
	return status;
}

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

char* File_readCharacters( IFile* self )
{
	/*
	 *	At most read one less than buffer size so that buffer is
	 *	null terminated.
	 */
	void* buffer = CRuntime_calloc( 1024, sizeof( char ) );
	const unsigned char* buf = (unsigned char*) buffer;

	/*
	 *	fread( buffer, 1023, 1, self->stream );
	 */
	File_read( self, buffer, 1023 );

	/*
	 *	If file is not ASCII, i.e., negative char values, return 0.
	 */
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
				CRuntime_free( buffer );
				buffer = NULL;
				break;
			}
		}
	}
	return buffer;
}

unsigned int File_read( const IFile* self, byte* buffer, unsigned int bufferSize )
{
	unsigned int read = fread( buffer, 1, bufferSize, self->stream );
	if ( feof( self->stream ) )
	{
		((IFile*)self)->eof = 1;
	}
	return read;
}

unsigned int File_write( IFile* self, const byte* buffer, unsigned int bufferSize )
{
	return fwrite( buffer, bufferSize, 1, self->stream );
}

const IPath* File_getPath( const IFile* self )
{
	return self->path;
}

/*
int File_create( const IFile* self )
{
	int status;
	if ( -1 != (status = _creat( Path_getNativeFormat( self->path ), _S_IREAD | _S_IWRITE )) )
	{
		close( status );
		status = 1;
	} else {
		status = 0;
	}
	return status;	
}
*/

/*
int File_delete( const IFile* self )
{
	int status = 0;
	if ( DeleteFile( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
	return status;
}
*/

/*
int File_remove( const IFile* self )
{
	int status = 0;
	if ( DeleteFile( Path_getNativeFormat( self->path ) ) )
	{
		status = 1;
	}
	return status;
}
*/

/*
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
*/

bool File_exists( const IFile* self )
{
	bool exists = 0;
	FILE* f = fopen( Path_getNativeFormat( self->path ), "r" );
	if ( f )
	{
		fclose( f );
		exists = 1;
	}
	//else if ( EACCES == errno )
	//{
	//	exists = 1;
	//}
	return exists;
}

bool File_isNewerThan( const IFile* self, const IFile* other )
{
	const ITime* self_modification_time  = File_getLastModification( self );
	const ITime* other_modification_time = File_getLastModification( other );
	
	unsigned long long self_seconds  = Time_getSecondsSinceEpoch( self_modification_time );
	unsigned long long other_seconds = Time_getSecondsSinceEpoch( other_modification_time );

	//const char* self_name = Path_getBasename( File_getPath( self ) );
	//const char* self_text = Time_getTextual( self_modification_time );
	//const char* other_name = Path_getBasename( File_getPath( other ) );
	//const char* other_text = Time_getTextual( other_modification_time );
	//fprintf( stderr, "File_isNewerThan: %s@%s compared to %s@%s\n", self_name, self_text, other_name, other_text );
	
	return ( self_seconds > other_seconds );
}

bool File_isOpen( const IFile* self )
{
	if ( self->stream )
	{
		return 1;
	} else {
		return 0;
	}
}

bool File_isEOF( const IFile* self )
{
	return self->eof;
}

const ITime*
File_getLastAccess( const IFile* self )
{
	if ( !self->times.lastAccess )
	{
		File_retrieveTimes( (IFile*) self );
	}
	return self->times.lastAccess;
}

const ITime*
File_getLastModification( const IFile* self )
{
	if ( !self->times.lastModification )
	{
		File_retrieveTimes( (IFile*) self );
	}
	return self->times.lastModification;
}

const ITime*
File_getLastChange( const IFile* self )
{
	if ( !self->times.lastChange )
	{
		File_retrieveTimes( (IFile*) self );
	}
	return self->times.lastChange;
}

IFile* File_GetFileForStandardDescriptor( flags stream )
{
	IFile* self = CRuntime_calloc( 1, sizeof( IFile ) );
	self->path = new_Path( NameForDescriptor( stream ) );
	self->closeOnFree = 0;
	
	switch( stream )
	{
	case OPENOCL_IO_PSA_FILE_STDIN:
		self->stream = stdin;
		break;
	case OPENOCL_IO_PSA_FILE_STDOUT:
		self->stream = stdout;
		break;
	case OPENOCL_IO_PSA_FILE_STDERR:
		self->stream = stderr;
		break;
	default:
		free_File( self );
		self = NULL;
	}
	
	return self; 
}

IFile* File_GetFileForStandardStream( void* stream )
{
	int fd1 = 4; // Unknown fileno( (FILE*) stream );

	IFile* self = CRuntime_calloc( 1, sizeof( IFile ) );
	self->path = new_Path( NameForDescriptor( fd1 ) );
	self->closeOnFree = 1;

	switch( fd1 )
	{
	case OPENOCL_IO_PSA_FILE_STDIN:
		self->stream = freopen( NULL, "rb", stream );
		break;
	case OPENOCL_IO_PSA_FILE_STDOUT:
		self->stream = freopen( NULL, "wb", stream );
		break;
	case OPENOCL_IO_PSA_FILE_STDERR:
		self->stream = freopen( NULL, "wb", stream );
		break;
	default:
		self->stream = stream;
		break;
	}
	
	return self; 
}

IFile* File_GetFileForStream( void* stream )
{
	IFile* self = NULL;
	if ( stream )
	{
		self = CRuntime_calloc( 1, sizeof( IFile ) );
		self->path = new_Path( "UNKNOWN" );
		self->stream = stream;
		self->closeOnFree = 0;
	}
	return self;
}	

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

void
File_retrieveTimes( IFile* self )
{
	struct stat buf;
	stat( Path_getCondensed( File_getPath( self ) ), &buf );
	
	self->times.lastAccess = new_Time( buf.st_atime );
	self->times.lastModification = new_Time( buf.st_mtime );
	self->times.lastChange = new_Time( buf.st_ctime );
}

