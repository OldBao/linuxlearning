#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include "ll_events.h"
#include "ll_log.h"

int
onRead(ll_event_loop_t *loop, ll_event_t *event) {
  char buf[512];
  bzero(buf, sizeof(buf));
  read(event->fd, buf, 512);
  LL_TRACE ("read %d : %s\n", event->fd, buf);
}

int
onAccept(ll_event_loop_t *loop, ll_event_t *event){
  LL_TRACE ("event happens in %d\n", event->fd);
  struct sockaddr_in addr;
  
  socklen_t len;
  int ret = accept(event->fd, (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    perror("accept");
    return 0;
  }

  int flags;
  if ((flags = fcntl(ret, F_GETFL, 0)) == -1)
    flags = 0;
  fcntl(ret, F_SETFL, flags | O_NONBLOCK);

  ll_event_t *ev = (ll_event_t *) malloc(sizeof (ll_event_t));

  ll_event_init(ev, ret, LL_EVENT_READ, onRead, NULL);

  ll_event_loop_add_event(loop, ev);
}


int
main(){
  ll_event_loop_t loop;

  ll_log_init(&g_logger, NULL, NULL, LL_LOG_LEVEL_TRACE);

  ll_event_loop_init(&loop, 32);

  ll_event_t ev;
  
  int fd;
  fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  
  if (-1 == bind(fd, (struct sockaddr *)&addr, sizeof (struct sockaddr))){
    perror("bind");
    return -1;
  }

  if (-1 == listen(fd, 1024)){
    perror("listen");
    return -1;
  }

  int flags;
  if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
    flags = 0;
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);

  ll_event_init(&ev, fd, LL_EVENT_READ, onAccept, NULL);

  ll_event_loop_add_event(&loop, &ev);

  ll_event_loop_loop(&loop);
}
