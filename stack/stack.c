#include "stack.h"

// StackNode* createStackNode(Vertice* treeNode, unsigned int weight) {
//     StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode));
//     stackNode->treeNode = treeNode;
//     stackNode->weight = weight;

//     return stackNode;
// }

void freeStackNode(StackNode* stackNode) {
    free(stackNode->curTreeNode->name);
    stackNode->curTreeNode->name = NULL;
    free(stackNode->curTreeNode);
    stackNode->curTreeNode = NULL;
    if (stackNode->parentDir != NULL) {
        free(stackNode->parentDir->name);
        stackNode->parentDir->name = NULL;
        free(stackNode->parentDir);
        stackNode->parentDir = NULL;
    }
    free(stackNode);
}

Stack* initStack(unsigned int maxSize) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->stackNodes = (StackNode**)malloc(maxSize * sizeof(StackNode));

    stack->size = 0;
    stack->maxSize = maxSize;
    return stack;
}

void destroyStack(Stack* stack) {
    if (!stackIsEmpty(stack)) {
        unsigned int i = 0;
        for (i = 0; i < stack->size; i++) {
            if (stack->stackNodes[i] != NULL) {
                freeStackNode(stack->stackNodes[i]);
                stack->stackNodes[i] = NULL;
            }
        }
    }
    free(stack->stackNodes);
    stack->stackNodes = NULL;
    free(stack);
}

char stackIsEmpty(Stack* stack) {
    return stack->size == 0;
}

void copyTreeNode(TreeNode* to, TreeNode* from) {
    to->visited = from->visited;
    to->firstChildNode = from->firstChildNode;
    to->nextNode = from->nextNode;
    to->prevNode = from->prevNode;
    to->iNodeP = from->iNodeP;
    to->contentsNum = from->contentsNum;
    to->type = from->type;

    return;
}

void pushToStack(Stack* stack, TreeNode* treeNode, TreeNode* parentDir) {
    if (stack->size >= stack->maxSize) {
        printf("Stack is full\n");
        return;
    }

    stack->stackNodes[stack->size] = (StackNode*)malloc(sizeof(StackNode));

    TreeNode* treeNodeToBePushed = initTreeNode(treeNode->name, treeNode->iNodeP, treeNode->type);

    copyTreeNode(treeNodeToBePushed, treeNode);

    TreeNode* parentDirToBePushed = NULL;
    if (parentDir != NULL) {
        parentDirToBePushed = initTreeNode(parentDir->name, parentDir->iNodeP, parentDir->type);
        copyTreeNode(parentDirToBePushed, parentDir);
    }

    stack->stackNodes[stack->size]->curTreeNode = treeNodeToBePushed;
    stack->stackNodes[stack->size]->parentDir = parentDirToBePushed;
    stack->size++;

    return;
}

void popFromStack(Stack* stack, StackNode** poppedStackNode) {
    if (stackIsEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }

    StackNode* stackNodeToBePopped = stack->stackNodes[stack->size - 1];

    (*poppedStackNode)->curTreeNode = initTreeNode(stackNodeToBePopped->curTreeNode->name, stackNodeToBePopped->curTreeNode->iNodeP, stackNodeToBePopped->curTreeNode->type);
    copyTreeNode((*poppedStackNode)->curTreeNode, stackNodeToBePopped->curTreeNode);

    if (stackNodeToBePopped->parentDir != NULL) {
        (*poppedStackNode)->parentDir = initTreeNode(stackNodeToBePopped->parentDir->name, stackNodeToBePopped->curTreeNode->iNodeP, stackNodeToBePopped->curTreeNode->type);
        copyTreeNode((*poppedStackNode)->parentDir, stackNodeToBePopped->parentDir);
    } else
        (*poppedStackNode)->parentDir = NULL;

    freeStackNode(stackNodeToBePopped);
    stackNodeToBePopped = NULL;
    stack->size--;
}

StackNode* peekFromStack(Stack* stack) {
    return stack->stackNodes[stack->size - 1];
}