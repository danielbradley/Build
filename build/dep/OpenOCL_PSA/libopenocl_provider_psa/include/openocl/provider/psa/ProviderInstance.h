/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_PROVIDER_PSA_PROVIDERINSTANCE_H
#define OPENOCL_PROVIDER_PSA_PROVIDERINSTANCE_H

#include "openocl/provider/psa/Provider.h"
//#include <openocl/base/psa/exportlite.h>
//#include <openocl/base/psa.h>

#ifdef __cplusplus
namespace openocl {
	namespace provider {
		namespace psa {
extern "C"
{
#endif

struct _Provider
{
	const char* type;
	const char* info;
	const char* vendor;
	const char* name;
	const char* version;
	void* object;
};

//EXPORT static struct _Provider* GetInstance();
//struct _Provider* GetInstance();

#ifdef __cplusplus
}
};};};
#endif
#endif
