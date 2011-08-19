#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Sequence.h>
#include <openocl/base/psa/Profile.h>

#include <stdio.h>

bool test01_01()
{
	ISequence* seq = new_Sequence();
	free_Sequence( seq );
	return 1;
}

bool test02_01()
{
	bool status = 1;
	ISequence* seq = new_Sequence();

	Sequence_add( seq, "Sunday" );
	Sequence_add( seq, "Tuesday" );
	Sequence_insert( seq, "Monday", 1 );

	{
		int i;
		int count = Sequence_count( seq );
		for ( i=0; i < count; i++ )
		{
			fprintf( stderr, "element: %s\n", (const char*) Sequence_get( seq, i ) );
		}
	}

	free_Sequence( seq );
	return status;
}

int main( int argc, char** argv )
{
	int status = 1;

	status &= Profile_TestFunction( "01:01", test01_01 );
	status &= Profile_TestFunction( "02:01", test02_01 );

	return !status;
}
