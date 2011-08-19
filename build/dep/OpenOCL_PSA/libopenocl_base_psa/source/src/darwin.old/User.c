/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Environment.h"
#include "openocl/base/psa/User.h"

char*
User_ID()
{
	const char* var = Environment_GetEnvironmentValue( "UID" );
	if ( !var ) var = "";
	
	return new_CharString( var );
}

char*
User_Name()
{
	const char* var = Environment_GetEnvironmentValue( "USER" );
	if ( !var ) var = "";

	return new_CharString( var );
}

char*
User_Directory()
{
	const char* var = Environment_GetEnvironmentValue( "HOME" );
	if ( !var ) var = "";

	return new_CharString( var );
}
