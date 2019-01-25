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

    return nameNode;
}

INodesList* initINodesList() {
    INodesList* iNodesList = (INodesList*)malloc(sizeof(INodesList));
    iNodesList->size = 0;
    iNodesList->firstINode = NULL;

    return iNodesList;
}

INode* initINode(ino_t id, time_t lastModTime, off_t size, char* firstName) {
    INode* iNode = (INode*)malloc(sizeof(INode));
    iNode->id = id;
    iNode->lastModTime = lastModTime;
    iNode->size = size;
    iNode->namesList = initNamesList();
    iNode->prevNode = NULL;
    iNode->nextNode = NULL;
    iNode->destINodeP = NULL;

    if (firstName != NULL) {
        // add the first name of the list
        addNameNodeToNamesList(iNode->namesList, firstName);
    }

    return iNode;
}

void freeNameNode(NameNode** nameNode) {
    if ((*nameNode) == NULL)
        return;

    free((*nameNode)->name);
    (*nameNode)->name = NULL;
    free(*nameNode);
    (*nameNode) = NULL;
    return;
}

void freeNamesList(NamesList** namesList) {
    NameNode* curNameNode = (*namesList)->firstNameNode;

    while (curNameNode != NULL) {
        free(curNameNode->name);
        curNameNode->name = NULL;

        if (curNameNode->nextNode != NULL) {
            curNameNode = curNameNode->nextNode;
            free(curNameNode->prevNode);

            curNameNode->prevNode = NULL;
        } else {
            free(curNameNode);
            curNameNode = NULL;
        }
    }
    free(*namesList);
    (*namesList) = NULL;
}

void freeINode(INode** iNode) {
    if ((*iNode)->namesList != NULL) {
        freeNamesList(&(*iNode)->namesList);
    }

    (*iNode)->namesList = NULL;
    free(*iNode);
    (*iNode) = NULL;
}

INode* findINodeInINodesList(INodesList* iNodesList, ino_t id) {
    INode* curINode = iNodesList->firstINode;
    while (curINode != NULL) {
        if (curINode->id == id) {
            return curINode;
        } else if (id < curINode->id) {  // no need for searching further since the list is sorted
            return NULL;
        }

        curINode = curINode->nextNode;
    }
    printf("INode not found\n");
    return NULL;
}

NameNode* findNameNodeInNamesList(NamesList* namesList, char* name) {
    if (namesList == NULL)
        return NULL;

    NameNode* curNameNode = namesList->firstNameNode;

    while (curNameNode != NULL) {
        if (strcmp(curNameNode->name, name) == 0) {
            return curNameNode;
        } else if (strcmp(name, curNameNode->name) < 0) {  // no need for searching further since the list is sorted
            return NULL;
        }
        curNameNode = curNameNode->nextNode;
    }
    return NULL;
}

INode* addINodeToINodesList(INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size, char* firstName) {
    if (iNodesList->size == 0) {
        iNodesList->firstINode = initINode(id, lastModTime, size, firstName);

        iNodesList->size++;
        return iNodesList->firstINode;
    } else {
        INode* curINode = iNodesList->firstINode;

        if (id < curINode->id) {
            // insert at start
            INode* iNodeToInsert = initINode(id, lastModTime, size, firstName);
            iNodeToInsert->nextNode = curINode;

            curINode->prevNode = iNodeToInsert;
            iNodesList->firstINode = iNodeToInsert;
            iNodesList->size++;
            return iNodesList->firstINode;
        }
        while (curINode != NULL) {
            if (curINode->nextNode != NULL) {
                if (id < curINode->nextNode->id) {
                    INode* iNodeToInsert = initINode(id, lastModTime, size, firstName);

                    iNodeToInsert->prevNode = curINode;
                    iNodeToInsert->nextNode = curINode->nextNode;

                    curINode->nextNode->prevNode = iNodeToInsert;

                    curINode->nextNode = iNodeToInsert;
                    iNodesList->size++;

                    printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);

                    return curINode->nextNode;
                }
            } else {
                // insert at the end
                curINode->nextNode = initINode(id, lastModTime, size, firstName);
                curINode->nextNode->prevNode = curINode;

                iNodesList->size++;
                printf("Inserted i-node with id |%ju|\n\n", (uintmax_t)id);
                return curINode->nextNode;
            }

            curINode = curINode->nextNode;
        }
    }

    return NULL;  // not normal behavior
}

