#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <json/json.h>

#include <iconv.h>
#include "ipostag.h"
#include "isegment.h"
#include "scwdef.h"
#include "ll_events.h"
#include "ll_log.h"
#include "ll_sockets.h"
#define BUFSIZE 1024*1024

typedef struct nshead_s {
  unsigned short id;
  unsigned short version;
  unsigned int log_id;
  char provider[16];
  unsigned int magic_num;
  unsigned int reserverd;
  unsigned int body_len;
} nshead_t;
#define NSHEAD_SIZE sizeof (nshead_t)

typedef struct _buf {
  char *data;
  int len, capacity;
} buf_t;


scw_worddict_t* pwdict;

void
process_buffer(buf_t *buf) {
  scw_out_t *pout = NULL;
  u_int scw_out_flag;

  json_object *obj = json_tokener_parse(buf->data);
  if (!obj) {
    LL_WARN("parse error");
    return;
  }

  const char* article = NULL;
  json_object_object_foreach (obj, key1, value1) {
    if (strcmp(key1, "query") == 0) {
      json_object_object_foreach (value1, key2, value2) {
        if (strcmp(key2, "article") == 0) {
          article = json_object_get_string(value2);
        }
      }
    }
  }

  if (!article) {
    LL_WARN("can't get article");
    return;
  }


  scw_out_flag = SCW_OUT_ALL | SCW_OUT_PROP;
  if ((pout = scw_create_out(10000, scw_out_flag)) == NULL) {
    LL_WARN ("error init output buffer");
    return;
  }

  scw_segment_words(pwdict, pout, article, strlen(article)+1, LANGTYPE_SIMP_CHINESE);
#define MAX_TOKEN 10240
  token_t tokens[MAX_TOKEN];
  int i;

  int count = scw_get_token_1(pout, SCW_OUT_WPCOMP | SCW_OUT_PROP , tokens,MAX_TOKEN);
  tag_postag(tokens, count);
  // view word phrase compond result
  LL_TRACE("============== Segment Result =============\n");
  for( i=0; i<count; i++){
    LL_TRACE("%s {%d} [%d,%d]  ", tokens[i].buffer, tokens[i].type, tokens[i].prop.m_lprop,tokens[i].prop.m_hprop); // output property
  }

  /*
  count = scw_get_token_1(pout,SCW_OUT_BASIC,tokens,MAX_TOKEN);
  // view basic word sep result
  LL_TRACE("============== Basic Word Result =============\n");
  for( i=0; i<count; i++) {
    LL_TRACE("%s  ", tokens[i].buffer);
  }

  count = scw_get_token_1(pout,SCW_OUT_SUBPH,tokens,MAX_TOKEN); //fprintf(stderr,"count of sub phrase = %d\n",count);
  // view sub phrase result
  LL_TRACE("============== Sub Phrase Result =============\n");
  for( i=0; i<count; i++){
    LL_TRACE("%s  ", tokens[i].buffer);
  }

  count = scw_get_token_1(pout,SCW_OUT_HUMANNAME,tokens,MAX_TOKEN);
  // view NER result
  LL_TRACE("============== Human Name Result =============\n");
  for( i=0; i<count; i++) {
    LL_TRACE("%s  ", tokens[i].buffer);
  }

  count = scw_get_token_1(pout,SCW_OUT_BOOKNAME,tokens,MAX_TOKEN);
  // view the book name result
  LL_TRACE("============== Book Name Result =============\n");
  for( i=0; i<count; i++){
    LL_TRACE("%s  ", tokens[i].buffer);
  }
  */

}

void
buffer_trans(buf_t *inbuf, buf_t *outbuf, const char *fr, const char *to) {
  /*
    iconv_t ic = iconv_open("utf8", "gbk");
    if (ic == (iconv_t) -1) {
      if (errno == EINVAL) {
        
      }
    }
    size_t avail = BUFSIZE;
    iconv(ic, (char **)&buffer, (size_t *)&len, (char **)&encode_buffer, &avail);
    LL_INFO ("msg: [%d][%s]", avail, encode_buffer);  
  */
}


int 
onTimeout(ll_event_loop_t *loop, ll_event_t *event) {
    LL_TRACE("timeout happens");
    return 0;
}


