/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_THREADS_PSA_MUTEX_PROTECTED_H
#define OPENOCL_THREADS_PSA_MUTEX_PROTECTED_H

#include "openocl/base/psa.h"
#include "openocl/threads/psa/Mutex.h"

void* Mutex_getState( IMutex* self );

#endif
