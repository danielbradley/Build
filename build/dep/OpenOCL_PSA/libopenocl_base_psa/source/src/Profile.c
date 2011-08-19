/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/Profile.h"

#include <stdio.h>

void
Profile_PrintResult( const char* id, bool result )
{
	if ( result )
	{
		fprintf( stdout, "%s: PASSED\n", id );
	} else {
		fprintf( stdout, "%s: FAILED\n", id );
	}
}

int
Profile_TestFunction( const char* id, bool(*function)() )
{
	int status;
	int mem = CRuntime_GetAllocationCount();
	status = function();
	mem = CRuntime_GetAllocationCount() - mem;
	
	if ( status )
	{
		fprintf( stdout, "<test id=\"%s\" result=\"PASSED\" mem=\"%i\"/>\n", id, mem );
	} else {
		fprintf( stdout, "<test id=\"%s\" result=\"FAILED\" mem=\"%i\"/>\n", id, mem );
	}
	return status;
}

int
Profile_SkipFunction( const char* id, bool(*function)(), const char* reason )
{
	fprintf( stdout, "<test id=\"%s\" result=\"SKIPPED\">%s</test>\n", id, reason );
	return 0;
}
