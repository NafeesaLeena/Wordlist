//Name: Nafeesa Leena
//Date: November. 22, 2024

#include "Wordlist.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
using std::setprecision;
using std::fixed;
using std::ifstream;
using std::istringstream;
using std::invalid_argument;
using std::max;


// Prints useful statistics about the word list
void Wordlist::printStatistics() const
{
	cout << "Number of different words: " << differentWords() << endl;
	cout << "    Total number of words: " << totalWords() << endl;
	cout << "       Most frequent word: " << mostFrequent() << endl;
	cout << "     Number of singletons: " << singletons()
		<< setprecision(0) << fixed
		<< " (" << 100.0 * singletons() / differentWords() << "%)"
		<< endl;
}

// Defualt Constructor 
Wordlist::Wordlist() 
{
	root = nullptr;
	size = 0;
}

// File constructor 
Wordlist::Wordlist(const string& filename)
{
	root = nullptr;
	size = 0;

	ifstream file(filename); // open file for reading
	// if file doesnt open give an error
	if (!file)
	{
		throw invalid_argument("File cannot be opened: " + filename);
	}

	string line;
	while (getline(file, line)) // read line by line
	{
		istringstream iss(line);
		string word;
		while (iss >> word) // extract words
		{
			insert(word); //insert each word into the wordlist
		}
	}
	file.close();
}

// Destructor
Wordlist::~Wordlist()
{
	destroyTree(root);
	root = nullptr;
	size = 0;
}

// Helper function to recursively delete the nodes
void Wordlist::destroyTree(AVLTreeNode* node)
{
	if (node != nullptr) 
	{
		destroyTree(node -> left); // delete the left subtree
		destroyTree(node -> right); // delete the right subtree
		delete node; 
	}
}

// Copy constructor
Wordlist::Wordlist(const Wordlist& other)
{
	root = nullptr;
	size = 0;

	if (other.root != nullptr)
	{
		root = copyNode(nullptr, other.root); //copy tree
		size = other.size; //copy size of the tree
	}
}

// Helper function to recusively copy the nodes
AVLTreeNode* Wordlist::copyNode(AVLTreeNode* parent, AVLTreeNode* otherNode)
{
	if (otherNode == nullptr)
	{
		return nullptr;
	}

	// create a new node with the same word and count as the other node
	AVLTreeNode* newNode = new AVLTreeNode(otherNode -> word); 
	newNode -> count = otherNode -> count; // copy count of occurrences
	newNode -> parent = parent; // set the parent to the current 
	newNode -> left = copyNode(newNode, otherNode -> left); // copy the left subtree
	newNode -> right = copyNode(newNode, otherNode -> right); // copy the right subtree
	newNode -> height = otherNode -> height; // copy the height of the node
	return newNode; // return the new node
}

// Assignment operator
Wordlist& Wordlist::operator = (const Wordlist& other)
{
	if (this != &other)
	{
		// get rif of current tree
		destroyTree(root);

		// copy the other tree
		if (other.root != nullptr)
		{
			root = copyNode(nullptr, other.root); // tree structure copy 
			size = other.size; // copy the size
		}
		else
		{
			root = nullptr;
			size = 0;
		}
	}

	return *this;
}

// Insert 
void Wordlist::insert(const string& word)
{
	root = insertNode(root, word, nullptr); // insert word starting from the root
}

// Helper function to insert a node and balance the tree
AVLTreeNode* Wordlist:: insertNode(AVLTreeNode* node, const string& word, AVLTreeNode* parent)
{
	if (node == nullptr)
	{
		size++;
		AVLTreeNode* newNode = new AVLTreeNode(word); // create a new node with the word
		newNode -> parent = parent; // set the parent of the new node
		return newNode; //return the new node

	}

	// if the word is smaller, insrrt into the left subtree
	if (word < node -> word)
	{
		node -> left = insertNode(node -> left, word, node);
	}

	// word is longer, insert into right subtree
	else if (word > node -> word)
	{
		node -> right = insertNode(node -> right, word, node);
	}

	else // word already exists, so increment the count
	{
		node -> count++;
		return node;
	}

	updateHeight(node);

	node = balanceNode(node); //balance the subtree rooted at the current node

	return node;

}

void Wordlist::updateHeight(AVLTreeNode* node)
{
	// height of the left subtree
	int leftHeight;
	if (node -> left != nullptr)
	{
		leftHeight = node -> left -> height;
	}
	else
	{
		leftHeight = -1;
	}

	// height of left subtree
	int rightHeight;
	if (node -> right != nullptr)
	{
		rightHeight = node -> right -> height;
	}
	else
	{
		rightHeight = -1;
	}

	node -> height = max(leftHeight, rightHeight) + 1; // add both left and right together than add 1
}

