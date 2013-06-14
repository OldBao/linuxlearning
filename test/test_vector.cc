#include "test_vector.h"

TEST_F(LLVectorTest, TestInteger){
  ll_vector_t* vector;
  int ret;
  int elem;
  int boundary;

  for (boundary = 0; boundary < 10; boundary++){
    vector = ll_vector_create(sizeof(int));
    ASSERT_TRUE(vector != NULL);

    for (elem = 0; elem < boundary; elem++){
      ret = ll_vector_append(vector, &elem);
      
      ASSERT_EQ(ret, LL_OK);
    }
    ASSERT_EQ(vector->length,(ll_vector_size_t) boundary);
    
    int *pelem;
  
    for (elem = 0; elem < boundary; elem++) {
      pelem = (int *)ll_vector_get(vector,elem);
      ASSERT_TRUE(pelem != NULL);    
      ASSERT_EQ(*pelem, elem);
    }

    ll_vector_destroy(vector, NULL);
  }

}

int
str_free(void *data) {
  char **str = (char **) data;
  free(*str);
  return LL_OK;
}

TEST_F(LLVectorTest, TestChar) {
  ll_vector_t* vector;
  int i;
  int ret;

  vector = ll_vector_create(sizeof(char *));

  ASSERT_TRUE(vector!=NULL);

  char *elem;
  
  for (i = 0; i < 10; i++){
    elem = (char *)malloc(16);
    memset(elem, 0, 16);
    snprintf(elem, 16, "ELEM%d", i);
    ret = ll_vector_append(vector, &elem);
    ASSERT_EQ(ret, LL_OK);
  }

  ASSERT_EQ(vector->length,(ll_vector_size_t)10);

  char **pelem;
  char buf[16];
  for (i = 0; i < 10; i++){
    pelem = (char **)ll_vector_get(vector, i);
    snprintf(buf, 16, "ELEM%d", i);
    ASSERT_TRUE(pelem != NULL);
    ASSERT_TRUE(*pelem != NULL);
    ASSERT_STREQ(*pelem, buf);
  }

  ll_vector_destroy(vector, str_free);
}


TEST_F(LLVectorTest, TestStruct) {
  struct test{
    int a;
    char b;
  } elem[10], *pelem;

  int i;
  ll_vector_t *vector = ll_vector_create(sizeof(struct test));
  int ret;

  for (i = 0; i < 10; i++){
    elem[i].a = i;
    elem[i].b = 'a' + i;
    
    ret = ll_vector_append(vector, &elem[i]);
    ASSERT_EQ(ret, LL_OK);
  }

  for (i = 0; i < 10; i++) {
    pelem = (struct test *)ll_vector_get(vector, i);
    ASSERT_TRUE(pelem != NULL);
    ASSERT_EQ(pelem->a, i);
    ASSERT_EQ(pelem->b, i + 'a');
  }
}

TEST_F(LLVectorTest, TestNegativeIndex) {
  ll_vector_t *vector;

  vector = ll_vector_create(sizeof(int));

  ASSERT_TRUE(vector != NULL);

  int i;
  int ret;
  int *pelem;

  for (i = 0; i < 10; i++) {
    ret = ll_vector_append(vector, &i);
    ASSERT_EQ(ret, LL_OK);
  }
  for (i = -10; i < 0; i++) { 
    pelem = (int *)ll_vector_get(vector, i);
    ASSERT_TRUE(pelem != NULL);
    ASSERT_EQ(*pelem, i+10);
  }

  pelem = (int *)ll_vector_get(vector, -11);
  ASSERT_TRUE(pelem == NULL);

  pelem = (int *)ll_vector_get(vector, 10);
  ASSERT_TRUE(pelem == NULL);
}

TEST_F(LLVectorTest, TestDelete) {
  int a;
  ll_vector_t *vector;
  int ret;
  int *p;

  vector = ll_vector_create(sizeof(int));

  ASSERT_TRUE(vector != NULL);

  a = 1;

  ret = ll_vector_append(vector, &a);
  
  ASSERT_EQ(ret, LL_OK);

  a = 2;

  ret = ll_vector_append(vector, &a);

  ASSERT_EQ(ret ,LL_OK);

  ASSERT_EQ(vector->length, 2);

  ll_vector_delete_at(vector, 0);

  p = (int *)ll_vector_get(vector, 0);

  ASSERT_EQ(*p, 2);

  ASSERT_EQ(vector->length, 1);

  ll_vector_delete_at(vector, 0);

  ASSERT_EQ(vector->length, 0);
}


TEST_F(LLVectorTest, TestIncreasing) {
  int ret;
  int i, *p;
  ll_vector_t *vector = ll_vector_create(sizeof(int));

  for (i = 0; i < 10000; i++) {
    ret = ll_vector_append(vector, &i);
    ASSERT_EQ(ret, LL_OK);
  }

  for (i = 0; i < 10000; i++) {
    p = (int *)ll_vector_get(vector, i);
    ASSERT_EQ(*p, i);
  }
  ASSERT_EQ(vector->length, 10000);
  
  ll_vector_destroy(vector, NULL);
}

TEST_F(LLVectorTest, TestAsStack) {
  ll_stack_t *stack;
  int         elem, *pelem;

  stack = ll_stack_create(sizeof(int));

  ASSERT_TRUE(stack != NULL);
  for(elem = 0; elem < 10000; elem++) {
    ll_stack_push(stack, &elem);
  }

  for(elem = 9999; elem >= 0; elem--) {
    pelem = (int *)ll_stack_top(stack);
  
    ASSERT_TRUE(LL_OK == ll_stack_pop(stack));

    ASSERT_EQ(*pelem, elem);
  }

  ASSERT_TRUE(ll_stack_top(stack) == NULL);

  ll_stack_destroy(stack, NULL);
}

