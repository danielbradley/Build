/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/MathPSA.h"
#include <cstdlib>

using namespace openocl::base;

//extern long        strtol( const char* str, char** endptr, int base );
//extern long long   strtoll( const char* str, char** endptr, int base );
//extern double strtod( const char* str, char** endptr );

double
MathPSA::asDouble( const char* str )
throw (Exception*)
{
	long double value = 0.0;
	char*  endptr;

	if ( null != str )
	{
		if ( 0 == (value = strtod( str, &endptr )) )
		{
			if ( endptr == str )
			{
				throw new Exception();
			}
		}
	}
	return value;
}

long double
MathPSA::asLongDouble( const char* str )
throw (Exception*)
{
	long double value;
	char* endptr;

	if ( null != str )
	{
		if ( 0 == (value = strtod( str, &endptr )) )
		{
			if ( endptr == str )
			{
				throw new Exception();
			}
		}
	}
	return value;
}

long long int
MathPSA::asLongLong( const char* str )
throw (Exception*)
{
	long long int value;
	char* endptr;

	if ( null != str )
	{
		if ( 0 == (value = strtoll( str, &endptr, 10 )) )
		{
			if ( endptr == str )
			{
				throw new Exception();
			}
		}
	}
	return value;
}

long long int
MathPSA::asLongLong( const char* str, unsigned int base )
throw (Exception*)
{
	long long int value;
	char* endptr;

	if ( null != str )
	{
		if ( 0 == (value = strtoll( str, &endptr, 10 )) )
		{
			if ( endptr == str )
			{
				throw new Exception();
			}
		}
	}
	return value;
}
