/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Math.h"
#include <stdlib.h>

extern long double strtold(const char *nptr, char **endptr);

int
Math_RoundToInteger_double( double aNumber )
{
	double big = aNumber * 1000;
	int big_int = (int) big;
	int modulo = (big_int % 1000);
	if ( modulo >= 500 )
	{
		big_int += (1000 - modulo);
	} else {
		big_int -= modulo;
	}
	return big_int / 1000;
}

double
Math_AsDouble( const char* str )
{
	long double value;
	char* endptr;

	if ( NULL != str )
	{
		if ( 0 == (value = strtod( str, &endptr )) )
		{
			if ( endptr == str )
			{
				//throw new Exception();
			}
		}
	}
	return value;
}

long double
Math_AsLongDouble( const char* str )
{
	long double value;
	char* endptr;

	if ( NULL != str )
	{
		if ( 0 == (value = strtold( str, &endptr )) )
		{
			if ( endptr == str )
			{
				//throw new Exception();
			}
		}
	}
	return value;
}

long long int
Math_AsLongLong( const char* str )
{
	long long int value;
	char* endptr;

	if ( NULL != str )
	{
		if ( 0 == (value = strtoll( str, &endptr, 10 )) )
		{
			if ( CharString_getLength( str ) && ( 'x' == str[1] ))
			{
				value = Math_HexAsLongLong( str );
			}
		}
	}
	return value;
}

long long int
Math_AsLongLong_base( const char* str, unsigned int base )
{
	long long int value;
	char* endptr;

	if ( NULL != str )
	{
		if ( 0 == (value = strtoll( str, &endptr, 10 )) )
		{
			if ( endptr == str )
			{
				//throw new Exception();
			}
		}
	}
	return value;
}
