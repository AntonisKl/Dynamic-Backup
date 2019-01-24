#include "tree.h"

DirTree* initDirTree(char* rootName, char* pathName, INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size) {
    DirTree* dirTree = (DirTree*)malloc(sizeof(DirTree));  // must be freed

    // NamesList* namesList = initNamesList();
    // printf("hello\n");
    // addNameNodeToNamesList(namesList, rootName);
    // printf("hello1\n");

    INode* createdINode = addINodeToINodesList(iNodesList, id, lastModTime, size, NULL, pathName);
    dirTree->rootNode = initTreeNode(rootName, pathName, createdINode, Directory, NULL);
    dirTree->size = 1;

    return dirTree;
}

TreeNode* initTreeNode(char* name, char* pathName, INode* iNodeP, Type type, NamesList* namesList) {  // namesList != NULL only when calling this function for stack
    TreeNode* treeNode = (TreeNode*)malloc(sizeof(TreeNode));
    treeNode->name = (char*)malloc(PATH_MAX);
    strcpy(treeNode->name, name);

    treeNode->pathName = (char*)malloc(PATH_MAX);
    strcpy(treeNode->pathName, pathName);
    printf("\nNAME1 = %s\n\n", treeNode->name);

    if (namesList == NULL) {
        // printf("paaaaaaaaaaaaaaaaaaaaaaaasssssssssssssed\n");
        treeNode->iNodeP = /*existingINode == NULL ? initINode(iNodeP->lastModTime, iNodeP->size, name)*/ iNodeP /*: existingINode*/;
    } else {
        treeNode->iNodeP = initINode(iNodeP->id, iNodeP->lastModTime, iNodeP->size, namesList, NULL);
    }

    treeNode->iNodeP->destINodeP = NULL;  // this will be changed after this function's call
    treeNode->type = type;
    treeNode->prevNode = NULL;
    treeNode->nextNode = NULL;
    treeNode->firstChildNode = NULL;
    if (type == Directory)
        treeNode->contentsNum = 0;
    else
        treeNode->contentsNum = -1;
    // printf("\nNAME2 = %s\n\n", treeNode->name);

    treeNode->visited = 0;
    // if (existingINode == NULL) {
    // addNameNodeToNamesList(treeNode->iNodeP->namesList, name);
    // }
    // treeNode->iNodeP->names                  // add current name to names LIST not array

    return treeNode;
}

void freeTreeNode(TreeNode** treeNode, INodesList* iNodesList) {
    int deleteRet = deleteNameNodeFromNamesList((*treeNode)->iNodeP->namesList, (*treeNode)->pathName);
    if (deleteRet == -1)
        return;
    printf("here\n\n\n");
    if ((*treeNode)->iNodeP->namesList->size == 0) {
        deleteINodeFromINodesList(iNodesList, (*treeNode)->iNodeP->id);
    }
    printf("after\n");
    // treeNode->iNodeP->namesNum--;
    //treeNode->iNodeP->names       // delete from names array|||| maybe make the array a list
    //                              // maybe delete the name and -- the namesNum in the destINode too (if it is != null of course) !!!!!!!!!!!!!

    //                              // maybe make the i-nodes' array a list too to be able to delete the corresponding i-node if namesNum == 0
    free((*treeNode)->name);
    (*treeNode)->name = NULL;
    free((*treeNode)->pathName);
    (*treeNode)->pathName = NULL;
    //                                                          MAYBE HERE SEE IF THERE ARE INODES THAT POINT TO THIS FILE/DIRECTORY AND DELETE THEM TOO
    free((*treeNode));
    (*treeNode) = NULL;
    return;
}

WdAndTreeNodesList* initWdAndTreeNodesList() {
    WdAndTreeNodesList* wdAndTreeNodesList = (WdAndTreeNodesList*)malloc(sizeof(WdAndTreeNodesList));
    wdAndTreeNodesList->size = 0;
    wdAndTreeNodesList->firstNode = NULL;

    return wdAndTreeNodesList;
}

WatchDescAndTreeNode* initWatchDescAndTreeNode(int wd, TreeNode* sourceTreeNode, TreeNode* destTreeNode, TreeNode* sourceParentDir, TreeNode* destParentDir) {
    WatchDescAndTreeNode* watchDescAndTreeNode = (WatchDescAndTreeNode*)malloc(sizeof(WatchDescAndTreeNode));
    watchDescAndTreeNode->wd = wd;
    watchDescAndTreeNode->sourceTreeNodeP = sourceTreeNode;
    watchDescAndTreeNode->destTreeNodeP = destTreeNode;
    watchDescAndTreeNode->sourceParentDirP = sourceParentDir;
    watchDescAndTreeNode->destParentDirP = destParentDir;
    watchDescAndTreeNode->nextNode = NULL;
    watchDescAndTreeNode->prevNode = NULL;
    // watchDescAndTreeNode->sourceTreeNodeP = NULL;
    // watchDescAndTreeNode->destTreeNodeP = NULL;

    // printf("\n\n\n\n                                    JUST INITIALIZED A NAME NODE WITH NAME: %s\n\n\n\n\n", watchDescAndTreeNode->name);
    return watchDescAndTreeNode;
}

void freeWatchDescAndTreeNode(WatchDescAndTreeNode** watchDescAndTreeNode) {
    // if (strcmp((*watchDescAndTreeNode)->name, "/home/antonis/Documents/Dynamic-Backup/destination/dummyfolder") == 0)
    //     return;
    // printf("name node freed1!\n");
    if ((*watchDescAndTreeNode) == NULL)
        return;

    // printf("name node name: %s\n", (*watchDescAndTreeNode)->name);

    free(*watchDescAndTreeNode);
    (*watchDescAndTreeNode) = NULL;
    // printf("name node freed!\n");
    return;
}

void freeWdAndTreeNodesList(WdAndTreeNodesList** wdAndTreeNodesList) {
    // printf("inside\n");
    if ((*wdAndTreeNodesList) == NULL)
        return;
    // printf("inside1\n");

    WatchDescAndTreeNode* curWatchDescAndTreeNode = (*wdAndTreeNodesList)->firstNode;

    while (curWatchDescAndTreeNode != NULL) {
        if (curWatchDescAndTreeNode->nextNode != NULL) {
            curWatchDescAndTreeNode = curWatchDescAndTreeNode->nextNode;
            free(curWatchDescAndTreeNode->prevNode);
            curWatchDescAndTreeNode->prevNode = NULL;
        } else {
            free(curWatchDescAndTreeNode);
            curWatchDescAndTreeNode = NULL;
        }
    }
    free(*wdAndTreeNodesList);
    (*wdAndTreeNodesList) = NULL;
    printf("wdAndTreeNodes list freed!\n");
}

