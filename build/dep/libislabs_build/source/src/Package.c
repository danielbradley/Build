/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/_Defines.h"
#include "islabs/build/BuildParameters.h"
#include "islabs/build/Package.protected.h"
#include "islabs/build/ProviderContext.h"
#include "islabs/build/Suite.protected.h"
#include "islabs/build/providers/ToolSet.h"
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/CharStringList.h>
#include <openocl/base/psa/List.h>
#include <openocl/base/psa/Stack.h>
#include <openocl/base/psa/Time.h>
#include <openocl/io/psa/File.h>
#include <openocl/io/psa/Link.h>

static IList* RetrieveIncludeDirs( IList* list, const IDirectory* directory );

Package* new_Package( BuildManager* bm, const IDirectory* location )
{
       Package* self = (Package*) CRuntime_calloc( 1, sizeof( Package ) );
       Package_init( self, bm, location );
       return self;
}

void free_Package( Package* self )
{
       Package_final( self );
}

void Package_init( Package* self, BuildManager* bm, const IDirectory* location )
{
	const IDirectory* dep     = Directory_getCachedSubdirectory( location, DIR_DEP );
	//const IDirectory* lib     = Directory_getCachedSubdirectory( location, DIR_LIB );
	const IDirectory* testing = Directory_getCachedSubdirectory( location, DIR_TESTING );

	Buildable_init( &self->super, bm, location );
	Buildable_setType( &self->super, PACKAGE );
	self->super.free    = (void(*)(Buildable*)) free_Package;
	self->super.buildTo = (bool(*)(const Buildable*,const IDirectory*,const BuildParameters*,int)) Package_buildTo;
	self->free    = free_Package;
	self->buildTo = Package_buildTo;

	Buildable_setUpToDate( &self->super, Package_isUpToDate( self, location ) );

	if ( dep )
	{
		const ISet* libraries = Directory_getCachedSubdirectories( dep );
		if ( libraries )
		{
			unsigned int max = Set_count( libraries );
			unsigned int i;
			for ( i=0; i < max; i++ )
			{
				const IDirectory* lib_dir = (const IDirectory*) Set_get( libraries, i );
				const Buildable* library_package = Buildable_getInstance( self->super.bm, lib_dir );
				if ( library_package )
				{
					Buildable_addChild( (Buildable*) self, library_package );
				}
			}
		}
	}

	if ( testing )
	{
		self->testSuite = Buildable_getInstance( self->super.bm, testing );
	}
}

void Package_final( Package* self )
{
       Buildable_final( &self->super );
       CRuntime_free( self );
}

bool Package_buildTo( const Package* self, const IDirectory* target, const BuildParameters* parameters, int depth )
{
	bool status = 1;

#ifdef DEBUG_MEMORY
	fprintf( stderr, "Package_buildTo: >> %lli\n", CRuntime_GetAllocationCount() );
	{
#endif
		status &= Package_buildChildren( self, target, parameters, depth - 1 );

		if ( (depth >= 0) && status )
		{
			if ( BuildParameters_isBuildPackages( parameters ) && (!Buildable_isBuilt( (const Buildable*) self ) || BuildParameters_isFull( parameters )) )
			{
				//fprintf( stderr, "\tPackage_buildTo: >>> %lli\n", CRuntime_GetAllocationCount() );
				status &= Package_buildPackage( self, target, parameters );
				//fprintf( stderr, "\tPackage_buildTo: <<< %lli\n", CRuntime_GetAllocationCount() );
				Buildable_setBuilt( (Buildable*) self, 1 );
			}
			if ( self->testSuite && (BuildParameters_isBuildTests( parameters ) || BuildParameters_isRunTests( parameters )) )
			{
				const IDirectory* test_suite_target = Directory_getCachedSubdirectory( target, DIR_TESTING );
				if ( !test_suite_target )
				{
					test_suite_target = Directory_getCachedSubdirectory_new( target, DIR_TESTING );
					Directory_createAll( test_suite_target );
				}
				status &= Suite_buildTo( (Suite*) self->testSuite, test_suite_target, parameters, depth );
			}
		}
#ifdef DEBUG_MEMORY
	}
	fprintf( stderr, "Package_buildTo: << %lli\n", CRuntime_GetAllocationCount() );
#endif
	return status;
}

