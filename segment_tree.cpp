#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <math.h>
#include <cstring>
using namespace std;

int seg_tree[1 << 21], n;
class Solution {
    void print_seg_tree() {
        int p = 2;
        for (int i = 0; i < 9; ++i) {
            if (i+1 == p) {
                p <<= 1;
                cout << '\n';
            }
            cout << seg_tree[i] << ' ';
        }
        cout << "\n\n";
    }

    int count_less(const int &num, int low = 0, int high = n-1, int idx = 0) {
        ++seg_tree[idx];
        if (low == high)
            return 0;
        int mid = (low + high) / 2, ret;
        if (num <= mid)
            return count_less(num, low, mid, idx*2 + 1);
        return seg_tree[idx*2+1] + count_less(num, mid+1, high, idx*2 + 2);
    }

public:	
	vector<int> constructLowerArray(int *ar, int n) {
        ::n = n;
	    // mapping numbers to smaller values
        map<int, int> mp;
        for (int i = 0; i < n; ++i)
            mp[ar[i]] = 0;
        
        int idx = 0;
        for (auto &p: mp)
            p.second = idx++;

        for (int i = 0; i < n; ++i)
            ar[i] = mp[ar[i]];

        // segment seg_tree
        int p = ceil(log2(n)+1);
        vector<int> ans;
        memset(seg_tree, 0, 1 << p+2);
        // printf("size = %d\n", 1<<p);
        // print_seg_tree();

        for (int i = n-1; i >= 0; --i) {
            // count less than current and add to seg_tree
            ans.push_back(count_less(ar[i]));
        }
        reverse(ans.begin(), ans.end());
        return ans;
	}
};

// lazy segment tree
// leetcode 2569
class SegmentTree {
    vector<int> tree;
    vector<bool> isFlipped;
    int left, right;
    const int N;

    int get_size() {
        int s = 1;
        while (s < N)
            s <<= 1;
        return s << 1;
    }

    void _flip(int idx, int low, int high) {
        // if current node is lazily flipped, flip it properly
        flipProper(idx, low, high);
        if (low > right || high < left) // out of range
            return;
        if (left <= low && high <= right) { // completely in range
            // flip this range
            isFlipped[idx] = true;
            flipProper(idx, low, high);
        }

        int mid = (low + high) >> 1, idx2 = idx << 1;
        _flip(idx2+1, low, mid);
        _flip(idx2+2, mid+1, high);
        tree[idx] = tree[idx2+1] + tree[idx2+2];        
    }
    
    void flipProper(int idx, int low, int high) {
        if (!isFlipped[idx])
            return;
        
        // if not leaf node, indicate to children that they need to be flipped later (lazy flip children)
        if (low != high) {
            isFlipped[idx*2+1] = !isFlipped[idx*2+1];
            isFlipped[idx*2+2] = !isFlipped[idx*2+2];
        }
        tree[idx] = (high-low+1) - tree[idx];
        // current node is now properly flipped, as we changed the contents
        // so it acts like a non flipped node only
        isFlipped[idx] = false;
    }

    int rangeSum(int idx, int low, int high) {
        // if current node is lazily flipped, flip properly
        flipProper(idx, low, high);
        if (low > right || high < left) // out of range
            return 0;
        if (left <= low && high <= right) // completely in range
            return tree[idx];

        int mid = (low + high) >> 1, idx2 = idx << 1;
        return rangeSum(idx2+1, low, mid) + rangeSum(idx2+2, mid+1, high);
    }

public:
    SegmentTree(int n) : N(n) {
        tree.assign(get_size(), 0);
        isFlipped.assign(get_size(), false);
    }

    void flipRange(int left, int right) {
        this->left = left;
        this->right = right;
        _flip(0, 0, N-1);
    }

    int getRangeSum(int left, int right) {
        this->left = left;
        this->right = right;
        return rangeSum(0, 0, N-1);
    }
};

int main() {
    cin >> n;
    int ar[n] = {2,5,1,8,4};
    for (int i = 0; i < n; ++i)
        cin >> ar[i];
    Solution sol;
    vector<int> v = sol.constructLowerArray(ar, n);
    for (int x: v)
        cout << x << ' ';
    return 0;
}
