/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifdef _WIN32
#include <windows.h>
int WINAPI DllMain( HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved )
{
	return TRUE;
}
#endif

//#include <islabs/build/build.h>
//#include <islabs/build/copyfiles.h>
//#include <islabs/build/execute.h>
#include <islabs/build/providers/ToolSetProvider.h>
#include <islabs/build/BuildParameters.h>
#include <islabs/build/Command.private.h>
#include <islabs/build/CurrentContext.h>
#include <islabs/build/Global.h>
#include <islabs/build/ProviderContext.h>
#include <islabs/build/SoName.h>

#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/List.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/File.h>
#include <openocl/io/psa/Path.h>
#include <openocl/provider/psa/ProviderInstance.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static SoName* generateSoName( const char* package_name, int isLibrary )
{
	SoName* soname = (SoName*) calloc( 1, sizeof( SoName ) );

	char* pkg_tmp = CharString_copy( package_name );

	char* prefix = (char*) strtok( pkg_tmp, "-" );
	char* major = (char*) strtok( NULL, "." );

	if ( isLibrary )
	{
		if ( major ) {
			soname->long_name = CharString_cat3( prefix, ".so.", major );
		} else {
			soname->long_name = CharString_cat2( prefix, ".so" );
		}
		soname->short_name = CharString_cat2( prefix, ".so" );

	} else {
		soname->long_name = CharString_copy( prefix );
		soname->short_name = CharString_copy( prefix );
	}

	free( pkg_tmp );

	return soname;
}

static void addFlags( IList* arguments, const char* flag, const IList* includeDirs )
{
	unsigned int i;
	unsigned int count = List_count( includeDirs );
	for ( i=0; i < count; i++ )
	{
		List_copyItem2( arguments, flag, includeDirs->items[i] );
	}
}

/*****************************************************************************************
static IList* translateArguments( const IList* args )
{
	IList* list = new_List();
	unsigned int count = List_count( args );
	unsigned int i;
	IPath* tmp_path;
	char* tmp;
	unsigned int length;
	for ( i=0; i < count; i++ )
	{
		switch ( args->items[i][0] )
		{
		case '-':
			switch ( args->items[i][1] )
			{
			// C Preprocessor flags
			case 'I':
				length = CharString_getLength( args->items[i] );
				tmp = CharString_substring( args->items[i], 2, length - 1 );
				tmp_path = new_Path( tmp );
				List_copyItem2( list, "/I", Path_getNativeFormat( tmp_path ) );
				free_Path( tmp_path );
				free( tmp );
				break;
			case 'D':
				length = CharString_getLength( args->items[i] );
				tmp = CharString_substring( args->items[i], 2, length - 1 );
				List_copyItem2( list, "/D", tmp );
				free( tmp );
				break;
			// C Compiler flags
			case 'c':
				List_copyItem( list, "/c" );
				break;
			case 'W':
				if ( 0 == CharString_compare( args->items[i], "-Wall" ) )
				{
					List_copyItem( list, "/W2" );
					break;
				}
			// Debug flags
				if ( 0 == CharString_compare( args->items[i], "-Wl,-G" ) )
				{
					List_copyItem( list, "/DEBUG" );
					break;
				}
				break;
			case 'g':
				List_copyItem( list, "/Zi" );
				List_copyItem( list, "/Yd" );
				break;
			case '-':
				if ( 0 == CharString_compare( args->items[i], "--debug-lib" ) )
				{
					List_copyItem( list, "/MDd" );
				}
				else if ( 0 == CharString_compare( args->items[i], "--debug-nolibc" ) )
				{
					List_copyItem( list, "/NODEFAULTLIB:libc" );
				}
				break;
				// Shared Library flags
			case 's':
				if ( 0 == CharString_compare( args->items[i], "-shared" ) )
				{
					List_copyItem( list, "/DLL" );
				}
				break;
			// Library link flags
			case 'l':
				length = CharString_getLength( args->items[i] );
				tmp = CharString_substring( args->items[i], 2, length - 1 );
				tmp_path = new_Path( tmp );
				List_addItem( list, CharString_cat2( "/DEFAULTLIB:", Path_getNativeFormat( tmp_path ) ) );
				free_Path( tmp_path );
				free( tmp );
				break;
			// Library location flags
			case 'L':
				length = CharString_getLength( args->items[i] );
				tmp = CharString_substring( args->items[i], 2, length - 1 );
				tmp_path = new_Path( tmp );
				List_addItem( list, CharString_cat2( "/LIBPATH:", Path_getNativeFormat( tmp_path ) ) );
				free_Path( tmp_path );
				free( tmp );
				break;
			default:
				continue;
			}
			break;
		case '/':
			List_addItem( list, CharString_copy( args->items[i] ) );
			break;
		default:
			continue;
		}
	}
	return list;
}
*****************************************************************************************/

