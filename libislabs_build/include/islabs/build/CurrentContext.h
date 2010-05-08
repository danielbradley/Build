/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_CURRENTCONTEXT_H
#define ISLABS_BUILD_CURRENTCONTEXT_H

#include "islabs/build.h"
#include "islabs/build/Global.h"
#include <openocl/base/psa/List.h>

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

struct _CurrentContext
{
	const Global* global;
	char* package_dir;
	char* target_dir;
	char* package_name;
	int   isLib;

	IList* FULL_CPPFLAGS;   // C preprocessor flags: -I..., -D..., /I..., /D...
	IList* FULL_CFLAGS;     // C compiler flags: -g, /Zi, -c, /c, -O2, -march=i386, -Wall, -W2
	IList* FULL_LFLAGS;     // Linker flags: -Wl,-G, /DEBUG
	IList* FULL_LDFLAGS;    // Linker/Loader paths: -L..., /LIBPATH:...
	IList* FULL_LIBFLAGS;   // Flags to create shared libraries: -shared, /DLL
	IList* FULL_LIBS;       // Generated library flags: -l..., /DEFAULTLIB:...
	IList* FULL_RUNLIBS;    // Generated rpath flags(ld.so): -Wl,-rpath,$ORIGIN/../lib

	IList* elf_libraries;
	IList* dll_libraries;
	IList* unix_archives;
	IList* windows_archives;

	const char* language;
	const char* compiler;
	const char* linker;
};

CurrentContext*
CurrentContext_new( const Global* global, const char* package_dir, const char* target_dir );

void
CurrentContext_delete( CurrentContext* this );

void
CurrentContext_print( const CurrentContext* this );

#ifdef __cplusplus
};};};
#endif

#endif
