#include <openocl/base/psa/Profile.h>
#include <openocl/base/psa/Stack.h>

static bool test01_01()
{
	int status = 1;

	IStack* stack = new_Stack();
	free_Stack( stack );

	return status;
}

static bool test02_01()
{
	int status = 1;

	IStack* stack = new_Stack();
	unsigned int max = DEFAULT_STACK_SIZE;
	unsigned int i;
	
	for ( i=0; i < max; i++ )
	{
		Stack_copyItem( stack, "Hello" );
	}
	
	free_Stack( stack );

	return status;
}

static bool test02_02()
{
	int status = 1;

	IStack* stack = new_Stack();
	unsigned int max = DEFAULT_STACK_SIZE + 1;
	unsigned int i;
	
	for ( i=0; i < max; i++ )
	{
		Stack_copyItem( stack, "Hello" );
	}
	
	free_Stack( stack );

	return status;
}

int main( int argc, char** argv )
{
	Profile_TestFunction( "01:01", test01_01 );

	Profile_TestFunction( "02:01", test02_01 );
	Profile_TestFunction( "02:02", test02_02 );

	return 0;
}
