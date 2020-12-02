/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/ProviderRegistry.h"

#include <openocl/base/psa/CRuntime.h>
#include <openocl/base/psa/CharString.h>
#include <openocl/base/psa/List.h>
#include <openocl/io/psa/Directory.h>
#include <openocl/io/psa/File.h>
#include <openocl/io/psa/Path.h>
#include <openocl/provider/psa/Provider.h>

#include <stdio.h>
#include <sys/types.h>

#define NR_OF_BUILTIN_PROVIDERS 8

#ifdef EMBED_PROVIDERS
extern Provider* CreateBuildProvider_islabs_darwin_apple();
extern Provider* CreateBuildProvider_islabs_linux_gnu_gnu();
extern Provider* CreateBuildProvider_islabs_win32_gnu();
extern Provider* CreateBuildProvider_islabs_win32_ms();
extern Provider* CreateBuildProvider_islabs_linux_gnu_lammpi();
extern Provider* CreateBuildProvider_islabs_jdk_sun();
extern Provider* CreateBuildProvider_islabs_dotnet_mono();
extern Provider* CreateBuildProvider_islabs_dotnet_ms();
#endif

struct _ProviderRegistry
{
	void**		providers;
	unsigned int	count;
};

ProviderRegistry* new_ProviderRegistry( const char* directory )
{
	ProviderRegistry* self = (ProviderRegistry*) CRuntime_calloc( 1, sizeof( ProviderRegistry ) );

	if ( directory )
	{
		IDirectory* dir = new_Directory( directory );
		fprintf( stdout, "Loading providers\n" );
		if ( Directory_refresh( dir ) )
		{
			const IList* list = Directory_getFileList( dir );
			unsigned int nr_of_providers = list->count;
			if ( 0 < nr_of_providers )
			{
				unsigned int i;
				self->providers = CRuntime_calloc( nr_of_providers + 1, sizeof( void* ) );
				for ( i=0; i < list->count; i++ )
				{
					if ( CharString_endsWith( list->items[i], ".provider" ) )
					{
						char* libname = CharString_cat3( directory, "/", list->items[i] );
						Provider* provider = Provider_Load( libname );
						if ( provider )
						{
							//Provider_print( provider, stdout );
							self->providers[self->count] = provider;
							self->count++;
						}
						free_CharString( libname );
					}
				}
			}
		}
		free_Directory( dir );
	}
#ifdef EMBED_PROVIDERS
	else
	{
		self->providers    = CRuntime_calloc( NR_OF_BUILTIN_PROVIDERS + 1, sizeof( void*) );
		self->providers[0] = CreateBuildProvider_islabs_linux_gnu_gnu();
		self->providers[1] = CreateBuildProvider_islabs_win32_gnu();
		self->providers[2] = CreateBuildProvider_islabs_win32_ms();
		self->providers[3] = CreateBuildProvider_islabs_linux_gnu_lammpi();
		self->providers[4] = CreateBuildProvider_islabs_darwin_apple();
		self->providers[5] = CreateBuildProvider_islabs_jdk_sun();
		self->providers[6] = CreateBuildProvider_islabs_dotnet_mono();
		self->providers[7] = CreateBuildProvider_islabs_dotnet_ms();
		self->count = NR_OF_BUILTIN_PROVIDERS;
	}
#endif
	return self;
}

void free_ProviderRegistry( ProviderRegistry* self )
{
	CRuntime_free( self->providers );
	CRuntime_free( self );
}

ToolSet*
ProviderRegistry_toolSet( const ProviderRegistry* self, const char* platform, const char* compiler )
{
	ToolSet* tool_set = NULL;
	const ToolSetProvider* ts_provider = NULL;
	const char* vendor;
	
	if ( (0 == CharString_compare( platform, "darwin" )) && (0 == CharString_compare( compiler, "gcc" )) )
	{
		vendor = "apple";
	}
	else if ( (0 == CharString_compare( platform, "darwin" )) && ((0 == CharString_compare( compiler, "g++" )) || (0 == CharString_compare( compiler, "clang" ))) )
	{
		vendor = "apple";
	}
	else if ( 0 == CharString_compare( compiler, "gcc" ) )
	{
		vendor = "gnu";
	}
	else if ( 0 == CharString_compare( compiler, "g++" ) )
	{
		vendor = "gnu";
	}
	else if ( 0 == CharString_compare( compiler, "mcs" ) )
	{
		vendor   = "mono";
		platform = "dotnet";
	}
	else if ( 0 == CharString_compare( compiler, "cl" ) )
	{
		vendor = "ms";
	}
	else if ( 0 == CharString_compare( compiler, "csc" ) )
	{
		vendor   = "ms";
		platform = "dotnet";
	}
	else if ( 0 == CharString_compare( compiler, "mpicc" ) )
	{
		vendor = "lammpi";
	}
	else if ( 0 == CharString_compare( compiler, "mpic++" ) )
	{
		vendor = "lammpi";
	}
	else if ( 0 == CharString_compare( compiler, "javac" ) )
	{
		vendor   = "sun";
		platform = "jdk";
	}
	else
	{
		vendor = "";
	}

	{
		char* vendor_id = CharString_cat3( platform, "_", vendor );

		unsigned int i;
		for ( i=0; i < self->count; i++ )
		{
			if ( 0 == CharString_compare( Provider_getName( self->providers[i] ), vendor_id ) )
			{
				ts_provider = (ToolSetProvider*) Provider_getObject( self->providers[i] );
				break;
			}
		}
		free_CharString( vendor_id );
	}
	if ( ts_provider )
	{
		tool_set  = ts_provider->getToolSet( compiler );
	}
	return tool_set;
}
