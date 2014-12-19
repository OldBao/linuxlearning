#ifndef LL_ASSERT_H_
#define LL_ASSERT_H_

#include <stdlib.h>

#include "ll_log.h"

#ifdef NDEBUG
#define ll_assert(condition, fmt...)
#else
#define ll_assert(condition, ...) {                                     \
    if (!(condition)) {                                                 \
      LL_ERROR ("Assertion (" #condition  ") Fail : " __VA_ARGS__ );   \
      abort ();                                                         \
    }                                                                   \
  }

#endif


#endif
