/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_CHARSTRING_H
#define OPENOCL_BASE_PSA_CHARSTRING_H

#include "openocl/base/psa/export.h"
#include <stdarg.h>

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#else
#ifndef BOOLEAN
#define BOOLEAN
	typedef int bool;
#endif
#endif

/*
 *  Lifecycle functions
 */
EXPORT char* new_CharString( const char* charString );
EXPORT char* new_CharString_delimiter( const char** tokens, const char delimiter );
EXPORT char* new_CharString_format_args( const char* format, ... );
EXPORT char* new_CharString_format_valist( const char* format, va_list args );
EXPORT void  free_CharString( char* self );

/*
 *  Functions
 */
EXPORT void CharString_replace( char* str, char old, char c );


/*
 *  Const Allocating functions
 */
EXPORT char* CharString_between( const char* self, const char* prefix, const char* suffix );
EXPORT char* CharString_cat2( const char* self, const char* two );
EXPORT char* CharString_cat3( const char* self, const char* two, const char* three );
EXPORT char* CharString_copy( const char* self );
EXPORT char* CharString_removeWhitespace( const char* self );
EXPORT char* CharString_substring( const char* self, int start, int end );
EXPORT char* CharString_substring_start_length( const char* self, int start, int length );
EXPORT char* CharString_token( const char* self, unsigned int start, char delimiter );

EXPORT char* CharString_basename( const char* path );
EXPORT char* CharString_basename_ifs( const char* path, char ifs );
EXPORT char* CharString_dirname( const char* path );
EXPORT char* CharString_dirname_ifs( const char* path, char ifs );

/*
 *  Boolean functions
 */
EXPORT bool CharString_contains( const char* self, const char* token );
EXPORT bool CharString_startsWith( const char* self, const char* token );
EXPORT bool CharString_endsWith( const char* self, const char* token );
EXPORT bool CharString_matches( const char* self, const char* pattern );

/*
 *  Metric functions
 */
EXPORT int CharString_compare( const char* self, const char* other ); 

/*
 *  Information functions
 */
EXPORT unsigned int CharString_getLength( const char* xxx );
EXPORT unsigned int CharString_indexOfNext( const char* self, unsigned int start, char delimiter );

EXPORT int CharString_CountPositions( int start, int end );


#ifdef __cplusplus
}
};};};
#endif
#endif
