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

void freeNameNode(NameNode* nameNode) {
    free(nameNode->name);
    free(nameNode);

    return;
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

int deleteNameNodeFromNamesList(NamesList* namesList, char* name) {
    NameNode* nameNodeToDelete = findNameNodeInNamesList(namesList, name);

    if (nameNodeToDelete == NULL) {
        printf("Name node with name %s not found in names' list\n", name);
        return -1;
    }

    if (nameNodeToDelete->prevNode != NULL)
        nameNodeToDelete->prevNode->nextNode = nameNodeToDelete->nextNode;

    if (nameNodeToDelete->nextNode != NULL)
        nameNodeToDelete->nextNode->prevNode = nameNodeToDelete->prevNode;

    freeNameNode(nameNodeToDelete);
    namesList->size--;

    return 0;
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
