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
#include <string.h>

unsigned long long
Math_HexAsLongLong( const char* hexStr )
{
	unsigned long long value = 0;
	unsigned int posn = 0;

	if ( '0' == hexStr[posn++] )
	{
		if ( 'x' == hexStr[posn++] )
		{
			//unsigned int mask = 0xFFFFFF;
			unsigned int balance = CharString_getLength( hexStr ) % 2;
			unsigned int sub = 0;
			while ( hexStr[posn] )
			{
				switch ( balance )
				{
				case 0:
					value <<= 8;
					sub = ToHex( hexStr[posn] );
					sub <<= 4;
					break;
				case 1:
					sub |= ToHex( hexStr[posn] );
					value |= sub;
					sub = 0;
				}
				++posn;
				++balance;
				balance %= 2;
			}
		}
	}
	return value;
}

byte ToHex( char hexChar )
{
	switch ( hexChar )
	{
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
	case 'a':
		return 10;
	case 'B':
	case 'b':
		return 11;
	case 'C':
	case 'c':
		return 12;
	case 'D':
	case 'd':
		return 13;
	case 'E':
	case 'e':
		return 14;
	case 'F':
	case 'f':
		return 15;
	default:
		return 0;
	}
}
