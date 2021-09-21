#include<iostream>
#include<utility>   // for std::pair
#include<queue>     // for level order traversal
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

    // returns a node address, and if it needs to be rotated or not
    pair<Node*, bool> _insert(Node* root, int &key, bool is_left_child=0)    {
        // find appropriate place for the key
        if (key < root->val)    {
            if (root->left) {
                auto [new_root, rotate] = _insert(root->left, key, 1);
                root->left = new_root;
                if (rotate) {
                    // as it came from the left child, a right rotation must be done
                    root = rotate_right(root);
                    // change colors too
                    // now root must be red and left child must be red as well
                    // make left child black too, as is the right
                    root->left->red = 0;
                }
            }
            else    // make new node
                root->left = new Node(key, 1);
        }
        else if (key > root->val)   {
            if (root->right)    {
                auto [new_root, rotate] = _insert(root->right, key, 0);
                root->right = new_root;
                if (rotate) {
                    root = rotate_left(root);
                    root->right->red = 0;
                }
            }
            else
                root->right = new Node(key, 1);
        }
        else    // key already exists
            return {root, 0};

        // check for problems
        
        // if both children exist and are red, then this node must be black
        if (root->left && root->right && root->left->red && root->right->red) {
            // change the children and parent colors
            root->red = 1;
            root->left->red = 0;
            root->right->red = 0;
            return {root, 0};
        }

        // now, if root is black, then no problem
        if (!root->red)
            return {root, 0};
        
        // check for double red problem in the left child
        if (root->left && root->left->red)  {
            // if root is a left child, then no internal rotation required
            if (is_left_child)
                return {root, 1};
            // otherwise, internal rotation to the right
            return {rotate_right(root), 1};
        }
        // check for double red in the right child
        else if (root->right && root->right->red)   {
            if (!is_left_child)
                return {root, 1};
            return {rotate_left(root), 1};
        }
        // no rotations required
        return {root, 0};
    }

    // performs right rotation on given root, and returns new root
    Node* rotate_right(Node *root)   {
        Node *left_child = root->left;
        Node *cright = left_child->right;
        root->left = cright;
        left_child->right = root;
        return left_child;
    }

    // performs left rotation on given root, and returns new root
    Node* rotate_left(Node *root)   {
        Node* right_child = root->right;
        Node *cleft = right_child->left;
        root->right = cleft;
        right_child->left = root;
        return right_child;
    }

    bool _find(int &key, Node* root)  {
        if (!root)
            return 0;
        if (root->val == key)
            return 1;
        if (key < root->val)
            return _find(key, root->left);
        return _find(key, root->right);
    }

    int get_black_height(Node* root)  {
        if (!root)
            return 0;
        return get_black_height(root->left) + !root->red;
    }
    
    bool _check_height(Node* root, int &h, int height)    {
        if (!root)  {
            return height == h;
        }
        return _check_height(root->left, h, height + !root->red) &&
               _check_height(root->right, h, height + !root->red);
    }

    bool _check_red(Node* root) {
        if (!root)
            return 1;
        if (root->red)  {
            if (root->left && root->left->red)
                return 0;
            if (root->right && root->right->red)
                return 0;
        }
        return _check_red(root->left) && _check_red(root->right);
    }

    void _inorder(Node* root)   {
        if (!root)
            return;
        _inorder(root->left);
        cout << root->val << " ";
        _inorder(root->right);
    }

public:
    RB_TREE() : root(nullptr)  {}

    // Insert a key method
    void insert(int key)    {
        if (!root)  {
            root = new Node(key, 0);
            return;
        }

        auto [node, rotate] = _insert(root, key);
        root = node;
        if (root->red)
            root->red = 0;
    }

    bool find(int key)  {
        return _find(key, root);
    }

    bool check_red()    {
        return _check_red(root);
    }

    int check_height()    {
        int h = get_black_height(root);
        bool b = _check_height(root, h, 0);
        if (b)
            return h;
        else
            return -1;
    }

    void level_order()  {
        cout << endl;
        queue<Node*> q;
        q.push(root);
        q.push(nullptr);
        while (q.size() > 1)    {
            Node *node = q.front();
            q.pop();
            if (!node)  {
                cout << endl;
                q.push(nullptr);
                continue;
            }
            if (node->red)
                printf("%d(red, {%d, %d}) ", node->val, (node->left ? node->left->val : -1), (node->right ? node->right->val : -1));
            else
                printf("%d(blk, {%d, %d}) ", node->val, (node->left ? node->left->val : -1), (node->right ? node->right->val : -1));
            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }
    }

    void inorder()  {
        cout << endl;
        cout << "Inorder traversal: ";
        _inorder(root);
    }
};

int main()  {
    RB_TREE t = RB_TREE();
    int n;
    cout << "Enter number of insertions to be done: ";
    cin >> n;
    for (int i=0; i<n; ++i)    {
        int v;
        v = rand() % int(1e9+7);
        cout << v << " ";
        // cin >> v;
        t.insert(v);
    }
    t.level_order();
    int h = t.check_height();
    if (h != -1)
        cout << "\nBlack height maintained as: " << h;
    else
        cout << "\nBlack height violated";
    if (t.check_red())
        cout << "\nRed-Black property maintained";
    else
        cout << "\nRed-Black property violated";
    t.inorder();
    return 0;
}