static int private_compile( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	//fprintf( stdout, "ISLabs::win32::ms::compile()\n" );

	bool status = 1;

	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context    = (ProviderContext*) providerContext;
	const IDirectory*      target     = (IDirectory*) targetIDirectory;

	unsigned int i;
	unsigned int count = List_count( context->sourceFiles );
//	fprintf( stdout, "ISLabs::linux-gnu::gnu::compile( %s )\n", Path_getCondensed( Directory_getRealPath( target ) ) );
	for ( i = 0; (i < count) && status; i++ )
	{
		//	cc $CPPFLAGS $LANGFLAGS $CFLAGS $SRC_FILE -o $OUTPUT
		
		IPath* p = new_Path( context->sourceFiles->items[i] );
		char* target_file = CharString_cat2( Path_getBasename( p ), ".obj" );
		char* target_path = CharString_cat3( Path_getNativeFormat( Directory_getRealPath( target ) ), "\\obj\\", target_file );

		Command* command;
		IList*   arguments = new_List();

		// Compiler
		List_copyItem( arguments, context->compiler );
		
		// No logo
		List_copyItem( arguments, "/nologo" );

		// C-preprocessor flags
		addFlags( arguments, "/I", context->includeDirs );
		List_addList( arguments, BuildParameters_getGlobal( parameters )->CPPFLAGS );
		
		// C flags
		List_copyItem( arguments, "/EHsc" ); // Enable Synchronous Exception Handling
		List_copyItem( arguments, "/GR" );   // Enable RTTI
		List_copyItem( arguments, "/W2" );   // Enable warnings
		List_copyItem( arguments, "/c" );    // Compile to object
		
		if ( BuildParameters_isRelease( parameters ) )
		{
			List_copyItem( arguments, "/MD" );   // Link to Multi-threaded LIBC
			//List_copyItem( arguments, "/Ox" );   // Optimize
		} else {
			List_copyItem( arguments, "/MD" );   // Link to Multi-threaded Debug LIBC
			List_copyItem( arguments, "/Od" );   // Disable optimization
			//List_copyItem( arguments, "/Zi" );   // Include debug
			//List_copyItem( arguments, "/Yd" );   // Include debug
		}
		List_addList( arguments, BuildParameters_getGlobal( parameters )->CFLAGS );
		
		// Source files
		List_copyItem( arguments, Path_getNativeFormat( p ) ); //context->sourceFiles->items[i] );
		
		// Output files
		List_copyItem2( arguments, "/Fo", target_path );

		command = new_Command( context->compiler, (const char**) arguments->items );
		if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
		{
			status &= Command_getResult( command );
		} else {
			fprintf( stderr, "failed: " );
			Command_print( command, stderr );
		}
		free_Command( command );
		free_List( arguments );
		free_CharString( target_file );
		free_Path( p );
	}
	return status;
}

static int private_link_library( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
//	fprintf( stdout, "ISLabs::linux-gnu::gnu::link()\n" );
	bool status = 1;
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	char* target_location_obj       = CharString_cat2( Path_getNativeFormat( Directory_getRealPath( target ) ), "\\obj\\" );
	char* target_location_lib       = CharString_cat2( Path_getNativeFormat( Directory_getRealPath( target ) ), "\\lib\\" );

	char* name = CharString_between( context->package_name, "lib", "-" );
	if ( name )
	{
		char* libname = CharString_cat2( name, ".lib" );
		char* dllname = CharString_cat2( name, ".dll" );

		char* full_lib_name = CharString_cat2( target_location_lib, libname );
		char* full_dll_name = CharString_cat2( target_location_lib, dllname );

		Command* command;
		IList* arguments = new_List();

		//  Linker
		List_copyItem( arguments, context->linker );

		//  No logo
		List_copyItem( arguments, "/nologo" );

		//	Do not use incremental link functionality of link
		//List_copyItem( arguments, "/INCREMENTAL:NO" );

		//  Linker flags
		List_copyItem( arguments, "/DLL" );
		if ( !BuildParameters_isRelease( parameters ) )
		{
			List_copyItem( arguments, "/DEBUG" );
		}
		List_copyItem2( arguments, "/IMPLIB:", full_lib_name );

		//  Object files: need to be before archive libraries so
		//                symbols can be found.
		addFlags( arguments, target_location_obj, context->objectFiles );

		//  Libraries	
		addFlags( arguments, "/LIBPATH:", context->libraryDirs );
		addFlags( arguments, "/DEFAULTLIB:", context->libraries );

		List_addList( arguments, BuildParameters_getGlobal( parameters )->LFLAGS );
		List_addList( arguments, BuildParameters_getGlobal( parameters )->LDFLAGS );

		List_copyItem2( arguments, "/OUT:", full_dll_name );

		command = new_Command( context->linker, (const char**) arguments->items );
		if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
		{
			status &= Command_getResult( command );
		} else {
			fprintf( stderr, "failed: " );
			Command_print( command, stderr );
		}
		free_Command( command );
		free_List( arguments );
	}
	free_CharString( target_location_obj );
	free_CharString( target_location_lib );

	return status;
}

