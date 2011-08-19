#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/base/psa/Iterator.h>

#include <stdio.h>

static bool test01_01()
{
	int status = 1;
	
	IIterator* it = NULL;
	status = (int)(it = new_Iterator());
	free_Iterator( it );
	
	return status;
}

static bool test10_01()
{
	int status = 1;

	const char* day1 = "Sunday";
	const char* day2 = "Monday";
	const char* day3 = "Tuesday";
	const char* day4 = "Wednesday";
	const char* day5 = "Thursday";
	const char* day6 = "Friday";
	const char* day7 = "Saturday";
	
	IIterator* it = new_Iterator();
	Iterator_addReference( it, day1 );
	Iterator_addReference( it, day2 );
	Iterator_addReference( it, day3 );
	Iterator_addReference( it, day4 );
	Iterator_addReference( it, day5 );
	Iterator_addReference( it, day6 );
	Iterator_addReference( it, day7 );
	free_Iterator( it );
	
	return status;
}

static bool test20_01()
{
	int status = 0;

	const char* day1 = "Sunday";
	const char* day2 = "Monday";
	const char* day3 = "Tuesday";
	const char* day4 = "Wednesday";
	const char* day5 = "Thursday";
	const char* day6 = "Friday";
	const char* day7 = "Saturday";
	
	IIterator* it = new_Iterator();
	Iterator_addReference( it, day1 );
	Iterator_addReference( it, day2 );
	Iterator_addReference( it, day3 );
	Iterator_addReference( it, day4 );
	Iterator_addReference( it, day5 );
	Iterator_addReference( it, day6 );
	Iterator_addReference( it, day7 );
	
	status |= CharString_compare( "Sunday", (const char*) Iterator_next( it ) );
	status |= CharString_compare( "Monday", (const char*) Iterator_next( it ) );
	status |= CharString_compare( "Tuesday", (const char*) Iterator_next( it ) );
	status |= CharString_compare( "Wednesday", (const char*) Iterator_next( it ) );
	status |= CharString_compare( "Thursday", (const char*) Iterator_next( it ) );
	status |= CharString_compare( "Friday", (const char*) Iterator_next( it ) );
	status |= CharString_compare( "Saturday", (const char*) Iterator_next( it ) );
	 	
	free_Iterator( it );
	
	return !status;
}

int main( int argc, char** argv )
{
	Profile_TestFunction( "01:01", test01_01 );
	Profile_TestFunction( "10:01", test10_01 );
	Profile_TestFunction( "20:01", test20_01 );

	return 0;
}
