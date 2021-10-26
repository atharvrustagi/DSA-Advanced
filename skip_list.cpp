#include<bits/stdc++.h>
using namespace std;

struct Node {
    Node **next;
    int val, n;
    Node(int v)  {
        n = 0;
        val = v;
        next = new Node*[10];
    }
    Node(int v, int s)  {
        n = 0;
        val = v;
        next = new Node*[s];
    }
};

Node *newnode(int v, int s) {
    Node *node = new Node(v, s);
    for (int i=0; i<s; ++i)
        node->next[i] = 0;
    return node;
}

class skip_list {
    Node *head;
    int size;

    void _insert(int &key, Node *cur, Node **ar, int n) {
        if (cur->val == key)
            return;
        // check current node's next pointers
        while (n > -1)  {
            // if next is bigger, go down
            if (cur->next[n]->val > key)
                ar[n--] = cur;
            else    {
                // go right and then return
                _insert(key, cur->next[n], ar, n);
                return;
            }
        }
        // if n is -1, insert here
        Node *node = newnode(key, size);
        // update lists
        do  {
            if (++n == size)
                break;
            Node *front = ar[n]->next[n];
            ar[n]->next[n] = node;
            node->next[n] = front;
            ++node->n;
        } while (rand() & 1);
    }

public:
    skip_list() {
        size = 10;
        head = newnode(INT_MIN, size);
        Node *tail = newnode(INT_MAX, size);
        for (int i=0; i<size; ++i)
            head->next[i] = tail;
        head->n = size;
        tail->n = size;
    }
    skip_list(int size) {
        this->size = size;
        head = newnode(INT_MIN, size);
        Node *tail = newnode(INT_MAX, size);
        for (int i=0; i<size; ++i)
            head->next[i] = tail;
        head->n = size;
        tail->n = size;
    }

    // insert a key
    void insert(int key)    {
        // keep a running array of pointers to assign later
        Node *ar[size] = {0};
        _insert(key, head, ar, size-1);
    }

    void print()    {
        Node *node = head->next[0];
        while (node->next[0])    {
            for (int i=0; i<node->n; ++i)
                cout << node->val << " ";
            cout << endl;
            node = node->next[0];
        }
    }

    void print_list()   {
        Node *node = head->next[0];
        while (node->next[0])   {
            cout << node->val << " ";
            node = node->next[0];
        }
        cout << endl;
    }

    void level_order()  {
        for (int i=size-1; i>=0; --i)  {
            Node *node = head->next[i];
            while (node)    {
                cout << node->val << " ";
                node = node->next[i];
            }
            cout << endl;
        }
    }
};


int main()  {
    int n;
    cin >> n;
    skip_list s;
    for (int i=0; i<n; ++i)
        s.insert(rand() % 1000);
    s.print();
}
