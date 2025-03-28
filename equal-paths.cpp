#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkDepth(Node* root, int depth, int& leafDepth){
    // edge case where root is null
    if(root == NULL){
        return true;
    }

    // we are at leaf
    if (root->left == NULL && root->right == NULL){
        // check if first leaf we found
        if(leafDepth == -1){
            leafDepth = depth;
       }
        return leafDepth == depth;
    }

    // we are not at leaf, recurse to leaf
    return checkDepth(root->left, depth+1, leafDepth) && checkDepth(root->right, depth+1, leafDepth);
    
}

bool equalPaths(Node * root)
{
    // Add your code below
    int leafDepth = -1;
    return checkDepth(root, 0, leafDepth);
}