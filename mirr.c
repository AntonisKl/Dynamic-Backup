#include "tree/tree.h"

int main(int argc, char **argv) {
    char *sourceDirName, *destDirName;

    handleFlags(argc, argv, &sourceDirName, &destDirName);

    INodesList *sourceINodesList = initINodesList(), *destINodesList = initINodesList();

    struct stat sourceStat, destStat;
    stat(sourceDirName, &sourceStat);
    stat(destDirName, &destStat);
    printf("source dir name: %s\n", sourceDirName);
    DirTree *sourceDirTree = initDirTree(sourceDirName, sourceDirName, sourceINodesList, sourceStat.st_ino, sourceStat.st_mtime, sourceStat.st_size),
            *destDirTree = initDirTree(destDirName, destDirName, destINodesList, destStat.st_ino, destStat.st_mtime, destStat.st_size);

    // printf("\n\nhaha name: %s\n", sourceDirTree->rootNode->name);

    populateTree(sourceDirName, 0, sourceDirTree, NULL, sourceINodesList);
    populateTree(destDirName, 0, destDirTree, NULL, destINodesList);

    // printf("\n\n\nsize of source tree: %u", sourceDirTree->size);
    WatchDescAndTreeNode sourceWdAndTreeNodes[sourceDirTree->size];
    int iNotifyFd = inotify_init();

    printf("Initial synchronization started. Please do not modify neither the source nor the destination directory during this process.\n");
    dfsFor2Trees(sourceDirTree, destDirTree, sourceINodesList, destINodesList, sourceDirTree->rootNode, destDirTree->rootNode, sourceWdAndTreeNodes, 0, iNotifyFd);
    printf("Initial synchronization finished. You can now start modifying the source directory.\n");

    // for (int i = 0; i < sourceDirTree->size; i++)
    // {
    //     printf("\n\nwd: %d\n\n", sourceWdAndTreeNodes[i].wd);
    // }
    // free memory
    INode *iNode1 = sourceINodesList->firstINode;
    while (iNode1 != NULL) {
        NameNode *nameNode = iNode1->namesList->firstNameNode;
        printf("changed node: %ju\n", iNode1->id);
        while (nameNode != NULL) {
            printf("name node's name: %s\n", nameNode->name);
            nameNode = nameNode->nextNode;
        }
        iNode1 = iNode1->nextNode;
    }
    freeDirTreeNodes(sourceDirTree->rootNode, sourceINodesList);
    free(sourceINodesList);
    free(sourceDirTree);

    INode *iNode = destINodesList->firstINode;
    while (iNode != NULL) {
        NameNode *nameNode = iNode->namesList->firstNameNode;
        printf("changed node: %ju\n", iNode->id);
        while (nameNode != NULL) {
            printf("name node's name: %s\n", nameNode->name);
            nameNode = nameNode->nextNode;
        }
        iNode = iNode->nextNode;
    }
    freeDirTreeNodes(destDirTree->rootNode, destINodesList);
    free(destINodesList);
    free(destDirTree);
}