#include <assert.h>
#include <stdio.h>
#include "ll_rbtree.h"

static const int BLACK = 1;
static const int RED = 0;

#define node_set_red         ll_rbtree_node_set_red
#define node_set_black(node) (node)->color = BLACK
#define node_is_black(node)  (!(node)||(node)->color == BLACK)
#define node_is_red(node)    ((node)&&(node)->color == RED)

static inline ll_rbtree_node_t *
parent(ll_rbtree_node_t *node, ll_rbtree_node_t *parent) {
  if (node == NULL) {
    return parent;
  }
  return node->parent;
}

static inline ll_rbtree_node_t *
grandpa(ll_rbtree_node_t *node, ll_rbtree_node_t* parent) {
  assert(node->parent == parent);
  if (!node) {
    if (!parent)
      return NULL;
    else 
      return parent->parent;
  }
  return node->parent->parent;
}

static inline ll_rbtree_node_t *
sibling(ll_rbtree_node_t *node, ll_rbtree_node_t *p) {
  ll_rbtree_node_t *s;
  if (!node) {
    return p->left? p->left : p->right;
  }

  if (node == p->left) {
    return p->right;
  } else if (node == p->right){
    return p->left;
  } else {
    assert(0);
  }
  return NULL;
}

static inline ll_rbtree_node_t *
uncle(ll_rbtree_node_t *node) {
  ll_rbtree_node_t *p = node->parent;
  ll_rbtree_node_t *g = grandpa(node, p);
  
  if (!p || !g) {
    return NULL;
  }
  
  if (p == g->left) {
    return g->right;
  } else {
    return g->left;
  }
}

static inline void 
left_rotate(ll_rbtree_t *tree, ll_rbtree_node_t *node) {
  assert(node);
  assert(node->right);

  ll_rbtree_node_t *p = parent(node, NULL), *rchild = node->right;
 
  node->right         = rchild->left; 
  if (node->right)
    node->right->parent = node;
  rchild->left        = node;
  rchild->parent      = node->parent;
  node->parent        = rchild; 

  
  if (p) {
    if (p->left == node) {
      p->left = rchild;
    } else if(p->right == node){
      p->right = rchild;
    } else {
      assert(0);
    } 
  } else {
    tree->root = rchild;
  }
  
}

static inline void 
right_rotate(ll_rbtree_t *tree, ll_rbtree_node_t *node) {
  assert(node);
  assert(node->left);

  ll_rbtree_node_t *p = parent(node, NULL), *lchild = node->left;
  assert(!p || (p && (node==p->left || node==p->right)));
  node->left         = lchild->right;  
  if (node->left)
    node->left->parent = node;
  lchild->right      = node;
  lchild->parent     = node->parent;
  node->parent       = lchild;
  
  if (p) {
    if (p->left == node) {
      p->left = lchild;
    } else if(p->right == node){
      p->right = lchild;
    } else {
      assert(0);
    }
  } else {
    tree->root = lchild;
  }
}

void 
ll_rbtree_init(ll_rbtree_t *t) {
  assert(t != NULL);
  
  t->root = NULL;
}

void
ll_rbtree_fixup_insert(ll_rbtree_t *t, ll_rbtree_node_t *n) {
  ll_rbtree_node_t *g, *u, *p;
  
  p = parent(n, NULL);

  // case I : root is NULL
  if (!p) {
    node_set_black(n);
    return;
  }

  int i;
  while (1) {
    p = parent(n, NULL);

    //outer condition
    if (p == NULL) {
      node_set_black(n);
      break;
    } else if (node_is_black(p)) {
      break;
    } else {
      g = grandpa(n, p);
      u = uncle(n);

      assert(g); //if p is red, his father must be black

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
	  left_rotate(t, p);
	  n = p;
	  continue;
	}

        if (n == p->left &&  p == g->right) {
	  right_rotate(t, p);
	  n = p;
	  continue;
	}

	/*
	 * case IV
	 *       G                    P
	 *     /   \                /	\
	 *    p     U       =>     n     g
	 *   / \   / \            / \   / \
	 *  n   3 4   5          1   2 3   U
	 * / \                            / \
	 *1   2                          4   5
	 */

	node_set_red(g);
	node_set_black(p);
	if (p == g->left)
	  right_rotate(t, g);
	else
	  left_rotate(t, g);
	assert(node_is_black(t->root));
      }
    }
  }
}

