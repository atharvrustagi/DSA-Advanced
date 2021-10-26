#include<bits/stdc++.h>
using namespace std;

struct Node {
    Node **next;
    int val, n;
    Node(int v)  {
        n = 0;
        val = v;
        next = new Node*[10];
        memset(next, 0, 10 * sizeof(next));
    }
    Node(int v, int s)  {
        n = 0;
        val = v;
        next = new Node*[s];
        memset(next, 0, s * sizeof(next));
    }
};

class skip_list {
    Node *head;
    int size;

    void _insert(int key, Node *cur, Node **ar, int n) {
        if (cur->val == key)
            return;
        // check current node's next pointers
        while (n > -1)  {
            // if next is bigger, go down
            if (cur->next[n]->val > key)    {
                ar[n--] = cur;
            }
            else    {
                // go right and then return
                _insert(key, cur->next[n], ar, n);
                return;
            }
        }
        // if n is -1, insert here
        Node *node = new Node(key, size);
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

    bool _find(int key, Node *head, int n) {
        if (head->val == key)
            return 1;
        while (n > -1)  {
            if (head->next[n]->val <= key)
                return _find(key, head->next[n], n);
            --n;
        }
        return 0;
    }

public:
    skip_list() {
        size = 10;
        head = new Node(INT_MIN, size);
        Node *tail = new Node(INT_MAX, size);
        for (int i=0; i<size; ++i)
            head->next[i] = tail;
        head->n = size;
        tail->n = size;
    }
    skip_list(int size) {
        this->size = size;
        head = new Node(INT_MIN, size);
        Node *tail = new Node(INT_MAX, size);
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

    // finds a key
    bool find(int key)  {
        return _find(key, head, size-1);
    }

    void show()    {
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
    unordered_set<int> us;
    for (int i=0; i<n; ++i) {
        int r = (rand() * rand()) % (int)1e9;
        us.insert(r);
        s.insert(r);
    }
    // s.print_list();
    // s.show();
    for (int i=0; i<1000; ++i)  {
        if (s.find(i) != us.count(i))
            cout << "\nFail " << i;
    }
    cout << "done";
    return 0;
}
