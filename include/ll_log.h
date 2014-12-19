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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "ll_macro.h"

typedef struct ll_log_s ll_log_t;

#define LL_LOG_LEVEL_TRACE 0
#define LL_LOG_LEVEL_DEBUG 1
#define LL_LOG_LEVEL_INFO  2
#define LL_LOG_LEVEL_WARN  3
#define LL_LOG_LEVEL_ERROR 4

#ifdef COLOR_LOG

#define LL_LOG_WITH_FL(logger, level, FG, fmt, arg...)	do{             \
    ll_log(logger, level,  FG, __FILE__ , __LL_LINE(__LINE__), __FUNCTION__ , \
           fmt, ##arg);}                                                \
  while(0)

#define LL_TRACE_EX(logger, fmt, arg...) 	LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_TRACE,  LL_ANSI_CYAN,   fmt, ##arg)
#define LL_DEBUG_EX(logger, fmt, arg...) 	LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_DEBUG,  LL_ANSI_GREEN,  fmt, ##arg)
#define LL_INFO_EX(logger,  fmt, arg...) 	LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_INFO,   LL_ANSI_BLUE,   fmt, ##arg)
#define LL_WARN_EX(logger,  fmt, arg...)        LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_WARN,   LL_ANSI_MAGINA, fmt, ##arg)
#define LL_ERROR_EX(logger, fmt, arg...) 	LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_ERROR,  LL_ANSI_RED,    fmt, ##arg)

#else
#define LL_LOG_WITH_FL(logger,level, fmt, arg...)	do{      \
    ll_log(logger, level,  "[" __FILE__ ":" __LL_LINE(__LINE__) "]"   \
           fmt                                                          \
           "\n", ##arg);}                                               \
  while(0)

#define LL_TRACE_EX(logger, fmt, arg...) 	LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_TRACE, 	fmt, ##arg)
#define LL_DEBUG_EX(logger, fmt, arg...) 	LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_DEBUG, 	fmt, ##arg)
#define LL_INFO_EX(logger, fmt, arg...) 	LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_INFO,	fmt, ##arg)
#define LL_WARN_EX(logger, fmt, arg...)         LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_WARN,	fmt, ##arg)
#define LL_ERROR_EX(loggerfmt, arg...) 	        LL_LOG_WITH_FL(logger, LL_LOG_LEVEL_ERROR,	fmt, ##arg)

#endif

#define LL_TRACE(fmt, arg...) LL_TRACE_EX(&g_logger, fmt, ##arg)
#define LL_DEBUG(fmt, arg...) LL_DEBUG_EX(&g_logger, fmt, ##arg)
#define LL_INFO(fmt, arg...) LL_INFO_EX(&g_logger, fmt, ##arg)
#define LL_WARN(fmt, arg...) LL_WARN_EX(&g_logger, fmt, ##arg)
#define LL_ERROR(fmt, arg...) LL_ERROR_EX(&g_logger, fmt, ##arg)
#define LL_PERROR(fmt, arg...) do {                             \
    char buf[1024];                                             \
    snprintf(buf, 1024, "%s error: %s", fmt, strerror(errno));  \
    LL_WARN(buf, ##arg);                                        \
  } while (0)


struct ll_log_s {
  int   filefd;
  FILE* fp;
  char  logger_name[12];
  char  path[LL_PATH_MAX];
  char  name[LL_PATH_MAX];
  int   log_level;
#ifdef LL_MULTHREAD
  pthread_mutex_t mutex;
#endif

  //void (*on_log_changed)(void);
};


int  ll_log_init(ll_log_t *logger, const char *path, const char *name, int log_level);
int  ll_set_log_file(ll_log_t* logger, const char* path, const char *name);
void ll_set_log_level(ll_log_t* logger, int level);
void ll_log(ll_log_t* logger, int log_level, const char* fgcolor, const char *fn, const char *line, const char *function, const char* fmt,...);

extern ll_log_t g_logger;

#endif /* LL_LOG_H_ */
