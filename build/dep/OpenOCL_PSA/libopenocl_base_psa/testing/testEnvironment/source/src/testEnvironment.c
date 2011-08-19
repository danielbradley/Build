#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Environment.h>
#include <openocl/base/psa/Profile.h>

#include <stdio.h>

static bool test01_01()
{
	bool status = 1;

	IEnvironment* env = new_Environment( "bin/testEnvironment" );
	free_Environment( env );

	return status;
}

static bool test05_01()
{
	bool status = 0;
	
	IEnvironment* env = new_Environment( "bin/testEnvironment" );
	char* dir = Environment_SearchPathFor( Environment_GetEnvironmentValue( "PATH" ), "ls" );
	if ( dir )
	{
		status = 1;
		free_CharString( dir );
	}
	free_Environment( env );
	
	return status;
}

static bool test10_01()
{
	bool status = 1;

	IEnvironment* env = new_Environment( "bin/testEnvironment" );
	
	//fprintf( stderr, "exec:    %s\n", Environment_getExec( env ) );
	//fprintf( stderr, "execDir: %s\n", Environment_getExecDir( env ) );
	//fprintf( stderr, "path:    %s\n", Environment_getPath( env ) );
	//fprintf( stderr, "origin:  %s\n", Environment_getOrigin( env ) );
	
	free_Environment( env );

	return status;
}

static bool test10_02()
{
	bool status = 1;

	IEnvironment* env = new_Environment( "cat" );
	
	//fprintf( stderr, "exec:    %s\n", Environment_getExec( env ) );
	//fprintf( stderr, "execDir: %s\n", Environment_getExecDir( env ) );
	//fprintf( stderr, "path:    %s\n", Environment_getPath( env ) );
	//fprintf( stderr, "origin:  %s\n", Environment_getOrigin( env ) );
	
	free_Environment( env );

	return status;
}

static bool test10_03()
{
	bool status = 1;

	IEnvironment* env = new_Environment( "./testEnvironment" );
	
	//fprintf( stderr, "exec:    %s\n", Environment_getExec( env ) );
	//fprintf( stderr, "execDir: %s\n", Environment_getExecDir( env ) );
	//fprintf( stderr, "path:    %s\n", Environment_getPath( env ) );
	//fprintf( stderr, "origin:  %s\n", Environment_getOrigin( env ) );
	
	free_Environment( env );

	return status;
}

static bool test10_04()
{
	bool status = 1;

	IEnvironment* env = new_Environment( "build" );
	
	//fprintf( stderr, "exec:    %s\n", Environment_getExec( env ) );
	//fprintf( stderr, "execDir: %s\n", Environment_getExecDir( env ) );
	//fprintf( stderr, "path:    %s\n", Environment_getPath( env ) );
	//fprintf( stderr, "origin:  %s\n", Environment_getOrigin( env ) );
	
	free_Environment( env );

	return status;
}


int main( int argc, char** argv )
{
	Profile_TestFunction( "01:01", test01_01 );
	Profile_TestFunction( "05:01", test05_01 );
	Profile_TestFunction( "10:01", test10_01 );
	Profile_TestFunction( "10:02", test10_02 );
	Profile_TestFunction( "10:03", test10_03 );
	Profile_TestFunction( "10:04", test10_04 );

	return 0;
}
