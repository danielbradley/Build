/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_FILE_PRIVATE_H
#define OPENOCL_IO_PSA_FILE_PRIVATE_H

#include "openocl/io/psa/File.h"
#include "openocl/io/psa/Path.h"
#include <openocl/base/psa/Time.h>
#include <stdio.h>

struct _IFileTimes
{
	ITime* lastAccess;
	ITime* lastModification;
	ITime* lastChange;
};

struct _File
{
	IPath* path;
	FILE*  stream;
	bool   eof;
	bool   closeOnFree;
	struct _IFileTimes times;
};

const char* NameForDescriptor( int fd );
void        File_retrieveTimes( IFile* self );

#endif
