/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/io/psa/SuperDirectory.private.h"

#include "openocl/base/psa/CharStringList.h"
#include "openocl/io/psa/Path.h"

#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

//--------------------------------------------------------------------
//	functions
//--------------------------------------------------------------------

int SuperDirectory_refresh( ISuperDirectory* self )
{
	int status = 1;
	
	if ( self->volumes )
	{
		free_List( self->volumes );
	}
	self->volumes = new_List();
	
	List_copyItem( self->volumes, "/" );
	
	return status;
}
