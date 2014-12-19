#ifndef LL_TIME_H_
#define LL_TIME_H_

#include <sys/time.h>
#include <time.h>

typedef struct timeval ll_timeval_t;
typedef long long ll_msec_t;

#define LL_TIME_INF ((ll_time_t) -1)
#define LL_MSTIME_INF ((ll_msec_t) -1)


#define LL_TV2MS(tv) ((tv).tv_sec * 1000 + (tv).tv_usec / 1000)
ll_msec_t ll_current_mtime();
const char *ll_current_timestr();
int ll_strfmtime_r(ll_msec_t tv, char *ret, size_t retlen);

#endif
