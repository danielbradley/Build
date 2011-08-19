/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#define _ALL_SOURCE 1
#include <windows.h>

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/CharStringList.h"
#include "openocl/base/psa/Process.h"

//#include <unistd.h>		/** fork() **/
#include <process.h>
#include <stdlib.h>
#include <string.h>
//#include <wait.h>		/** waitpid() **/

typedef struct _IProcessIO
{
	const char* in;
	const char* out;
	const char* error;
	
	HANDLE h_in;
	HANDLE h_out;
	HANDLE h_error;
} IProcessIO;

struct _IProcess
{
	char*                executable;
	char**               arguments;
	char*                searchPath;
	int                  usePath;
	PROCESS_INFORMATION* pi;
	IProcessIO           io;
	int                  exitStatus;
	bool                 finished;
	bool                 exitedNormally;
	bool				 useAltStd;
};

IProcess* new_Process( const char* executable, const char** arguments )
{
	IProcess* self = (IProcess*) CRuntime_calloc( 1, sizeof( IProcess ) );
	self->executable = new_CharString( executable );
	self->arguments = new_CharStringList( arguments );

	self->pi = NULL;
	return self;
}


void free_Process( IProcess* self )
{
	free_CharString( self->executable );
	free_CharStringList( self->arguments );

	if ( self->pi ) CRuntime_free( self->pi );
	CRuntime_free( self );
}

int Process_start( IProcess* self )
{
	char* command_line = CharStringList_toCharString( (const char**) self->arguments );
	STARTUPINFO si;
	int status;
	self->pi = CRuntime_calloc( 1, sizeof( PROCESS_INFORMATION ) );
	GetStartupInfo( &si );

	if ( self->useAltStd )
	{
		si.dwFlags = STARTF_USESTDHANDLES;
		if ( self->io.h_in )    si.hStdInput  = self->io.h_in;
		if ( self->io.h_out )   si.hStdOutput = self->io.h_out;
		if ( self->io.h_error ) si.hStdError  = self->io.h_error;
	}

	status = CreateProcess( NULL, command_line, NULL, NULL, FALSE, 0, NULL, NULL, &si, self->pi );

	if ( 0 == status )
	{
		CRuntime_free( self->pi );
		self->pi = NULL;
	}
	CRuntime_free( command_line );
	return status;
}

int Process_waitFor( IProcess* self )
{
	int status = 1;
	DWORD tstat;

	WaitForSingleObject( self->pi->hProcess, INFINITE );
	if ( GetExitCodeProcess( self->pi->hProcess, &tstat ) )
	{
		self->finished = 1;
		self->exitedNormally = 1;
	} else {
		status = 0;
	}
	return status;
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
	self->io.h_in = CreateFile( location, STANDARD_RIGHTS_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );	
	self->useAltStd = 1;
}

void Process_setStandardOut( IProcess* self, const char* location )
{
	self->io.out   = CharString_copy( location );
	self->io.h_out = CreateFile( location, STANDARD_RIGHTS_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );	
	self->useAltStd = 1;
}

void Process_setStandardError( IProcess* self, const char* location )
{
	self->io.error   = CharString_copy( location );
	self->io.h_error = CreateFile( location, STANDARD_RIGHTS_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );	
	self->useAltStd = 1;
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
	return self->pi->dwProcessId;
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

//
// Class methods
//
unsigned int Process_GetCurrentID()
{
	return _getpid();
}

unsigned int Process_GetCurrentParentID()
{
	return 0; //getppid();
}
