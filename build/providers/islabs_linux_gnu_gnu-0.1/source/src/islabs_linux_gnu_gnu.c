/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

//#include <islabs/build/build.h>
//#include <islabs/build/copyfiles.h>
//#include <islabs/build/execute.h>
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

		if ( CharString_contains( context->compiler, "cc" ) )
		{
			List_addList( arguments, BuildParameters_getGlobal( parameters )->CPPFLAGS );
		}
		else
		if ( CharString_contains( context->compiler, "++" ) )
		{
			List_addList( arguments, BuildParameters_getGlobal( parameters )->CXXFLAGS );
		}
		
		// C flags
		List_copyItem( arguments, "-Wall" );
		List_copyItem( arguments, "-fPIC" );
		List_copyItem( arguments, "-c" );
		
		if ( BuildParameters_isRelease( parameters ) )
		{
			//List_copyItem( arguments, "-O2" );
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
		addFlags( arguments, "", context->unix_archives );
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

	if ( BuildParameters_getVerbosity( parameters ) )
	{
		Command_print( command, stderr );
	}

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

	char* name = CharString_between( context->package_name, "", "-" );
	if ( !name )
	{
		name = new_CharString( context->package_name );
	}
	{
		char* libname           = CharString_cat2( name, ".a" );
		char* full_lib_location = CharString_cat3( target_location, "/lib/", libname );
		char* obj_dir           = CharString_cat2( target_location, "/obj/" );

		if ( 0 < List_count( context->objectFiles ) )
		{
			Command* command;
			IList* arguments = new_List();
			IList* native_arguments;
	
			List_copyItem( arguments, context->archiver );
			List_copyItem( arguments, "rcs" );
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

/*************************************************************************
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
				IList* language_flags = new_List();

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
						List_copyItem( language_flags, "-std=c99" );
						//List_copyItem( language_flags, "-Wdeclaration-after-statement" );
					}
					if ( compiler )
					{
						char* object_file = CharString_cat2( prefix, ".o" );
						char* source_file = CharString_cat3( src_dir, "/", filename );  

						IList* native_arguments;
						IList* arguments = new_List();
						List_copyItem( arguments, compiler );
						List_addList( arguments, current->FULL_CPPFLAGS );
						List_addList( arguments, language_flags );
						List_addList( arguments, current->FULL_CFLAGS );
						List_copyItem( arguments, source_file );
						List_copyItem( arguments, "-o" );
						List_copyItem3( arguments, current->target_dir, "/obj/", object_file );

						native_arguments = Path_ConvertListToNative( arguments );
						status &= execute( global, compiler, (const char**) native_arguments->items );
						free_List( native_arguments );

						free( object_file );
						free( source_file );
					}
				}
				free_List( language_flags );
			}
		}
		free_List( list );
	}	
	return status;
}

static int private_linkify( const void* aGlobal, const void* currentContext )
{
	int status = 0;

	Global* global = (Global*) aGlobal;
	CurrentContext* current = (CurrentContext*) currentContext;

	char* target_dir = CharString_cat2( current->target_dir, "/bin" );
	char* pkg_tmp = CharString_copy( current->package_dir );
	char* package_name = CharString_basename( pkg_tmp );

	SoName* so_name = generateSoName( package_name, current->isLib );
	
	char* obj_dir = CharString_cat2( current->target_dir, "/obj" );
	IList* file_list = retrieveFileList( obj_dir );
	
	if ( current->isLib )
	{
		char* full_so_name_long = CharString_cat3( current->target_dir, "/lib/", so_name->long_name );
		char* full_so_name_short = CharString_cat3( current->target_dir, "/lib/", so_name->short_name );

		char* linker1 = CharString_copy( current->compiler );
		char* linker2 = CharString_copy( current->compiler );


		IList* arguments = new_List();
		List_addItem( arguments, linker1 );
		List_addList( arguments, current->FULL_LFLAGS );
		List_addList( arguments, current->FULL_LIBFLAGS );

		List_addList( arguments, current->FULL_LDFLAGS );
		List_addList( arguments, current->FULL_LIBS );
		List_addList( arguments, current->FULL_RUNLIBS );
		List_addList( arguments, file_list );
		List_copyItem( arguments, "-Wl,-soname" );
		List_copyItem2( arguments, "-Wl,", so_name->long_name );
		List_addItem( arguments, CharString_copy( "-o" ) );
		List_copyItem( arguments, full_so_name_long );

		if ( global->verbosity > 1 )
		{
			List_print( arguments );
		}
		//printf( "%s\n", full_so_name_long );
		execute( global, linker1, (const char**) arguments->items );
		free_List( arguments );

		//
		//	Sanctioned output: full name of end product output.
		//
		printf( "%s\n", full_so_name_long );
	
		free( full_so_name_long );

		arguments = new_List();
		List_addItem( arguments, linker2 );
		List_addList( arguments, current->FULL_LFLAGS );
		List_addList( arguments, current->FULL_LIBFLAGS );
		List_addList( arguments, current->FULL_LDFLAGS );
		List_addList( arguments, current->FULL_LIBS );
		List_addList( arguments, current->FULL_RUNLIBS );
		List_addList( arguments, file_list );
		List_copyItem( arguments, "-Wl,-soname" );
		List_copyItem2( arguments, "-Wl,", so_name->long_name );
		List_addItem( arguments, CharString_copy( "-o" ) );
		List_copyItem( arguments, full_so_name_short );
		//List_print( arguments );
		execute( global, linker2, (const char**) arguments->items );
		free_List( arguments );

		//
		//	Sanctioned output: full name of end product output.
		//
		printf( "%s\n", full_so_name_short );

		free( full_so_name_short );
		status = 1;
	} else {

		char* full_so_name = CharString_cat3( current->target_dir, "/bin/", so_name->short_name );
		char* linker = CharString_copy( current->compiler );

		IList* arguments = new_List();
		List_addItem( arguments, linker );
		List_addList( arguments, current->FULL_LFLAGS );
		
		if ( !global->monolithic )
		{
			List_addList( arguments, current->FULL_RUNLIBS );
		}
		List_addList( arguments, file_list );
		
		if ( global->monolithic )
		{
			List_addList( arguments, current->unix_archives );
		} else {
			List_addList( arguments, current->FULL_LIBS );
			List_addList( arguments, current->FULL_LDFLAGS );
		}

		List_addItem( arguments, CharString_copy( "-o" ) );
		List_addItem( arguments, full_so_name );

		if ( global->verbosity > 1 )
		{
			List_print( arguments );
		}

		printf( "%s\n", full_so_name );
		execute( global, linker, (const char**) arguments->items );

		free_List( arguments );

		if ( global->verbosity )
		{
			Global_tab( global, stdout );
			printf( "%s\n", so_name->long_name );
		}

		if ( global->copyLibs && !global->monolithic && List_count( current->elf_libraries ) )
		{
			if ( 0 == copyfiles( current->elf_libraries, target_dir ) )
			{
				fprintf( stdout, "linkify.c: copying files failed\n" );
			}
		}

		//printf( "g++ -L -l -Wl,-rpath,xxx -o %s%s\n", bin_dir, so_name, file_list );
		status = 1;
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
		char* libname = CharString_cat2( name, ".a" );
		char* full_lib_name = CharString_cat3( current->target_dir, "/lib/", libname );
		IPath* full_lib_path = new_Path( full_lib_name );
		char* lib_arg = CharString_copy( Path_getNativeFormat( full_lib_path ) );
	
		obj_dir = CharString_cat2( current->target_dir, "/obj" );
		file_list = retrieveFileList( obj_dir );

		if ( 0 < List_count( file_list ) )
		{
			IList* native_arguments = new_List();

			List_copyItem( native_arguments, global->AR );		//	lib
			List_copyItem( native_arguments, "rcs" );		//		/nologo
			List_copyItem( native_arguments, lib_arg );
			List_addList( native_arguments, file_list );		//		obj\*.o
		
			printf( "%s\n", full_lib_name );
			status &= execute( global, global->AR, (const char**) native_arguments->items );

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
*************************************************************************/

static ToolSet* new_ToolSet( const char* compiler )
{
	ToolSet* toolset = (ToolSet*) calloc( 1, sizeof( ToolSet ) );
	toolset->compile   = &private_compile;
	toolset->link      = &private_link;
	//toolset->objectify = NULL; //&private_objectify;
	//toolset->linkify   = NULL; //&private_linkify;
	//toolset->archive   = NULL; //&private_archive;
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
	provider->info = "ISLabs Linux GNU C, C++";
	provider->vendor = "ISLabs";
	provider->name = "linux-gnu_gnu";
	provider->version = "0.1";
	provider->object = new_ToolSetProvider();	
	return provider;
}

Provider* CreateBuildProvider_islabs_linux_gnu_gnu()
{
	return GetInstance();
}


