/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_PATH_H
#define OPENOCL_IO_PSA_PATH_H

#include <openocl/base/psa/export.h>

#include <openocl/base/psa.h>
#include <openocl/base/psa/List.h>

#ifdef __cplusplus
	namespace openocl {
		namespace io {
			namespace psa {
	extern "C" {
#endif

typedef struct _Path IPath;

#define OPENOCL_IO_PSA_TYPE_NONE                   0x0000
#define OPENOCL_IO_PSA_TYPE_UNKNOWN                0x0001
#define OPENOCL_IO_PSA_TYPE_FS                     0x0011
#define OPENOCL_IO_PSA_TYPE_FS_DEVICE              0x0111
#define OPENOCL_IO_PSA_TYPE_FS_DEVICE_BLOCK        0x1111
#define OPENOCL_IO_PSA_TYPE_FS_DEVICE_CHARACTER    0x2111
#define OPENOCL_IO_PSA_TYPE_FS_DIRECTORY           0x0211
#define OPENOCL_IO_PSA_TYPE_FS_DIRECTORY_SUPER     0x1211
#define OPENOCL_IO_PSA_TYPE_FS_FILE                0x0411
#define OPENOCL_IO_PSA_TYPE_FS_LINK                0x0811
#define OPENOCL_IO_PSA_TYPE_FS_SYMLINK             0x0811
#define OPENOCL_IO_PSA_TYPE_IO                     0x0020
#define OPENOCL_IO_PSA_TYPE_IO_FIFO                0x0120
#define OPENOCL_IO_PSA_TYPE_IO_SOCKET              0x0220
#define OPENOCL_IO_PSA_TYPE_NET                    0x0040

//
//  Lifecycle functions
//
EXPORT	IPath* new_Path( const char* location );
EXPORT	void  free_Path( IPath* self );

EXPORT	IPath* Path_copy( const IPath* self );

//
//  Accessors
//
EXPORT	const char* Path_getCommon( const IPath* self );
EXPORT	const char* Path_getCondensed( const IPath* self );
EXPORT	const char* Path_getProtocol( const IPath* self );
EXPORT	const char* Path_getHostname( const IPath* self );
EXPORT	const char* Path_getVolume( const IPath* self );
EXPORT	const char* Path_getDirname( const IPath* self );
EXPORT	const char* Path_getBasename( const IPath* self );
EXPORT	const char* Path_getExtension( const IPath* self );
EXPORT	const char* Path_getAbsolute( const IPath* self );
EXPORT	const char* Path_getUNC( const IPath* self );
EXPORT	const char* Path_getURL( const IPath* self );
EXPORT	const char* Path_getNativeFormat( const IPath* self );

EXPORT	bool        Path_hasExtension( const IPath* self, const char* extension );
EXPORT	bool        Path_isAbsolute( const IPath* self );
EXPORT	bool        Path_isLink( const IPath* self );
EXPORT	bool        Path_exists( const IPath* self );

EXPORT	char*       Path_relativeTo( const IPath* self, const char* location );
EXPORT	char*		Path_retrieveLinkTarget( const IPath* self );

EXPORT	IPath*      Path_GetCurrentDirectory();
EXPORT	bool        Path_SetCurrentDirectory( const char* location );
EXPORT	bool        Path_ChangeCurrentDirectory( const char* location );
EXPORT	bool        Path_RevertCurrentDirectory();
EXPORT	void        Path_ClearDirectoryHistory();
EXPORT	char		Path_GetFileSeparator();
EXPORT	char		Path_GetPathSeparator();

EXPORT	char*       Path_CondensePath( const char* path );
EXPORT	char*       Path_ConvertToNative( const char* path );
EXPORT	IList*      Path_ConvertListToNative( const IList* list );

EXPORT	int         Path_getType( const IPath* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
