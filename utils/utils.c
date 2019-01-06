#include "utils.h"

NamesList* initNamesList() {
    NamesList* namesList = (NamesList*)malloc(sizeof(NamesList));
    namesList->size = 0;
    namesList->firstNameNode = NULL;

    return namesList;
}

NameNode* initNameNode(char* name) {
    NameNode* nameNode = (NameNode*)malloc(sizeof(NameNode));
    nameNode->name = malloc(sizeof(name));
    strcpy(nameNode->name, name);

    return nameNode;
}

INodesList* initINodesList() {
    INodesList* iNodesList = (INodesList*)malloc(sizeof(INodesList));
    iNodesList->size = 0;
    iNodesList->firstINode = NULL;

    return iNodesList;
}

INode* initINode(ino_t id, time_t lastModTime, off_t size /*, INode* destINodeP*/, NamesList* namesList, char* firstName) {
    INode* iNode = (INode*)malloc(sizeof(INode));
    iNode->id = id;
    iNode->lastModTime = lastModTime;
    iNode->size = size;
    iNode->namesList = initNamesList();

    if (firstName != NULL) {
        addNameNodeToNamesList(iNode->namesList, firstName);
    } else if (namesList != NULL) {
        NameNode* curDestNameNode = iNode->namesList->firstNameNode;
        NameNode* curSourceNameNode = namesList->firstNameNode;

        if (curSourceNameNode != NULL) {
            iNode->namesList->firstNameNode = initNameNode(curSourceNameNode->name);
        }

        while (curSourceNameNode != NULL) {
            curDestNameNode->nextNode = initNameNode(curSourceNameNode->name);

            curSourceNameNode = curSourceNameNode->nextNode;
            curDestNameNode = curDestNameNode->nextNode;
        }
    }

    // iNode->namesList->firstNameNode = initNameNode(firstName);
    // iNode->namesList->size++;

    return iNode;
}

void freeNameNode(NameNode* nameNode) {
    free(nameNode->name);
    nameNode->name = NULL;
    free(nameNode);
    nameNode = NULL;

    return;
}

void freeNamesList(NamesList* namesList) {
    NameNode* curNameNode = namesList->firstNameNode;

    while (curNameNode != NULL) {
        free(curNameNode->name);
        curNameNode->name = NULL;
        curNameNode = curNameNode->nextNode;
        free(curNameNode->prevNode);
    }
}

void freeINode(INode* iNode) {
    freeNamesList(iNode->namesList);
    iNode->namesList = NULL;
    freeINode(iNode);
    iNode = NULL;
}

INode* findINodeInINodesList(INodesList* iNodesList, ino_t id) {
    INode* curINode = iNodesList->firstINode;
    while (curINode != NULL) {
        if (curINode->id == id) {
            return curINode;
        } else if (id < curINode->id)
            return NULL;

        curINode = curINode->nextNode;
    }

    return NULL;
}

NameNode* findNameNodeInNamesList(NamesList* namesList, char* name) {
    NameNode* curNameNode = namesList->firstNameNode;
    while (curNameNode != NULL) {
        if (strcmp(curNameNode->name, name) == 0) {
            return curNameNode;
        } else if (strcmp(name, curNameNode->name) < 0)
            return NULL;

        curNameNode = curNameNode->nextNode;
    }

    return NULL;
}

INode* addINodeToINodesList(INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size, NamesList* namesList, char* firstName) {
    if (iNodesList->size == 0) {
        iNodesList->firstINode = initINode(id, lastModTime, size, namesList, firstName);

        iNodesList->size++;
        printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);
        return iNodesList->firstINode;
    } else {
        INode* curINode = iNodesList->firstINode;

        if (id < curINode->id) {
            // insert at start
            INode* iNodeToInsert = initINode(id, lastModTime, size, namesList, firstName);
            iNodeToInsert->nextNode = curINode;

            curINode->prevNode = iNodeToInsert;
            iNodesList->firstINode = iNodeToInsert;
            iNodesList->size++;
            printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);
            return iNodesList->firstINode;
        } else {
            while (curINode != NULL) {
                if (curINode->nextNode != NULL) {
                    if (id < curINode->id) {
                        INode* iNodeToInsert = initINode(id, lastModTime, size, namesList, firstName);
                        iNodeToInsert->prevNode = curINode;
                        iNodeToInsert->nextNode = curINode->nextNode;

                        curINode->nextNode->prevNode = iNodeToInsert;
                        curINode->nextNode = iNodeToInsert;
                        iNodesList->size++;
                        printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);

                        return curINode->nextNode;
                    }
                } else {
                    curINode->nextNode = initINode(id, lastModTime, size, namesList, firstName);
                    curINode->nextNode->prevNode = curINode;

                    // iNodesList->lastINode = iNodesList->lastINode->nextNode;
                    iNodesList->size++;
                    printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);
                    return curINode->nextNode;
                }

                curINode = curINode->nextNode;
            }
        }
    }

    return NULL;  // not normal behavior
}

