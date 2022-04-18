/*
==================================================================================================
Name        :   BinarySearchTree.h
Author      :   Craig O'Loughlin
Version     :   1
Date        :   04/17/2022

Description:

    A custom binary tree implementation using data nodes constructed on heap. There are functions
    for insertion and searching, standard forward iterator access as well as destructor to delete 
    all nodes. There is no balancing available on this tree, it is up to the user at the moment
    to ensure decent balance.

==================================================================================================
*/

#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iterator>
#include <cstddef>
#include <vector>

/**
 * @brief Basic binary tree with insert and search functions. Can forward iterate all elements.
 * 
 * Average:
 * Insertion: O(logn) per item
 * Search: O(logn) per search
 * Print all in order: O(n) (at least 3n)
 * 
 * @param T any data object. If using a custom object it must define at least operator< and operator== comparisons
 *          against self. The object class should contain an identification field that will be used 
 *          for positioning and searching of data objects in this tree.
 * 
 */
template <typename T>
class BinarySearchTree {

    /* A template class is written with a generic type and is compiled to a specific type when needed,
    like when an instantiation of a type is asked for in code.

    A template class is normally written in a header file because it is compiled on request. If this code
    is in a .cpp file it never compiles (unless a specific type is explicitly instantiated in that file).

    I wanted to make this tree generic as an exercise (never done it before) and also this tree
    can now work with many different types of data!
    */

    private:

        // private node class, contains a field to store an object and left and right nodes. 
        // assign data during instantiation
        template <typename dataT>
        struct Node {
            dataT data;
            Node* left = nullptr;
            Node* right = nullptr;

            Node(dataT data) {
                this->data = data;
            }
        };

        // private fields/functions
        Node<T>* root;
        void addNode(Node<T>* node, T data);
        void inOrder(Node<T>* node);
        T recursiveSearch(Node<T>* node, T searchData);
        void deleteNodesFrom(Node<T>* node);

        std::vector<Node<T>*> addressStack; // for in order element iterator //change to a root node..ll?

    public:

        // accessible functions
        BinarySearchTree();
        virtual ~BinarySearchTree();
        void Insert(T data);
        T Search(T searchData);
        void Clear();
        bool isEmpty();

        struct BST_Iterator {

            // https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp

            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;

            BST_Iterator(Node<T>** ptr) { current = ptr; } // constructor
            T operator*() const { return (*current)->data; } 
            Node<T>** operator->() { return current; }      
            BST_Iterator& operator++() { // define what to do when ++iter, this case pointer+1
                current++; 
                return *this; 
            }

            // when comparing iterator objects, compare their current addresses
            friend bool operator==(const BST_Iterator& lhs, const BST_Iterator& rhs) { return lhs.current == rhs.current; }
            friend bool operator!=(const BST_Iterator& lhs, const BST_Iterator& rhs) { return lhs.current != rhs.current; }

            private:
                Node<T>** current; // address of a node address 
                                    //i think of https://i.redd.it/2a1nq2qnxkq81.png
        };

        BST_Iterator begin() {

            // O(n) operation to create this iterator but simplest to implement
            // https://www.cs.odu.edu/~zeil/cs361/latest/Public/treetraversal/index.html

            /* This iterator works by opening each node on stack in order low -> high and adding each node address
                to a vector. The vector start address is returned on begin(). Vectors are guaranteed contiguous memory
                locations so on each iteration the starting pointer is incremented to get the next node in order.

                The reason iterators exist is to provide uniform access to elements. The reason I added this code is for a learning,
                I also like how objects stored here can be worked with in iterator loops like other std structures. Makes 
                this tree feel more "official".
            */
            
            addressStack.clear();
            inOrder(root);  // create a vector of addresses in order
            addressStack.push_back(nullptr);    // add a final position to vector
            return BST_Iterator(&addressStack[0]); // return the starting address of the vector

        } //end is the final position in the address "stack" (including the empty nullspot)
        BST_Iterator end() { return BST_Iterator(&addressStack.at(addressStack.size() - 1)); }
};

// constructor
template <typename T>
BinarySearchTree<T>::BinarySearchTree() { 
    root = nullptr;
}

// destructor
template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    deleteNodesFrom(root);
}

/**
 * @brief Add a data object to this tree in ordered position. Uses operator< to find position.
 * 
 * @param data any object class. 
 */
template <typename T>
void BinarySearchTree<T>::Insert(T data) {

    if (root == nullptr) {
        root = new Node<T>(data);
    }
    else {
        addNode(root, data);
    }
}

// recursive search for the datapoint insertion location
template <typename T>
void BinarySearchTree<T>::addNode(Node<T>* node, T data) {

    if (node == nullptr) return;

    if (data < node->data) { // courses operator< overload

        // move down the left node and continue searching for a spot
        if (node->left == nullptr) node->left = new Node<T>(data);
        else addNode(node->left, data);

    }
    else {
        
        // or, move down the right node and continue searching for a spot
        if (node->right == nullptr) node->right = new Node<T>(data);
        else addNode(node->right, data);
    }
}

// open each node on stack in order from 'node'. Add in order to addresslist
template <typename T>
void BinarySearchTree<T>::inOrder(Node<T>* node) {

    if (node == nullptr) return;

    // left -> node -> right
    inOrder(node->left);
    addressStack.push_back(node); //add 'node' (which is an address to node) to address "stack" for iterator
    inOrder(node->right);
}


/** @brief Get the specified object from the tree. Uses operator== and operator<.
 * @param searchData data object with the identifying field filled.
 * @return A copy of the data object if found else searchData
 */
template <typename T>
T BinarySearchTree<T>::Search(T searchData) { //Get
    return recursiveSearch(root, searchData);
}

template <typename T>
T BinarySearchTree<T>::recursiveSearch(Node<T>* node, T searchData) {

    /*--- it is up to the object to define what field is compared ---*/

    if (node == nullptr) return searchData; // case: not found
    if (node->data == searchData) return node->data; // case: found

    // the search continues
    if (searchData < node->data) {
        return recursiveSearch(node->left, searchData);
    } else {
        return recursiveSearch(node->right, searchData);
    }
}

/** @brief Empty the tree of all contents.
 */
template <typename T>
void BinarySearchTree<T>::Clear() {
    deleteNodesFrom(root);
    root = nullptr;
}

// get all node addresses and delete
template <typename T>
void BinarySearchTree<T>::deleteNodesFrom(Node<T>* node) {
    if (node == nullptr) return;
    deleteNodesFrom(node->left);
    deleteNodesFrom(node->right);
    delete node;
}

/** @brief Is it?
 */
template <typename T>
bool BinarySearchTree<T>::isEmpty() {
    return root == nullptr;
}

#endif