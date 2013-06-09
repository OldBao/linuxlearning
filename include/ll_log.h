/**
 * @file   ll_log.h
 * @author zgxcassar@gmail.com
 * @date   Fri Jun  6 10:01:26 2013
 * 
 * @brief  a log interface  
 * 
 * 
 */

#ifndef LL_LOG_H_
#define LL_LOG_H_

#include <stdint.h>

#include "ll_core.h"

const u_int  LL_LOG_DEBUG   = 1;
const u_int  LL_LOG_INFO    = 2;
const u_int  LL_LOG_WARNING = 3;
const u_int  LL_LOG_ERROR   = 4;

typedef struct ll_log_s ll_log_t;

#ifdef COLOR_LOG

#define LL_LOG_WITH_FL(level, FG, fmt, arg...)	do{		 \
    ll_log(level,  "[" BG __FILE__ "]"				 \
                   "[" __LL_LINE(__LINE__) "]"			 \
                   "[" __FUNCTION__ "]:"                         \
                   fmt "\n" LL_ANSI_CLEARALL, ##arg);		 \
    while(0)

#define LL_DEBUG(fmt, arg...) 	LL_LOG_WITH_FL(LL_LOG_DEBUG,  LL_ANSI_GREEN,  fmt, ##arg)
#define LL_INFO(fmt, arg...) 	LL_LOG_WITH_FL(LL_LOG_NOTICE, LL_ANSI_BLUE,   fmt, ##arg)
#define LL_WARNING(fmt, arg...) LL_LOG_WITH_FL(LL_LOG_WARNING,LL_ANSI_MAGINA, fmt, ##arg)
#define LL_ERROR(fmt, arg...) 	LL_LOG_WITH_FL(LL_LOG_ERROR,  LL_ANSI_RED,    fmt, ##arg)

#else
#define LL_LOG_WITH_FL(level, fmt, arg...) do{				\
    ll_log(level, "[" __FILE__ "]"					\
	   "[" __LL_LINE(__LINE__) "]"					\
	   "[" __FUNCTION__ "]:"					\
	   fmt "\n", ##arg);						\
  }while(0)

#define LL_DEBUG(fmt, arg...) 	LL_LOG_WITH_FL(LL_LOG_DEBUG, 	fmt, ##arg)
#define LL_INFO(fmt, arg...) 	LL_LOG_WITH_FL(LL_LOG_NOTICE,	fmt, ##arg)
#define LL_WARNING(fmt, arg...) LL_LOG_WITH_FL(LL_LOG_WARNING,	fmt, ##arg)
#define LL_ERROR(fmt, arg...) 	LL_LOG_WITH_FL(LL_LOG_ERROR,	fmt, ##arg)

#endif

struct ll_log_s {
  int   filefd;
  FILE* f_fp;
  char  path[LL_PATH_MAX];
  char  name[LL_PATH_MAX];
  int   log_level;
  int   enable_color_log;

  int   roll_max;
  int   if_check;
  
  int   notify_fd;

  pthread_mutex_t mutex;

  void (*on_log_changed)(void);
};

int  ll_set_log_file(const char* path);
void ll_set_log_level(u_int level);
void ll_log(int log_level, const char* fmt,...);

#endif /* LL_LOG_H_ */