NameNode* addNameNodeToNamesList(NamesList* namesList, char* name) {
    if (namesList->size == 0) {
        namesList->firstNameNode = initNameNode(name);

        namesList->size++;
        printf("Inserted |%s| to NamesList\n\n", name);
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
            printf("Inserted |%s| to NamesList\n\n", name);
            return namesList->firstNameNode;
        }
        while (curNameNode != NULL) {
            if (curNameNode->nextNode != NULL) {
                if (strcmp(name, curNameNode->nextNode->name) < 0) {
                    NameNode* nameNodeToInsert = initNameNode(name);
                    nameNodeToInsert->prevNode = curNameNode;
                    nameNodeToInsert->nextNode = curNameNode->nextNode;

                    curNameNode->nextNode->prevNode = nameNodeToInsert;
                    curNameNode->nextNode = nameNodeToInsert;
                    namesList->size++;
                    printf("Inserted |%s| to NamesList\n\n", name);
                    return curNameNode->nextNode;
                }
            } else {
                // insert at the end
                curNameNode->nextNode = initNameNode(name);
                curNameNode->nextNode->prevNode = curNameNode;

                namesList->size++;
                printf("Inserted |%s| to NamesList\n\n", name);
                return curNameNode->nextNode;
            }

            curNameNode = curNameNode->nextNode;
        }
    }

    return NULL;  // not normal behavior
}

int deleteINodeFromINodesList(INodesList* iNodesList, ino_t id) {
    INode* iNodeToDelete = findINodeInINodesList(iNodesList, id);

    if (iNodeToDelete == NULL) {
        printf("INode with id %ju not found in i-nodes' list\n", (uintmax_t)id);
        return -1;
    }

    if (iNodeToDelete->id == iNodesList->firstINode->id) {
        iNodesList->firstINode = iNodeToDelete->nextNode;
    }

    if (iNodeToDelete->prevNode != NULL)
        iNodeToDelete->prevNode->nextNode = iNodeToDelete->nextNode;

    if (iNodeToDelete->nextNode != NULL)
        iNodeToDelete->nextNode->prevNode = iNodeToDelete->prevNode;

    freeINode(&iNodeToDelete);
    iNodesList->size--;

    return 0;
}

int deleteNameNodeFromNamesList(NamesList* namesList, char* name) {
    if (namesList == NULL)
        return -1;
    NameNode* nameNodeToDelete = findNameNodeInNamesList(namesList, name);

    if (nameNodeToDelete == NULL) {
        printf("Name node with name %s not found in names' list\n", name);
        return -1;
    }

    if (strcmp(nameNodeToDelete->name, namesList->firstNameNode->name) == 0) {
        namesList->firstNameNode = nameNodeToDelete->nextNode;
    }
    if (nameNodeToDelete->prevNode != NULL) {
        nameNodeToDelete->prevNode->nextNode = nameNodeToDelete->nextNode;
    }
    if (nameNodeToDelete->nextNode != NULL) {
        nameNodeToDelete->nextNode->prevNode = nameNodeToDelete->prevNode;
    }

    freeNameNode(&nameNodeToDelete);
    namesList->size--;

    return 0;
}

void deleteFileOrDirectory(char* path) {
    int pid = fork();
    if (pid == 0) {
        // printf("\n\nDeleting file with name: %s\n", path);
        char* args[] = {"rm", "-rf", path, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else if (pid == -1) {
        perror("fork error");
        exit(1);
    } else {
        wait(NULL);
    }
    wait(NULL);
}

void copyFileOrDirectory(char* sourcePath, char* destPath) {
    int pid = fork();
    if (pid == 0) {
        // printf("\n\nCopying file with name: %s\n", sourcePath);
        char* args[] = {"cp", sourcePath, destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else if (pid == -1) {
        perror("fork error");
        exit(1);
    } else {
        wait(NULL);
    }
    wait(NULL);
}

void copyDirAttributes(char* sourcePath, char* destPath) {
    int pid = fork();
    if (pid == 0) {
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
    } else if (pid == -1) {
        perror("fork error");
        exit(1);
    } else {
        wait(NULL);
    }
}

void copyFileModTime(char* sourcePath, char* destPath) {
    int pid = fork();
    if (pid == 0) {
        char reference[PATH_MAX + 12];
        strcpy(reference, "--reference=");
        strcat(reference, sourcePath);

        char* args[] = {"touch", "-m", reference, destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else if (pid == -1) {
        perror("fork error");
        exit(1);
    } else {
        wait(NULL);
    }
}

void createDirAndCopyAttributes(char* sourcePath, char* destPath) {
    int pid = fork();
    if (pid == 0) {
        char* args[] = {"mkdir", destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else if (pid == -1) {
        perror("fork error");
        exit(1);
    } else {
        wait(NULL);
    }

    copyDirAttributes(sourcePath, destPath);
}

void linkFile(char* sourcePath, char* destPath) {
    int pid = fork();
    if (pid == 0) {
        char* args[] = {"ln", sourcePath, destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else if (pid == -1) {
        perror("fork error");
        exit(1);
    } else {
        wait(NULL);
    }
}

void moveFile(char* sourcePath, char* destPath) {
    int pid = fork();
    if (pid == 0) {
        char* args[] = {"mv", sourcePath, destPath, NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else if (pid == -1) {
        perror("fork error");
        exit(1);
    } else {
        wait(NULL);
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