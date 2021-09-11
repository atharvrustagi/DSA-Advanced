#include<iostream>
#include<cstring>
#include<queue>
using namespace std;

// Node of a B-Tree
struct Node {
    int *ar;        // pointer for the integer array
    int n1;          // current size of the arrays
    Node **child;   // child array, similar to Node *child[]
    bool has_children;
    // initialise values
    Node(int order) : ar(new int[order]), child(new Node*[order+1]) {
        has_children = 0;
        n1 = 0;
        memset(ar, 0, order * 4);
        memset(child, NULL, order * 4);
    }
};

// B-Tree implementation
class B_Tree    {
    Node *root;
    int N1;     // maximum size of array
    int order;  // order of the tree
    int N12;        // (N1 + 1) / 2
public:
    // default order = 5
    B_Tree()    {
        order = 5;
        N1 = order-1;
        N12 = (N1 + 1) / 2;
        root = new Node(order);
    }
    // initialise the tree
    B_Tree(int order)    {
        N1 = order-1;
        N12 = (N1 + 1) / 2;
        this->order = order;
        root = new Node(order);
    }

    // inserts a number into the B-Tree
    void insert(int num)    {
        Node *root = this->root;
        Node *res = _insert(num, root);
        if (res)    {       // if it is not nullptr, that means we gotta modify the root
            // create a new parent node
            Node *parent = new Node(order);
            // this node will have children
            parent->has_children = 1;
            // add value to array
            parent->ar[0] = res->ar[N1];
            parent->n1 = 1;     // contains 1 element yet
            // link children
            // 1st child => root, 2nd child => res
            parent->child[0] = root;
            parent->child[1] = res;
            this->root = parent;            
        }
    }

    // prints the level order traversal of the B-Tree
    void level_order()  {
        Node *node = this->root;
        queue<Node*> q;
        q.push(node);
        q.push(nullptr);
        while (q.size()>1)  {
            node = q.front();
            q.pop();
            if (!node)  {
                q.push(nullptr);
                cout << endl;
                continue;
            }
            cout << "(";
            for (int i=0; i<node->n1-1; ++i)
                printf("%d, ", node->ar[i]);
            printf("%d) ", node->ar[node->n1-1]);
            if (node->has_children) {
                for (int i=0; i<=node->n1; ++i)
                    q.push(node->child[i]);
            }
        }
    }

private:
    Node* _insert(int num, Node *root)  {
        // locate key
        int idx = upper_bound(num, root);
        if (idx>0 && root->ar[idx-1] == num)  {  // element found
            return nullptr; // finished
        }
        // check if child node exists
        if (root->has_children)   {
            Node *res = _insert(num, root->child[idx]);
            if (!res)
                return res;     // finished
            // child returned a node, need to modify
            // insertion to be done at idx
            insertion_sort(res->ar[N1], root, idx);
            // child also needs to be linked, and others shifted
            insertion_sort_nodes(res, root, idx+1);
            // check that node is not full
            if (root->n1 <= N1)
                return nullptr;
            // node must be split
            int mid = root->ar[N12];
            // create new node
            Node *right_node = new Node(order);
            // this root has children, so sibling will also have children
            right_node->has_children = 1;
            // copy data to new node
            int i, j;
            for (i=N12+1, j=0; i<=N1; ++j, ++i)   {
                right_node->ar[j] = root->ar[i];
                right_node->child[j] = root->child[i];
            }
            right_node->child[j] = root->child[i];
            // change size
            right_node->n1 = j;
            root->n1 = N12;
            // middle element as the last element of right node
            right_node->ar[N1] = mid;
            return right_node;
        }
        // else try to insert here
        else    {
            if (root->n1 < N1)  {   // can insert in this node
                insertion_sort(num, root, idx);
                return nullptr; // finished
            }
            else    {   // node is full, split and do stuff
                // num is to be inserted at idx
                // find middle element after insertion
                insertion_sort(num, root, idx);
                // this element will become parent
                int mid = root->ar[N12];
                // create new node, and copy contents
                Node *right_node = new Node(order);
                int i, j;
                for (i=N12+1, j=0; i<=N1; ++j, ++i)   {
                    right_node->ar[j] = root->ar[i];
                }
                right_node->n1 = j;
                root->n1 = N12;
                // make the middle element, as the last element of the new node
                right_node->ar[N1] = mid;
                return right_node;                
            }
        }
    }

    int upper_bound(int num, Node *root)    {
        int low = 0, high = root->n1;
        while (low < high)  {
            int mid = (low + high) / 2;
            if (root->ar[mid] <= num)
                low = mid+1;
            else
                high = mid;
        }
        return low;
    }

    void insertion_sort(int num, Node *root, int idx)   {
        // shift all elements to the right starting from idx included
        for (int i=root->n1++; i>idx; --i)
            root->ar[i] = root->ar[i-1];
        root->ar[idx] = num;
    }

    void insertion_sort_nodes(Node *node, Node *root, int idx)  {
        // shift all elements to the right starting from idx included
        for (int i=root->n1+1; i>idx; --i)
            root->child[i] = root->child[i-1];
        root->child[idx] = node;
    }
};

int main()  {
    int order = 5;
    int elements[] = {15,30,45,1,3,7,90,105,34,37,42,48,54,69,60,74,100,84,89,9,22,24};
    int n = sizeof(elements)/4;
    // cin >> n;
    B_Tree set = B_Tree(order);
    for (int i=0; i<n; ++i)
        set.insert(elements[i]);
    set.level_order();
    return 0;
}
