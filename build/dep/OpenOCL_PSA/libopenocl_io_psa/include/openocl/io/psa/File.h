/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_FILE_H
#define OPENOCL_IO_PSA_FILE_H

#include "openocl/io/psa/Path.h"
#include <openocl/base/psa.h>
#include <openocl/base/psa/Time.h>

#ifdef __cplusplus
	using namespace openocl::base::psa;

	namespace openocl {
		namespace io {
			namespace psa {
	extern "C" {
#endif

typedef struct _File IFile;

#define OPENOCL_IO_PSA_FILE_STDIN  0
#define OPENOCL_IO_PSA_FILE_STDOUT 1
#define OPENOCL_IO_PSA_FILE_STDERR 2

EXPORT	IFile*	new_File( const char* location );
EXPORT	IFile*	new_File_path( const IPath* path );
EXPORT	void	free_File( IFile* self );
EXPORT	int		File_open( IFile* self, const char* access );
EXPORT	int             File_flush( IFile* self );
EXPORT	int		File_close( IFile* self );

/*
 *	Returns nr of bytes read, or 0 if error.
 */
EXPORT	char*           File_readCharacters( IFile* self );
EXPORT	unsigned int    File_read( const IFile* self, byte* buffer, unsigned int bufferSize );
EXPORT	unsigned int    File_write( IFile* self, const byte* buffer, unsigned int bufferSize );

EXPORT	const IPath*    File_getPath( const IFile* self );

EXPORT	int             File_create( const IFile* self );
EXPORT	int             File_delete( const IFile* self );
EXPORT	int             File_remove( const IFile* self );

EXPORT	bool            File_copyTo( const IFile* self, const IPath* target, bool force );
EXPORT	bool            File_exists( const IFile* self );
EXPORT	bool            File_isNewerThan( const IFile* self, const IFile* other );
EXPORT	bool            File_isOpen( const IFile* self );
EXPORT	bool            File_isEOF( const IFile* self );

EXPORT	const ITime*	File_getLastAccess( const IFile* self );
EXPORT	const ITime*	File_getLastModification( const IFile* self );
EXPORT	const ITime*	File_getLastChange( const IFile* self );

EXPORT	IFile*		File_GetFileForStandardDescriptor( flags stream );
EXPORT	IFile*		File_GetFileForStandardStream( void* stream );
EXPORT	IFile*		File_GetFileForStream( void* stream );

#ifdef __cplusplus
	}};};};
#endif
#endif
