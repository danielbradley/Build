#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Profile.h>

#include <stdio.h>
//
//	Test copy
//

static bool test01_01()
{
	int status = 1;
	unsigned int i;
	for ( i=0; i < 100; i++ )
	{
		char* happy = CharString_copy( "joy joy" );
		CRuntime_free( happy );
	}
	return status;
}

//
//	Test matches
//

static bool test02_01_01()
{
	int status = 1;
	
	char* filename = new_CharString( "libopenocl_base-1.0" );
	status &= CharString_matches( filename, "libopenocl_base-1.0" );
	free_CharString( filename );
	
	return status;
}

static bool test02_01_02()
{
	int status = 1;
	
	char* filename = new_CharString( "0" );
	status &= CharString_matches( filename, "0" );
	free_CharString( filename );
	
	return status;
}

static bool test02_01_03()
{
	int status = 1;
	
	char* filename = new_CharString( "" );
	status &= CharString_matches( filename, "" );
	free_CharString( filename );
	
	return status;
}


static bool test02_01()
{
	int status = 1;
	
	char* filename = new_CharString( "libopenocl_base-1.0" );
	status &= CharString_matches( filename, "libopenocl_base-?.0" );
	free_CharString( filename );
	
	return status;
}

static bool test02_02()
{
	int status = 1;
	
	char* filename = new_CharString( "libopenocl_base-1.0" );
	status &= CharString_matches( filename, "*openocl_base-?.0" );
	free_CharString( filename );
	
	return status;
}

static bool test02_03()
{
	int status = 1;
	
	char* filename = new_CharString( "StringBuffer.cpp" );
	status &= CharString_matches( filename, "*.cpp" );
	status &= !CharString_matches( filename, "*.c*" );
	free_CharString( filename );
	
	return status;
}

static bool test02_04()
{
	int status = 1;
	
	char* filename = new_CharString( "CVS" );
	status &= !CharString_matches( filename, "*.c" );
	free_CharString( filename );
	
	return status;
}

static bool test02_05()
{
	int status = 1;
	
	char* filename = new_CharString( "libopenocl_base-1.0" );
	status &= CharString_matches( filename, "*openocl_base-?.0" );
	free_CharString( filename );
	
	return status;
}

static bool test02_06()
{
	int status = 1;
	
	char* filename = new_CharString( "Bag.obj" );
	status &= CharString_matches( filename, "*.obj" );
	free_CharString( filename );
	
	return status;
}

static bool test02_07()
{
	int status = 1;
	
	char* filename = new_CharString( "Bag.c.obj" );
	status &= CharString_matches( filename, "*.obj" );
	free_CharString( filename );
	
	return status;
}

static bool test02_08()
{
	int status = 1;
	
	char* filename = new_CharString( "Bag.obj" );
	status &= CharString_matches( filename, "*.obj*" );
	free_CharString( filename );
	
	return status;
}


//	Extra character on end.
static bool test02_11()
{
	int status = 1;
	
	char* filename = new_CharString( "libopenocl_base-1.0" );
	status &= !CharString_matches( filename, "libopenocl_base-?.01" );
	free_CharString( filename );
	
	return status;
}

//	Extra ? wildcard
static bool test02_12()
{
	int status = 1;
	
	char* filename = new_CharString( "libopenocl_base-1.0" );
	status &= !CharString_matches( filename, "*openocl_base-??.0" );
	free_CharString( filename );
	
	return status;
}

//	Improper extension
static bool test02_13()
{
	int status = 1;
	
	char* filename = new_CharString( "StringBuffer.cpp" );
	status &= !CharString_matches( filename, "*.xcpp" );
	status &= !CharString_matches( filename, "*.xc*" );
	free_CharString( filename );
	
	return status;
}

static bool test02_14()
{
	int status = 1;
	
	char* filename = new_CharString( "CVS" );
	status &= !CharString_matches( filename, "*.c" );
	free_CharString( filename );
	
	return status;
}

