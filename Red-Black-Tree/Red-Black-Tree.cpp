#include <fstream>
#include <vector>
#include <cstdint>
#include <queue>
using namespace std;

struct Node
{
    int32_t val;
    bool red;
    Node *parent;
    Node *left_child;
    Node *right_child;
    Node() : val(0), red(false), parent(nullptr), left_child(nullptr), right_child(nullptr) {}
    Node(int32_t data) : val(data), red(true), parent(nullptr), left_child(nullptr), right_child(nullptr) {}
};

struct RBT
{
    Node *root;
    Node *nil;

    RBT()
    {
        nil = new Node;
        nil->red = false;
        nil->parent = nil;
        nil->left_child = nil;
        nil->right_child = nil;
        root = nil;
    }
    ~RBT()
    {
        deletree(root);
        delete nil;
    }

    // rotate
    void left_rotate(Node *x)
    {
        if (x == nil || x->right_child == nil)
            return;

        Node *y = x->right_child;
        x->right_child = y->left_child;
        if (y->left_child != nil)
            y->left_child->parent = x;
        y->parent = x->parent;

        if (x == root)
            root = y;
        else if (x == x->parent->left_child)
            x->parent->left_child = y;
        else
            x->parent->right_child = y;

        y->left_child = x;
        x->parent = y;
    }
    void right_rotate(Node *x)
    {
        if (x == nil || x->left_child == nil)
            return;

        Node *y = x->left_child;
        x->left_child = y->right_child;
        if (y->right_child != nil)
            y->right_child->parent = x;
        y->parent = x->parent;

        if (x == root)
            root = y;
        else if (x == x->parent->right_child)
            x->parent->right_child = y;
        else
            x->parent->left_child = y;

        y->right_child = x;
        x->parent = y;
    }

    // tools
    Node *find(int32_t key)
    {
        Node *current = root;
        while (current != nil)
        {
            if (key == current->val)
                return current;
            else if (key < current->val)
                current = current->left_child;
            else
                current = current->right_child;
        }
        return current;
    }
    void print(ostream &out)
    {
        if (!out)
            return;

        queue<Node *> waitlist;
        waitlist.push(root);
        Node *current;
        while (!waitlist.empty())
        {
            current = waitlist.front();
            waitlist.pop();
            if (current == nil)
                continue;

            waitlist.push(current->left_child);
            waitlist.push(current->right_child);

            out << "Node: " << current->val << ", Parent: ";
            if (current->parent == nil)
                out << "nil";
            else
                out << current->parent->val;
            out << ", Left: ";
            if (current->left_child == nil)
                out << "nil";
            else
                out << current->left_child->val;
            out << ", Right: ";
            if (current->right_child == nil)
                out << "nil";
            else
                out << current->right_child->val;
            out << ", Color: " << ((current->red) ? "Red" : "Black") << "\n";
        }
    }
    void transplant(Node *u, Node *v)
    {
        if (u->parent == nil)
            root = v;
        else if (u == u->parent->left_child)
            u->parent->left_child = v;
        else
            u->parent->right_child = v;

        v->parent = u->parent;
    }
    Node *mini(Node *key)
    {
        Node *tmp = key;
        while (tmp->left_child != nil)
            tmp = tmp->left_child;
        return tmp;
    }
    void deletree(Node *key)
    {
        if (key != nil)
        {
            deletree(key->left_child);
            deletree(key->right_child);
            delete (key);
        }
    }

    // insert
    void insert_fixup(Node *new_node)
    {
        while (new_node->parent->red)
        {
            if (new_node->parent == new_node->parent->parent->left_child)
            {
                Node *y = new_node->parent->parent->right_child;
                if (y->red)
                {
                    new_node->parent->red = false;
                    y->red = false;
                    new_node->parent->parent->red = true;
                    new_node = new_node->parent->parent;
                }
                else
                {
                    if (new_node == new_node->parent->right_child)
                    {
                        new_node = new_node->parent;
                        left_rotate(new_node);
                    }
                    new_node->parent->red = false;
                    new_node->parent->parent->red = true;
                    right_rotate(new_node->parent->parent);
                }
            }
            else
            {
                Node *y = new_node->parent->parent->left_child;
                if (y->red)
                {
                    new_node->parent->red = false;
                    y->red = false;
                    new_node->parent->parent->red = true;
                    new_node = new_node->parent->parent;
                }
                else
                {
                    if (new_node == new_node->parent->left_child)
                    {
                        new_node = new_node->parent;
                        right_rotate(new_node);
                    }
                    new_node->parent->red = false;
                    new_node->parent->parent->red = true;
                    left_rotate(new_node->parent->parent);
                }
            }
        }
        root->red = false;
    }
    void insert(Node *new_node)
    {
        Node *x = root;
        Node *y = nil;
        while (x != nil)
        {
            y = x;
            if (new_node->val < x->val)
                x = x->left_child;
            else
                x = x->right_child;
        }
        new_node->parent = y;

        if (y == nil)
            root = new_node;
        else if (new_node->val < y->val)
            y->left_child = new_node;
        else
            y->right_child = new_node;
        new_node->left_child = nil;
        new_node->right_child = nil;
        new_node->red = true;
        insert_fixup(new_node);
    }

