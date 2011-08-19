/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_CHARSTRINGLIST_H
#define OPENOCL_BASE_PSA_CHARSTRINGLIST_H

#include "openocl/base/psa/export.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#else
#ifndef BOOLEAN
#define BOOLEAN
	typedef int bool;
#endif
#endif

/*
 *  Lifecycle functions
 */
EXPORT char** new_CharStringList_default();
EXPORT char** new_CharStringList( const char** list );
EXPORT void free_CharStringList( char** self );

EXPORT int CharStringList_copyItem( char** self, const char* item );

/*
 *  Allocating const functions
 */
EXPORT char* CharStringList_toCharString( const char** self );

/*
 *  Information functions
 */
EXPORT unsigned int CharStringList_getLength( const char** self );

#ifdef __cplusplus
}
};};};
#endif
#endif
