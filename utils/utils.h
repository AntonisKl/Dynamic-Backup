#ifndef UTILS_H
#define UTILS_H

#include <dirent.h>
#include <inttypes.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum Type {
    Directory,
    File
} Type;

typedef struct NameNode {
    char* name;  // path name
    struct NameNode *nextNode, *prevNode;
} NameNode;

// NamesList: alphabetically sorted list of names
typedef struct NamesList {
    NameNode* firstNameNode;
    unsigned int size;
} NamesList;

typedef struct INode {
    ino_t id;                  // the i-node number/id as read from the struct stat
    time_t lastModTime;        // last modification time as read from the struct stat
    off_t size;                // size of the File/Directory as read from the struct stat
    NamesList* namesList;      // path names' list
    struct INode* destINodeP;  // will be null for a destination i-node
    Type type;                 // can be File or Directory
    struct INode *nextNode, *prevNode;
} INode;

// INodesList: sorted list of INodes in ascending order of id
typedef struct INodesList {
    INode* firstINode;
    unsigned int size;
} INodesList;

// initialize NamesList
NamesList* initNamesList();

// initialize NameNode
NameNode* initNameNode(char* name);

// initialize INodesList
INodesList* initINodesList();

// initialize INode
INode* initINode(ino_t id, time_t lastModTime, off_t size /*, INode* destINodeP*/, char* firstName);

// free NameNode
void freeNameNode(NameNode** nameNode);

// free NamesList
void freeNamesList(NamesList** namesList);

// free INode
void freeINode(INode** iNode);

// find an INode in INodesList by id
INode* findINodeInINodesList(INodesList* iNodesList, ino_t id);

// find a NameNode in NamesList by name
NameNode* findNameNodeInNamesList(NamesList* namesList, char* name);

// add INode to INodesList and maintain the sort order
INode* addINodeToINodesList(INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size, char* firstName);

// add NameNode to NamesList and maintain the sort order
NameNode* addNameNodeToNamesList(NamesList* namesList, char* name);

int deleteINodeFromINodesList(INodesList* iNodesList, ino_t id);

int deleteNameNodeFromNamesList(NamesList* namesList, char* name);

// calls exec to delete a file or directory and waits until the operation is done
void deleteFileOrDirectory(char* path);

// calls exec to copy a file or directory and waits until the operation is done
void copyFileOrDirectory(char* sourcePath, char* destPath);

// calls exec to copy a directory's attributes/metadata and waits until the operation is done
void copyDirAttributes(char* sourcePath, char* destPath);

// creates a new directory and copies the source directory's attributes/metadata and waits until the operation is done
void createDirAndCopyAttributes(char* sourcePath, char* destPath);

// links a file to destPath
void linkFile(char* sourcePath, char* destPath);

// moves a file from sourcePath to destPath
void moveFile(char* sourcePath, char* destPath);

// handles arguments given as input
void handleFlags(int argc, char** argv, char** sourceDirName, char** destDirName);

#endif