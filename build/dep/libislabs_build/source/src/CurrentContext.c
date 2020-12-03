/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/CurrentContext.h"
#include "islabs/build/providers/ToolSetProvider.h"

#include <openocl/base/psa/CharString.h>
#include <openocl/io/psa/Path.h>
#include <openocl/provider/psa/Provider.h>

//#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char*
packageDirToPackageName( const char* package_dir );


CurrentContext*
CurrentContext_new( const Global* global, const char* package_dir, const char* target_dir )
{
	CurrentContext* this = (CurrentContext*) calloc( 1, sizeof( CurrentContext ) );
	this->global = global;
	this->package_dir = CharString_copy( package_dir );
	this->target_dir = CharString_copy( target_dir );

	this->package_name = packageDirToPackageName( package_dir );
	this->isLib = CharString_startsWith( this->package_name, "lib" );

	this->FULL_CPPFLAGS = new_List();
	this->FULL_CFLAGS   = new_List();
	this->FULL_CXXFLAGS = new_List();
	this->FULL_LFLAGS   = new_List();
	this->FULL_LDFLAGS  = new_List();
	this->FULL_LIBFLAGS = new_List();
	this->FULL_LIBS     = new_List();
	this->FULL_RUNLIBS  = new_List();

	this->elf_libraries = new_List();
	this->dll_libraries = new_List();
	this->unix_archives = new_List();
	this->windows_archives = new_List();

	this->language = "";
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

	return this;
}

void
CurrentContext_delete( CurrentContext* this )
{
	free( this->package_dir );
	free( this->target_dir );
	
	free( this->package_name );
	
	free_List( this->FULL_CPPFLAGS );
	free_List( this->FULL_CFLAGS );	
	free_List( this->FULL_CXXFLAGS );	
	free_List( this->FULL_LFLAGS );	
	free_List( this->FULL_LDFLAGS );	
	free_List( this->FULL_LIBFLAGS );	
	free_List( this->FULL_LIBS );	
	free_List( this->FULL_RUNLIBS );
	free_List( this->dll_libraries );
	free_List( this->elf_libraries );
	free_List( this->unix_archives );
	free_List( this->windows_archives );

	free( this );
}

void
CurrentContext_print( const CurrentContext* this )
{
	printf( "CurrentContext:\n" );

	printf( "\t%s\n", this->package_dir );
	printf( "\t%s\n", this->target_dir );
	printf( "\t%s\n", this->package_name );

	List_print( this->FULL_CPPFLAGS );
	List_print( this->FULL_CFLAGS   );
	List_print( this->FULL_CXXFLAGS );
	List_print( this->FULL_LDFLAGS  );
	List_print( this->FULL_LIBFLAGS );
	List_print( this->FULL_LIBS     );
	List_print( this->FULL_RUNLIBS  );
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

