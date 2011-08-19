#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Index.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/base/psa/Stack.h>

#include <stdio.h>

bool test01_01()
{
	IIndex* index = new_Index();
	free_Index( index );
	return 1;
}

bool test02_01()
{
	bool status = 1;
	IIndex* index = new_Index();

	status &= !Index_add( index, "Sunday", "Play" );
	status &= !Index_add( index, "Monday", "Work" );
	
	free_Index( index );
	return status;
}

bool test03_01()
{
	bool status = 1;

	IIndex* index = new_Index();

	status &= !Index_add( index, "Sunday", "Play" );
	status &= !Index_add( index, "Monday", "Work" );

	status &= ( 0 != Index_get( index, "Sunday" ));
	status &= ( 0 != Index_get( index, "Sunday" ));
	
	free_Index( index );
	return status;
}

bool test03_02()
{
	bool status = 1;
	const char* sunday = NULL;
	const char* monday = NULL;

	IIndex* index = new_Index();

	status &= !Index_add( index, "Sunday", "Play" );
	status &= !Index_add( index, "Monday", "Work" );

	sunday = Index_get( index, "Sunday" );
	monday = Index_get( index, "Monday" );

	status &= (sunday && monday);
	status &= !CharString_compare( sunday, "Play" );
	status &= !CharString_compare( monday, "Work" );

	free_Index( index );

	return status;
}

bool test03_03()
{
	/*
	 *	This test checks adding two with the same key.
	 */
	bool status = 1;
	const char* sunday = NULL;
	const char* monday = NULL;

	IIndex* index = new_Index();

	status = status && (bool) !Index_add( index, "Sunday", "Play" );
	status = status && (bool) !Index_add( index, "Monday", "Work" );
	status = status && (bool) Index_add( index, "Monday", "Work" );

	sunday = Index_get( index, "Sunday" );
	monday = Index_get( index, "Monday" );

	status &= (sunday && monday);
	status &= !CharString_compare( sunday, "Play" );
	status &= !CharString_compare( monday, "Work" );

	free_Index( index );

	return status;
}

bool test04_01()
{
	/*
	 *  Tests Index_containsKey
	 */
	bool status = 1;

	IIndex* index = new_Index();

	status &= !Index_add( index, "Sunday", "Play" );
	status &= !Index_add( index, "Monday", "Work" );

	status &= Index_containsKey( index, "Sunday" );
	status &= Index_containsKey( index, "Monday" );
	status &= !Index_containsKey( index, "Tuesday" );
	status &= !Index_containsKey( index, "Wednesday" );

	free_Index( index );

	return status;
}

bool test05_01()
{
	/*
	 *  Tests Index_containsValue
	 */
	const char* play1   = "Play";
	const char* work1   = "Work";
	const char* play2   = "PlayMate";
	const char* work2   = "WorkMate";
	 	 
	bool status = 1;

	IIndex* index = new_Index();

	status = status && !Index_add( index, "Sunday", play1 );
	status = status && !Index_add( index, "Monday", work1 );

	status = status && Index_containsValue( index, play1 );
	status = status && Index_containsValue( index, work1 );
	status = status && !Index_containsValue( index, play2 );
	status = status && !Index_containsValue( index, work2 );

	status = status && Index_add( index, "Sunday", play2 );
	status = status && Index_add( index, "Monday", work2 );

	status = status && Index_containsValue( index, play2 );
	status = status && Index_containsValue( index, work2 );
	status = status && !Index_containsValue( index, play1 );
	status = status && !Index_containsValue( index, work1 );

	free_Index( index );

	return status;
}

bool test06_01()
{
	/*
	 *  Tests Index_addIfNew
	 */
	const char* play1   = "Play";
	const char* work1   = "Work";
	const char* play2   = "PlayMate";
	const char* work2   = "WorkMate";
	 	 
	bool status = 1;

	IIndex* index = new_Index();

	status &= !Index_add( index, "Sunday", play1 );
	status &= !Index_add( index, "Monday", work1 );

	status &= Index_containsValue( index, play1 );
	status &= Index_containsValue( index, work1 );
	status &= !Index_containsValue( index, play2 );
	status &= !Index_containsValue( index, work2 );

	status &= Index_addIfNew( index, "Sunday", play2 );
	status &= Index_addIfNew( index, "Monday", work2 );

	status &= !Index_containsValue( index, play2 );
	status &= !Index_containsValue( index, work2 );
	status &= Index_containsValue( index, play1 );
	status &= Index_containsValue( index, work1 );

	free_Index( index );

	return status;
}

