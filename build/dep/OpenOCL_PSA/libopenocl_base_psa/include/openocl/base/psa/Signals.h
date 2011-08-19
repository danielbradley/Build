/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_SIGNALS_H
#define OPENOCL_BASE_PSA_SIGNALS_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

/*
 *  Lifecycle functions
 */
//void (*Signals_SetSignal( int signum, void (*handler)(int)) )(int);
//void (*Signals_ResetSignal( int signum ) )(int);

//bool Signals_IsSupported();

#ifdef __cplusplus
}
};};};
#endif
#endif

