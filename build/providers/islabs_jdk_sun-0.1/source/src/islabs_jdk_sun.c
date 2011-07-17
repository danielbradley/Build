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

static int private_compile( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	bool status = 1;
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	//IList* cppflags = translateArguments( context->FULL_CPPFLAGS );

	unsigned int i;
	unsigned int count = List_count( context->sourceFiles );
	//fprintf( stdout, "ISLabs::darwin::sun::compile (javac) ( %s )\n", Path_getCondensed( Directory_getRealPath( target ) ) );

	{
		// javac -g $JFLAGS -g -sourcepath source/java .java -d obj
		
		//IPath* p = new_Path( context->sourceFiles->items[i] );
		char* target_path = CharString_cat2( Path_getCondensed( Directory_getRealPath( target ) ), "/obj" );
		char* source_path = CharString_cat2( context->packageDir, "/source/java" );

		//{
		//	IDirectory* obj_dir = new_Directory( target_path );
		//	Directory_createAll( obj_dir );
		//	free_Directory( obj_dir );
		//}

		Command* command;
		IList*   arguments = new_List();
		IList*   native_arguments;
		
		// Compiler
		List_copyItem( arguments, context->compiler );
		
		// C-preprocessor flags
		//  addFlags( arguments, "-I", context->includeDirs );
		
		if ( !BuildParameters_isRelease( parameters ) )
		{
			List_copyItem( arguments, "-g" );
		}
		List_addList( arguments, BuildParameters_getGlobal( parameters )->JFLAGS );

		// Source files location
		List_copyItem( arguments, "-sourcepath" );
		List_copyItem( arguments, source_path );

		// Source files
		for ( i = 0; i < count; i++ )
		{
			List_copyItem( arguments, context->sourceFiles->items[i] );
		}
		
		// Output files
		List_copyItem( arguments, "-d" );
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
		free_CharString( target_path );
		free_CharString( source_path );
		//free_CharString( target_file );
		//free_Path( p );
	}
	return status;
}

static int private_jar( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	int status = 1;
	
	const BuildParameters* parameters = (BuildParameters*) buildParameters;
	const ProviderContext* context = (ProviderContext*) providerContext;
	const IDirectory*      target = (IDirectory*) targetIDirectory;

	const char* target_location = Path_getCondensed( Directory_getRealPath( target ) );
	
	//  jar cf <JARFILE> <CLASS_FILES>...

	char* libname          = CharString_cat2( context->package_name, ".jar" );
	char* manifest_fileloc = CharString_cat2( context->packageDir, "/source/java/META-INF/MANIFEST.MF" );
	char* full_bin_loc     = CharString_cat2( target_location, "/bin" );
	char* full_lib_loc     = CharString_cat2( target_location, "/lib" );
	char* full_bin_fileloc = CharString_cat3( target_location, "/bin/", libname );
	char* full_lib_fileloc = CharString_cat3( target_location, "/lib/", libname );
	char* obj_dir          = CharString_cat2( target_location, "/obj/" );

	bool include_manifest = 0;
	bool application = 0;
	{
		IFile* manifest = new_File( manifest_fileloc );
		if ( File_exists( manifest ) )
		{
			char* characters = NULL;
			include_manifest = 1;
			File_open( manifest, "r" );
			characters = File_readCharacters( manifest );
			File_close( manifest );
			if ( CharString_contains( characters, "Main-Class" ) )
			{
				application = 1;
			}
			free_CharString( characters );
		}
		free_File( manifest );
	}

	//	Create lib dir
	{
		IDirectory* bin_or_lib_dir = NULL;
		if ( application )
		{
			bin_or_lib_dir = new_Directory( full_bin_loc );
		} else {
			bin_or_lib_dir = new_Directory( full_lib_loc );
		}
		Directory_createAll( bin_or_lib_dir );
		free_Directory( bin_or_lib_dir );
	}

	if ( 0 < List_count( context->objectFiles ) )
	{
		Command* command;
		IList* arguments = new_List();
		IList* native_arguments;

		List_copyItem( arguments, context->archiver );

		if ( include_manifest )
		{
			List_copyItem( arguments, "cmf" );
			List_copyItem( arguments, manifest_fileloc );
		} else  {
			List_copyItem( arguments, "cf" );
		}

		//	Specify appropriate location for jar file depending on whether this is
		//	an application or not.
		if ( application )
		{
			List_copyItem( arguments, full_bin_fileloc );
		} else {
			List_copyItem( arguments, full_lib_fileloc );
		}
		
		{
			unsigned int i;
			unsigned int count = List_count( context->objectFiles );
			for ( i=0; i < count; i++ )
			{
				//char* jar_root = CharString_cat2( "-C ", obj_dir );
				//List_copyItem( arguments, jar_root );
				//List_copyItem( arguments, "-C" );
				//List_copyItem( arguments, obj_dir );
				List_copyItem( arguments, context->objectFiles->items[i] );
				//free_CharString( jar_root );
			}
		}

		{
			Path_ChangeCurrentDirectory( obj_dir );

			native_arguments = Path_ConvertListToNative( arguments );
			command = new_Command( context->archiver, (const char**) native_arguments->items );

			if ( !BuildParameters_isNo( parameters ) && Command_run( command ) && Command_waitFor( command ) )
			{
				fprintf( stdout, "%s", command->command );
				status &= Command_getResult( command );
			} else {
				fprintf( stderr, "failed: " );
				Command_print( command, stderr );
			}
			Path_RevertCurrentDirectory();
		}
		free_Command( command );
		free_List( native_arguments );
		free_List( arguments );
	}

	free_CharString( libname );
	free_CharString( manifest_fileloc );
	free_CharString( full_bin_loc );
	free_CharString( full_lib_loc );
	free_CharString( full_bin_fileloc );
	free_CharString( full_lib_fileloc );
	free_CharString( obj_dir );

	return status;
}

static int private_link( const void* buildParameters, const void* providerContext, const void* targetIDirectory )
{
	int status;
	const ProviderContext* context = (ProviderContext*) providerContext;

	status = private_jar( buildParameters, providerContext, targetIDirectory );

	return status;
}

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
	provider->info = "ISLabs JDK Sun (Sun Java Development Kit)";
	provider->vendor = "ISLabs";
	provider->name = "jdk_sun";
	provider->version = "0.1";
	provider->object = new_ToolSetProvider();	
	return provider;
}

Provider* CreateBuildProvider_islabs_jdk_sun()
{
	return GetInstance();
}