static int private_link_executable( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
//	fprintf( stdout, "ISLabs::linux-gnu::gnu::link()\n" );
	bool status = 1;
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	SoName* so_name = generateSoName( context->package_name, context->isLib );
	char* target_location_obj       = CharString_cat2( Path_getNativeFormat( Directory_getRealPath( target ) ), "\\obj\\" );
	char* target_location_bin       = CharString_cat2( Path_getNativeFormat( Directory_getRealPath( target ) ), "\\bin\\" );

	char* full_so_name = CharString_cat3( target_location_bin, so_name->short_name, ".exe" );


	if ( full_so_name )
	{
		Command* command;
		IList* arguments = new_List();

		//  Linker
		List_copyItem( arguments, context->linker );

		//  No logo
		List_copyItem( arguments, "/nologo" );

		//	Do not use incremental link functionality of link
		List_copyItem( arguments, "/INCREMENTAL:NO" );

		//  Linker flags
		if ( !BuildParameters_isRelease( parameters ) )
		{
			List_copyItem( arguments, "/DEBUG" );
		}

		//  Object files: need to be before archive libraries so
		//                symbols can be found.
		{
			//	With previous versions of MS Visual Studio the thrownew.obj
			//	had to be included. Need to check version of MSVS. For now
			//	have just commented out.
			//if ( 0 == CharString_compare( context->language, "C++" ) )
			//{
			//	//List_copyItem( arguments, "thrownew.obj" );
			//}
			addFlags( arguments, target_location_obj, context->objectFiles );
		}
		
		//  System library directories
		//
		addFlags( arguments, "/LIBPATH:", context->systemLibraryDirs );

		//  Libraries	
		if ( BuildParameters_isMonolithic( parameters ) )
		{
			//int max = List_count( context->windows_archives );
			//int i;
			//for ( i=0; i < max; i++ )
			//{
			//	char* winpath = new_CharString( context->windows_archives->items[i] );
			//	CharString_replace( winpath, '/', '\\' );
			//	List_addItem( arguments, winpath );
			//}
			List_addList( arguments, context->windows_archives );
			addFlags( arguments, "/DEFAULTLIB:", context->system_libraries );
		} else {
			addFlags( arguments, "/LIBPATH:", context->libraryDirs );
			addFlags( arguments, "/DEFAULTLIB:", context->libraries );
		}

		List_addList( arguments, BuildParameters_getGlobal( parameters )->LFLAGS );
		List_addList( arguments, BuildParameters_getGlobal( parameters )->LDFLAGS );

		List_copyItem2( arguments, "/OUT:", full_so_name );

		command = new_Command( context->linker, (const char**) arguments->items );
		if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
		{
			status &= Command_getResult( command );
		} else {
			fprintf( stderr, "failed: " );
			Command_print( command, stderr );
		}
		free_Command( command );
		free_List( arguments );
	}
	free_CharString( target_location_obj );
	free_CharString( target_location_bin );

	return status;
}

