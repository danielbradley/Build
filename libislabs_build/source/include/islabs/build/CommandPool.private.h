/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef _ISLABS_BUILD_COMMANDPOOL_PRIVATE_H_
#define _ISLABS_BUILD_COMMANDPOOL_PRIVATE_H_

#include "islabs/build.private.h"

CommandPool* new_CommandPool();
void free_CommandPool( CommandPool* self );

void CommandPool_add( CommandPool* self, Command* aCommand );
int  CommandPool_runAll( CommandPool* self );

#endif
