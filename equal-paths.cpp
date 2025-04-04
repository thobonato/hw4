#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkDepth(Node* root, int currDepth, int& firstLeafDepth){
    // edge case where root is null
    // return true since no paths to explore
    if(root == NULL){
        return true;
    }

    // we are at a leaf, track depth we got to
    if (root->left == NULL && root->right == NULL){
        // check if first leaf found
        if(firstLeafDepth == -1){
          // track leafdepth
          firstLeafDepth = currDepth;
       }
      // not at first leaf, return if depth here is equal
      return currDepth == firstLeafDepth;
    }

    // we are not at leaf, recurse to a leaf, returning the AND of all the leafs 
    // if one is false, equal paths is fully false
    return checkDepth(root->left, currDepth+1, firstLeafDepth) && checkDepth(root->right, currDepth+1, firstLeafDepth);
    
}

bool equalPaths(Node * root)
{
    // Add your code below
    // must use helper function
    int firstLeafDepth = -1;
    return checkDepth(root, 0, firstLeafDepth);
}