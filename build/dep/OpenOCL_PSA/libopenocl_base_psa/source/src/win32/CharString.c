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
#include <stdlib.h>
#include <stdio.h>

/*
 *  _vsnprintf is detailed at:
 *  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclib/html/vclrf_vscprintf_vscwprintf.asp
 */
char* new_CharString_format_valist( const char* format, va_list args )
{
	int written;
	int size = 128;
	char* self = CRuntime_calloc( size, sizeof( char ) );

	while( -1 == (written = _vsnprintf( self, size, format, args )) )
	{
		CRuntime_free( self );
		size *= 2;
		self = CRuntime_calloc( size, sizeof( char ) );
	}
	return self;
}
