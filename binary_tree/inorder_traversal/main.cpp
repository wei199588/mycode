#include "inorder_traversal.h"

int main()
{
    TreeNode father;
    TreeNode left;
    TreeNode right;
    left.data = 3;
    left.leftNode = nullptr;
    left.rightNode = nullptr;
    right.data = 5;
    right.leftNode = nullptr;
    right.rightNode = nullptr;
    father.data = 8;
    father.leftNode = &left;
    father.rightNode = &right;

    InorderTraversal inordertraversal;
    std::vector<int> res;
    res = inordertraversal.InorderByRecursive(&father);
    for(int i = 0; i < res.size(); ++i)
    {
        std::cout << res.at(i) << " ";
    }
    std::cout << std::endl;

}