/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_PATH_PRIVATE_H
#define OPENOCL_IO_PSA_PATH_PRIVATE_H

#include "openocl/io/psa/Path.h"

IPath* new_Path_UNC( const char* location );
IPath* new_Path_URL( const char* location );
IPath* new_Path_WinPOSIX_dynamic( const char* location );
IPath* new_Path_WinPOSIX( const char* location );
void Path_setVolume( IPath* self, char* volume );

struct _Path
{
	char* original;
	char* protocol;
	char* hostname;
	char* volume;
	char* common;
	char* absolute;

	char* basename;
	char* condensed;
	char* dirname;
	char* extension;
	char* native;
	char* unc;
	char* url;
	int   type;
};

#endif
