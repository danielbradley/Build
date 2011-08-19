/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>

#include "openocl/provider/psa/Provider.h"
#include "openocl/provider/psa/ProviderInstance.h"
#include <stdlib.h>
#include <stdio.h>

//typedef Provider* (CALLBACK* MY_PROC)();

void* doit()
{
	return calloc( 10, sizeof( char ) );
}

Provider* Provider_Load( const char* provider )
{
	Provider* (CALLBACK*MyGetInstance)();
	Provider* prov = NULL;
	//void* (CALLBACK*GetInstance)();
	//void* (*function)();
	//function = &doit;

	//if ( CharString_endsWith( provider, "dll" ) )
	{
		//MY_PROC GetInstance;

		//IPath* path = new_Path( provider );
		HMODULE handle = LoadLibrary( provider );// Path_getNativeFormat( path ) );
		//free_Path( path );
		if ( handle )
		{
			MyGetInstance = (Provider*(CALLBACK*)()) GetProcAddress( handle, "GetInstance" );
			if ( NULL != MyGetInstance )
			{
				prov = MyGetInstance();
			} else {
				DWORD error = GetLastError();
				fprintf( stdout, "%s (%i)\n", "build: error retriving function", error );
			}
		} else {
			fprintf( stdout, "%s\n", "build: error loading library" );
		}
	}
	return prov;
}

const char* Provider_getType( const Provider* self )
{
	return self->type;
}

const char* Provider_getInfo( const Provider* self )
{
	return self->info;
}

const char* Provider_getVendor( const Provider* self )
{
	return self->vendor;
}

const char* Provider_getName( const Provider* self )
{
	return self->name;
}

const char* Provider_getVersion( const Provider* self )
{
	return self->version;
}

void* Provider_getObject( const Provider* self )
{
	return self->object;
}

void Provider_print( const Provider* self, void* stream )
{
	FILE* out = (FILE*) stream;

	fprintf( out, "%-9s%s\n", "Type:",    Provider_getType( self ) );
	fprintf( out, "%-9s%s\n", "Info:",    Provider_getInfo( self ) );
	fprintf( out, "%-9s%s\n", "Vendor:",  Provider_getVendor( self ) );
	fprintf( out, "%-9s%s\n", "Name:",    Provider_getName( self ) );
	fprintf( out, "%-9s%s\n", "Version:", Provider_getVersion( self ) );
	fprintf( out, "\n" );
}

