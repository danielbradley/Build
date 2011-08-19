/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

/*
 *	This is just the export parts of openocl/base/psa/export.h
 *  This should only be included by the provider library.
 */

#ifndef OPENOCL_BASE_PSA_EXPORTLITE_H
#define OPENOCL_BASE_PSA_EXPORTLITE_H

#ifndef EXPORT
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
#endif

#endif
