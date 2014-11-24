/**
 * @file   ll_log.c
 * @author zgxcassar@gmail.com
 * @date   Fri Jun  6 23:36:57 2013
 * 
 * @brief  
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "ll_log.h"

ll_log_t g_logger;

#define LOG_FILE_MODE  O_CREAT | O_APPEND | O_WRONLY
static const char* maplevel[] = {
  "TRACE",
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR"
};

static const char* level2str(int level) {
  if (level > LL_LOG_LEVEL_ERROR || level < LL_LOG_LEVEL_TRACE)
    return "Unknown";
  else
    return maplevel[level];
}

int
ll_log_init(ll_log_t *logger, const char *path, const char *name, int log_level) {
  if (0 != ll_set_log_file(logger, path, name)) {
    return -1;
  }

#ifdef LL_MULTHREAD
  if (0 != pthread_mutex_init(&logger->mutex, NULL)){
    perror ("pthread mutex init");
    return -1;
  }
#endif

  strncpy (logger->logger_name, "DEFAULT", 12);
  
  return 0;
}

int
ll_log_set_name(ll_log_t *logger, const char *name) {
  if (name) 
    strncpy(logger->logger_name, name, 11);
}

int 
ll_set_log_file(ll_log_t *logger, const char* path, const char* name){
  char fullpath[LL_PATH_MAX];
  memset(fullpath, 0, sizeof (fullpath));
  if (!path && !name) {
    logger->filefd = STDOUT_FILENO;
  } else {
    if (path) {
      strncat(fullpath, path, LL_PATH_MAX);
    }
    if (name) {
      strncat(fullpath, name, LL_PATH_MAX);
    }
    logger->filefd = open(fullpath, LOG_FILE_MODE);
    if (logger->filefd == -1) {
      perror ("open");
      return -1;
    }
  }

  logger->fp = fdopen(STDOUT_FILENO, "a+");
  if (!logger->fp) {
    perror ("fdopen");
    return -1;
  }

  return 0;
}

void 
ll_set_log_level(ll_log_t *logger, int level) {
  if (level > LL_LOG_LEVEL_ERROR || level < LL_LOG_LEVEL_DEBUG)
    return;
  logger->log_level = level;
}

void ll_log(ll_log_t *logger, int log_level, const char* fgcolor, const char *fn, const char *line, const char *fmt...) {
  va_list ap;
  if (log_level < logger->log_level) {
    return;
  }
  
  if (logger->fp) {
    struct timeval timeofday;
    gettimeofday(&timeofday, NULL);
    time_t now = timeofday.tv_sec;
    struct tm newtime;
    struct tm *date = localtime_r(&now, &newtime);
#ifdef LL_MULTHREAD
    pthread_mutex_lock(&(logger.mutex));
#endif
    fprintf(logger->fp, "[%s][%s][%02d-%02d %02d:%02d:%02d][%s:%s]%s\t", 
            logger->logger_name, 
            level2str(log_level),
            date->tm_mon + 1,
            date->tm_mday,
            date->tm_hour,
            date->tm_min,
            date->tm_sec,
            fn,
            line,
            fgcolor);

    va_start(ap, fmt);
    vfprintf(logger->fp, fmt, ap);
    if (fn != NULL){
      fprintf (logger->fp, "%s\n", LL_ANSI_CLRALL);
    }
#ifdef LL_MULTHREAD
    pthread_mutex_unlock(&(logger.mutex));
#endif
  }
  else{
    printf ("please set log file first\n");
  }
}
