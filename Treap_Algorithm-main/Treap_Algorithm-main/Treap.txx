#include <algorithm>
#include <stack>
#include <stdexcept>

#include "Treap.hpp"

#ifndef TREAP_TXX
#define TREAP_TXX

#include "Treap.hpp"
#include <algorithm> // For std::max
#include <stdexcept> // For exception handling


template <typename KeyType, typename ValueType>
Treap<KeyType, ValueType>::Treap()
    : root(nullptr) // Initialize root to nullptr
{
    std::random_device rd;
    rndgen.seed(rd());
}


template <typename KeyType, typename ValueType>
Treap<KeyType, ValueType>::~Treap()
{
    clear(root); // Recursively delete all nodes
}


template <typename KeyType, typename ValueType>
void Treap<KeyType, ValueType>::clear(Node* node)
{
    if (node) {
        clear(node->childl); // Clear left subtree
        clear(node->childr); // Clear right subtree
        delete node;         // Delete current node
    }
}


template <typename KeyType, typename ValueType>
Treap<KeyType, ValueType>::Treap(const Treap& rhs)
    : rndgen(rhs.rndgen), root(nullptr) // Initialize rndgen and root
{
    if (rhs.root) {
        root = copy(rhs.root, nullptr); // Deep copy of the treap
    }
}


template <typename KeyType, typename ValueType>
typename Treap<KeyType, ValueType>::Node* Treap<KeyType, ValueType>::copy(Node* node, Node* parentNode)
{
    if (!node)
        return nullptr; // Base case: null node

    // Create a new node with the same key and data
    Node* newNode = new Node(node->key, node->data, parentNode);
    newNode->priority = node->priority; // Preserve the priority

    // Recursively copy left and right subtrees
    newNode->childl = copy(node->childl, newNode);
    newNode->childr = copy(node->childr, newNode);

    return newNode; // Return the newly copied node
}


template <typename KeyType, typename ValueType>
Treap<KeyType, ValueType>& Treap<KeyType, ValueType>::operator=(Treap rhs)
{
    swap(rhs, *this); // Swap current treap with the copied treap
    return *this;      // Return the assigned treap
}


template <typename KeyType, typename ValueType>
void Treap<KeyType, ValueType>::swap(Treap& x, Treap& y)
{
    std::swap(x.root, y.root);       // Swap root pointers
    std::swap(x.rndgen, y.rndgen);   // Swap random number generators
}


template <typename KeyType, typename ValueType>
bool Treap<KeyType, ValueType>::empty()
{
    return root == nullptr; // Treap is empty if root is nullptr
}


template <typename KeyType, typename ValueType>
std::size_t Treap<KeyType, ValueType>::height()
{
    return computeHeight(root); // Start computation from the root
}


template <typename KeyType, typename ValueType>
std::size_t Treap<KeyType, ValueType>::computeHeight(Node* node) const
{
    if (!node)
        return 0; // Base case: null node contributes 0 to height

    // Recursively compute heights of left and right subtrees
    std::size_t leftHeight = computeHeight(node->childl);
    std::size_t rightHeight = computeHeight(node->childr);

    // Height is 1 (current node) plus the maximum of left and right subtree heights
    return 1 + std::max(leftHeight, rightHeight);
}


template <typename KeyType, typename ValueType>
ValueType Treap<KeyType, ValueType>::search(const KeyType& key, bool& found)
{
    Node* current = root; // Start search from the root

    while (current) {
        if (key == current->key) {
            found = true;            // Key found
            return current->data;    // Return associated value
        }
        else if (key < current->key) {
            current = current->childl; // Move to left child
        }
        else {
            current = current->childr; // Move to right child
        }
    }

    found = false;          // Key not found
    return ValueType();     // Return default-constructed ValueType
}


template <typename KeyType, typename ValueType>
void Treap<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value)
{
    Node* parent = nullptr; // Parent of the current node
    Node* current = root;   // Start insertion from the root

    // Traverse the treap to find the correct insertion point
    while (current) {
        parent = current;
        if (key == current->key) {
            throw std::invalid_argument("Duplicate key insertion is not allowed.");
        }
        else if (key < current->key) {
            current = current->childl; // Move to left child
        }
        else {
            current = current->childr; // Move to right child
        }
    }

    // Create a new node with the given key and value
    Node* newNode = new Node(key, value, parent);
    newNode->priority = rndgen(); // Assign a random priority

    // Attach the new node to its parent
    if (!parent) {
        root = newNode; // Treap was empty; new node becomes root
    }
    else if (key < parent->key) {
        parent->childl = newNode; // Insert as left child
    }
    else {
        parent->childr = newNode; // Insert as right child
    }

    // Restore heap property by performing rotations
    Node* node = newNode;
    while (node->parent && node->priority > node->parent->priority) {
        rotate(node); // Rotate node upwards if its priority is higher than its parent's
    }
}


template <typename KeyType, typename ValueType>
void Treap<KeyType, ValueType>::rotate(Node* node)
{
    Node* parent = node->parent; // Parent of the node to rotate
    if (!parent) return;         // Cannot rotate if node has no parent

    if (node == parent->childl) {
        // Perform right rotation
        parent->childl = node->childr; // Replace parent's left child with node's right child
        if (node->childr) {
            node->childr->parent = parent; // Update parent pointer of node's right child
        }
        node->childr = parent; // Make parent the right child of node
    }
    else {
        // Perform left rotation
        parent->childr = node->childl; // Replace parent's right child with node's left child
        if (node->childl) {
            node->childl->parent = parent; // Update parent pointer of node's left child
        }
        node->childl = parent; // Make parent the left child of node
    }

    // Update parent pointers
    node->parent = parent->parent; // Node takes parent's position in the tree
    parent->parent = node;         // Parent's new parent is node

    // Update the grandparent's child pointer
    if (node->parent) {
        if (parent == node->parent->childl) {
            node->parent->childl = node; // Node becomes left child of grandparent
        }
        else {
            node->parent->childr = node; // Node becomes right child of grandparent
        }
    }
    else {
        root = node; // Node becomes the new root if there is no grandparent
    }
}


template <typename KeyType, typename ValueType>
void Treap<KeyType, ValueType>::remove(const KeyType& key)
{
    // Find the node to remove
    Node* node = root;
    while (node && node->key != key) {
        if (key < node->key)
            node = node->childl; // Move to left child
        else
            node = node->childr; // Move to right child
    }

    if (!node) {
        throw std::invalid_argument("Key not found in treap.");
    }

    // Rotate the node down until it becomes a leaf node
    while (node->childl || node->childr) {
        if (!node->childl) {
            rotate(node->childr); // Rotate left child upwards
        }
        else if (!node->childr) {
            rotate(node->childl); // Rotate right child upwards
        }
        else {
            // Both children exist; rotate the child with higher priority
            if (node->childl->priority > node->childr->priority) {
                rotate(node->childl); // Rotate left child upwards
            }
            else {
                rotate(node->childr); // Rotate right child upwards
            }
        }
    }

    // At this point, node is a leaf and can be safely removed
    if (node->parent) {
        if (node->parent->childl == node) {
            node->parent->childl = nullptr; // Remove node from parent's left child
        }
        else {
            node->parent->childr = nullptr; // Remove node from parent's right child
        }
    }
    else {
        root = nullptr; // Treap becomes empty
    }

    delete node; // Free the memory allocated for the node
}

#endif // TREAP_TXX

