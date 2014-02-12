/**
 * @file   ll_rbtree.h
 * @author zgxcassar@gmail.com
 * @date   Jan Dec. 2013/12/9
 * 
 * @brief  
 * 
 * 
 */

#ifndef LL_RBTREE_H_
#define LL_RBTREE_H_

#include "ll_core.h"

typedef struct ll_rbtree_s ll_rbtree_t;
typedef struct ll_rbtree_node_s ll_rbtree_node_t;

struct ll_rbtree_s {
  ll_rbtree_node_t *root;

  //int count;
};

struct ll_rbtree_node_s {
  int color;
  ll_rbtree_node_t *parent, *left, *right;
};


void ll_rbtree_init(ll_rbtree_t *tree);

void ll_rbtree_node_set_red(ll_rbtree_node_t *n);
void ll_rbtree_node_init(ll_rbtree_node_t *n); 
void ll_rbtree_delete_node(ll_rbtree_t *tree, ll_rbtree_node_t *n);
void ll_rbtree_replace_node(ll_rbtree_node_t *dst, ll_rbtree_node_t *src);
void ll_rbtree_fixup_insert(ll_rbtree_t *t, ll_rbtree_node_t *n);
void ll_rbtree_fixup_delete(ll_rbtree_t *t, ll_rbtree_node_t *n);

ll_rbtree_node_t* ll_rbtree_successor(ll_rbtree_node_t *node);

void ll_rbtree_traverse_firstorder(ll_rbtree_t *t, ll_general_callback_ft cb);
void ll_rbtree_traverse_postorder(ll_rbtree_t *t, ll_general_callback_ft cb);

void ll_rbtree_destroy(ll_rbtree_t *t, ll_general_callback_ft *free_func);

#endif
