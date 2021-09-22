#include<iostream>
#include<queue>
#include<SFML/Graphics.hpp>
#define ll long long
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
    int tree_size;

    // returns a node address, and if it needs to be rotated or not
    ll _insert(Node* root, int &key, bool is_left_child=0)    {
        // find appropriate place for the key
        if (key < root->val)    {
            if (root->left) {
                ll address = _insert(root->left, key, 1);
                Node *new_root = (Node*)abs(address);
                bool rotate = address<0;
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
            else  {  // make new node
                root->left = new Node(key, 1);
                ++tree_size;
            }
        }
        else if (key > root->val)   {
            if (root->right)    {
                ll address = _insert(root->right, key, 0);
                Node *new_root = (Node*)abs(address);
                bool rotate = address<0;
                root->right = new_root;
                if (rotate) {
                    root = rotate_left(root);
                    root->right->red = 0;
                }
            }
            else    {
                root->right = new Node(key, 1);
                ++tree_size;
            }
        }
        else    // key already exists
            return (ll)root;

        // check for problems

        // if both children exist and are red, then this node must be black
        if (root->left && root->right && root->left->red && root->right->red) {
            // change the children and parent colors
            root->red = 1;
            root->left->red = 0;
            root->right->red = 0;
            return (ll)root;
        }
 
        // now, if root is black, then no problem
        if (!root->red)
            return (ll)root;
        
        // check for double red problem in the left child
        if (root->left && root->left->red)  {
            // if root is a left child, then no internal rotation required
            if (is_left_child)
                return -(ll)root;
            // otherwise, internal rotation to the right
            return -(ll)rotate_right(root);
        }
        // check for double red in the right child
        else if (root->right && root->right->red)   {
            if (!is_left_child)
                return -(ll)root;
            return -(ll)rotate_left(root);
        }
        // no rotations required
        return (ll)root;
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

    Node* _find(int &key, Node* root)  {
        if (!root)
            return 0;
        if (root->val == key)
            return root;
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

    ll _erase(int &key, Node* root) {
        // locate the key
        if (key < root->val)    {
            _erase(key, root->left);
        }
        else if (key > root->val)   {
            _erase(key, root->right);
        }
        // this is the key
        else    {
            ;
        }

        // if this is a leaf node
        if (!root->left && !root->right)    {
            // delete this node simply
            delete root;
            // best case, red node with no children
            if (root->red)
                return 0;

            // black node
            // indicate to parent that there is double black problem
            return -1;
        }

        // root has at least 1 child
        // if root has only 1 child and it is red, replace the node with child
        if (root->left && !root->right && root->left->red)  {
            root->val = root->left->val;
            delete root->left;
            root->left = nullptr;
            return 0;
        }
        if (!root->left && root->right && root->right->red) {
            root->val = root->right->val;
            delete root->right;
            root->right = nullptr;
            return 0;
        }

        // root has at least 1 black child


    }

public:
    RB_TREE() : root(nullptr), tree_size(0)  {}

    int size()  {
        return tree_size;
    }

    // Inserts a key
    void insert(int key)    {
        if (!root)  {
            root = new Node(key, 0);
            ++tree_size;
            return;
        }

        ll address = _insert(root, key);
        root = (Node*)address;
        if (root->red)
            root->red = 0;
    }

    void erase(int key) {
        // find if key exists
        Node* node = _find(key, root);
        if (!node)
            return;
        // find the inorder predecessor or successor
        // we'll find the predecessor
        if (node->left) {
            node = node->left;
            while (node->right)
                node = node->right;
        }
        int predecessor = node->val;
        _erase(predecessor, root);
        // replace node if it still exists
        if (node)
            node->val = predecessor;
    }

    // Returns true if key is found in the tree, else false
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

    Node* GET_ROOT()    {
        return root;
    }

    void inorder()  {
        cout << endl;
        cout << "Inorder traversal: ";
        _inorder(root);
    }
};

void randomize(int n, int *ar)  {
    for (int i=0; i<n; ++i)
        ar[i] = i+1;
    while (n > 0)  {
        // choose a random index
        int idx = rand() % n;
        // swap
        int tmp = ar[n-1];
        ar[n-1] = ar[idx];
        ar[idx] = tmp;
        --n;
    }
}

float dimX = 1000, dimY = 1000;
float nodeSize = 50;
sf::Font font;
sf::Text text;
sf::RectangleShape square(sf::Vector2f(nodeSize, nodeSize));
auto red = sf::Color(255, 0, 0),
     blk = sf::Color(0, 0, 0);

void init() {
    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setCharacterSize(nodeSize/2.5);
    text.setFillColor(sf::Color::White);
    square.setOrigin(nodeSize/2, nodeSize/2);
}

void draw_line(float x1, float y1, float x2, float y2, sf::RenderWindow &win)   {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(x1, y1), sf::Color::Black),
        sf::Vertex(sf::Vector2f(x2, y2), sf::Color::Black)
    };
    win.draw(line, 2, sf::Lines);
}

void plot_node(const Node* node, float x, float y, sf::RenderWindow &win)  {
    if (node && node->red)
        square.setFillColor(red);
    else
        square.setFillColor(blk);
    square.setPosition(x, y);
    win.draw(square);
    text.setString((node ? to_string(node->val) : "null"));
    sf::FloatRect f;
    f = text.getLocalBounds();
    text.setOrigin((f.top+f.height)/2, (f.left+f.width)/2);
    text.setPosition(x, y);
    win.draw(text);
}

// current node, current pos
void make_tree(const Node* root, float x, float y, float gap, sf::RenderWindow &win)  {
    if (!root)
        return;
    // draw lines first
    if (root->left)  {
        draw_line(x, y, x - gap/2, y+100, win);
        make_tree(root->left, x - gap/2, y+100, gap/2, win);
    }
    if (root->right)    {
        draw_line(x, y, x + gap/2, y+100, win);
        make_tree(root->right, x + gap/2, y+100, gap/2, win);
    }
    plot_node(root, x, y, win);
}

void save_tree(const Node* root, sf::RenderWindow &win)    {
    static int img_no = 1;
    win.clear(sf::Color::White);
    // traverse and make the image, inorder
    draw_line(dimX/2, 50, dimX/2 - dimX/4, 50+100, win);
    draw_line(dimX/2, 50, dimX/2 + dimX/4, 50+100, win);
    plot_node(root, dimX/2, 50, win);
    make_tree(root->left, dimX/2 - dimX/4, 50+100, dimX/4, win);
    make_tree(root->right, dimX/2 + dimX/4, 50+100, dimX/4, win);
    sf::Image img = win.capture();
    string name = "imgs/tree" + to_string(img_no++);
    name += ".png";
    img.saveToFile(name);
}

int main()  {
    RB_TREE* tree = new RB_TREE();
    int n;
    cout << "Number of elements: ";
    cin >> n;
    int ar[n];
    bool random = 0;
    cout << "Random? (0/1 only): ";
    cin >> random;
    cout << "\nOrder of insertion: ";
    sf::RenderWindow win(sf::VideoMode(dimX, dimY), "Tree");
    init();
    if (random)
        randomize(n, ar);
    for (int i=0; i<n; ++i) {
        if (random)
            cout << ar[i] << " ";
        else    {
            cin >> ar[i];
            // ar[i] = i+1;
        }
        tree->insert(ar[i]);
        save_tree(tree->GET_ROOT(), win);
    }
    tree->inorder();
    tree->level_order();
    win.display();

    while (win.isOpen())    {
        sf::Event event;
        while (win.pollEvent(event))
            if (event.type == sf::Event::Closed)
                win.close();
    }
    return 0;
}
