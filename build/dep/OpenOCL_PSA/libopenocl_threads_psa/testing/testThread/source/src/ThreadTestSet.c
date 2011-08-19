#include <openocl/base/psa/Profile.h>
#include <openocl/threads/psa/ConditionVariable.h>
#include <openocl/threads/psa/Mutex.h>
#include <openocl/threads/psa/Semaphore.h>
#include <openocl/threads/psa/Thread.h>
#include <stdio.h>

int threads = 0;
IConditionVariable* cv;
IMutex* p_mutex;
IMutex* cv_mutex;
IMutex* mutex;
ISemaphore* semaphore;

void* locking_printer( void* obj )
{
	const char* str = (const char*) obj;
	unsigned int i;

	for ( i=0; i < 1000; i++ )
	{
		Mutex_lock( p_mutex );
		fprintf( stderr, "%s|", str );
		fprintf( stderr, ":%i|", i );
		Mutex_unlock( p_mutex );
	}
	return NULL;
}

void* locking_printer_cv( void* obj )
{
	const char* str = (const char*) obj;
	unsigned int i;

	for ( i=0; i < 1000; i++ )
	{
		Mutex_lock( p_mutex );
		fprintf( stderr, "%s|", str );
		fprintf( stderr, ":%i|", i );
		Mutex_unlock( p_mutex );
	}

	Mutex_lock( cv_mutex );
	{
		threads--;
		if ( 0 == threads ) ConditionVariable_signal( cv );
	}
	Mutex_unlock( cv_mutex );

	return NULL;
}

void* locking_printer_sem( void* obj )
{
	const char* str = (const char*) obj;
	unsigned int i;

	for ( i=0; i < 1000; i++ )
	{
		Mutex_lock( p_mutex );
		fprintf( stderr, "%s|", str );
		fprintf( stderr, ":%i|", i );
		Mutex_unlock( p_mutex );
	}

	Semaphore_signal( semaphore );

	return NULL;
}

void* printer( void* obj )
{
	unsigned int i;
	for ( i=0; i < 100; i++ )
	{
		const char* str = (const char*) obj;
		while ( *str )
		{
			fprintf( stderr, "%c", *str );
			str++;
		}
		fprintf( stderr, ":%i\n", i );
	}
	Mutex_lock( cv_mutex );
	{
		threads--;
		if ( 0 == threads ) ConditionVariable_signal( cv );
	}
	Mutex_unlock( cv_mutex );
	return NULL;
}

void* printer2( void* obj )
{
	unsigned int i;
	for ( i=0; i < 100; i++ )
	{
		const char* str = (const char*) obj;
		while ( *str )
		{
			fprintf( stderr, "%c", *str );
			str++;
		}
		fprintf( stderr, ":%i\n", i );
	}
	Semaphore_signal( semaphore );
	return NULL;
}

void* printer3( void* obj )
{
	const char* str;
	unsigned int i;

	for ( i=0; i < 100; i++ )
	{
		Mutex_lock( mutex );
		str = (const char*) obj;
		while ( *str )
		{
			fprintf( stderr, "%c", *str );
			fflush( stderr );
			str++;
		}
		fprintf( stderr, ":%i\n", i );
		Mutex_unlock( mutex );
	}
	Semaphore_signal( semaphore );
	return NULL;
}

//
//	Multiple threads synchonized on completion by waiting for each thread.
//
bool test01_01()
{
	int status = 1;

	const char* str1 = "By a tree, by a river,";
	const char* str2 = "there's a hole in the ground.";
	const char* str3 = "Where an old man of Babel";
	const char* str4 = "goes around and around.";
	const char* str5 = "His mind is a beacon";
	const char* str6 = "in the middle of the night.";
	const char* str7 = "In a strange kind of fasion,";
	const char* str8 = "there's no wrong and no right.";

	IThread* thread1 = new_Thread( locking_printer, (void*) str1 );
	IThread* thread2 = new_Thread( locking_printer, (void*) str2 );
	IThread* thread3 = new_Thread( locking_printer, (void*) str3 );
	IThread* thread4 = new_Thread( locking_printer, (void*) str4 );
	IThread* thread5 = new_Thread( locking_printer, (void*) str5 );
	IThread* thread6 = new_Thread( locking_printer, (void*) str6 );
	IThread* thread7 = new_Thread( locking_printer, (void*) str7 );
	IThread* thread8 = new_Thread( locking_printer, (void*) str8 );

	p_mutex = new_Mutex();

	status &= Thread_run( thread1 );
	status &= Thread_run( thread2 );
	status &= Thread_run( thread3 );
	status &= Thread_run( thread4 );
	status &= Thread_run( thread5 );
	status &= Thread_run( thread6 );
	status &= Thread_run( thread7 );
	status &= Thread_run( thread8 );

	Thread_wait( thread1 );
	Thread_wait( thread2 );
	Thread_wait( thread3 );
	Thread_wait( thread4 );
	Thread_wait( thread5 );
	Thread_wait( thread6 );
	Thread_wait( thread7 );
	Thread_wait( thread8 );

	free_Thread( thread1 );
	free_Thread( thread2 );
	free_Thread( thread3 );
	free_Thread( thread4 );
	free_Thread( thread5 );
	free_Thread( thread6 );
	free_Thread( thread7 );
	free_Thread( thread8 );

	free_Mutex( p_mutex );

	return status;
}

