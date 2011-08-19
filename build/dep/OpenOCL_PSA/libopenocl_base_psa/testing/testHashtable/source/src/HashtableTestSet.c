#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Profile.h>
#include <openocl/base/psa/Hashtable.internal.h>

bool test01_01()
{
	IHashtable* hashtable = new_Hashtable();
	free_Hashtable( hashtable );
	return 1;
}

bool test02_01()
{
	bool status = 1;
	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", "Play" );
	status = status && !Hashtable_put( hashtable, "Monday", "Work" );
	
	free_Hashtable( hashtable );
	return status;
}

bool test03_01()
{
	bool status = 1;

	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", "Play" );
	status = status && !Hashtable_put( hashtable, "Monday", "Work" );

	status = status && ( 0 != Hashtable_get( hashtable, "Sunday" ));
	status = status && ( 0 != Hashtable_get( hashtable, "Sunday" ));
	
	free_Hashtable( hashtable );
	return status;
}

bool test03_02()
{
	bool status = 1;
	const char* sunday = NULL;
	const char* monday = NULL;

	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", "Play" );
	status = status && !Hashtable_put( hashtable, "Monday", "Work" );

	sunday = Hashtable_get( hashtable, "Sunday" );
	monday = Hashtable_get( hashtable, "Monday" );

	status = status && (sunday && monday);
	status = status && !CharString_compare( sunday, "Play" );
	status = status && !CharString_compare( monday, "Work" );

	free_Hashtable( hashtable );

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

	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", "Play" );
	status = status && !Hashtable_put( hashtable, "Monday", "Work" );
	status = status &&  Hashtable_put( hashtable, "Monday", "Work" );

	sunday = Hashtable_get( hashtable, "Sunday" );
	monday = Hashtable_get( hashtable, "Monday" );

	status = status && (sunday && monday);
	status = status && !CharString_compare( sunday, "Play" );
	status = status && !CharString_compare( monday, "Work" );

	free_Hashtable( hashtable );

	return status;
}

bool test04_01()
{
	/*
	 *  Tests Hashtable_containsKey
	 */
	bool status = 1;

	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", "Play" );
	status = status && !Hashtable_put( hashtable, "Monday", "Work" );

	status = status && Hashtable_containsKey( hashtable, "Sunday" );
	status = status && Hashtable_containsKey( hashtable, "Monday" );
	status = status && !Hashtable_containsKey( hashtable, "Tuesday" );
	status = status && !Hashtable_containsKey( hashtable, "Wednesday" );

	free_Hashtable( hashtable );

	return status;
}

bool test05_01()
{
	/*
	 *  Tests Hashtable_containsValue
	 */
	const char* play1   = "Play";
	const char* work1   = "Work";
	const char* play2   = "PlayMate";
	const char* work2   = "WorkMate";
	 	 
	bool status = 1;

	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", play1 );
	status = status && !Hashtable_put( hashtable, "Monday", work1 );

	status = status && Hashtable_containsValue( hashtable, play1 );
	status = status && Hashtable_containsValue( hashtable, work1 );
	status = status && !Hashtable_containsValue( hashtable, play2 );
	status = status && !Hashtable_containsValue( hashtable, work2 );

	status = status && Hashtable_put( hashtable, "Sunday", play2 );
	status = status && Hashtable_put( hashtable, "Monday", work2 );

	status = status && Hashtable_containsValue( hashtable, play2 );
	status = status && Hashtable_containsValue( hashtable, work2 );
	status = status && !Hashtable_containsValue( hashtable, play1 );
	status = status && !Hashtable_containsValue( hashtable, work1 );

	free_Hashtable( hashtable );

	return status;
}

bool test06_01()
{
	/*
	 *  Tests Hashtable_putIfNew
	 */
	const char* play1   = "Play";
	const char* work1   = "Work";
	const char* play2   = "PlayMate";
	const char* work2   = "WorkMate";
	 	 
	bool status = 1;

	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", play1 );
	status = status && !Hashtable_put( hashtable, "Monday", work1 );

	status = status && Hashtable_containsValue( hashtable, play1 );
	status = status && Hashtable_containsValue( hashtable, work1 );
	status = status && !Hashtable_containsValue( hashtable, play2 );
	status = status && !Hashtable_containsValue( hashtable, work2 );

	status = status && Hashtable_putIfNew( hashtable, "Sunday", play2 );
	status = status && Hashtable_putIfNew( hashtable, "Monday", work2 );

	status = status && !Hashtable_containsValue( hashtable, play2 );
	status = status && !Hashtable_containsValue( hashtable, work2 );
	status = status && Hashtable_containsValue( hashtable, play1 );
	status = status && Hashtable_containsValue( hashtable, work1 );

	free_Hashtable( hashtable );

	return status;
}

bool test07_01()
{
	/*
	 *  Tests Hashtable_putIfNew
	 */
	const char* play1   = "Play";
	const char* work1   = "Work";
	const char* play2   = "PlayMate";
	const char* work2   = "WorkMate";
	 	 
	bool status = 1;

	IHashtable* hashtable = new_Hashtable();

	status = status && !Hashtable_put( hashtable, "Sunday", play1 );
	status = status && !Hashtable_put( hashtable, "Monday", work1 );

	status = status && Hashtable_containsValue( hashtable, play1 );
	status = status && Hashtable_containsValue( hashtable, work1 );
	status = status && !Hashtable_containsValue( hashtable, play2 );
	status = status && !Hashtable_containsValue( hashtable, work2 );

	status = status && Hashtable_putIfNew( hashtable, "Sunday", play2 );
	status = status && Hashtable_putIfNew( hashtable, "Monday", work2 );

	status = status && !Hashtable_containsValue( hashtable, play2 );
	status = status && !Hashtable_containsValue( hashtable, work2 );
	status = status && Hashtable_containsValue( hashtable, play1 );
	status = status && Hashtable_containsValue( hashtable, work1 );

	status = status && Hashtable_remove_key( hashtable, "Sunday" );
	status = status && !Hashtable_containsValue( hashtable, play1 );

	status = status && Hashtable_remove_value( hashtable, work1 );
	status = status && !Hashtable_containsValue( hashtable, work1 );
	

	free_Hashtable( hashtable );

	return status;
}



int main( int argc, char** argv )
{
	int status = 1;

	status = status && Profile_TestFunction( "01:01", test01_01 );

	status = status && Profile_TestFunction( "02:01", test02_01 );

	status = status && Profile_TestFunction( "03:01", test03_01 );
	status = status && Profile_TestFunction( "03:02", test03_02 );
	status = status && Profile_TestFunction( "03:03", test03_03 );

	status = status && Profile_TestFunction( "04:01", test04_01 );

	status = status && Profile_TestFunction( "05:01", test05_01 );

	status = status && Profile_TestFunction( "06:01", test06_01 );

	status = status && Profile_TestFunction( "07:01", test07_01 );

	return !status;
}
