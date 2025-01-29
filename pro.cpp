#include <iostream>
using namespace std;

class Node {
public:
    int key;
    Node* left;
    Node* right;
    int height;

    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    int getHeight(Node* node) {
        if (node) {
            return node->height;
        }
        else {
            return 0;
        }
    }


    int getBalance(Node* node) {
        if (node) {
            return getHeight(node->left) - getHeight(node->right);
        }
        else {
            return 0;
        }
    }


    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    Node* insert(Node* node, int key) {
        if (!node)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rotateRight(node);

        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);

        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }
    Node* findMin(Node* node) {
        while (node->left)
            node = node->left;
        return node;
    }

    Node* deleteNode(Node* root, int key) {
        if (!root)
            return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;

                if (!temp) {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;

                delete temp;
            }
            else {
                Node* temp = findMin(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        if (!root)
            return root;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    Node* search(Node* node, int key) {
        if (!node || node->key == key)
            return node;

        if (key < node->key)
            return search(node->left, key);

        return search(node->right, key);
    }

    void inOrder(Node* node) {
        if (node) {
            inOrder(node->left);
            cout << node->key << " ";
            inOrder(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }
    void deleteKey(int key) {
        root = deleteNode(root, key);
    }

    bool searchKey(int key) {
        return search(root, key) != nullptr;
    }
    void inOrderTraversal() {
        inOrder(root);
        cout << endl;
    }
};
struct hashnode {
    AVLTree tree;
    bool occupied;

    hashnode() : occupied(false) {}
};

class Hashtable {
private:
    hashnode* table;
    int capacity;

    int hashfunction(int val) {
        return val % capacity;
    }

public:
    Hashtable(int capacity = 8) : capacity(capacity) {
        table = new hashnode[capacity];
    }

    void insertval(int val) {
        int index = hashfunction(val);
        table[index].occupied = true;
        table[index].tree.insert(val);
    }
    bool searchVal(int val) {
        int index = hashfunction(val);
        if (table[index].occupied) {
            return table[index].tree.searchKey(val);
        }
        return false;
    }

    void deleteVal(int val) {
        int index = hashfunction(val);
        if (table[index].occupied) {
            table[index].tree.deleteKey(val);
        }
    }
    void print() {
        for (int i = 0; i < capacity; i++) {
            cout << "Index " << i << ": ";
            if (table[i].occupied) {
                table[i].tree.inOrderTraversal();
            }
            else {
                cout << "Empty" << endl;
            }
        }
    }

    ~Hashtable() {
        delete[] table;
    }
};
class Hashtablel {
private:
    int* table;
    bool* occupied;
    int capacity;
    int size;

    int hashfunction(int val) {
        return val % capacity;
    }

public:
    Hashtablel(int capacity = 8) : capacity(capacity), size(0) {
        table = new int[capacity];
        occupied = new bool[capacity] {false};
    }

    void insertval(int val) {
        if (size == capacity) {
            cout << "Hashtable is full!" << endl;
            return;
        }

        int index = hashfunction(val);
        while (occupied[index]) {

            index = (index + 1) % capacity;
        }

        table[index] = val;
        occupied[index] = true;
        size++;
    }


    bool searchVal(int val) {
        int index = hashfunction(val);
        int originalIndex = index;

        while (occupied[index]) {
            if (table[index] == val) {
                return true;
            }
            index = (index + 1) % capacity;

            if (index == originalIndex) {
                break;
            }
        }

        return false;
    }


    void deleteVal(int val) {
        int index = hashfunction(val);
        int originalIndex = index;

        while (occupied[index]) {
            if (table[index] == val) {
                occupied[index] = false;
                size--;
                return;
            }
            index = (index + 1) % capacity;

            if (index == originalIndex) {
                break;
            }
        }

        cout << "Value not found!" << endl;
    }


    void print() {
        for (int i = 0; i < capacity; i++) {
            if (occupied[i]) {
                cout << "Index " << i << ": " << table[i] << endl;
            }
            else {
                cout << "Index " << i << ": Empty" << endl;
            }
        }
    }

    ~Hashtablel() {
        delete[] table;
        delete[] occupied;
    }
};
int main()
{

    cout << "This is the AVL Approach: " << endl;
    Hashtable table;

    table.insertval(5);
    table.insertval(2);
    table.insertval(1);
    table.insertval(3);


    cout << "Hashtable with AVL Trees:" << endl;
    table.print();


    if (table.searchVal(3)) {
        cout << "Value 3 found in the hashtable." << endl;
    }
    else {
        cout << "Value 3 not found in the hashtable." << endl;
    }


    table.deleteVal(3);
    cout << "After deleting value 3:" << endl;
    table.print();

    cout << endl << endl;
    cout << "Linear Probing Approach: " << endl;
    Hashtablel tablel;


    tablel.insertval(5);
    tablel.insertval(2);
    tablel.insertval(1);
    tablel.insertval(3);


    cout << "Hashtable with Linear Probing:" << endl;
    tablel.print();

    if (tablel.searchVal(3)) {
        cout << "Value 3 found in the hashtable." << endl;
    }
    else {
        cout << "Value 3 not found in the hashtable." << endl;
    }

    tablel.deleteVal(3);
    cout << "After deleting value 3:" << endl;
    table.print();

    return 0;
}