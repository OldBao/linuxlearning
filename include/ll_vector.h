/**
 * @file   ll_vector.h
 * @author zgxcassar@gmail.com
 * @date   Fri Jun  7 19:05:00 2013
 * 
 * @brief  
 * 
 * 
 */

#ifndef LL_VECTOR_H_
#define LL_VECTOR_H_

#include "ll_core.h"
#ifdef LL_VECTOR64
typedef int64_t ll_vector_size_t;
#define LL_SIZE_MAX LONG_MAX
#else
typedef int32_t  ll_vector_size_t;
#define LL_SIZE_MAX INT_MAX
#endif

typedef struct ll_vector_s ll_vector_t;

struct ll_vector_s {
  void *data;      //storage area

  ll_vector_size_t   elem_size;  //one elem size
  ll_vector_size_t   size;      //maximum elem size
  ll_vector_size_t   length;    //current elem count
};

/** 
 * 
 * 
 * 
 * @return return NULL if malloc data error 
 */
ll_vector_t* ll_vector_create(int elemSize);


/** 
 * 
 * 
 * @param vector 
 * @param pos    if pos is negative, it will get last elem, just like python does
 * 
 * @return       NULL if pos is invalid
 */
void * ll_vector_get(ll_vector_t *vector, ll_vector_size_t pos);

/** 
 * copy insert, a easy understand but not effcient way
 * 
 * @param vector 
 * @param data   data size should equals to elemSize
 * @param pos    insert after which position
 * 
 * @return       LL_OK on success
 *               LL_FAILURE on failure
 */
int ll_vector_insert_after(ll_vector_t *vector, void *data, ll_vector_size_t pos);


/** 
 * 
 * 
 * @param vector 
 * @param data 
 * @param pos 
 * 
 * @return 
 */
int ll_vector_append(ll_vector_t *vector, void* data);
/** 
 * 
 * 
 * @param vector 
 * @param data 
 * @param pos    
 * 
 * @return 
 * @brief   act sames as insert_after except insert elem after pos
 */
int ll_vector_insert_before(ll_vector_t *vector, void *data, ll_vector_size_t pos);

/** 
 * 
 * 
 * @param vector 
 * @param data 
 * @param pos 
 * 
 * @return  LL_OK on success
 *          LL_FAILURE on failure
 */
int ll_vector_delete_at(ll_vector_t *vector, ll_vector_size_t pos);

/** 
 * traverse the vector, if cb is NULL, do nothing at all
 * 
 * @param vector 
 * @param cb     general callback functions
 * 
 * @return 
 */
int ll_vector_traverse(ll_vector_t *vector, ll_general_callback_ft cb);

/** 
 * 
 * 
 * @param vector 
 * @param start 
 * @param len 
 * 
 * @return 
 */
ll_vector_t* ll_vector_sub(ll_vector_t *vector, ll_vector_size_t start, ll_vector_size_t end);

/** 
 * destroy the vector, all data will be free individually, if cb is NULL, just free  
 * vector storage
 * 
 * @param vector 
 * 
 * @return 
 */
void ll_vector_destroy(ll_vector_t *vector, ll_general_callback_ft cb);

#endif
