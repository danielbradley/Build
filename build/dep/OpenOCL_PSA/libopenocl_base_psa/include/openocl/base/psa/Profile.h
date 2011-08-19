/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_PROFILE_H
#define OPENOCL_BASE_PSA_PROFILE_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

EXPORT	void Profile_PrintResult( const char* id, bool result );
EXPORT	int  Profile_TestFunction( const char* id, bool(*function)() );
EXPORT	int  Profile_SkipFunction( const char* id, bool(*function)(), const char* reason );

#endif
