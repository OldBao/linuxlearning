#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include <unistd.h>

#include "ll_events.h"

#ifdef HAVE_EPOLL
#include <sys/epoll.h>
#endif

typedef struct ll_epoll_loop_s ll_epoll_loop_t;

struct ll_epoll_loop_s {
  int epfd;
  struct epoll_event *ep_events;
};


int 
_epoll_init(ll_event_loop_t *loop) {
  assert(loop);

  ll_epoll_loop_t* ep_loop = (ll_epoll_loop_t *) malloc(sizeof (ll_epoll_loop_t));
  loop->implement_data = ep_loop;

  ep_loop->epfd = epoll_create(1024);
  if (ep_loop->epfd == -1) {
    return -1;
  }

  ep_loop->ep_events = (struct epoll_event *) malloc (sizeof (struct epoll_event) * loop->max_events);
  if (!ep_loop->ep_events) {
    return -1;
  }

  return 0;
}


int 
_epoll_add_event(ll_event_loop_t *loop, ll_event_t *event, uint32_t flags) {
  assert (loop);
  assert (event);
  assert (loop->implement_data);

  ll_epoll_loop_t *ep_loop = (ll_epoll_loop_t *) loop->implement_data;
  struct epoll_event ep_event;
  int cmd;
  
  bzero (&ep_event, sizeof (epoll_event));

  cmd = event->flags == LL_EVENT_INIT ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;

  flags |= event->flags;
  if (flags & LL_EVENT_READ)   ep_event.events |= EPOLLIN | EPOLLONESHOT;
  if (flags & LL_EVENT_WRITE)  ep_event.events |= EPOLLOUT;
  
  ep_event.data.ptr = event;

  if (-1 == epoll_ctl(ep_loop->epfd, cmd, event->fd, &ep_event)){
    return -1;
  }

  return 0;
}


int
_epoll_del_event(ll_event_loop_t *loop, ll_event_t *event) {
  return 0;
}

int
_epoll_process(ll_event_loop_t *loop, ll_timeval_t *tv) {
  assert(loop);
  assert(loop->implement_data);

  ll_epoll_loop_t *ep_loop = (ll_epoll_loop_t *)loop->implement_data;
  int timeout = tv ? tv->tv_sec*1000 + tv->tv_usec / 1000 : -1;
  int nevents = 0, i;

  nevents = epoll_wait(ep_loop->epfd, ep_loop->ep_events, loop->max_events, timeout);
  if (nevents < 0) {
    return -1;
  }
  LL_DEBUG ("%d events happens", nevents);

  for (i = 0; i < nevents; i++) {
    struct epoll_event *ep_event = ep_loop->ep_events + i;
    
    int flags = 0;

    if (ep_event->events & EPOLLIN) flags  |= LL_EVENT_READ;
    if (ep_event->events & EPOLLOUT ||
        ep_event->events & EPOLLERR ||
        ep_event->events & EPOLLHUP)
      flags |= LL_EVENT_WRITE;

    ll_event_t *event = (ll_event_t *) ep_event->data.ptr;
    event->flags = flags;
    ll_list_append(&loop->fire_event_list, &event->fire_node);
  }

  return nevents;
}

int
_epoll_destroy(ll_event_loop_t *loop) {
  assert(loop);
  assert(loop->implement_data);

  ll_epoll_loop_t *ep_loop = (ll_epoll_loop_t *)loop->implement_data;

  free (ep_loop->ep_events);
  close (ep_loop->epfd);
}

#ifdef HAVE_EPOLL
ll_event_if_t g_event_if = {
  _epoll_init,
  _epoll_add_event,
  _epoll_del_event,
  _epoll_process,
  _epoll_destroy
};
#endif