WatchDescAndTreeNode* findWatchDescAndTreeNodeInWdAndTreeNodesList(WdAndTreeNodesList* wdAndTreeNodesList, int wd) {
    WatchDescAndTreeNode* curWatchDescAndTreeNode = wdAndTreeNodesList->firstNode;

    while (curWatchDescAndTreeNode != NULL) {
        printf("in while in find name: %d\n", curWatchDescAndTreeNode->wd);
        if (curWatchDescAndTreeNode->wd == wd) {
            return curWatchDescAndTreeNode;
        } else if (wd < curWatchDescAndTreeNode->wd) {
            return NULL;
        }
        curWatchDescAndTreeNode = curWatchDescAndTreeNode->nextNode;
    }
    return NULL;
}

WatchDescAndTreeNode* addWatchDescAndTreeNodeToWdAndTreeNodesList(WdAndTreeNodesList* wdAndTreeNodesList, int wd, TreeNode* sourceTreeNode, TreeNode* destTreeNode, TreeNode* sourceParentDir, TreeNode* destParentDir) {
    // printf("\n\n\nADDING NAME NODE WITH NAME %s TO NAMES LIST\n\n\n\n", name);
    if (wdAndTreeNodesList->size == 0) {
        wdAndTreeNodesList->firstNode = initWatchDescAndTreeNode(wd, sourceTreeNode, destTreeNode, sourceParentDir, destParentDir);

        wdAndTreeNodesList->size++;
        // printf("Inserted |%s|\n\n", name);
        return wdAndTreeNodesList->firstNode;
    } else {
        WatchDescAndTreeNode* curWatchDescAndTreeNode = wdAndTreeNodesList->firstNode;

        if (wd < curWatchDescAndTreeNode->wd) {
            // insert at start
            WatchDescAndTreeNode* watchDescAndTreeNodeToInsert = initWatchDescAndTreeNode(wd, sourceTreeNode, destTreeNode, sourceParentDir, destParentDir);
            watchDescAndTreeNodeToInsert->nextNode = curWatchDescAndTreeNode;

            curWatchDescAndTreeNode->prevNode = watchDescAndTreeNodeToInsert;
            wdAndTreeNodesList->firstNode = watchDescAndTreeNodeToInsert;
            wdAndTreeNodesList->size++;
            // printf("Inserted |%s|\n\n", name);
            return wdAndTreeNodesList->firstNode;
        }
        // else {
        while (curWatchDescAndTreeNode != NULL) {
            if (curWatchDescAndTreeNode->nextNode != NULL) {
                if (wd < curWatchDescAndTreeNode->nextNode->wd) {
                    WatchDescAndTreeNode* watchDescAndTreeNodeToInsert = initWatchDescAndTreeNode(wd, sourceTreeNode, destTreeNode, sourceParentDir, destParentDir);
                    watchDescAndTreeNodeToInsert->prevNode = curWatchDescAndTreeNode;
                    watchDescAndTreeNodeToInsert->nextNode = curWatchDescAndTreeNode->nextNode;

                    curWatchDescAndTreeNode->nextNode->prevNode = watchDescAndTreeNodeToInsert;
                    curWatchDescAndTreeNode->nextNode = watchDescAndTreeNodeToInsert;
                    wdAndTreeNodesList->size++;
                    // printf("Inserted |%s|\n\n", name);
                    return curWatchDescAndTreeNode->nextNode;
                }
            } else {
                curWatchDescAndTreeNode->nextNode = initWatchDescAndTreeNode(wd, sourceTreeNode, destTreeNode, sourceParentDir, destParentDir);
                curWatchDescAndTreeNode->nextNode->prevNode = curWatchDescAndTreeNode;

                // wdAndTreeNodesList->lastWatchDescAndTreeNode = wdAndTreeNodesList->lastWatchDescAndTreeNode->nextNode;
                wdAndTreeNodesList->size++;
                // printf("Inserted |%s|\n\n", name);
                return curWatchDescAndTreeNode->nextNode;
            }

            curWatchDescAndTreeNode = curWatchDescAndTreeNode->nextNode;
        }
        // }
    }

    return NULL;  // not normal behavior
}

int deleteWatchDescAndTreeNodeFromWdAndTreeNodesList(WdAndTreeNodesList* wdAndTreeNodesList, int wd) {
    WatchDescAndTreeNode* watchDescAndTreeNodeToDelete = findWatchDescAndTreeNodeInWdAndTreeNodesList(wdAndTreeNodesList, wd);

    if (watchDescAndTreeNodeToDelete == NULL) {
        // printf("Name node with name %s not found in names' list\n", name);
        return -1;
    }

    if (watchDescAndTreeNodeToDelete->wd < wdAndTreeNodesList->firstNode->wd) {
        wdAndTreeNodesList->firstNode = watchDescAndTreeNodeToDelete->nextNode;
    }

    if (watchDescAndTreeNodeToDelete->prevNode != NULL)
        watchDescAndTreeNodeToDelete->prevNode->nextNode = watchDescAndTreeNodeToDelete->nextNode;

    if (watchDescAndTreeNodeToDelete->nextNode != NULL)
        watchDescAndTreeNodeToDelete->nextNode->prevNode = watchDescAndTreeNodeToDelete->prevNode;

    freeWatchDescAndTreeNode(&watchDescAndTreeNodeToDelete);
    wdAndTreeNodesList->size--;

    return 0;
}

// TreeNode* addTreeNodeToDir(DirTree* tree, char* nodeName, INode* iNodeP, Type type, char* dirName) {
//     if (tree->size == 0) {
//         tree->rootNode = (TreeNode*)malloc(sizeof(TreeNode));
//         tree->rootNode->name = (char*)malloc(sizeof(nodeName));
//         strcpy(tree->rootNode->name, nodeName);
//         tree->rootNode->iNodeP = iNodeP;   // points to the i-node entry in the i-nodes' array
//         tree->rootNode->type = Directory;  // it is the root directory

