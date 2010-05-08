/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef _ISLABS_BUILD_STACK_H_
#define _ISLABS_BUILD_STACK_H_

#include "islabs/build.h"
#define DEFAULT_STACK_SIZE 128

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

Stack* new_Stack();
void free_Stack( Stack* self );

void Stack_addItem( Stack* self, char* token );
void Stack_copyItem( Stack* self, const char* token );
char* Stack_pop( Stack* self );

void Stack_expand( Stack* self );

const char** Stack_getAsCharStringList( const Stack* self );

#ifdef __cplusplus
};};};
#endif

#endif
