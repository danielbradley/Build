/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_NODE_PRIVATE_H
#define OPENOCL_BASE_PSA_NODE_PRIVATE_H

#include "openocl/base/psa/Node.internal.h"

struct _INode
{
	const INode* parent;
	INode*       sibling;
	INode*       child;
	INode*       rightChild;

	char*        key;
	const void*  object;
};

#endif
