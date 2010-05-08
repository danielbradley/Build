/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_GLOBAL_H
#define ISLABS_BUILD_GLOBAL_H

#include "islabs/build.h"
#include "islabs/build/Arguments.h"
#include "islabs/build/ProviderRegistry.h"
#include <openocl/base/psa/List.h>
#include <stdio.h>

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

struct _Global {
	char* PLATFORM;
	char* OSTYPE;
	char* HOSTTYPE;

	char* CC;
	char* C_PLUS_PLUS;
	char* JAVAC;
	char* LD;
	char* AR;
	char* JAR;
	char* providerDirectory;

	IList* builtPackages;

	//
	//	The following lists store flags that will be passed to the
	//	various tools. These are also the names of the environment
	//	variables that build will check for additional flags.
	//

    IList* CPPFLAGS;		// C preprocessor flags: -I..., -D..., /I..., /D...
    IList* CFLAGS;			// C compiler flags: -g, /Zi, -c, /c, -O2, -march=i386, -Wall, -W2
	IList* LFLAGS;			// Linker flags: -Wl,-G, /DEBUG, -l, -l..., /DEFAULTLIB:...
    IList* LDFLAGS;			// Linker/Loader paths: -L..., /LIBPATH:...
	IList* LIBFLAGS;		// Flags to create shared libraries: -shared, /DLL 
	IList* JFLAGS;		    // Javac compiler flags: -classpath

	IList* DLLList;
	
	unsigned int verbosity;
	int indentation;
	int copyLibs;
	int recursive;
	int no;
	int buildStatic;
	int debug;
	int debugLib;
	int release;
	int monolithic;
	int withProfiling;
	FILE* logFile;
	
	ProviderRegistry* registry;
};

Global*
Global_new( const Arguments* args );

void
Global_delete( Global* self );

void
Global_increment( const Global* self );

void
Global_decrement( const Global* self );

void
Global_print( const Global* self );

void
Global_tab( const Global* self, FILE* stream );

void
Global_clearline( const Global* self, FILE* stream, unsigned int nr );

#ifdef __cplusplus
};};};
#endif

#endif
