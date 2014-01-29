#include "ll_rbtree.h"
#include "test_rbtree.h"

#define BLACK 1
#define RED 0

typedef struct my_s my_t;

int node_is_red(ll_rbtree_node_t *n)
{ 
  return  ((n) != NULL && (n)->color == RED);
}

int node_is_black(ll_rbtree_node_t *n) 
{
  return ((n) == NULL || (n)->color == BLACK);
}

struct my_s {
  int value;
  ll_rbtree_node_t node;
};

void
check_node(ll_rbtree_node_t *node, int *level) {
  int cur_black;

  ASSERT_TRUE (node_is_red(node) || node_is_black(node));

  if (node == NULL){
     *level = 1;
     return;
  }

  if (node_is_red(node)) { //Property 3
    ASSERT_TRUE(node_is_black(node->left));
    ASSERT_TRUE(node_is_black(node->right));
    cur_black = 0;

  } else {
    cur_black = 1;

  }
  int left, right;
  check_node(node->left, &left);
  check_node(node->right, &right);
  ASSERT_EQ(left, right);
  *level = left + cur_black;
  return;
}

void
check(ll_rbtree_t *tree){
  if (tree->root != NULL) {
    ASSERT_TRUE(node_is_black(tree->root)); //Property 2
    int level;
    check_node(tree->root, &level);
  }
}

void my_insert(ll_rbtree_t *tree, my_t *mys) {
  assert(tree);
  
  ll_rbtree_node_init(&mys->node);

  if (!tree->root) {
    tree->root = &mys->node;
  } else {
    ll_rbtree_node_t *tmp = tree->root, *parent = NULL;

    while (true){
      if (tmp == NULL) break;
      parent = tmp;
      my_t *tmpd = LL_CONTAINER_OF(tmp, my_t, node);
      if (tmpd->value > mys->value)  tmp = tmp->left;
      else tmp = tmp->right;
    }

    if (LL_CONTAINER_OF(parent, my_t, node)->value > mys->value) parent->left = &mys->node;
    else parent->right = &mys->node;
    mys->node.parent = parent;
    ll_rbtree_node_set_red(&mys->node);
  }

  ll_rbtree_fixup_insert(tree, &mys->node);
  check(tree);
}

int
myvisit(void *data) {
  my_t *me = (my_t *) data;

  printf ("%d ", me->value);
}

TEST_F(LLRbtreeTest, TestNormal) {
  const int NODES = 1024;
  int i;
  ll_rbtree_t tree;
  my_t nodes[NODES];

  srand(time(NULL));
  for (i = 0; i < NODES; i++) {
    nodes[i].value = i;
    my_insert(&tree, &nodes[i]);
  }
  
  //  ll_rbtree_traverse_firstorder(&tree, myvisit);
}
