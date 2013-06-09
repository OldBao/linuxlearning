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

TEST_F(LLVectorTest, TestChar) {
  ll_vector_t* vector;
  int i;
  int ret;

  vector = ll_vector_create(sizeof(const char *));

  ASSERT_TRUE(vector!=NULL);

  char elem[10][16];
  
  for (i = 0; i < 10; i++){
    memset(elem[i], 0, 16);
    snprintf(elem[i], 16, "ELEM%d", i);
    ret = ll_vector_append(vector, elem[i]);
    ASSERT_EQ(ret, LL_OK);
  }

  ASSERT_EQ(vector->length,(ll_vector_size_t)10);

  char **pelem;
  char buf[16];
  for (i = 0; i < 10; i++){
    pelem = (char **)ll_vector_get(vector, 0);
    snprintf(buf, 16, "ELEM%d", i);
    ASSERT_TRUE(pelem != NULL);
    ASSERT_TRUE(*pelem != NULL);
    ASSERT_STREQ(*pelem, buf);
  }
}

/*
TEST_F(LLVectorTest, TestString) {
  ll_vector_t* vector;
  int i;

  vector = ll_vector_create(sizeof(char) * 16);

  ASSERT_TRUE(vector!=NULL);

  char elem[10][16];
  
  for (i = 0; i < 10; i++){
    memset(elem[i], 0, 16);
    snprintf(elem[i], 16, "ELEM%d", i);
    ll_vector_append(vector, elem[i]);
  }

  ASSERT_TRUE(vector->length, 10);

  char **pelem;
  char buf[16];
  for (i = 0; i < 10; i++){
    pelem = (char **)ll_vector_get(vector, 0);
    snprintf(buf, 16, "ELEM%d", i);
    ASSERT_TRUE(pelem != NULL);
    ASSERT_TRUE(*pelem != NULL);
    ASSERT_STREQ(*pelem, buf);
  }  
}
*/

TEST_F(LLVectorTest, TestStruct) {
  struct {
    int a;
    char b;
  } elem1, elem2, elem3;

  ll_vector_t *vector = ll_vector_create(sizeof(elem1));

  elem1.a = 1; elem2.a = 2; elem3.a = 3;
  elem1.b = 'a'; elem2.b = 'b'; elem3.b = 'c';
  
  ll_vector_append(vector, &elem1);
  ll_vector_append(vector, &elem2);
  ll_vector_append(vector, &elem3);
}
