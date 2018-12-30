#ifndef UTILS_H
#define UTILS_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum Type {
    Directory,
    File
} Type;

typedef struct NameNode {
    char* name;
    struct NameNode *nextNode, *prevNode;
} NameNode;

typedef struct NamesList {
    NameNode* firstNameNode;
    unsigned int size;
} NamesList;

typedef struct INode {
    time_t lastModTime;  // in seconds
    off_t size;
    NamesList* namesList;
    // char** names;
    // unsigned int namesNum;
    struct INode* destINode;  // will be null for a destination i-node
    Type type;
} INode;

NamesList* initNamesList();

NameNode* initNameNode(char* name);

void freeNameNode(NameNode* nameNode);

NameNode* findNameNodeInNamesList(NamesList* namesList, char* name);

NameNode* addNameNodeToNamesList(NamesList* namesList, char* name);

int deleteNameNodeFromNamesList(NamesList* namesList, char* name);

#endif