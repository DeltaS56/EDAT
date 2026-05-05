#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 2 // init stack capacity
#define FCT_CAPACITY 2 // multiply the stack capacity

struct _Stack {
  void **item; /*!<Static array of elements*/
  int top; /*!<index of the top element in the stack*/
  int capacity; /*!<xcapacity of the stack*/
};

Stack * stack_init ();
void stack_free (Stack *s);
Status stack_push (Stack *s, const void *ele);
void * stack_pop (Stack *s);
void * stack_top (const Stack *s);
Bool stack_isEmpty (const Stack *s);
size_t stack_size (const Stack *s);
int stack_print(FILE* fp, const Stack *s,  P_stack_ele_print f);
