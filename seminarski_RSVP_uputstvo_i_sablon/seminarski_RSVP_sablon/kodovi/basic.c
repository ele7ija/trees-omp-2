#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

// Definition for a binary tree TreeNode.
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

bool traverse(struct TreeNode* root, u_int64_t* sum) {
    if(root==NULL) return false;

    (*sum) -= root->val;
    if(root->right==NULL&&root->left==NULL&&*sum==0)
    {
        return true;
    }
    u_int64_t lsum, rsum;
    lsum = rsum = *sum;
    return traverse(root->left, &lsum) || traverse(root->right, &rsum);
}