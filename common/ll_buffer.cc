#include "buffer.h"

int _ll_trunk_create();
int _ll_trunk_free(ll_trunk_t* tr);
int _ll_trunk_free_between(ll_trunk_t *start, ll_trunk_t *end);


int
ll_buffer_init(ll_buffer_t *buffer){
  buffer->size = buffer->readpos = buffer->writepos = 0;
  ll_list_init(&buffer->trunk_head);
}



int
ll_buffer_write(ll_buffer_t *buffer, cosnt char* user_buf, size_t size) {
  assert(buffer);
  assert(user_buf);

  if (size > )
}