NameNode* addNameNodeToNamesList(NamesList* namesList, char* name) {
    if (namesList->size == 0) {
        namesList->firstNameNode = initNameNode(name);

        namesList->size++;
        printf("Inserted |%s|\n\n", name);
        return namesList->firstNameNode;
    } else {
        NameNode* curNameNode = namesList->firstNameNode;

        if (strcmp(name, curNameNode->name) < 0) {
            // insert at start
            NameNode* nameNodeToInsert = initNameNode(name);
            nameNodeToInsert->nextNode = curNameNode;

            curNameNode->prevNode = nameNodeToInsert;
            namesList->firstNameNode = nameNodeToInsert;
            namesList->size++;
            printf("Inserted |%s|\n\n", name);
            return namesList->firstNameNode;
        } else {
            while (curNameNode != NULL) {
                if (curNameNode->nextNode != NULL) {
                    if (strcmp(name, curNameNode->nextNode->name) < 0) {
                        NameNode* nameNodeToInsert = initNameNode(name);
                        nameNodeToInsert->prevNode = curNameNode;
                        nameNodeToInsert->nextNode = curNameNode->nextNode;

                        curNameNode->nextNode->prevNode = nameNodeToInsert;
                        curNameNode->nextNode = nameNodeToInsert;
                        namesList->size++;
                        printf("Inserted |%s|\n\n", name);

                        return curNameNode->nextNode;
                    }
                } else {
                    curNameNode->nextNode = initNameNode(name);
                    curNameNode->nextNode->prevNode = curNameNode;

                    // namesList->lastNameNode = namesList->lastNameNode->nextNode;
                    namesList->size++;
                    printf("Inserted |%s|\n\n", name);
                    return curNameNode->nextNode;
                }

                curNameNode = curNameNode->nextNode;
            }
        }
    }

    return NULL;  // not normal behavior
}

int deleteINodeFromINodesList(INodesList* iNodesList, ino_t id) {
    INode* iNodeToDelete = findINodeInINodesList(iNodesList, id);

    if (iNodeToDelete == NULL) {
        printf("I-node with id %ju not found in names' list\n", (uintmax_t)id);
        return -1;
    }

    if (iNodeToDelete->id == iNodesList->firstINode->id) {
        iNodesList->firstINode = iNodeToDelete->nextNode;
    }

    if (iNodeToDelete->prevNode != NULL)
        iNodeToDelete->prevNode->nextNode = iNodeToDelete->nextNode;

    if (iNodeToDelete->nextNode != NULL)
        iNodeToDelete->nextNode->prevNode = iNodeToDelete->prevNode;

    freeINode(iNodeToDelete);
    iNodesList->size--;

    return 0;
}

int deleteNameNodeFromNamesList(NamesList* namesList, char* name) {
    NameNode* nameNodeToDelete = findNameNodeInNamesList(namesList, name);

    if (nameNodeToDelete == NULL) {
        printf("Name node with name %s not found in names' list\n", name);
        return -1;
    }

    if (strcmp(nameNodeToDelete->name, namesList->firstNameNode->name) == 0) {
        namesList->firstNameNode = nameNodeToDelete->nextNode;
    }

    if (nameNodeToDelete->prevNode != NULL)
        nameNodeToDelete->prevNode->nextNode = nameNodeToDelete->nextNode;

    if (nameNodeToDelete->nextNode != NULL)
        nameNodeToDelete->nextNode->prevNode = nameNodeToDelete->prevNode;

    freeNameNode(nameNodeToDelete);
    namesList->size--;

    return 0;
}

void deleteFileOrDirectory(char* path) {
    if (fork() == 0) {
        char* args[] = {"rm", "-rf", path, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
}

void copyFileOrDirectory(char* sourcePath, char* destPath) {
    if (fork() == 0) {
        char* args[] = {"cp", sourcePath, destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
}

void handleFlags(int argc, char** argv, char** sourceDirName, char** destDirName) {
    if (argc != 3) {
        printf("Invalid flags. Exiting...\n");
        exit(1);
    }

    (*sourceDirName) = argv[1];
    (*destDirName) = argv[2];
}

// TreeNode* findFileTreeNodeInDir(TreeNode* parentDir, char* name) {
//     if (parentDir == NULL) {
//         printf("Find -> NULL parent directory given as parameter\n");
//         return NULL;
//     }

//     TreeNode* curTreeNode = parentDir->firstChildNode;

//     while (curTreeNode != NULL) {
//         if (strcmp(name, curTreeNode->name) < 0)
//             break;

//         if (curTreeNode->type == File && strcmp(name, curTreeNode->name) == 0)
//             return curTreeNode;

//         curTreeNode = curTreeNode->nextNode;
//     }

//     printf("Find -> Directory with name %s not found inside directory with name %s\n", name, curTreeNode->name);
//     return NULL;  // not found
// }
