/**
 * @file   ll_list.h
 * @author zgxcassar@gmail.com
 * @date   Thu Jun 13 23:47:21 2013
 * 
 * @brief  a linked list functions
 * 
 * 
 */


#ifndef LL_LIST_H_
#define LL_LIST_H_

//#include "ll_core.h"
#include "ll_macro.h"

typedef struct ll_list_node_s ll_list_node_t;

struct ll_list_node_s {
  ll_list_node_t *prev, *next;
};

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

/*find sort function should be implement by users*/

#endif
