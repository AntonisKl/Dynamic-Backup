#include "../utils/utils.h"

typedef struct TreeNode {
    char* name;
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

TreeNode* initTreeNode(char* name, INode* iNodeP, Type type, INode* existingINode);

void freeTreeNode(TreeNode* treeNode);

TreeNode* addTreeNodeToDir(DirTree* tree, TreeNode* parentDir, char* name, INode* iNodeP, Type type, TreeNode* sourceTreeNode);

int deleteTreeNodeFromDir(TreeNode* parentDir, char* name);

TreeNode* findTreeNodeInDir(TreeNode* parentDir, char* name, Type type);  // type = -1 when we don't care about the type of the tree node