#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ll_vector.h"

const int DEFAULT_SIZE = 16;
const int INCREASING_FACOTR = 1; 

static inline int
ll_vector_increasing_if(ll_vector_t *vector) {
  assert(vector);

  if (vector->size << 1 > LL_SIZE_MAX) {
    return FAILURE;
  }

  vector->size  <<= 1;

  if (vector->length >= vector->size) {
    vector->data = realloc(vector->size * vector->elem_size);

    if (!vector->data) {
      return FAILURE;
    }
  }

  return LL_OK;
}

static inline void*
ll_vector_addr_in(ll_vector_t* vector, ll_vector_size_t pos) {
  //examine boudary must be done by caller
  return (char *) (vector->data) + (vector->elem_size * pos);
}

ll_vector_t*
ll_vector_create(int elemsize) {
  assert(elemsize > 0);
  
  ll_vector_t *vector;

  vector = (ll_vector_t *) malloc(sizeof(ll_vector_t));
  if (!vector) {
    return NULL;
  }

  vector->data = malloc(DEFAULT_SIZE * elemsize);
  if (!vector->data) {
    return NULL;
  }

  memset(vector->data, 0, elemsize * DEFAULT_SIZE);
  vector->elem_size = elemsize;
  vector->size     = DAFAULT_SIZE;
  vector->length   = 0;

  return vector;
}


void *
ll_vector_get(ll_vector_t *vector, int pos) {
  assert(vector);

  int real_pos;

  if (abs(pos) > vector->length) {
    return NULL;
  }

  if (pos < 0) {
    real_pos = length + pos;
  } else {
    real_pos = pos;
  }

  return ll_vector_addr_in(vector, pos, real_pos);
}



int
ll_vector_insert_after(ll_vector_t *vector, void *data, ll_vector_size_t pos) {
  assert(vector);
  assert(data);
  
  ll_vector_size_t i = vector->length - 1;

  if (pos < 0 || pos > vector->length) {
    return -1;
  }

  if (FAILURE == ll_vector_increasing_if(vector)){
    return -1;
  }

  while(i > pos) {
    memcpy(ll_vecotr_addr_in(vector, i), ll_vector_addr_in(vector, i - 1), vector->elem_size);
  }

  memcpy(ll_vector_addr_in(vector, pos), data, vector->elem_size);

  return LL_OK;
}


