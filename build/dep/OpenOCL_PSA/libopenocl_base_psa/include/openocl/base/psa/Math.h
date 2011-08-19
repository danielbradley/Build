/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_MATH_H
#define OPENOCL_BASE_SPA_MATH_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

byte ToHex( char hexChar );
EXPORT  int		Math_RoundToInteger_double( double aNumber );

EXPORT	double		Math_AsDouble( const char* str );
EXPORT	long double	Math_AsLongDouble( const char* str );
EXPORT	long long int	Math_AsLongLong( const char* str );
EXPORT	long long int	Math_AsLongLong_base( const char* str, unsigned int base );
EXPORT  unsigned long long Math_HexAsLongLong( const char* hexStr );


#ifdef __cplusplus
}
};};};
#endif
#endif