static int private_link_archive( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	int status = 1;
	
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	const char* target_location = Path_getNativeFormat( Directory_getRealPath( target ) );
	
	//	Linux: ar rcs <LIBNAME> <OBJECTS>... 
	//	Win32: lib /OUT:<LIBNAME> <OBJECTS>...

	char* name              = CharString_between( context->package_name, "", "-" );
	if ( !name )
	{
		name = new_CharString( context->package_name );
	}
	{
		char* libname           = CharString_cat2( name, ".lib" );
		char* full_lib_location = CharString_cat3( target_location, "\\lib\\", libname );
		char* obj_dir           = CharString_cat2( target_location, "\\obj\\" );

		if ( 0 < List_count( context->objectFiles ) )
		{
			Command* command;
			IList* arguments = new_List();

			//  Archiver
			List_copyItem( arguments, context->archiver );

			//  No logo
			List_copyItem( arguments, "/nologo" );

			//  Output
			List_copyItem2( arguments, "/OUT:", full_lib_location );

			//  Input object files
			addFlags( arguments, obj_dir, context->objectFiles );

			command = new_Command( context->archiver, (const char**) arguments->items );

			if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
			{
				status &= Command_getResult( command );
			} else {
				fprintf( stderr, "failed: " );
				Command_print( command, stderr );
			}
			free_Command( command );
			free_List( arguments );
		}

		free_CharString( obj_dir );
		free_CharString( full_lib_location );
		free_CharString( libname );
	}
	free_CharString( name );

	return status;
}

static int private_link( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	int status;
	const ProviderContext* context = (ProviderContext*) providerContext;
	if ( context->isLib )
	{
		status = private_link_library( buildParameters, providerContext, targetIDirectory );
		status = private_link_archive( buildParameters, providerContext, targetIDirectory );
	} else {
		status = private_link_executable( buildParameters, providerContext, targetIDirectory );
	}
	return status;
}


