//
// Created by Rui.Mu on 2019-02-02.
//

#ifndef COMP9024_STACK_H
#define COMP9024_STACK_H

typedef struct StackRep *Stack;

Stack newStack();             // set up empty stack
void  dropStack(Stack);       // remove unwanted stack
int   StackIsEmpty(Stack);    // check whether stack is empty
void  StackPush(Stack, int);  // insert an int on top of stack
int   StackPop(Stack);        // remove int from top of stack

#endif //COMP9024_STACK_H
