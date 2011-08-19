#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Sort.h>
#include <openocl/base/psa/Profile.h>

#include <stdio.h>

bool test01_01()
{
	ISort* sort = new_Sort();
	free_Sort( sort );
	return 1;
}

//
//	Tests proper allocation and deallocation of tuples inside sort.
//

bool test02_01()
{
	bool status = 1;
	ISort* sort = new_Sort();

	Sort_insert( sort, "Sunday",  "2" );
	Sort_insert( sort, "Tuesday", "3" );
	Sort_insert( sort, "Monday",  "1" );

	free_Sort( sort );

	return status;
}

//
//	Tests sorting of tuples inside sort.
//

bool test10_01()
{
	bool status = 1;
	ISort* sort = new_Sort();
	IIterator* it;
	const char* value;

	Sort_insert( sort, "Sunday",  "2" );
	Sort_insert( sort, "Tuesday", "3" );
	Sort_insert( sort, "Monday",  "1" );

	it = Sort_iterator_values( sort );
	while ( (value = (const char*) Iterator_next( it )) )
	{
		fprintf( stderr, "value: %s\n", value );
	}
	free_Iterator( it );
	free_Sort( sort );

	return status;
}

int main( int argc, char** argv )
{
	int status = 1;

	status &= Profile_TestFunction( "01:01", test01_01 );
	status &= Profile_TestFunction( "02:01", test02_01 );
	status &= Profile_TestFunction( "10:01", test10_01 );

	return !status;
}
