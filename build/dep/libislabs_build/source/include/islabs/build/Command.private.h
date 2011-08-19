/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_COMMAND_PRIVATE_H
#define ISLABS_BUILD_COMMAND_PRIVATE_H

#include "islabs/build/Command.internal.h"
#include <openocl/base/psa/Process.h>

struct _Command {
	char* command;
	char** arguments;
	int finished;
	int result;
	IProcess* process;
	char* in;
	char* out;
	char* error;
};

#endif
