/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/ProviderContext.h"
#include "islabs/build/providers/ToolSetProvider.h"

#include <openocl/base/psa/CharString.h>
#include <openocl/io/psa/Path.h>
#include <openocl/provider/psa/Provider.h>

//#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char*
packageDirToPackageName( const char* package_dir );


ProviderContext*
new_ProviderContext( const BuildParameters* parameters, const char* package_dir, const char* target_dir )
{
	ProviderContext* self = (ProviderContext*) calloc( 1, sizeof( ProviderContext ) );
	self->parameters  = parameters;
	self->packageDir = CharString_copy( package_dir );
	self->targetDir  = CharString_copy( target_dir );

	self->package_name = packageDirToPackageName( package_dir );
	self->isLib = CharString_startsWith( self->package_name, "lib" );

	self->FULL_CPPFLAGS = new_List();
	self->FULL_CFLAGS   = new_List();
	self->FULL_LFLAGS   = new_List();
	self->FULL_LDFLAGS  = new_List();
	self->FULL_LIBFLAGS = new_List();
	self->FULL_LIBS     = new_List();
	self->FULL_RUNLIBS  = new_List();

	self->includeDirs        = new_List();
	self->sourceFiles        = new_List();
	self->psaFiles           = new_List();
	self->objectFiles        = new_List();

	self->libraries          = new_List();
	self->libraryDirs        = new_List();
	self->runtimeLibraryDirs = new_List();
	self->systemLibraryDirs  = new_List();

	self->elf_libraries = new_List();
	self->dll_libraries = new_List();
	self->unix_archives = new_List();
	self->windows_archives = new_List();

	self->system_libraries = new_List();

	self->language = NULL;
	self->compiler = NULL;
	self->linker   = NULL;
	self->archiver = NULL;

	//
	//	Test retrieving a ToolSet via a provider
	//
	//fprintf( stdout, "Loading provider\n" );
	//Provider* provider = Provider_Load( "providers/liblinux_gnu_c.so" );
	//if ( provider )
	//{
	//	Provider_print( provider, stdout );
	//	ToolSetProvider* toolset = (ToolSetProvider*) Provider_getObject( provider );
	//	//toolset->objectify( NULL, NULL, NULL );
	//	//toolset->linkify( NULL, NULL );
	//}

	return self;
}

void
free_ProviderContext( ProviderContext* self )
{
	free( self->packageDir );
	free( self->targetDir );
	
	free( self->package_name );
	
	free_List( self->FULL_CPPFLAGS );
	free_List( self->FULL_CFLAGS );	
	free_List( self->FULL_LFLAGS );	
	free_List( self->FULL_LDFLAGS );	
	free_List( self->FULL_LIBFLAGS );	
	free_List( self->FULL_LIBS );	
	free_List( self->FULL_RUNLIBS );

	free_List( self->includeDirs );
	free_List( self->sourceFiles );
	free_List( self->psaFiles );
	free_List( self->objectFiles );

	free_List( self->libraries );
	free_List( self->libraryDirs );
	free_List( self->runtimeLibraryDirs );
	free_List( self->systemLibraryDirs );

	free_List( self->dll_libraries );
	free_List( self->elf_libraries );
	free_List( self->unix_archives );
	free_List( self->windows_archives );

	free_List( self->system_libraries );

	free( self );
}

void
ProviderContext_print( const ProviderContext* self )
{
	fprintf( stdout, "<context package_name=\"%s\">\n", self->package_name );
	{
		BuildParameters_print( self->parameters );
		fprintf( stdout, "\t<source dir=\"%s\"/>\n", self->packageDir );
		fprintf( stdout, "\t<target dir=\"%s\"/>\n", self->targetDir );
		fprintf( stdout, "\t<libraries>\n" );
		{
			List_print( self->libraries );
		}
		fprintf( stdout, "\t</libraries>\n" );

		fprintf( stdout, "\t<elfLibraries>\n" );
		{
			List_print( self->elf_libraries );
		}
		fprintf( stdout, "\t</elfLibraries>\n" );

		fprintf( stdout, "\t<dllLibraries>\n" );
		{
			List_print( self->dll_libraries );
		}
		fprintf( stdout, "\t</dllLibraries>\n" );

		fprintf( stdout, "\t<unixArchives\n" );
		{
			List_print( self->unix_archives );
		}
		fprintf( stdout, "\t</unixArchives>\n" );

		fprintf( stdout, "\t<windowsArchives\n" );
		{
			List_print( self->windows_archives );
		}
		fprintf( stdout, "\t</windowsArchives>\n" );

		fprintf( stdout, "\t<systemLibraries\n" );
		{
			List_print( self->system_libraries );
		}
		fprintf( stdout, "\t</systemLibraries>\n" );

		fprintf( stdout, "\t<libraryDirectories>\n" );
		{
			List_print( self->libraryDirs );
		}
		fprintf( stdout, "\t</libraryDirectories>\n" );
		fprintf( stdout, "\t<systemLibraryDirectories>\n" );
		{
			List_print( self->systemLibraryDirs );
		}
		fprintf( stdout, "\t</systemLibraryDirectories>\n" );
		fprintf( stdout, "\t<runtime>\n" );
		{
			List_print( self->runtimeLibraryDirs );
		}
		fprintf( stdout, "\t</runtime>\n" );
		fprintf( stdout, "\t<include>\n" );
		{
			List_print( self->includeDirs );
		}
		fprintf( stdout, "\t</include>\n" );
		fprintf( stdout, "\t<source>\n" );
		{
			List_print( self->sourceFiles );
		}
		fprintf( stdout, "\t</source>\n" );
		fprintf( stdout, "\t<objects>\n" );
		{
			List_print( self->objectFiles );
		}
		fprintf( stdout, "\t</objects>\n" );

	}
	fprintf( stdout, "</context>\n" );

	
	
	//List_print( self->FULL_CPPFLAGS );
	//List_print( self->FULL_CFLAGS );
	//List_print( self->FULL_LDFLAGS );
	//List_print( self->FULL_LIBFLAGS );
	//List_print( self->FULL_LIBS );
	//List_print( self->FULL_RUNLIBS );
}

char*
packageDirToPackageName( const char* package_dir )
{
	IPath* tmp_path = new_Path( package_dir );
	char* tmp = CharString_copy( Path_getBasename( tmp_path ) );
	free_Path( tmp_path );
	return tmp;
	
	//char* tmp = CharString_copy( Path_getCommon( tmp_path ) );
	//char* package_name = CharString_basename( tmp );

	//free_Path( tmp_path );
	//free( tmp );
	//return package_name;
}
