// Lab 9 - Task 2: Word Frequency Indexing using BST
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

struct WordNode {
    string word;
    int freq;
    WordNode* left;
    WordNode* right;
    WordNode(const string& w) : word(w), freq(1), left(nullptr), right(nullptr) {}
};

class WordBST {
    WordNode* root;
    int uniqueWords;
    int totalWords;

    WordNode* insert(WordNode* node, const string& w) {
        if (!node) { uniqueWords++; return new WordNode(w); }
        if (w < node->word) node->left = insert(node->left, w);
        else if (w > node->word) node->right = insert(node->right, w);
        else node->freq++;                         // word already exists
        return node;
    }

    void inorderPrint(WordNode* node) const {
        if (!node) return;
        inorderPrint(node->left);
        cout << "  " << node->word << " : " << node->freq << endl;
        inorderPrint(node->right);
    }

    // Collect all nodes (in alphabetical order) into a vector
    void collect(WordNode* node, vector<WordNode*>& v) const {
        if (!node) return;
        collect(node->left, v);
        v.push_back(node);
        collect(node->right, v);
    }

    int maxFreq(WordNode* node) const {
        if (!node) return 0;
        return max(node->freq, max(maxFreq(node->left), maxFreq(node->right)));
    }

    void printWithFreq(WordNode* node, int f) const {
        if (!node) return;
        printWithFreq(node->left, f);
        if (node->freq == f) cout << "  " << node->word << " (" << f << " times)" << endl;
        printWithFreq(node->right, f);
    }

    void destroy(WordNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    WordBST() : root(nullptr), uniqueWords(0), totalWords(0) {}
    ~WordBST() { destroy(root); }

    void insertWord(const string& w) {
        root = insert(root, w);
        totalWords++;
    }

    // Split paragraph into words: keep letters only, convert to lowercase
    void parseParagraph(const string& text) {
        string word = "";
        for (char c : text) {
            if (isalpha((unsigned char)c)) {
                word += (char)tolower((unsigned char)c);
            } else if (!word.empty()) {
                insertWord(word);
                word = "";
            }
        }
        if (!word.empty()) insertWord(word);
    }

    void displayAlphabetical() const {
        cout << "Words in Alphabetical Order:\n";
        inorderPrint(root);
    }

    // Sort by frequency (high to low); ties stay alphabetical (stable insertion sort)
    void displayByFrequency() const {
        vector<WordNode*> v;
        collect(root, v);
        for (size_t i = 1; i < v.size(); i++) {
            WordNode* key = v[i];
            int j = (int)i - 1;
            while (j >= 0 && v[j]->freq < key->freq) {
                v[j + 1] = v[j];
                j--;
            }
            v[j + 1] = key;
        }
        cout << "Words by Frequency (Highest to Lowest):\n";
        for (WordNode* n : v) cout << "  " << n->word << " : " << n->freq << endl;
    }

    void findMostFrequentWord() const {
        int f = maxFreq(root);
        cout << "Most Frequent Word(s):\n";
        printWithFreq(root, f);
    }

    void displaySummary() const {
        cout << "Total Unique Words: " << uniqueWords << endl;
        cout << "Total Word Count:   " << totalWords << endl;
    }
};

int main() {
    string paragraph = "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";

    WordBST index;
    index.parseParagraph(paragraph);

    cout << "===== Task 2: Word Frequency Indexing using BST =====\n\n";
    cout << "Paragraph: \"" << paragraph << "\"\n\n";

    index.displayAlphabetical();
    cout << endl;
    index.displayByFrequency();
    cout << endl;
    index.findMostFrequentWord();
    cout << endl;
    index.displaySummary();

    return 0;
}