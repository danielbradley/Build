/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_H
#define OPENOCL_BASE_PSA_H

#ifndef __cplusplus
	#ifndef BOOLEAN
	#define BOOLEAN
		typedef int bool;
	#endif
#endif

#ifndef BYTE
#define BYTE
	#ifndef __RPCNDR_H__
		typedef unsigned char byte;
	#endif
#endif

#ifndef FLAGS
#define FLAGS
	typedef unsigned long int flags;
#endif

#ifndef MASK
#define MASK
	typedef unsigned long int mask;
#endif

#endif
