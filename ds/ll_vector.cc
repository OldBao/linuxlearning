#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "ll_vector.h"

const int DEFAULT_SIZE = 16;

static inline int
ll_vector_increasing_if(ll_vector_t *vector) {
  assert(vector);

  if (vector->size << 1 > LL_SIZE_MAX) {
    return LL_FAILURE;
  }

  vector->size  <<= 1;

  if (vector->length >= vector->size) {
    vector->data = realloc(vector->data, vector->size * vector->elem_size);

    if (!vector->data) {
      return LL_FAILURE;
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
  vector->size     = DEFAULT_SIZE;
  vector->length   = 0;

  return vector;
}


void *
ll_vector_get(ll_vector_t *vector, ll_vector_size_t pos) {
  assert(vector);

  ll_vector_size_t real_pos;

  if (pos < 0) {
    real_pos = vector->length + pos;
  } else {
    real_pos = pos;
  }

  if (real_pos >= vector->length) {
    return NULL;
  }

  return ll_vector_addr_in(vector, real_pos);
}


int 
ll_vector_append(ll_vector_t* vector, void *data) {
  return ll_vector_insert_before(vector, data, vector->length);
}

int
ll_vector_insert_after(ll_vector_t *vector, void *data, ll_vector_size_t pos) {
  assert(vector);
  assert(data);
  
  ll_vector_size_t i;

  if (pos < 0 || pos > vector->length) {
    return -1;
  }

  if (LL_FAILURE == ll_vector_increasing_if(vector)){
    return -1;
  }

  //be carefull vector->length is unsigned int
  if (vector->length > 0){
    for(i = vector->length; i > pos; i--) {
      memcpy(ll_vector_addr_in(vector, i), 
	     ll_vector_addr_in(vector, i - 1), 
	     vector->elem_size);
      i--;
    }
  }

  memcpy(ll_vector_addr_in(vector, pos), data, vector->elem_size);

  vector->length++;

  return LL_OK;
}


int
ll_vector_insert_before(ll_vector_t *vector, void *data, ll_vector_size_t pos) {
  if (pos == 0){
    return ll_vector_insert_after(vector, data, 0);
  } else {
    return ll_vector_insert_after(vector, data, pos - 1);
  }
}


int 
ll_vector_delete_at(ll_vector_t *vector, void *data, ll_vector_size_t pos) {
  assert(vector);
  assert(data);


  ll_vector_size_t i;

  if (pos < 0 || pos >= vector->length) {
    return LL_FAILURE;
  }

  if (vector->length > 0){
    for(i = pos; i < vector->length; i++) {
      memcpy(ll_vector_addr_in(vector, i), 
	     ll_vector_addr_in(vector, i+1), 
	     vector->elem_size);
    }
  } else {
    memset(ll_vector_addr_in(vector, 0), 0, vector->elem_size);
  }

  vector->length--;
  return LL_OK;
}

int
ll_vector_traverse(ll_vector_t *vector, ll_general_callback_ft cb) {
  assert(vector);

  if (cb == NULL) {
    return LL_OK;
  }

  ll_vector_size_t i;

  for (i = 0; i < vector->length; i++) {
    cb(ll_vector_addr_in(vector,i));
  }

  return LL_OK;
}

/*
ll_vector_t* 
ll_vector_sub(ll_vector_t* vector, int start, int end) {
  ll_vector_t* sub_vector = ll_vector_create
}

*/

void
ll_vector_destroy(ll_vector_t *vector, ll_general_callback_ft cb) {
  assert(vector);

  if (cb != NULL) {
    ll_vector_size_t i;

    for (i = 0; i < vector->length; i++) {
      cb(ll_vector_addr_in(vector,i));
    }

  }

  if (vector->data)
    free(vector->data);
  if (vector)
    free(vector);
}
