#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void rotateRight(AVLNode<Key,Value>* n);
    virtual void rotateLeft(AVLNode<Key,Value>* n);

};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x){
  AVLNode<Key, Value>* y = x->getLeft();

  // rotate right
  x->setLeft(y->getRight());
  if(y->getRight() != NULL){
    // set parent
    y->getRight()->setParent(x);
  }

  // update parents
  y->setParent(x->getParent());
  // check if need to set root
  if(x->getParent() == NULL){
    this->root_ = y;
  } 
  else if(x == x->getParent()->getRight()){ // see if update parent right or left
    x->getParent()->setRight(y);
  } else {
    x->getParent()->setLeft(y);
  }

  // final settings
  y->setRight(x);
  x->setParent(y);

  // update balance factors
  if (y->getBalance() == 0) {
      // special case (common during deletion)
      x->setBalance(-1);
      y->setBalance(1);
  } else {
      x->setBalance(0);
      y->setBalance(0);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x){
  AVLNode<Key, Value>* y = x->getRight();

  // rotate left
  x->setRight(y->getLeft());
  if(y->getLeft() != NULL){
    // set parent
    y->getLeft()->setParent(x);
  }

  // update parents
  y->setParent(x->getParent());
  // check if need to set root
  if(x->getParent() == NULL){
    this->root_ = y;
  } 
  else if(x == x->getParent()->getRight()){ // see if update parent right or left
    x->getParent()->setRight(y);
  } else {
    x->getParent()->setLeft(y);
  }

  // final settings
  y->setLeft(x);
  x->setParent(y);

  // update balance factors
  if (y->getBalance() == 0) {
      // special case (common during deletion)
      x->setBalance(1);
      y->setBalance(-1);
  } else {
      x->setBalance(0);
      y->setBalance(0);
  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    Key key = new_item.first;
    Value val = new_item.second;
    AVLNode<Key, Value>* placedAt;

    // tree is empty, just add
    if(this->root_ == NULL){
        this->root_ = new AVLNode<Key, Value>(key, val, NULL);
        // store placement, set balance
        placedAt = static_cast<AVLNode<Key, Value>*>(this->root_);
        placedAt->setBalance(0);
        // exit
        return;
    }
    
    // loop until tree is at an end or you found the key
    AVLNode<Key, Value>* tmp = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(tmp != NULL){
        // found the exact key, overwrite and return
        if(tmp->getKey() == key){
            tmp->setValue(val);
            return;
        }
        // still looking, decide to look left or right
        if(key > tmp->getKey()){
            // if right is null, add it there
            if(tmp->getRight() == NULL){
                tmp->setRight(new AVLNode<Key, Value>(key, val, tmp));
                // store placement, set balance
                placedAt = static_cast<AVLNode<Key, Value>*>(tmp->getRight());
                placedAt->setBalance(0);
                // exit while
                break;
            }
            // else, make tmp right
            tmp = tmp->getRight();
        } else {
            // if left is null, add it there
            if(tmp->getLeft() == NULL){
                tmp->setLeft(new AVLNode<Key, Value>(key, val, tmp));
                
                // store placement, set balance
                placedAt = static_cast<AVLNode<Key, Value>*>(tmp->getLeft());
                placedAt->setBalance(0);
                // exit while
                break;
            }
            // else, make tmp left
            tmp = tmp->getLeft();
        }
    }

  // travel up and find the first unbalanced node
  AVLNode<Key, Value>* z = placedAt;
  AVLNode<Key, Value>* y;
  while (z->getParent() != NULL){
    y = z;
    z = z->getParent();

    // as you travel up,updateBalance
    if(z->getLeft() == y){
      z->updateBalance(-1);
    } else {
      z->updateBalance(1);
    }

    // subtree hasn't changed
    if(z->getBalance() == 0){
      break;
    }
    
    // right-heavy imbalance
    if (z->getBalance() > 1) {
        // right-right case
        if (z->getRight()->getBalance() >= 0) {
            // left rotation
            rotateLeft(z);
        }
        // right-left case
        else {
            // first rotate right on child, then left on z
            rotateRight(z->getRight());
            rotateLeft(z);
        }
        break;
    }
    // left-heavy imbalance
    else if (z->getBalance() < -1) {
      // left-left case
      if (z->getLeft()->getBalance() <= 0) {
          // right rotation
          rotateRight(z);
      }
      // left-right case
      else {
          // first rotate left on child, then right on z
          rotateLeft(z->getLeft());
          rotateRight(z);
      }
      break;
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* curr = nullptr;
    if (this->root_ != nullptr){
        curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    }
    AVLNode<Key, Value>* parent = NULL;
    bool wasLeftChild = false;
    bool found = false;

    // looking for node to remove
    while(curr != NULL){
        // found the exact key
        if(curr->getKey() == key){
            found = true;
            
            // check if node has two children
            if(curr->getLeft() != NULL && curr->getRight() != NULL){
                Node<Key, Value>* predNode = this->predecessor(static_cast<Node<Key, Value>*>(curr));
                AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(predNode);
                
                // store parent before swap
                parent = pred->getParent();
                if (parent == curr) {
                    // predecessor is child of curr
                    parent = pred;
                }
                nodeSwap(curr, pred);
            } else {
                parent = curr->getParent();
            }

            // now curr has at most one child
            // store if this was left or right child for rebalancing
            wasLeftChild = (parent != NULL && parent->getLeft() == curr);

            // find replacement 
            AVLNode<Key, Value>* repl = NULL;
            if(curr->getLeft() != NULL){
                repl = curr->getLeft();
            } else {
                repl = curr->getRight();
            }

            if(curr->getParent() == NULL){
                // removing root
                this->root_ = repl;
                if (repl != NULL) repl->setParent(NULL);
            } else {
                // not root
                if(parent->getLeft() == curr){
                    parent->setLeft(repl);
                } else {
                    parent->setRight(repl);
                }

                if(repl != NULL){
                    repl->setParent(parent);
                }
            }

            delete curr;

            // if node not found or tree is now empty, we are done
            if (!found || parent == NULL) {
                return;
            }
            
            // Rebalance the tree starting from parent of the deleted node
            AVLNode<Key, Value>* n = parent;
            bool heightDecreased = true; // Flag indicating subtree height decreased
            
            while (n != NULL && heightDecreased) {
                int8_t oldBalance = n->getBalance();
                
                // Update balance factor based on which child was removed
                if (wasLeftChild) {
                    n->updateBalance(1); // Lost a left child, balance increases
                } else {
                    n->updateBalance(-1); // Lost a right child, balance decreases
                }
                
                int8_t newBalance = n->getBalance();
                
                // Check if we need to perform a rotation
                if (newBalance < -1 || newBalance > 1) {
                    // Save old parent to determine child direction for next iteration
                    AVLNode<Key, Value>* oldParent = n->getParent();
                    bool wasLeftChildTemp = (oldParent != NULL && oldParent->getLeft() == n);
                    
                    // Perform rotations
                    if (newBalance > 1) {
                        // Right-heavy
                        AVLNode<Key, Value>* rightChild = n->getRight();
                        if (rightChild->getBalance() >= 0) {
                            // Right-Right case
                            rotateLeft(n);
                        } else {
                            // Right-Left case
                            rotateRight(rightChild);
                            rotateLeft(n);
                        }
                    } else {
                        // Left-heavy
                        AVLNode<Key, Value>* leftChild = n->getLeft();
                        if (leftChild->getBalance() <= 0) {
                            // Left-Left case
                            rotateRight(n);
                        } else {
                            // Left-Right case
                            rotateLeft(leftChild);
                            rotateRight(n);
                        }
                    }
                    
                    // After rotations, the height might be unchanged
                    heightDecreased = (n->getBalance() == 0);
                    
                    // Update for next iteration
                    wasLeftChild = wasLeftChildTemp;
                    n = oldParent;
                } else {
                    // No rotation needed
                    // If balance went from ±1 to 0, height decreased
                    // If balance went from 0 to ±1, height unchanged
                    heightDecreased = (oldBalance != 0 && newBalance == 0);
                    
                    // Update for next iteration
                    wasLeftChild = (n->getParent() != NULL && n->getParent()->getLeft() == n);
                    n = n->getParent();
                }
            }
            
            return;
        }

        // still looking for key, decide to look left or right
        if(key > curr->getKey()){
            // go right
            curr = curr->getRight();
        } else {
            // go left
            curr = curr->getLeft();
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
