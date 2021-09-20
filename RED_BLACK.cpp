#include<iostream>
using namespace std;

// Node of a Red-Black Tree
struct Node {
    bool red;
    int val;
    Node *left, *right;
    Node(): val(0), red(1), left(nullptr),  right(nullptr) {}
    Node(int v): val(v), red(1), left(nullptr),  right(nullptr) {}
    Node(int v, int r): val(v), red(r), left(nullptr),  right(nullptr) {}
};

class RB_TREE   {
    Node *root;
    int height;

/*
    Node* _insert(Node *root, int key, bool is_left_child=0)    {
        // compare with current value
        if (root->val < key)    {
            // go left if possible, else create new node at left
            if (root->left)
                _insert(root->left, key, 1);
            else
                root->left = new Node(key); 
        }
        // similarly for right
        else    {
            if (root->right)
                _insert(root->right, key, 0);
            else
                root->right = new Node(key);
        }

        // check for problems

        // if both children exist and are red, then this node must be black
        if (root->left && root->right && root->left->red && root->right->red) {
            // change the children and parent colors
            root->red = 1;
            root->left->red = 0;
            root->right->red = 0;
            return nullptr;
        }

        // now if root is black, no problem
        if (!root->red)
            return nullptr;

        // double red problem, if root node and child node are red
        if (root->left && root->left->red)  {
            // check if this node (root) is the left or the right child
            if (!is_left_child)     // right child
                return rotate_left(root);
            else
                return root;
        }

        if (root->right && root->right->red)  {
            // check if this node (root) is the left or the right child
            if (is_left_child)     // right child
                return rotate_right(root);
            else
                return root;
        }
        return nullptr;
    }
*/
    
    Node* _insert(Node* root, int key, bool is_left_child=0)    {
        // find appropriate place for the key
        if (root->val < key)    {
            if (root->left)
                _insert(root->left, key, 1);
            else    // make new node
                root->left = new Node(key, 1);
        }
        else if (root->val > key)   {
            if (root->right)
                _insert(root->right, key, 1);
            else
                root->right = new Node(key, 1);
        }
        else    // key already exists
            return nullptr;
        

        // check for problems
        
        // if both children exist and are red, then this node must be black
        if (root->left && root->right && root->left->red && root->right->red) {
            // change the children and parent colors
            root->red = 1;
            root->left->red = 0;
            root->right->red = 0;
            return nullptr;
        }

        // now, if root is black, then no problem
        if (!root->red)
            return nullptr;
        
        // check for double red problem
        if (root->left && root->left->red)  {
            // if root is a left child, then no internal rotation required
            if (!is_left_child)
                return root;
            // otherwise, internal rotation required
            
        }
    }

    // performs right rotation on root, and returns new root
    Node* rotate_right(Node *root)   {
        Node *left_child = root->left;
        Node *cright = left_child->right;
        root->left = cright;
        left_child->right = root;
        return left_child;
    }

    // performs left rotation on root, and returns new root
    Node* rotate_left(Node *root)   {
        Node* right_child = root->right;
        Node *cleft = right_child->left;
        root->right = cleft;
        right_child->left = root;
        return right_child;
    }

public:
    RB_TREE()   {
        root = nullptr;
        height = 0;
    }

    // Insert a key method
    void insert(int key)    {
        if (!root)  {
            root = new Node(key, 0);
            return;
        }

        _insert(root, key);

    }

};

int main()  {
    // RB_TREE t = RB_TREE();
    cout << __cplusplus;
    return 0;
}