bool Package_buildChildren( const Package* self, const IDirectory* target, const BuildParameters* parameters, int depth )
{
	bool status = 1;

	const char* package_location = Path_getCondensed( Directory_getRealPath( self->super.directory ) );
	const char* target_location  = Path_getCondensed( Directory_getRealPath( target ) );

	if ( 0 == CharString_compare( package_location, target_location ) )
	{	
		unsigned int i;
		unsigned int count = Set_count( self->super.children );
		for ( i=0; i < count; i++ )
		{
			Buildable* b = (Buildable*) Set_get( self->super.children, i );
			if ( b->directory && status )
			{
				const IDirectory* next_child_target = b->directory;
				status &= Buildable_buildTo( b, next_child_target, parameters, depth );
			}
		}
	} else {
		const IDirectory* target_lib = Directory_getCachedSubdirectory( target, DIR_LIB );
		unsigned int i;
		unsigned int count = Set_count( self->super.children );
		
		if ( !target_lib )
		{
			target_lib = Directory_getCachedSubdirectory_new( target, DIR_LIB );
		//	Directory_createAll( target_lib );
		}
		
		for ( i=0; i < count; i++ )
		{
			Buildable* b = (Buildable*) Set_get( self->super.children, i );
			const char* package_name = Buildable_getName( b );
			const IDirectory* target_lib_package = Directory_getCachedSubdirectory( target_lib, package_name );
			if ( !target_lib_package )
			{
				target_lib_package = Directory_getCachedSubdirectory_new( target_lib, package_name );
			//	Directory_createAll( target_lib_package );
			}

			if ( b->directory )
			{
				status &= Buildable_buildTo( b, target_lib_package, parameters, depth );
			}
		}
	}
	return status;
}

