#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#define N 5
#define MAX_THREADS 1

// Definition for a binary tree TreeNode.
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode* newTreeNode(int val)
{
    struct TreeNode* node = malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = node->right = NULL;
    return (node);
}

bool traverse(struct TreeNode* root, u_int64_t* sum)
{
    usleep(1);
    if(root==NULL) return false;

    (*sum) -= root->val;
    // printf("%d [%lu]\n", root->val, *sum);
    if(root->right==NULL&&root->left==NULL&&*sum==0)
    {
        printf("found!\n");
        return true;
    }
    u_int64_t lsum, rsum;
    lsum = rsum = *sum;
    return traverse(root->left, &lsum) || traverse(root->right, &rsum);
}

bool traverse_parallel(struct TreeNode* root, u_int64_t* sum, int level)
{
    if(root==NULL) return false;

    (*sum) -= root->val;
    if(root->right==NULL&&root->left==NULL&&*sum==0)
    {
        return true;
    }
    u_int64_t lsum, rsum;
    lsum = rsum = *sum;
    bool left, right;
    left = right = false;

    int max = omp_get_num_threads();
    if (pow(level, 2) <= max) {
        #pragma omp task shared(left)
        {
            left = traverse_parallel(root->left, &lsum, level+1);
        }
        #pragma omp task shared(right)
        {
            right = traverse_parallel(root->right, &rsum, level+1);
        }
        #pragma omp taskwait
    } else {
        left = traverse_parallel(root->left, &lsum, level+1);
        right = traverse_parallel(root->right, &rsum, level+1);
    }
    return left || right;
}

bool pathSum(struct TreeNode* root, u_int64_t* sum){
    bool res;
    #pragma omp parallel num_threads(MAX_THREADS) shared(res)
    {
        #pragma omp single
        res = traverse_parallel(root, sum, 0);
    }
    return res;
}