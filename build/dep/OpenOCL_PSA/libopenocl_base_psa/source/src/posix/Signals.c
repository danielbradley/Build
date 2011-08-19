/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/Signals.h"
#include <signal.h>

void (*Signals_SetSignal( int signum, void (*handler)(int)))(int)
{
	return signal( signum, handler );
}

void (*Signals_ResetSignal( int signum ))(int)
{
	return signal( signum, SIG_DFL );
}
