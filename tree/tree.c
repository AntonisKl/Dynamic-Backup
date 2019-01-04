#include "tree.h"

DirTree* initDirTree() {
    DirTree* dirTree = (DirTree*)malloc(sizeof(DirTree));  // must be freed
    dirTree->size = 0;
    dirTree->rootNode = NULL;

    return dirTree;
}

TreeNode* initTreeNode(char* name, INode* iNodeP, Type type, INode* existingINode) {
    TreeNode* treeNode = (TreeNode*)malloc(sizeof(TreeNode));
    treeNode->name = (char*)malloc(sizeof(name));
    treeNode->iNodeP = existingINode == NULL ? /*initINode(iNodeP->lastModTime, iNodeP->size, name)*/ iNodeP : existingINode;

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
    if (existingINode == NULL) {
        addNameNodeToNamesList(treeNode->iNodeP->namesList, name);
    }
    // treeNode->iNodeP->names                  // add current name to names LIST not array
    return treeNode;
}

void freeTreeNode(TreeNode* treeNode) {
    deleteNameNodeFromNamesList(treeNode->iNodeP->namesList, treeNode->name);
    // treeNode->iNodeP->namesNum--;
    //treeNode->iNodeP->names       // delete from names array|||| maybe make the array a list
    //                              // maybe delete the name and -- the namesNum in the destINode too (if it is != null of course) !!!!!!!!!!!!!

    //                              // maybe make the i-nodes' array a list too to be able to delete the corresponding i-node if namesNum == 0
    free(treeNode->name);
    //                                                          MAYBE HERE SEE IF THERE ARE INODES THAT POINT TO THIS FILE/DIRECTORY AND DELETE THEM TOO
    free(treeNode);

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

TreeNode* addTreeNodeToDir(DirTree* tree, TreeNode* parentDir, char* name, INode* iNodeP, Type type, TreeNode* sourceTreeNode /*this is null for source directory*/) {
    // if (deleteTreeNodeFromDir(parentDir, name) == 0) {  // if tree node with the same name exists in current directory then delete it first
    //     printf("A directory/file with name %s already existed in directory with name %s so it has been deleted\n", name, parentDir->name);
    // }

    TreeNode* existingTreeNode = findTreeNodeInDir(parentDir, name, -1);

    INode* existingINode = NULL;

    if (sourceTreeNode != NULL && existingTreeNode != NULL) {  // we are trying to add a tree node to a *destination* directory
        if (existingTreeNode->type != type) {
            /*optimize this*/ if (deleteTreeNodeFromDir(parentDir, name) == 0) {  // if tree node with the same name exists in current directory then delete it first
                printf("A %s with name %s already existed in directory as a %s with name %s so it has been deleted\n",
                       type == File ? "file" : "directory", name, type == Directory ? "file" : "directory", parentDir->name);
            }
        } else {
            if (sourceTreeNode->iNodeP->lastModTime != existingTreeNode->iNodeP->lastModTime || sourceTreeNode->iNodeP->size != existingTreeNode->iNodeP->size) {
                if (deleteTreeNodeFromDir(parentDir, name) == 0) {  // if tree node with the same name exists in current directory then delete it first
                    printf("A %s with name %s already existed in directory as a %s with name %s so it has been deleted\n",
                           type == File ? "file" : "directory", name, type == Directory ? "file" : "directory", parentDir->name);
                }

                // if (sourceTreeNode->iNodeP->destINodeP != NULL) { // normally we are sure that this is != NULL at this point because we found an existing treeNode with same type and name
                existingINode = sourceTreeNode->iNodeP->destINodeP;
                addNameNodeToNamesList(sourceTreeNode->iNodeP->destINodeP->namesList, name);
                // }
            }
        }
    }

    // if (findTreeNodeInDir(parentDir, name, -1) != NULL)
    // {
    //     printf("A directory/file with name %s already exists in directory with name %s so it will be deleted first\n", name, parentDir->name);
    //     freeTreeNode()
    // }

    TreeNode* curTreeNode = parentDir->firstChildNode;
    if (curTreeNode == NULL) {
        parentDir->firstChildNode = initTreeNode(name, iNodeP, type, existingINode);
        tree->size++;
        parentDir->contentsNum++;

        printf("Directory/File with name %s added successfully to directory with name %s\n", name, parentDir->name);
        return parentDir->firstChildNode;
    }

    if (strcmp(name, curTreeNode->name) <= 0) {
        TreeNode* treeNodeToAdd = initTreeNode(name, iNodeP, type, existingINode);

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
                TreeNode* treeNodeToAdd = initTreeNode(name, iNodeP, type, existingINode);
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
            TreeNode* treeNodeToAdd = initTreeNode(name, iNodeP, type, existingINode);
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

int deleteTreeNodeFromDir(TreeNode* parentDir, char* name) {
    if (parentDir == NULL) {
        printf("Delete -> NULL parent directory given as parameter\n");
        return -1;
    }

    TreeNode* curTreeNode = parentDir->firstChildNode;

    if (strcmp(name, curTreeNode->name) == 0) {
        parentDir->firstChildNode = curTreeNode->nextNode;
    }

    while (curTreeNode != NULL) {
        if (strcmp(name, curTreeNode->name) < 0)
            break;

        if (strcmp(name, curTreeNode->name) == 0) {
            freeTreeNode(curTreeNode);
            printf("Delete -> Directory/File with name %s deleted successfully from directory with name %s\n", name, parentDir->name);
            return 0;
        }

        curTreeNode = curTreeNode->nextNode;
    }

    printf("Delete -> Directory/File with name %s not found inside directory with name %s\n", name, curTreeNode->name);
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
