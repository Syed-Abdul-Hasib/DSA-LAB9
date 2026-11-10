// Lab 9 - Task 1: Range Query and Successor/Predecessor Finder using BST
#include <iostream>
#include <stack>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
    Node* root;

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);
        return node; // duplicates ignored
    }

    void inorder(Node* node) const {
        if (!node) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

    // Recursive range query: prints keys in [low, high], counts comparisons
    void rangeRec(Node* node, int low, int high, int& comps) const {
        if (!node) return;
        comps++;                                  // compare with low
        if (low < node->key) rangeRec(node->left, low, high, comps);
        comps++;                                  // check if inside range
        if (node->key >= low && node->key <= high) cout << node->key << " ";
        comps++;                                  // compare with high
        if (high > node->key) rangeRec(node->right, low, high, comps);
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void insert(int key) { root = insert(root, key); }

    void displayInorder() const {
        cout << "Inorder Traversal: ";
        inorder(root);
        cout << endl;
    }

    // Inorder predecessor and successor of a key (iterative, O(h))
    void findPreSuc(int key) const {
        Node* pre = nullptr;
        Node* suc = nullptr;
        Node* curr = root;

        while (curr) {
            if (key < curr->key) {
                suc = curr;              // possible successor
                curr = curr->left;
            } else if (key > curr->key) {
                pre = curr;              // possible predecessor
                curr = curr->right;
            } else {
                // Key found: predecessor = max of left subtree
                if (curr->left) {
                    Node* t = curr->left;
                    while (t->right) t = t->right;
                    pre = t;
                }
                // Successor = min of right subtree
                if (curr->right) {
                    Node* t = curr->right;
                    while (t->left) t = t->left;
                    suc = t;
                }
                break;
            }
        }

        if (!curr) cout << "(Key " << key << " not in tree, showing nearest values)\n";
        cout << "Key = " << key << endl;
        cout << "Inorder Predecessor: " << (pre ? to_string(pre->key) : "None") << endl;
        cout << "Inorder Successor:   " << (suc ? to_string(suc->key) : "None") << endl;
    }

    void rangeQueryRecursive(int low, int high) const {
        int comps = 0;
        cout << "Recursive Range [" << low << ", " << high << "]: ";
        rangeRec(root, low, high, comps);
        cout << "\nComparisons (recursive): " << comps << endl;
    }

    // Iterative range query using a stack (inorder with pruning)
    void rangeQueryIterative(int low, int high) const {
        int comps = 0;
        stack<Node*> st;
        Node* curr = root;
        cout << "Iterative Range [" << low << ", " << high << "]: ";

        while (curr || !st.empty()) {
            // Go left only while the left subtree can contain values >= low
            while (curr) {
                st.push(curr);
                comps++;                          // compare with low
                if (low < curr->key) curr = curr->left;
                else curr = nullptr;
            }
            curr = st.top();
            st.pop();

            comps++;                              // check if inside range
            if (curr->key >= low && curr->key <= high) cout << curr->key << " ";

            comps++;                              // compare with high
            if (high > curr->key) curr = curr->right;
            else curr = nullptr;
        }
        cout << "\nComparisons (iterative): " << comps << endl;
    }
};

int main() {
    int arr[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    int n = sizeof(arr) / sizeof(arr[0]);

    BST tree;
    for (int i = 0; i < n; i++) tree.insert(arr[i]);

    cout << "===== Task 1: BST Range Query & Successor/Predecessor =====\n\n";
    tree.displayInorder();

    cout << "\n--- Predecessor / Successor ---\n";
    tree.findPreSuc(40);

    cout << "\n--- Range Query (30 to 70) ---\n";
    tree.rangeQueryRecursive(30, 70);
    tree.rangeQueryIterative(30, 70);

    return 0;
}