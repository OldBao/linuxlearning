#ifndef LL_SOCKET_H_
#define LL_SOCKET_H_

#include <assert.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ll_types.h>

typedef struct ll_socket_s ll_socket_t;

#define LL_MAX_ADDR_LEN 512

#define LL_SOCKET_TYPE_TCP 0
#define LL_SOCKET_TYPE_UDP 1
#define LL_SOCKET_TYPE_UNIX 2


struct ll_socket_s {
  int fd;

  uint32_t listen : 1;
  
  uint32_t nonblocking : 1;

  uint32_t type : 2; //TCP, UDP, UNIX STREAM

  char addr_str[LL_MAX_ADDR_LEN];

  int backlog;

  struct sockaddr sockaddr;
  socklen_t sockaddr_len;
};


int ll_init_tcp_server_socket(ll_socket_t *sock, const char *addr, int backlog);
int ll_socket_close(ll_socket_t *socket);
int ll_socket_destroy(ll_socket_t *socket);

int ll_getaddr(int proto, const char *addr, struct sockaddr *sockaddr, socklen_t *len);
int ll_socket_set_reuse(ll_socket_t *socket);
int ll_socket_set_nonblocking(ll_socket_t *socket);

#endif
