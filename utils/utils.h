#ifndef UTILS_H
#define UTILS_H

#include <dirent.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/inotify.h>
#include <signal.h>
#include <libgen.h>

typedef enum Type {
    Directory,
    File
} Type;

typedef struct NameNode {
    char* name;  // path name
    struct NameNode *nextNode, *prevNode;
} NameNode;

typedef struct NamesList {
    NameNode* firstNameNode;
    unsigned int size;
} NamesList;

typedef struct INode {
    ino_t id;
    time_t lastModTime;  // in seconds
    off_t size;
    NamesList* namesList; // path names
    // char** names;
    // unsigned int namesNum;
    struct INode* destINodeP;  // will be null for a destination i-node
    Type type;
    struct INode *nextNode, *prevNode;
} INode;

typedef struct INodesList {
    INode* firstINode;
    unsigned int size;
} INodesList;

NamesList* initNamesList();

NameNode* initNameNode(char* name);

INodesList* initINodesList();

INode* initINode(ino_t id, time_t lastModTime, off_t size /*, INode* destINodeP*/, NamesList* namesList, char* firstName);

void freeNameNode(NameNode** nameNode);

void freeNamesList(NamesList** namesList);

void freeINode(INode** iNode);

INode* findINodeInINodesList(INodesList* iNodesList, ino_t id);

NameNode* findNameNodeInNamesList(NamesList* namesList, char* name);

INode* addINodeToINodesList(INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size, NamesList* namesList, char* firstName);

NameNode* addNameNodeToNamesList(NamesList* namesList, char* name);

int deleteINodeFromINodesList(INodesList* iNodesList, ino_t id);

int deleteNameNodeFromNamesList(NamesList* namesList, char* name);

void deleteFileOrDirectory(char* path);

void copyFileOrDirectory(char* sourcePath, char* destPath);

void copyDirAttributes(char* sourcePath, char* destPath);

void createDirAndCopyAttributes(char* sourcePath, char* destPath);

void renameFileOrDirectory(char* oldPathName, char* newPathName);

void linkFile(char* sourcePath, char* destPath);

void handleFlags(int argc, char** argv, char** sourceDirName, char** destDirName);

#endif