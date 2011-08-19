/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_EXPORT_H
#define OPENOCL_BASE_PSA_EXPORT_H

#include "openocl/base/psa/debug.h"

#ifdef _WIN32
	#ifdef __cplusplus
	#define EXPORT extern "C" __declspec(dllexport)
	#else
	#define EXPORT __declspec(dllexport)
	#endif
#else
	#ifdef __cplusplus
		#define EXPORT extern "C"
	#else	
	#define EXPORT
	#endif
#endif

/*
 *	CRuntime.h in this file so that its functions are defined
 *	for all source files that include "openocl/base/psa/debug.h"
 *	via this file.
 *
 *	However, it needs to be included after the definition of
 *	EXPORT as it itself is exported.
 */ 
#include "openocl/base/psa/CRuntime.h"

#endif
