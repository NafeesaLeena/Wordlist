//Name: Nafeesa Leena
//Date: November. 22, 2024

#pragma once
#include <string>
#include <iostream>
using std::string;
using std::cin;
using std::cout;
using std::endl;

class AVLTreeNode {
public:
	// Attributes 
	string word; // word - node's string data
	unsigned int count; // count - number of times the word appears
	AVLTreeNode* left; // left - pointer to left child
	AVLTreeNode* right; // right - pointer to right child
	AVLTreeNode* parent; // parent - pointer to parent
	unsigned int height; // height - node's height
	

	// Constructors
	AVLTreeNode(const string& w)
	{
		word = w;
		count = 1;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
		height = 0;
	}
};

// Wordlist class
class Wordlist
{
private:
	// Class attributes go here
	// Your class MUST have a root node named root (below)
	AVLTreeNode* root; // DO NOT REMOVE
	unsigned int size; // Number of nodes (distinct words)

	// Helper function to recursively delete the nodes
    void destroyTree(AVLTreeNode* node);

	// Helper function to recusively copy the nodes
	AVLTreeNode* copyNode(AVLTreeNode* parent, AVLTreeNode* otherNode);

	// Insertion Helper functions
	AVLTreeNode* insertNode(AVLTreeNode* node, const string& word, AVLTreeNode* parent);
	AVLTreeNode* balanceNode(AVLTreeNode* node);
	void updateHeight(AVLTreeNode* node);
	int getBalance(AVLTreeNode *node);

	// Removal helper methods
    AVLTreeNode* removeNode(AVLTreeNode* node, const string& word, bool& removed);
    AVLTreeNode* findMin(AVLTreeNode* node);
    AVLTreeNode* balanceAfterDeletion(AVLTreeNode* node);
	
	// Rotations
	AVLTreeNode* rotateLeft(AVLTreeNode* x);
	AVLTreeNode* rotateRight(AVLTreeNode* y);

	// Helper functions for statistics print
	unsigned int totalWordsHelper(AVLTreeNode* node) const;
	void printWordsHelper(AVLTreeNode* node, unsigned int& index) const;
	unsigned int singletonsHelper(AVLTreeNode* node) const;
	void findMostFrequent(AVLTreeNode* node, string& word, unsigned int& maxCount) const;


public:
	// public methods go here
	Wordlist(); // Defualt constructor
	Wordlist(const string& filename); // File constructor
	Wordlist(const Wordlist& other); // Copy constructor 

	// Destructor
	~Wordlist();

	// Overloaded assignment operator
	Wordlist& operator = (const Wordlist& other);

	// Mutators 
	void insert(const string& word);
	bool remove(const string& word);

	// Accessors
	unsigned int getCount(const string& word) const;
	bool contains(const string& word) const;
	unsigned int differentWords() const;
	unsigned int totalWords() const;
	string mostFrequent() const;
	unsigned int singletons() const;

	// Prints useful statistics about the word list
	void printStatistics() const;

	// Prints the words in ascending order with the counts
	void printWords() const;


	// Returns the root of the AVL tree
	AVLTreeNode* getRoot() const { return root; }; // DO NOT REMOVE
};