bool test07_01()
{
	/*
	 *  Tests Index_freeObjects
	 */
	IStack* stack1 = new_Stack(); 
	IStack* stack2 = new_Stack(); 
	 
	bool status = 1;

	IIndex* index = new_Index();

	status &= !Index_add( index, "Sunday", stack1 );
	status &= !Index_add( index, "Monday", stack2 );

	status &= Index_containsValue( index, stack1 );
	status &= Index_containsValue( index, stack2 );

	Index_freeObjects( index, (void(*)(void*)) free_Stack );
	free_Index( index );

	return status;
}

/*
 *	Test series 10 tests the iterator returning functions.
 *
 *	Test 01 tests for appropriate return of keys.
 *	Test 02 tests for appropriate return of values.
 *	Test 03 tests for alphabetical return of keys.
 *	Test 04 tests for alphabetical return of values.
 */

static bool test10_01()
{
	bool result = 1;

	const char* key1 = "Sunday";
	const char* key2 = "Monday";
	const char* key3 = "Tuesday";
	const char* key4 = "Wednesday";
	const char* key5 = "Thursday";
	const char* key6 = "Friday";
	const char* key7 = "Saturday";
	const char* key;

	IIterator* it;
	IIndex* index = new_Index();
	Index_add( index, key1, "Relax" );
	Index_add( index, key2, "Work" );
	Index_add( index, key3, "Golf" );
	Index_add( index, key4, "Friends" );
	Index_add( index, key5, "House" );
	Index_add( index, key6, "Washing" );
	Index_add( index, key7, "Car" );
	
	it = Index_iterator_keys( index );
	while ( (key = Iterator_next( it )) )
	{
		fprintf( stderr, "keys: %s\n", key );
	}
	free_Iterator( it );
	
	free_Index( index );
	
	return result;
}

static bool test10_02()
{
	bool result = 1;

	const char* key1 = "Sunday";
	const char* key2 = "Monday";
	const char* key3 = "Tuesday";
	const char* key4 = "Wednesday";
	const char* key5 = "Thursday";
	const char* key6 = "Friday";
	const char* key7 = "Saturday";
	const char* key;

	IIterator* it;
	IIndex* index = new_Index();
	Index_add( index, key1, "Relax" );
	Index_add( index, key2, "Work" );
	Index_add( index, key3, "Golf" );
	Index_add( index, key4, "Friends" );
	Index_add( index, key5, "House" );
	Index_add( index, key6, "Washing" );
	Index_add( index, key7, "Car" );
	
	it = Index_iterator_values( index );
	while ( (key = Iterator_next( it )) )
	{
		fprintf( stderr, "values: %s\n", key );
	}
	free_Iterator( it );
	
	free_Index( index );
	
	return result;
}

static bool test10_03()
{
	bool result = 1;

	const char* key1 = "Sunday";
	const char* key2 = "Monday";
	const char* key3 = "Tuesday";
	const char* key4 = "Wednesday";
	const char* key5 = "Thursday";
	const char* key6 = "Friday";
	const char* key7 = "Saturday";
	const char* key;

	IIterator* it;
	IIndex* index = new_Index();
	Index_add( index, key1, "Relax" );
	Index_add( index, key2, "Work" );
	Index_add( index, key3, "Golf" );
	Index_add( index, key4, "Friends" );
	Index_add( index, key5, "House" );
	Index_add( index, key6, "Washing" );
	Index_add( index, key7, "Car" );
	
	it = Index_iterator_alphabetical_keys( index );
	while ( (key = Iterator_next( it )) )
	{
		fprintf( stderr, "keys: %s\n", key );
	}
	free_Iterator( it );
	
	free_Index( index );
	
	return result;
}

static bool test10_04()
{
	bool result = 1;

	const char* key1 = "Sunday";
	const char* key2 = "Monday";
	const char* key3 = "Tuesday";
	const char* key4 = "Wednesday";
	const char* key5 = "Thursday";
	const char* key6 = "Friday";
	const char* key7 = "Saturday";
	const char* key;

	IIterator* it;
	IIndex* index = new_Index();
	Index_add( index, key1, "Relax" );
	Index_add( index, key2, "Work" );
	Index_add( index, key3, "Golf" );
	Index_add( index, key4, "Friends" );
	Index_add( index, key5, "House" );
	Index_add( index, key6, "Washing" );
	Index_add( index, key7, "Car" );
	
	it = Index_iterator_alphabetical_values( index );
	while ( (key = Iterator_next( it )) )
	{
		fprintf( stderr, "values: %s\n", key );
	}
	free_Iterator( it );
	
	free_Index( index );
	
	return result;
}

