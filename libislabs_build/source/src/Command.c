/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/Command.private.h"
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/CharStringList.h>
#include <openocl/base/psa/Process.h>

#include <stdio.h>

Command* new_Command( const char* command, const char** arguments )
{
	Command* self = CRuntime_calloc( 1, sizeof( Command ) );
	self->command = new_CharString( command );
	self->arguments = new_CharStringList( arguments );
	return self;
}

void free_Command( Command* self )
{
	free_CharString( self->command );
	free_CharStringList( self->arguments );
	if ( self->process ) {
		free_Process( self->process );
	}
	if ( self->in ) {
		free_CharString( self->in );
	}	
	if ( self->out ) {
		free_CharString( self->out );
	}
	if ( self->error ) {
		free_CharString( self->error );
	}	
	CRuntime_free( self );
}

int Command_run( Command* self )
{
	int status;
	self->process = new_Process( self->command, (const char**) self->arguments );
	Process_setUsePath( self->process, 1 );
	
	if ( self->in ) {
		Process_setStandardOut( self->process, self->in );
	}	
	if ( self->out ) {
		Process_setStandardOut( self->process, self->out );
	}
	if ( self->error ) {
		Process_setStandardError( self->process, self->error );
	}	
	status = Process_start( self->process );
	return status;
}

int Command_waitFor( Command* self )
{
	if ( Process_waitFor( self->process ) )
	{
		self->finished = 1;
		self->result = Process_hasExitedNormally( self->process );
	}
	return self->finished;
}

int Command_hasFinished( const Command* self )
{
	return self->finished;
}

int Command_getResult( const Command* self )
{
	return self->result;
}

void Command_setStandardIn( Command* self, const char* location )
{
	self->in = CharString_copy( location );
}

void Command_setStandardOut( Command* self, const char* location )
{
	self->out = CharString_copy( location );
}

void Command_setStandardError( Command* self, const char* location )
{
	self->error = CharString_copy( location );
}

void Command_print( const Command* self, FILE* stream )
{
//	char* args = new_CharString_delimiter( (const char**) self->arguments, ' ' );
//	fprintf( stream, "%s\n", args );
//	free_CharString( args );

	const char** list = (const char**) self->arguments;
	unsigned int max = CharStringList_getLength( list );
	unsigned int i;
	fprintf( stream, "%s\n", list[0] );
	for ( i=1; i < max; i++ )
	{
		fprintf( stream, "\t%s\n", list[i] );
	}
}
