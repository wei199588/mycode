#include "inorder_traversal.h"

InorderTraversal::InorderTraversal()
{
}

InorderTraversal::~InorderTraversal()
{
}

std::vector<int> InorderTraversal::InorderByRecursive(TreeNode *root)
{
    std::vector<int> res;
    if(root == nullptr)
        return res;
    step(res, root);
    return res;  
}

void InorderTraversal::step(std::vector<int> &result, TreeNode *node)
{
    if(node == nullptr)
        return;
    step(result,node->leftNode);
    result.push_back(node->data);
    step(result, node->rightNode);
}