#include "../utils/utils.h"

typedef struct TreeNode {
    char* name, *pathName; // including this directory's/file's name
    char visited;
    INode* iNodeP;
    struct TreeNode *nextNode, *prevNode, *firstChildNode;  // covers all different cases by setting null the variables that we don't need
    Type type;
    int contentsNum;  // will be -1 for files
} TreeNode;

typedef struct DirTree {
    unsigned int size;
    TreeNode* rootNode;
} DirTree;

DirTree* initDirTree();

TreeNode* initTreeNode(char* name, char* pathName, INode* iNodeP, Type type, NamesList* namesList);

void freeTreeNode(TreeNode* treeNode);

TreeNode* addTreeNodeToDir(DirTree* tree, TreeNode* parentDir, char* name, char* pathName, INode* iNodeP, time_t lastModTime, off_t size, Type type, TreeNode* sourceTreeNode /*this is null for source directory*/);

int deleteTreeNodeFromDir(TreeNode* parentDir, char* name, char* pathName);

TreeNode* findTreeNodeInDir(TreeNode* parentDir, char* name, Type type);  // type = -1 when we don't care about the type of the tree node