//
//	Test compare
//

static bool test03_01()
{
	int status = 1;
	
	const char* tokens[5] = { "local", "checkouts", "openocl-1.0", "libopenocl_base_psa-1.0", NULL };
	
	char* filename = new_CharString_delimiter( tokens, '/' );
	status &= !CharString_compare( filename, "local/checkouts/openocl-1.0/libopenocl_base_psa-1.0" );
	free_CharString( filename );
	
	return status;
}

//
//	Test removeWhitespace
//

static bool test04_01()
{
	int status = 0;
	
	const char* string = "      /local/checkouts/openocl-1.0/libopenocl_base_psa-1.0";
	const char* expected = "/local/checkouts/openocl-1.0/libopenocl_base_psa-1.0";

	char* no_whitespace = CharString_removeWhitespace( string );
	
	if ( 0 == CharString_compare( no_whitespace, expected ) )
	{
		status = 1;
	}
	free_CharString( no_whitespace );
	
	return status;
}

//
//	Test new_CharString_format_args
//

static bool test05_01()
{
	int status = 0;
	
	char* string = new_CharString_format_args( "%i", 1 );
	status = 1;
	free_CharString( string );
	
	return status;
}

static char* using_valist( const char* format, ... )
{
	char* tmp;

	va_list ap;
	va_start( ap, format );
	
	tmp = new_CharString_format_valist( format, ap );
	
	va_end( ap );
	
	return tmp;
}

static bool test05_02()
{
	int status = 0;
	
	char* string = using_valist( "%i %i %i %i", 1, 3, 4, 4 );
	status = 1;
	free_CharString( string );
	
	return status;
}

//
//	Test substring
//

static bool test06_01()
{
	int status = 1;
	
	const char* source = "The";
	char* one = CharString_substring( source, 1, 0 );
	if ( CharString_getLength( one ) != 2 ) status = 0;
	free_CharString( one );
	
	return status;
}

static bool test06_02()
{
	int status = 1;
	
	const char* source = "The";
	char* one = CharString_substring( source, 1, 10 );
	if ( CharString_getLength( one ) != 2 ) status = 0;
	free_CharString( one );
	
	return status;
}

static bool test06_03()
{
	int status = 1;
	
	const char* source = "The";
	char* one = CharString_substring( source, 1, -10 );
	if ( CharString_getLength( one ) != 2 ) status = 0;
	free_CharString( one );
	
	return status;
}


int main( int argc, char** argv )
{
	Profile_TestFunction( "01:01", test01_01 );
	
	Profile_TestFunction( "02:01:01", test02_01_01 );
	Profile_TestFunction( "02:01:02", test02_01_02 );
	Profile_TestFunction( "02:01:03", test02_01_03 );
	Profile_TestFunction( "02:01", test02_01 );
	Profile_TestFunction( "02:02", test02_02 );
	Profile_TestFunction( "02:03", test02_03 );
	Profile_TestFunction( "02:04", test02_04 );
	Profile_TestFunction( "02:05", test02_05 );
	Profile_TestFunction( "02:06", test02_06 );
	Profile_TestFunction( "02:07", test02_07 );
	Profile_TestFunction( "02:08", test02_08 );
	Profile_TestFunction( "02:11", test02_11 );
	Profile_TestFunction( "02:12", test02_12 );
	Profile_TestFunction( "02:13", test02_13 );
	Profile_TestFunction( "02:14", test02_14 );

	Profile_TestFunction( "03:01", test03_01 );
	Profile_TestFunction( "04:01", test04_01 );

	Profile_TestFunction( "05:01", test05_01 );
	Profile_TestFunction( "05:02", test05_02 );

	Profile_TestFunction( "06:01", test06_01 );
	Profile_TestFunction( "06:02", test06_02 );
	Profile_TestFunction( "06:03", test06_03 );

	return 0;
}
