#include<iostream>
#include<cstring>
using namespace std;

// Node of a B-Tree
struct Node {
    int *ar;        // pointer for the integer array
    int n1, n2;          // current size of the arrays
    Node **child;   // child array, similar to Node *child[]
    // initialise values
    Node(int order) : ar(new int[order]),  n1(0), n2(0), child(new Node*[order+1]) {
        memset(ar, 0, order * 4);
        memset(child, NULL, order * 4);
    }
};

// B-Tree implementation
class B_Tree    {
    Node *root;
    int N1, N2, order;  // order of the tree
public:
    // default order = 5
    B_Tree()    {
        order = 5;
        N1 = order-1;
        N2 = order;
        root = new Node(order);
    }
    // initialise the tree
    B_Tree(int order)    {
        N1 = order-1;
        N2 = order;
        this->order = order;
        root = new Node(order);
    }

    // inserts a number into the B-Tree
    void insert(int num)    {
        _insert(num, root);
    }

private:
    // void _insert(int num, Node *root)  {
    //     // if this node has no children, insertion will be done here
    //     if (root->n2 == 0)  {
    //         // if this node can accomodate this number, insert it
    //         if (root->n1 < N1)    {
    //             int idx = lower_bound(num, root);
    //             insertion_sort(num, root, idx);
    //             return;
    //         }
    //         else    {   // cannot accomodate this number
    //             // find at what position it will be inserted
    //             int idx = lower_bound(num, root);
    //         }
    //     }
    //     // otherwise
    // }

    Node* _insert(int num, Node *root)  {
        // locate key
        int idx = upper_bound(num, root);
        if (idx>0 && root->ar[idx-1] == num)  {  // element found
            return nullptr; // finished
        }
        // check if child node exists
        if (idx < root->n2)   {
            Node *res = _insert(num, root->child[idx]);
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
                int mid = root->ar[N1/2];
                // create new node, and copy contents
                Node *right_node = new Node(order);
                for (int i=N1/2+1, j=0; i<N1; ++j, ++i)   {
                    right_node->ar[j] = root->ar[i];
                }

            }
        }
        

        // otherwise
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
};

int main()  {
    int order = 5;
    Node *node = new Node(order);
    for (int i=0; i<order-1; ++i)
        cout << node->ar[i] << " ";
    cout << endl;
    for (int i=0; i<order; ++i)
        cout << node->child[i] << " ";
    
    B_Tree root = B_Tree();
    return 0;
}
