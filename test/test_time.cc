#include "test_time.h"
#include "ll_time.h"

TEST_F (LLTimeTest, TestFtime) {
  ll_msec_t t = 1418982845000;

  char buffer[1024];
  ll_strfmtime_r (t, buffer, 1024);

  ASSERT_STREQ("2014-12-19 17:54:05", buffer);
}
