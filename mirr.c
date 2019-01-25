#include "tree/tree.h"

static WdAndTreeNodesList *wdAndTreeNodesList;
static DirTree *sourceDirTree, *destDirTree;
static INodesList *sourceINodesList, *destINodesList;
static char *curName;

// intHandler: catches the interrupt signal and frees the allocated memory
void intHandler(int dummy) {
    freeWdAndTreeNodesList(&wdAndTreeNodesList);
    freeDirTreeAndINodesList(sourceDirTree, sourceINodesList);
    freeDirTreeAndINodesList(destDirTree, destINodesList);
    if (curName != NULL) {
        free(curName);
    }
    printf("\nFreed memory\n\n");
    exit(0);
}

int main(int argc, char **argv) {
    // start listening for interrupt signal
    signal(SIGINT, intHandler);

    char *sourceDirName, *destDirName;

    handleFlags(argc, argv, &sourceDirName, &destDirName);

    // initialize structures
    sourceINodesList = initINodesList();
    destINodesList = initINodesList();

    struct stat sourceStat, destStat;
    stat(sourceDirName, &sourceStat);
    stat(destDirName, &destStat);
    sourceDirTree = initDirTree(basename(sourceDirName), sourceDirName, sourceINodesList, sourceStat.st_ino, sourceStat.st_mtime, sourceStat.st_size);
    destDirTree = initDirTree(basename(destDirName), destDirName, destINodesList, destStat.st_ino, destStat.st_mtime, destStat.st_size);
    wdAndTreeNodesList = initWdAndTreeNodesList();
    int iNotifyFd = inotify_init();
    // end of initialization

    printf("Initial synchronization started. Please do not modify neither the source nor the destination directory during this process.\n");
    // start populating the structures
    populateTree(sourceDirName, 0, sourceDirTree, NULL, sourceINodesList);
    populateTree(destDirName, 0, destDirTree, NULL, destINodesList);

    // synchronize destination's structures with source's structures
    dfsFor2Trees(sourceDirTree, destDirTree, sourceINodesList, destINodesList, sourceDirTree->rootNode, destDirTree->rootNode, wdAndTreeNodesList, iNotifyFd);

    // add watches to each entry of the wdAndTreeNodesList
    addWatches(sourceDirTree->rootNode, destDirTree->rootNode, wdAndTreeNodesList, iNotifyFd);
    printf("Initial synchronization finished. You can now start modifying the source directory.\n");

    // start listening for events
    startWatchingDirectory(iNotifyFd, wdAndTreeNodesList, sourceDirTree, sourceINodesList, destDirTree, destINodesList, &curName);

    // free allocated memory
    freeWdAndTreeNodesList(&wdAndTreeNodesList);
    freeDirTreeAndINodesList(sourceDirTree, sourceINodesList);
    freeDirTreeAndINodesList(destDirTree, destINodesList);
    printf("\nFreed memory\n\n");
}