bool Package_buildPackage( const Package* self, const IDirectory* target, const BuildParameters* parameters )
{
	bool status = 1;

#ifdef DEBUG_MEMORY
	fprintf( stderr, "\tPackage_buildPackage: >> %lli\n", CRuntime_GetAllocationCount() );
	{
#endif

	const IDirectory* target_bin = Directory_getCachedSubdirectory_new( target, "bin" );
///
	const IDirectory* target_dep = Directory_getCachedSubdirectory_new( target, "dep" );
	const IDirectory* target_obj = Directory_getCachedSubdirectory_new( target, "obj" );
	const IDirectory* target_lib = Directory_getCachedSubdirectory_new( target, "lib" );
	const IDirectory* dep        = Directory_getCachedSubdirectory( self->super.directory, "dep" );
	const IDirectory* source     = Directory_getCachedSubdirectory( self->super.directory, "source" );

	const char*  ostype            = BuildParameters_getOSType( parameters );
	const char** platform_subparts = BuildParameters_getPlatformSubparts( parameters );
	//char*        compiler          = CharString_basename( BuildParameters_getGlobal( parameters )->CC );

	const char*  package_location  = Path_getCondensed( Directory_getRealPath( self->super.directory ) );
	const char*  target_location   = Path_getCondensed( Directory_getRealPath( target ) );

	ProviderContext* context       = new_ProviderContext( parameters, package_location, target_location );

	IList* depthfirst_lib_directories;
	IList* dependency_ordered_lib_list;

////
//	fprintf( stderr, "\t\tPackage_buildTo: >>> %lli\n", CRuntime_GetAllocationCount() );
//	{

	if ( !Buildable_isUpToDate( &self->super ) )
	{
		BuildParameters_setForce( (BuildParameters*) parameters, 1 );
	}

	//--------------------------------------------------------------------------
	//	Obtain references to (or create) necessary directories
	//

	if ( dep && Directory_exists( dep ) )
	{
		if ( !Directory_exists( target_dep ) )
		{
			Directory_createAll( target_dep );
		}
	}

	if ( context->isLib )
	{
		if ( !Directory_exists( target_lib ) )
		{
			Directory_createAll( target_lib );
		}
	}
	else
	{
		if ( !Directory_exists( target_bin ) )
		{
			Directory_createAll( target_bin );
		}
	}


	if ( !target_obj )
	{
		target_obj = Directory_getCachedSubdirectory_new( target, "obj" );
	}
	if ( !Directory_exists( target_obj ) )
	{
		Directory_createAll( target_obj );
	}

	//
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	//	Determine library dependencies
	//
	//	o  First searchs the target library directory if it exists
	//	o  else searches the package library directory
	//
	//	Search both handles the situation where you are building into a
	//	different target from the source.

	if ( 0 != CharString_compare( package_location, target_location ) )
	{
		if ( target_dep )
		{
			depthfirst_lib_directories = Directory_searchCached_depthFirst_followLinks( target_dep, DIR_LIB, 1 );
		} else {
			depthfirst_lib_directories = new_List();
		}
	}
	else if ( 0 == CharString_compare( package_location, target_location ) )
	{
		//	XXX	I would really like this to use Directory_searchCached_shallow_followLinks
		//		as the the binary should only like to its immediate dependencies. However,
		//		for some reason during linking the dependencies of dependencies are not being
		//		resolved properly unless EVERY library is indicated.
		//
		//		Hmmm, also this wouldn't work for monolithic executables.
		
		if ( dep )
		{
			depthfirst_lib_directories = Directory_searchCached_depthFirst_followLinks( dep, DIR_LIB, 1 );
		} else {
			depthfirst_lib_directories = new_List();
		}
		//List_print( depthfirst_lib_directories );
	} else {
		if ( dep )
		{
			depthfirst_lib_directories = Directory_searchCached_depthFirst_followLinks( dep, DIR_LIB, 1 );
		} else {
			depthfirst_lib_directories = new_List();
		}
	}

	//
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	//	Reorder: We need to rearrange the list of libraries so that they are
	//	in dependency order (necessary when building static executables).
	//	
	//	Reduce: This also removes any directories underneath "testing",
	//	"examples", and "profiling".
	// 

	if ( 1 )
	{
		dependency_ordered_lib_list = Package_ReorderAndReduceLibList( depthfirst_lib_directories );
	} else {
		dependency_ordered_lib_list = new_List_items( depthfirst_lib_directories );
		List_addList( dependency_ordered_lib_list, depthfirst_lib_directories );
	}

	//
	//--------------------------------------------------------------------------

	
//	fprintf( stdout, "buildable: %s\n", package_location );
//	fprintf( stdout, "buildable: %s\n\n", target_location );

	if ( dependency_ordered_lib_list )
	{
		Package_DetermineDependencies( context, (IList*) dependency_ordered_lib_list );
	}

	//
	//	If there is a "<PACKAGE>/lib" directory search its subdirectories (and links)
	//	for "include" directories.
	//
	if ( dep )
	{
		IList* list = RetrieveIncludeDirs( NULL, dep );
		if ( self->includePrivateHeaders )
		{
			Package_AddPrivateHeaderDirs( list );
		} 
		List_addList( context->includeDirs, list );
		free_List( list );
	}
	List_copyItem2( context->includeDirs, package_location, "/include" );
	List_copyItem2( context->includeDirs, package_location, "/source/include" );

	if ( source )
	{
		const IDirectory* java         = Directory_getCachedSubdirectory( source, SRC_DIR_JAVA );
		const IDirectory* csharp       = Directory_getCachedSubdirectory( source, SRC_DIR_CSHARP );
		const IDirectory* c            = Directory_getCachedSubdirectory( source, SRC_DIR_C );
		const IDirectory* cplusplus    = Directory_getCachedSubdirectory( source, SRC_DIR_CPLUSPLUS );
		const IDirectory* objcplusplus = Directory_getCachedSubdirectory( source, SRC_DIR_OBJECTIVE_CPLUSPLUS );
		
//		if ( java )
//		{
//			IList* search_results = Directory_searchCached_depthFirst_filter_followLinks( java, ".java", 0 );
//
//			unsigned int count = List_count( search_results );
//			unsigned int i;
//
//			context->language = "Java";
//
//			for ( i=0; i < count; i++ )
//			{
//				const char* location = search_results->items[i];
//				IFile* file = new_File( location );
//				
//				const IPath* path = File_getPath( file );		//	source/java/mypackage/MyObject.java
//				bool isJava = Path_hasExtension( path, ".java" );
//
//				if ( isJava )
//				{
//					const char* dirname = CharString_dirname( location );
//					const char* filename = CharString_basename( location );
//					
//					char* class_prefix   = CharString_token( filename, 0, '.' );
//					char* class_name     = CharString_cat2( class_prefix, ".class" );
//					char* rel_class_loc  = CharString_cat3( dirname, "/", class_name );
//					char* class_location = CharString_cat3( context->packageDir, "/", rel_class_loc );
//					char* file_location  = CharString_cat3( context->packageDir, "/source/java/", location );
//
//					IFile* target_class = new_File( class_location );
//					
//					//	If the source file is newer than the object file the file
//					//	is added to the list of files to compile.
//					
//					if ( BuildParameters_isForce( parameters ) || self->forceBuild || !File_exists( target_class ) || File_isNewerThan( file, target_class ) )
//					{
//						if ( BuildParameters_getVerbosity( parameters ) )
//						{
//							fprintf( stderr, "queueing: %s\n", rel_class_loc );
//						}
//						List_copyItem( context->sourceFiles, file_location );
//					}
//					
//					//	The object file name is always added to objectFiles list as it
//					//	specifies what objects to include in the final library/executable.
//					
//					List_copyItem( context->objectFiles, rel_class_loc );
//					
//					free_File( target_class );
//					free_CharString( class_prefix );
//					free_CharString( class_name );
//					free_CharString( rel_class_loc );
//					free_CharString( class_location );
//					free_CharString( file_location );
//				}
//				free_File( file );
//			}
//			
//			free_List( search_results );
//		}
//		else if ( cs )
//		{
//			IList* search_results = Directory_searchCached_depthFirst_filter_followLinks( cs, ".cs", 0 );
//
//			unsigned int count = List_count( search_results );
//			unsigned int i;
//
//			context->language = "CSharp";
//
//			for ( i=0; i < count; i++ )
//			{
//				const char* location = search_results->items[i];
//				IFile* file = new_File( location );
//
//				const IPath* path = File_getPath( file );		//	source/java/mypackage/MyObject.java
//				bool isCSharp = Path_hasExtension( path, ".cs" );
//
//				if ( isCSharp )
//				{
//					const char* dirname = CharString_dirname( location );
//					const char* filename = CharString_basename( location );
//					
//					char* class_prefix   = CharString_token( filename, 0, '.' );
//					char* class_name     = CharString_cat2( class_prefix, ".netmodule" );
//					char* rel_class_loc  = CharString_cat3( dirname, "/", class_name );
//					char* class_location = CharString_cat3( context->packageDir, "/", rel_class_loc );
//					char* file_location  = CharString_cat3( context->packageDir, "/source/cs/", location );
//
//					IFile* target_class = new_File( class_location );
//					
//					//	If the source file is newer than the object file the file
//					//	is added to the list of files to compile.
//					
//					if ( BuildParameters_isForce( parameters ) || self->forceBuild || !File_exists( target_class ) || File_isNewerThan( file, target_class ) )
//					{
//						if ( BuildParameters_getVerbosity( parameters ) )
//						{
//							fprintf( stderr, "queueing: %s\n", rel_class_loc );
//						}
//						List_copyItem( context->sourceFiles, file_location );
//					}
//					
//					//	The object file name is always added to objectFiles list as it
//					//	specifies what objects to include in the final library/executable.
//					
//					List_copyItem( context->objectFiles, rel_class_loc );
//					
//					free_File( target_class );
//					free_CharString( class_prefix );
//					free_CharString( class_name );
//					free_CharString( rel_class_loc );
//					free_CharString( class_location );
//					free_CharString( file_location );
//				}
//				free_File( file );
//			}
//			
//			free_List( search_results );
//		}
		if ( c || cplusplus || objcplusplus )
		{
			const IDirectory* src = NULL;
			
			     if ( c            ) src = c;
			else if ( cplusplus    ) src = cplusplus;
			else if ( objcplusplus ) src = objcplusplus;
		
			const ISet* src_files = Directory_getCachedFiles( src );
			unsigned int count = Set_count( src_files );
			unsigned int i;
			for ( i=0; i < count; i++ )
			{
				const IFile* file = (const IFile*) Set_get( src_files, i );
				bool isC           = Path_hasExtension( File_getPath( file ), ".c" );
				bool isC_PLUS_PLUS = Path_hasExtension( File_getPath( file ), ".cpp" );
				bool isO_PLUS_PLUS = Path_hasExtension( File_getPath( file ), ".mm" );
				if ( isC || isC_PLUS_PLUS || isO_PLUS_PLUS )
				{
					const IPath* file_path     = File_getPath( file );
					const char*  file_location = Path_getCondensed( file_path );
					char*        file_base_o = CharString_cat2( Path_getBasename( file_path ), ".obj" );
					const IFile* target_object = Directory_getCachedFile( target_obj, file_base_o );
					
					//	If the source file is newer than the object file the file
					//	is added to the list of files to compile.
					
					if ( BuildParameters_isForce( parameters ) || self->forceBuild || !target_object || File_isNewerThan( file, target_object ) )
					{
						if ( BuildParameters_getVerbosity( parameters ) )
						{
							fprintf( stderr, "queueing: %s\n", file_base_o );
						}
						List_copyItem( context->sourceFiles, file_location );
					}
					
					//	The object file name is always added to objectFiles list as it
					//	specifies what objects to include in the final library/executable.
					
					List_addItem( context->objectFiles, file_base_o );
					if ( isC_PLUS_PLUS )
					{
							context->language = "C++";
					}
					else if ( isO_PLUS_PLUS )
					{
							context->language = "O++";
					}
					else if ( !context->language && isC )
					{
						context->language = "C";
					}
				}
			}

			//----------------------------------------------------------------------------
			//	The following section has been depreciated in favour of the
			//	PLATFORM method of determining platform specific adapters

			if ( 0 ) //ostype )
			{
				const IDirectory* psa = Directory_getCachedSubdirectory( src, ostype );
				if ( psa )
				{
					const ISet* psa_files = Directory_getCachedFiles( psa );
					unsigned int count = Set_count( psa_files );
					unsigned int i;
					for ( i=0; i < count; i++ )
					{
						const IFile* file = (const IFile*) Set_get( psa_files, i );
						bool isC           = Path_hasExtension( File_getPath( file ), ".c" );
						bool isC_PLUS_PLUS = Path_hasExtension( File_getPath( file ), ".cpp" );
						if ( isC || isC_PLUS_PLUS )
						{
							const IPath*  file_path     = File_getPath( file );
							const char*  file_location = Path_getCondensed( file_path );
							char*        file_base_o = CharString_cat2( Path_getBasename( file_path ), ".obj" );

							List_addItem( context->objectFiles, file_base_o );
							List_copyItem( context->sourceFiles, file_location );
							
							if ( isC_PLUS_PLUS )
							{
								context->language = "C++";
							} else if ( !context->language && isC ) {
								context->language = "C";
							}
						}
					}
				}
				
			}

			//
			//----------------------------------------------------------------------------
			
			if ( platform_subparts )
			{
				unsigned int nr_of_platform_directories = CharStringList_getLength( platform_subparts );

				for ( i=0; i < nr_of_platform_directories; i++ )
				{
					const IDirectory* psa = Directory_getCachedSubdirectory( src, platform_subparts[i] );
					
					if ( psa )
					{
						const ISet* psa_files = Directory_getCachedFiles( psa );
						unsigned int count = Set_count( psa_files );
						unsigned int i;
						for ( i=0; i < count; i++ )
						{
							const IFile* file = (const IFile*) Set_get( psa_files, i );
							bool isC           = Path_hasExtension( File_getPath( file ), ".c" );
							bool isC_PLUS_PLUS = Path_hasExtension( File_getPath( file ), ".cpp" );
							if ( isC || isC_PLUS_PLUS )
							{
								const IPath*  file_path     = File_getPath( file );
								const char*  file_location = Path_getCondensed( file_path );
								char*        file_base_o = CharString_cat2( Path_getBasename( file_path ), ".obj" );
								const IFile* target_object = Directory_getCachedFile( target_obj, file_base_o );
					
								//	If the source file is newer than the object file the file
								//	is added to the list of files to compile.
					
								if ( BuildParameters_isForce( parameters ) || self->forceBuild || !target_object || File_isNewerThan( file, target_object ) )
								{
									if ( BuildParameters_getVerbosity( parameters ) )
									{
										fprintf( stderr, "queueing: %s\n", file_base_o );
									}
									List_copyItem( context->sourceFiles, file_location );
								}

								//	The object file name is always added to objectFiles list as it
								//	specifies what objects to include in the final library/executable.

								List_addItem( context->objectFiles, file_base_o );

								if ( isC_PLUS_PLUS )
								{
									context->language = "C++";
								} else if ( !context->language && isC ) {
									context->language = "C";
								}
							}
						}
					}
				}
			}
			
		}
	}

	if ( context->language )
	{
		if ( 0 == CharString_compare( context->language, "C" ) )
		{
			context->compiler = BuildParameters_getGlobal( parameters )->CC;
			context->linker   = BuildParameters_getGlobal( parameters )->LD;
			context->archiver = BuildParameters_getGlobal( parameters )->AR;
		}
		else if ( 0 == CharString_compare( context->language, "C++" ) )
		{
			context->compiler = BuildParameters_getGlobal( parameters )->C_PLUS_PLUS;
			context->linker   = BuildParameters_getGlobal( parameters )->LD;
			context->archiver = BuildParameters_getGlobal( parameters )->AR;
		}
		else if ( 0 == CharString_compare( context->language, "O++" ) )
		{
			context->compiler = "clang";//BuildParameters_getGlobal( parameters )->C_PLUS_PLUS;
			context->linker   = BuildParameters_getGlobal( parameters )->LD;
			context->archiver = BuildParameters_getGlobal( parameters )->AR;
		}
		else if ( 0 == CharString_compare( context->language, "Java" ) )
		{
			context->compiler = BuildParameters_getGlobal( parameters )->JAVAC;
			context->linker   = BuildParameters_getGlobal( parameters )->JAVAC;
			context->archiver = BuildParameters_getGlobal( parameters )->JAR;
		}
		else if ( 0 == CharString_compare( context->language, "CSharp" ) )
		{
			context->compiler = "mcs"; //BuildParameters_getGlobal( parameters )->JAVAC; // Not really needed??
			context->linker   = BuildParameters_getGlobal( parameters )->JAVAC; // Not really needed??
			context->archiver = BuildParameters_getGlobal( parameters )->JAR;   // Not really needed??
		}

		if ( 1 < BuildParameters_getVerbosity( parameters ) )
		{
			ProviderContext_print( context );
		}

		{
			IPath* compiler_path = new_Path( context->compiler );
			const char* compiler_base = Path_getBasename( compiler_path );
			const ToolSet* toolset = ProviderRegistry_toolSet( BuildParameters_getGlobal( parameters )->registry, ostype, compiler_base );
			if ( toolset )
			{
				fprintf( stdout, "%s", target_location );
				fprintf( stdout, " (%s on %s)", compiler_base, ostype );
				
				if ( BuildParameters_isForce( parameters ) || self->forceBuild )
				{
					fprintf( stdout, " (forced)\n" );
				} else {
					fprintf( stdout, "\n" );
				}
				status &= ToolSet_compile( toolset, parameters, context, target );
				if ( status )
				{
					status &= ToolSet_link( toolset, parameters, context, target );
				}
			} else {
				fprintf( stderr, "build: could not find toolset provider for %s on %s\n", compiler_base, ostype );
			}
			free_Path( compiler_path );
		}
	}
	free_ProviderContext( context );
	free_List( depthfirst_lib_directories );
	free_List( dependency_ordered_lib_list );
///

#ifdef DEBUG_MEMORY
	}
	fprintf( stderr, "\tPackage_buildPackage: << %lli\n", CRuntime_GetAllocationCount() );
#endif
	return status;
}

