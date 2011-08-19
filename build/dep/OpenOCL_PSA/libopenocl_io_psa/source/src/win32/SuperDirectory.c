/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#include <tchar.h>
#include "openocl/io/psa/SuperDirectory.private.h"

#include "openocl/base/psa/CharStringList.h"
#include "openocl/io/psa/Path.h"
#include <stdio.h>

//--------------------------------------------------------------------
//	functions
//--------------------------------------------------------------------

int SuperDirectory_refresh( ISuperDirectory* self )
{
	int status = 1;
	//TCHAR buffer[MAX_PATH];
	//int handle;

	if ( self->volumes )
	{
		free_List( self->volumes );
	}
	self->volumes = new_List();
	List_copyItem( self->volumes, "C:\\" );

	//handle = FindFirstVolume( buffer, MAX_PATH );
	//List_copyItem( self->volumes, dirname );
	//FindVolumeClose( search_handle );
	
	return status;
}
