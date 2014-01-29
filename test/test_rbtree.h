/**
 * @file   test_rbtree.h
 * @author zgxcassar@gmail.com
 * @date   Tues Dec 14 00:18:20 2013
 * 
 * @brief  list testes
 * 
 * 
 */

#ifndef TEST_RBTREE_H_
#define TEST_RBTREE_H_

#include <gtest/gtest.h>

#include "ll_rbtree.h"

class LLRbtreeTest : public ::testing::Test {
 protected:
  virtual ~LLRbtreeTest() {}
  
  void SetUp(){}
  void TearDown() {}
};
#endif
