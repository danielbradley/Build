/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_NODE_INTERNAL_H
#define OPENOCL_BASE_PSA_NODE_INTERNAL_H

typedef struct _INode INode;

INode*       new_Node();
void         free_Node( INode* self );

void         Node_setParent( INode* self, const INode* parent );
void         Node_setChild( INode* self, INode* child );
void         Node_setLeftChild( INode* self, INode* child );
void         Node_setRightChild( INode* self, INode* child );
void         Node_setSibling( INode* self, INode* sibling );
void         Node_setKey( INode* self, const char* key );
void         Node_setObject( INode* self, const void* object );
void         Node_setKeyAndObject( INode* self, const char* key, const void* object );

void         Node_addChild( INode* self, INode* child );
void         Node_addLeftChild( INode* self, INode* child );
void         Node_addRightChild( INode* self, INode* child );
void         Node_addSibling( INode* self, INode* sibling );

const INode* Node_getParent( const INode* self );
const INode* Node_getChild( const INode* self );
const INode* Node_getLeftChild( const INode* self );
const INode* Node_getRightChild( const INode* self );
const INode* Node_getSibling( const INode* self );

const char*  Node_getKey( const INode* self );
const void*  Node_getObject( const INode* self );

#endif
