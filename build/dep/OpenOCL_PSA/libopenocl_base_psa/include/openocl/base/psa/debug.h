/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_DEBUG_H
#define OPENOCL_BASE_PSA_DEBUG_H

/*
 *	Do not include this file!
 *
 *	This file should be included by "openocl/base/psa/export.h" in
 *	the PSA libraries, and "openocl/types.h" in the Open OCL C++
 *	libraries.
 *
 *	Thus PSA objects get this via "openocl/base/psa/export.h" and
 *	OpenOCL objects get this via "openocl/base/Object.h" via
 *	"openocl/types.h"
 *
 *	Do NOT include this file!
 */

//#ifdef OPENOCL_DEBUG
//#ifndef _WIN32
//#define OPENOCL_USE_DEBUG_NEW_AND_DELETE
//#endif
//#endif

//#ifndef OPENOCL_DEBUG
//#define CRuntime_malloc		malloc
//#define CRuntime_calloc		calloc
//#define CRuntime_realloc	realloc
//#define CRuntime_free		free
//#define CRuntime_GetAllocationCount() 0
//#define OPENOCL_BASE_PSA_DIRECT_MEMORY_ALLOCATION
//#endif

#endif
