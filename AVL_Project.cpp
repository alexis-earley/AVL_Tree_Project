#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

class AVLTree {

public:
    class Node { //holds the data for each node in the tree
    public:
        long id;
        string name;
        Node* left;
        Node* right;
        Node() { //default constructor
            id = -1;
            name = "";
            left = nullptr;
            right = nullptr;
        }
        Node(long id, string name) { //constructor with name and ID
            this->id = id;
            this->name = name;
            left = nullptr;
            right = nullptr;
        }
        Node(long id, string name, Node* left, Node* right) { //constructor with name, ID, left and right
            this->id = id;
            this->name = name;
            this->left = left;
            this->right = right;
        }
    };

    //public methods
    //for use in main
    void insertNameID(string name, long id);
    void removeID(long id);
    void searchName(string);
    void searchID(long id);
    void printPreorder();
    void printInorder();
    void printPostorder();
    long printLevelCount();
    void removeInOrderN(long num);
    AVLTree();
    AVLTree(Node* root);
    ~AVLTree();

private:
    Node* AVLRoot; //holds the root of the tree

    //private methods
    Node* insert(Node* root, string name, long id);
    long returnBalance(Node* root);
    Node* rotateLeft(Node *node);
    Node* rotateRight(Node *node);
    Node* rotateLeftRight(Node *node);
    Node* rotateRightLeft(Node *node);
    Node* deleteNode(Node* root, long ID);
    Node* inOrderPred(Node* node);
    void search(Node* root, string name, vector<Node*>& nameVector);
    string search(Node* root, long id);
    void preorder(Node* root);
    void inorder(Node* root);
    void postorder(Node* root);
    void addInOrder(Node* root, vector<long>& elementVector);
    void preOrder(Node* root, vector<Node*>& nodeVector);
    Node* makeTree(vector<Node*>& nodeVector, long first, long last);
    Node* convertToBBST(Node* root);
    long height(Node* root);
    long heightForAVL(Node* root, vector<long>& balanceVector);
    bool isAVL(Node* root);
    void destructNodes(Node* root);
};

void AVLTree::insertNameID(string name, long id) { //inserts new node in tree with given name and ID
    bool inTree = true;
    if (AVLRoot == nullptr) {
        inTree = false; //if the tree is empty, the element certainly isn't in it
    }
    else { //otherwise search to see if the element is in the tree
        if (search(AVLRoot, id) == "unsuccessful") {
            inTree = false;
        }
    }
    if (!inTree) { //if it's not in the tree, insert it
        if (AVLRoot != nullptr) {
            AVLRoot = insert(AVLRoot, name, id);
        }
        else {
            AVLRoot = insert(nullptr, name, id);
        }
        cout << "successful" << endl;
    }
    else {
        cout << "unsuccessful" << endl;
    }
}

void AVLTree::removeID(long id) { //remove a node with a certain ID
    if (AVLRoot != nullptr) {
        AVLRoot = deleteNode(AVLRoot, id);
    }
    else {
        cout << "unsuccessful" << endl;
    }
}

// Search for student by name
// Return id if found, or unsuccessful if not found
void AVLTree::searchName(string name) {
    vector<Node*> nameVector;
    if (AVLRoot != nullptr) {
        search(AVLRoot, name, nameVector);
    }
    else { //not in tree if it's empty
        cout << "unsuccessful" << endl;
        return;
    }
    long numResults = nameVector.size();
    if (numResults != 0) { //print out all people with that name
        for (int i = 0; i < numResults; i++) {
            Node* currentNode = nameVector[i];
            int currentID = currentNode->id;
            string stringID = to_string(currentID);
            if (stringID.length() < 8) { //add leading zeros if the ID is less than 10000000
                for (int i = 0; i < (8 - stringID.length()); i++) {
                    cout << "0";
                }
            }
            cout << currentNode->id << endl;
        }
    }
    else {
        cout << "unsuccessful" << endl; //if there were no people with that name
    }
}

// Search for student by id
// Return name if found, or "unsuccessful"
void AVLTree::searchID(long id) {
    if (AVLRoot != nullptr) { //otherwise will throw an error
        cout << search(AVLRoot, id) << endl;
    }
    else {
        cout << "unsuccessful" << endl; //if there are no items in the array, none will have matching IDs
    }
}

// Return comma-separated string of preorder traversal
void AVLTree::printPreorder() {
    if (AVLRoot != nullptr) { //otherwise will throw an error
        preorder(AVLRoot);
        cout << endl;
    }
}

