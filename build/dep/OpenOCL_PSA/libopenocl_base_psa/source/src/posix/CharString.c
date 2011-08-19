/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/*
 *  The only difference between this and the win32 version
 *  is that the win32 version has an underscore ('_') before
 *  the function name.
 */
char* new_CharString_format_valist( const char* format, va_list args )
{
	unsigned int written;
	unsigned int size = 512;
	char* self = CRuntime_calloc( size, sizeof( char ) );

	written = vsnprintf( self, size, format, args );
	if ( written >= size )
	{
		CRuntime_free( self );
		self = CRuntime_calloc( written + 1, sizeof( char ) );
		written = vsnprintf( self, written + 1, format, args );
	}
	return self;
}
