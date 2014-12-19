#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "ll_log.h"
#include "ll_sockets.h"

int ll_init_tcp_server_socket(ll_socket_t *sock, const char *addr, int backlog) {
  assert (sock);
  assert (addr);
  int ret;
  
  sock->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock->fd == -1)  {
    LL_PERROR ("socket");
    return -1;
  }

  ll_socket_set_reuse(sock);

  if (strlen(addr) > LL_MAX_ADDR_LEN) {
    LL_WARN ("address is too long [%d/%d]", strlen(addr), LL_MAX_ADDR_LEN);
    return -1;
  }
  strncpy (sock->addr_str, addr, LL_MAX_ADDR_LEN);
  
  if (-1 == ll_getaddr(LL_SOCKET_TYPE_TCP, addr, &sock->sockaddr, &sock->sockaddr_len)){
    return -1;
  }
  
  ret = bind(sock->fd, &sock->sockaddr, sock->sockaddr_len);
  if (-1 == ret) {
    LL_PERROR ("bind");
    return -1;
  }


  ret = listen (sock->fd, backlog);
  if (-1 == ret) {
    LL_PERROR ("listen");
    return -1;
  }

  sock->listen = 1;
  sock->type = LL_SOCKET_TYPE_TCP;

  return 0;
}


int
ll_getaddr(int proto, const char *addr, struct sockaddr *sockaddr, socklen_t* socklen) {
  assert(socket);
  assert(addr);

  char host[LL_MAX_ADDR_LEN], port[LL_MAX_ADDR_LEN];
  bzero(host, LL_MAX_ADDR_LEN);
  bzero(port, LL_MAX_ADDR_LEN);

  char* colon_pos;
  
  if (addr[0] == ':') {
    memcpy(host, "0.0.0.0", sizeof ("0.0.0.0"));
    memcpy(port, addr+1, LL_MAX_ADDR_LEN);
  } else {
    if ((colon_pos = strchr (addr, ':')) == NULL) {
      LL_WARN ("invalid addr %s [hint: use 'host:port' format]");
      return -1;
    }  
    memcpy(host, addr, colon_pos-addr);
    memcpy(port, colon_pos+1, LL_MAX_ADDR_LEN);
  }

  int ret;
  struct addrinfo  hints, *res;

  bzero (&hints, sizeof (hints));
  switch (proto) {
  case LL_SOCKET_TYPE_TCP:
    hints.ai_socktype = SOCK_STREAM;
    break;
  case LL_SOCKET_TYPE_UDP:
    hints.ai_socktype = SOCK_DGRAM;
    break;
  default:
    hints.ai_socktype= 0;
  }
    
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_INET;
  hints.ai_protocol = 0;
    
  if ((ret = getaddrinfo (host, port, &hints, &res)) != 0){
    LL_WARN ("getaddrinfo of [%s:%s] error : %s", host, port, gai_strerror(ret));
    return -1;
  }

  memcpy(sockaddr, res->ai_addr, res->ai_addrlen);
  *socklen = res->ai_addrlen;
  freeaddrinfo (res);

  return 0;
}


int
ll_socket_set_reuse(ll_socket_t *sock) {
  assert (sock);

  int opt = 1;
  return setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
}

int
ll_socket_set_nonblocking(ll_socket_t *sock) {
  int flags;
  flags = fcntl (sock->fd, F_GETFL, 0);

  if (flags < 0) {
    LL_PERROR ("fcntl get");
    return -1;
  }

  flags = fcntl(sock->fd, F_SETFL, flags | O_NONBLOCK);
  
  if (flags < 0) {
    LL_PERROR ("fcntl set");
  }

  return 0;
}

