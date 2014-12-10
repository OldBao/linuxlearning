#include "ll_time.h"

ll_msec_t 
ll_current_mtime(){
  ll_timeval_t tv;
  gettimeofday(&tv, NULL);
  return LL_TV2MS(tv);
}
