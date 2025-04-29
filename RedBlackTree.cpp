#include <iostream>
#include <stdexcept>
#include <string>

#include "RedBlackTree.h"

//default constructor

RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

//initial data

RedBlackTree::RedBlackTree(int newData) {
    root = new RBTNode{newData, COLOR_RED, nullptr, nullptr, nullptr, false};
    numItems = 1;
    root->color = COLOR_BLACK;
}


//copy constructor

RedBlackTree::RedBlackTree(const RedBlackTree &rbt) {
    root = CopyOf(rbt.root);
    numItems = rbt.numItems;
}

// infix - left,root, right for tree
string RedBlackTree::ToInfixString(const RBTNode *n) {
    if (n == nullptr || n->IsNullNode) return "";
    string result = ToInfixString(n->left);
    result += (n->color == COLOR_BLACK ? " B" : " R") + to_string(n->data);
    result += ToInfixString(n->right);
    return result;
}

//prefix - root, left, right

string RedBlackTree::ToPrefixString(const RBTNode *n) {
    if (n == nullptr || n->IsNullNode) return "";
    string result = (n->color == COLOR_BLACK ? " B" : " R") + to_string(n->data) + " ";
    result += ToPrefixString(n->left);
    result += ToPrefixString(n->right);
    return result;
}

//postfix - left, right, root

string RedBlackTree::ToPostfixString(const RBTNode *n) {
    if (n == nullptr || n->IsNullNode) return "";
    string result = ToPostfixString(n->left);
    result += ToPostfixString(n->right);
    result += (n->color == COLOR_BLACK ? "B" : "R") + to_string(n->data) + " ";
    return result;
}

//new node with given data into tree

void RedBlackTree::Insert(int newData) {
    RBTNode* newNode = new RBTNode{newData, COLOR_RED, nullptr, nullptr, nullptr, false};
    BasicInsert(newNode);
    InsertFixUp(newNode);
    numItems++;
}

//check to see if data is contained

bool RedBlackTree::Contains(int data) const {
    return Get(data) != nullptr;
}

//returns min value for tree

int RedBlackTree::GetMin() const {
    if (root == nullptr) throw runtime_error("Tree is empty");
    RBTNode* node = root;
    while (node->left != nullptr && !node->left->IsNullNode) {
        node = node->left;
    }
    return node->data;
}

//return max value for tree

int RedBlackTree::GetMax() const {
    if (root == nullptr) throw runtime_error("Tree is empty");
    RBTNode* node = root;
    while (node->right != nullptr && !node->right->IsNullNode) {
        node = node->right;
    }
    return node->data;
}

//insert the node in the tree

void RedBlackTree::BasicInsert(RBTNode *node) {
    if (root == nullptr) {
        root = node;
        root->color = COLOR_BLACK; // Root is always black
        return;
    }
    
    RBTNode* current = root;
    RBTNode* parent = nullptr;
    
    while (current != nullptr && !current->IsNullNode) {
        parent = current;
        if (node->data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    node->parent = parent;
    if (node->data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }
}

//fixes red-black after insertion 

void RedBlackTree::InsertFixUp(RBTNode *node) {

    while (node != root && node->parent->color == COLOR_RED) {
        if (IsLeftChild(node->parent)) {

            RBTNode *uncle = node->parent->parent->right;
            if (uncle != nullptr && uncle->color == COLOR_RED) {

                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                node = node->parent->parent;
            } else {

                if (IsRightChild(node)) {

                    node = node->parent;
                    LeftRotate(node);
                }

                node->parent->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                RightRotate(node->parent->parent);
            }
        } else {
            RBTNode *uncle = node->parent->parent->left;

            if (uncle != nullptr && uncle->color == COLOR_RED) {
                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                node = node->parent->parent;
            } else {

                if (IsLeftChild(node)) {
                    node = node->parent;
                    RightRotate(node);
                }
                
                node->parent->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                LeftRotate(node->parent->parent);

            }
        }
    }
    root->color = COLOR_BLACK;
}

//finds the node 

RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* current = root;
    while (current != nullptr && !current->IsNullNode) {
        if (data == current->data) {
            return current;
        } else if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

//check if given node is left, right
bool RedBlackTree::IsLeftChild(RBTNode *node) const {
    return node == node->parent->left;
}

//check if given node is right, left
bool RedBlackTree::IsRightChild(RBTNode *node) const {
    return node == node->parent->right;
}

//left rotation

void RedBlackTree::LeftRotate(RBTNode *node) {
    RBTNode *y = node->right;
    node->right = y->left;
    if (y->left != nullptr && !y->left->IsNullNode) {
        y->left->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == nullptr) {
        root = y;
    } else if (node == node->parent->left) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }
    y->left = node;
    node->parent = y;
}
//right rotation

void RedBlackTree::RightRotate(RBTNode *node) {
    RBTNode *y = node->left;
    node->left = y->right;
    if (y->right != nullptr && !y->right->IsNullNode) {
        y->right->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == nullptr) {
        root = y;
    } else if (node == node->parent->right) {
        node->parent->right = y;
    } else {
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
}
//creates copy from given node 

RBTNode* RedBlackTree::CopyOf(const RBTNode *node) {
    if (node == nullptr) return nullptr;
    RBTNode* newNode = new RBTNode{node->data, node->color, nullptr, nullptr, nullptr, false};
    newNode->left = CopyOf(node->left);
    newNode->right = CopyOf(node->right);
    if (newNode->left != nullptr) newNode->left->parent = newNode;
    if (newNode->right != nullptr) newNode->right->parent = newNode;
    return newNode;
}
