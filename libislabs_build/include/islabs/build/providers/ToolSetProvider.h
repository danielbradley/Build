/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef PROVIDERS_TOOLSETPROVIDER_H
#define PROVIDERS_TOOLSETPROVIDER_H

#include "islabs/build/providers/ToolSet.h"
#include <stdlib.h>

typedef struct _ToolSetProvider ToolSetProvider;

struct _ToolSetProvider
{
	ToolSet* (*getToolSet)( const char* compiler );
};

#endif