//         tree->size++;
//         return tree->rootNode;
//     }

//     TreeNode* curTreeNode = tree->rootNode;
//     char* curDirName = curTreeNode->name;

//     while (curTreeNode != NULL) {
//         if (curTreeNode->type == Directory && strcmp(curTreeNode->name, dirName) == 0)
//         {

//         }

//         curTreeNode = curTreeNode->nextNode != NULL ? curTreeNode->nextNode : curTreeNode->firstChildNode;
//         if (curTreeNode->type == Directory)
//     }
// }

TreeNode* addTreeNodeToDir(DirTree* tree, TreeNode* parentDir, char* name, char* pathName, INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size, Type type, TreeNode* sourceTreeNode /*this is null while populating tree*/) {
    // if (deleteTreeNodeFromDir(parentDir, name) == 0) {  // if tree node with the same name exists in current directory then delete it first
    //     printf("A directory/file with name %s already existed in directory with name %s so it has been deleted\n", name, parentDir->name);
    // }
    // printf("add tree node 1\n");
    TreeNode* existingTreeNode = findTreeNodeInDir(parentDir, name, -1);
    printf("add tree node 2\n");

    INode* existingINode = NULL;

    if (sourceTreeNode != NULL && existingTreeNode != NULL) {  // we are trying to add a tree node to a *destination* directory
        printf("noooooo\n");
        if (existingTreeNode->type != type) {
            /*optimize this*/ if (deleteTreeNodeFromDir(tree, parentDir, name, pathName, iNodesList) == 0) {  // if tree node with the same name exists in current directory then delete it first
                deleteFileOrDirectory(pathName);
                printf("A %s with name %s already existed in directory as a %s with name %s so it has been deleted\n",
                       type == File ? "file" : "directory", name, type == Directory ? "file" : "directory", parentDir->name);
            }
        } else {
            if (sourceTreeNode->iNodeP->lastModTime != existingTreeNode->iNodeP->lastModTime || sourceTreeNode->iNodeP->size != existingTreeNode->iNodeP->size) {
                if (existingTreeNode->type == Directory) {
                    // special treatment for already existing directories

                    // char newPath[PATH_MAX];
                    // strcpy(newPath, parentDir->pathName);
                    // strcat(newPath, sourceTreeNode->name);

                    // renameFileOrDirectory(existingTreeNode->pathName, newPath);
                    copyDirAttributes(sourceTreeNode->pathName, existingTreeNode->pathName);
                    struct stat curStat;
                    stat(pathName, &curStat);

                    existingTreeNode->iNodeP->lastModTime = curStat.st_mtime;
                    existingTreeNode->iNodeP->size = curStat.st_size;
                    // deleteNameNodeFromNamesList(existingTreeNode->iNodeP->namesList, existingTreeNode->pathName);
                    // addNameNodeToNamesList(existingTreeNode->iNodeP->namesList, newPath);

                    sourceTreeNode->iNodeP->destINodeP = existingTreeNode->iNodeP;
                    // linkFile(sourceTreeNode->pathName, pathName);

                    // free(existingTreeNode->name);
                    // existingTreeNode->name = NULL;
                    // free(existingTreeNode->pathName);
                    // existingTreeNode->pathName = NULL;

                    // strcpy(existingTreeNode->name, sourceTreeNode->name);
                    // strcpy(existingTreeNode->pathName, newPath);

                    return existingTreeNode;
                }

                if (deleteTreeNodeFromDir(tree, parentDir, name, pathName, iNodesList) == 0) {  // if tree node with the same name exists in current directory then delete it first
                    deleteFileOrDirectory(pathName);
                    printf("A %s with name %s already existed in directory as a %s with name %s so it has been deleted\n",
                           type == File ? "file" : "directory", name, type == Directory ? "file" : "directory", parentDir->name);
                }

                // if (sourceTreeNode->iNodeP->destINodeP != NULL) { // normally we are sure that this is != NULL at this point because we found an existing treeNode with same type and name
                existingINode = sourceTreeNode->iNodeP->destINodeP;  // if it is != NULL then there is no need to create a new i-node
                // addNameNodeToNamesList(sourceTreeNode->iNodeP->destINodeP->namesList, name);
                // }
            } else {
                printf("Directory/File with name %s already in directory with name %s\n", name, parentDir->name);
                return existingTreeNode;
            }
        }
    } else {
        printf("checking for existing inode\n");
        // check if INode already exists
        if (sourceTreeNode == NULL) {
            existingINode = findINodeInINodesList(iNodesList, id);
        }
    }

    printf("add tree node 3\n");

    INode* createdINode = NULL;
    if (sourceTreeNode == NULL && existingINode == NULL) {
        createdINode = addINodeToINodesList(iNodesList, id, lastModTime, size, NULL, pathName);  /// WILL THIS WORK?? I WANT THE VALUE OF THE INODEP PARAMETER TO CHANGE
    }
    printf("add tree node 4\n");

    // if (findTreeNodeInDir(parentDir, name, -1) != NULL)
    // {
    //     printf("A directory/file with name %s already exists in directory with name %s so it will be deleted first\n", name, parentDir->name);
    //     freeTreeNode()
    // }

    TreeNode* curTreeNode = parentDir->firstChildNode;
    if (curTreeNode == NULL) {
        if (sourceTreeNode != NULL) {
            if (type == File) {
                copyFileOrDirectory(sourceTreeNode->pathName, pathName);
            } else {
                createDirAndCopyAttributes(sourceTreeNode->pathName, pathName);
            }
        }
        if (sourceTreeNode != NULL && existingINode == NULL) {
            printf("willl add new inode to inodes list\n");
            struct stat curStat;
            stat(pathName, &curStat);
            createdINode = addINodeToINodesList(iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, NULL, pathName);
        }
        parentDir->firstChildNode = initTreeNode(name, pathName, existingINode == NULL ? createdINode : existingINode, type, NULL);
        tree->size++;
        parentDir->contentsNum++;
        printf("Directory/File with name %s added successfully to directory with name %s\n", name, parentDir->name);
        return parentDir->firstChildNode;
    }
    printf("add tree node 5\n");

    if (strcmp(name, curTreeNode->name) <= 0) {
        if (sourceTreeNode != NULL) {
            if (type == File) {
                copyFileOrDirectory(sourceTreeNode->pathName, pathName);
            } else {
                createDirAndCopyAttributes(sourceTreeNode->pathName, pathName);
            }
        }

        if (sourceTreeNode != NULL && existingINode == NULL) {
            struct stat curStat;
            stat(pathName, &curStat);
            createdINode = addINodeToINodesList(iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, NULL, pathName);
        }

        TreeNode* treeNodeToAdd = initTreeNode(name, pathName, existingINode == NULL ? createdINode : existingINode, type, NULL);

        treeNodeToAdd->nextNode = curTreeNode;

        curTreeNode->prevNode = treeNodeToAdd;
        parentDir->firstChildNode = treeNodeToAdd;
        tree->size++;
        parentDir->contentsNum++;

        if (sourceTreeNode != NULL) {
            sourceTreeNode->iNodeP->destINodeP = treeNodeToAdd->iNodeP;
        }

        printf("Directory/File with name %s added successfully to directory with name %s\n", name, parentDir->name);
        return treeNodeToAdd;
    }
    printf("add tree node 6\n");

    while (curTreeNode != NULL) {
        if (curTreeNode->nextNode != NULL) {
            if (strcmp(name, curTreeNode->nextNode->name) <= 0) {
                if (sourceTreeNode != NULL) {
                    if (type == File) {
                        copyFileOrDirectory(sourceTreeNode->pathName, pathName);
                    } else {
                        createDirAndCopyAttributes(sourceTreeNode->pathName, pathName);
                    }
                }

                if (sourceTreeNode != NULL && existingINode == NULL) {
                    struct stat curStat;
                    stat(pathName, &curStat);
                    createdINode = addINodeToINodesList(iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, NULL, pathName);
                }

                TreeNode* treeNodeToAdd = initTreeNode(name, pathName, existingINode == NULL ? createdINode : existingINode, type, NULL);
                treeNodeToAdd->prevNode = curTreeNode;
                treeNodeToAdd->nextNode = curTreeNode->nextNode;
                curTreeNode->nextNode->prevNode = treeNodeToAdd;
                curTreeNode->nextNode = treeNodeToAdd;

                tree->size++;
                parentDir->contentsNum++;

                if (sourceTreeNode != NULL) {
                    sourceTreeNode->iNodeP->destINodeP = treeNodeToAdd->iNodeP;
                }

                printf("Directory/File with name %s added successfully to directory with name %s\n", name, parentDir->name);
                return treeNodeToAdd;
            }
        } else {
            if (sourceTreeNode != NULL) {
                if (type == File) {
                    copyFileOrDirectory(sourceTreeNode->pathName, pathName);
                } else {
                    createDirAndCopyAttributes(sourceTreeNode->pathName, pathName);
                }
            }

            if (sourceTreeNode != NULL && existingINode == NULL) {
                struct stat curStat;
                stat(pathName, &curStat);
                createdINode = addINodeToINodesList(iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, NULL, pathName);
            }

            TreeNode* treeNodeToAdd = initTreeNode(name, pathName, existingINode == NULL ? createdINode : existingINode, type, NULL);
            treeNodeToAdd->prevNode = curTreeNode;
            curTreeNode->nextNode = treeNodeToAdd;

            tree->size++;
            parentDir->contentsNum++;

            if (sourceTreeNode != NULL) {
                sourceTreeNode->iNodeP->destINodeP = treeNodeToAdd->iNodeP;
            }
            printf("Directory/File with name %s added successfully to directory with name %s\n", name, parentDir->name);
            return treeNodeToAdd;  // was curTreeNode but why??????
        }
        curTreeNode = curTreeNode->nextNode;
    }

    printf("Oops\n");
    return NULL;  // not normal
}

