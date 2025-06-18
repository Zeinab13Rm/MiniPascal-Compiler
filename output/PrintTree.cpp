#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

struct TreeNode {
    string name;
    vector<TreeNode*> children;
    TreeNode(const string& n) : name(n) {}
};

void printTree(const TreeNode* node, int depth = 0) {
    if (!node) return;
    
    // Print indentation
    for (int i = 0; i < depth; ++i) {
        if (i == depth - 1) cout << "  ├── ";
        else cout << "  │   ";
    }
    if (depth == 0) cout << "• ";
    
    cout << node->name << endl;
    
    // Print children
    for (size_t i = 0; i < node->children.size(); ++i) {
        printTree(node->children[i], depth + 1);
    }
}

TreeNode* buildTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return nullptr;
    }

    TreeNode* root = nullptr;
    stack<TreeNode*> nodeStack;
    string line;

    while (getline(file, line)) {
        // Calculate indentation level
        size_t indent = line.find_first_not_of(" \t");
        if (indent == string::npos) continue;  // skip empty lines
        
        string content = line.substr(indent);
        TreeNode* newNode = new TreeNode(content);

        if (indent == 0) {
            // Root node
            root = newNode;
            nodeStack.push(root);
        } else {
            // Find parent based on indentation
            while (nodeStack.size() > indent/2) {
                nodeStack.pop();
            }
            
            if (!nodeStack.empty()) {
                nodeStack.top()->children.push_back(newNode);
                nodeStack.push(newNode);
            }
        }
    }

    file.close();
    return root;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " Test0.ast.txt" << endl;
        return 1;
    }

    TreeNode* root = buildTreeFromFile(argv[1]);
    if (root) {
        printTree(root);
        
        // Clean up memory (in a real app you'd want a proper destructor)
        // This is simplified for the example
        delete root;
    }

    return 0;
}