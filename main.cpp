#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdlib.h>

#include <ctime>
#include <chrono>
#include <windows.h>

using namespace std;

struct Node {
	vector<vector<int>> puzzle;
	int f;
	int h;
	int g;
	Node* up;
	Node* down;
	Node* left;
	Node* right;
	Node* before;
};

Node* MakeNode(vector<vector<int>> puzzle, int f, int h, int g) {
	Node* node = new Node();
	node->puzzle = puzzle;
	node->f = f;
	node->h = h;
	node->g = g;
	node->up = 0;
	node->down = 0;
	node->left = 0;
	node->right = 0;
	node->before = 0;
	return node;
}

//Global variables
int row, col = 0;
int rowNum, colNum = 0;
int numNodes;
int input_alg;
bool goal = false;
vector<Node*> allNodesVector;
vector<Node*> nodeVector;
vector<Node*> trace;

// cout functions to user
void startUp() {
	cout << "Welcome to the Eight Puzzle Solver\n";
	cout << "Please enter (1) to use the default puzzle, or (2) to use your own puzzle:\n\n";
}

void selectAlg() {
	cout << "Please choose the algorithm:\n";
	cout << "(1) for the Uniform Cost Search\n" <<
			"(2) for the Misplaced Tile heuristic\n" <<
			"(3) for the Manhattan Distance heuristic\n\n";
}

// default puzzle
vector<vector<int>> defaultPuzzle() {
	// 1 2 3
	// 0 4 5
	// 7 8 6
	vector<vector<int>> puzzle;
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v2.push_back(0);
	v2.push_back(4);
	v2.push_back(5);
	v3.push_back(7);
	v3.push_back(8);
	v3.push_back(6);
	puzzle.push_back(v1);
	puzzle.push_back(v2);
	puzzle.push_back(v3);
	return puzzle;
}

// puzzle entered by user
vector<vector<int>> userPuzzle() {
	vector<vector<int>> puzzle;
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	int num1, num2, num3 = 0;

	cout << "Enter the first 3 numbers for the first row" 
		 << "\n(press enter after each number entered): ";
	cin >> num1;
	cin >> num2;
	cin >> num3;
	v1.push_back(num1);
	v1.push_back(num2);
	v1.push_back(num3);

	cout << "Enter the first 3 numbers for the second row"
		<< "\n(press enter after each number entered): ";
	cin >> num1;
	cin >> num2;
	cin >> num3;
	v2.push_back(num1);
	v2.push_back(num2);
	v2.push_back(num3);

	cout << "Enter the first 3 numbers for the third row"
		<< "\n(press enter after each number entered): ";
	cin >> num1;
	cin >> num2;
	cin >> num3;

	cout << "\n\n";
	v3.push_back(num1);
	v3.push_back(num2);
	v3.push_back(num3);

	puzzle.push_back(v1);
	puzzle.push_back(v2);
	puzzle.push_back(v3);
	return puzzle;
}

// prints out a puzzle
void printPuzzle(vector<vector<int>> puzzle_print) {
	for (int i = 0; i < puzzle_print.size(); i++) {
		for (int j = 0; j < puzzle_print[i].size(); j++) {
			cout << puzzle_print[i][j];
		}
		cout << "\n";
	}
}

// goal state of the puzzle
vector<vector<int>> goalState() {
	// 1 2 3
	// 4 5 6
	// 7 8 0
	vector<vector<int>> goalStatePuzzle;
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	v3.push_back(7);
	v3.push_back(8);
	v3.push_back(0);
	goalStatePuzzle.push_back(v1);
	goalStatePuzzle.push_back(v2);
	goalStatePuzzle.push_back(v3);
	return goalStatePuzzle;
}

// will find the 0 inside of the puzzle (row and col used in move functions below)
void findZero(vector<vector<int>> puzzle) {
	for (int i = 0; i < puzzle.size(); i++) {
		for (int j = 0; j < puzzle[i].size(); j++) {
			if (puzzle[i][j] == 0) {
				// 0 is at row, col
				row = i;
				col = j;
			}
		}
	}
}

// finds where a number is in the puzzle
void findNum(vector<vector<int>> puzzle, int num) {
	for (int i = 0; i < puzzle.size(); i++) {
		for (int j = 0; j < puzzle[i].size(); j++) {
			if (puzzle[i][j] == num) {
				// the num is at rowNum, colNum
				rowNum = i;
				colNum = j;
			}
		}
	}
}

// returns true if the 0 in the puzzle can be moved
bool checkUp(vector<vector<int>> puzzle) {
	for (int i = 0; i < puzzle.size(); i++) {
		if (puzzle[0][i] == 0) {
			return false;
		}
	}
	return true;
}

// returns true if the 0 in the puzzle can be moved
bool checkDown(vector<vector<int>> puzzle) {
	for (int i = 0; i < puzzle.size(); i++) {
		if (puzzle[puzzle.size() - 1][i] == 0) {
			return false;
		}
	}
	return true;
}