void
ll_rbtree_fixup_delete(ll_rbtree_t *tree, ll_rbtree_node_t *n, ll_rbtree_node_t *p) {
  assert(tree);
  ll_rbtree_node_t *s;

  while (n != tree->root && node_is_black(n)) {
    p = parent(n, p);

    if (n == p->left) {
      s = p->right;
      if (node_is_red(s)) {
        node_set_black(s);
        node_set_red(p);
        left_rotate(tree, p);
        continue;
      } else {
        if (s == NULL
            || (node_is_black(s->left) && node_is_black(s->right))){
          if (s)  node_set_red(s);
          n = p;
        } else {
          if (node_is_black(s->right)) {
            node_set_black(s->left);
            node_set_red(s);
            right_rotate(tree, s);
            continue;
          } else {
            s->color = p->color;
            node_set_black(p);
            node_set_black(s->right);
            left_rotate(tree, p);
            n = tree->root;
          }
        }
      }
    } else {
      s = p->left;
      if (node_is_red(s)) {
        node_set_black(s);
        node_set_red(p);
        right_rotate(tree, p);
        continue;
      } else {
        if (s == NULL
            || (node_is_black(s->left) && node_is_black(s->right))){
          if (s) node_set_red(s);
          n = p;
        } else {
          if (node_is_black(s->left)) {
            node_set_black(s->right);
            node_set_red(s);
            left_rotate(tree, s);
            continue;
          } else {
            s->color = p->color;
            node_set_black(p);
            node_set_black(s->left);
            right_rotate(tree, p);
            n = tree->root;
          }
        }
      }
    }
  }
  if (n)
    node_set_black(n);
}

void 
ll_rbtree_node_set_red(ll_rbtree_node_t *node) {
  assert(node);

  node->color = RED;
}

void
ll_rbtree_node_init(ll_rbtree_node_t *n) {
  assert(n);
  n->left   = NULL;
  n->right  = NULL;
  n->parent = NULL;
  ll_rbtree_node_set_red(n);
}

void 
ll_rbtree_delete_node(ll_rbtree_t* tree, ll_rbtree_node_t *node) {
  assert(tree);
  assert(tree->root);

  ll_rbtree_node_t *c, *y, *p;
  int color;
  p = NULL;

  if (!node->left || !node->right) y = node;
  else y = ll_rbtree_successor(node);
  color = y->color;

  c = y->left ? y->left : y->right; //c now point to y's non-null child (NULL if y is leaf)
  if (c) c->parent = y->parent;

  if (!y->parent) tree->root = c;
  else {
    if (y == y->parent->left) y->parent->left = c;
    else y->parent->right = c;
  }
  p = y->parent;
  if (y != node) {
    if (node == y->parent) {
      p = y;
    }
    ll_rbtree_replace_node(tree, node, y);
  }
  
  if (color == BLACK) {
    ll_rbtree_fixup_delete(tree, c, p);
  }
}


void
ll_rbtree_replace_node(ll_rbtree_t *tree, ll_rbtree_node_t *src, ll_rbtree_node_t *dst) {
  assert(dst);
  assert(src);

  if (src->left) {
    src->left->parent = dst;
  }
  if (src->right) {
    src->right->parent = dst;
  }

  if (src->parent) {
    if (src->parent->left == src) {
      src->parent->left = dst;
    } else if (src->parent->right == src) {
      src->parent->right = dst;
    } else {
      assert(0);
    }
  } else {
    tree->root = dst;
  }
  *dst = *src;
  
}

ll_rbtree_node_t *
ll_rbtree_successor(ll_rbtree_node_t *n) {
  assert(n);
  ll_rbtree_node_t *p;

  if (n->right) {
    n = n->right;
    while (n->left)  n = n->left;
  } else {
    p = parent(n, NULL);
    while (p && n == p->right) {
      n = p;
      p = parent(n, NULL);
    }
  }

  return n;
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
