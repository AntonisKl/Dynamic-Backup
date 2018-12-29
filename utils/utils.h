#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef enum Type {
    Directory,
    File
} Type;

typedef struct INode {
    time_t lastModTime; // in seconds
    off_t size;
    char** names;
    unsigned int namesNum;
    struct INode* destINode; // will be null for a destination i-node
    Type type;
} INode;

typedef struct TreeNode {
    char* name;
    INode* iNodeP;
    struct TreeNode* nextNode, *prevNode, *firstChildNode; // covers all different cases by setting null the variables that we don't need
    Type type;
    int contentsNum; // will be -1 for files
} TreeNode;

typedef struct DirTree {
    unsigned int size;
    TreeNode* rootNode;
} DirTree;

#endif