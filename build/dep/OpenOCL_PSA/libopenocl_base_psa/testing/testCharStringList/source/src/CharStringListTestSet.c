#include <openocl/base/psa/CharStringList.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Profile.h>

int test01_01()
{
	int status = 1;
	unsigned int i;
	for( i=0; i < 1000; i++ )
	{
		char** cs_list = new_CharStringList_default();
		int j;
		for ( j=0; j < 10; j++ )
		{
			CharStringList_copyItem( cs_list, "magic" );
		}
		free_CharStringList( cs_list );
	}
	return status;
}

int main( int argc, char** argv )
{
	int status = 1;
	status &= Profile_TestFunction( "01:01", test01_01 );
	return !status;
}