// Print inorder traversal
void AVLTree::printInorder() {
    if (AVLRoot != nullptr) { //otherwise will throw an error
        inorder(AVLRoot);
        cout << endl;
    }
}

// Print postorder traversal
void AVLTree::printPostorder() {
    if (AVLRoot != nullptr) { //otherwise will throw an error
        postorder(AVLRoot);
        cout << endl;
    }
}

// Print levels in current AVL tree
long AVLTree::printLevelCount() {
    if (AVLRoot != nullptr) { //otherwise will throw an error
        return height(AVLRoot);
    }
    else {
        return 0;
    }
}

//remove the Nth element in the in order tranversal
//developed with the help of Marc Diaz in office hours
void AVLTree::removeInOrderN(long num) {
    if (AVLRoot != nullptr) {
        vector<long> elementVector;
        addInOrder(AVLRoot, elementVector); //add the elements in order to a vector
        if (num < elementVector.size()) {
            long elementID = elementVector[num];
            AVLRoot = deleteNode(AVLRoot, elementID); //if this item is in the vector, delete it
        }
        else {
            cout << "unsuccessful" << endl;
        }
    }
    else { //there are no items in the array
        cout << "unsuccessful" << endl;
    }
}

AVLTree::AVLTree() { //constructor; creates an empty tree
    AVLRoot = nullptr;
}

AVLTree::AVLTree(Node* root) { //constructor; creates a tree with one root
    AVLRoot = root;
}

AVLTree::~AVLTree() { //destructor
    if (AVLRoot != nullptr) {
        destructNodes(AVLRoot);
    }
}

//adapted from slide 31 of "Trees" class PowerPoint
//recursively inserts a node into the tree, connecting it to its parent
AVLTree::Node* AVLTree::insert(Node* root, string name, long id) {
    if (root == nullptr) {
        return new Node(id, name);
    }

    if (id < root->id) {
        root->left = insert(root->left, name, id);
    }
    else {
        root->right = insert(root->right, name, id);
    }

    int nodeBalance = returnBalance(root);

    if ((nodeBalance > 1) && (id < (root->left)->id)) { //left left
        return rotateRight(root); }

    if ((nodeBalance < -1) && (id > (root->right)->id)) { //right right
        return rotateLeft(root); }

    if ((nodeBalance > 1) && (id > (root->left)->id)) { //left right
        return rotateLeftRight(root); }

    if ((nodeBalance < -1) && (id < (root->right)->id)) { //rightLeft
        return rotateRightLeft(root); }

    return root;
}

long AVLTree::returnBalance(Node* root) {
    if (root == nullptr) {return 0;}
    else {return (height(root->left)) - (height(root->right));}
}

AVLTree::Node* AVLTree::rotateLeft(Node *node)
{
    Node* rightChild = node->right;
    Node* rLChild = rightChild->left;
    rightChild->left = node;
    node->right = rLChild;
    return rightChild;
}

AVLTree::Node* AVLTree::rotateRight(Node *node)
{
    Node* leftChild = node->left;
    Node* lRChild = leftChild->right;
    leftChild->right = node;
    node->left = lRChild;
    return leftChild;
}

AVLTree::Node* AVLTree::rotateLeftRight(Node *node)
{
    Node* leftChild = node->left;
    node->left = rotateLeft(leftChild);
    return rotateRight(node);
}

AVLTree::Node* AVLTree::rotateRightLeft(Node *node)
{
    Node* rightChild = node->right;
    node->right = rotateRight(rightChild);
    return rotateLeft(node);
}

//deletes a node from a tree
AVLTree::Node* AVLTree::deleteNode(Node* root, long ID)
{
    if (root == nullptr) { //root is not in the tree
        cout << "unsuccessful" << endl;
        return root;
    }
    if (ID < root->id) {//move to the left in the tree
        root->left = deleteNode(root->left, ID);
    }
    else if (ID > root->id) { //move to the right in the tree
        root->right = deleteNode(root->right, ID);
    }
    else {
        if (root->left == nullptr) { //just has a right child or has no children
            Node* rightNode = root->right;
            delete root;
            cout << "successful" << endl;
            return rightNode;
        }
        else if (root->right == nullptr) { //just has a left child
            Node* leftNode = root->left;
            delete root;
            cout << "successful" << endl;
            return leftNode;
        }
        else { //has two children
            Node* inOrderPre = inOrderPred(root->left);
            root->name = inOrderPre->name;
            root->id = inOrderPre->id;
            root->left = deleteNode(root->left, inOrderPre->id);
        }
    }
    return root;
}

