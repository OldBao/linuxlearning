#ifndef LL_EVENT_H_
#define LL_EVENT_H_

#include "ll_rbtree.h"
#include "ll_time.h"
#include "ll_list.h"

#include <sys/epoll.h>
typedef struct ll_event_loop_s ll_event_loop_t;
typedef struct ll_event_s ll_event_t;
typedef struct ll_event_if_s ll_event_if_t;
typedef struct ll_event_context_s ll_event_context_t;
typedef int (*ll_event_cb_t)(ll_event_loop_t *loop, ll_event_t *event);

#define LL_EVENT_INIT 0x0
#define LL_EVENT_READ 0x1
#define LL_EVENT_WRITE 0x2

struct ll_event_loop_s {
  int max_events;
  ll_rbtree_t timeout_tree;
  ll_list_t fire_event_list;

  void *implement_data;
};

struct ll_event_if_s {
  int (*init)(ll_event_loop_t *loop);
  int (*add_event)(ll_event_loop_t *loop, ll_event_t* event, uint32_t flags);
  int (*del_event)(ll_event_loop_t *loop, ll_event_t* event);
  int (*process)(ll_event_loop_t *loop, ll_timeval_t* tm);
  int (*destroy)(ll_event_loop_t *loop);
};

struct ll_event_s {
  int fd;
  uint32_t flags;
  ll_event_cb_t callback;
  void *private_data;

  ll_timeval_t timeout;

  ll_rbtree_node_t timeout_node;
  ll_list_node_t fire_node;
};

int ll_event_init(ll_event_t *event, int fd, int flag, ll_event_cb_t callback, void *private_data);

int ll_event_loop_init(ll_event_loop_t *loop, int max_events);
int ll_event_loop_add_event(ll_event_loop_t *loop, ll_event_t *event);
int ll_event_loop_del_event(ll_event_loop_t *loop, ll_event_t *event);
int ll_event_loop_loop(ll_event_loop_t *loop);
int ll_event_loop_destroy(ll_event_loop_t *loop);

#endif
