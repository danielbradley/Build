/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_USER_H
#define OPENOCL_BASE_PSA_USER_H

#include "openocl/base/psa/export.h"
#include <stdarg.h>

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

EXPORT	char* User_ID();
EXPORT	char* User_Name();
EXPORT	char* User_Directory();

#ifdef __cplusplus
}
};};};
#endif
#endif
