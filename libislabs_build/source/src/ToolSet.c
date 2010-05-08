/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "islabs/build/providers/ToolSet.h"

int ToolSet_compile( const ToolSet* self,
                     const void*    buildParameters,
		     const void*    providerContext,
		     const void*    target )
{
	return self->compile( buildParameters, providerContext, target );
}

int ToolSet_link( const ToolSet* self,
                  const void*    buildParameters,
		  const void*    providerContext,
		  const void*    target )
{
	return self->link( buildParameters, providerContext, target );
}
