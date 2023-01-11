#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

#define N 5

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

void traverse(struct TreeNode* root,int sum,int **ret,int *returnSize,int** returnColumnSizes,int *arr,int idx)
{
    if(root==NULL)
        return;
    printf("%d\n", root->val);
    sum-=root->val;
    arr[idx++]=root->val;
    if(root->right==NULL&&root->left==NULL&&sum==0)
    {
        (*returnColumnSizes)[*returnSize]=idx;
        ret[*returnSize]=malloc(sizeof(int)*idx);
        memcpy(ret[*returnSize], arr, idx * sizeof(int));
        (*returnSize)++;
    }
    traverse(root->left,sum,ret,returnSize,returnColumnSizes,arr,idx);
    traverse(root->right,sum,ret,returnSize,returnColumnSizes,arr,idx);
    
}
int** pathSum(struct TreeNode* root, int sum, int* returnSize, int** returnColumnSizes){
    *returnSize=0;
    
    if(root==NULL)
        return NULL;
    
    *returnColumnSizes=malloc(sizeof(int)*50);
    int **ret=malloc(sizeof(int*)*50);
    int *arr=malloc(sizeof(int)*100);
    int idx=0;
    traverse(root,sum,ret,returnSize,returnColumnSizes,arr,idx);
    printf("Return size: %d\n", *returnSize);
    return ret;
}

void printMatrix(int** columnSizes, int* returnSize, int** matrix) {
    int row, columns;
    for (row=0; row<*returnSize; row++)
    {
        int numberColumns = (*columnSizes)[row];
        for(columns=0; columns<numberColumns; columns++)
        {
            printf("%d     ", matrix[row][columns]);
        }
        printf("\n");
    }
}

void dfs(struct TreeNode* root) {
    if (root==NULL) return;
    printf("%d\n", root->val);
    dfs(root->left);
    dfs(root->right);
}

int main() {
    struct TreeNode *root = newTreeNode(5);
    root->left = newTreeNode(4);
    root->right = newTreeNode(8);
    root->left->left = newTreeNode(11);
    root->left->left->left = newTreeNode(7);
    root->left->left->right = newTreeNode(2);
    root->right->left = newTreeNode(13);
    root->right->right = newTreeNode(4);
    root->right->right->right= newTreeNode(1);
    root->right->right->left= newTreeNode(5);
    int* returnSize = malloc(sizeof(int));
    int* columnSizes = malloc(sizeof(int));

    int** sol = pathSum(root, 22, returnSize, &columnSizes);
    printMatrix(&columnSizes, returnSize, sol);
    printf("Max pathSum of the given binary tree is: %d", *returnSize);
    return 0;
}