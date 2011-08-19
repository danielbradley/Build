#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/Math.h>
#include <openocl/base/psa/Profile.h>

int test01_01()
{
	int status = 1;
	status &= ( 0 == Math_RoundToInteger_double( 0.400001 ) );
	status &= ( 0 == Math_RoundToInteger_double( 0.450000 ) );
	status &= ( 0 == Math_RoundToInteger_double( 0.490000 ) );
	status &= ( 1 == Math_RoundToInteger_double( 0.500000 ) );
	status &= ( 1 == Math_RoundToInteger_double( 0.500001 ) );
	status &= ( 1 == Math_RoundToInteger_double( 0.500011 ) );
	status &= ( 1 == Math_RoundToInteger_double( 0.600000 ) );

    return status;

}

int test02_01()
{
	int status = 1;

	status &= (15    == Math_HexAsLongLong( "0xf" ));
	status &= (16    == Math_HexAsLongLong( "0x10" ));
	status &= (17    == Math_HexAsLongLong( "0x11" ));
	status &= (273   == Math_HexAsLongLong( "0x111" ));
	status &= (2730  == Math_HexAsLongLong( "0xaaa" ));
	status &= (2730  == Math_HexAsLongLong( "0xAAA" ));
	status &= (3003  == Math_HexAsLongLong( "0xBBB" ));
	status &= (65535 == Math_HexAsLongLong( "0xFFFF" ));

	return status;
}

int test02_02()
{
	int status = 1;

	status &= (15    == Math_AsLongLong( "0xf" ));
	status &= (16    == Math_AsLongLong( "0x10" ));
	status &= (17    == Math_AsLongLong( "0x11" ));
	status &= (273   == Math_AsLongLong( "0x111" ));
	status &= (2730  == Math_AsLongLong( "0xaaa" ));
	status &= (2730  == Math_AsLongLong( "0xAAA" ));
	status &= (3003  == Math_AsLongLong( "0xBBB" ));
	status &= (65535 == Math_AsLongLong( "0xFFFF" ));

	return status;
}

int main( int argc, char** argv )
{
	int status = 1;
	status &= Profile_TestFunction( "01:01", test01_01 );
	status &= Profile_TestFunction( "02:01", test02_01 );
	status &= Profile_TestFunction( "02:02", test02_02 );
	return !status;
}