int Wordlist::getBalance(AVLTreeNode* node)
{
	int leftHeight;
	if (node -> left != nullptr)
	{
		leftHeight = node -> left -> height;
	}
	else
	{
		leftHeight = -1;
	}

	int rightHeight;
	if (node -> right != nullptr)
	{
		rightHeight= node -> right -> height;
	}
	else
	{
		rightHeight = -1;
	}

	return leftHeight - rightHeight;
}

AVLTreeNode* Wordlist::balanceNode(AVLTreeNode* node)
{
	int balance = getBalance(node); // get balance factor

	//left heavy
	if (balance > 1)
	{
		//left-left case
		if (getBalance(node -> left) >= 0)
		{
			return rotateRight(node);
		}

		//left-right case
		else
		{
			node -> left = rotateLeft(node -> left);
			return rotateRight(node);
		}
	}

	// right heavy
	else if(balance < -1)
	{
		//right-right case
		if (getBalance(node->right) <= 0)
		{
			return rotateLeft(node);
		}

		//right-left case
		else
		{
			node -> right = rotateRight(node -> right);
			return rotateLeft(node);
		}
	}

	// node is balanced
	return node;

}

AVLTreeNode* Wordlist::rotateRight(AVLTreeNode* y)
{
	AVLTreeNode* x = y -> left; // set the left child as the new root of the subtree
	AVLTreeNode* T2 = x -> right; //save the right child of x

	// rotation
	x -> right = y; //make y the right child of x
	y -> left = T2; // set T2 as the left child of y

	// update parents
	x -> parent = y -> parent;
	y -> parent = x;
	if (T2)
	{
		T2 -> parent = y;
	}

	// update child pointers of parent
	if (x -> parent != nullptr)
	{
		if (x -> parent -> left == y)
		{
			x -> parent -> left = x;
		}
		else if (x -> parent -> right == y)
		{
			x -> parent -> right = x;
		}
	}
	else
	{
		root = x;
	}

	// update heights
	updateHeight(y);
	updateHeight(x);

	return x;
}

AVLTreeNode* Wordlist::rotateLeft(AVLTreeNode* x)
{
    AVLTreeNode* y = x -> right;
    AVLTreeNode* T2 = y -> left;

    // rotation
    y -> left = x;
    x -> right = T2;

    // update parents
    y -> parent = x -> parent;
    x -> parent = y;
    if (T2)
	{
        T2 -> parent = x;
	}

	// update child pointers of parents
	if (y -> parent != nullptr)
	{
		if (y -> parent -> left == x)
		{
			y -> parent -> left = y;
		}
		else if (y -> parent -> right == x)
		{
			y -> parent -> right = y;
		}
	}
	else
	{
		root = y;
	}

    // update heights
    updateHeight(x);
    updateHeight(y);

    return y;
}

// print words
void Wordlist::printWords() const
{
    unsigned int index = 1; // keeps track of the line number for output
	printWordsHelper(root, index); 
}

void Wordlist::printWordsHelper(AVLTreeNode* node, unsigned int& index) const
{
	if (node == nullptr)
	{
		return;
	}
	printWordsHelper(node -> left, index); //print the left subtree

	// print the current node with its word, count and line number
	cout << index << ". " << node -> word << " " << node -> count << endl;
	index++;
	
	printWordsHelper(node -> right, index);
}

// singletons
unsigned int Wordlist::singletons() const
{
    return singletonsHelper(root);
}

unsigned int Wordlist::singletonsHelper(AVLTreeNode* node) const
{
	if (node == nullptr)
	{
		return 0;
	}

	// check if the current node is a singleton 
	unsigned int count;
	if (node -> count == 1)
	{
		count = 1;
	}
	else
	{
		count = 0;
	}

	// sum of singletons in the left and right subtree, plus the current node
	return count + singletonsHelper(node -> left) + singletonsHelper(node -> right);
}


// totalWords
unsigned int Wordlist::totalWords() const
{
    return totalWordsHelper(root);
}

unsigned int Wordlist::totalWordsHelper(AVLTreeNode* node) const
{
	if (node == nullptr)
	{
		return 0;
	}

	// return the sum of the current node count and the counts of its subtree
	return node -> count + totalWordsHelper(node -> left) + totalWordsHelper(node -> right);
}

