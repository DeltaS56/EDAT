#include <stdlib.h>

#include "types.h"
#include "stack.h"

/**
* @brief: Merges two stacks
*
* @param sin1, first input stack
* @param sin2, second input stack
* @param sout, result stack
* @return The function returns OK or ERROR
**/
Status mergeStacks (Stack *sin1, Stack *sin2, Stack *sout) {
    void *e = NULL;
    Stack *ps = NULL;

    if(!sin1 || !sin2 || !sout) {
        return ERROR;
    }

    while(stack_isEmpty(sin1) == FALSE && stack_isEmpty(sin2) == FALSE) {
        if((*(int*)stack_top(sin1)) > (*(int*)stack_top(sin2))) {
            e = stack_pop(sin1);

        } else {
            e = stack_pop(sin2);
        }
        stack_push(sout, e);
    }

    if(stack_isEmpty(sin1) == TRUE) {
        ps = sin2;
    } else {
        ps = sin1;
    }

    while(stack_isEmpty(ps) == FALSE) {
        e = stack_pop(ps);
        stack_push(sout, e);
    }

    return OK;  
}