int deleteTreeNodeFromDir(DirTree* tree, TreeNode* parentDir, char* name, char* pathName, INodesList* iNodesList) {
    printf("hello there\n\n");

    if (parentDir == NULL) {
        printf("Delete -> NULL parent directory given as parameter\n");
        return -1;
    }

    if (parentDir->firstChildNode == NULL) {
        printf("Delete -> Directory/File with name %s not found inside directory with name %s\n", name, parentDir->name);
        return -1;  // not found
    }
    printf("hello there1\n\n");

    TreeNode* curTreeNode = parentDir->firstChildNode;
    // printf("hello there1.5\n\n");

    if (strcmp(name, curTreeNode->name) < 0) {
        printf("Delete -> Directory/File with name %s not found inside directory with name %s\n", name, parentDir->name);
        return -1;  // not found
    }
    printf("hello there2\n\n");

    if (strcmp(name, curTreeNode->name) == 0) {
        printf("hello there inside\n\n");

        parentDir->firstChildNode = curTreeNode->nextNode;
        printf("hello there inside1\n\n");

        if (parentDir->firstChildNode != NULL) {
            if (parentDir->firstChildNode->prevNode != NULL) {
                freeTreeNode(&(parentDir->firstChildNode->prevNode), iNodesList);
                parentDir->firstChildNode->prevNode = NULL;
            }
        }
        // printf("hello there inside2\n\n");

        printf("hello there inside3\n\n");

        tree->size--;
        // if (fork() == 0) {
        //     char* args[] = {"rm", "-rf", pathName, NULL};
        //     if (execvp(args[0], args) == -1) {
        //         perror("execvp failed");
        //         exit(1);
        //     }
        // }
        printf("Delete -> Directory/File with name %s deleted successfully from directory with name %s\n", name, parentDir->name);
        return 0;
    }
    printf("hello there3\n\n");

    curTreeNode = curTreeNode->nextNode;

    while (curTreeNode != NULL) {
        if (strcmp(name, curTreeNode->name) < 0)
            break;

        if (strcmp(name, curTreeNode->name) == 0) {
            curTreeNode->prevNode->nextNode = curTreeNode->nextNode;  // no need to check for NULL cause we are past the first tree node of this directory

            if (curTreeNode->nextNode != NULL) {
                curTreeNode->nextNode->prevNode = curTreeNode->prevNode;
            }

            freeTreeNode(&curTreeNode, iNodesList);  //////////////////////// do more stuff here
            tree->size--;
            // if (fork() == 0) {
            //     char* args[] = {"rm", "-rf", pathName, NULL};
            //     if (execvp(args[0], args) == -1) {
            //         perror("execvp failed");
            //         exit(1);
            //     }
            // }
            printf("Delete -> Directory/File with name %s deleted successfully from directory with name %s\n", name, parentDir->name);
            return 0;
        }

        curTreeNode = curTreeNode->nextNode;
    }

    printf("Delete -> Directory/File with name %s not found inside directory with name %s\n", name, parentDir->name);
    return -1;  // not found
}

