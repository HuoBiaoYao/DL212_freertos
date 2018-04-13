#include "stdlib.h"
#include "string.h"
#include "bitree.h"
 
#include "traverse.h"

void Test_Tree(void){
  BiTree *tree = (BiTree*)malloc(sizeof(BiTree)); 
	List *list = (List*)malloc(sizeof(List)); 
  int i1 = 0x2B; int *pi1 = &i1;
  int i2 = 0x22; int *pi2 = &i2;
  int i3 = 0x11; int *pi3 = &i3;
  int i4 = 0x33; int *pi4 = &i4;
  int i5 = 0x44; int *pi5 = &i5;
	int i6 = 0x88; int *pi6 = &i6;
	
 
	bitree_init(tree, NULL_P); 
	list_init(list, NULL_P); 
 
	//²åÈë
  bitree_ins_left(tree, NULL_P, pi1);
  bitree_ins_left(tree, bitree_root(tree), pi2);
  bitree_ins_right(tree,bitree_root(tree), pi3);
  bitree_ins_left(tree, bitree_root(tree)->left, pi4);
  bitree_ins_right(tree, bitree_root(tree)->right, pi5);
  bitree_ins_left(tree, bitree_root(tree)->right->right, pi6);
  postorder(tree->root,list);
   // printf("t1->size=%d\n",bitree_size(tree));
  //É¾³ý
 // bitree_rem_left(tree,bitree_root(tree)->left);
 // bitree_rem_right(tree,bitree_root(tree)->right);
  // printf("t1->size=%d\n",bitree_size(tree));
/*
  //ºÏ²¢
  BiTree *tree2 = (BiTree*)malloc(sizeof(BiTree));
  bitree_init(tree2, NULL_P);
  int i7 = 1;int *pi7 = &i7;
  int i8 = 2;int *pi8 = &i8;
  bitree_ins_left(tree2, NULL_P, pi7);

  BiTree *merge = (BiTree*)malloc(sizeof(BiTree));
  bitree_merge(merge, tree, tree2, pi8);
  //  printf("t1->size=%d\n",bitree_size(merge));

  bitree_destroy(merge);*/
  //´Ý»Ù 
  bitree_destroy(tree);
}

void bitree_init(BiTree *tree,void (*destroy)(void *data)){
  tree->size = 0;
	tree->destroy = destroy;
	tree->root = NULL_P;
	
	return;
}

void bitree_destroy(BiTree *tree){
  bitree_rem_left(tree,NULL_P);
	memset(tree,0,sizeof(BiTree));
	
	return;
}

int bitree_ins_left(BiTree *tree,BiTreeNode *node,const void *data){
	BiTreeNode *new_node,**position;
	
	if(node == NULL_P){
	  if(bitree_size(tree) > 0){
		  return -1;
		}
		position = &tree->root;
	}
	else{
	  if(bitree_left(node) != NULL_P){
		  return -1;
		}
		position = &node->left;
	}
	if((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL_P){
	  return -1;
	}
	new_node->data = (void*)data;
	new_node->left = NULL_P;
	new_node->right = NULL_P;
	*position = new_node;
	tree->size++;
	
  return 0;
}

int bitree_ins_right(BiTree *tree,BiTreeNode *node,const void *data){
  BiTreeNode *new_node,**position;
	
	if(node == NULL_P){
	  if(bitree_size(tree) > 0){
		  return -1;
		}
		position = &tree->root;
	}
	else{
	  if(bitree_right(node) != NULL_P){
		  return -1;
		}
		position = &node->right;
	}
	if((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL_P){
	  return -1;
	}
	new_node->data = (void*)data;
	new_node->left = NULL_P;
	new_node->right = NULL_P;
	*position = new_node;
	tree->size++;
	
  return 0;
}

void bitree_rem_left(BiTree *tree,BiTreeNode *node){
	BiTreeNode **position;
	
	if(bitree_size(tree) == 0){
	  return;
	}
	if(node == NULL_P){
	  position = &tree->root;
	}
	else{
	  position = &node->left;
	}
	if(*position != NULL_P){
	  bitree_rem_left(tree,*position);
		bitree_rem_right(tree,*position);
		if(tree->destroy != NULL_P){
		  tree->destroy((*position)->data);
		}
		free(*position);
		*position = NULL_P;
		tree->size--;
	}
	
  return;
}

void bitree_rem_right(BiTree *tree,BiTreeNode *node){
  BiTreeNode **position;
	
	if(bitree_size(tree) == 0){
	  return;
	}
	if(node == NULL_P){
	  position = &tree->root;
	}
	else{
	  position = &node->right;
	}
	if(*position != NULL_P){
	  bitree_rem_left(tree,*position);
		bitree_rem_right(tree,*position);
		if(tree->destroy != NULL_P){
		  tree->destroy((*position)->data);
		}
		free(*position);
		*position = NULL_P;
		tree->size--;
	}
	
  return;
}

int bitree_merge(BiTree *merge,BiTree *left,BiTree *right,const void *data){
	bitree_init(merge,left->destroy);
	if(bitree_ins_left(merge,NULL_P,data) != 0){
	  bitree_destroy(merge);
		return -1;
	}
	bitree_root(merge)->left = bitree_root(left);
	bitree_root(merge)->right = bitree_root(right);
	merge->size = merge->size+bitree_size(left)+bitree_size(right);
	left->root = NULL_P;
	left->size = 0;
	right->root = NULL_P;
	right->size = 0;
	
  return 0;
}













