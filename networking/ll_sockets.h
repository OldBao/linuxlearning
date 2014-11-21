#ifndef LL_SOCKET_H_
#define LL_SOCKET_H_

#include <assert.h>
#include <strings.h>

#include <ll_types.h>

typedef struct ll_socket_s ll_socket_t;

#define LL_SOCKET_TYPE_TCP 0
#define LL_SOCKET_TYPE_UDP 1
#define LL_SOCKET_TYPE_UNIX 2

struct ll_socket_s {
  int fd;

  uint32_t listen : 1;
  
  uint32_t nonblocking : 1;

  uint32_t type : 2; //TCP, UDP, UNIX STREAM
};


ll_socket_t * ll_create_tcp_server_socket(uint16_t port, const char *addr);
int ll_socket_listen(ll_socket_t *socket);
int ll_socket_close(ll_socket_t *socket);


#endif
