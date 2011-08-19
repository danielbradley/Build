/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/CommandPool.private.h"
#include "islabs/build/Command.private.h"
#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/Set.h>

struct _CommandPool {
	ISet* commands;
};

CommandPool* new_CommandPool()
{
	CommandPool* self = CRuntime_calloc( 1, sizeof( Command ) );
	self->commands = new_Set();
	return self;
}

void free_CommandPool( CommandPool* self )
{
	unsigned int max = Set_count( self->commands );
	unsigned int i;
	for ( i=0; i < max; i++ )
	{
		Command* command = (Command*) Set_get( self->commands, i );
		free_Command( command );
	}
	free_Set( self->commands );
	CRuntime_free( self );
}

void CommandPool_add( CommandPool* self, Command* aCommand )
{
	Set_add( self->commands, aCommand );
}

int CommandPool_runAll( CommandPool* self )
{
	int result = 1;
	unsigned int max = Set_count( self->commands );
	unsigned int i;
	for ( i=0; i < max; i++ )
	{
		Command* command = (Command*) Set_get( self->commands, i );
		Command_run( command );
	}
	
	for ( i=0; i < max; i++ )
	{
		Command* command = (Command*) Set_get( self->commands, i );
		if ( !Command_hasFinished( command ) )
		{
			Command_waitFor( command );
			result &= Command_getResult( command );
		}
	}
	return result;
}
		
		
		


