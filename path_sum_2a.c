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
#define MAX_THREADS 12

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
    if(root->right==NULL&&root->left==NULL&&*sum==0)
    {
        // printf("found!\n");
        return true;
    }
    u_int64_t lsum, rsum;
    lsum = rsum = *sum;
    return traverse(root->left, &lsum) || traverse(root->right, &rsum);
}

bool traverse_parallel(struct TreeNode* root, u_int64_t* sum, int l, int curr_l)
{
    usleep(1);
    if(root==NULL) return false;

    // printf("Current value: %d, want: %lu\n", root->val, *sum);
    (*sum) -= root->val;
    if(root->right==NULL&&root->left==NULL&&*sum==0)
    {
        printf("found!\n");
        return true;
    }
    u_int64_t lsum, rsum;
    lsum = rsum = *sum;
    bool left, right;

    if (curr_l <= l) {
        printf("running in parallel curr_l: %d\n", curr_l);
        #pragma omp task shared(left)
        {
            left = traverse_parallel(root->left, &lsum, l, curr_l+1);
        }
        #pragma omp task shared(right)
        {
            right = traverse_parallel(root->right, &rsum, l, curr_l+1);
        }
        #pragma omp taskwait
    } else {
        left = traverse(root->left, &lsum);
        right = traverse(root->right, &rsum);
    }
    // printf("Current value: %d, %d, %d\n", root->val, left, right);
    return left || right;
}

bool pathSum(struct TreeNode* root, u_int64_t* sum){
    // traverse(root, sum);
    bool res;
    #pragma omp parallel num_threads(MAX_THREADS) shared(res)
    {
        #pragma omp single
        {
            int l = floor(log(omp_get_num_threads()) / log(2));
            res = traverse_parallel(root, sum, l, 0);
        }
    }
    return res;
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

int dfs(struct TreeNode* root) {
    if (root==NULL) return 0;
    printf("%d\n", root->val);
    return root-> val + dfs(root->left);
}

int middfs(struct TreeNode* root, bool left) {
    if (root==NULL) return 0;
    printf("%d\n", root->val);
    if (left) {
        return root-> val + middfs(root->left, !left);
    } else {
        return root-> val + middfs(root->right, !left);
    }
}

int parentIndex(int i) {
    bool lchild = false;
    if (i % 2 == 1) {
        lchild = true;
    }
    if (lchild) {
        return (i - 1) / 2;
    } else {
        return (i - 2) / 2;
    }
}

struct TreeNode** readTree() {
    struct TreeNode** nodes = malloc(sizeof(struct TreeNode*)*100000);
    FILE* f = fopen("example100000.txt", "r");
    bool flag = false;
    int i = 0;
    int curr_num = 0;
    bool isNull = false;
    while (!flag) {
        int c = fgetc(f);
        // printf("got: %d\n", c);
        if (c == 'n') {
            isNull = true;
            continue;
        }else if (c == ',' || c == ']') {
            // printf("i: %d\n", i);
            if (c == ']') {
                flag = true;
            }
            if (isNull) {
                // printf("null\n");
                isNull = false;
                i++;
                continue;
            }
            // printf("whole num %d\n", curr_num);
            struct TreeNode* newNode = newTreeNode(curr_num);
            curr_num = 0;
            nodes[i] = newNode;
            if (i == 0) {
                i++;
                continue;
            }
            int parent = parentIndex(i);
            if (i % 2 == 1) {
                nodes[parent]->left = newNode;
                // printf("left of %d\n", nodes[parent]->val);
                
            } else {
                nodes[parent]->right = newNode;
                // printf("right of %d\n", nodes[parent]->val);
            }
            i++;
            continue;
        }
        int num = c - '0';
        if (num < 0 || num > 9) {
            // printf("other char\n");
            continue;
        }
        // printf("num %d\n", num);
        curr_num = curr_num * 10 + num;
    }

    return nodes;
}

int main() {
    struct TreeNode** nodes = readTree();
    struct TreeNode* root = nodes[0];
    
    // 27
    // 3904.
    // 37574.

    // 26.
    // 2638.
    // 24941.
    u_int64_t* sum = malloc(sizeof(u_int64_t));
    *sum = 25;

    // printf("dfs: %d\n", dfs(root));
    // printf("middfs: %d\n", middfs(root, false));

    double start = omp_get_wtime();
    bool exists = pathSum(root, sum);
    double end = omp_get_wtime();
    if (exists) {
        printf("Sol exists!");
    }
    printf("time: %f\n", end - start);
    free(nodes);
    return 0;
}