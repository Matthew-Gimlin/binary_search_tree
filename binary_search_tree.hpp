// Created by Matthew Gimlin on July 15, 2022.

// This class is a binary search tree data structure. Each node
// of the tree has a key value and a data value. Left children
// are nodes with smaller key values than its parent, and right
// children are nodes with larger key values.

#pragma once

#include <utility>
#include <queue>
#include <iostream>

template<typename KeyType, typename ValueType>
class BinarySearchTree
{
public:
    using SizeType       = std::size_t;
    using Pair           = std::pair<KeyType, ValueType>;
    using Pointer        = Pair*;
    using ConstPointer   = const Pair*;
    using Reference      = Pair&;
    using ConstReference = const Pair&;

private:
    struct BinaryNode
    {
        Pair data;
        BinaryNode* left;
        BinaryNode* right;

        /**
         * @brief Default constructor.
         * @param newData The data pair of the new node.
         * @param newLeft The left child of the new node.
         * @param newRight The right child of the new node.
         * 
         * Copies data into a new node.
         */
        BinaryNode( ConstReference newData,
                    BinaryNode* newLeft = nullptr,
                    BinaryNode* newRight = nullptr )
            : data(newData),
              left(newLeft),
              right(newRight)
        { }
        
        /**
         * @brief Move constructor.
         * @param newData The data pair of the new node.
         * @param newLeft The left child of the new node.
         * @param newRight The right child of the new node.
         * 
         * Moves data into a new node.
         */
        BinaryNode( Pair&& newData,
                    BinaryNode* newLeft = nullptr,
                    BinaryNode* newRight = nullptr )
            : data( std::move(newData) ),
              left(newLeft),
              right(newRight)
        { }
    };

    using NodePointer      = BinaryNode*;
    using ConstNodePointer = const BinaryNode*;

    NodePointer m_Root;
    SizeType m_Size;

public:
    /**
     * @brief Default constructor.
     * 
     * Creates a tree with nullptr as the root.
     */
    BinarySearchTree()
        : m_Root(nullptr),
          m_Size(0)
    { }

    /**
     * @brief Initialize constructor.
     * @param data The data of the root.
     * 
     * Creates a tree with data in the root.
     */
    BinarySearchTree(ConstReference data)
        : m_Root( new BinaryNode(data) ),
          m_Size(1)
    { }

    /**
     * @brief Copy constructor.
     * @param other The tree to copy.
     * 
     * Creates a new tree by copying the contents of the other.
     */
    BinarySearchTree(const BinarySearchTree& other)
        : m_Root( Copy(other.m_Root) ),
          m_Size(other.m_Size)
    { }

    /**
     * @brief Move constructor.
     * @param other The tree to move.
     * 
     * Creates a new tree by moving the contents of the other.
     */
    BinarySearchTree(BinarySearchTree&& other)
        : m_Root(other.m_Root),
          m_Size(other.m_Size)
    {
        other.m_Root = nullptr;
        other.m_Size = 0;
    }

    ~BinarySearchTree() { Clear(m_Root); }

