#include<iostream>
#include<set>
#include "RED_BLACK_TREE.h"
#include<chrono>
using namespace std;

int *ar;

bool verify(RB_TREE& tree, set<int> &s)   {
    bool r = 1;
    int h = tree.check_height();
    if (h == -1)    {
        r = 0;
        cout << "\nHeight not maintained!";
    }
    if (!tree.check_red())  {
        r = 0;
        cout << "\nDouble red problem!";
    }
    int n = tree.size();
    tree.inorder(ar);
    int i = 0;
    for (auto e: s) {
        if (ar[i++] != e) {
            cout << "\nInorder traversal not correct!";
            r = 0;
            break;
        }
    }
    return r;
}

void randomize(int n)  {
    while (n > 0)  {
        // choose a random index
        int idx = rand() % n;
        // swap
        swap(ar[idx], ar[--n]);
    }
}

int main()  {
    int n;
    bool eval_set = 1;
    n = 1000000;
    ar = new int[n];
    // cout << "Enter the number of elements: ";
    // cin >> n;
    // cout << "Evaluate tree or set? (1 for set, 0 for tree): ";
    // cin >> eval_set;

    RB_TREE tree;
    set<int> s;
    for (int i=0; i<n; ++i) {
        int r = (rand() * rand()) % (int)(1e9+7);
        // cout << r << " ";
        ar[i] = r;
        if (eval_set) s.insert(r);
        else          tree.insert(r);
    }

    // start timer
    auto start = chrono::high_resolution_clock::now();
    // start code
    if (eval_set)  cout << "\nSet size after insertion is " << s.size();
    else           cout << "\nTreeSet size after insertion is " << tree.size();
    // shuffle ar
    randomize(n);
    // remove half the elements randomly
    for (int i=0, n2=n/2; i<n2; ++i) {
        int r = ar[i];
        if (eval_set) s.erase(r);
        else          tree.erase(r);
    }
    if (eval_set)  cout << "\nSet size after deletion is " << s.size();
    else           cout << "\nTreeSet size after deletion is " << tree.size();

    if (eval_set)  s.clear();
    else           tree.clear();

    // end code
    auto stop = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    printf("\nTime taken for operations: %llf seconds", duration / 1e6);
    return 0;
}
