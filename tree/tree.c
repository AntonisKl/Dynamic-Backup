#include "tree.h"

DirTree* initDirTree(char* rootName, char* pathName, INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size) {
    DirTree* dirTree = (DirTree*)malloc(sizeof(DirTree));  // must be freed
    dirTree->size = 0;
    NamesList* namesList = initNamesList();
    addNameNodeToNamesList(namesList, rootName);
    INode* createdINode = addINodeToINodesList(iNodesList, id, lastModTime, size, NULL, pathName);
    dirTree->rootNode = initTreeNode(rootName, pathName, createdINode, Directory, NULL);

    return dirTree;
}

TreeNode* initTreeNode(char* name, char* pathName, INode* iNodeP, Type type, NamesList* namesList) {  // namesList != NULL only when calling this function for stack
    TreeNode* treeNode = (TreeNode*)malloc(sizeof(TreeNode));
    treeNode->name = (char*)malloc(sizeof(name));
    strcpy(treeNode->name, name);
    treeNode->pathName = (char*)malloc(sizeof(pathName));
    strcpy(treeNode->pathName, pathName);

    if (namesList == NULL) {
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

    treeNode->visited = 0;
    // if (existingINode == NULL) {
    // addNameNodeToNamesList(treeNode->iNodeP->namesList, name);
    // }
    // treeNode->iNodeP->names                  // add current name to names LIST not array
    return treeNode;
}

void freeTreeNode(TreeNode* treeNode, INodesList* iNodesList) {
    deleteNameNodeFromNamesList(treeNode->iNodeP->namesList, treeNode->name);
    if (treeNode->iNodeP->namesList->size == 0) {
        deleteINodeFromINodesList(iNodesList, treeNode->iNodeP->id);
    }
    // treeNode->iNodeP->namesNum--;
    //treeNode->iNodeP->names       // delete from names array|||| maybe make the array a list
    //                              // maybe delete the name and -- the namesNum in the destINode too (if it is != null of course) !!!!!!!!!!!!!

    //                              // maybe make the i-nodes' array a list too to be able to delete the corresponding i-node if namesNum == 0
    free(treeNode->name);
    treeNode->name = NULL;
    free(treeNode->pathName);
    treeNode->pathName = NULL;
    //                                                          MAYBE HERE SEE IF THERE ARE INODES THAT POINT TO THIS FILE/DIRECTORY AND DELETE THEM TOO
    free(treeNode);
    treeNode = NULL;
    return;
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

TreeNode* addTreeNodeToDir(DirTree* tree, TreeNode* parentDir, char* name, char* pathName, INodesList* iNodesList, ino_t id, time_t lastModTime, off_t size, Type type, TreeNode* sourceTreeNode /*this is null for source directory*/) {
    // if (deleteTreeNodeFromDir(parentDir, name) == 0) {  // if tree node with the same name exists in current directory then delete it first
    //     printf("A directory/file with name %s already existed in directory with name %s so it has been deleted\n", name, parentDir->name);
    // }

    TreeNode* existingTreeNode = findTreeNodeInDir(parentDir, name, -1);

    INode* existingINode = NULL;

    if (sourceTreeNode != NULL && existingTreeNode != NULL) {  // we are trying to add a tree node to a *destination* directory
        if (existingTreeNode->type != type) {
            /*optimize this*/ if (deleteTreeNodeFromDir(parentDir, name, pathName, iNodesList) == 0) {  // if tree node with the same name exists in current directory then delete it first
                printf("A %s with name %s already existed in directory as a %s with name %s so it has been deleted\n",
                       type == File ? "file" : "directory", name, type == Directory ? "file" : "directory", parentDir->name);
            }
        } else {
            if (sourceTreeNode->iNodeP->lastModTime != existingTreeNode->iNodeP->lastModTime || sourceTreeNode->iNodeP->size != existingTreeNode->iNodeP->size) {
                if (deleteTreeNodeFromDir(parentDir, name, pathName, iNodesList) == 0) {  // if tree node with the same name exists in current directory then delete it first
                    printf("A %s with name %s already existed in directory as a %s with name %s so it has been deleted\n",
                           type == File ? "file" : "directory", name, type == Directory ? "file" : "directory", parentDir->name);
                }

                // if (sourceTreeNode->iNodeP->destINodeP != NULL) { // normally we are sure that this is != NULL at this point because we found an existing treeNode with same type and name
                existingINode = sourceTreeNode->iNodeP->destINodeP;
                // addNameNodeToNamesList(sourceTreeNode->iNodeP->destINodeP->namesList, name);
                // }
            } else {
                printf("Directory/File with name %s already in directory with name %s\n", name, parentDir->name);
                return existingTreeNode;
            }
        }
    } else {
        // check if INode already exists
        existingINode = findINodeInINodesList(iNodesList, id);
    }

    INode* createdINode = NULL;
    if (existingINode == NULL) {
        createdINode = addINodeToINodesList(iNodesList, id, lastModTime, size, NULL, pathName);  /// WILL THIS WORK?? I WANT THE VALUE OF THE INODEP PARAMETER TO CHANGE
    }

    // if (findTreeNodeInDir(parentDir, name, -1) != NULL)
    // {
    //     printf("A directory/file with name %s already exists in directory with name %s so it will be deleted first\n", name, parentDir->name);
    //     freeTreeNode()
    // }

    TreeNode* curTreeNode = parentDir->firstChildNode;
    if (curTreeNode == NULL) {
        parentDir->firstChildNode = initTreeNode(name, pathName, existingINode == NULL ? createdINode : existingINode, type, NULL);
        tree->size++;
        parentDir->contentsNum++;

        printf("Directory/File with name %s added successfully to directory with name %s\n", name, parentDir->name);
        return parentDir->firstChildNode;
    }

    if (strcmp(name, curTreeNode->name) <= 0) {
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

    while (curTreeNode != NULL) {
        if (curTreeNode->nextNode != NULL) {
            if (strcmp(name, curTreeNode->nextNode->name) <= 0) {
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
            TreeNode* treeNodeToAdd = initTreeNode(name, pathName, existingINode == NULL ? createdINode : existingINode, type, NULL);
            treeNodeToAdd->prevNode = curTreeNode;
            curTreeNode->nextNode = treeNodeToAdd;

            tree->size++;
            parentDir->contentsNum++;

            if (sourceTreeNode != NULL) {
                sourceTreeNode->iNodeP->destINodeP = treeNodeToAdd->iNodeP;
            }

            printf("Directory/File with name %s added successfully to directory with name %s\n", name, parentDir->name);
            return curTreeNode;
        }
        curTreeNode = curTreeNode->nextNode;
    }

    printf("Oops\n");
    return NULL;  // not normal
}

int deleteTreeNodeFromDir(TreeNode* parentDir, char* name, char* pathName, INodesList* iNodesList) {
    if (parentDir == NULL) {
        printf("Delete -> NULL parent directory given as parameter\n");
        return -1;
    }

    if (parentDir->firstChildNode == NULL) {
        printf("Delete -> Directory/File with name %s not found inside directory with name %s\n", name, parentDir->name);
        return -1;  // not found
    }

    TreeNode* curTreeNode = parentDir->firstChildNode;

    if (strcmp(name, curTreeNode->name) < 0) {
        printf("Delete -> Directory/File with name %s not found inside directory with name %s\n", name, parentDir->name);
        return -1;  // not found
    }

    if (strcmp(name, curTreeNode->name) == 0) {
        parentDir->firstChildNode = curTreeNode->nextNode;
        freeTreeNode(parentDir->firstChildNode->prevNode, iNodesList);
        parentDir->firstChildNode->prevNode = NULL;

        if (fork() == 0) {
            char* args[] = {"rm", "-rf", pathName, NULL};
            if (execvp(args[0], args) == -1) {
                perror("execvp failed");
                exit(1);
            }
        }
        printf("Delete -> Directory/File with name %s deleted successfully from directory with name %s\n", name, parentDir->name);
        return 0;
    }

    curTreeNode = curTreeNode->nextNode;

    while (curTreeNode != NULL) {
        if (strcmp(name, curTreeNode->name) < 0)
            break;

        if (strcmp(name, curTreeNode->name) == 0) {
            curTreeNode->prevNode->nextNode = curTreeNode->nextNode;  // no need to check for NULL cause we are past the first tree node of this directory

            if (curTreeNode->nextNode != NULL) {
                curTreeNode->nextNode->prevNode = curTreeNode->prevNode;
            }

            freeTreeNode(curTreeNode, iNodesList);  //////////////////////// do more stuff here
            if (fork() == 0) {
                char* args[] = {"rm", "-rf", pathName, NULL};
                if (execvp(args[0], args) == -1) {
                    perror("execvp failed");
                    exit(1);
                }
            }
            printf("Delete -> Directory/File with name %s deleted successfully from directory with name %s\n", name, parentDir->name);
            return 0;
        }

        curTreeNode = curTreeNode->nextNode;
    }

    printf("Delete -> Directory/File with name %s not found inside directory with name %s\n", name, parentDir->name);
    return -1;  // not found
}

TreeNode* findTreeNodeInDir(TreeNode* parentDir, char* name, Type type) {  // type = -1 when we don't care about the type of the tree node
    if (parentDir == NULL) {
        printf("Find -> NULL parent directory given as parameter\n");
        return NULL;
    }

    TreeNode* curTreeNode = parentDir->firstChildNode;

    while (curTreeNode != NULL) {
        if (strcmp(name, curTreeNode->name) < 0)
            break;

        if ((type == -1 && strcmp(name, curTreeNode->name) == 0) || (curTreeNode->type == type && strcmp(name, curTreeNode->name) == 0))
            return curTreeNode;

        curTreeNode = curTreeNode->nextNode;
    }

    printf("Find -> Directory/File with name %s not found inside directory with name %s\n", name, curTreeNode->name);
    return NULL;  // not found
}

void freeDirTree(TreeNode* treeNode, INodesList* iNodesList) {
    if (treeNode == NULL) {
        return;
    }

    if (treeNode->type == Directory) {
        freeDirTree(treeNode->firstChildNode, iNodesList);
    }

    freeDirTree(treeNode->nextNode, iNodesList);

    freeTreeNode(treeNode, iNodesList);
}

void populateTree(const char* dirName, int indent, DirTree* dirTree, TreeNode** parentDir, INodesList* iNodesList) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(dirName)) == NULL) {
        perror("Could not open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[PATH_MAX];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);

        struct stat curStat;
        stat(path, &curStat);

        if (/*entry->d_type == DT_DIR*/ S_ISREG(curStat.st_mode)) {  // if it is a directory
            if (parentDir != NULL) {
                (*parentDir) = addTreeNodeToDir(dirTree, *parentDir, entry->d_name, path, iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, Directory, NULL);
            } else {
                (*parentDir) = dirTree->rootNode;
            }

            printf("Handled directory: %*s[%s]\n", indent, "", entry->d_name);
            populateTree(path, indent + 2, dirTree, parentDir, iNodesList);
        } else {  // if it is a file
            addTreeNodeToDir(dirTree, *parentDir, entry->d_name, path, iNodesList, curStat.st_ino, curStat.st_mtime, curStat.st_size, File, NULL);

            printf("Handled file: %*s- %s\n", indent, "", entry->d_name);
        }
    }

    closedir(dir);
}