IList* Package_ReorderAndReduceLibList( const IList* libs )
{
	IStack* stack = new_Stack();
	IList* blist = new_List();
	
	if ( stack )
	{
		unsigned int max = List_count( libs );
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			if ( CharString_contains( libs->items[i], "testing" ) ) continue;
			if ( CharString_contains( libs->items[i], "examples" ) ) continue;
			if ( CharString_contains( libs->items[i], "profiling" ) ) continue;

			Stack_copyItem( stack, libs->items[i] );
		}

		if ( blist )
		{
			char* item;
			while ( (item = Stack_pop( stack )) )
			{
				List_addItem( blist, item );
			}
		}
		free_Stack( stack );
	}

	return blist;
}

/*
 *	The input to this function is a list of library dependency directories
 *	eg	./lib/libopenocl_util-1.0.1/lib
 *		./lib/libopenocl_util-1.0.1/lib/libopenocl_base-1.0.1/lib
 *
 *	The simple thing to do is just concatenate the "-L" to the front and return
 *	a really, really big list. Unfortunately when we try this under Windows we
 *	get a command line too long error :<.
 *
 *	Luckily we only really need to return the set of library dirs that contain
 *	all required libraries, not every single library dir. Therefore if we keep
 *	a list of added libraries, we can simply add a library dir when we encounter
 *	a new library. However, we are in effect now doing this in
 *	determineLibs/determineLibFlags, therefore all we really need to do is add
 *	the directory there when a new -l<library> is encountered, making this function
 *	redundant.
 *
 *	void
 *	determineLibDirs( CurrentContext* current, const List* dep_list )
 *	{
 *		unsigned int i = 0;
 *		unsigned int count = List_count( dep_list );
 *		for ( i=0; i < count; i++ )
 *		{
 *			char* lib_dir = String_cat3( current->target_dir, "/lib/", dep_list->items[i] );
 *			List_copyItem2( current->FULL_LDFLAGS, "-L", lib_dir );
 *			free( lib_dir );
 *		}
 *		char* lib_dir = String_cat( current->package_dir, "/source" );
 *		List_copyItem3( current->FULL_LDFLAGS, "-L", lib_dir, "/lib" );
 *		free( lib_dir );
 *	}
 */

