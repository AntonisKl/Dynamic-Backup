#include "tree/tree.h"

static WdAndTreeNodesList *wdAndTreeNodesList;
static DirTree *sourceDirTree, *destDirTree;
static INodesList *sourceINodesList, *destINodesList;
static char *curName;

void intHandler(int dummy) {
    freeWdAndTreeNodesList(&wdAndTreeNodesList);
    freeDirTreeAndINodesList(sourceDirTree, sourceINodesList);
    freeDirTreeAndINodesList(destDirTree, destINodesList);
    if (curName != NULL) {
        free(curName);
    }
    printf("\nfreed memory\n\n");
    exit(0);
}

int main(int argc, char **argv) {
    signal(SIGINT, intHandler);

    char *sourceDirName, *destDirName;

    handleFlags(argc, argv, &sourceDirName, &destDirName);

    sourceINodesList = initINodesList();
    destINodesList = initINodesList();

    struct stat sourceStat, destStat;
    stat(sourceDirName, &sourceStat);
    stat(destDirName, &destStat);
    printf("source dir name: %s\n", sourceDirName);
    sourceDirTree = initDirTree(basename(sourceDirName), sourceDirName, sourceINodesList, sourceStat.st_ino, sourceStat.st_mtime, sourceStat.st_size);
    destDirTree = initDirTree(basename(destDirName), destDirName, destINodesList, destStat.st_ino, destStat.st_mtime, destStat.st_size);

    populateTree(sourceDirName, 0, sourceDirTree, NULL, sourceINodesList);
    populateTree(destDirName, 0, destDirTree, NULL, destINodesList);

    wdAndTreeNodesList = initWdAndTreeNodesList();
    int iNotifyFd = inotify_init();

    printf("Initial synchronization started. Please do not modify neither the source nor the destination directory during this process.\n");
    dfsFor2Trees(sourceDirTree, destDirTree, sourceINodesList, destINodesList, sourceDirTree->rootNode, destDirTree->rootNode, wdAndTreeNodesList, iNotifyFd);

    addWatches(sourceDirTree->rootNode, destDirTree->rootNode, wdAndTreeNodesList, iNotifyFd);
    printf("Initial synchronization finished. You can now start modifying the source directory.\n");

    // WatchDescAndTreeNode *curWdAndTreeNode = wdAndTreeNodesList->firstNode;
    // while (curWdAndTreeNode != NULL) {
    //     printf("wdAndTreeNode-> wd: %d, sourceName: %s, destName: %s\n", curWdAndTreeNode->wd, curWdAndTreeNode->sourceTreeNodeP->pathName, curWdAndTreeNode->destTreeNodeP->pathName);

    //     curWdAndTreeNode = curWdAndTreeNode->nextNode;
    // }

    startWatchingDirectory(iNotifyFd, wdAndTreeNodesList, sourceDirTree, sourceINodesList, destDirTree, destINodesList, &curName);

    // printf("hello\n");

    freeWdAndTreeNodesList(&wdAndTreeNodesList);
    freeDirTreeAndINodesList(sourceDirTree, sourceINodesList);
    freeDirTreeAndINodesList(destDirTree, destINodesList);
}