TreeNode* findTreeNodeInDir(TreeNode* parentDir, char* name, Type type) {  // type = -1 when we don't care about the type of the tree node
    printf("find tree node 1\n");

    if (parentDir == NULL) {
        printf("Find -> NULL parent directory given as parameter\n");
        return NULL;
    }
    printf("find tree node 2\n");

    TreeNode* curTreeNode = parentDir->firstChildNode;
    printf("find tree node 3\n");

    while (curTreeNode != NULL) {
        if (strcmp(name, curTreeNode->name) < 0)
            break;

        if ((type == -1 && strcmp(name, curTreeNode->name) == 0) || (curTreeNode->type == type && strcmp(name, curTreeNode->name) == 0))
            return curTreeNode;

        curTreeNode = curTreeNode->nextNode;
    }

    printf("Find -> Directory/File with name %s not found inside directory with name %s\n", name, parentDir->name);
    return NULL;  // not found
}

void freeDirTreeNodes(TreeNode* treeNode, INodesList* iNodesList) {
    if (treeNode == NULL)
        return;

    if (treeNode->type == Directory) {
        freeDirTreeNodes(treeNode->firstChildNode, iNodesList);
    }

    freeDirTreeNodes(treeNode->nextNode, iNodesList);

    // printf("reached\n");
    freeTreeNode(&treeNode, iNodesList);
}

void populateTree(const char* dirName, int indent, DirTree* dirTree, TreeNode* parentDir, INodesList* iNodesList) {
    DIR* dir;
    struct dirent* entry;
    printf("dirname = %s\n", dirName);

    printf("populating tree\n");
    if ((dir = opendir(dirName)) == NULL) {
        perror("Could not open directory");
        return;
    }
    printf("populating tree1\n");

    while ((entry = readdir(dir)) != NULL) {
        // printf("in while loop entry: %s\n", entry->d_name);
        char path[PATH_MAX];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // printf("in while loop1\n");

        snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);
        printf("path:%s\n", path);
        struct stat curStat;
        stat(path, &curStat);
        // printf("ino: %ju\n",  (uintmax_t)curStat.st_ino);

        if (/*entry->d_type == DT_DIR*/ !S_ISREG(curStat.st_mode)) {  // if it is a directory
                                                                      // if (parentDir != NULL) {
                                                                      //     (*parentDir) = addTreeNodeToDir(dirTree, *parentDir, entry->d_name, path, iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, Directory, NULL);
                                                                      // } else {
            if (parentDir == NULL) {
                parentDir = dirTree->rootNode;
            }

            TreeNode* addedDir = addTreeNodeToDir(dirTree, parentDir, entry->d_name, path, iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, Directory, NULL);

            printf("Handled directory: %*s[%s]\n", indent, "", entry->d_name);
            populateTree(path, indent + 2, dirTree, addedDir, iNodesList);
        } else {  // if it is a file
            // printf("hello-1\n");

            if (parentDir == NULL) {
                // printf("hello-0\n");
                parentDir = dirTree->rootNode;
            }
            // printf("hello\n");

            addTreeNodeToDir(dirTree, parentDir, entry->d_name, path, iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, File, NULL);

            printf("Handled file: %*s- %s\n", indent, "", entry->d_name);
        }
    }
    printf("populating tree2\n");

    closedir(dir);
    printf("closed dir\n");
}

// TreeNode* advanceTreeNode(TreeNode* curTreeNode, TreeNode** parentDir) {
//     if (curTreeNode != NULL) {
//         if (curTreeNode->firstChildNode != NULL) {
//             (*parentDir) = curTreeNode;
//             return curTreeNode->firstChildNode;  // get into the directory
//         } else {
//             return curTreeNode->nextNode;
//         }
//     }
//     return NULL;
// }

