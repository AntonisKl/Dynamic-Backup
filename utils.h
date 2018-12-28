#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct INode {
    struct timespec lastModTime;
    off_t size;
    char** names;
    unsigned int namesNum;
    struct INode* destINode; // will be null for a destination i-node
} INode;

typedef struct DirTreeNode {
    char* name;
    struct INode* iNodeP;
    struct DirTreeNode* nextNode, *prevNode, *firstChildNode; // covers all different cases by setting null the variables that we don't need
} DirTreeNode;

#endif