/*
 *	This function searches each directory in the list dep_list for
 *	libraries that are to be linked.
 *
 *	The dep list is relative build's current directory so it could potentially
 *	be relative to anywhere.
 *
 *	eg.	test-build/lib/libopenocl_base_psa-1.0
 *		../checkouts/openocl-1.0/libopenocl_io_psa-1.0
 *
 *	This is ok for the standard flags -l<lib>, which only takes a library name,
 *	and -L<dir>, which takes a directory path relative to the compiler's current
 *	directory (same as build).
 *
 *	However, for dynamic linkage -Wl,-R$ORIGIN, these paths need to be made
 *	relative to lib directory.
 *
 *	current->target_dir				test-build
 *
 */
void Package_DetermineDependencies( ProviderContext* context, const IList* dep_list )
{
	IPath* tmp_path;
	unsigned int i = 0;
	unsigned int count = List_count( dep_list );
	for ( i=0; i < count; i++ )
	{
		char* relative = NULL;
		tmp_path = new_Path( dep_list->items[i] );
		
		relative = Path_relativeTo( tmp_path, context->targetDir );
		Package_DetermineLibraries( context, dep_list->items[i], relative );
		free_Path( tmp_path );
		free_CharString( relative );
	}

	{
		char* lib_dir = CharString_cat2( context->packageDir, "/source/lib" );
		//	printf( "Checking if directory: %s\n", lib_dir );
		Package_DetermineLibraries( context, lib_dir, NULL );
		free( lib_dir );
	}

	//	printf( "Have detected the following libs\n" );
	//	List_print( current->FULL_LIBS );

}

