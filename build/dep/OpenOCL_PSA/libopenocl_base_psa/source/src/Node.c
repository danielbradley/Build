/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Node.private.h"

INode* new_Node()
{
	return (INode*) CRuntime_calloc( 1, sizeof( INode ) );
}

void free_Node( INode* self )
{
	if ( self->sibling )
	{
		free_Node( self->sibling );
	}
	if ( self->child )
	{
		free_Node( self->child );
	}
	if ( self->rightChild )
	{
		free_Node( self->rightChild );
	}
	if ( self->key )
	{
		free_CharString( self->key );
	}
	CRuntime_free( self );
}

void
Node_setParent( INode* self, const INode* parent )
{
	self->parent = parent;
}

void
Node_setChild( INode* self, INode* child )
{
	self->child = child;
}

void
Node_setLeftChild( INode* self, INode* child )
{
	self->child = child;
}

void
Node_setSibling( INode* self, INode* sibling )
{
	self->sibling = sibling;
}

void
Node_setRightChild( INode* self, INode* child )
{
	self->rightChild = child;
}

void
Node_setKey( INode* self, const char* key )
{
	self->key    = CharString_copy( key );
}

void
Node_setObject( INode* self, const void* object )
{
	self->object = object;
}

void
Node_setKeyAndObject( INode* self, const char* key, const void* object )
{
	self->key    = CharString_copy( key );
	self->object = object;
}

void
Node_addChild( INode* self, INode* child )
{
	Node_addSibling( child, self->child );
	self->child = child;
}

void
Node_addLeftChild( INode* self, INode* child )
{
	Node_addSibling( child, self->child );
	self->child = child;
}

void
Node_addRightChild( INode* self, INode* child )
{
	Node_addSibling( child, self->rightChild );
	self->rightChild = child;
}

void
Node_addSibling( INode* self, INode* sibling )
{
	if ( self->sibling )
	{
		Node_addSibling( self->sibling, sibling );
	} else {
		self->sibling = sibling;
	}
}

const INode*
Node_getParent( const INode* self )
{
	return self->parent;
}

const INode*
Node_getChild( const INode* self )
{
	return self->child;
}

const INode*
Node_getLeftChild( const INode* self )
{
	return self->child;
}

const INode*
Node_getRightChild( const INode* self )
{
	return self->rightChild;
}

const INode*
Node_getSibling( const INode* self )
{
	return self->sibling;
}

const char*
Node_getKey( const INode* self )
{
	return self->key;
}

const void*
Node_getObject( const INode* self )
{
	return self->object;
}
