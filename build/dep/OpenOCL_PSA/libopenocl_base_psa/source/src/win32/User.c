/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#include <lmcons.h>
#include <direct.h>
#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Environment.h"
#include "openocl/base/psa/User.h"

char*
User_ID()
{
	return new_CharString( "0" );
}

char*
User_Name()
{
	char* name = NULL;
	DWORD buffCharCount = UNLEN + 1;
	TCHAR infoBuf[UNLEN+1];

	if ( GetUserName( infoBuf, &buffCharCount ) )
	{
		name = new_CharString( infoBuf );
	}
	return name;
}

char*
User_Directory()
{
	return new_CharString( Environment_GetEnvironmentValue( "HOMEPATH" ) );
}