// returns true if the 0 in the puzzle can be moved
bool checkLeft(vector<vector<int>> puzzle) {
	for (int i = 0; i < puzzle.size(); i++) {
		if (puzzle[i][0] == 0) {
			return false;
		}
	}
	return true;
}

// returns true if the 0 in the puzzle can be moved
bool checkRight(vector<vector<int>> puzzle) {
	for (int i = 0; i < puzzle.size(); i++) {
		if (puzzle[i][puzzle.size() - 1] == 0) {
			return false;
		}
	}
	return true;
}

// will move the 0 in the puzzle up
vector<vector<int>> moveUp(vector<vector<int>> puzzle) {
	findZero(puzzle);
	swap(puzzle[row][col], puzzle[row - 1][col]);
	return puzzle;
}

// will move the 0 in the puzzle down
vector<vector<int>> moveDown(vector<vector<int>> puzzle) {
	findZero(puzzle);
	swap(puzzle[row][col], puzzle[row + 1][col]);
	return puzzle;
}

// will move the 0 in the puzzle left
vector<vector<int>> moveLeft(vector<vector<int>> puzzle) {
	findZero(puzzle);
	swap(puzzle[row][col], puzzle[row][col - 1]);
	return puzzle;
}

// will move the 0 in the puzzle right
vector<vector<int>> moveRight(vector<vector<int>> puzzle) {
	findZero(puzzle);
	swap(puzzle[row][col], puzzle[row][col + 1]);
	return puzzle;
}

// calculates tile heuristic
int calcH_Tile(vector<vector<int>> puzzle) {
	int num = 0;
	int correct = 1;
	for (int i = 0; i < puzzle.size(); i++) {
		for (int j = 0; j < puzzle[i].size(); j++) {
			if (puzzle[i][j] != correct) {
				num++;
			}
			correct++;
			if (correct == 9) {
				break;
			}
		}
	}
	return num;
}

// calculates manhattan distance
int calcH_Man(vector<vector<int>> puzzle) {
	int total = 0;
	// calculates where 1 is vs should be
	findNum(puzzle, 1);
	total += rowNum - 0;
	total += colNum - 0;

	findNum(puzzle, 2);
	total += rowNum - 0;
	total += abs(colNum - 1);
	
	findNum(puzzle, 3);
	total += rowNum - 0;
	total += abs(colNum - 2);

	findNum(puzzle, 4);
	total += abs(rowNum - 1);
	total += abs(colNum - 0);

	findNum(puzzle, 5);
	total += abs(rowNum - 1);
	total += abs(colNum - 1);

	findNum(puzzle, 6);
	total += abs(rowNum - 1);
	total += abs(colNum - 2);

	findNum(puzzle, 7);
	total += abs(rowNum - 2);
	total += abs(colNum - 0);

	findNum(puzzle, 8);
	total += abs(rowNum - 2);
	total += abs(colNum - 1);

	return total;
}

// calculates the nodes-> h value depending on the input_alg
int calcH(vector<vector<int>> puzzle) {
	if (input_alg == 1) {
		return 0;
	}
	else if (input_alg == 2) {
		return calcH_Tile(puzzle);
	}
	else {
		return calcH_Man(puzzle);
	}
}

// will sort the children in the nodeVector based on their f value
// smallest f towards the front
void sortChildren(Node* current) {
	for (int i = 0; i < nodeVector.size()+1; i++) {
		if (current->f < nodeVector[i]->f) {
			nodeVector.insert(nodeVector.begin() + i, current);
			break;
		}
		if (i == nodeVector.size() - 1) {
			nodeVector.push_back(current);
			break;
		}
	}
}

// checks to see if the puzzle has been used before in the allNodesVector
// returns true if the puzzle has not been used before
bool checkVectors(vector<vector<int>> puzzle) {
	for (int i = 0; i < allNodesVector.size(); i++) {
		if (allNodesVector[i]->puzzle == puzzle) {
			return true;
		}
	}
	return false;
}

