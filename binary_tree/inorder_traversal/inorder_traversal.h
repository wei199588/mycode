#include <iostream>
#include <vector>

struct TreeNode
{
    int data = -9999;
    TreeNode *leftNode = nullptr;
    TreeNode *rightNode = nullptr;
};

class InorderTraversal
{
public:
    InorderTraversal();
    ~InorderTraversal();

    std::vector<int> InorderByRecursive(TreeNode *root);

private:
    void step(std::vector<int> &result, TreeNode *node);

};


