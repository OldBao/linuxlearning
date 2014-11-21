/**
 * @File   ll_list.h
 * @author zgxcassar@gmail.com
 * @date   Thu Jun 13 23:47:21 2013
 * 
 * @brief  a linked list functions
 * 
 * 
 */


#ifndef LL_LIST_H_
#define LL_LIST_H_

#include "ll_macro.h"
#include <assert.h>

typedef struct ll_list_node_s ll_list_node_t;
typedef ll_list_node_t ll_list_t;
struct ll_list_node_s {
  ll_list_node_t *prev, *next;
};

#define ll_list_foreach(h, n) for(ll_list_node *n = ll_list_head(h); n != ll_list_end(h); n = ll_list_next(h))
/*
#define ll_list_foreach_ex(h, n, c, m) {\
    ll_list_node_t *node;                                               \
    c *n;                                                               \
    for(node = ll_list_head(h); n = ll_list_item(node, c, m), node != ll_list_end(h); node = ll_list_next(node)) 
#define ll_list_foreach_end }
*/

#define ll_list_init(head) {	      \
    (head)->prev = head;	      \
    (head)->next = head;	      \
}

#define ll_list_empty(head) ((head)->prev == (head))

//this macro can be also used for insert after
#define ll_list_append(head, node){   \
    (node)->prev     = (head)->prev;  \
    (node)->prev->next = node;	      \
    (node)->next       = head;	      \
    (head)->prev       = node;	      \
  }

#define ll_list_head(head)  (head)->next

#define ll_list_tail(head)  (head)->prev

#define ll_list_end(head)   (head)

#define ll_list_remove(node){		\
    assert((node)->prev);		\
    assert((node)->next);		\
    (node)->prev->next = (node)->next;	\
    (node)->next->prev = (node)->prev;	\
    (node)->next = NULL;		\
    (node)->prev = NULL;		\
  }

#define ll_list_next(node)  (node)->next

#define ll_list_prev(node)  (node)->prev

#define ll_list_item(l, s, i) LL_CONTAINER_OF(l, s, i)

#define ll_list_clear(h) {                              \
  ll_list_node_t *p = (h)->next, *q = p->next;          \
  while (p != h) {                                      \
  p->prev = p->next = NULL;                             \
  p = q;                                                \
  q = q->next;                                          \
  }                                                     \
}
/*find sort function should be implement by users*/

#endif