    // delete
    void remove_fixup(Node *x)
    {
        while (x != root && !x->red)
        {
            if (x == x->parent->left_child)
            {
                Node *w = x->parent->right_child;
                if (w->red)
                {
                    w->red = false;
                    x->parent->red = true;
                    left_rotate(x->parent);
                    w = x->parent->right_child;
                }

                if (!w->left_child->red && !w->right_child->red)
                {
                    w->red = true;
                    x = x->parent;
                }
                else
                {
                    if (!w->right_child->red)
                    {
                        w->left_child->red = false;
                        w->red = true;
                        right_rotate(w);
                        w = x->parent->right_child;
                    }
                    w->red = x->parent->red;
                    x->parent->red = false;
                    w->right_child->red = false;
                    left_rotate(x->parent);
                    x = root;
                }
            }
            else
            {
                Node *w = x->parent->left_child;
                if (w->red)
                {
                    w->red = false;
                    x->parent->red = true;
                    right_rotate(x->parent);
                    w = x->parent->left_child;
                }

                if (!w->left_child->red && !w->right_child->red)
                {
                    w->red = true;
                    x = x->parent;
                }
                else
                {
                    if (!w->left_child->red)
                    {
                        w->right_child->red = false;
                        w->red = true;
                        left_rotate(w);
                        w = x->parent->left_child;
                    }
                    w->red = x->parent->red;
                    x->parent->red = false;
                    w->left_child->red = false;
                    right_rotate(x->parent);
                    x = root;
                }
            }
        }
        x->red = false;
    }
    void remove(Node *key)
    {
        Node *x;
        Node *y = key;
        bool y_red = y->red;

        if (key->left_child == nil)
        {
            x = key->right_child;
            transplant(key, x);
        }
        else if (key->right_child == nil)
        {
            x = key->left_child;
            transplant(key, x);
        }
        else
        {
            y = mini(key->right_child);
            y_red = y->red;
            x = y->right_child;
            if (y->parent == key)
                x->parent = y;
            else
            {
                transplant(y, y->right_child);
                y->right_child = key->right_child;
                y->right_child->parent = y;
            }

            transplant(key, y);
            y->left_child = key->left_child;
            y->left_child->parent = y;
            y->red = key->red;
        }
        delete (key);

        if (!y_red)
            remove_fixup(x);
    }
};

int main()
{
    ifstream infile("input.txt");
    ofstream outfile("output.txt");

    RBT tree;
    int t;
    infile >> t;
    for (int k = 0; k < t; ++k)
    {
        int option, n;
        infile >> option >> n;
        // 1: inserting elements
        // 2: deleting elements
        // n: elements to be operated

        vector<int32_t> input(n);
        for (int j = 0; j < n; ++j)
            infile >> input[j];

        if (option == 1)
        {
            // insert
            int size = input.size();
            outfile << "Insert:";
            for (int i = 0; i < size; i++)
            {
                outfile << ' ' << input[i];
                if (i != size - 1)
                    outfile << ',';
                Node *new_node = new Node(input[i]);
                tree.insert(new_node);
            }
            outfile << "\n";
            tree.print(outfile);
        }
        else if (option == 2)
        {
            // remove
            int size = input.size();
            outfile << "Delete:";
            for (int i = 0; i < size; i++)
            {
                outfile << ' ' << input[i];
                if (i != size - 1)
                    outfile << ',';
                Node *key = tree.find(input[i]);
                tree.remove(key);
            }
            outfile << "\n";
            tree.print(outfile);
        }
        if (k != t - 1)
            outfile << "\n";
    }
    infile.close();
    outfile.close();
    return 0;
}
