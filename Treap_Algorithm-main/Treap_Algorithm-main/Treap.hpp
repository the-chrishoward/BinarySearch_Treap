#ifndef _TREAP_H_
#define _TREAP_H_

#include <random>

#include "abstract_bst.hpp"

template <typename KeyType, typename ValueType>
class Treap : public AbstractBST<KeyType, ValueType>
{
public:
  Treap();
  
  ~Treap();

  Treap(const Treap& rhs);

  Treap& operator=(Treap rhs);

  void swap(Treap& x, Treap& y);
  
  bool empty();

  void insert(const KeyType& key, const ValueType& value);
  
  ValueType search(const KeyType& key, bool& found);
  
  void remove(const KeyType& key);
  
  std::size_t height();

private:
    // Random number generator
    std::mt19937 rndgen;


    struct Node
    {
        KeyType key; // Key of the node
        ValueType data; // Associated value
        std::mt19937::result_type priority; // Priority for heap property
        Node* parent; // Pointer to parent node
        Node* childl; // Pointer to left child
        Node* childr; // Pointer to right child

        Node(const KeyType& k, const ValueType& d, Node* p = nullptr)
            : key(k), data(d), priority(0), parent(p), childl(nullptr), childr(nullptr)
        {
            // Priority will be assigned during insertion
        }
    };

    Node* root; // Pointer to the root of the treap

    // Private helper methods

    // Recursively clear all nodes
    void clear(Node* node);
 
    // Recursively copies all nodes
    Node* copy(Node* node, Node* parentNode);

    // Computes the height given the current node
    std::size_t computeHeight(Node* node) const;

    // Performs a rotation
    void rotate(Node* node);
};

#include "Treap.txx"

#endif // _TREAP_H_