void Package_DetermineLibraries( ProviderContext* context, const char* lib_dir, const char* relative_lib_dir )
{
	//	For every lib directory found, this function analyses each file in that directory.
	//	If the file is a library: *.a *.lib *.so *.dll it will add it to the appropriate
	//	list.
	//
	//	If it is a link file it will check to see if it points to a library file, in which
	//	case it will add it as a system file. Or to a directory, in which case it will ignore
	//	it.

	IDirectory* lib = new_Directory( lib_dir );

	if ( Directory_refresh( lib ) )
	{
		const char* nix_so = ".so";
		const IList* file_list = Directory_getFileList( lib );
		unsigned int count = List_count( file_list );
		unsigned int i;
		for ( i=0; i < count; i++ )
		{
            		char* file     = CharString_copy( file_list->items[i] );
			char* file_dir = NULL;	// Used to add lib dirs for system libraries

			//-------------------------------------------------------------------------
			//	The following section is crudely used to skip system libraries that
			//	are only relevant to a particular platform;
			//
			//	eg. <pkg>/source/lib/libdl.so.link --> /usr/libdl.so
			//
			//	-ldl will cause an error under windows as libdl doesn't exist,
			//	therefore when a link is encountered that doesn't point to a valid
			//	object then it is skipped.
			//
			//	However, there will be a problem when the library exists but does
			//	not need to be linked on all platforms eg. libm, libsocket.
			
			if ( CharString_endsWith( file, ".link" ) )
			{
				bool link_exists = 0;
				bool link_to_dir = 0;

				char* file_path = CharString_cat3( lib_dir, "/", file );
				ILink* ilink = new_Link();
			
				if ( Link_load( ilink, file_path ) )
				{
					const char* target = Link_getTarget( ilink );
					IFile* target_file = new_File( target );
					link_exists = File_exists( target_file );
					if ( OPENOCL_IO_PSA_TYPE_FS_DIRECTORY == Path_getType( File_getPath( target_file ) ) )
					{
						link_to_dir = 1;
					}

					//	If the link doesn't point to a directory, it should be a system library.
					//	Is this true??

					if ( link_exists && !link_to_dir )
					{
						const IPath* path = File_getPath( target_file );
						const char* dirname = Path_getDirname( path );

						//	fprintf( stdout, "Target not dir: %s\n", Path_getExtension( path ) );

						file = CharString_copy( Path_getBasename( path ) );
						file_dir = CharString_copy( dirname );

						if ( 0 == CharString_compare( Path_getExtension( path ), "dylib" ) )
						{
							nix_so = ".dylib";
						}
					}
					//	else
					//	{
					//		fprintf( stdout, "Target exists: %s\n", target );
					//	}
					free_File( target_file );
				}
				free_Link( ilink );
				free_CharString( file_path );

				if ( !link_exists )
				{
					continue;
				}
			}

			//
			//-------------------------------------------------------------------------

			if ( '.' != file[0] )
			{
				char* libname1 = CharString_between( file, "", ".dll" );	// Windows DLL
				char* libname2 = CharString_between( file, "lib", nix_so );	// *nix Shared object
				char* libname3 = CharString_between( file, "lib", ".a" );	// Unix object library (archive)
				char* libname4 = CharString_between( file, "lib", ".lib" );	// Windows object library (archive)
				const char* flag = NULL;
				char  fs[2] = { Path_GetFileSeparator(), '\0'};
				char* native_path = CharString_cat3( Path_getNativeFormat( Directory_getPath( lib ) ), fs, file );

				if ( libname1 )
				{
					flag = libname1;
					
					if ( relative_lib_dir )
					{
						List_addItemIfNew( context->dll_libraries, CharString_copy( native_path ) );
					} else {
						List_addItemIfNew( context->system_libraries, CharString_copy( libname1 ) );
					}
				}
				else if ( libname2 )
				{
					flag = libname2;

					if ( relative_lib_dir && !file_dir )
					{
						//	XXX is this test needed??
						//if ( !CharString_endsWith( file, ".so" ) )
						//{
							List_addItemIfNew( context->elf_libraries, CharString_copy( native_path ) );
						//}
					} else {
						List_addItemIfNew( context->system_libraries, CharString_copy( libname2 ) );
					}
				}
				else if ( libname3 )
				{
					List_addItemIfNew( context->unix_archives, CharString_copy( native_path ) );
				}
				else if ( libname4 )
				{
					List_addItemIfNew( context->windows_archives, CharString_copy( native_path ) );
				}
				
				if ( flag && file_dir )
				{
					List_addItemIfNew( context->systemLibraryDirs, CharString_copy( file_dir ) );
				}
				
				if( flag && List_addItemIfNew( context->libraries, CharString_copy( flag ) ) )
				{
					List_addItemIfNew( context->libraryDirs, CharString_copy( lib_dir ) );

					//	"Only add to runtime dir list if we are searching non system dependency directory ie."
					//	"not ./source/lib, and not if it is a static library, ie not if libname3 is non-null."
					if ( relative_lib_dir && !(libname3 || libname4) )
					{
						List_addItemIfNew( context->runtimeLibraryDirs, CharString_copy( relative_lib_dir ) );
					}
				}

				if ( native_path ) free_CharString( native_path );
				if ( libname1 )    free_CharString( libname1 );
				if ( libname2 )    free_CharString( libname2 );
				if ( libname3 )    free_CharString( libname3 );
				if ( libname4 )    free_CharString( libname4 );
			}

			if ( file_dir ) free_CharString( file_dir );
			if ( file )     free_CharString( file );
		}
	}
	free_Directory( lib );

	//	printf( "Have detected the following libs\n" );
	//	List_print( current->FULL_LIBS );
}

