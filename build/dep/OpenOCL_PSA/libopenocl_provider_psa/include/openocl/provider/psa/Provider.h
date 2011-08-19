/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_PROVIDER_PSA_PROVIDER_H
#define OPENOCL_PROVIDER_PSA_PROVIDER_H

#include "openocl/base/psa/exportlite.h"
//#include <openocl/base/psa.h>

#ifdef __cplusplus
namespace openocl {
	namespace provider {
		namespace psa {
extern "C"
{
#endif

typedef struct _Provider Provider;

EXPORT	Provider* Provider_Load( const char* provider );
EXPORT	void	free_Provider( Provider* self );

EXPORT	const char*	Provider_getType( const Provider* self );
EXPORT	const char*	Provider_getInfo( const Provider* self );
EXPORT	const char*	Provider_getVendor( const Provider* self );
EXPORT	const char*	Provider_getName( const Provider* self );
EXPORT	const char*	Provider_getVersion( const Provider* self );
EXPORT	void*      	Provider_getObject( const Provider* self );

EXPORT	void		Provider_print( const Provider* self, void* stream );

#ifdef __cplusplus
}
};};};
#endif
#endif
