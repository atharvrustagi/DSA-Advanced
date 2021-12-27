#include<iostream>
#include "RB_TREE.H"
#include<set>
#include<vector>
using namespace std;

bool verify(rb_tree& tree, set<int> &s)   {
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
    int ar[n];
    tree.inorder(ar);
    int i = 0;
    // cout << endl;
    for (int e: s) {
        // cout << ar[i] << " ";
        if (ar[i++] != e) {
            cout << "\nInorder traversal not correct!";
            r = 0;
            break;
        }
    }
    return r;
}

bool test1(int t_no)    {
    // constructor and destructor test 1
    printf("Running test %d: ", t_no);
    rb_tree t;
    return 1;
}

bool test2(int t_no)    {
    // constructor and destructor test 2
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.clear();
    return 1;
}

bool test3(int t_no)    {
    // insertion test 1
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.clear();
    t.insert(5);
    return 1;
}

bool test4(int t_no)    {
    // insertion and deletion test
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.insert(5);
    t.erase(5);
    return 1;
}

bool test5(int t_no)    {
    // insertion and deletion test
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.insert(5);
    t.erase(5);
    t.clear();
    return 1;
}

bool test6(int t_no)    {
    // deletion test 
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.erase(5);
    return 1;
}

bool test7(int t_no)    {
    // heavy insertion and deletion test
    printf("Running test %d: ", t_no);
    vector<int> test;
    for (int i=0; i<10000; ++i)
        test.push_back(rand() % rand());
    // new set
    set<int> s;
    // new rb_tree
    rb_tree t;

    int n = test.size();

    // insert all elements and check the traversal for each iteration
    for (int v: test) {
        t.insert(v);
        s.insert(v);
        // check traversal
        if (!verify(t, s))
            return 0;
    }

    // delete half the elements and check the traversal for each iteration
    for (int j=0; j<n; j+=2) {
        t.erase(test[j]);
        s.erase(test[j]);
        // check traversal
        if (!verify(t, s))
            return 0;
    }
    return 1;
}

void run_tests()    {
    typedef bool (*BoolTests) (int t);
    BoolTests tests[] = {test1, test2, test3, test4, test5, test6, test7};
    int n = sizeof(tests) / sizeof(tests[0]);
    for (int i=0; i<n; ++i) {
        cout << (tests[i](i+1) ? "Passed\n" : "Failed");
    }
}

int main()  {
    run_tests();
    return 0;
}
