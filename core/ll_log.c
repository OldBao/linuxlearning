/**
 * @file   ll_log.c
 * @author zgxcassar@gmail.com
 * @date   Fri Jun  6 23:36:57 2013
 * 
 * @brief  
 * 
 * 
 */

#include <stdarg.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "ll_log.h"

#define LOG_FILE_MODE  O_CREAT | O_APPEND | O_WRONLY;

static const char* maplevel[] = {
			"DEBUG",
			"NOTICE",
			"WARNING",
			"ERROR"
			};

static const char* level2str(int level) {
	if (level > LL_LOG_ERROR || level < LL_LOG_DEBUG)
		return "Unknown";
	else
		return maplevel[level];
}

/** 
 * 
 * 
 * @param path 
 * 
 * @return 0 on success
 *        -1 on fail
 */
int 
gx_set_log_file(ll_log_t *log, const char* path, const char* name){
	int logfd;

	

	return 0;
}

void gx_set_log_level(int level) {
	if (level > GX_LOG_ERROR || level < GX_LOG_TRACE)
		return;
	gx_globals.g_log->log_level = level;
}

void gx_log(int log_level, const char* fmt,...) {
	va_list ap;
	if (log_level < gx_globals.g_log->log_level) {
		return;
	}

	if (gx_globals.g_log->f_fp) {
		struct timeval timeofday;
		gettimeofday(&timeofday, NULL);
		time_t now = timeofday.tv_sec;
		struct tm newtime;
		struct tm *date = localtime_r(&now, &newtime);
		pthread_mutex_lock(&(gx_globals.g_log->mutex));
		fprintf(gx_globals.g_log->f_fp, "[%s]", level2str(log_level));
		fprintf(gx_globals.g_log->f_fp, "[%02d-%02d %02d:%02d:%02d]",
				date->tm_mon + 1,
				date->tm_mday,
				date->tm_hour,
				date->tm_min,
				date->tm_sec);
		va_start(ap, fmt);
		vfprintf(gx_globals.g_log->f_fp, fmt, ap);
		pthread_mutex_unlock(&(gx_globals.g_log->mutex));
	}
	else{
		perror("fdopen");
	}

}