//
//	Multiple threads synchonized on completion by waiting on a condition variable.
//
bool test01_02()
{
	int status = 1;

	const char* str1 = "May the force be with you";

	IThread* thread1 = new_Thread( locking_printer_cv, (void*) str1 );

	cv = new_ConditionVariable();
	cv_mutex = new_Mutex();

	Mutex_lock( cv_mutex );
	{
		threads++;
		status &= Thread_run( thread1 );
		while ( threads ) ConditionVariable_wait( cv, cv_mutex );
	}
	Mutex_unlock( cv_mutex );

	Thread_wait( thread1 );

	free_ConditionVariable( cv );
	free_Mutex( cv_mutex );
	free_Thread( thread1 );

	return status;
}

bool test01_02_01()
{
	int status = 1;

	const char* str1 = "1";
	const char* str2 = "2";
	const char* str3 = "3";
	const char* str4 = "4";

	IThread* thread1 = new_Thread( locking_printer_cv, (void*) str1 );
	IThread* thread2 = new_Thread( locking_printer_cv, (void*) str2 );
	IThread* thread3 = new_Thread( locking_printer_cv, (void*) str3 );
	IThread* thread4 = new_Thread( locking_printer_cv, (void*) str4 );

	cv = new_ConditionVariable();
	cv_mutex = new_Mutex();

	Mutex_lock( cv_mutex );
	{
		threads = 4;
		status &= Thread_run( thread1 );
		status &= Thread_run( thread2 );
		status &= Thread_run( thread3 );
		status &= Thread_run( thread4 );
		while (threads) ConditionVariable_wait( cv, cv_mutex );
	}
	Mutex_unlock( cv_mutex );

	free_ConditionVariable( cv );
	free_Mutex( cv_mutex );
	free_Thread( thread1 );
	free_Thread( thread2 );
	free_Thread( thread3 );
	free_Thread( thread4 );

	return status;
}

//
//	Multiple threads synchonized on completion by waiting on a semaphore.
//
bool test01_03()
{
	int status = 1;

	const char* str1 = "1";
	const char* str2 = "2";
	const char* str3 = "3";
	const char* str4 = "4";

	IThread* thread1 = new_Thread( locking_printer_sem, (void*) str1 );
	IThread* thread2 = new_Thread( locking_printer_sem, (void*) str2 );
	IThread* thread3 = new_Thread( locking_printer_sem, (void*) str3 );
	IThread* thread4 = new_Thread( locking_printer_sem, (void*) str4 );

	semaphore = new_Semaphore_value( -3 );
	mutex = new_Mutex();

	status &= Thread_run( thread1 );
	status &= Thread_run( thread2 );
	status &= Thread_run( thread3 );
	status &= Thread_run( thread4 );

	Semaphore_wait( semaphore );
	Semaphore_signal( semaphore );

	free_Mutex( mutex );
	free_Semaphore( semaphore );
	free_Thread( thread1 );
	free_Thread( thread2 );
	free_Thread( thread3 );
	free_Thread( thread4 );

	return status;
}

int main( int argc, char** argv )
{
	int status = 1;

	status &= Profile_TestFunction( "01:01", test01_01 );
	status &= Profile_TestFunction( "01:02", test01_02 );
	status &= Profile_TestFunction( "01:02:01", test01_02_01 );
	status &= Profile_TestFunction( "01:02", test01_03 );
	//status &= Profile_TestFunction( "01:03", test01_04 );
	
	return !status;
}
