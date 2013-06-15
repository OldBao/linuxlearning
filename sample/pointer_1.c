/**
 * @file   pointer_1.c
 * @author zgxcassar@gmail.com
 * @date   Sat Jun 15 22:55:49 2013
 * 
 * @brief  first usage of pointer
 * 
 * 
 */


#include <stdio.h>
#include <unistd.h>

int
main(){
  int value;
  int *pointer = &value;

  //%zu is an standard GNU extension, it's 
  printf("Address of 'value' is %zu", (size_t)&value);
  printf("Address of 'pointer' is %zu", (size_t)&pointer);

  return 0;
}