AVLTree::Node* AVLTree::inOrderPred(Node* node) { //locates the in order predecessor for the previous function
    if (node->right == nullptr) {
        return node;
    }
    else {
        return inOrderPred(node->right);
    }
}

void AVLTree::search(Node* root, string name, vector<Node*>& nameVector) { //searches for a certain name in the tree
    if (root->name.compare(name) == 0) {
        nameVector.push_back(root);
    }
    if (root->left != nullptr) {
        search(root->left, name, nameVector);
    }
    if (root->right != nullptr) {
        search(root->right, name, nameVector);
    }
}

string AVLTree::search(Node* root, long id) { //searches for a certain ID for the tree
    long rootID = root->id;
    if (id == rootID) { //found element
        return root->name;
    }
    else if (id < rootID) { //look at left hand side of tree
        if (root->left != nullptr) {
            return search(root->left, id);
        }
        else {
            return "unsuccessful";
        }
    }
    else if (id > rootID) {
        if (root->right != nullptr) { //look at right hand side of tree
            return search(root->right, id);
        }
        else {
            return "unsuccessful";
        }
    }
}

//adapted from my response to Stepik question 4.1.1
//prints preorder transversal of nodes
void AVLTree::preorder(Node* root) {
    cout << root->name;
    if (root->left != nullptr) {
        cout << ", ";
        preorder(root->left);
    }
    if (root->right != nullptr) {
        cout << ", ";
        preorder(root->right);
    }
}

//prints inorder transversal of nodes
void AVLTree::inorder(Node* root) {
    if (root->left != nullptr) {
        inorder(root->left);
        cout << ", ";
    }
    cout << root->name;
    if (root->right != nullptr) {
        cout << ", ";
        inorder(root->right);
    }
}

//prints postorder transversal of nodes
void AVLTree::postorder(Node* root) {
    if (root->left != nullptr) {
        postorder(root->left);
        cout << ", ";
    }
    if (root->right != nullptr) {
        postorder(root->right);
        cout << ", ";
    }
    cout << root->name;
}

//adapted from my response to Stepik question 4.1.2
//adds elements in order to elementVector
//used when removing the Nth element in the in order tranversal
void AVLTree::addInOrder(Node* root, vector<long>& elementVector) {
    if (root->left != nullptr) {
        addInOrder(root->left, elementVector);
    }
    elementVector.push_back(root->id);
    if (root->right != nullptr) {
        addInOrder(root->right, elementVector);
    }
}

//adapted from my response to Stepik question 5.1.2
//puts elements in preorder into nodeVector
void AVLTree::preOrder(Node* root, vector<Node*>& nodeVector)
{
    if (root->left != nullptr) {
        preOrder(root->left, nodeVector);
    }
    nodeVector.push_back(root);
    if (root->right != nullptr) {
        preOrder(root->right, nodeVector);
    }
}

//adapted from my response to Stepik question 5.1.2
//makes a balanced tree using inorder transversal
AVLTree::Node* AVLTree::makeTree(vector<Node*>& nodeVector, long first, long last)
{
    long mid = ceil((first + last) / 2); //calculates mid element, this is the next root
    Node* root = nodeVector[mid];

    if (first <= mid - 1) {
        root->left = makeTree(nodeVector, first, mid - 1);
    }
    else {
        root->left = nullptr; //out of nodes on this path
    }

    if (mid + 1 <= last) {
        root->right = makeTree(nodeVector, mid + 1, last);
    }
    else {
        root->right = nullptr; //out of nodes on this path
    }

    return root;
}

//adapted from my response to Stepik question 5.1.2
//converts to a balanced tree
AVLTree::Node* AVLTree::convertToBBST(Node* root)
{
    vector<Node*> nodeVector;
    preOrder(root, nodeVector);
    return makeTree(nodeVector, 0, nodeVector.size() - 1);
}

//adapted from my response to Stepik question 5.2.1
//calculates the height of the tree
long AVLTree::height(Node* root) {
    if (root == nullptr) {return 0;} //don't want to throw an exception
    long heightLeft;
    long heightRight;
    //find height of the left side
    if (root->left != nullptr) {
        heightLeft = height(root->left) + 1;
    }
    else {
        heightLeft = 1;
    }

    //find height of the right side
    if (root->right != nullptr) {
        heightRight = height(root->right) + 1;
    }
    else {
        heightRight = 1;
    }

    //return maximum height
    return max(heightLeft, heightRight);
}