void dfsFor2Trees(DirTree* sourceDirTree, DirTree* destDirTree, INodesList* sourceINodesList, INodesList* destINodesList, TreeNode* curSourceDir,
                  TreeNode* curDestDir, WdAndTreeNodesList* wdAndTreeNodesList, int iNotifyFd) {
    TreeNode *curSourceTreeNode, *curDestTreeNode;
    curSourceTreeNode = curSourceDir->firstChildNode;
    curDestTreeNode = curDestDir->firstChildNode;
    printf("============================================== cur dest dir name: %s\n", curDestDir->pathName);
    // TreeNode *curSourceParentDir = NULL, *curDestParentDir = NULL;
    struct stat curStat;
    if (curSourceTreeNode == NULL)
        printf("curSourceTreeNode == NULL\n");

    if (curDestTreeNode == NULL)
        printf("curDestTreeNode == NULL\n");

    char treeNodeHandled;
    while (curSourceTreeNode != NULL || curDestTreeNode != NULL) {
        treeNodeHandled = 0;
        printf("-----------------> current source tree node: %s, current destination tree node: %s\n",
               curSourceTreeNode == NULL ? "NULL" : curSourceTreeNode->pathName, curDestTreeNode == NULL ? "NULL" : curDestTreeNode->pathName);
        if (curDestTreeNode != NULL) {
            TreeNode* foundSourceTreeNode = findTreeNodeInDir(curSourceDir, curDestTreeNode->name, curDestTreeNode->type);
            // maybe foundSourceTreeNode can be replaced by curSourceTreeNode here cause names are ordered alphabetically
            if (foundSourceTreeNode == NULL) {
                char tempPathName[PATH_MAX], tempName[PATH_MAX];
                strcpy(tempPathName, curDestTreeNode->pathName);
                strcpy(tempName, curDestTreeNode->name);

                printf("will delete file/dir with path name: %s\n", tempPathName);

                curDestTreeNode = curDestTreeNode->nextNode;  // do this now because otherwise afterwards curDestTreeNode will be NULL
                deleteTreeNodeFromDir(destDirTree, curDestDir, tempName, tempPathName, destINodesList);
                deleteFileOrDirectory(tempPathName);
            }
            //  else {
            //     stat(foundSourceTreeNode->pathName, &curStat);
            //     char newPath[PATH_MAX];
            //     strcpy(newPath, curDestDir->pathName);
            //     strcat(newPath, "/");
            //     strcat(newPath, foundSourceTreeNode->name);
            //     printf("in else of dfs\n");
            //     curDestTreeNode = addTreeNodeToDir(destDirTree, curDestDir, foundSourceTreeNode->name, newPath, destINodesList,
            //                                        curStat.st_ino, curStat.st_mtime, curStat.st_size, foundSourceTreeNode->type, foundSourceTreeNode);

            //     // treeNodeHandled = 1;

            // }
        }
        //  else {
        if (treeNodeHandled == 0) {
            if (curSourceTreeNode == NULL)
                continue;

            stat(curSourceTreeNode->pathName, &curStat);
            int linkedTreeNodes = 0;

            if (curSourceTreeNode->type == File) {
                TreeNode* tempDestTreeNode = curDestTreeNode;
                while (tempDestTreeNode != NULL) {
                    if (tempDestTreeNode->iNodeP->id == curStat.st_ino) {
                        curSourceTreeNode->iNodeP->destINodeP = tempDestTreeNode->iNodeP;
                        linkFile(curSourceTreeNode->pathName, tempDestTreeNode->pathName);

                        linkedTreeNodes = 1;
                        break;
                    }
                    tempDestTreeNode = tempDestTreeNode->nextNode;
                }
            }

            if (linkedTreeNodes == 0) {
                char newPath[PATH_MAX];
                strcpy(newPath, curDestDir->pathName);  // check for curDestDir == NULL ??????????????????????????????????????????????????????????????????????
                strcat(newPath, "/");
                strcat(newPath, curSourceTreeNode->name);
                printf("\n\n\ncur dest dir name: %s\n\n\n\n", curDestDir->pathName);

                curDestTreeNode = addTreeNodeToDir(destDirTree, curDestDir, curSourceTreeNode->name, newPath, destINodesList,
                                                   curStat.st_ino, curStat.st_mtime, curStat.st_size, curSourceTreeNode->type, curSourceTreeNode);
            }
            // }
        }
        // if (curSourceTreeNode == NULL) {
        //     deleteTreeNodeFromDir(destDirTree, curDestDir, curDestTreeNode->name, curDestTreeNode->pathName, destINodesList);
        //     deleteFileOrDirectory(curDestTreeNode->pathName);
        // } else {  // 2 cases more
        //     if ()
        //         addTreeNodeToDir(destDirTree, curDestDir, )
        // }

        // if (curSourceTreeNode != NULL) {
        //     addTreeNodeToDir(destDirTree, curDestParentDir, );
        // }

        if (curSourceTreeNode != NULL && curDestTreeNode != NULL) {
            if (curSourceTreeNode->type == Directory && curDestTreeNode->type == Directory) {
                // if (curSourceTreeNode->type == Directory) {
                // sourceWdAndTreeNodes[index].wd = inotify_add_watch(iNotifyFd, curSourceTreeNode->pathName, IN_ALL_EVENTS);
                // sourceWdAndTreeNodes[index].sourceTreeNodeP = curSourceTreeNode;
                // sourceWdAndTreeNodes[index].destTreeNodeP = curDestTreeNode;

                addWatchDescAndTreeNodeToWdAndTreeNodesList(wdAndTreeNodesList, -1, curSourceTreeNode, curDestTreeNode, curSourceDir, curDestDir);

                // }
                printf("1111111111111111111111111111111111111111111111111111111111111111111 will call dfs for dest tree node with name: %s and source: %s\n",
                       curDestTreeNode->pathName, curSourceTreeNode->pathName);
                dfsFor2Trees(sourceDirTree, destDirTree, sourceINodesList, destINodesList, curSourceTreeNode, curDestTreeNode,
                             wdAndTreeNodesList, iNotifyFd);
            }
            //  else if (curSourceTreeNode->type == File && curDestTreeNode->type == File)
            // {

            // }
            // curSourceTreeNode = curSourceTreeNode->nextNode;
        }
        if (curSourceTreeNode != NULL) {
            // if (curSourceTreeNode->type == File) {
            curSourceTreeNode = curSourceTreeNode->nextNode;
            // }
        }
        if (curDestTreeNode != NULL) {
            // if (curDestTreeNode->type == File) {
            curDestTreeNode = curDestTreeNode->nextNode;
            // }
        }
    }

    printf("finished\n");
    return;
    // curSourceTreeNode = advanceTreeNode(curSourceTreeNode, &curSourceParentDir);
    // curDestTreeNode = advanceTreeNode(curDestTreeNode, &curDestParentDir);
    // dfsFor2Trees(sourceDirTree, destDirTree, curSourceTreeNode, curDestTreeNode, curSourceParentDir, curDestParentDir);
}

void freeDirTreeAndINodesList(DirTree* dirTree, INodesList* iNodesList) {
    INode* iNode = iNodesList->firstINode;
    while (iNode != NULL) {
        NameNode* nameNode = iNode->namesList->firstNameNode;
        printf("changed node: %ju\n", iNode->id);
        while (nameNode != NULL) {
            printf("name node's name: %s\n", nameNode->name);
            nameNode = nameNode->nextNode;
        }
        iNode = iNode->nextNode;
    }
    freeDirTreeNodes(dirTree->rootNode, iNodesList);
    free(iNodesList);
    free(dirTree);
}