    /**
     * @brief Copy assignment operator.
     * @param other The tree to copy.
     * 
     * Recreates the tree by copying the contents of the other.
     */
    BinarySearchTree& operator=(const BinarySearchTree& other)
    {
        if (this == &other) return *this;

        Clear();
        m_Root = Copy(other.m_Root);
        m_Size = other.m_Size;

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other The tree to move.
     * 
     * Recreates the tree by moving the contents of the other.
     */
    BinarySearchTree& operator=(BinarySearchTree&& other)
    {
        if (this == &other) return *this;

        Clear();
        m_Root = other.m_Root;
        m_Size = other.m_Size;
        other.m_Root = nullptr;
        other.m_Size = 0;

        return *this;
    }

    // private member data getters
    ConstReference Root() const { return ConstReference(m_Root->data); }
    SizeType Size() const { return m_Size; }
    bool Empty() const { return m_Size == 0; }

    // get minimum and maximum nodes of the tree
    ConstReference Min() const { return Min(m_Root)->data; }
    ConstReference Max() const { return Max(m_Root)->data; }

    // find nodes in the tree
    bool Contains(const KeyType& key) const { return Find(key, m_Root) != nullptr; }
    ValueType& Find(const KeyType& key) { return Find(key, m_Root)->data.second; }
    const ValueType& Find(const KeyType& key) const { return Find(key, m_Root)->data.second; }

    // delete all the nodes in the tree
    void Clear()
    {
        Clear(m_Root);
        m_Size = 0;
    }

    // insert a node into the tree
    void Insert(ConstReference data) { Insert(data, m_Root); }
    void Insert(Pair&& data) { Insert( std::move(data), m_Root ); }
    
    // remove a node from the tree
    void Erase(KeyType key) { Erase(key, m_Root); }

    // used in LevelByLevel
    static constexpr NodePointer DELIMETER = nullptr;

    /**
     * @brief Level by level traversal.
     * @param out The stream to print the tree out.
     * 
     * Outputs the nodes in each level of the tree.
     */
    void LevelByLevel(std::ostream& out = std::cout) const
    {
        if (m_Root == nullptr) return;

        ConstNodePointer DELIMETER = nullptr;

        std::queue<ConstNodePointer> q;
        q.push(m_Root);
        q.push(DELIMETER);

        while (1)
        {
            ConstNodePointer curr = q.front();
            q.pop();

            // the current level has been traversed
            if (curr == DELIMETER)
            {
                out << '\n';
                
                if (q.empty()) break;
                q.push(DELIMETER);
            }

            // the current level is still being traversed
            else
            {
                out << curr->data.second << ' ';

                // push children nodes for the next level
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
        }
    }

private:
    /**
     * @brief Finds the minimum node.
     * @param node The root of the tree to find the minimum key in.
     * @return The minimum node.
     * 
     * Finds the node with the smallest key value.
     */
    ConstNodePointer Min(ConstNodePointer node) const
    {
        if (node == nullptr) return nullptr;

        // the minimum is the furthest left child
        while (node->left)
            node = node->left;
        
        return node;
    }

    /**
     * @brief Finds the maximum node.
     * @param node The root of the tree to find the maximum key in.
     * @return The maximum node.
     * 
     * Finds the node with the largest key value.
     */
    ConstNodePointer Max(ConstNodePointer node) const
    {
        if (node == nullptr) return nullptr;

        // the maximum is the furthest right child
        while (node->right)
            node = node->right;
        
        return node;
    }

    /**
     * @brief Finds a node in the tree.
     * @param key The key of the node to find.
     * @param node The root of the tree to find in.
     * @return The node with the key value.
     * 
     * Finds the node with a certain key value in the tree.
     */
    NodePointer Find(const KeyType& key, NodePointer node)
    {
        if (node == nullptr) return nullptr;

        if (key < node->data.first)
            return Find(key, node->left);
        
        else if (key > node->data.first)
            return Find(key, node->right);
        
        else return node;
    }

    /**
     * @brief Finds a node in the tree.
     * @param key The key of the node to find.
     * @param node The root of the tree to find in.
     * @return The node with the key value.
     * 
     * Finds the node with a certain key value in the tree.
     */
    ConstNodePointer Find(const KeyType& key, NodePointer node) const
    {
        if (node == nullptr) return nullptr;

        if (key < node->data.first)
            return Find(key, node->left);
        
        else if (key > node->data.first)
            return Find(key, node->right);
        
        else return node;
    }

    /**
     * @brief Copies a tree.
     * @param node The current node to copy.
     * @return The root of the new tree.
     * 
     * Recursively copies the nodes of a tree.
     */
    NodePointer Copy(ConstNodePointer node)
    {
        if (node == nullptr) return nullptr;

        // copy the node and its children
        NodePointer root = new BinaryNode(node->data);
        root->left = Copy(node->left);
        root->right = Copy(node->right);

        return root;
    }

    /**
     * @brief Deletes the tree.
     * @param node The root of the tree to delete.
     * 
     * Recursively destructs each node of the tree.
     */
    void Clear(NodePointer& node)
    {
        if (node == nullptr) return;

        // delete a node's children before itself
        if (node->left) Clear(node->left);
        if (node->right) Clear(node->right);

        delete node;
        node = nullptr;
    }
    
    /**
     * @brief Inserts a new node into the tree.
     * @param data The data of the new node.
     * @param node The node to search for a place to insert.
     * @return The newly inserted node.
     * 
     * Recursively finds a place to insert, and copies the data into a new node.
     */
    NodePointer Insert(ConstReference data, NodePointer& node)
    {
        if (node == nullptr)
        {
            ++m_Size;
            node = new BinaryNode(data);
        }

        // smaller key values go to the left child
        else if (data.first < node->data.first)
            node->left = Insert(data, node->left);
        
        // larger key values go to the right child
        else if (data.first > node->data.first)
            node->right = Insert(data, node->right);
        
        return node;
    }

    /**
     * @brief Inserts a new node into the tree.
     * @param data The data of the new node.
     * @param node The node to search for a place to insert.
     * @return The newly inserted node.
     * 
     * Recursively finds a place to insert, and moves the data into a new node.
     */
    NodePointer Insert(Pair&& data, NodePointer& node)
    {
        if (node == nullptr)
        {
            ++m_Size;
            node = new BinaryNode( std::move(data) );
        }

        // smaller key values go to the left child
        else if (data.first < node->data.first)
            node->left = Insert( std::move(data), node->left );
        
        // larger key values go to the right child
        else if (data.first > node->data.first)
            node->right = Insert( std::move(data), node->right );
        
        return node;
    }

    /**
     * @brief Erases a node from the tree.
     * @param key The key of the node to delete.
     * @param node The root of the tree to delete in.
     * @return The node that replaced the deleted one.
     * 
     * Recursively finds a node, and deletes it.
     */
    NodePointer Erase(const KeyType& key, NodePointer& node)
    {
        if (node == nullptr) return nullptr;

        // smaller key values go to the left child
        if (key < node->data.first)
            node->left = Erase(key, node->left);
        
        // larger key values go to the right child
        else if (key > node->data.first)
            node->right = Erase(key, node->right);
        
        // the node to delete has two children
        // replace this node with the smallest in the right subtree
        else if (node->left && node->right)
        {
            node->data = Min(node->right)->data;
            Erase(node->data.first, node->right);
        }

        // the node to delete has one or zero children
        // replace this node with its child (if it has one)
        else
        {
            NodePointer old = node;

            // if the node has no children, it is set to nullptr
            if (node->left) node = node->left;
            else node = node->right;

            delete old;
            --m_Size;
        }

        return node;
    }
};
