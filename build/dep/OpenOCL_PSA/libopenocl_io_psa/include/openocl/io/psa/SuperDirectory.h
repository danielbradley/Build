/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_IO_PSA_SUPERDIRECTORY_H
#define OPENOCL_IO_PSA_SUPERDIRECTORY_H

#include "openocl/io/psa/Directory.h"
#include "openocl/io/psa/File.h"
#include "openocl/io/psa/Path.h"
#include <openocl/base/psa.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Set.h>
#include <openocl/base/psa/Time.h>

#ifdef __cplusplus
	using namespace openocl::base::psa;

	namespace openocl {
		namespace io {
			namespace psa {
	extern "C" {
#endif

typedef struct _SuperDirectory ISuperDirectory;

//	lifecycle functions
EXPORT	ISuperDirectory*  new_SuperDirectory();
EXPORT	void              free_SuperDirectory( ISuperDirectory* self );

//	functions
EXPORT	bool         SuperDirectory_refresh( ISuperDirectory* self );

//	allocating constant functions
EXPORT	IDirectory*  SuperDirectory_subdirectory( const ISuperDirectory* self, const char* name );

//	constant functions
EXPORT	const IList* SuperDirectory_getVolumes( const ISuperDirectory* self );
EXPORT	bool         SuperDirectory_containsVolume( const ISuperDirectory* self, const char* dirname );

#ifdef __cplusplus
}
};};};
#endif

#endif