void Package_AddPrivateHeaderDirs( IList* includeDirsList )
{
	IList* privateIncludeDirs = new_List();
	unsigned int count = List_count( includeDirsList );
	unsigned int i = 0;
	
	for ( i=0; i < count; i++ )
	{
		char* inc = CharString_cat2( includeDirsList->items[i], "/../source/include" );
		List_addItem( privateIncludeDirs, inc );
	}
	
	List_addList( includeDirsList, privateIncludeDirs );
	free_List( privateIncludeDirs );
}

bool Package_isUpToDate( Package* self, const IDirectory* location )
{
	bool up2date = 0;

	IList* modified_headers = new_List();

	const IDirectory* headers = Directory_getCachedSubdirectory( location, "include" );
	const IDirectory* objects = Directory_getCachedSubdirectory( location, "obj" );
	const IDirectory* source  = Directory_getCachedSubdirectory( location, "source" );

	ITime* epoch = new_Time( 0 );
	ITime* now   = Time_Now();
	
	unsigned long long newest_header         = Time_getSecondsSinceEpoch( epoch );
	unsigned long long newest_private_header = Time_getSecondsSinceEpoch( epoch );
	unsigned long long oldest_obj            = Time_getSecondsSinceEpoch( now );
	
	int newest_header_index;
	int oldest_object_index;
	
	unsigned long long temp;

	if ( objects )
	{
		IList* obj_list    = Directory_search( (IDirectory*) objects, "*.obj", 1 );

		int count = List_count( obj_list );
		int i;

		for ( i=0; i < count; i++ )
		{
			IFile* obj = new_File( obj_list->items[i] );
			const ITime* time = File_getLastModification( obj );

			// fprintf( stderr, "%20s: %s\n", Path_getBasename( File_getPath( obj ) ), Time_getTextual( time ) );
		
			if ( (temp = Time_getSecondsSinceEpoch( time )) < oldest_obj )
			{
				oldest_object_index = i;
				oldest_obj = temp;
			}

			free_File( obj );
		}
		free_List( obj_list );
	}

	if ( headers )
	{
		IList* header_list = Directory_search( (IDirectory*) headers, "*.h", 1 );

		int count = List_count( header_list );
		int i;
		
		for ( i=0; i < count; i++ )
		{
			IFile* header = new_File( header_list->items[i] );
			const ITime* time = File_getLastModification( header );
		
			// fprintf( stderr, "%20s: %s\n", Path_getBasename( File_getPath( header ) ), Time_getTextual( time ) );
		
			if ( newest_header < (temp = Time_getSecondsSinceEpoch( time )) )
			{
				newest_header_index = i;
				newest_header = temp;
			}

			if ( temp > oldest_obj )
			{		
				List_copyItem( modified_headers, Path_getNativeFormat( File_getPath( header ) ) );
			}

			free_File( header );
		}
		free_List( header_list );
	}

	if ( source )
	{
		const IDirectory* private_headers = Directory_getCachedSubdirectory_new( source, "include" );
		if ( private_headers )
		{
			IList* private_headers_list = Directory_search( (IDirectory*) private_headers, "*.h", 1 );
			int count = List_count( private_headers_list );
			int i;
			
			//  fprintf( stderr, "Headers:%s\n", Path_getAbsolute( Directory_getPath( private_headers ) ) ); 
			//  List_print( private_headers_list );
		
			for ( i=0; i < count; i++ )
			{
				IFile* header = new_File( private_headers_list->items[i] );
				const ITime* time = File_getLastModification( header );
	
				if ( newest_private_header < (temp = Time_getSecondsSinceEpoch( time )) )
				{
					newest_private_header = temp;
				}
				if ( temp > oldest_obj )
				{		
					List_copyItem( modified_headers, Path_getNativeFormat( File_getPath( header ) ) );
				}
				free_File( header );
			}
			free_List( private_headers_list );
		}
	}


	//fprintf( stderr, "Newest header: %s\n", Time_getTextual( newest_header ) );
	//fprintf( stderr, "Oldest object: %s\n", Time_getTextual( oldest_obj ) );

	{
		bool public_headers_modified;
		bool private_headers_modified;
	
		self->timeOfLastPublicHeaderModified  = newest_header;
		self->timeOfLastPrivateHeaderModified = newest_private_header;
		self->timeOfOldestObjectFileModified  = oldest_obj;

		public_headers_modified  = self->timeOfLastPublicHeaderModified  > self->timeOfOldestObjectFileModified;
		private_headers_modified = self->timeOfLastPrivateHeaderModified > self->timeOfOldestObjectFileModified;

		if ( public_headers_modified )
		{
			fprintf( stderr, "%s: public headers modified\n", Path_getNativeFormat( Directory_getPath( location ) ) );
			self->forceBuild = 1;
		}
		if ( private_headers_modified )
		{
			fprintf( stderr, "%s: private headers modified\n", Path_getNativeFormat( Directory_getPath( location ) ) );
			self->forceBuild = 1;
		}
		List_print( modified_headers );
		
		up2date = !public_headers_modified;
	}

	free_Time( epoch );
	free_Time( now );

	free_List( modified_headers );
	
	return up2date;
}

IList*
RetrieveIncludeDirs( IList* list, const IDirectory* dir )
{
	const ISet* subdirs = Directory_getCachedSubdirectories( dir );
	unsigned int count = Set_count( subdirs );
	unsigned int i;
	if ( NULL == list )
	{
		list = new_List();
	}
	for ( i=0; i < count; i++ )
	{
		const IDirectory* subdir = (const IDirectory*) Set_get( subdirs, i );
		const IPath* path = Directory_getRealPath( subdir );
		const char* name = Path_getBasename( path );
		
		if ( CharString_startsWith( name, PREFIX_LIB ) ) // If starts with lib eg. libsuperman
		{
			if ( Directory_containsDirectory( subdir, "include" ) )
			{
				List_addItem( list, CharString_cat3( Path_getCondensed( path ), "/", "include" ) );
			}
		}
		else
		{
			list = RetrieveIncludeDirs( list, subdir );
		}
	}
	
	return list;
	
//	return Directory_searchCached_shallow_followLinks( package_lib, "include", 1 );
}
