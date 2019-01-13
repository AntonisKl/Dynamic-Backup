#include "utils.h"

NamesList* initNamesList() {
    NamesList* namesList = (NamesList*)malloc(sizeof(NamesList));
    namesList->size = 0;
    namesList->firstNameNode = NULL;

    return namesList;
}

NameNode* initNameNode(char* name) {
    NameNode* nameNode = (NameNode*)malloc(sizeof(NameNode));
    nameNode->name = (char*)malloc(PATH_MAX);
    strcpy(nameNode->name, name);
    nameNode->nextNode = NULL;
    nameNode->prevNode = NULL;

    printf("\n\n\n\n                                    JUST INITIALIZED A NAME NODE WITH NAME: %s\n\n\n\n\n", nameNode->name);
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
    iNode->prevNode = NULL;
    iNode->nextNode = NULL;
    iNode->destINodeP = NULL;

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

void freeNameNode(NameNode** nameNode) {
    // if (strcmp((*nameNode)->name, "/home/antonis/Documents/Dynamic-Backup/destination/dummyfolder") == 0)
    //     return;
    printf("name node freed1!\n");
    if ((*nameNode) == NULL)
        return;

    printf("name node name: %s\n", (*nameNode)->name);

    free((*nameNode)->name);
    (*nameNode)->name = NULL;
    free(*nameNode);
    (*nameNode) = NULL;
    printf("name node freed!\n");
    return;
}

void freeNamesList(NamesList** namesList) {
    // printf("inside\n");
    if ((*namesList) == NULL)
        return;
    // printf("inside1\n");

    NameNode* curNameNode = (*namesList)->firstNameNode;

    while (curNameNode != NULL) {
        free(curNameNode->name);
        curNameNode->name = NULL;
        // printf("inside2\n");

        if (curNameNode->nextNode != NULL) {
            curNameNode = curNameNode->nextNode;
            free(curNameNode->prevNode);
            // printf("inside4\n");

            curNameNode->prevNode = NULL;
        } else {
            free(curNameNode);
            curNameNode = NULL;
        }
    }
    free(*namesList);
    (*namesList) = NULL;
    printf("names list freed!\n");
}

void freeINode(INode** iNode) {
    printf("in free1\n");
    freeNamesList(&(*iNode)->namesList);
    printf("in free2\n");

    (*iNode)->namesList = NULL;
    free(*iNode);
    (*iNode) = NULL;
    printf("i-node freed!\n");
}

INode* findINodeInINodesList(INodesList* iNodesList, ino_t id) {
    printf("in find i-node\n");
    INode* curINode = iNodesList->firstINode;
    while (curINode != NULL) {
        printf("id: %ju\n\n\n\n", curINode->id);
        if (curINode->id == id) {
            return curINode;
        } else if (id < curINode->id) {
            return NULL;
        }

        curINode = curINode->nextNode;
    }
    printf("node not found\n");
    return NULL;
}

NameNode* findNameNodeInNamesList(NamesList* namesList, char* name) {
    NameNode* curNameNode = namesList->firstNameNode;

    while (curNameNode != NULL) {
        printf("in while in find name: %s\n", curNameNode->name);
        if (strcmp(curNameNode->name, name) == 0) {
            return curNameNode;
        } else if (strcmp(name, curNameNode->name) < 0) {
            return NULL;
        }

        curNameNode = curNameNode->nextNode;
    }

    return NULL;
}

INode* addINodeToINodesList(INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size, NamesList* namesList, char* firstName) {
    printf("in addINodeToINodesList\n");
    if (iNodesList->size == 0) {
        printf("in addINodeToINodesList: inodeslist's size= 0\n");

        iNodesList->firstINode = initINode(id, lastModTime, size, namesList, firstName);

        iNodesList->size++;
        printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);
        return iNodesList->firstINode;
    } else {
        printf("in addINodeToINodesList: inodeslist's size != 0\n");

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
        }
        //  else {
        while (curINode != NULL) {
            if (curINode->nextNode != NULL) {
                if (id < curINode->nextNode->id) {
                    printf("in addINodeToINodesList: id < curINode->id\n");

                    INode* iNodeToInsert = initINode(id, lastModTime, size, namesList, firstName);
                    printf("in addINodeToINodesList: after initINode\n");

                    iNodeToInsert->prevNode = curINode;
                    iNodeToInsert->nextNode = curINode->nextNode;
                    printf("in addINodeToINodesList: after initINode1\n");
                    if (curINode->nextNode != NULL) printf("heeeeeeeeeeeeeeeeeeeelloo\n");

                    curINode->nextNode->prevNode = iNodeToInsert;
                    printf("in addINodeToINodesList: after initINode2\n");

                    curINode->nextNode = iNodeToInsert;
                    iNodesList->size++;

                    printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);
                    printf("in addINodeToINodesList: after initINode3\n");

                    return curINode->nextNode;
                }
            } else {
                printf("in addINodeToINodesList: id >= curINode->id\n");

                curINode->nextNode = initINode(id, lastModTime, size, namesList, firstName);
                curINode->nextNode->prevNode = curINode;

                // iNodesList->lastINode = iNodesList->lastINode->nextNode;
                iNodesList->size++;
                printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);
                return curINode->nextNode;
            }

            curINode = curINode->nextNode;
        }
        // }
    }

    return NULL;  // not normal behavior
}

