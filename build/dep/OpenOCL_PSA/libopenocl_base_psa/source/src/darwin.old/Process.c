/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/CharStringList.h"
#include "openocl/base/psa/Process.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>		/** fork() **/
#include <sys/wait.h>		/** waitpid() **/
#include <sys/stat.h>

typedef struct _IProcessIO
{
	char* in;
	char* out;
	char* error;
} IProcessIO;

struct _IProcess
{
	char*      executable;
	char**     arguments;
	char*      searchPath;
	int        usePath;
	pid_t      pid;
	IProcessIO io;
	int        exitStatus;
	bool       finished;
	bool       exitedNormally;
};

IProcess* new_Process( const char* executable, const char** arguments )
{
	IProcess* self = (IProcess*) CRuntime_calloc( 1, sizeof( IProcess ) );
	self->executable = new_CharString( executable );
	self->arguments = new_CharStringList( arguments );
	return self;
}

void free_Process( IProcess* self )
{
	free_CharString( self->executable );
	free_CharStringList( self->arguments );
	if ( self->searchPath ) {
		free_CharString( self->searchPath );
	}
	if ( self->io.in ) {
		free_CharString( self->io.in );
	}
	if ( self->io.out ) {
		free_CharString( self->io.out );
	}
	if ( self->io.error ) {
		free_CharString( self->io.error );
	}
	CRuntime_free( self );
}

int Process_start( IProcess* self )
{
	int state = 1;
	
	self->pid = fork();
	if ( -1 == self->pid )
	{
		state = 0;
	}
	else if ( 0 == self->pid )
	{
		int in;
		int out;
		int error;
		
		if ( self->io.in )
		{
			in = open( self->io.in, O_RDONLY );
			dup2( in, STDIN_FILENO );
			close( in );
		}
		
		if ( self->io.out )
		{
			if ( -1 == (out = open( self->io.out, O_WRONLY | O_APPEND )) )
			{
				if ( -1 == (out = creat( self->io.out, S_IRWXU )) )
				{
					perror( NULL );
				}
			}
			dup2( out, STDOUT_FILENO );
			close( out );
		}
		
		if ( self->io.error )
		{
			if ( -1 == (error = open( self->io.error, O_WRONLY | O_APPEND )) )
			{
				if ( -1 == (error = creat( self->io.error, S_IRWXU )) )
				{
					perror( NULL );
				}
			}
			dup2( error, STDERR_FILENO );
			close( error );
		}
	
		if ( self->usePath ) {
			execvp( self->executable, (char*const*) self->arguments );
		} else  {
			execv( self->executable, (char*const*) self->arguments );
		}
	}
	return state;
}

int Process_waitFor( IProcess* self )
{
	if ( !self->finished )
	{
		int status;
		int pid = waitpid( self->pid, &status, 0 );
		if ( self->pid == pid )
		{
			self->finished = 1;
			if ( WIFEXITED( status ) )
			{
				if ( 0 == (self->exitStatus = WEXITSTATUS( status )) )
				{
					self->exitedNormally = 1;
				}
			}
		}
	}
	return self->finished;
}

void Process_setSearchPath( IProcess* self, const char* aSearchPath )
{
	if ( self->searchPath ) free_CharString( self->searchPath );
	self->searchPath = new_CharString( aSearchPath );
}

void Process_setUsePath( IProcess* self, int aBoolean )
{
	self->usePath = aBoolean;
}

void Process_setStandardIn( IProcess* self, const char* location )
{
	self->io.in = CharString_copy( location );
}

void Process_setStandardOut( IProcess* self, const char* location )
{
	self->io.out = CharString_copy( location );
}

void Process_setStandardError( IProcess* self, const char* location )
{
	self->io.error = CharString_copy( location );
}

const char* Process_getExecutable( const IProcess* self )
{
	return self->executable;
}

const char** Process_getArguments( const IProcess* self )
{
	return (const char**) self->arguments;
}

unsigned int Process_getProcessID( const IProcess* self )
{
	return self->pid;
}

const char* Process_getSearchPath( const IProcess* self )
{
	return self->searchPath;
}

int	Process_getExitStatus( const IProcess* self )
{
	return self->exitStatus;
}

bool	Process_hasFinished( const IProcess* self )
{
	return self->finished;
}

bool	Process_hasExitedNormally( const IProcess* self )
{
	return self->exitedNormally;
}

/*
 *	Class methods
 */
unsigned int Process_GetCurrentID()
{
	return getpid();
}

unsigned int Process_GetCurrentParentID()
{
	return getppid();
}
