#define MAX_THREADS 12

bool traverse_parallel(struct TreeNode* root, u_int64_t* sum, int l, int curr_l)
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

    if (curr_l <= l) {
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
    return left || right;
}

bool pathSum(struct TreeNode* root, u_int64_t* sum){
    bool res;
    #pragma omp parallel num_threads(MAX_THREADS) shared(res)
    {
        #pragma omp single
        {
            int l = floor(log(omp_get_num_threads()) / log(2)) - 1;
            res = traverse_parallel(root, sum, l, 0);
        }
    }
    return res;
}
