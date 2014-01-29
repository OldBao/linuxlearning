#include <assert.h>

#include "ll_rbtree.h"

static const int BLACK = 1;
static const int RED = 0;


#define node_set_red ll_rbtree_node_set_red
#define node_set_black(node) (node)->color = BLACK
#define node_is_black(node) (node)->color == BLACK
#define node_is_red(node) (node)->color == RED


static inline ll_rbtree_node_t *
parent(ll_rbtree_node_t *node) {
  return node->parent;
}

static inline ll_rbtree_node_t *
grandpa(ll_rbtree_node_t *node) {
  return node->parent->parent;
}

static inline ll_rbtree_node_t *
uncle(ll_rbtree_node_t *node) {
  ll_rbtree_node_t *p = node->parent;
  ll_rbtree_node_t *g = grandpa(node);
  
  if (p == g->left) {
    return g->right;
  } else {
    return g->left;
  }
}

static inline void 
left_rotate(ll_rbtree_node_t *node) {
  assert(node);

  ll_rbtree_node_t *g, *p;
  
  p  = parent(node);
  g  = grandpa(node);

  if (p == NULL) {
    //rotate your sister
    return;
  }
  
  p->left = node->right;
  node->parent = p->parent;
  p->parent = node;
  node->right = p;
  
  if (g != NULL) {
    if (p == g->left) {
      g->left = node;
    } else {
      g->right = node;
    }
  }
}

static inline void 
right_rotate(ll_rbtree_node_t *node) {
  assert(node);

  ll_rbtree_node_t *g, *p;
  
  p  = parent(node);
  g = grandpa(node);

  if (p == NULL) {
    //rotate your sister
    return;
  }
  
  p->right = node->left;
  node->parent = p->parent;
  p->parent = node;
  node->left = p;
  
  if (g != NULL) {
    if (p == g->left) {
      g->left = node;
    } else {
      g->right = node;
    }
  }
}

void
ll_rbtree_fixup_insert(ll_rbtree_t *t, ll_rbtree_node_t *n) {
  ll_rbtree_node_t *g, *u, *p;
  
  p = parent(n);

  // case I : root is NULL
  if (p == NULL) {
    node_set_black(n);
    return;
  }

  while (true) {
    //outer condition
    if (p == NULL) {
      node_set_black(n);
      break;
    } else if (node_is_black(p)) {
      break;
    } else {
      g = grandpa(n); 
      u = uncle(n);
      
      if (node_is_red(u)) {
	/*
	 * case II (NLR not consider left or right)
	 *       G                   g
	 *     /   \                / \
	 *    p     u       =>     P    U
	 *   / \   / \            / \  / \
	 *  n   1 2   3          n  1  2  3
	 *
	 * note: p will never be root (prop 1), so g is at least in root level
	 */
	node_set_black(p);
	node_set_black(u);
	node_set_red(g);

	n = g;
      } else if (node_is_black(u)){
	/*
	 * case III
	 *       G                   G
	 *     /   \                /  \
	 *    p     U       =>     n    U   (case IV)
	 *   / \   / \            /    / \
	 *  1   n 2   3          p    2   3
	 *
	 */
	if (n == p->right && p == g->left) {
	  left_rotate(n);
	  n = p;
	  p = parent(n);
	}

        if (n == p->left &&  p == g->right) {
	  right_rotate(n);
	  n = p;
	  p = parent(n);
	}

	/*
	 * case IV
	 *       G                    P
	 *     /   \                /   \
	 *    p     U       =>     n     g
	 *   / \   / \            / \   / \
	 *  n   3 4   5          1   2 3   U
         * / \                            / \
	 *1   2                          4   5
	 */

	node_set_red(g);
	node_set_black(p);
	if (p == g->left)
	  right_rotate(g);
	else
	  left_rotate(g);
      }
    
    }
  }
}

void 
ll_rbtree_node_set_red(ll_rbtree_node_t *node) {
  node->color = RED;
}

void
ll_rbtree_node_init(ll_rbtree_node_t *n) {
  assert(n);
  n->left = NULL;
  n->right = NULL;
  ll_rbtree_node_set_red(n);
}

void
ll_rbtree_traverse_firstorder(ll_rbtree_t *t, ll_rbtree_node_t *root, ll_general_callback_ft cb) {
  if (root == NULL) {
    return;
  } else {
    ll_rbtree_traverse_firstorder(t, root->left, cb);
    cb(root);
    ll_rbtree_traverse_firstorder(t, root->right, cb);
  }
}


