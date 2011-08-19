#include <openocl/base/psa/Profile.h>
#include <openocl/io/psa/Link.h>

#include <stdio.h>

int test01_01()
{
	ILink* link = new_Link();
	free_Link( link );
	return 1;
}

int test02_01()
{
	ILink* link = new_Link();
	Link_load( link, "testdata/somewhere.link" );
	free_Link( link );
	return 1;
}

int test03_01()
{
	int result = 1;
	ILink* link = new_Link();
	Link_load( link, "testdata/dynamic.link" );
	if ( !Link_getPath( link ) )
	{
		result = 0;
	}

	free_Link( link );
	return result;
}

int test04_01()
{
	int result = 1;
	ILink* link = new_Link();
	Link_setAllowDynamic( link, 1 );
	Link_load( link, "testdata/dynamic.link" );
	if ( Link_getPath( link ) )
	{
		fprintf( stderr, "%s", Path_getCommon( Link_getPath( link ) ) );
	} else {
		result = 0;
	}

	free_Link( link );
	return result;
}

int test04_02()
{
	int result = 1;
	ILink* link = new_Link();
	Link_setAllowDynamic( link, 1 );
	Link_load( link, "testdata/dynamic.link" );
	if ( Link_getPath( link ) )
	{
		fprintf( stderr, "%s", Path_getCommon( Link_getPath( link ) ) );
	} else {
		result = 0;
	}

	free_Link( link );
	return result;
}

int test04_03()
{
	int result = 1;
	ILink* link = new_Link();
	Link_setAllowDynamic( link, 1 );
	Link_load( link, "testdata/dynamic2.link" );
	if ( Link_getPath( link ) )
	{
		result = 0;
		fprintf( stderr, "%s", Path_getCommon( Link_getPath( link ) ) );
	}
	
	free_Link( link );
	return result;
}

int main( int argc, char** argv )
{
	int status = 1;

	status &= Profile_TestFunction( "test01_01", test01_01 );
	status &= Profile_TestFunction( "test02_01", test02_01 );
	status &= Profile_TestFunction( "test03_01", test03_01 );
	status &= Profile_TestFunction( "test04_01", test04_01 );
	status &= Profile_TestFunction( "test04_02", test04_02 );
	status &= Profile_TestFunction( "test04_03", test04_03 );

	return !status;
}