// mostFrequent
string Wordlist::mostFrequent() const
{
    if (root == nullptr)
	{
		throw invalid_argument("Wordlist is empty");
	}

	string word;
	unsigned int maxCount = 0; 
	findMostFrequent(root, word, maxCount); 
	return word + " " + std::to_string(maxCount); //return the word and its count
}

void Wordlist::findMostFrequent(AVLTreeNode* node, string& word, unsigned int& maxCount) const
{
    if (node == nullptr)
    {
        return;
    }
    findMostFrequent(node -> left, word, maxCount); //search the left subtree

	// update the most frequent word if the current node has a higher count, or if count are equal and current word is smaller
    if (node -> count > maxCount || (node -> count == maxCount && node -> word < word))
    {
        maxCount = node -> count;
        word = node -> word;
    }
    
	findMostFrequent(node -> right, word, maxCount);
}

// differentWords
unsigned int Wordlist::differentWords() const
{
    return size;
}

bool Wordlist::remove(const string& word)
{
	bool removed = false; //track if a node was removed
	root = removeNode(root, word, removed);

	// if the tree is not empty after removal, make sure the parents root is nullptr
	if (root)
	{
		root -> parent = nullptr;
	}
	if (removed) // decrement the size of the tree
	{
		size--;
	}

	return removed;
}


AVLTreeNode* Wordlist::removeNode(AVLTreeNode* node, const string& word, bool& removed)
{
	if (node == nullptr)
	{
		return nullptr;
	}

	if (word < node -> word) // word is smaller, search left subtree
	{
		node -> left = removeNode(node -> left, word, removed);
		if (node -> left) // update parent pointer
		{
			node -> left -> parent = node;
		}
	}
	else if (word > node -> word) //word is larger, search right subtree
	{
		node -> right = removeNode(node -> right, word, removed);
		if (node -> right) //update parent pointer
		{
			node -> right -> parent = node;
		}
	}
	else //word matches the current node
	{
		removed = true;

		// node has at most one child
		if (node -> left == nullptr || node -> right == nullptr)
		{
			// select the non-null child
			AVLTreeNode* temp;
			if (node -> left != nullptr)
			{
				temp = node -> left;
			}
			else
			{
				temp = node -> right;
			}

			// no children, delete the node
			if (temp == nullptr)
			{
				temp = node;
				node = nullptr;
			}
			else // one child, replace the current node with the child
			{
				temp -> parent = node -> parent;
				*node = *temp;
			}
			delete temp;
		}
		else // if node has 2 children..
		{
			// find the in-order succesor
			AVLTreeNode* temp = findMin(node -> right);

			// replace the current node word and count with the succesors
			node -> word = temp -> word;
			node -> count = temp -> count;

			// remove the successor from the right subtree
			node -> right = removeNode(node -> right, temp -> word, removed);
			if (node -> right) // update pointer
			{
				node -> right -> parent = node;
			}
		}
	}

	if (node == nullptr)
	{
		return node;
	}

	updateHeight(node);

	return balanceAfterDeletion(node); // balance the tree and return the new root of the subtree
}



AVLTreeNode* Wordlist::findMin(AVLTreeNode* node)
{
	AVLTreeNode* current = node; //start at the given node
	while (current -> left != nullptr) // traverse left until the smallest node is found
	{
		current = current -> left;
	}
	return current;
}

AVLTreeNode* Wordlist::balanceAfterDeletion(AVLTreeNode* node)
{
	int balance = getBalance(node); // get balance factor

	// left heavy
	if (balance > 1)
	{
		//left-left case
		if (getBalance(node -> left) >= 0)
		{
			return rotateRight(node);
		}

		//left-right case
		else
		{
			node -> left = rotateLeft(node -> left);
			return rotateRight(node);
		}
	}

	else if (balance < -1)
	{
		//right-right case
		if (getBalance(node -> right) <= 0)
		{
			return rotateLeft(node);
		}

		// right-left case
		else
		{
			node -> right = rotateRight(node -> right);
			return rotateLeft(node);
		}
	}

	// node is balanced
	return node;
}

unsigned int Wordlist::getCount(const string& word) const
{
	AVLTreeNode* node = root; // start at root
	while (node != nullptr) // traverse the tree to search for the word
	{
		if (word < node -> word) //if smaller, go to left subtree
		{
			node = node -> left;
		}
		else if (word > node -> word) //if larger, go to the right subtree
		{
			node = node -> right;
		}
		else // found the word, return its count
		{
			return node -> count;
		}
	}

	return 0;
}

bool Wordlist::contains(const string& word) const
{
	return getCount(word) > 0; // true if the words count is greater than 0
}
