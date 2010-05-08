/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_COMMAND_INTERNAL_H
#define ISLABS_BUILD_COMMAND_INTERNAL_H

#include "islabs/build.private.h"
#include <stdio.h>

Command* new_Command( const char* command, const char** arguments );
void free_Command( Command* self );

int  Command_run( Command* self );
int  Command_waitFor( Command* self );
int  Command_hasFinished( const Command* self );
int  Command_getResult( const Command* self );	

void Command_setStandardIn( Command* self, const char* location );
void Command_setStandardOut( Command* self, const char* location );
void Command_setStandardError( Command* self, const char* location );

void Command_print( const Command* self, FILE* stream );

#endif