//adapted from my response to Stepik question 5.2.1
//calculates the height of the tree
long AVLTree::heightForAVL(Node* root, vector<long>& balanceVector) {
    long heightLeft;
    long heightRight;
    //find height of the left side
    if (root->left != nullptr) {
        heightLeft = heightForAVL(root->left, balanceVector) + 1;
    }
    else {
        heightLeft = 1;
    }

    //calculate height of the right side
    if (root->right != nullptr) {
        heightRight = heightForAVL(root->right, balanceVector) + 1;
    }
    else {
        heightRight = 1;
    }

    //add left - right to array
    balanceVector.push_back(heightLeft - heightRight);

    //return height of this subtree
    return max(heightLeft, heightRight);
}

//adapted from my response to Stepik question 5.2.1
//check to see if the tree is balanced
bool AVLTree::isAVL(Node* root)
{
    vector<long> balanceVector;
    heightForAVL(root, balanceVector);

    for (long i = 0; i < balanceVector.size(); i++) {
        if ((balanceVector[i] < -1) || (balanceVector[i] > 1)) {
            return false;
        }
    }
    return true;
}

//used as part of the destructor
void AVLTree::destructNodes(Node* root) {
    if (root != nullptr) {
        destructNodes(root->left);
        destructNodes(root->right);
        delete root;
    }
}

//main function that checks to see if the string is valid and turns it into an int
int convertID(string id) {
    if (id.size() != 8) {
        return -1; //not valid
    }
    try {
        int intID = stoi(id);
        if (intID >= 0) {
            return intID;
        }
        else {
            return -1; //not valid
        }
    }
    catch (invalid_argument & e) {
        return -1; //not valid
    }
}

bool isName(string name) { //checks to see if the name is valid
    for (int i = 0; i < name.length(); i++) {
        int ascii = int(name[i]);
        if (!((ascii == 32) || ((ascii >= 65) && (ascii <= 90)) || ((ascii >= 97) && (ascii <= 122)))) { //if it is not a character or space
            return false;
        }
    }
    return true;
}


int main()
{
    AVLTree tree; //create a tree

    string commandStr; //first line entered
    getline(cin, commandStr);
    int numCommands = stoi(commandStr);

    string currLine;
    string word;
    vector<vector<string>> commands; //holds all commands, with each vector holding each word at a different element
    vector<string> words;
    stringstream ss;
    for (int i = 0; i < numCommands; i++) { //repeat for each command
        getline(cin, currLine);
        ss.clear();
        ss.str(currLine);
        commands.push_back(words); //add each word to array
        while (ss >> word) {
            commands[i].push_back(word);
        }
    }

    for (int i = 0; i < numCommands; i++) { //again repeat for each command
        vector<string> words = commands[i];
        if (words[0] == "search") { //searching for something

            string toSearch = "";
            int i;
            for (i = 1; i < words.size(); i ++) {
                toSearch += words[i];
                toSearch += " ";
            }

            if (toSearch.substr(0, 1) == "\"") { //searching for a name
                string name = toSearch.substr(1, toSearch.size() - 3); //take off the end quote and last space
                if (isName(name)) {
                    tree.searchName(name);
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
            else { //searching for an element
                string num = toSearch.substr(0,toSearch.size() - 1);
                int id = convertID(num); //take off last space
                if (id != -1) {
                    tree.searchID(id);
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
        }
        else if (words[0] == "insert") { //inserting a name and id
            string original = "";
            int i;
            for (i = 1; i < words.size() - 1; i ++) {
                original += words[i];
                original += " ";
            }
            string name = original.substr(1, original.size() - 3); //take off the end quote and last space
            int id = convertID(words[i]);
            if ((id != -1) && isName(name)) { //check to make sure both are valid
                tree.insertNameID(name, id);
            }
            else {
                cout << "unsuccessful" << endl; //not valid
            }
        }
        else if (words[0] == "printLevelCount") {
            cout << tree.printLevelCount() << endl; //print levels
        }
        else if (words[0] == "printInorder") {
            tree.printInorder(); //print inorder transversal
        }
        else if (words[0] == "printPreorder") {
            tree.printPreorder(); //print preorder transversal
        }
        else if (words[0] == "printPostorder") {
            tree.printPostorder(); //print postorder transversal
        }
        else if (words[0] == "remove") { //removing an ID
            int id = convertID(words[1]);
            if (id != -1) {
                tree.removeID(id);
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }
        else if (words[0] == "removeInorder") { //removing the Nth element in an order
            try {
                int element = stoi(words[1]);
                tree.removeInOrderN(element);
            }
            catch (invalid_argument & e) {
                cout << "unsuccessful" << endl;
            }
        }
    }
}