// will make the children up, down, left, right if possible
void makeChildren(Node* root_puzzle){
	// ***** up *****
	// check if the puzzle can move up
	if (checkUp(root_puzzle->puzzle)) {
		// create child and make the child's puzzle move the 0 up
		Node* child = MakeNode(root_puzzle->puzzle, root_puzzle->f, root_puzzle->h, root_puzzle->g);
		child->puzzle = moveUp(child->puzzle);

		// checks the child's puzzle to see if it has been used before, if not run this code
		if (!checkVectors(child->puzzle)) {
			// set the root_puzzle's up to be child and calculate child's f, g, h
			// push the child into the allNodesVector and sort in nodeVector
			// sort the children based on the f value, increase number of nodes
			root_puzzle->up = child;
			child->before = root_puzzle;
			child->g++;
			child->h = calcH(child->puzzle);
			child->f = child->h + child->g;
			allNodesVector.push_back(child);
			sortChildren(child);
			numNodes++;
		}
		// puzzle has already been used, so set the root_puzzle's up to be NULL
		else {
			root_puzzle->up = NULL;
		}
	}
	else {
		root_puzzle->up = NULL;
	}

	// ***** down *****
	if (checkDown(root_puzzle->puzzle)) {
		Node* child = MakeNode(root_puzzle->puzzle, root_puzzle->f, root_puzzle->h, root_puzzle->g);
		child->puzzle = moveDown(child->puzzle);

		if (!checkVectors(child->puzzle)) {
			root_puzzle->down = child;
			child->before = root_puzzle;
			child->g++;
			child->h = calcH(child->puzzle);
			child->f = child->h + child->g;
			allNodesVector.push_back(child);
			sortChildren(child);
			numNodes++;
		}
		else {
			root_puzzle->down = NULL;
		}
	}
	else {
		root_puzzle->down = NULL;
	}

	// ***** left *****
	if (checkLeft(root_puzzle->puzzle)) {
		Node* child = MakeNode(root_puzzle->puzzle, root_puzzle->f, root_puzzle->h, root_puzzle->g);
		child->puzzle = moveLeft(child->puzzle);
		
		if (!checkVectors(child->puzzle)) {
			root_puzzle->left = child;
			child->before = root_puzzle;
			child->g++;
			child->h = calcH(child->puzzle);
			child->f = child->h + child->g;
			allNodesVector.push_back(child);
			sortChildren(child);
			numNodes++;
		}
		else {
			root_puzzle->left = NULL;
		}
	}
	else {
		root_puzzle->left = NULL;
	}

	// ***** right *****
	if (checkRight(root_puzzle->puzzle)) {
		Node* child = MakeNode(root_puzzle->puzzle, root_puzzle->f, root_puzzle->h, root_puzzle->g);
		child->puzzle = moveRight(child->puzzle);

		if (!checkVectors(child->puzzle)) {
			root_puzzle->right = child;
			child->before = root_puzzle;
			child->g++;
			child->h = calcH(child->puzzle);
			child->f = child->h + child->g;
			allNodesVector.push_back(child);
			sortChildren(child);
			numNodes++;
		}
		else {
			root_puzzle->right = NULL;
		}
	}
	else {
		root_puzzle->right = NULL;
	}
}

// the algorithm function
void algorithm(Node* current) {
	// check if goal state
	if (current->puzzle == goalState()) {
		goal = true;
		cout << "\nThe solution is: \n";
		printPuzzle(current->puzzle);
		cout << "The solution depth was: " << current->g << endl;
		cout << "Number of nodes expanded: " << numNodes << endl << endl;
		return;
	}
	// check if vector is empty
	if (nodeVector.empty()) {
		cout << "failure";
		cout << endl;
		return;
	}
	// expand or create children and then remove from the nodeVector
	makeChildren(current);
	nodeVector.erase(nodeVector.begin());
}


int main() {
	int input_type;
	vector<vector<int>> puzzle;

	// create the root node
	Node* root_node = MakeNode(puzzle, 0, 0, 0);
	root_node->before = NULL;

	// add root node into the both vectors
	allNodesVector.push_back(root_node);
	nodeVector.push_back(root_node);

	// choose puzzle (default or create one)
	startUp();
	cin >> input_type;
	if (input_type == 1) {
		cout << "Here is the puzzle:\n";
		// get the default puzzle and store in root_puzzle
		root_node->puzzle = defaultPuzzle();
		// print the puzzle to the user
		printPuzzle(root_node->puzzle);
		cout << endl;
	}
	else {
		cout << "Please enter in a puzzle:\n\n";
		root_node->puzzle = userPuzzle();
		cout << "Here is the puzzle:\n";
		printPuzzle(root_node->puzzle);
		cout << endl;
	}

	// select the algorithm
	selectAlg();
	cin >> input_alg;
	if (input_alg == 1) {
		cout << "You picked: Uniform Cost Search.\n\n";
	}
	else if (input_alg == 2){
		cout << "You picked: Misplaced Tile heuristic.\n\n";
	}
	else {
		cout << "You picked: Manhattan Distance heuristic.\n\n";
	}

	// update roots h and f based on the input
	root_node->h = calcH(root_node->puzzle);
	root_node->f = root_node->h + root_node->g;

	// timer
	time_t now = time(0);
	int timer = now + 900;

	// run algorithm until a solution or failure
	while (!goal) {
		now = time(0); // timer
		if (now >= timer) {
			cout << "15 minutes is up.\n";
			break;
		}
		algorithm(root_node);
		// need to update the root_node to be the front of nodeVector and call algorithm again
		root_node = nodeVector[0];
	}

	if (goal) {
		// move the node into the trace vector for traceback
		while (root_node->before != NULL) {
			trace.push_back(root_node);
			root_node = root_node->before;
		}
		trace.push_back(root_node);

		// print out traceback
		for (int i = trace.size() - 1; i >= 0; i--) {
			cout << "The best state to expand with a h(n) = "
				<< trace[i]->h << ", and an g(n) = "
				<< trace[i]->g << " is...\n";
			printPuzzle(trace[i]->puzzle);
			cout << endl;
		}
	}
}
