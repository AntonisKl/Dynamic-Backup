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

    printf("\n\nhaha name: %s\n", sourceDirTree->rootNode->name);

    populateTree(sourceDirName, 0, sourceDirTree, NULL, sourceINodesList);
    populateTree(destDirName, 0, destDirTree, NULL, destINodesList);

    freeDirTreeNodes(sourceDirTree->rootNode, sourceINodesList);
    freeDirTreeNodes(destDirTree->rootNode, destINodesList);
    // do the initial synchronization with DFS on both trees
}