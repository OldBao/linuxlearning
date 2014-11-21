#ifndef LL_BUFFER_H_
#define LL_BUFFER_H_

typedef struct ll_trunk_s ll_trunk_t;
typedef struct ll_buffer_s ll_buffer_t;

int ll_buffer_init(ll_buffer_t *buffer);
int ll_buffer_read(ll_buffer_t *buffer, char *user_buf, size_t len);
int ll_buffer_append(ll_buffer_t *buffer, char* user_buf, size_t len);
int ll_buffer_clear(ll_buffer_t *buffer);
int ll_buffer_destroy(ll_buffer_t *buff);

#define ll_buffer_size(b) ((b)->size)

struct ll_trunk_s {
  void  *data;
  size_t size;
  size_t readpos, write_pos;
  
  ll_list_node_t node;
};


struct ll_buffer_t {
  int size, tc;
  size_t readpos, writepos;

  ll_trunk_t head;
};
#endif