void addWatches(TreeNode* sourceRootDir, TreeNode* destRootDir, WdAndTreeNodesList* wdAndTreeNodesList, int iNotifyFd) {
    printf("adding watches1\n\n");

    WatchDescAndTreeNode* curWdAndTreeNode = wdAndTreeNodesList->firstNode;
    while (curWdAndTreeNode != NULL) {
        // if (curWdAndTreeNode->sourceTreeNodeP == NULL)
        //     printf("NULL\n\n");
        if (curWdAndTreeNode->wd == -1) {
            curWdAndTreeNode->wd = inotify_add_watch(iNotifyFd, curWdAndTreeNode->sourceTreeNodeP->pathName, IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF | IN_MOVED_FROM | IN_MOVED_TO);
        }
        curWdAndTreeNode = curWdAndTreeNode->nextNode;
    }
    addWatchDescAndTreeNodeToWdAndTreeNodesList(wdAndTreeNodesList,
                                                inotify_add_watch(iNotifyFd, sourceRootDir->pathName, IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF | IN_MOVED_FROM | IN_MOVED_TO),
                                                sourceRootDir, destRootDir, NULL, NULL);

    printf("adding watches2\n\n");
}

void startWatchingDirectory(int iNotifyFd, WdAndTreeNodesList* wdAndTreeNodesList, DirTree* sourceDirTree, INodesList* sourceINodesList,
                            DirTree* destDirTree, INodesList* destINodesList) {
    struct inotify_event iNotifyEvent;
    int readRet, wd;
    uint32_t mask, nameLen;
    char* curName = NULL;
    while ((readRet = read(iNotifyFd, &iNotifyEvent, sizeof(struct inotify_event) + NAME_MAX + 1)) != -1) {
        wd = iNotifyEvent.wd;
        mask = iNotifyEvent.mask;
        nameLen = iNotifyEvent.len;
        if (nameLen > 0) {
            curName = (char*)malloc(nameLen);

            strcpy(curName, iNotifyEvent.name);
            if (curName == NULL)
                printf("hello1 \n");

            printf("new event->wd: %d, mask: %d, name: %s\n", wd, mask, iNotifyEvent.name);
        }
        printf("new event->wd: %d, mask: %d\n", wd, mask);

        WatchDescAndTreeNode* curWdAndTreeNode = findWatchDescAndTreeNodeInWdAndTreeNodesList(wdAndTreeNodesList, wd);  // it should NOT return NULL
        printf("hello\n");

        if (curWdAndTreeNode == NULL)
            printf("NULL\n\n");
        TreeNode* curSourceParentDir = curWdAndTreeNode->sourceTreeNodeP;
        TreeNode* curDestParentDir = curWdAndTreeNode->destTreeNodeP;
        TreeNode* curSourceGParentDir = curWdAndTreeNode->sourceParentDirP;
        TreeNode* curDestGParentDir = curWdAndTreeNode->destParentDirP;

        printf("hello\n");

        char pathName[PATH_MAX];
        struct stat curStat;

        if (nameLen > 0 && mask != IN_DELETE) {
            strcpy(pathName, curSourceParentDir->pathName);
            strcat(pathName, "/");
            strcat(pathName, curName);
            stat(pathName, &curStat);
        }

        switch (mask) {
            case IN_CREATE: {
                printf("create\n");

                // add tree node to source directory
                TreeNode* createdSourceTreeNode = addTreeNodeToDir(sourceDirTree, curSourceParentDir, curName, pathName, sourceINodesList,
                                                                   curStat.st_ino, curStat.st_mtime, curStat.st_size, File, NULL);

                // TreeNode *foundSourceTreeNode = findTreeNodeInDir(curSourceTreeNode, curName, -1);
                if (createdSourceTreeNode == NULL) {
                    printf("Oops...\n");
                    break;
                }
                int linkedTreeNodes = 0;

                TreeNode* tempDestTreeNode = curDestParentDir->firstChildNode;
                while (tempDestTreeNode != NULL) {
                    if (tempDestTreeNode->iNodeP->id == curStat.st_ino) {
                        createdSourceTreeNode->iNodeP->destINodeP = tempDestTreeNode->iNodeP;
                        linkFile(createdSourceTreeNode->pathName, tempDestTreeNode->pathName);  // maybe this is redundant

                        linkedTreeNodes = 1;
                        break;
                    }
                    tempDestTreeNode = tempDestTreeNode->nextNode;
                }

                if (linkedTreeNodes == 0) {
                    char newPath[PATH_MAX];
                    strcpy(newPath, curDestParentDir->pathName);  // check for curDestDir == NULL ??????????????????????????????????????????????????????????????????????
                    strcat(newPath, "/");
                    strcat(newPath, createdSourceTreeNode->name);
                    printf("\n\n\ncur dest dir name: %s\n\n\n\n", curDestParentDir->pathName);

                    addTreeNodeToDir(destDirTree, curDestParentDir, createdSourceTreeNode->name, newPath, destINodesList,
                                     curStat.st_ino, curStat.st_mtime, curStat.st_size, createdSourceTreeNode->type,
                                     createdSourceTreeNode);
                }
                break;
            }
            case IN_CREATE | IN_ISDIR: {
                printf("create dir\n");

                // add tree node to source directory
                TreeNode* createdSourceTreeNode = addTreeNodeToDir(sourceDirTree, curSourceParentDir, curName, pathName, sourceINodesList,
                                                                   curStat.st_ino, curStat.st_mtime, curStat.st_size, Directory, NULL);

                // TreeNode *foundSourceTreeNode = findTreeNodeInDir(curSourceTreeNode, curName, -1);
                if (createdSourceTreeNode == NULL) {
                    printf("Oops...\n");
                    break;
                }

                char newPath[PATH_MAX];
                strcpy(newPath, curDestParentDir->pathName);  // check for curDestDir == NULL ??????????????????????????????????????????????????????????????????????
                strcat(newPath, "/");
                strcat(newPath, createdSourceTreeNode->name);
                printf("\n\n\ncur dest dir name: %s\n\n\n\n", curDestParentDir->pathName);

                TreeNode* createdDestTreeNode = addTreeNodeToDir(destDirTree, curDestParentDir, createdSourceTreeNode->name, newPath, destINodesList,
                                                                 curStat.st_ino, curStat.st_mtime, curStat.st_size, createdSourceTreeNode->type,
                                                                 createdSourceTreeNode);

                addWatchDescAndTreeNodeToWdAndTreeNodesList(wdAndTreeNodesList, inotify_add_watch(iNotifyFd, createdSourceTreeNode->pathName, IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF | IN_MOVED_FROM | IN_MOVED_TO),
                                                            createdSourceTreeNode, createdDestTreeNode, curSourceParentDir, curDestParentDir);

                break;
            }
            case IN_ATTRIB: {
                printf("attrib\n");

                if (nameLen <= 0) {
                    // printf("blaaaaaaa\n");
                    break;
                }

                TreeNode* modSourceTreeNode = findTreeNodeInDir(curSourceParentDir, curName, S_ISREG(curStat.st_mode) ? File : Directory);
                if (modSourceTreeNode->iNodeP->lastModTime != curStat.st_mtime) {
                    modSourceTreeNode->iNodeP->lastModTime = curStat.st_mtime;
                    printf("hellooooo111111\n");

                    TreeNode* foundDestTreeNode = findTreeNodeInDir(curDestParentDir, curName, S_ISREG(curStat.st_mode) ? File : Directory);
                    if (foundDestTreeNode == NULL) {
                        printf("Oops...\n");
                        break;
                    }

                    foundDestTreeNode->iNodeP->lastModTime = curStat.st_mtime;
                }
                break;
            }
            case IN_MODIFY: {
                printf("modify\n");
                if (nameLen <= 0) {
                    // printf("blaaaaaaa\n");
                    break;
                }

                TreeNode* modSourceTreeNode = findTreeNodeInDir(curSourceParentDir, curName, S_ISREG(curStat.st_mode) ? File : Directory);
                if (modSourceTreeNode == NULL) {
                    printf("Oops...\n");
                    break;
                }

                if (modSourceTreeNode->iNodeP->size != curStat.st_size) {
                    modSourceTreeNode->iNodeP->size = curStat.st_size;
                }

                // if (modSourceTreeNode->iNodeP->lastModTime != curStat.st_mtime) {
                modSourceTreeNode->iNodeP->lastModTime = curStat.st_mtime;
                char newPath[PATH_MAX];
                strcpy(newPath, curDestParentDir->pathName);  // check for curDestDir == NULL ??????????????????????????????????????????????????????????????????????
                strcat(newPath, "/");
                strcat(newPath, modSourceTreeNode->name);

                // addTreeNodeToDir(destDirTree, curDestParentDir, modSourceTreeNode->name, newPath, destINodesList,
                //                  curStat.st_ino, curStat.st_mtime, curStat.st_size, modSourceTreeNode->type,
                //                  modSourceTreeNode);

                copyFileOrDirectory(modSourceTreeNode->pathName, newPath);

                // TreeNode* foundDestTreeNode = findTreeNodeInDir(curDestParentDir, curName, S_ISREG(curStat.st_mode) ? File : Directory);
                // if (foundDestTreeNode == NULL) {
                //     printf("Oops...\n");
                //     break;
                // }

                // foundDestTreeNode->iNodeP->lastModTime = curStat.st_mtime;
                // }
                break;
            }
            case IN_CLOSE_WRITE: {
                printf("close write\n");

                if (nameLen <= 0) {
                    // printf("blaaaaaaa\n");
                    break;
                }

                TreeNode* modSourceTreeNode = findTreeNodeInDir(curSourceParentDir, curName, S_ISREG(curStat.st_mode) ? File : Directory);
                if (modSourceTreeNode == NULL) {
                    printf("Oops...\n");
                    break;
                }

                if (modSourceTreeNode->iNodeP->size != curStat.st_size) {
                    modSourceTreeNode->iNodeP->size = curStat.st_size;
                }

                // if (modSourceTreeNode->iNodeP->lastModTime != curStat.st_mtime) {
                modSourceTreeNode->iNodeP->lastModTime = curStat.st_mtime;
                char newPath[PATH_MAX];
                strcpy(newPath, curDestParentDir->pathName);  // check for curDestDir == NULL ??????????????????????????????????????????????????????????????????????
                strcat(newPath, "/");
                strcat(newPath, modSourceTreeNode->name);

                copyFileOrDirectory(modSourceTreeNode->pathName, newPath);

                // addTreeNodeToDir(destDirTree, curDestParentDir, modSourceTreeNode->name, newPath, destINodesList,
                //                  curStat.st_ino, curStat.st_mtime, curStat.st_size, modSourceTreeNode->type,
                //                  modSourceTreeNode);
                break;
            }
            case IN_DELETE: {
                printf("delete\n");

                if (nameLen <= 0) {
                    // printf("blaaaaaaa\n");
                    break;
                }

                // TreeNode* deleteSourceTreeNode = findTreeNodeInDir(curSourceParentDir, curName, S_ISREG(curStat.st_mode) ? File : Directory);
                // if (deleteSourceTreeNode == NULL) {
                //     printf("Oops...\n");
                //     break;
                // }

                char sourcePath[PATH_MAX];
                strcpy(sourcePath, curSourceParentDir->pathName);  // check for curDestDir == NULL ??????????????????????????????????????????????????????????????????????
                strcat(sourcePath, "/");
                strcat(sourcePath, curName);

                char destPath[PATH_MAX];
                strcpy(destPath, curDestParentDir->pathName);  // check for curDestDir == NULL ??????????????????????????????????????????????????????????????????????
                strcat(destPath, "/");
                strcat(destPath, curName);

                deleteFileOrDirectory(sourcePath);
                deleteFileOrDirectory(destPath);

                // deleteTreeNodeFromDir(sourceDirTree, curSourceParentDir, curName, sourcePath, sourceINodesList);
                deleteTreeNodeFromDir(destDirTree, curDestParentDir, curName, destPath, destINodesList);

                break;
            }
            case IN_DELETE_SELF: {
                printf("delete self\n");

                // deleteFileOrDirectory(curSourceParentDir->pathName);
                deleteFileOrDirectory(curDestParentDir->pathName);

                deleteTreeNodeFromDir(sourceDirTree, curSourceGParentDir, curSourceParentDir->name, curSourceParentDir->pathName, sourceINodesList);
                deleteTreeNodeFromDir(destDirTree, curDestGParentDir, curSourceParentDir->name, curDestParentDir->pathName, destINodesList);
                break;
            }
            case IN_MOVED_FROM:
                printf("moved from\n");

                break;
            case IN_MOVED_TO:
                printf("moved to\n");
                break;
        }

        printf("ha\n");
        if (curName != NULL) {
            free(curName);
            curName = NULL;
        }
    }

    if (readRet == -1) {
        perror("read inotify desc error");
        exit(1);
    }
}