//
//	Series 20 tests Index_remove_key
//

static bool test20_01()
{
	bool result = 1;

	const char* key1 = "Sunday";
	const char* key2 = "Monday";
	const char* key3 = "Tuesday";
	const char* key4 = "Wednesday";
	const char* key5 = "Thursday";
	const char* key6 = "Friday";
	const char* key7 = "Saturday";
	const char* key;

	const char* value1 = "Relax";
	const char* value2 = "Work";
	const char* value3 = "Golf";
	const char* value4 = "Friends";
	const char* value5 = "House";
	const char* value6 = "Washing";
	const char* value7 = "Car";

	IIterator* it;
	IIndex* index = new_Index();
	Index_add( index, key1, value1 );
	Index_add( index, key2, value2 );
	Index_add( index, key3, value3 );
	Index_add( index, key4, value4 );
	Index_add( index, key5, value5 );
	Index_add( index, key6, value6 );
	Index_add( index, key7, value7 );

	Index_remove_key( index, key1 );
	Index_remove_key( index, key2 );
	Index_remove_key( index, key3 );
	Index_remove_key( index, key4 );
	Index_remove_key( index, key5 );
	Index_remove_key( index, key6 );
	Index_remove_key( index, key7 );
	
	it = Index_iterator_alphabetical_values( index );
	while ( (key = Iterator_next( it )) )
	{
		fprintf( stderr, "values: %s\n", key );
	}
	free_Iterator( it );
	
	free_Index( index );
	
	return result;
}

//
//	Series 30 tests Index_remove_value
//

static bool test30_01()
{
	bool result = 1;

	const char* key1 = "Sunday";
	const char* key2 = "Monday";
	const char* key3 = "Tuesday";
	const char* key4 = "Wednesday";
	const char* key5 = "Thursday";
	const char* key6 = "Friday";
	const char* key7 = "Saturday";
	const char* key;

	const char* value1 = "Relax";
	const char* value2 = "Work";
	const char* value3 = "Golf";
	const char* value4 = "Friends";
	const char* value5 = "House";
	const char* value6 = "Washing";
	const char* value7 = "Car";

	IIterator* it;
	IIndex* index = new_Index();
	Index_add( index, key1, value1 );
	Index_add( index, key2, value2 );
	Index_add( index, key3, value3 );
	Index_add( index, key4, value4 );
	Index_add( index, key5, value5 );
	Index_add( index, key6, value6 );
	Index_add( index, key7, value7 );

	Index_remove_value( index, value1 );
	Index_remove_value( index, value2 );
	Index_remove_value( index, value3 );
	Index_remove_value( index, value4 );
	Index_remove_value( index, value5 );
	Index_remove_value( index, value6 );
	Index_remove_value( index, value7 );
	
	it = Index_iterator_alphabetical_values( index );
	while ( (key = Iterator_next( it )) )
	{
		fprintf( stderr, "values: %s\n", key );
	}
	free_Iterator( it );
	
	free_Index( index );
	
	return result;
}

int main( int argc, char** argv )
{
	int status = 1;

	status &= Profile_TestFunction( "01:01", test01_01 );

	status &= Profile_TestFunction( "02:01", test02_01 );

	status &= Profile_TestFunction( "03:01", test03_01 );
	status &= Profile_TestFunction( "03:02", test03_02 );
	status &= Profile_TestFunction( "03:03", test03_03 );

	status &= Profile_TestFunction( "04:01", test04_01 );

	status &= Profile_TestFunction( "05:01", test05_01 );

	status &= Profile_TestFunction( "06:01", test06_01 );

	status &= Profile_TestFunction( "07:01", test07_01 );

	status &= Profile_TestFunction( "10:01", test10_01 );
	status &= Profile_TestFunction( "10:02", test10_02 );
	status &= Profile_TestFunction( "10:03", test10_03 );
	status &= Profile_TestFunction( "10:04", test10_04 );

	status &= Profile_TestFunction( "20:01", test20_01 );

	status &= Profile_TestFunction( "30:01", test30_01 );

	return !status;
}
