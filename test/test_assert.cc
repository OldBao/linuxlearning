/**
 * @file   test_assert.cc
 * @author Gethin Zhang <zhangguanxing01@baidu.com>
 * @date   Fri Dec 19 19:18:01 2014
 * 
 * @brief  
 * 
 * 
 */
#include "test_assert.h"
#include "ll_assert.h"

TEST_F (LLAssertTest, TestAssert) {
  ll_assert (1 == 0, "wocao");
  EXPECT_EXIT (ll_assert(1 == 0, "1 can't be equal with 0"), ::testing::KilledBySignal(SIGABRT), "");
}

TEST_F (LLAssertTest, TestAssertNot) {
  
}


