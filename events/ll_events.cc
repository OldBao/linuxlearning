#include "ll_events.h"
#include "ll_log.h"

extern ll_event_if_t g_event_if;

int
ll_event_init(ll_event_t *event, int fd, int flags, ll_event_cb_t callback, void *private_data) {
  event->fd = fd;
  event->flags = flags;
  event->callback = callback;
  event->private_data = private_data;
}
int
ll_event_loop_init(ll_event_loop_t *loop, int size) {
  loop->max_events = size;
  ll_rbtree_init(&loop->timeout_tree);
  ll_list_init(&loop->fire_event_list);

  g_event_if.init(loop);
}

int
ll_event_loop_loop(ll_event_loop_t *loop) {
  ll_timeval_t tv;
  int nevents;

  while (1) {
    LL_DEBUG("loop");
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    nevents = g_event_if.process(loop, &tv);
    if (nevents < 0) {
      perror("process event error\n");
    }

    ll_event_t *event;
    ll_list_node_t *pnode = ll_list_head(&loop->fire_event_list), *tmp;

    while (pnode != ll_list_end(&loop->fire_event_list)) {
      event = ll_list_item(pnode, ll_event_t, fire_node);

      event->callback(loop, event);

      tmp = pnode;
      pnode = ll_list_next(pnode);
      ll_list_remove(tmp);
    }

  }
}

int
ll_event_loop_add_event(ll_event_loop_t *loop, ll_event_t *event) {
  int ret;
  ret = g_event_if.add_event(loop, event, 0);
  return ret;
}

/*
static void
timeout_tree_insert(ll_rbtree_t *tree, ll_event_t *event) {
  assert(tree);
  assert(event);

  if (!tree->root) {
    tree->root = &event->timeout_node;
  } else {
    ll_rbtree_node_t *tmp = tree->root, *parent = NULL;
    while (true) {
      if (tmp == NULL) break;
      parent = tmp;
      ll_event_t *tmpe = LL_CONTAINER_OF(tmp, ll_event_t, timeout_node);
      if ((tmpe->timeout - event->timeout) > 0){
        parent->left = &event->timeout_node;
      } else {
        parent->right = &event->timeout_node;
      }
      
      event->timeout_node.parent = parent;
      ll_rbtree_node_set_red(&event->timeout_node);
    }
  }

  ll_rbtree_fixup_insert(tree, &event->timeout_node);
}

static 
ll_event_t* timeout_tree_min(ll_rbtree_t *tree){
  assert(tree);

  if (!tree->root) return NULL;

  ll_rbtree_node_t *tmp = tree->root, *parent = NULL;
  
  while (true) {
    
  }
}

static int
timeout_tree_init(ll_rbtree_t *tree) {
  ll_rbtree_init(tree);
}

static int
timeout_tree_get_min_time(ll_rbtree_t *tree) {
  
}
*/




