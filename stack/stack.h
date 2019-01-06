#ifndef STACK_H
#define STACK_H

#include "../tree/tree.h"
#include "../utils/utils.h"

typedef struct StackNode {
    TreeNode *curTreeNode, *parentDir;
} StackNode;

typedef struct Stack {
    unsigned int size, maxSize;
    StackNode** stackNodes;
} Stack;

void freeStackNode(StackNode* stackNode);

Stack* initStack(unsigned int maxSize);

void destroyStack(Stack* stack);

char stackIsEmpty(Stack* stack);

void copyTreeNode(TreeNode* to, TreeNode* from);

void pushToStack(Stack* stack, TreeNode* treeNode, TreeNode* parentDir);

void popFromStack(Stack* stack, StackNode** poppedStackNode);

StackNode* peekFromStack(Stack* stack);

#endif