NameNode* addNameNodeToNamesList(NamesList* namesList, char* name) {
    printf("\n\n\nADDING NAME NODE WITH NAME %s TO NAMES LIST\n\n\n\n", name);
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
        }
        // else {
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
        // }
    }

    return NULL;  // not normal behavior
}

int deleteINodeFromINodesList(INodesList* iNodesList, ino_t id) {
    INode* iNodeToDelete = findINodeInINodesList(iNodesList, id);
    // printf("after in function\n");

    if (iNodeToDelete == NULL) {
        printf("I-node with id %ju not found in i-nodes' list\n", (uintmax_t)id);
        return -1;
    }
    // printf("after in function1\n");

    if (iNodeToDelete->id == iNodesList->firstINode->id) {
        iNodesList->firstINode = iNodeToDelete->nextNode;
    }
    // printf("after in function2\n");

    if (iNodeToDelete->prevNode != NULL)
        iNodeToDelete->prevNode->nextNode = iNodeToDelete->nextNode;
    // printf("after in function3\n");

    if (iNodeToDelete->nextNode != NULL)
        iNodeToDelete->nextNode->prevNode = iNodeToDelete->prevNode;
    // printf("after in function4\n");

    freeINode(&iNodeToDelete);
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

    freeNameNode(&nameNodeToDelete);
    namesList->size--;

    return 0;
}

void deleteFileOrDirectory(char* path) {
    if (fork() == 0) {
        printf("\n\nDeleting file with name: %s\n\n\n", path);
        char* args[] = {"rm", "-rf", path, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
    wait(NULL);
}

void copyFileOrDirectory(char* sourcePath, char* destPath) {
    if (fork() == 0) {
        printf("\n\nCopying file with name: %s\n\n\n", sourcePath);
        char* args[] = {"cp", sourcePath, destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
    wait(NULL);
}

void copyDirAttributes(char* sourcePath, char* destPath) {
    // rsync -ptgo -A -X -d --no-recursive --exclude=* first-dir/ second-dir

    if (fork() == 0) {
        char sourcePathComplete[PATH_MAX];
        strcpy(sourcePathComplete, sourcePath);
        strcat(sourcePathComplete, "/");

        char destPathComplete[PATH_MAX];
        strcpy(destPathComplete, destPath);
        strcat(destPathComplete, "/");

        char* args[] = {"rsync", "-ptgo", "-A", "-X", "-d", "--no-recursive", "--exclude=*", sourcePathComplete, destPathComplete, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
    wait(NULL);
}

void createDirAndCopyAttributes(char* sourcePath, char* destPath) {
    if (fork() == 0) {
        printf("\n\nCreating directoryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy\n\n\n");
        char* args[] = {"mkdir", destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
    wait(NULL);

    copyDirAttributes(sourcePath, destPath);
}

void renameFileOrDirectory(char* oldPathName, char* newPathName) {
    if (fork() == 0) {
        char* args[] = {"mv", oldPathName, newPathName, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
    wait(NULL);
}

void linkFileOrDirectory(char* sourcePath, char* destPath) {
    if (fork() == 0) {
        char* args[] = {"ln", sourcePath, destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    }
    wait(NULL);
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
