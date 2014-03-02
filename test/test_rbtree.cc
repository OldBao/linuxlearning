#include "ll_rbtree.h"
#include "test_rbtree.h"

#include <stdio.h>

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
  if (node->left){
    ASSERT_TRUE(node->left->parent == node);
  }
  if (node->right){
    ASSERT_TRUE(node->right->parent == node);
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
  ASSERT_TRUE(node_is_black(tree->root)); //Property 2

  if (tree->root != NULL) {
    int level;
    check_node(tree->root, &level);
  }
}

void my_insert(ll_rbtree_t *tree, my_t *mys) {
  assert(tree);

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

void
my_delete(ll_rbtree_t *tree, my_t *s) {
  ll_rbtree_delete_node(tree, &s->node);
  check(tree);
}

int
myvisit(void *data) {
  my_t *me = (my_t *) data;

  printf ("%d ", me->value);
}

TEST_F(LLRbtreeTest, Test1024) {
  const int NODES = 1024;
  int i;
  ll_rbtree_t tree;
  ll_rbtree_init(&tree);
  my_t nodes[NODES];
  srand(time(NULL));

  for (i = 0; i < NODES; i++) {
    nodes[i].value = rand();
    ll_rbtree_node_init(&nodes[i].node);
    my_insert(&tree, &nodes[i]);
  }

}

TEST_F(LLRbtreeTest, TestOne) {
  const int NODES = 1;
  int i;
  ll_rbtree_t tree;
  ll_rbtree_init(&tree);

  my_t nodes[NODES];
  srand(time(NULL));

  for (i = 0; i < NODES; i++) {
    ll_rbtree_node_init(&nodes[i].node);
    nodes[i].value = i;
    my_insert(&tree, &nodes[i]);
  }
}

TEST_F(LLRbtreeTest, TestRotateRoot) {
  ll_rbtree_t tree;
  ll_rbtree_init(&tree);

  my_t node2, node3, node4, node5, node6;
  ll_rbtree_node_init(&node2.node);
  node2.value = 2;
  ll_rbtree_node_init(&node3.node);
  node3.value = 3;
  ll_rbtree_node_init(&node4.node);
  node4.value = 4;
  ll_rbtree_node_init(&node5.node);
  node5.value = 5;
  ll_rbtree_node_init(&node6.node);
  node6.value = 6;

  my_insert(&tree, &node5);
  my_insert(&tree, &node2);
  my_insert(&tree, &node6);
  my_insert(&tree, &node3);
  my_insert(&tree, &node4);
}

TEST_F(LLRbtreeTest, TestDeleteONE) {
  const int NODES = 1;
  ll_rbtree_t tree;
  ll_rbtree_init(&tree);
  
  my_t *nodes = new my_t[NODES];
  srand(time(NULL));
  int i;

  for(i = 0; i < NODES; i++) {
    ll_rbtree_node_init(&nodes[i].node);
    nodes[i].value = i;
    my_insert(&tree, &nodes[i]);
  }

  for (i = 0; i < NODES; i++) {
    my_delete(&tree, &nodes[i]); 
  }

}


TEST_F(LLRbtreeTest, TestDelete1024) {
  const int NODES = 4;
  ll_rbtree_t tree;
  ll_rbtree_init(&tree);
  
  my_t *nodes = new my_t[NODES];
  srand(time(NULL));
  int i;
  int values[] = {2,3,1,4};

  for(i = 0; i < NODES; i++) {
    ll_rbtree_node_init(&nodes[i].node);
    nodes[i].value = values[i];
    printf("insert %d\n", nodes[i].value);
    my_insert(&tree, &nodes[i]);
  }

  for (i = 0; i < NODES; i++) {
    my_delete(&tree, &nodes[i]); 
  }

}
