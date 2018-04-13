#ifndef __TRAVERSE_H
#define __TRAVERSE_H
#include "mylist.h"
#include "bitree.h"
 
int preorder(const BiTreeNode *node,List *list);
int inorder (const BiTreeNode *node,List *list);
int postorder(const BiTreeNode *node,List *list);

#endif
