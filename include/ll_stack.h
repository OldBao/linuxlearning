/**
 * @file   ll_stack.h
 * @author zgxcassar@gmail.com
 * @date   Fri Jun 13 23:42:21 2013
 * 
 * @brief  a vector based stack
 * 
 * 
 */
#ifndef LL_STACK_H_
#define LL_STACK_H_

#include "ll_vector.h"

typedef ll_vector_t ll_stack_t;

//这里只实现了基于动态数组的栈，其实简单的栈和队列就是标准数组的一个操作子集
#define ll_stack_create ll_vector_create
#define ll_stack_push   ll_vector_append
#define ll_stack_pop(stack)    ll_vector_delete_at(stack, -1)
#define ll_stack_top(stack)    ll_vector_get(stack, -1)
#define ll_stack_destroy       ll_vector_destroy

#endif
