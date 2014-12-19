#ifndef LL_EVENT_H_
#define LL_EVENT_H_

#include "ll_rbtree.h"
#include "ll_time.h"
#include "ll_list.h"

#include <sys/epoll.h>
typedef uint32_t ll_event_flags_t;
typedef struct ll_event_loop_s ll_event_loop_t;
typedef struct ll_event_s ll_event_t;
typedef struct ll_event_if_s ll_event_if_t;
typedef struct ll_event_context_s ll_event_context_t;
typedef struct ll_event_cb_s ll_event_cb_t;

#define LL_EVENT_INIT 0x0
#define LL_EVENT_READ 0x1
#define LL_EVENT_WRITE 0x2
#define LL_EVENT_TIMEOUT 0x4


struct ll_event_loop_s {
  int max_events;
  ll_rbtree_t timeout_tree;

  void *implement_data;
};


struct ll_event_if_s {
  int (*init)(ll_event_loop_t *loop);
  int (*add_event)(ll_event_loop_t *loop, ll_event_t* event, ll_event_flags_t flags);
  int (*del_event)(ll_event_loop_t *loop, ll_event_t* event, ll_event_flags_t flags);
  int (*process)(ll_event_loop_t *loop, ll_msec_t timeout);
  int (*destroy)(ll_event_loop_t *loop);
};


struct ll_event_cb_s {
  int (*on_read) (ll_event_loop_t *loop, ll_event_t *event);
  int (*on_write) (ll_event_loop_t *loop, ll_event_t *event);
  int (*on_timeout) (ll_event_loop_t *loop, ll_event_t *event);
};


struct ll_event_s {
  int fd;
  ll_event_flags_t flags;
  void *private_data;

  ll_event_cb_t callbacks;
  ll_msec_t abstimeout, timeout;

  ll_rbtree_node_t timeout_node;
};

int ll_event_loop_add_event(ll_event_loop_t *loop, ll_event_t *event, ll_event_flags_t flags, ll_msec_t timeout = LL_MSTIME_INF);

int ll_event_init(ll_event_t *event, int fd, ll_event_cb_t callback, void *private_data);

int ll_event_loop_init(ll_event_loop_t *loop, int max_events);
int ll_event_loop_reset_timeout_event(ll_event_loop_t *loop, ll_event_t *event, ll_msec_t timeout);
int ll_event_loop_del_event(ll_event_loop_t *loop, ll_event_t *event, ll_event_flags_t flags = LL_EVENT_READ | LL_EVENT_WRITE | LL_EVENT_TIMEOUT);
int ll_event_loop_loop(ll_event_loop_t *loop);
int ll_event_loop_destroy(ll_event_loop_t *loop);

#endif