int
onRead(ll_event_loop_t *loop, ll_event_t *event) {
  LL_DEBUG("onRead");

  buf_t *wbuf = (buf_t *) event->private_data;

  char buffer[BUFSIZE], encode_buffer[BUFSIZE];
  
  int len;
  bzero(buffer, sizeof(buffer));

  len = read(event->fd, buffer, wbuf->capacity);
  switch (len) {
  case 0 :
    LL_INFO ("remote closed");
    ll_event_loop_del_event(loop, event);
    break;
  case -1:
    LL_PERROR("read");
    ll_event_loop_del_event(loop, event);
    break;
  default:
    nshead_t *n = (nshead_t *)buffer;
    if (n->magic_num != 0xfb709394) {
      LL_WARN ("nshead magic num not right %x", n->magic_num);
    }

    LL_INFO ("%u %u %u %s %x %d", n->id, n->version, n->log_id, n->provider, n->magic_num, n->body_len);
    
    LL_INFO ("origin read [%d][%s]", len-NSHEAD_SIZE, buffer+NSHEAD_SIZE);

    int _f;
    if ((_f = open("packet.txt", O_CREAT | O_WRONLY)) < 0) {
      LL_PERROR ("open packet.txt");
    }  else {
      write(_f, buffer+NSHEAD_SIZE, len-NSHEAD_SIZE);
      close(_f);
    }

    memcpy (wbuf->data, buffer+NSHEAD_SIZE, len-NSHEAD_SIZE);
    wbuf->len = len-NSHEAD_SIZE;
    process_buffer(wbuf);
    ll_event_loop_add_event(loop, event, LL_EVENT_WRITE);
    break;
  }

}

int
onWrite(ll_event_loop_t *loop, ll_event_t *event) {
  LL_DEBUG ("onWrite");
  int len;
  buf_t *wbuf = (buf_t *) event->private_data;

  if (wbuf->len > 0) {
    len = write (event->fd, wbuf->data, wbuf->len);
    
    switch (len) {
    case -1:
      LL_PERROR("write");
      break;
    default:
      wbuf->len -= len;
      memcpy(wbuf->data, wbuf->data+len, wbuf->len);
      LL_INFO("write %d, left %d", len, wbuf->len);
    }
  }

  ll_event_loop_del_event(loop, event, LL_EVENT_WRITE);
}


int
onAccept(ll_event_loop_t *loop, ll_event_t *event) {
  LL_TRACE("on Accpet");

  struct sockaddr addr;
  socklen_t len = sizeof (struct sockaddr_in);
  int ret = accept(event->fd, (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    LL_PERROR("accept");
    return 0;
  }

  ll_event_t *ev = (ll_event_t *) malloc(sizeof (ll_event_t));

  buf_t* buf = (buf_t *) malloc(sizeof(buf_t));
  buf->data = (char *) malloc(BUFSIZE);
  buf->capacity = BUFSIZE;
  buf->len = 0;

  ll_event_cb_t cb = {onRead, onWrite, onTimeout};
  ll_event_init(ev, ret, cb, buf);

  ll_event_loop_add_event(loop, ev, LL_EVENT_READ);
  return 0;
}

int
main(int argc, char **argv) {
  ll_event_loop_t loop;

  ll_log_init(&g_logger, "./log/", "extract.log", LL_LOG_LEVEL_TRACE);

  ll_event_loop_init(&loop, 32);

  ll_socket_t server_socket;

  if (argc != 3) {
    LL_ERROR ("./newsseg <dict> <postag>");
    return -1;
  }

  if (0 != tag_open(argv[2])){
    LL_WARN ("load tag dict error");
    return -1;
  }
  
  if ((pwdict = scw_load_worddict(argv[1])) == NULL) {
    LL_WARN ("load word dict error");
    return -1;
  }

  
  if (-1 == ll_init_tcp_server_socket(&server_socket, ":8041", 32)){
    LL_ERROR ("create socket error");
    return -1;
  }

  ll_socket_set_nonblocking(&server_socket);

  ll_event_t ev;
  ll_event_cb_t cb= {onAccept, NULL, NULL};
  ll_event_init(&ev, server_socket.fd, cb, NULL);
  
  ll_event_loop_add_event(&loop, &ev, LL_EVENT_READ);
  
  return ll_event_loop_loop(&loop);
}