/*******************************************************************************
static int private_objectify( const void* aGlobal, void* currentContext, const void* directory, const void* suffix )
{
	int status = 1;
	if ( directory )
	{
		Global* global = (Global*) aGlobal;
		CurrentContext* current = (CurrentContext*) currentContext;
		IDirectory* dir = (IDirectory*) directory;

		const char* src_dir = Path_getCommon( Directory_getRealPath( dir ) );
		IList* list = Directory_filteredFileList( dir, suffix );

		unsigned int i;
		for ( i=0; i < list->count; i++ )
		{
			//status = compile( global, compiler, current, src_dir, entry->d_name, prefix );
			const char* filename = list->items[i];

			if ( 0 == CharString_startsWith( filename, "." ) )
			{

				char* tmp_file = CharString_copy( filename );
				char* prefix = (char*) strtok( tmp_file, "." );
				char* suffix = (char*) strtok( NULL, "." );
				char* unexpected_suffix = (char*) strtok( NULL, "." );

				if ( (NULL != suffix) && (NULL == unexpected_suffix) )
				{
					const char* compiler = NULL;
					if ( 0 == strcmp( suffix, "cpp" ) )
					{
						compiler = global->C_PLUS_PLUS;
						current->language = "C++";
					}
					else if ( 0 == strcmp( suffix, "cxx" ) )
					{
						compiler = global->C_PLUS_PLUS;
						current->language = "C++";
					}
					else if ( 0 == strcmp( suffix, "C" ) )
					{
						compiler = global->C_PLUS_PLUS;
						current->language = "C++";
					}
					else if ( 0 == strcmp( suffix, "c" ) )
					{
						compiler = global->CC;
						if ( 0 != CharString_compare( current->language, "C++" ) )
						{
							current->language = "C";
						}
					}
					if ( compiler )
					{
						char* object_file = CharString_cat2( prefix, ".o" );
						char* source_file = CharString_cat3( src_dir, "/", filename );  
						char* target = CharString_cat3( current->target_dir, "/obj/", object_file );
						IPath* compiler_path = new_Path( compiler );
						IPath* object_file_path = new_Path( object_file );
						IPath* source_file_path = new_Path( source_file );
						IPath* target_path = new_Path( target );
						const char* native_compiler = Path_getNativeFormat( compiler_path );
						char* native_target_arg = CharString_cat2( "/Fo", Path_getNativeFormat( target_path ) );

						IList* native_arguments = new_List();
						IList* quoted_native_arguments;
						IList* cpp_flags = translateArguments( current->FULL_CPPFLAGS );
						IList* c_flags = translateArguments( current->FULL_CFLAGS );

						List_copyItem( native_arguments, native_compiler );	//	cl
						List_copyItem( native_arguments, "/nologo" );		//		/nologo
						List_addList( native_arguments, cpp_flags );		//		/I... /D...
						List_copyItem( native_arguments, "/EHsc" ); // Enable Synchronous Exception Handling
						List_copyItem( native_arguments, "/GR" );	// Enable RTTI
						List_addList( native_arguments, c_flags );			//		/c
						List_copyItem( native_arguments, Path_getNativeFormat( source_file_path ) );
						List_addItem( native_arguments, native_target_arg );

						quoted_native_arguments = List_quotedList( native_arguments );

						status &= execute( global, native_compiler, (const char**) quoted_native_arguments->items );
						//fprintf( stdout, "#" );

						free_List( quoted_native_arguments );
						free_List( native_arguments );

						free_Path( target_path );
						free_Path( source_file_path );
						free_Path( object_file_path );
						free_Path( compiler_path );

						free( target );
						free( object_file );
						free( source_file );
						if ( !status ) break;
					}
				}
			}
		}
		//fprintf( stdout, "\r" );
		free_List( list );
	}	
	return status;
}

static int private_linkify( const void* aGlobal, const void* currentContext )
{
	int status = 1;
    
	Global* global = (Global*) aGlobal;
	CurrentContext* current = (CurrentContext*) currentContext;

	//	package_dir == /SOME/DIRECTORY/.../[package_id]<.version>
	//	

	char* obj_dir;
	IList* file_list;

	SoName* so_name = generateSoName( current->package_name, current->isLib );
	
	//printf( "package: %s soname: %s\n", package_name, so_name->long_name );
	obj_dir = CharString_cat2( current->target_dir, "/obj" );
	file_list = retrieveFileList( obj_dir );

	if ( current->isLib )
	{
		char* name = CharString_between( current->package_name, "lib", "-" );
		if ( name )
		{
			char* libname = CharString_cat2( name, ".lib" );
			char* dllname = CharString_cat2( name, ".dll" );

			char* full_lib_name = CharString_cat3( current->target_dir, "/lib/", libname );
			char* full_dll_name = CharString_cat3( current->target_dir, "/lib/", dllname );
			IPath* full_lib_path = new_Path( full_lib_name );
			IPath* full_dll_path = new_Path( full_dll_name );

			IList* quoted_native_arguments;
			IList* native_arguments = new_List();
			char* lib_arg = CharString_cat2( "/IMPLIB:", Path_getNativeFormat( full_lib_path ) );
			char* dll_arg = CharString_cat2( "/OUT:", Path_getNativeFormat( full_dll_path ) );

			IList* l_flags = translateArguments( current->FULL_LFLAGS );
			IList* ld_flags = translateArguments( current->FULL_LDFLAGS );
			IList* lib_flags = translateArguments( current->FULL_LIBFLAGS );
			IList* libs = translateArguments( current->FULL_LIBS );

			List_copyItem( native_arguments, global->LD );		//	link
			List_copyItem( native_arguments, "/nologo" );		//		/nologo
			List_addList( native_arguments, lib_flags );		//		/DLL              (FULL_LIBFLAGS)
			List_addList( native_arguments, l_flags );			//		/DEBUG            (FULL_LFLAGS)
			List_addItem( native_arguments, lib_arg );			//		/IMPLIB:lib\library.lib
			List_addList( native_arguments, file_list );		//		obj\*.o
			List_addList( native_arguments, libs );				//		/DEFAULTLIB:library       (FULL_LIBS)
			List_addList( native_arguments, ld_flags );			//		/LIBPATH:somedir          (FULL_LDFLAGS)
			List_addItem( native_arguments, dll_arg );			//		/OUT:lib\library.dll

			quoted_native_arguments = List_quotedList( native_arguments );
			printf( "%s\n", full_dll_name );
			execute( global, global->LD, (const char**) quoted_native_arguments->items );

			free_List( quoted_native_arguments );
			free_List( native_arguments );
			free_Path( full_dll_path );
			free_Path( full_lib_path );
			free( full_dll_name );
			free( full_lib_name );
		} else {
			fprintf( stdout, "Warning: %s\n", current->package_name );
		}
	} else {

		char* target_dir = CharString_cat2( current->target_dir, "/bin" );
		char* full_so_name = CharString_cat3( current->target_dir, "/bin/", so_name->short_name );
		IPath* bin_path = new_Path( full_so_name ); 
		char* bin_arg = CharString_cat3( "/OUT:", Path_getNativeFormat( bin_path ), ".exe" );

		IList* l_flags = translateArguments( current->FULL_LFLAGS );
		IList* ld_flags = translateArguments( current->FULL_LDFLAGS );
		IList* libs = translateArguments( current->FULL_LIBS );

		IList* quoted_native_arguments;
		IList* native_arguments = new_List();

		List_copyItem( native_arguments, global->LD );			//	link
		List_copyItem( native_arguments, "/nologo" );			//		/nologo
		//List_copyItem( native_arguments, "/DEBUG" );			//		/DEBUG
		List_addList( native_arguments, l_flags );

		if ( 0 == CharString_compare( current->language, "C++" ) )
		{
			List_copyItem( native_arguments, "thrownew.obj" );		//		causes new to throw
		}
		List_addList( native_arguments, file_list );			//		obj\*.o
		if ( global->monolithic )
		{
			List_addList( native_arguments, current->windows_archives );
		} else {
			List_addList( native_arguments, ld_flags );			//		/LIBPATH:somedirector
			List_addList( native_arguments, libs );				//		/DEFAULTLIB:somelibrary
		}
		List_addItem( native_arguments, bin_arg );				//		/OUT:bin\some.exe
     
		quoted_native_arguments = List_quotedList( native_arguments );

		printf( "          %s\n", full_so_name );
		execute( global, global->LD, (const char**) quoted_native_arguments->items );

		free_List( native_arguments );
		free_Path( bin_path );
		free( full_so_name );

		if ( !global->buildStatic && List_count( global->DLLList ) ) {
			copyfiles( global->DLLList, target_dir );
		}
		free( target_dir );
	}	

	free_List( file_list );
	free( so_name );

	return status;
}

static int private_archive( const void* aGlobal, const void* currentContext )
{
	int status = 1;
    
	Global* global = (Global*) aGlobal;
	CurrentContext* current = (CurrentContext*) currentContext;

	//	Linux: ar rcs <LIBNAME> <OBJECTS>... 
	//	Win32: lib /OUT:<LIBNAME> <OBJECTS>...
	if ( current->isLib )
	{
		char* obj_dir;
		IList* file_list;
		char* name = CharString_between( current->package_name, "", "-" );
		char* libname = CharString_cat2( name, ".lib" );
		char* full_lib_name = CharString_cat3( current->target_dir, "/lib/", libname );
		IPath* full_lib_path = new_Path( full_lib_name );
		char* lib_arg = CharString_cat2( "/OUT:", Path_getNativeFormat( full_lib_path ) );

		obj_dir = CharString_cat2( current->target_dir, "/obj" );
		file_list = retrieveFileList( obj_dir );

		if ( 0 < List_count( file_list ) )
		{
			IList* quoted_native_arguments;
			IList* native_arguments = new_List();

			List_copyItem( native_arguments, global->AR );		//	lib
			List_copyItem( native_arguments, "/nologo" );		//		/nologo
			List_copyItem( native_arguments, lib_arg );
			List_addList( native_arguments, file_list );		//		obj\*.o

			quoted_native_arguments = List_quotedList( native_arguments );
			printf( "%s\n", full_lib_name );
			status &= execute( global, global->AR, (const char**) quoted_native_arguments->items );

			free_List( quoted_native_arguments );
			free_List( native_arguments );
			free( obj_dir );
			free( lib_arg );
			free_Path( full_lib_path );
			free( full_lib_name );
			free( libname );
			free( name );
		}
	}
	return status;
}
*******************************************************************************/


static ToolSet* new_ToolSet( const char* compiler )
{
	ToolSet* toolset = (ToolSet*) calloc( 1, sizeof( ToolSet ) );
	toolset->compile   = &private_compile;
	toolset->link      = &private_link;
	return toolset;
}

static ToolSet* private_getToolSet( const char* compiler )
{
	return new_ToolSet( compiler );
}

static ToolSetProvider* new_ToolSetProvider()
{
	ToolSetProvider* self = (ToolSetProvider*) calloc( 1, sizeof( ToolSetProvider ) );
	self->getToolSet = &private_getToolSet;
	return self;
}

#ifndef OPENOCL_PROVIDER
static
#endif
Provider* GetInstance()
{
	struct _Provider* provider = calloc( 1, sizeof( struct _Provider ) );
	provider->type = "ToolSetProvider";
	provider->info = "ISLabs Win32 MS C, C++";
	provider->vendor = "ISLabs";
	provider->name = "windows_ms";
	provider->version = "0.1";
	provider->object = new_ToolSetProvider();
	return provider;
}

EXPORT Provider* CreateBuildProvider_islabs_win32_ms()
{
	return GetInstance();
}
