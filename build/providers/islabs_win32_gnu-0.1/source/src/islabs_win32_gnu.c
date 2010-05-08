/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifdef win32
#include <windows.h>
int WINAPI DllMain( HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved )
{
	return TRUE;
}
#endif

#include <islabs/build/providers/ToolSetProvider.h>
#include <islabs/build/Command.private.h>
#include <islabs/build/CurrentContext.h>
#include <islabs/build/Global.h>
#include <islabs/build/ProviderContext.h>
#include <islabs/build/SoName.h>

#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/List.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/File.h>
#include <openocl/io/psa/Path.h>
#include <openocl/provider/psa/ProviderInstance.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//static int private_compile( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
//{
//	fprintf( stdout, "ISLabs::win32::gnu::compile()\n" );
//
//	return 1;
//}

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

static char* generateDLLPrefix( const char* packageName )
{
	unsigned int length = CharString_getLength( packageName );
	char* tmp = CharString_substring( packageName, 3, length - 1 );
	unsigned int posn = 0;
	length -= 3;
	while( (posn < length) && ('-' != tmp[posn]) )
	{
		posn++;
	}
	if ( '-' == tmp[posn] )
	{
		tmp[posn] = '\0';
	}
	
	return tmp;
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

static int private_compile( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	bool status = 1;
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	//IList* cppflags = translateArguments( context->FULL_CPPFLAGS );

	unsigned int i;
	unsigned int count = List_count( context->sourceFiles );
//	fprintf( stdout, "ISLabs::linux-gnu::gnu::compile( %s )\n", Path_getCondensed( Directory_getRealPath( target ) ) );
	for ( i = 0; (i < count) && status; i++ )
	{
		//	cc $CPPFLAGS $LANGFLAGS $CFLAGS $SRC_FILE -o $OUTPUT
		
		IPath* p = new_Path( context->sourceFiles->items[i] );
		char* target_file = CharString_cat2( Path_getBasename( p ), ".obj" );
		char* target_path = CharString_cat3( Path_getCondensed( Directory_getRealPath( target ) ), "/obj/", target_file );

		Command* command;
		IList*   arguments = new_List();
		IList*   native_arguments;
		
		// Compiler
		List_copyItem( arguments, context->compiler );
		
		// C-preprocessor flags
		addFlags( arguments, "-I", context->includeDirs );
		List_addList( arguments, BuildParameters_getGlobal( parameters )->CPPFLAGS );
		
		// C flags
		List_copyItem( arguments, "-Wall" );
		//  List_copyItem( arguments, "-fPIC" );  //  Not used MinGW
		List_copyItem( arguments, "-c" );
		
		if ( BuildParameters_isRelease( parameters ) )
		{
			List_copyItem( arguments, "-O2" );
		} else {
			List_copyItem( arguments, "-g" );
		}
		List_addList( arguments, BuildParameters_getGlobal( parameters )->CFLAGS );
		
		// Source files
		List_copyItem( arguments, context->sourceFiles->items[i] );
		
		// Output files
		List_copyItem( arguments, "-o" );
		List_copyItem( arguments, target_path );

		native_arguments = Path_ConvertListToNative( arguments );
		command = new_Command( context->compiler, (const char**) native_arguments->items );
		if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
		{
			status &= Command_getResult( command );
		} else {
			fprintf( stderr, "failed: " );
			Command_print( command, stderr );
		}
		free_Command( command );
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

	SoName* so_name = generateSoName( context->package_name, context->isLib );
	char* target_location_obj       = CharString_cat2( Path_getCondensed( Directory_getRealPath( target ) ), "/obj/" );
	char* target_location_lib       = CharString_cat2( Path_getCondensed( Directory_getRealPath( target ) ), "/lib/" );
	char* target_location_lib_long  = CharString_cat2( target_location_lib, so_name->long_name );
	char* target_location_lib_short = CharString_cat2( target_location_lib, so_name->short_name );

	Command* command;
	IList* arguments = new_List();
	IList* native_arguments;

	//  Linker
	List_copyItem( arguments, context->linker );
	
	//  Linker flags
	if ( !BuildParameters_isRelease( parameters ) )
	{
		List_copyItem( arguments, "-Wl,-G" );
	}
	List_copyItem( arguments, "-shared" );

	//  Object files: need to be before archive libraries so
	//                symbols can be found.
	addFlags( arguments, target_location_obj, context->objectFiles );

	//  Libraries	
	addFlags( arguments, "-L", context->libraryDirs );
	addFlags( arguments, "-l", context->libraries );

	List_addList( arguments, BuildParameters_getGlobal( parameters )->LFLAGS );
	List_addList( arguments, BuildParameters_getGlobal( parameters )->LDFLAGS );

	List_copyItem( arguments, "-Wl,-R$ORIGIN" );
	List_copyItem( arguments, "-Wl,-R$ORIGIN/../lib" );

	if ( BuildParameters_isDeep( parameters ) )
	{
		addFlags( arguments, "-Wl,-R$ORIGIN/../", context->runtimeLibraryDirs );
	}
	
	List_copyItem( arguments, "-Wl,-soname" );
	List_addItem( arguments, CharString_cat2( "-Wl,", so_name->long_name ) );
	
	List_copyItem( arguments, "-o" );
	List_copyItem( arguments, target_location_lib_long );

	native_arguments = Path_ConvertListToNative( arguments );
	command = new_Command( context->linker, (const char**) native_arguments->items );
	if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
	{
		status &= Command_getResult( command );
	} else {
		fprintf( stderr, "failed: " );
		Command_print( command, stderr );
	}
	free_Command( command );
	free_List( native_arguments );
	
	List_copyItem( arguments, "-o" );
	List_copyItem( arguments, target_location_lib_short );

	native_arguments = Path_ConvertListToNative( arguments );
	command = new_Command( context->linker, (const char**) native_arguments->items );
	if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
	{
		status &= Command_getResult( command );
	} else {
		fprintf( stderr, "failed: " );
		Command_print( command, stderr );
	}
	free_Command( command );
	free_List( native_arguments );

	free_List( arguments );
	
	free_CharString( target_location_obj );
	free_CharString( target_location_lib );
	free_CharString( target_location_lib_long );
	free_CharString( target_location_lib_short );

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
	char* target_location_obj       = CharString_cat2( Path_getCondensed( Directory_getRealPath( target ) ), "/obj/" );
	char* target_location_bin_short = CharString_cat3( Path_getCondensed( Directory_getRealPath( target ) ), "/bin/", so_name->short_name );

	Command* command;
	IList* arguments = new_List();
	IList* native_arguments;

	//  Linker
	List_copyItem( arguments, context->linker );
	
	//  Linker flags
	//if ( !BuildParameters_isRelease( parameters ) )
	//{
	//	List_copyItem( arguments, "-Wl,-G" );
	//}
	//List_copyItem( arguments, "-shared" );

	//  Object files: need to be before archive libraries so
	//                symbols can be found.
	addFlags( arguments, target_location_obj, context->objectFiles );

	//  System library directories
	//
	addFlags( arguments, "-L", context->systemLibraryDirs );
	
	if ( BuildParameters_isMonolithic( parameters ) )
	{
		addFlags( arguments, "", context->windows_archives );
		addFlags( arguments, "-l", context->system_libraries );
	} else {
		addFlags( arguments, "-L", context->libraryDirs );
		addFlags( arguments, "-l", context->libraries );
	}

	List_addList( arguments, BuildParameters_getGlobal( parameters )->LFLAGS );
	List_addList( arguments, BuildParameters_getGlobal( parameters )->LDFLAGS );

	List_copyItem( arguments, "-Wl,-R$ORIGIN" );
	List_copyItem( arguments, "-Wl,-R$ORIGIN/../lib" );

	if ( BuildParameters_isDeep( parameters ) )
	{
		addFlags( arguments, "-Wl,-R$ORIGIN/../", context->runtimeLibraryDirs );
	}

	
	//List_copyItem( arguments, "-Wl,-soname" );
	//List_addItem( arguments, CharString_cat2( "-Wl,", so_name->long_name ) );
	
	List_copyItem( arguments, "-o" );
	List_copyItem( arguments, target_location_bin_short );

	native_arguments = Path_ConvertListToNative( arguments );
	command = new_Command( context->linker, (const char**) native_arguments->items );
	if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
	{
		status &= Command_getResult( command );
	} else {
		fprintf( stderr, "failed: " );
		Command_print( command, stderr );
	}
	free_Command( command );
	free_List( native_arguments );

	free_List( arguments );
	free_CharString( target_location_obj );
	free_CharString( target_location_bin_short );

	return status;
}

static int private_link_archive( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	int status = 1;
	
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	const char* target_location = Path_getCondensed( Directory_getRealPath( target ) );
	
	//	Linux: ar rcs <LIBNAME> <OBJECTS>... 
	//	Win32: lib /OUT:<LIBNAME> <OBJECTS>...
	//	MinGW: ar -rcs <LIBNAME> <OBJECTS>

	char* name              = CharString_between( context->package_name, "", "-" );
	if ( !name )
	{
		name = new_CharString( context->package_name );
	}
	{
		char* libname           = CharString_cat2( name, ".lib" );
		char* full_lib_location = CharString_cat3( target_location, "/lib/", libname );
		char* obj_dir           = CharString_cat2( target_location, "/obj/" );

		if ( 0 < List_count( context->objectFiles ) )
		{
			Command* command;
			IList* arguments = new_List();
			IList* native_arguments;

			List_copyItem( arguments, context->archiver );
			List_copyItem( arguments, "-r" );
			List_copyItem( arguments, "-c" );
			List_copyItem( arguments, "-s" );
			List_copyItem( arguments, full_lib_location );
			addFlags( arguments, obj_dir, context->objectFiles );

			native_arguments = Path_ConvertListToNative( arguments );

			command = new_Command( context->archiver, (const char**) native_arguments->items );
			if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
			{
				status &= Command_getResult( command );
			} else {
				fprintf( stderr, "failed: " );
				Command_print( command, stderr );
			}
			free_Command( command );
			free_List( native_arguments );
			free_List( arguments );
		}

		free_CharString( obj_dir );
		free_CharString( full_lib_location );
		free_CharString( libname );
	}
	free_CharString( name );

	return status;
}

static int private_link_dll( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
//	fprintf( stdout, "ISLabs::linux-gnu::gnu::link()\n" );
	bool status = 1;
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	//SoName* so_name = generateSoName( context->package_name, context->isLib );
	char* dll_prefix = generateDLLPrefix( context->package_name );

	char* target_location_obj       = CharString_cat2( Path_getCondensed( Directory_getRealPath( target ) ), "/obj/" );
	char* target_location_lib       = CharString_cat2( Path_getCondensed( Directory_getRealPath( target ) ), "/lib/" );

	char* target_location_lib_dll    = CharString_cat3( target_location_lib, dll_prefix, ".dll" );
	char* target_location_lib_import = CharString_cat3( target_location_lib, dll_prefix, ".lib" );
	
	//char* target_location_lib_long  = CharString_cat2( target_location_lib, so_name->long_name );
	//char* target_location_lib_short = CharString_cat2( target_location_lib, so_name->short_name );

	Command* command;
	IList* arguments = new_List();
	IList* native_arguments;

	//  Linker
	List_copyItem( arguments, context->linker );
	
	//  Linker flags
	//if ( !BuildParameters_isRelease( parameters ) )
	//{
	//	List_copyItem( arguments, "-Wl,-G" );
	//}
	List_copyItem( arguments, "-shared" );

	List_addList( arguments, BuildParameters_getGlobal( parameters )->LFLAGS );
	List_addList( arguments, BuildParameters_getGlobal( parameters )->LDFLAGS );

	List_copyItem( arguments, "-dll" );
	List_copyItem( arguments, "-o" );
	List_copyItem( arguments, target_location_lib_dll );

	List_addItem( arguments, CharString_cat2( "-Wl,--out-implib,", target_location_lib_import ) );

	//  Object files: need to be before archive libraries so
	//                symbols can be found.
	List_copyItem( arguments, "-Wl,--whole-archive" );
	addFlags( arguments, target_location_obj, context->objectFiles );

	//  Libraries	
	List_copyItem( arguments, "-Wl,--no-whole-archive" );
	addFlags( arguments, "-l", context->libraries );

	List_copyItem( arguments, "-Wl,--export-all-symbols" );
	List_copyItem( arguments, "-Wl,--enable-auto-import" );
	List_copyItem( arguments, "-Wl,--enable-runtime-pseudo-reloc" );

	addFlags( arguments, "-L", context->libraryDirs );

	//List_copyItem( arguments, "-Wl,-R$ORIGIN" );
	//List_copyItem( arguments, "-Wl,-R$ORIGIN/../lib" );

	//if ( BuildParameters_isDeep( parameters ) )
	//{
	//	addFlags( arguments, "-Wl,-R$ORIGIN/../", context->runtimeLibraryDirs );
	//}
	
	//List_copyItem( arguments, "-Wl,-soname" );
	//List_addItem( arguments, CharString_cat2( "-Wl,", so_name->long_name ) );
	
	//List_copyItem( arguments, "-o" );
	//List_copyItem( arguments, target_location_lib_long );

	native_arguments = Path_ConvertListToNative( arguments );
	command = new_Command( context->linker, (const char**) native_arguments->items );
	Command_setStandardOut( command, "NUL" );
	if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
	{
		status &= Command_getResult( command );
	} else {
		fprintf( stderr, "failed: " );
		Command_print( command, stderr );
	}
	free_Command( command );
	free_List( native_arguments );
	free_List( arguments );
	
	free_CharString( target_location_obj );
	free_CharString( target_location_lib );
	free_CharString( target_location_lib_dll );
	free_CharString( target_location_lib_import );

	return status;
}

static int private_link( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	int status = 1;

	const ProviderContext* context = (ProviderContext*) providerContext;
	//fprintf( stderr, "Linking using islabs_win32_gnu\n" );
	if ( context->isLib )
	{
		status = private_link_library( buildParameters, providerContext, targetIDirectory );
		status = private_link_archive( buildParameters, providerContext, targetIDirectory );
		status = private_link_dll( buildParameters, providerContext, targetIDirectory );
	} else {
		status = private_link_executable( buildParameters, providerContext, targetIDirectory );
	}
	return status;
}

//static int private_link( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
//{
//	fprintf( stdout, "ISLabs::win32::gnu::link()\n" );
//
//	return 1;
//}

/****************************************************************************
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
					}
					else if ( 0 == strcmp( suffix, "cxx" ) )
					{
						compiler = global->C_PLUS_PLUS;
					}
					else if ( 0 == strcmp( suffix, "C" ) )
					{
						compiler = global->C_PLUS_PLUS;
					}
					else if ( 0 == strcmp( suffix, "c" ) )
					{
						compiler = global->CC;
					}
					if ( compiler )
					{
						char* object_file = CharString_cat2( prefix, ".o" );
						char* source_file = CharString_cat3( src_dir, "/", filename );  

						IList* native_arguments;
						IList* arguments = new_List();
						List_copyItem( arguments, compiler );
						List_addList( arguments, current->FULL_CPPFLAGS );
						List_addList( arguments, current->FULL_CFLAGS );
						List_copyItem( arguments, source_file );
						List_copyItem( arguments, "-o" );
						List_copyItem3( arguments, current->target_dir, "/obj/", object_file );

						native_arguments = Path_ConvertListToNative( arguments );
						status &= execute( global, compiler, (const char**) native_arguments->items );
						free_List( native_arguments );

						free( object_file );
						free( source_file );
						if ( !status ) break;
					}
				}
			}
		}
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

	char* dllname;
	char* obj_dir;
	IList* file_list;

	SoName* so_name = generateSoName( current->package_name, current->isLib );
	char* tmp = CharString_copy( current->package_name );
	unsigned int length = strlen( tmp );
	unsigned int posn = 0;
	while( (posn < length) && ('-' != tmp[posn]) )
	{
		posn++;
	}
	if ( '-' == tmp[posn] )
	{
		tmp[posn] = '\0';
	}
	
	dllname = CharString_cat2( tmp, ".dll" );
	
	//printf( "package: %s soname: %s\n", package_name, so_name->long_name );

	obj_dir = CharString_cat2( current->target_dir, "/obj" );
	file_list = retrieveFileList( obj_dir );

	//
	//
	//	gcc -Wall -g -shared -Wl,-G <targets> -Llib -L../lib
	//        -o ARCH/i386/linux-gnu/lib/openocl.so.1.0rc1.full
	//        -Wl,-soname -Wl,openocl.so.1

	if ( current->isLib )
	{
		//char* full_so_name_long = CharString_cat3( current->target_dir, "lib/", so_name->long_name );
		char* full_dll_name = CharString_cat3( current->target_dir, "/lib/", dllname );


		//Path* dll_path = new_Path( full_dll_name );
		//File* dll_file = new_File( dll_path );
		//if ( File_exists( dll_file ) )
		//{
		//	File_delete( dll_file );
		//}
		//free_File( dll_file );
		//free_Path( dll_path );
		
		char* linker1 = CharString_copy( current->compiler );

		//
		//	The order of the arguments have been rearranged to an order
		//	that seems to work best when compiling under cygwin.
		// *
		//	gcc -shared -o lib/${MODULE}.dll \
		//		-Wl,--whole-archive ${OBJECTS} \
		//		-Wl,--no-whole-archive -lstdc++ \
		//		-Wl,--export-all-symbols \
		//		-Wl,--enable-auto-import
		//
		//	I haven't included the follow flag as I don't appreciate what
		//	it is for (and I don't really care).
		//
		//		-Wl,--out-implib=lib/lib${MODULE}.dll.a
		//

		IList* native_arguments;
		IList* arguments = new_List();
		List_addItem( arguments, linker1 );

		List_addList( arguments, current->FULL_LIBFLAGS );

		List_addItem( arguments, CharString_copy( "-o" ) );
		List_addItem( arguments, full_dll_name );

		List_copyItem( arguments, "-Wl,--whole-archive" );
		List_addList( arguments, file_list );

		List_copyItem( arguments, "-Wl,--no-whole-archive" );
		List_addList( arguments, current->FULL_LIBS );

		List_copyItem( arguments, "-Wl,--export-all-symbols" );
		List_copyItem( arguments, "-Wl,--enable-auto-import" );
		
		List_addList( arguments, current->FULL_LDFLAGS ); // LibDir

		//List_addList( arguments, current->FULL_RUNLIBS );
		//List_copyItem( arguments, "-Wl,-soname" );
		//List_copyItem2( arguments, "-Wl,", so_name->long_name );
		
		native_arguments = Path_ConvertListToNative( arguments );
		//List_print( native_arguments );
		printf( "%s\n", full_dll_name );

		execute( global, linker1, (const char**) native_arguments->items );

		free_List( native_arguments );

		free_List( arguments );

	} else {

		char* target_dir = CharString_cat2( current->target_dir, "/bin" );
		char* full_so_name = CharString_cat3( current->target_dir, "/bin/", so_name->short_name );
		char* linker = CharString_copy( current->compiler );

		IList* native_arguments;
		IList* arguments = new_List();
		List_addItem( arguments, linker );
		List_addList( arguments, current->FULL_LDFLAGS );
		List_addList( arguments, current->FULL_LIBS );
		//List_addList( arguments, current->FULL_RUNLIBS );
		List_addList( arguments, file_list );
		List_addItem( arguments, CharString_copy( "-o" ) );
		List_addItem( arguments, full_so_name );

		native_arguments = Path_ConvertListToNative( arguments );
		//List_print( native_arguments );
		printf( "          %s\n", full_so_name );
		execute( global, linker, (const char**) native_arguments->items );
		free_List( native_arguments );
		free_List( arguments );

		//printf( "g++ -L -l -Wl,-rpath,xxx -o %s%s\n", bin_dir, so_name, file_list );

		//printf( "Printing DLLList\n" );
		//List_print( global->DLLList );

		if ( List_count( global->DLLList ) ) {
			copyfiles( global->DLLList, target_dir );
		}
	}	

	free_List( file_list );
	free( so_name );

	return status;
}
****************************************************************************/


static ToolSet* new_ToolSet( const char* compiler )
{
	ToolSet* toolset   = (ToolSet*) calloc( 1, sizeof( ToolSet ) );
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
	provider->info = "ISLabs Win32 GNU C, C++";
	provider->vendor = "ISLabs";
	provider->name = "windows_gnu";
	provider->version = "0.1";
	provider->object = new_ToolSetProvider();
	return provider;
}

EXPORT Provider* CreateBuildProvider_islabs_win32_gnu()
{
	return GetInstance();
}
