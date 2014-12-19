/**
 * @file   test_assert.h
 * @author Gethin Zhang <zhangguanxing01@baidu.com>
 * @date   Fri Dec 19 19:18:22 2014
 * 
 * @brief  
 * 
 * 
 */

#ifndef TEST_LL_ASSERT_H_
#define TEST_LL_ASSERT_H_

#include "ll_test.h"
#include "ll_log.h"

class LLAssertTest : public ::testing::Test {
 protected:
  virtual ~LLAssertTest() {}

  void SetUp(){
  	ll_log_init(&g_logger, NULL, NULL, LL_LOG_LEVEL_ERROR);
  }

  void TearDown(){}
};

#endif

