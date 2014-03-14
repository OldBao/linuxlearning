/**
 * @file   ll_events.h
 * @author zhangguanxing <zgxcassar@gmail.com>
 * @date   Fri Mar 14 20:01:57 2014
 * 
 * @brief  
 * 
 * 
 */

#ifndef LL_EVENT_H_
#define LL_EVENT_H_

#include "ll_core.h"

typedef struct ll_event_s ll_event_t;
typedef struct ll_event_context_t *ctx;

struct ll_event_loop_s {
  int epfd;
  struct epoll_event* events;
  int nevents;

  ll_vector_t connections;
  ll_rbtree_t timer;
};

struct ll_event_s {
  int accept : 1;
  int read   : 1;

  
  ll_event_t *next;
};

#endif

