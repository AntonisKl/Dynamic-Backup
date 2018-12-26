#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct DirTreeNode {
    char* name;
    struct stat* iNodeP;
    struct DirTreeNode* dirTreeNodes;
} DirTreeNode;

#endif