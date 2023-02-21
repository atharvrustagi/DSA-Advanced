#include <iostream>
#include <vector>
using namespace std;

/*
Persistent Segment Tree
store every change at every iteration that we make
at every iteration, at most log(n) changes are made
so we will store only those log(n) changes
therefore the space complexity remains O(nlog(n))

Example question -> Median Queries (Hackerearth)
*/

struct Node {
    int val;
    Node *left, *right;
    Node(int v = 0): left(nullptr), right(nullptr), val(v) {}
    Node(Node *node): left(node->left), right(node->right), val(node->val) {}
};

class PST {
    const int N;
    int index, value, left, right;
    // store all root versions
    vector<Node*> roots;

    void init_build(Node *cur, int low, int high) {
        if (low == high) return;
        int mid = (low + high) >> 1;
        init_build(cur->left = new Node(), low, mid);
        init_build(cur->right = new Node(), mid+1, high);
        cur->val = cur->left->val + cur->right->val;
    }

    void build_version(Node *cur, Node *prev, int low, int high) {
        if (low == high) {
            cur->val += value;
            return;
        }
        int mid = (low + high) >> 1;
        if (index <= mid) {
            // need to create the left child, but the right child will be the same as previous version
            cur->left = new Node(prev->left);
            cur->right = prev->right;
            build_version(cur->left, prev->left, low, mid);
        }
        else {
            // need to create the right child, but the left child will be the same as previous version
            cur->right = new Node(prev->right);
            cur->left = prev->left;
            build_version(cur->right, prev->right, mid+1, high);
        }
        cur->val = cur->left->val + cur->right->val;
    }

    int _query(Node *cur, int low, int high) {
        if (high < left || low > right)
            return 0;
        if (left <= low && high <= right)
            return cur->val;
        int mid = (low + high) >> 1;
        return _query(cur->left, low, mid) + _query(cur->right, mid+1, high);
    }

public:
    PST(int n) : N(n) {
        // build the first (or the 0th) version
        Node *root = new Node();
        init_build(root, 0, n-1);
        roots.push_back(root);
    }

    // creates a new version
    // modifies the tree according to the version specified
    // only the changes made till prev_version will be there in the new version
    void update(int index, int val, int prev_version = -1) {
        Node *root = new Node();
        this->index = index, this->value = val;
        if (prev_version == -1)
            prev_version = get_latest_version();
        build_version(root, roots[prev_version], 0, N-1);
        roots.push_back(root);
    }

    int get_latest_version() {
        return roots.size()-1;
    }

    // returns the query for a given range for a specific version
    // if version is not set, returns the query for the latest version
    int query(int left, int right, int version = -1) {
        if (version == -1)
            version = get_latest_version();
        this->left = left, this->right = right;
        return _query(roots[version], 0, N-1);
    }
};

int main() {
    return 0;
}